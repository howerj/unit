#ifndef _UNIT_H_
#define _UNIT_H_

#define UNIT_TEST_AUTHOR "Richard James Howe"
#define UNIT_TEST_LICENSE "Public Domain"
#define UNIT_TEST_EMAIL "howe.r.j.89@gmail.com"
#define UNIT_TEST_REPO "https://github.com/howerj/unit"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifndef UNIT_TESTS
#define UNIT_TESTS (1)
#endif

#ifndef UNIT_TEST_COLOR
#define UNIT_TEST_COLOR (0)
#endif

typedef struct {
	FILE *output;
	int fatal, color;
	unsigned passed, run;
} unit_test_t;

typedef int (*unit_test_module_f) (void);
typedef void (*unit_test_f) (unit_test_t *t);

static inline const char *unit_red(unit_test_t *t)    { assert(t); return t->color ? "\x1b[1;31m" : ""; }
static inline const char *unit_green(unit_test_t *t)  { assert(t); return t->color ? "\x1b[1;32m" : ""; }
static inline const char *unit_yellow(unit_test_t *t) { assert(t); return t->color ? "\x1b[1;33m" : ""; }
static inline const char *unit_blue(unit_test_t *t)   { assert(t); return t->color ? "\x1b[1;34m" : ""; }
static inline const char *unit_reset(unit_test_t *t)  { assert(t); return t->color ? "\x1b[0m" : ""; }

static inline unit_test_t _unit_test_start(FILE *output, const char *file, const char *func, unsigned line) {
	unit_test_t t = { .run = 0, .passed = 0, };
	t.output = output;
	if (fprintf(t.output, "Start tests: %s in %s:%u\n\n", func, file, line) < 0)
		t.fatal = -1;
	t.color = (UNIT_TEST_COLOR >= 2) || (UNIT_TEST_COLOR >= 1 && !!getenv("COLOR")); 
	return t;
}

static inline void _unit_test_statement(unit_test_t *t, const char *expr_str) {
	assert(t);
	assert(t->output);
	assert(expr_str);
	if (fprintf(t->output, "   %sSTATE%s: %s\n", unit_yellow(t), unit_reset(t), expr_str) < 0)
		t->fatal = -1;
}

static inline void _unit_test(unit_test_t *t, int failed, const char *expr_str, const char *file, const char *func, unsigned line, int die) {
	assert(t);
	assert(t->output);
	assert(expr_str);
	assert(file);
	assert(func);
	if (failed) {
		if (fprintf(t->output, "  %sFAILED%s: %s (%s:%s:%u)\n", unit_red(t), unit_reset(t), expr_str, file, func, line) < 0)
			t->fatal = -1;
		if (die) {
			(void)fputs("VERIFY FAILED - EXITING\n", t->output);
			exit(EXIT_FAILURE);
		}
	} else {
		fprintf(t->output, "      %sOK%s: %s\n", unit_green(t), unit_reset(t), expr_str);
		t->passed++;
	}
	t->run++;
}

static inline int unit_test_finish(unit_test_t *t) {
	assert(t);
	assert(t->output);
	if (fprintf(t->output, "Tests passed/total: %u/%u\n", t->passed, t->run) < 0)
		t->fatal = -1;
	if (t->run != t->passed) {
		(void)fprintf(t->output, "[%sFAILED%s]\n", unit_red(t), unit_reset(t));
		return -1;
	}
	if (fprintf(t->output, "[%sSUCCESS%s]\n", unit_green(t), unit_reset(t)) < 0)
		t->fatal = -1;
	return t->fatal ? -1 : 0;
}

#define unit_test_statement(T, EXPR) _unit_test_statement((T), (# EXPR));
#define unit_test_start()            _unit_test_start(stderr, __FILE__, __func__, __LINE__)
#define unit_test(T, EXPR)           _unit_test((T), 0 == (EXPR), (# EXPR), __FILE__, __func__, __LINE__, 0)
#define unit_test_verify(T, EXPR)    _unit_test((T), 0 == (EXPR), (# EXPR), __FILE__, __func__, __LINE__, 1)

#endif /* _UNIT_H_ */
