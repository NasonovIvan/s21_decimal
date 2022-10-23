#include "s21_decimal.h"

/*
    function s21_div
    Функция деления двух чисел типа decimal
*/
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // обнуление result
  memset(&(result->bits), 0, sizeof(result->bits));
  // получение десятичных степеней 10 из чисел decimal
  int exp_1 = exp_decimal_bin2dec(value_1);
  int exp_2 = exp_decimal_bin2dec(value_2);
  int error = div_main(value_1, exp_1, value_2, exp_2, result);
  return error;
}

/*
    function div_main
    Основная функция деления двух чисел типа decimal
*/
int div_main(s21_decimal value_1, int exp_1, s21_decimal value_2, int exp_2,
             s21_decimal *result) {
  int error = 0;
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
  int check_equal_1 = 0;
  int check_equal_2 = 0;
  for (int i = 0; i < NUM_255; i++) {
    if (value_1_arr[i] == 1) {
      check_equal_1 = 1;
      break;
    }
  }
  for (int i = 0; i < NUM_255; i++) {
    if (value_2_arr[i] == 1) {
      check_equal_2 = 1;
      break;
    }
  }
  // проверка деления на 0
  if (!check_equal_2) {
    return 3;
  } else if (!check_equal_1) {
    return 0;
  }
  // какая степень меньше, такую будем домножать на 10 для "выравнивания"
  // порядка степеней в числах
  // если error = 123 значит переполнение мантисы и в зависимости от знаков двух
  // чисел возвращаем ошибку 1 или 2
  error = add_mul_1010(value_1_arr, value_2_arr, &exp_1, &exp_2);
  error = 0;
  // проверка на конечный ноль когда можно просто дописать нули в число
  int zero_end_arr = 0;
  int zero_end_val = 0;
  // экспонента для результата и определения остатка от деления
  int exp_result = 0;
  // если первое число меньше второго
  int div_less_flag = 0;
  // индекс для записи результата деления в массив result_arr_copy
  int index_result = 0;
  // массив result_arr_copy где будем содержать результат деления
  int result_arr_copy[NUM_255];
  for (int i = 0; i < NUM_255; i++) {
    result_arr_copy[i] = 2;
  }
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
  // если первое число меньше второго
  int exp_one = 1;
  if (count_bit_1 < count_bit_2) {
    // если первое число меньше второго, то добавляем нули в первое число +
    // начинаем сразу добавлять экспоненту
    div_less_flag = 1;
    while (div_less_val(value_1_arr, value_2_arr)) {
      div_mul_1010(value_1_arr, exp_one);
      index_1 = 0;
      count_bit_1 = 0;
      for (int i = 254; i >= 0; i--) {
        if (value_1_arr[i] == 1) index_1 = 1;
        if (index_1) {
          count_bit_1++;
        }
      }
      result_arr_copy[index_result] = 0;
      index_result++;
      exp_result++;
      if (exp_result >= 28) {
        // если не удается найти остаток 28 раз тогда делаем break и завершаем
        // цикл
        break;
      }
    }
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

  //  count_bit = количество битов в result_arr_copy
  int count_bit = 0;
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

  // индекс первого деления нужен для сдвига и записи 0 в result_arr_copy
  int start = 1;
  // заполняем arr_help первыми count_bit_2 цифрами value_2_arr индексами с 1 по
  // (count_bit_2 - 1)
  for (int i = 1; i < count_bit_2 + 1; i++) {
    arr_help[i] = value_1_arr[index_value_1];
    index_value_1--;
  }
  // флаг остановки цикла деления
  int flag_stop = count_bit_1;
  // цикл деления
  while (flag_stop) {
    // если уже > 28
    if (exp_result >= 28) {
      flag_stop = 0;
      break;
    }
    //  arr_help необходимо сравнить с value_2_arr. Если число меньше, то должны
    //  добавить еще один элемент из value_1_arr в arr_help
    while (div_less_number(arr_help, value_2_arr_help, count_bit_2)) {
      // делаем сдвиг arr_help влево на 1 бит + в последний эелемент ставим еще
      // одно число из value_1_arr
      for (int i = 0; i < count_bit_2; i++) {
        arr_help[i] = arr_help[i + 1];
      }
      if (index_value_1 >= 0 && !div_less_flag) {
        arr_help[count_bit_2] = value_1_arr[index_value_1];
        index_value_1--;
      } else {
        exp_result++;
        arr_help[count_bit_2] = 0;
      }
      // проверяем arr_help когда скопировали последний элемент из value_1_arr
      // Если при деление до остатка arr_help != value_2_arr_help значит
      // продолжаем деление
      if (index_value_1 < 0 || div_less_flag) {
        // проверяем на сколько большое число
        if (index_result > 96) {
          flag_stop = 0;
          break;
        } else {
          flag_stop = 0;
          for (int i = 0; i < count_bit_2 + 1; i++) {
            if (arr_help[i] != value_2_arr_help[i]) {
              flag_stop = 1;
              break;
            }
          }
        }
      }
      // пишем 0 в result_arr_copy если добавили разряд, а число все еще меньше
      // value_2_arr. Здесь же учитываем индекс первого деления
      if (!start && div_less_number(arr_help, value_2_arr_help, count_bit_2)) {
        result_arr_copy[index_result] = 0;
        index_result++;
      }
      if (exp_result >= 28) {
        // если не удается найти остаток 28 раз тогда делаем break и завершаем
        // цикл
        flag_stop = 0;
        break;
      }
    }
    // обнуление индекса первого деления
    start = 0;
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
    result_arr_copy[index_result] = 1;
    index_result++;
    // обновление arr_help
    for (int i = 0; i < count_bit_2 + 1; i++) {
      arr_help[i] = arr_help_copy[i];
    }
    // проверка на конечный 0. Если после деления в числе остались только нули и
    // вычитание тоже == 0, тогда дописываем оставшиеся нули в результат и
    // завершаем цикл
    zero_end_arr = 0;
    zero_end_val = 0;
    for (int i = 0; i < count_bit_2 + 1; i++) {
      if (arr_help[i] == 1) zero_end_arr = 1;
    }
    for (int i = index_value_1; i >= 0; i--) {
      if (value_1_arr[i] == 1) zero_end_val = 1;
    }
    if (!zero_end_arr && !zero_end_val) {
      while (index_value_1 >= 0) {
        result_arr_copy[index_result] = 0;
        index_result++;
        index_value_1--;
      }
      break;
    }
  }
  // запись результата деления из result_arr_copy в result_arr
  // поиск конца результата в result_arr_copy
  for (int i = 254; i >= 0; i--) {
    if (result_arr_copy[i] != 2) {
      index = i;
      break;
    }
  }
  // используем start как индекс для записи result_arr
  start = 0;
  // запись из result_arr_copy в result_arr
  // инициализация (по сути обнуление) result_arr чтобы вписать туда новое число
  for (int i = 0; i < NUM_255; i++) {
    result_arr[i] = 2;
  }
  while (index >= 0) {
    result_arr[start] = result_arr_copy[index];
    index--;
    start++;
  }
  // в result_arr находится результат деления без учета деленгия с остатком.
  // Теперь нужно exp_result раз умножить результат на 1010 двоично если у нас
  // не случай деления маленького числа на большее
  if (!div_less_flag) error = div_mul_1010(result_arr, exp_result);
  // считаем какое количество битов в result_arr
  index_1 = 0;
  count_bit = 0;
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
  index_2 = exp_result;
  int exp_result_copy = exp_result;
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
      add_div_10(result_arr, &exp_result_copy, &count_bit);
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
  if (error == 123 && (sign_index == 1 || sign_index == 2)) {
    return 1;
  } else if (error == 123 && (sign_index == 3 || sign_index == 4)) {
    return 2;
  }
  index = 95;
  write_float_decimal_exp_more(result_arr, result, index, 2);
  // создаем массив в котором будет двоичная запись count_10
  int count_10_bit[8] = {0};
  count10_to_bin(&exp_result, count_10_bit);
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
  free(arr_help);
  free(arr_help_copy);
  return 0;
}

/*
    function div_less_number
    Функция сравнивает вспомогательный массив arr_help с value_2_arr. Если
    меньше, то возвращает 1. Если больше либо равно, то возвращает 0
*/
int div_less_number(int *arr_help, int *value_2_arr_help, int count_bit_2) {
  // если число >= value_2_arr то вернется 0
  int answer = 0;
  // j = count_bit_2 потому что первый элемент в arr_help может быть 0, а значит
  // проверяем со сдвигом и учитывая это
  for (int i = 0; i < count_bit_2 + 1; i++) {
    if (arr_help[i] != value_2_arr_help[i]) {
      if (arr_help[i] == 1) {
        //  arr_help больше или равно value_2_arr
        answer = 0;
        break;
      } else {
        //  arr_help меньше value_2_arr
        answer = 1;
        break;
      }
    }
  }
  return answer;
}

/*
    function div_less_val
    Функция сравнивает value_1_arr с value_2_arr. Если
    меньше, то возвращает 1. Если больше либо равно, то возвращает 0
*/
int div_less_val(int *value_1_arr, int *value_2_arr) {
  // если число >= value_2_arr то вернется 0
  int answer = 0;
  for (int i = 254; i >= 0; i--) {
    if ((value_1_arr[i] != value_2_arr[i]) && value_1_arr[i] != 2 &&
        value_2_arr[i] != 2) {
      if (value_1_arr[i] == 1) {
        //  value_1_arr больше или равно value_2_arr
        answer = 0;
        break;
      } else {
        //  value_1_arr меньше value_2_arr
        answer = 1;
        break;
      }
    }
  }
  return answer;
}

/*
    function div_mul_1010
    Производит умножение result_arr на 1010 exp_1 раз. Необходима для функции
    деления
*/
int div_mul_1010(int *result_arr, int exp_1) {
  // флаг ошибки для переполнения
  int error = 0;
  // индекс для цикла умножения и массив где будет храниться промежуточный
  // результат
  int result[NUM_255];
  int index = 0;
  int j = 2;
  int memory = 0;
  // определяем разность показателей степеней и создаем копию массива, который
  // имеет малую экспоненту. Далее этот массив и будем умножать на 10 пока
  // экспоненты не станут равными
  int exp_delta = exp_1;
  int value_min_exp[NUM_255];
  for (int i = 0; i < NUM_255; i++) {
    value_min_exp[i] = result_arr[i];
  }
  // умножаем на 10 (1010 в двоичной) пока дельта не станет = 0
  while (exp_delta) {
    // инициализация каждый цикл
    index = 0;
    j = 2;
    for (int i = 0; i < NUM_255; i++) {
      result[i] = 2;
    }
    // счет количества занятых битов для понимания "а можно ли вообще наше число
    // умножать на 10? А вдруг оно уже занимает 96 бита и после умножения не
    // поместится в decimal"
    int count_bit = 0;
    int count_1_bits = 0;
    for (int i = 254; i >= 0; i--) {
      if (value_min_exp[i] == 1) {
        index = 1;
        count_1_bits++;
      }
      if (index) count_bit++;
    }
    //  printf("count_bit %d\n", count_bit);
    // вернули индекс в прежнее значение
    index = 0;
    // если единичных битов 96 или больше то умножать дальше нельзя -> будет
    // переполнение ставим error 123 (спец ошибка показывающая максимальное
    // заполнение decimal). Умножаем дальше, потому что может быть ситуация
    // когда
    //  MAX_DECIMAL - 0.07 и технически нужно записать в результат MAX_DECIMAL
    // То есть эта ошибка сигнализирует о том, что у нас возможно произошло
    // переполнение, но так ли это, решается в основной функции, учитывая знаки
    // слагаемых
    if (count_1_bits >= 96) {
      error = 123;
    }
    // последний элемент всегда 0 при умножении на 10, а следующие два элемента
    // всегда копии
    result[index] = 0;
    index++;
    result[index] = value_min_exp[index - 1];
    index++;
    result[index] = value_min_exp[index - 1];
    index++;
    // цикл сложения
    while (value_min_exp[j] != 2) {
      if (value_min_exp[j] == 1 && value_min_exp[j - 2] == 1) {
        // нет в памяти доп разряда
        if (memory == 0) {
          result[index] = 0;
          memory = 1;
        } else {
          result[index] = 1;
        }
      } else if ((value_min_exp[j] == 0 && value_min_exp[j - 2] == 1) ||
                 (value_min_exp[j] == 1 && value_min_exp[j - 2] == 0)) {
        //  нет в памяти доп разряда
        if (memory == 0) {
          result[index] = 1;
        } else {
          result[index] = 0;
        }
      } else if (value_min_exp[j] == 0 && value_min_exp[j - 2] == 0) {
        //  нет в памяти доп разряда
        if (memory == 0) {
          result[index] = 0;
        } else {
          result[index] = 1;
          memory = 0;
        }
      }
      index++;
      j++;
    }
    //  сложение первых двух разрядов
    for (int k = 0; k <= 1; k++, j++, index++) {
      if (value_min_exp[j - 2] == 1 && memory == 1) {
        result[index] = 0;
      } else if ((value_min_exp[j - 2] == 0 && memory == 1) ||
                 (value_min_exp[j - 2] == 1 && memory == 0)) {
        result[index] = 1;
        memory = 0;
      } else if (value_min_exp[j - 2] == 0 && memory == 0) {
        result[index] = 0;
      }
    }
    if (memory == 1) {
      result[index] = 1;
      memory = 0;
    }
    //  запись в правильном порядке в новый массив
    //  обновляем value_min_exp
    for (int k = 0; k < NUM_255; k++) {
      value_min_exp[k] = result[k];
    }
    //  уменьшаем дельту и увеличиваем показатель экспоненты
    exp_delta--;
  }
  for (int k = 0; k < NUM_255; k++) {
    result_arr[k] = value_min_exp[k];
  }
  return error;
}
