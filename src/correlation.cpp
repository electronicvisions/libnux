#include <stddef.h>

#include "libnux/correlation.h"


void reset_all_correlations()
{
	for (size_t row = 0; row < dls_num_rows; row++) {
		reset_correlation(row);
	}
}
