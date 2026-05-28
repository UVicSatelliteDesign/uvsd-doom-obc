/**
 * @file  test_example.c
 * @brief Example / template unit tests for the UVSD DOOM OBC.
 *
 * This file shows the testing patterns to follow when adding real tests.
 * Each logical module gets its own test_<module>.c file in this directory.
 *
 * HOW TO ADD A NEW TEST
 * ─────────────────────
 * 1. Create tests/unit/test_<your_module>.c (copy this file as a template).
 * 2. #include the module header you want to test.
 * 3. Write setUp() / tearDown() for per-test fixture work (can be empty).
 * 4. Write void test_<thing>(void) functions using Unity assertions.
 * 5. Call RUN_TEST(test_<thing>) inside the main() block at the bottom.
 * 6. Add the new .c file to TESTS in tests/Makefile.
 *
 * AVAILABLE UNITY ASSERTIONS (see tests/unity/unity.h for the full list)
 * ───────────────────────────────────────────────────────────────────────
 *   TEST_ASSERT_TRUE(cond)
 *   TEST_ASSERT_FALSE(cond)
 *   TEST_ASSERT_EQUAL(expected, actual)
 *   TEST_ASSERT_EQUAL_INT / _UINT32 / _FLOAT …
 *   TEST_ASSERT_NULL(ptr)
 *   TEST_ASSERT_NOT_NULL(ptr)
 *   TEST_ASSERT_EQUAL_STRING(exp, act)
 *   TEST_ASSERT_EQUAL_MEMORY(exp, act, len)
 *   TEST_FAIL_MESSAGE("reason")
 */

#include "../unity/unity.h"

/* ── Fixture ─────────────────────────────────────────────────────────────── */

void setUp(void)    { /* called before every test — initialise state here */ }
void tearDown(void) { /* called after  every test — clean up here          */ }

/* ── Helper: a trivial pure-C function to verify the harness works ───────── */

/** Returns the larger of two int32 values. */
static int32_t max_int32(int32_t a, int32_t b) { return (a >= b) ? a : b; }

/* ── Tests ───────────────────────────────────────────────────────────────── */

void test_max_returns_larger_when_a_is_bigger(void)
{
    TEST_ASSERT_EQUAL_INT(10, max_int32(10, 3));
}

void test_max_returns_larger_when_b_is_bigger(void)
{
    TEST_ASSERT_EQUAL_INT(7, max_int32(2, 7));
}

void test_max_returns_value_when_equal(void)
{
    TEST_ASSERT_EQUAL_INT(5, max_int32(5, 5));
}

void test_max_handles_negative_values(void)
{
    TEST_ASSERT_EQUAL_INT(-1, max_int32(-1, -5));
}

/* ── Runner ──────────────────────────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_max_returns_larger_when_a_is_bigger);
    RUN_TEST(test_max_returns_larger_when_b_is_bigger);
    RUN_TEST(test_max_returns_value_when_equal);
    RUN_TEST(test_max_handles_negative_values);

    return UNITY_END();
}