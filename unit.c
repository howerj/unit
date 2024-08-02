#include "unit.h"

#ifndef NELEMS
#define NELEMS(X) (sizeof (X) / sizeof ((X)[0]))
#endif

static int test1(void) {
	unit_test_t t = unit_test_start();

	unit_test_statement(&t, 1);
	unit_test(&t, 1 == 1);
	unit_test(&t, 2 == 2);

	return unit_test_finish(&t);
}


static int test2(void) {
	unit_test_t t = unit_test_start();

	unit_test(&t, 1 + 1 == 2);
	unit_test(&t, 2 + 2 == 3); // !!

	return unit_test_finish(&t);
}

typedef int (*test)(void);

static int run_all_tests(void) {
	/* If all of the test functions (e.g. `test1`, `test2`, ...) are defined as static and
	 * `UNIT_TESTS` is zero then the tests will be compiled out, no need for complicated build
	 * system magic. If `run_all_tests` is part of a modules API it will always return true in that
	 * case, but it means you do not have to do any macro magic to remove the tests. */
	if (!UNIT_TESTS)
		return 0;

	test tests[] = {
		test1,
		test2,
	};

	int r = 0;
	for (int i = 0; i < (int)NELEMS(tests); i++) {
		test fn = tests[i];
		if (fn() < 0)
			r = -i;
	}
	return r;
}

int main(void) {
	return run_all_tests();
}
