/**
 * @file  test_command.c
 * @brief Unit tests for OBC Requirement 1:
 *        "Control flow of communication between TTC and all other subsystems."
 *
 * The OBC receives commands from the TTC radio and dispatches them to the
 * appropriate subsystem. These tests verify that incoming commands are parsed
 * correctly and forwarded to the right destination.
 *
 * OPEN QUESTION: Confirm with the electrical team whether subsystems will ever
 * send unprompted messages to the OBC, or whether all communication is
 * initiated by a command received from the radio.
 *
 */

#include "../unity/unity.h"
#include "command.h"

/* ── Fixture ─────────────────────────────────────────────────────────────── */

void setUp(void)    {}
void tearDown(void) {}

/* ── Tests ───────────────────────────────────────────────────────────────── */

void test_dispatch_rejects_null_command(void)
{
    /* Passing NULL to the dispatcher must return an error, never crash. */
    TEST_IGNORE_MESSAGE("TODO: implement when command_dispatcher.c exists");
}

void test_dispatch_rejects_zero_length_command(void)
{
    /* A zero-length command buffer is invalid and must be rejected. */
    TEST_IGNORE_MESSAGE("TODO: implement when command_dispatcher.c exists");
}

void test_dispatch_rejects_unknown_command_id(void)
{
    /* A command ID that does not map to any known subsystem must be rejected. */
    TEST_IGNORE_MESSAGE("TODO: implement when command_dispatcher.c exists");
}

void test_payload_command_dispatched_to_payload(void)
{
    /* A command addressed to the payload subsystem must be forwarded there. */
    TEST_IGNORE_MESSAGE("TODO: implement when command_dispatcher.c exists");
}

void test_adcs_command_dispatched_to_adcs(void)
{
    /* May be removed later: A command addressed to ADCS must be forwarded to ADCS. */
    TEST_IGNORE_MESSAGE("TODO: implement when command_dispatcher.c exists");
}

void test_eps_command_dispatched_to_eps(void)
{
    /* A command addressed to EPS must be forwarded to EPS. */
    TEST_IGNORE_MESSAGE("TODO: implement when command_dispatcher.c exists");
}

void test_obc_command_handled_locally(void)
{
    /* A command targeting the OBC itself must be handled internally,
     * not forwarded to any subsystem. */
    TEST_IGNORE_MESSAGE("TODO: implement when command_dispatcher.c exists");
}

/* ── Runner ──────────────────────────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_dispatch_rejects_null_command);
    RUN_TEST(test_dispatch_rejects_zero_length_command);
    RUN_TEST(test_dispatch_rejects_unknown_command_id);
    RUN_TEST(test_payload_command_dispatched_to_payload);
    RUN_TEST(test_adcs_command_dispatched_to_adcs);
    RUN_TEST(test_eps_command_dispatched_to_eps);
    RUN_TEST(test_obc_command_handled_locally);

    return UNITY_END();
}