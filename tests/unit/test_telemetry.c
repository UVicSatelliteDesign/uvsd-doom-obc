/**
 * @file  test_telemetry.c
 * @brief Unit tests for OBC Requirement 4:
 *        "Collection of telemetry from on board subsystems, storing
 *         telemetry, and forwarding telemetry to TTC when requested."
 *
 * Three distinct sub-concerns are tested here:
 *   A) Collection  — ingesting a telemetry frame from a subsystem
 *   B) Storage     — writing to and reading from the telemetry store
 *   C) Forwarding  — packaging stored telemetry for TTC on request
 *
 * TODO: #include "telemetry.h" once the module is written.
 */

#include "../unity/unity.h"
#include <stdint.h>
#include <string.h>

/* ── Placeholder types ───────────────────────────────────────────────────── */

#define TELEMETRY_MAX_PAYLOAD_BYTES  64U
#define TELEMETRY_STORE_CAPACITY     128U   /* number of frames the store holds */

typedef struct {
    uint32_t timestamp_ms;
    uint8_t  source_id;
    uint8_t  payload[TELEMETRY_MAX_PAYLOAD_BYTES];
    uint8_t  payload_len;
} TelemetryFrame_t;

typedef enum {
    TELEM_OK               = 0,
    TELEM_ERR_NULL         = -1,
    TELEM_ERR_LEN          = -2,
    TELEM_ERR_STORE_FULL   = -3,
    TELEM_ERR_STORE_EMPTY  = -4,
    TELEM_ERR_NOT_FOUND    = -5
} TelemStatus_t;

/* ── Fixture ─────────────────────────────────────────────────────────────── */

void setUp(void)    {}
void tearDown(void) {}

/* ══ A) Collection tests ══════════════════════════════════════════════════ */

void test_collect_rejects_null_frame(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

void test_collect_rejects_zero_payload_length(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

void test_collect_rejects_payload_exceeding_max_size(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

void test_collect_valid_frame_returns_ok(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

void test_collect_records_correct_source_id(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

/* ══ B) Storage tests ═════════════════════════════════════════════════════ */

void test_store_is_empty_after_init(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

void test_store_returns_error_when_full(void)
{
    /* After inserting TELEMETRY_STORE_CAPACITY frames, the next insert
     * must return TELEM_ERR_STORE_FULL without corrupting existing data. */
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

void test_store_retrieves_frame_by_index(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

void test_store_retrieves_frames_in_insertion_order(void)
{
    /* Frames must be retrievable in the order they were collected. */
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

void test_store_count_increments_on_each_collect(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

void test_store_count_does_not_exceed_capacity(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}

/* ══ C) Forwarding tests ══════════════════════════════════════════════════ */

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

    /* Collection */
    RUN_TEST(test_collect_rejects_null_frame);
    RUN_TEST(test_collect_rejects_zero_payload_length);
    RUN_TEST(test_collect_rejects_payload_exceeding_max_size);
    RUN_TEST(test_collect_valid_frame_returns_ok);
    RUN_TEST(test_collect_records_correct_source_id);

    /* Storage */
    RUN_TEST(test_store_is_empty_after_init);
    RUN_TEST(test_store_returns_error_when_full);
    RUN_TEST(test_store_retrieves_frame_by_index);
    RUN_TEST(test_store_retrieves_frames_in_insertion_order);
    RUN_TEST(test_store_count_increments_on_each_collect);
    RUN_TEST(test_store_count_does_not_exceed_capacity);

    /* Forwarding */
    RUN_TEST(test_forward_request_returns_error_on_empty_store);
    RUN_TEST(test_forward_packages_most_recent_frame_when_requested);
    RUN_TEST(test_forward_output_buffer_null_returns_error);
    RUN_TEST(test_forward_reports_correct_output_length);

    return UNITY_END();
}