#include "s21_decimal.h"

/*
    function s21_from_decimal_to_int
    Перевод переменной src из типа s21_decimal в тип int
*/

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error = 0;
  //  count_bit = количество битов в decimal числе
  int count_bit = 0;
  // массивы с битами
  int value_1_arr[NUM_255];
  int index_1 = 0;
  // инициализация
  for (int i = 0; i < NUM_255; i++) {
    value_1_arr[i] = 2;
  }
  for (int i = 0; i < 96; i++) {
    if (test_bit(src.bits[i / 32], i % 32)) {
      value_1_arr[i] = 1;
    } else {
      value_1_arr[i] = 0;
    }
  }
  for (int i = 254; i >= 0; i--) {
    if (value_1_arr[i] == 1) index_1 = 1;
    if (index_1) {
      count_bit++;
    }
  }
  // получение степени числа
  int exp_1 = exp_decimal_bin2dec(src);
  // если битов в числе больше 32 и экспонента не ноль
  if (count_bit > 32 && exp_1 > 0 && exp_1 <= 28) {
    // делим на 10 пока не выделим всю целую часть числа decimal
    //  index_2 используем для понимания сколько раз нужно поделить на
    //  1010, чтобы выделить целую часть числа
    int index_2 = exp_1;
    while (index_2) {
      index_1 = 0;
      add_div_10(value_1_arr, &exp_1, &count_bit);
      // обновление count_bit
      count_bit = 0;
      for (int i = 254; i >= 0; i--) {
        if (value_1_arr[i] == 1) index_1 = 1;
        if (index_1) {
          count_bit++;
        }
      }
      index_2--;
    }
    // когда выделили целую часть считаем биты в получившемся числе, если больше
    //  32 значит число точно не поместиться в decimal, если меньше, то переносим
    // это число в инт
    if (count_bit > 32) {
      error = 1;
    } else {
      *dst = 0;
      for (int i = 0; i < count_bit; i++) {
        if (value_1_arr[i]) {
          set_1_bit_int(dst, i);
        } else {
          set_0_bit_int(dst, i);
        }
      }
      if (test_bit(src.bits[3], 31)) {
        *dst *= -1;
      }
    }
  } else {
    for (int i = 32; i < 96; i++) {
      if (test_bit(src.bits[i / 32], i % 32) != 0) {
        error = 1;
        break;
      }
    }
    if (!error) {
      *dst = 0;
      for (int i = 31; i >= 0; i--) {
        if (test_bit(src.bits[0], i) != 0) {
          set_1_bit_int(dst, i);
        }
      }
      if (test_bit(src.bits[3], 31)) {
        *dst *= -1;
      }
      *dst /= (int)pow(10, exp_decimal_bin2dec(src));
    }
  }
  return error;
}
