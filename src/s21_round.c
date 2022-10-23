#include "s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int error = 0;
  int sign_op = 0;
  int exp = s21_10_conv(value);
  int last_digit = 0;
  s21_decimal *res = result;

  sign_op = test_bit(value.bits[3], 31);

  if (exp != 0) {
    s21_decimal buf = {0};
    buf = value;
    s21_truncate_buf(&buf, exp - 1);
    last_digit = return_last_digit_and_truncate_buf_by_it(&buf);
    *res = buf;
    if (last_digit >= 5) {
      s21_decimal one = {{1, 0, 0, 0}};
      if (last_digit == 5) {
        if (test_bit((res->bits[0]), 0)) {
          s21_add(*res, one, res);
        }
      } else if (last_digit > 5) {
        s21_add(*res, one, res);
      }
    }
    *result = *res;
    if (sign_op) {
      set_1_bit(&result->bits[3], 31);
    }
  } else {
    *result = value;
  }

  return error;
}
