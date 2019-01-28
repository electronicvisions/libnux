/*
    The stack grows infinitely. With '-fstack-limit-symbol=stack_redzone',
    the program exits with error code 12 (= ENOMEM), whereas without this option,
    it doesn't finish, since the program memory region gets partially overwritten.
*/
int start(void)
{
	__attribute__((unused)) int volatile i = 5;
	start();
	return 0;
}
