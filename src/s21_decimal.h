#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIT 8
#define SIZE_BITS 4
#define NUM_255 255
#define TRUE 1
#define FALSE 0
#define BIT_SIZE 4

#define S21_MAX_UINT 4294967295
#define is_fin(x) __builtin_isfinite(x)
#define is_nan(x) __builtin_isnan(x)
#define is_inf(x) __builtin_isinf(x)

typedef struct {                 // bits[0] младшие 32 бита
  unsigned int bits[SIZE_BITS];  // bits[1] средние 32 бита
} s21_decimal;                   // bits[2] старшие 32 бита
// bits[3] содержит коэффициент масштабирования и знак
/*
                        about bits[3]
- Биты от 0 до 15, младшее слово, не используются и должны быть равны нулю
- Биты с 16 по 23 должны содержать показатель степени от 0 до 28, который
указывает степень 10 для разделения целого числа
- Биты с 24 по 30 не используются и должны быть равны нулю
- Бит 31 содержит знак; 0 означает положительный, а 1 означает отрицательный

*/
typedef union {
  float flt;
  unsigned int bits;
} lens_t;

// Вспомогательные функции по работе с битами
int test_bit(unsigned int value, int BitNumber);
int test_bit_int(int value, int BitNumber);
void set_1_bit(unsigned int *value, int BitNumber);
void set_1_bit_int(int *value, int BitNumber);
void set_0_bit(unsigned int *value, int BitNumber);
void set_0_bit_int(int *value, int BitNumber);
int check_bits_decimal_int(s21_decimal *src, int num);
void invert_bit(unsigned int *value, int BitNumber);
int is_int_negative(int value);
int float_2_bits(float src);
int exp_float_bin2dec(/*uint32_t*/ int bits);
int exp_decimal_bin2dec(s21_decimal dst);
void count10_to_bin(int *count_10, int *count_10_bit);
void write_float_decimal_exp_less(int *bits_copy, s21_decimal *dst, int index,
                                  int bit);
void write_float_decimal_exp_more(int *result, s21_decimal *dst, int index,
                                  int bit);
void exp_less_23(int *all_bits_float, int *count_10, int integer_bits,
                 int fractional_bits, s21_decimal *dst, int index_less_0);
int exp_more_23(int *all_bits_float, int exp, s21_decimal *dst);
int float2decimal_main(int float_bits, int exp, s21_decimal *dst);
int s21_is_less_num(s21_decimal num1, s21_decimal num2, int result);
int s21_is_greater_num(s21_decimal num1, s21_decimal num2, int result);
void s21_unshift(s21_decimal *number);
int s21_shift(s21_decimal *number);
void s21_invert_mantisa(s21_decimal *value);
int s21_addiction_logic(s21_decimal value_1, s21_decimal value_2,
                        s21_decimal *result);
int get_sign(s21_decimal *num);
void mult_10(s21_decimal value, s21_decimal *result);
s21_decimal s21_superior_10(int power, s21_decimal *value);
int s21_10_conv(s21_decimal value);
void s21_rev_10_conv(s21_decimal *value, int result);
int check_scale_less(s21_decimal num1, s21_decimal num2, int result);
int check_scale_greater(s21_decimal num1, s21_decimal num2, int result);
int s21_zero(s21_decimal value);
void s21_truncate_buf(s21_decimal *buf, int exp);
int return_last_digit_and_truncate_buf_by_it(s21_decimal *buf);
int add_mul_1010(int *value_1_arr, int *value_2_arr, int *exp_1, int *exp_2);
void add_div_10(int *result_arr, int *exp_1, int *count_bit);
int add_less_1010(int *arr_help, int *arr_10);
int add_main(s21_decimal value_1, int exp_1, s21_decimal value_2, int exp_2,
             s21_decimal *result);
int mul_main(s21_decimal value_1, int exp_1, s21_decimal value_2, int exp_2,
             s21_decimal *result);
void copy_zero(int *help_arr);
void copy_value(int *help_arr, int *value);
int div_main(s21_decimal value_1, int exp_1, s21_decimal value_2, int exp_2,
             s21_decimal *result);
int div_less_number(int *arr_help, int *value_2_arr_help, int count_bit_2);
int div_less_val(int *value_1_arr, int *value_2_arr);
int div_mul_1010(int *result_arr, int exp_1);
int mod_main(s21_decimal value_1, int exp_1, s21_decimal value_2, int exp_2,
             s21_decimal *result);

// Arithmetic Operators
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// Convertors and parsers
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Comparison Operators
int s21_is_less(s21_decimal num1, s21_decimal num2);
int s21_is_less_or_equal(s21_decimal num1, s21_decimal num2);
int s21_is_greater(s21_decimal num1, s21_decimal num2);
int s21_is_greater_or_equal(s21_decimal num1, s21_decimal num2);
int s21_is_equal(s21_decimal num1, s21_decimal num2);
int s21_is_not_equal(s21_decimal num1, s21_decimal num2);

// Others
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);

#endif  // SRC_S21_DECIMAL_H_
