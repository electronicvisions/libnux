/*
    Enable gcc option '-fstack-protector-all' for test to fail.
    Returns 0xdeafbeef (3735928559), if stack-guard has been enabled.
*/
int start(void)
{
	__attribute__((unused)) int volatile a[2];

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
	/* Write out of bounds into array, overwriting stack-guard magic value. */
	a[2] = 5;
#pragma GCC diagnostic pop

	return 0;
}
