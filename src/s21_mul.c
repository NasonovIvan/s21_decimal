#include "s21_decimal.h"

/*
    function s21_mul
    Функция умножения двух чисел типа decimal и запись в result
*/

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // обнуление result
  memset(&(result->bits), 0, sizeof(result->bits));
  // получение десятичных степеней 10 из чисел decimal
  int exp_1 = exp_decimal_bin2dec(value_1);
  int exp_2 = exp_decimal_bin2dec(value_2);
  int error = mul_main(value_1, exp_1, value_2, exp_2, result);
  return error;
}

/*
    function mul_main
    Основная функция умножения двух чисел типа decimal
*/
int mul_main(s21_decimal value_1, int exp_1, s21_decimal value_2, int exp_2,
             s21_decimal *result) {
  int error = 0;
  // для запоминания доп разрядов при сложении
  int memory = 0;
  // индекс в цикле сложения и еще он нужен для записи числа в decimal
  int index = 0;
  //  индекс указывающий какого знака поданные числа value_1 и value_2
  //  соответственно
  //  1 : + и +
  //  2 : - и -
  //  3 : + и -
  //  4 : - и +
  int sign_index = 0;
  // проверка знаков чисел и выставление sign_index
  if (test_bit(value_1.bits[3], 31) == 0 &&
      test_bit(value_2.bits[3], 31) == 0) {
    sign_index = 1;
  } else if (test_bit(value_1.bits[3], 31) && test_bit(value_2.bits[3], 31)) {
    sign_index = 2;
  } else if (test_bit(value_1.bits[3], 31) == 0 &&
             test_bit(value_2.bits[3], 31)) {
    sign_index = 3;
  } else if (test_bit(value_1.bits[3], 31) &&
             test_bit(value_2.bits[3], 31) == 0) {
    sign_index = 4;
  }
  // массивы с битами
  int value_1_arr[NUM_255];
  int value_2_arr[NUM_255];
  int help_arr_1[NUM_255];
  int help_arr_2[NUM_255];
  int result_arr[NUM_255];
  // инициализация
  for (int i = 0; i < NUM_255; i++) {
    result_arr[i] = 0;
    value_1_arr[i] = 2;
    value_2_arr[i] = 2;
    help_arr_1[i] = 0;
    help_arr_2[i] = 0;
  }
  for (int i = 0; i < 96; i++) {
    if (test_bit(value_1.bits[i / 32], i % 32)) {
      value_1_arr[i] = 1;
    } else {
      value_1_arr[i] = 0;
    }
    if (test_bit(value_2.bits[i / 32], i % 32)) {
      value_2_arr[i] = 1;
    } else {
      value_2_arr[i] = 0;
    }
  }
  // переменные для подсчета и контроля количества битов. Это необходимо для
  // отлавливания переполнения и банковского округления
  int count_bit_1 = 0;
  int count_bit_2 = 0;
  int count_bit = 0;
  int index_1 = 0;
  int index_2 = 0;
  for (int i = 254; i >= 0; i--) {
    if (value_1_arr[i] == 1) index_1 = 1;
    if (value_2_arr[i] == 1) index_2 = 1;
    if (index_1) {
      count_bit_1++;
    }
    if (index_2) {
      count_bit_2++;
    }
  }
  for (int i = 0; i < NUM_255; i++) {
    if (value_2_arr[i] == 2) value_2_arr[i] = 0;
    if (value_1_arr[i] == 2) value_1_arr[i] = 0;
  }
  // счетчик сдвига разрядов при сложении в столбик
  int shift = 0;
  // производим умножение в столбик (через сложение)
  // первое число будем складывать столько раз, сколько
  // единичных битов во 2 числе
  // цикл по наименьшему числу
  for (; index < count_bit_2; index++) {
    // цикл по наибольшему числу (которое складываем само с собой)
    // пропуск первого шага тк не с чем складывать
    if (index == 0) {
      if (value_2_arr[index] == 0) {
        copy_zero(help_arr_1);
      } else if (value_2_arr[index] == 1) {
        copy_value(help_arr_1, value_1_arr);
      }
      shift++;
      continue;
    } else {
      if (value_2_arr[index] == 1) {
        // в спомогательный массив копируем значение value_1_arr с учетом
        // сдвига (на месте сдвига уже стоят нули, тк вспомогательные массивы
        // сразу инициализируются нулями и обнуляются каждый цикл)
        // NUM_255 - shift - 1 тк из-за сдвига мы можем попасть в память других
        // массивов, поэтому нельзя выходить за пределы 254
        for (int i = 0; i < NUM_255 - shift - 1; i++) {
          help_arr_2[i + shift] = value_1_arr[i];
        }
        // сложение help_arr_1 и help_arr_2 -> запись в result
        for (int i = 0; i < NUM_255; i++) {
          if (help_arr_1[i] == 1 && help_arr_2[i] == 1) {
            // нет в памяти доп разряда
            if (memory == 0) {
              result_arr[i] = 0;
              memory = 1;
            } else {
              result_arr[i] = 1;
            }
          } else if ((help_arr_1[i] == 0 && help_arr_2[i] == 1) ||
                     (help_arr_1[i] == 1 && help_arr_2[i] == 0)) {
            // нет в памяти доп разряда
            if (memory == 0) {
              result_arr[i] = 1;
            } else {
              result_arr[i] = 0;
            }
          } else if (help_arr_1[i] == 0 && help_arr_2[i] == 0) {
            // нет в памяти доп разряда
            if (memory == 0) {
              result_arr[i] = 0;
            } else {
              result_arr[i] = 1;
              memory = 0;
            }
          }
        }
        // результат сложения копируется в help_arr_1 для следующего шага цикла
        // (если мы не складываем последний раз)
        copy_value(help_arr_1, result_arr);
        copy_zero(help_arr_2);
        copy_zero(result_arr);
        shift++;
      } else if (value_2_arr[index] == 0) {
        shift++;
      }
    }
  }
  copy_value(result_arr, help_arr_1);
  // считаем какое количество битов в result_arr
  index_1 = 0;
  for (int i = 254; i >= 0; i--) {
    if (result_arr[i] == 1) index_1 = 1;
    if (index_1) {
      count_bit++;
    }
  }
  // банковское округление
  // делим результат на 10 пока он не поместиться в децимал
  // занулим индекс начала result_arr (для этого будем использовать старый
  // index_1) index_2 используем для понимания сколько раз нужно поделить на
  // 1010, чтобы выделить целую часть числа
  index_2 = exp_1 + exp_2;
  int exp = exp_1 + exp_2;
  if (count_bit > 96) {
    while (count_bit > 96) {
      index_1 = 0;
      // если выделили целую часть числа и оно больше 96 бит, значит
      // переполнение
      if (!index_2) {
        count_bit = 0;
        for (int i = 254; i >= 0; i--) {
          if (result_arr[i] == 1) index_1 = 1;
          if (index_1) {
            count_bit++;
          }
        }
        if (count_bit > 96) {
          error = 123;
          break;
        }
      }
      add_div_10(result_arr, &exp, &count_bit);
      // обновление count_bit
      count_bit = 0;
      for (int i = 254; i >= 0; i--) {
        if (result_arr[i] == 1) index_1 = 1;
        if (index_1) {
          count_bit++;
        }
      }
      index_2--;
    }
  }
  if (error == 123 && (sign_index == 1 || sign_index == 2)) {
    return 1;
  } else if (error == 123 && (sign_index == 3 || sign_index == 4)) {
    return 2;
  }
  // printf("result_arr:\n");
  // for (int i = 80; i >= 0; i--) {
  //   printf("%d", result_arr[i]);
  // }
  // printf("\n");
  // index = 95;
  write_float_decimal_exp_more(result_arr, result, 95, 2);
  // создаем массив в котором будет двоичная запись count_10
  int count_10_bit[8] = {0};
  count10_to_bin(&exp, count_10_bit);
  // запись в decimal c 16 до 23 бита в формате
  // младшие биты ближе к 16 а старшие ближе к 23
  // запись похожа на запись типа int в decimal
  for (int i = 0, j = 16; i < 8; i++, j++) {
    if (count_10_bit[i]) {
      set_1_bit(&(result->bits[3]), j);
    } else {
      set_0_bit(&(result->bits[3]), j);
    }
  }
  // ставим 31 бит отвечающий за знак в 1 если sign_index == 3 || sign_index ==
  // 4 (то есть умножили отрицательное и положительное число)
  if (sign_index == 3 || sign_index == 4) {
    set_1_bit(&(result->bits[3]), 31);
  }
  // случай когда умножили число любого знака на ноль и тогда ставить минус не
  // нужно. Если в result_arr нет битов = 1, значит ответ 0. За это будет
  // отвечать переменная check_equal
  int check_equal = 0;
  if (sign_index == 3 || sign_index == 4) {
    for (int i = 0; i < NUM_255; i++) {
      if (result_arr[i] == 1) {
        check_equal = 1;
        break;
      }
    }
    if (!check_equal) set_0_bit(&(result->bits[3]), 31);
  }
  // printf("result_arr:\n");
  // for (int i = 98; i >= 0; i--) {
  //   printf("%d", result_arr[i]);
  // }
  // printf("\n");
  return 0;
}

/*
    function copy_zero
    Функция копирования нулей в спомогательный массив
    необходима в функции умножения
*/
void copy_zero(int *help_arr) {
  for (int i = 0; i < NUM_255; i++) {
    help_arr[i] = 0;
  }
}

/*
    function copy_value
    Функция копирования значений числа в спомогательный массив
    необходима в функции умножения
*/
void copy_value(int *help_arr, int *value) {
  for (int i = 0; i < NUM_255; i++) {
    help_arr[i] = value[i];
  }
}
