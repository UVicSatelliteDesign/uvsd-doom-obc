/**
 * @file  test_telemetry.c
 * @brief Unit tests for OBC Requirement 3:
 *        "Collection of telemetry from on board subsystems, storing
 *         telemetry, and forwarding telemetry to TTC when requested."
 *
 * Telemetry collection is handled by the OBC reading directly from GPIO,
 * so there is no collection layer to test here. These tests cover:
 *   A) Storage   — writing to and reading from the telemetry store
 *   B) Forwarding — packaging stored telemetry for TTC on request
 *
 * TODO: #include "telemetry.h" once the module is written.
 *       Placeholder types should be defined in telemetry.h and
 *       included here at that point.
 */

#include "../unity/unity.h"
#include <stdint.h>
#include <string.h>

/* ── Fixture ─────────────────────────────────────────────────────────────── */

void setUp(void)    {}
void tearDown(void) {}

/* ══ A) Storage tests ═════════════════════════════════════════════════════ */

void test_store_is_empty_after_init(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

void test_store_returns_error_when_full(void)
{
    /* When the store is full, the next insert must return an error
     * without corrupting existing data. */
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

void test_store_retrieves_frame_by_index(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

void test_store_retrieves_frames_in_insertion_order(void)
{
    /* Frames must be retrievable in the order they were stored. */
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

void test_store_count_increments_on_each_write(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

void test_store_count_does_not_exceed_capacity(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

/* ══ B) Forwarding tests ══════════════════════════════════════════════════ */

void test_forward_request_returns_error_on_empty_store(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

void test_forward_packages_most_recent_frame_when_requested(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

void test_forward_output_buffer_null_returns_error(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

void test_forward_reports_correct_output_length(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

/* ── Runner ──────────────────────────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    /* Storage */
    RUN_TEST(test_store_is_empty_after_init);
    RUN_TEST(test_store_returns_error_when_full);
    RUN_TEST(test_store_retrieves_frame_by_index);
    RUN_TEST(test_store_retrieves_frames_in_insertion_order);
    RUN_TEST(test_store_count_increments_on_each_write);
    RUN_TEST(test_store_count_does_not_exceed_capacity);

    /* Forwarding */
    RUN_TEST(test_forward_request_returns_error_on_empty_store);
    RUN_TEST(test_forward_packages_most_recent_frame_when_requested);
    RUN_TEST(test_forward_output_buffer_null_returns_error);
    RUN_TEST(test_forward_reports_correct_output_length);

    return UNITY_END();
}