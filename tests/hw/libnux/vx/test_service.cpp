#include "libnux/scheduling/Service.hpp"
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

int count_function = 0;
int count_class_member_function = 0;

void test_function()
{
	++count_function;
}

class test_class
{
	int member_variable;

public:
	test_class(int variable)
	{
		member_variable = variable;
	}
	void member_function()
	{
		count_class_member_function += member_variable;
	}
};

void test_service_function_exec()
{
	testcase_begin("test_service_function_exec");
	Service service = &test_function;
	service();
	test_equal(count_function, 1);
	testcase_end();
}

void start()
{
	test_init();
	test_service_function_exec();
	test_summary();
	test_shutdown();
}
