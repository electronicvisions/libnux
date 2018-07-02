/*
    Enable gcc option '-fstack-protector-all' for test to fail.
    Returns 0xdeafbeef (3735928559), if stack-guard has been enabled.
*/
int start(void)
{
	__attribute__((unused)) int a[2];

	/* Write out of bounds into array, overwriting stack-guard magic value. */
	for (int i = 0; i < 3; ++i) {
		a[i] = 5;
	}

	return 0;
}
