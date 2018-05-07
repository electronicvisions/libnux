#include "libnux/scheduling/Service.hpp"
#include "libnux/unittest.h"

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
	test_class(int variable) { member_variable = variable; }
	void member_function() { count_class_member_function += member_variable; }
};

void test_service_function_exec()
{
	libnux_testcase_begin("test_service_function_exec");
	auto service = Service_Function<0, &test_function>();
	service.exec();
	libnux_test_equal(count_function, 1);
	libnux_testcase_end();
}

void test_service_class_exec()
{
	libnux_testcase_begin("test_service_class_exec");
	test_class test = test_class(1);
	auto service = Service_Class<0, test_class, &test_class::member_function>(test);
	// exec
	service.exec();
	// no third element to be fetched
	libnux_test_equal(count_class_member_function, 1);
	libnux_testcase_end();
}

void start()
{
	libnux_test_init();
	test_service_function_exec();
	test_service_class_exec();
	libnux_test_summary();
	libnux_test_shutdown();
}
