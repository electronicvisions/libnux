#include <s2pp.h>
#include "libnux/vx/mailbox.h"
#include "libnux/vx/new.h"
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

template<typename T>
struct CRTP {
	CRTP() {
		testcase_begin(T::test_name);
	}

	~CRTP() {
		testcase_end();
	}
};

struct VectorTestCRTP : public CRTP<VectorTestCRTP> {
	VectorTestCRTP() {
		__vector uint8_t lhs = {
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
			30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
			44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
			58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
			72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
			86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
			100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
			112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
			124, 125, 126, 127};
		__vector uint8_t rhs = vec_splat_u8(1);
		__vector uint8_t res = vec_add(lhs, rhs);
		for (uint32_t index = 0; index < sizeof(__vector uint8_t); index++) {
			test_equal(res[index], index + 1);
		}
	}
	constexpr static const char* test_name = "vector_add_CRTP";
};

struct INHE {
	INHE(const char* test_name) {
		testcase_begin(test_name);
	}

	~INHE() {
		testcase_end();
	}
};

struct VectorTestINHE : public INHE {
	VectorTestINHE() :
		INHE("vector_add_INHE")
	{
		__vector uint8_t lhs = {
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
			30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
			44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
			58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
			72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
			86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
			100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
			112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
			124, 125, 126, 127};
		__vector uint8_t rhs = vec_splat_u8(1);
		__vector uint8_t res = vec_add(lhs, rhs);
		for (uint32_t index = 0; index < sizeof(__vector uint8_t); index++) {
			test_equal(res[index], index + 1);
		}
	}
};

struct VIRT {
	VIRT() = default;

	void operator()() {
		testcase_begin(get_test_name());
		test();
	}

	virtual ~VIRT() {
		testcase_end();
	}

	virtual void test() = 0;
	virtual const char* get_test_name() = 0;
};

struct VectorTestVIRT : public VIRT {
	void test() override {
		__vector uint8_t lhs = {
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
			30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
			44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
			58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
			72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
			86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
			100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
			112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
			124, 125, 126, 127};
		__vector uint8_t rhs = vec_splat_u8(1);
		__vector uint8_t res = vec_add(lhs, rhs);
		for (uint32_t index = 0; index < sizeof(__vector uint8_t); index++) {
			test_equal(res[index], index + 1);
		}
	}

	const char* get_test_name() override {
		return "vector_add_VIRT";
	}
};

struct VectorTestGLOB {
	void test() {
		testcase_begin(test_name);
		__vector uint8_t lhs = {
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
			30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
			44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
			58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
			72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
			86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
			100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
			112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
			124, 125, 126, 127};
		__vector uint8_t rhs = vec_splat_u8(1);
		__vector uint8_t res = vec_add(lhs, rhs);
		for (uint32_t index = 0; index < sizeof(__vector uint8_t); index++) {
			test_equal(res[index], index + 1);
		}
	}

	VectorTestGLOB() = default;

	~VectorTestGLOB() {
		testcase_end();
	}

	constexpr static const char* test_name = "vector_add_GLOB";
};

VectorTestGLOB test4;

struct VectorTestNEW {
	VectorTestNEW(char const* /*test_name*/) {
		mailbox_write_string(__PRETTY_FUNCTION__);
		mailbox_write_string("\n");
	}

	VectorTestNEW() : VectorTestNEW("VectorTestNEW_default")
	{
		mailbox_write_string(__PRETTY_FUNCTION__);
		mailbox_write_string("\n");
	}

	~VectorTestNEW() {
		mailbox_write_string(__PRETTY_FUNCTION__);
		mailbox_write_string("\n");
	}
};

struct TestInit {
	TestInit() {
		test_init();
	}

	~TestInit() {
		test_summary();
		test_shutdown();
	}
};

void print_memory_layout() {
	void* p = 0;
	mailbox_write_string("MEMORY LAYOUT:");
	mailbox_write_string("\n\theap_base = ");
	mailbox_write_int(reinterpret_cast<intptr_t>(&heap_base));
	mailbox_write_string("\n\theap_end = ");
	mailbox_write_int(reinterpret_cast<intptr_t>(&heap_end));
	mailbox_write_string("\n\tcurrent stack ptr = ");
	mailbox_write_int(reinterpret_cast<intptr_t>(&p));
	mailbox_write_string("\n\tmailbox_base = ");
	mailbox_write_int(reinterpret_cast<intptr_t>(&mailbox_base));
	mailbox_write_string("\n\tmailbox_end = ");
	mailbox_write_int(reinterpret_cast<intptr_t>(&mailbox_end));
	mailbox_write_string("\n");
}

void start(void) {
	VectorTestNEW* test5 = new VectorTestNEW("VectorTestNEW");
	delete(test5);

	VectorTestNEW* test6 = new VectorTestNEW[2];
	delete[](test6);

	char* buf = new char[sizeof(VectorTestNEW)];
	VectorTestNEW* test7 = new (buf) VectorTestNEW("VectorTestNEWagain");
	delete(test7);

	print_memory_layout();

	TestInit t;
	{
		VectorTestCRTP test0;
	}
	{
		VectorTestINHE test2;
	}
	{
		VectorTestVIRT test3;
		test3();
	}


	test4.test(); // non-working output (mailbox ordering...)
}
