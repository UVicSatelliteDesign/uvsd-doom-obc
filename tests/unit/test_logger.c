/**
 * @file  test_logger.c
 * @brief Unit tests for OBC Requirement 4:
 *        "Store spacecraft logs."
 *
 * Tests cover writing log entries, reading them back, handling a full log,
 * and correct severity filtering.
 *
 * TODO: #include "logger.h" once the module is written.
 *       Placeholder types (LogLevel_t, LogEntry_t, LogStatus_t) should be
 *       defined in logger.h and included here at that point.
 */

#include "../unity/unity.h"
#include <stdint.h>
#include <string.h>

/* ── Fixture ─────────────────────────────────────────────────────────────── */

void setUp(void)    {}
void tearDown(void) {}

/* ── Tests ───────────────────────────────────────────────────────────────── */

void test_log_rejects_null_message(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when logger.c exists");
}

void test_log_rejects_empty_string_message(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when logger.c exists");
}

void test_log_write_returns_ok_for_valid_entry(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when logger.c exists");
}

void test_log_stores_correct_level(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when logger.c exists");
}

void test_log_stores_correct_message_content(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when logger.c exists");
}

void test_log_entry_count_increments_on_write(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when logger.c exists");
}

void test_log_read_returns_entries_in_chronological_order(void)
{
    /* Entries must be retrievable oldest-first. */
    TEST_IGNORE_MESSAGE("TODO: implement when logger.c exists");
}

void test_log_full_returns_error_without_data_loss(void)
{
    /* When the log is full, writing must return an error and must
     * NOT overwrite the oldest unread entry without an explicit wrap policy. */
    TEST_IGNORE_MESSAGE("TODO: implement when logger.c exists");
}

void test_log_critical_entries_are_never_overwritten(void)
{
    /* Critical log entries must be preserved even when the log wraps. */
    TEST_IGNORE_MESSAGE("TODO: implement when logger.c exists");
}

void test_log_is_empty_after_init(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when logger.c exists");
}

void test_log_message_truncated_to_max_length_without_crash(void)
{
    /* A message exceeding the maximum length must be truncated, not
     * overrun the buffer. */
    TEST_IGNORE_MESSAGE("TODO: implement when logger.c exists");
}

void test_log_all_severity_levels_accepted(void)
{
    /* Every defined log level must be accepted as valid. */
    TEST_IGNORE_MESSAGE("TODO: implement when logger.c exists");
}

/* ── Runner ──────────────────────────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_log_rejects_null_message);
    RUN_TEST(test_log_rejects_empty_string_message);
    RUN_TEST(test_log_write_returns_ok_for_valid_entry);
    RUN_TEST(test_log_stores_correct_level);
    RUN_TEST(test_log_stores_correct_message_content);
    RUN_TEST(test_log_entry_count_increments_on_write);
    RUN_TEST(test_log_read_returns_entries_in_chronological_order);
    RUN_TEST(test_log_full_returns_error_without_data_loss);
    RUN_TEST(test_log_critical_entries_are_never_overwritten);
    RUN_TEST(test_log_is_empty_after_init);
    RUN_TEST(test_log_message_truncated_to_max_length_without_crash);
    RUN_TEST(test_log_all_severity_levels_accepted);

    return UNITY_END();
}