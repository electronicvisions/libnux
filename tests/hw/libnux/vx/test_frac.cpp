#include <s2pp.h>
#include <stddef.h>

#include "libnux/vx/mailbox.h"
#include "libnux/vx/random.h"
#include "libnux/vx/unittest.h"
#include "libnux/vx/frac.h"

using namespace libnux::vx;

template <class T>
T random_typed(uint32_t* seed)
{
	xorshift32(seed);
	return *reinterpret_cast<T*>(seed);
}

#define TEST_ASM(VecOp)                                                                            \
	asm volatile(VecOp " %[ret], %[a], %[b]\n"                                                     \
	             : [ ret ] "=qv"(v_ret)                                                            \
	             : [ a ] "qv"(v_a), [ b ] "qv"(v_b));

#define TEST(Op, Type, VecSplat, Name, VecOp)                                                      \
	void test_##Name(uint32_t* seed, size_t num)                                                   \
	{                                                                                              \
		testcase_begin(#Type " " #Op);                                                             \
		Type f_a, f_b;                                                                             \
		__vector Type::data_type v_a;                                                              \
		__vector Type::data_type v_b;                                                              \
		__vector Type::data_type v_ret;                                                            \
		Type f_ret;                                                                                \
                                                                                                   \
		for (size_t i = 0; i < num; ++i) {                                                         \
			auto a = random_typed<Type::data_type>(seed);                                          \
			auto b = random_typed<Type::data_type>(seed);                                          \
			f_a = Type(a);                                                                         \
			f_b = Type(b);                                                                         \
			v_a = VecSplat(a);                                                                     \
			v_b = VecSplat(b);                                                                     \
                                                                                                   \
			TEST_ASM(VecOp)                                                                        \
                                                                                                   \
			f_ret = f_a Op f_b;                                                                    \
                                                                                                   \
			size_t const j =                                                                       \
			    xorshift32(seed) % sizeof(decltype(v_ret)) / sizeof(decltype(v_ret[0]));           \
			test_true(f_ret.data() == v_ret[j]);                                                   \
		}                                                                                          \
                                                                                                   \
		testcase_end();                                                                            \
	}

TEST(+,frac8_t,vec_splat_s8,frac8_add,"fxvaddbfs")
TEST(-,frac8_t,vec_splat_s8,frac8_sub,"fxvsubbfs")
TEST(*,frac8_t,vec_splat_s8,frac8_mul,"fxvmulbfs")
TEST(+,frac16_t,vec_splat_s16,frac16_add,"fxvaddhfs")
TEST(-,frac16_t,vec_splat_s16,frac16_sub,"fxvsubhfs")
TEST(*,frac16_t,vec_splat_s16,frac16_mul,"fxvmulhfs")
#undef TEST

void test_frac8_scale()
{
	testcase_begin("frac8_t scale");
	frac8_t f_a(100), f_b(112);

	frac8_t f_ret_mul = f_a * f_b;
	frac8_t f_ret_add = f_a + f_b;
	frac8_t f_ret_sub = f_a - f_b;

	// expected scale: frac8_t in [-1,1)
	test_true(f_ret_mul.scale() == INT8_MAX + 1);

	// scaled
	test_true(f_ret_mul.data() == f_a.data() * f_b.data() / 128);

	// not scaled
	test_true(f_ret_add.data() == INT8_MAX); // saturation
	test_true(f_ret_sub.data() == -12);

	testcase_end();
}

void test_frac16_scale()
{
	testcase_begin("frac16_t scale");
	frac16_t f_a(300), f_b(350);

	frac16_t f_ret_mul = f_a * f_b;
	frac16_t f_ret_add = f_a + f_b;
	frac16_t f_ret_sub = f_a - f_b;

	// expected scale: frac16_t in [-1,1)
	test_true(f_ret_mul.scale() == INT16_MAX + 1);

	// scaled
	test_true(f_ret_mul.data() == f_a.data() * f_b.data() / (INT16_MAX + 1));

	// not scaled
	test_true(f_ret_add.data() == 650);
	test_true(f_ret_sub.data() == -50);

	testcase_end();
}

void start()
{
	uint32_t seed = 1234;
	size_t num = 10;
	test_init();
	test_frac8_add(&seed, num);
	test_frac8_mul(&seed, num);
	test_frac8_sub(&seed, num);
	test_frac16_add(&seed, num);
	test_frac16_mul(&seed, num);
	test_frac16_sub(&seed, num);
	test_frac8_scale();
	test_frac16_scale();
	test_summary();
	test_shutdown();
}
