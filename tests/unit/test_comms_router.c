/**
 * @file  test_comms_router.c
 * @brief Unit tests for OBC Requirement 1:
 *        "Control flow of communication between TTC and all other subsystems."
 *
 * Tests the routing layer that sits between the TTC (Telemetry, Tracking &
 * Command) radio and the other subsystems (payload, ADCS, EPS, etc.).
 *
 * TODO: #include "comms_router.h" once the module is written.
 *       Until then these tests act as a specification — they define the
 *       interface and expected behaviour before the implementation exists.
 *       Add RUN_TEST() calls and remove the TEST_IGNORE() stubs as each
 *       function is implemented.
 */

#include "../unity/unity.h"

/* ── Placeholder types (move to comms_router.h when that file exists) ─────── */

typedef enum {
    SUBSYSTEM_TTC     = 0,
    SUBSYSTEM_PAYLOAD = 1,
    SUBSYSTEM_ADCS    = 2,
    SUBSYSTEM_EPS     = 3,
    SUBSYSTEM_COUNT
} SubsystemID_t;

typedef enum {
    ROUTE_OK            = 0,
    ROUTE_ERR_NULL      = -1,
    ROUTE_ERR_SUBSYSTEM = -2,
    ROUTE_ERR_LEN       = -3
} RouteStatus_t;

/* ── Fixture ─────────────────────────────────────────────────────────────── */

void setUp(void)    {}
void tearDown(void) {}

/* ── Tests ───────────────────────────────────────────────────────────────── */

void test_route_rejects_null_payload(void)
{
    /* Passing NULL data to the router must return an error, never crash. */
    TEST_IGNORE_MESSAGE("TODO: implement when comms_router.c exists");
}

void test_route_rejects_zero_length_message(void)
{
    /* A zero-byte message is meaningless and should be rejected. */
    TEST_IGNORE_MESSAGE("TODO: implement when comms_router.c exists");
}

void test_route_rejects_oversized_message(void)
{
    /* Messages exceeding the maximum frame size must be rejected. */
    TEST_IGNORE_MESSAGE("TODO: implement when comms_router.c exists");
}

void test_route_rejects_invalid_subsystem_id(void)
{
    /* A destination ID >= SUBSYSTEM_COUNT is invalid. */
    TEST_IGNORE_MESSAGE("TODO: implement when comms_router.c exists");
}

void test_route_to_payload_succeeds_with_valid_message(void)
{
    /* A well-formed message addressed to PAYLOAD must return ROUTE_OK. */
    TEST_IGNORE_MESSAGE("TODO: implement when comms_router.c exists");
}

void test_route_to_adcs_succeeds_with_valid_message(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when comms_router.c exists");
}

void test_route_to_eps_succeeds_with_valid_message(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when comms_router.c exists");
}

void test_ttc_command_is_not_forwarded_back_to_ttc(void)
{
    /* Commands arriving from TTC must never be echoed back to TTC. */
    TEST_IGNORE_MESSAGE("TODO: implement when comms_router.c exists");
}

/* ── Runner ──────────────────────────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_route_rejects_null_payload);
    RUN_TEST(test_route_rejects_zero_length_message);
    RUN_TEST(test_route_rejects_oversized_message);
    RUN_TEST(test_route_rejects_invalid_subsystem_id);
    RUN_TEST(test_route_to_payload_succeeds_with_valid_message);
    RUN_TEST(test_route_to_adcs_succeeds_with_valid_message);
    RUN_TEST(test_route_to_eps_succeeds_with_valid_message);
    RUN_TEST(test_ttc_command_is_not_forwarded_back_to_ttc);

    return UNITY_END();
}