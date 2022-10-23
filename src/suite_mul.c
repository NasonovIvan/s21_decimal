#include "tests_suitcases.h"

START_TEST(mul_test_1) {
  int num1 = -10;
  int num2 = -10;
  int prod_int = 100;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_int_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  int res_int = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_int(res_dec, &res_int);
  ck_assert_int_eq(res_int, prod_int);
}
END_TEST

START_TEST(mul_test_2) {
  int num1 = 10;
  int num2 = 20;
  int prod_int = 200;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_int_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  int res_int = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_int(res_dec, &res_int);
  ck_assert_int_eq(res_int, prod_int);
}
END_TEST

START_TEST(mul_test_3) {
  int num1 = -10;
  int num2 = 20;
  int prod_int = -200;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_int_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  int res_int = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_int(res_dec, &res_int);
  ck_assert_int_eq(res_int, prod_int);
}
END_TEST

START_TEST(mul_test_4) {
  int num1 = 9403;
  int num2 = 202;
  int res_origin = 1899406;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_int_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  int res_int = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_int(res_dec, &res_int);
  ck_assert_int_eq(res_int, res_origin);
}
END_TEST

START_TEST(mul_test_5) {
  int num1 = -32768;
  int num2 = 2;
  int res_origin = -65536;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_int_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  int res_int = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_int(res_dec, &res_int);
  ck_assert_int_eq(res_int, res_origin);
}
END_TEST

START_TEST(mul_test_6) {
  int num1 = -32768;
  int num2 = 32768;
  int res_origin = -1073741824;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_int_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  int res_int = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_int(res_dec, &res_int);
  ck_assert_int_eq(res_int, res_origin);
}
END_TEST

START_TEST(mul_test_7) {
  float num1 = 9403.0e2;
  int num2 = 202;
  float res_origin = 189940600;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_float_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  float res_float = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_float(res_dec, &res_float);
  ck_assert_float_eq(res_float, res_origin);
}
END_TEST

START_TEST(mul_test_8) {
  float num1 = 9403.0e2;
  float num2 = 9403.0e2;
  float res_origin = 884164090000;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_float_to_decimal(num1, &a);
  s21_from_float_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  float res_float = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_float(res_dec, &res_float);
  ck_assert_float_eq(res_float, res_origin);
}
END_TEST

START_TEST(s21_mul_1) {
  s21_decimal dec1 = {{4, 0, 0, 0}};
  s21_decimal dec2 = {{10, 0, 0, 0}};

  s21_decimal d22;
  s21_mul(dec1, dec2, &d22);

  int qqqq = 0;
  s21_from_decimal_to_int(d22, &qqqq);
  ck_assert_int_eq(qqqq, 40);
}
END_TEST

START_TEST(s21_mul_big_1) {
  s21_decimal dst1;
  dst1.bits[0] = 0b01110000000000000000000000000000;
  dst1.bits[1] = 0b10110011000000110001000010100111;
  dst1.bits[2] = 0b11100010001011101010010010010011;
  dst1.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal dst2;
  dst2.bits[0] = 0b00000000000000000000000000000001;
  dst2.bits[1] = 0b10000000000000100000000000000000;
  dst2.bits[2] = 0b00000000000000000000000000000000;
  dst2.bits[3] = 0b00000000000000000000000000000000;

  s21_decimal d22;
  int error = s21_mul(dst1, dst2, &d22);

  // int qqqq = 0;
  // s21_from_decimal_to_int(d22, &qqqq);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(s21_mul_big_2) {
  s21_decimal dst1;
  dst1.bits[0] = 0b01110000000000000000000000000000;
  dst1.bits[1] = 0b10110011000000110001000010100111;
  dst1.bits[2] = 0b11100010001011101010010010010011;
  dst1.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal dst2;
  dst2.bits[0] = 0b00000000000000000000000000000001;
  dst2.bits[1] = 0b10000000000000100000000000000000;
  dst2.bits[2] = 0b00000000000000000000000000000000;
  dst2.bits[3] = 0b10000000000000000000000000000000;

  s21_decimal d22;
  int error = s21_mul(dst1, dst2, &d22);

  // int qqqq = 0;
  // s21_from_decimal_to_int(d22, &qqqq);
  ck_assert_int_eq(error, 2);
}
END_TEST

// START_TEST(s21_mul_2) {
//   s21_decimal dec1 = {{100, 0, 0, 0}};     // 100
//   s21_decimal dec2 = {{999999, 0, 0, 0}};  // 9999.99
//   s21_set_scale(&dec2, 2);
//   s21_decimal res1;
//   s21_mul(dec1, dec2, &res1);
//   float g;
//   s21_from_decimal_to_float(res1, &g);
//   ck_assert_float_eq(g, 999999);
// }
// END_TEST

// START_TEST(s21_mul_3) {
//   s21_decimal dec1 = {{100, 0, 0, 0}};  // -100
//   s21_set_bit(&dec1, 127, 1);
//   s21_decimal dec2 = {{999999, 0, 0, 0}};  // 9999.99
//   s21_set_scale(&dec2, 2);
//   s21_decimal res1;
//   s21_mul(dec1, dec2, &res1);

//   float g;
//   s21_from_decimal_to_float(res1, &g);
//   ck_assert_float_eq(g, -999999);
// }
// END_TEST

// START_TEST(s21_mul_4) {
//   s21_decimal dec1 = {{1002, 0, 0, 0}};  // -100.2
//   s21_set_scale(&dec1, 1);
//   s21_set_bit(&dec1, 127, 1);

//   s21_decimal dec2 = {{999999, 0, 0, 0}};  // -9999.99
//   s21_set_scale(&dec2, 2);
//   s21_set_bit(&dec2, 127, 1);

//   s21_decimal res1;
//   s21_mul(dec1, dec2, &res1);
//   float g;
//   s21_from_decimal_to_float(res1, &g);
//   ck_assert_float_eq_tol(g, 1001998.998, 1e-06);
// }
// END_TEST

// START_TEST(s21_mul_5) {
//   s21_decimal dec1 = {{0, 0, 0, 0}};  // -0.0
//   s21_set_scale(&dec1, 1);
//   s21_set_bit(&dec1, 127, 1);

//   s21_decimal dec2 = {{999999, 0, 0, 0}};  // -9999.99
//   s21_set_scale(&dec2, 2);
//   s21_set_bit(&dec2, 127, 1);

//   s21_decimal res1;
//   s21_mul(dec1, dec2, &res1);
//   int g;
//   s21_from_decimal_to_int(res1, &g);
//   float r = (float)g * pow(10, -s21_get_scale(res1));
//   ck_assert_float_eq(r, 0.0);
// }
// END_TEST

// START_TEST(s21_mul_too_big) {
//   s21_decimal dec1 = {{BIT_SIZE - 1, BIT_SIZE - 1, BIT_SIZE - 1, 0}};
//   s21_decimal dec2 = {{2, 0, 0, 0}};

//   s21_decimal res;
//   int ret = s21_mul(dec1, dec2, &res);

//   ck_assert_int_eq(ret, 1);
// }
// END_TEST

// START_TEST(s21_mul_rounding) {
//   s21_decimal dec1 = {{123456789, 0, 0, 0}};
//   s21_set_scale(&dec1, 10);
//   s21_decimal dec2 = {{123456789, 0, 0, 0}};
//   s21_set_scale(&dec2, 10);

//   s21_decimal res;
//   s21_mul(dec1, dec2, &res);
//   float g;
//   s21_from_decimal_to_float(res, &g);
//   ck_assert_float_eq_tol(g, 0.00015241578512359410524368286132812, 1e-06);
// }
// END_TEST

// START_TEST(s21_mul_too_big_neg) {
//   s21_decimal dec1 = {{BIT_SIZE - 1, BIT_SIZE - 1, BIT_SIZE - 1, 0}};
//   s21_set_bit(&dec1, 127, 1);
//   s21_decimal dec2 = {{2, 0, 0, 0}};

//   s21_decimal res;
//   int ret = s21_mul(dec1, dec2, &res);

//   ck_assert_int_eq(ret, 2);
// }
// END_TEST

Suite *suite_mul(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("s21_mul");
  tc = tcase_create("case_mul");

  tcase_add_test(tc, mul_test_1);
  tcase_add_test(tc, mul_test_2);
  tcase_add_test(tc, mul_test_3);
  tcase_add_test(tc, mul_test_4);
  tcase_add_test(tc, mul_test_5);
  tcase_add_test(tc, mul_test_6);
  tcase_add_test(tc, mul_test_7);
  tcase_add_test(tc, mul_test_8);
  tcase_add_test(tc, s21_mul_1);
  tcase_add_test(tc, s21_mul_big_1);
  tcase_add_test(tc, s21_mul_big_2);
  // tcase_add_test(tc, s21_mul_2);
  // tcase_add_test(tc, s21_mul_3);
  // tcase_add_test(tc, s21_mul_4);
  // tcase_add_test(tc, s21_mul_5);
  // tcase_add_test(tc, s21_mul_too_big);
  // tcase_add_test(tc, s21_mul_too_big_neg);
  // tcase_add_test(tc, s21_mul_rounding);

  suite_add_tcase(s, tc);
  return s;
}
