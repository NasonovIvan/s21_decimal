#include "s21_decimal.h"

/*
    function s21_mod
    Функция деления с остатком двух чисел типа decimal
*/
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // обнуление result
  memset(&(result->bits), 0, sizeof(result->bits));
  // получение десятичных степеней 10 из чисел decimal
  int exp_1 = exp_decimal_bin2dec(value_1);
  int exp_2 = exp_decimal_bin2dec(value_2);
  int error = mod_main(value_1, exp_1, value_2, exp_2, result);
  return error;
}

/*
    function mod_main
    Основная функция деления с остатком двух чисел типа decimal
*/
int mod_main(s21_decimal value_1, int exp_1, s21_decimal value_2, int exp_2,
             s21_decimal *result) {
  // int error = 0;
  // индекс в цикле сложения и еще он нужен для записи числа в decimal
  int index = 0;
  //  индекс указывающий какого знака поданные числа value_1 и value_2
  //  соответственно
  //  1 : + и +
  //  2 : - и -
  //  3 : + и -
  //  4 : - и +
  int sign_index = 0;
  // если первое число меньше второго
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
  int value_2_arr_help[NUM_255];
  int result_arr[NUM_255];
  // инициализация
  for (int i = 0; i < NUM_255; i++) {
    result_arr[i] = 2;
    value_1_arr[i] = 2;
    value_2_arr[i] = 2;
    value_2_arr_help[i] = 2;
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
  // случай когда ноль поделили на число любого знака и тогда ответ в любом
  // случае должен быть 0. Если в value_1_arr нет битов = 1, значит ответ 0. За
  // это будет отвечать переменная check_equal. Также сделаем проверку на случай
  // деления на ноль: если в value_2_arr нет битов = 1, значит ошибка.
  int check_equal = 0;
  for (int i = 0; i < NUM_255; i++) {
    if (value_2_arr[i] == 1) {
      check_equal = 1;
      break;
    }
  }
  if (!check_equal) return 3;
  check_equal = 0;
  for (int i = 0; i < NUM_255; i++) {
    if (value_1_arr[i] == 1) {
      check_equal = 1;
      break;
    }
  }
  if (!check_equal) return 0;
  // находим наибольшую степень - она и будет экспонентной в остатке
  int exp = 0;
  if (exp_1 >= exp_2) {
    exp = exp_1;
  } else {
    exp = exp_2;
  }
  // какая степень меньше, такую будем домножать на 10 для "выравнивания"
  // порядка степеней в числах
  // если error = 123 значит переполнение мантисы и в зависимости от знаков двух
  // чисел возвращаем ошибку 1 или 2
  add_mul_1010(value_1_arr, value_2_arr, &exp_1, &exp_2);
  // переменные для подсчета и контроля количества битов. Это необходимо для
  // отлавливания переполнения и банковского округления
  int count_bit_1 = 0;
  int count_bit_2 = 0;
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
  // если первое число меньше второго, то ответом будет первое число
  int greater = -1;
  // если greater = 1 то value_1 > value_2
  // если greater = 0 то либо они равны либо value_1 < value_2
  greater = s21_is_greater_num(value_1, value_2, greater);
  if (greater == 0) {
    result->bits[0] = value_1.bits[0];
    result->bits[1] = value_1.bits[1];
    result->bits[2] = value_1.bits[2];
    result->bits[3] = value_1.bits[3];
    return 0;
  }
  // обнуление последней 2 в value_2_arr перед началом числа
  for (int i = 254; i >= 0; i--) {
    if (value_2_arr[i] == 1) {
      value_2_arr[i + 1] = 0;
      break;
    }
  }
  // записываем value_2_arr в другом порядке
  for (int i = count_bit_2, j = 0; i >= 0; i--, j++) {
    value_2_arr_help[j] = value_2_arr[i];
  }
  // массив result_arr_copy где будем содержать результат деления
  // int result_arr_copy[NUM_255];
  // for (int i = 0; i < NUM_255; i++) {
  //   result_arr_copy[i] = 2;
  // }
  //  count_bit = количество битов в result_arr_copy
  // массив в котором будет содержаться промежуточное число и из которого будем
  // вычитать value_2 + инициализация
  const int size_array = count_bit_2 + 1;
  int *arr_help = (int *)calloc(size_array, sizeof(int));
  int *arr_help_copy = (int *)calloc(size_array, sizeof(int));
  for (int i = 0; i < count_bit_2 + 1; i++) {
    arr_help[i] = 0;
    arr_help_copy[i] = 0;
  }
  // индекс начала value_1_arr (первый элемент) тк value_1_arr / value_2_arr
  int index_value_1 = count_bit_1 - 1;
  // индекс для записи результата деления в массив result_arr_copy
  // int index_result = 0;
  // индекс первого деления нужен для сдвига и записи 0 в result_arr_copy
  int start = 1;
  // заполняем arr_help первыми count_bit_2 цифрами value_2_arr индексами с 1 по
  // (count_bit_2 + 1)
  for (int i = 1; i < count_bit_2 + 1; i++) {
    arr_help[i] = value_1_arr[index_value_1];
    index_value_1--;
  }
  // цикл деления
  do {
    //  arr_help необходимо сравнить с value_2_arr. Если число меньше, то должны
    //  добавить еще один элемент из value_1_arr в arr_help
    while (div_less_number(arr_help, value_2_arr_help, count_bit_2) &&
           index_value_1 >= 0) {
      // делаем сдвиг arr_help влево на 1 бит + в последний эелемент ставим еще
      // одно число из value_1_arr
      for (int i = 0; i < count_bit_2; i++) {
        arr_help[i] = arr_help[i + 1];
      }
      arr_help[count_bit_2] = value_1_arr[index_value_1];
      index_value_1--;
      // пишем 0 в result_arr_copy если добавили разряд, а число все еще меньше
      //  value_2_arr. Здесь же учитываем индекс первого деления
      // if (!start && div_less_number(arr_help, value_2_arr_help, count_bit_2))
      // {
      //   result_arr_copy[index_result] = 0;
      //   index_result++;
      // }
    }
    // обнуление индекса первого деления
    start = 0;
    // если индекс ушел в отрицательное значение в прошлом цикле то завершаем,
    // результат, который в arr_help, и есть целая часть искомого остатка от
    // деления
    if (index_value_1 < 0 &&
        div_less_number(arr_help, value_2_arr_help, count_bit_2))
      break;
    // процесс вычитания value_2_arr из arr_help
    // запись в arr_help_copy полученного результата
    for (int i = count_bit_2; i >= 0; i--) {
      if (arr_help[i] == 0 && value_2_arr_help[i] == 0) {
        arr_help_copy[i] = 0;
      } else if (arr_help[i] == 1 && value_2_arr_help[i] == 0) {
        arr_help_copy[i] = 1;
      } else if (arr_help[i] == 0 && value_2_arr_help[i] == 1) {
        // цикл для реализации ситуации когда (10000 - 1010) и 1 нужно брать не
        // из соседнего разряда
        for (int j = i - 1; j >= 0; j--) {
          if (arr_help[j] == 0) {
            arr_help[j] = 1;
          } else if (arr_help[j] == 1) {
            arr_help[j] = 0;
            break;
          }
        }
        arr_help_copy[i] = 1;
      } else if (arr_help[i] == 1 && value_2_arr_help[i] == 1) {
        arr_help_copy[i] = 0;
      }
    }
    // когда вычитание произошло, то нужно дописать в result_arr_copy 1
    // result_arr_copy[index_result] = 1;
    // index_result++;
    // обновление arr_help
    for (int i = 0; i < count_bit_2 + 1; i++) {
      arr_help[i] = arr_help_copy[i];
    }
  } while (index_value_1 >= 0);
  // запись результата остатка из arr_help в result_arr
  // поиск начала результата в arr_help
  for (int i = 0; i < count_bit_2 + 1; i++) {
    if (arr_help[i] == 1) {
      index = i;
      break;
    }
  }
  // используем start как индекс для записи result_arr
  start = count_bit_2 - index;
  // запись из arr_help в result_arr
  // инициализация (по сути обнуление) result_arr чтобы вписать туда новое число
  for (int i = 0; i < NUM_255; i++) {
    result_arr[i] = 2;
  }
  while (index < count_bit_2 + 1) {
    result_arr[start] = arr_help[index];
    index++;
    start--;
  }
  // считаем какое количество битов в result_arr
  index_1 = 0;
  int count_bit = 0;
  for (int i = 254; i >= 0; i--) {
    if (result_arr[i] == 1) index_1 = 1;
    if (index_1) {
      count_bit++;
    }
  }
  // вторая проверка переполняемости decimal. Добавлена для случая когда
  // складываются 2 больших числа, не близких к максимальному, но дающие
  // результат, больше максимального

  //  банковское округление
  // делим результат на 10 пока он не поместиться в децимал
  // занулим индекс начала result_arr (для этого будем использовать старый
  //  index_1) index_2 используем для понимания сколько раз нужно поделить на
  //  1010, чтобы выделить целую часть числа
  index_2 = exp;
  int exp_copy = exp;
  int error = 0;
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
      add_div_10(result_arr, &exp_copy, &count_bit);
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
  // еще раз проверяем на переполнение
  if (error == 123 && (sign_index == 1 || sign_index == 3)) {
    return 1;
  } else if (error == 123 && (sign_index == 2 || sign_index == 4)) {
    return 2;
  }
  index = 95;
  write_float_decimal_exp_more(result_arr, result, index, 2);
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
  if (sign_index == 2 || sign_index == 4) {
    set_1_bit(&(result->bits[3]), 31);
  }
  free(arr_help);
  free(arr_help_copy);
  return 0;
}
