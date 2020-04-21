#include <cstddef>
#include <s2pp.h>

#include "libnux/unittest.h"

volatile uint8_t u8_value = 5;
constexpr uint8_t u8_constant = 7;

volatile int8_t s8_value = -13;
constexpr int8_t s8_constant = -27;

volatile uint16_t u16_value = 5;
constexpr uint16_t u16_constant = 7;

volatile int16_t s16_value = -13;
constexpr int16_t s16_constant = -27;

void start() {
	libnux_test_init();

	libnux_testcase_begin("splat u8");
	__vector uint8_t u8_vec_from_variable = vec_splat_u8(u8_value);
	__vector uint8_t u8_vec_from_constant = vec_splat_u8(u8_constant);
	for (size_t i = 0; i < sizeof(__vector uint8_t); ++i) {
		libnux_test_equal(u8_vec_from_variable[i], u8_value);
		libnux_test_equal(u8_vec_from_constant[i], u8_constant);
	}
	libnux_testcase_end();

	libnux_testcase_begin("splat s8");
	__vector int8_t s8_vec_from_variable = vec_splat_s8(s8_value);
	__vector int8_t s8_vec_from_constant = vec_splat_s8(s8_constant);
	for (size_t i = 0; i < sizeof(__vector int8_t); ++i) {
		libnux_test_equal(s8_vec_from_variable[i], s8_value);
		libnux_test_equal(s8_vec_from_constant[i], s8_constant);
	}
	libnux_testcase_end();

	libnux_testcase_begin("splat u16");
	__vector uint16_t u16_vec_from_variable = vec_splat_u16(u16_value);
	__vector uint16_t u16_vec_from_constant = vec_splat_u16(u16_constant);
	for (size_t i = 0; i < sizeof(__vector uint16_t) / sizeof(uint16_t); ++i) {
		libnux_test_equal(u16_vec_from_variable[i], u16_value);
		libnux_test_equal(u16_vec_from_constant[i], u16_constant);
	}
	libnux_testcase_end();

	libnux_testcase_begin("splat s16");
	__vector int16_t s16_vec_from_variable = vec_splat_s16(s16_value);
	__vector int16_t s16_vec_from_constant = vec_splat_s16(s16_constant);
	for (size_t i = 0; i < sizeof(__vector int16_t) / sizeof(uint16_t); ++i) {
		libnux_test_equal(s16_vec_from_variable[i], s16_value);
		libnux_test_equal(s16_vec_from_constant[i], s16_constant);
	}
	libnux_testcase_end();

	libnux_test_summary();
	libnux_test_shutdown();
}
