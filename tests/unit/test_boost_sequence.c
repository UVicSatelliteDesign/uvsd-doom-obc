/**
 * @file  test_boot_sequence.c
 * @brief Unit tests for OBC Requirement 2:
 *        "Boot sequence and deployment (antennas)."
 *
 * The boot sequence is a state machine. Tests verify that:
 *   - States transition in the correct order
 *   - The mandatory 30-minute post-ejection hold is enforced before
 *     antenna deployment (standard CubeSat requirement)
 *   - Deployment is not re-triggered if already deployed
 *   - The sequence handles subsystem init failures gracefully
 *
 * TODO: #include "boot_sequence.h" once the module is written.
 */

#include "../unity/unity.h"

/* ── Placeholder types ───────────────────────────────────────────────────── */

typedef enum {
    BOOT_STATE_POWER_ON      = 0,
    BOOT_STATE_HW_INIT       = 1,
    BOOT_STATE_HOLD           = 2,   /* mandatory wait before antenna deploy */
    BOOT_STATE_ANTENNA_DEPLOY = 3,
    BOOT_STATE_NOMINAL        = 4,
    BOOT_STATE_ERROR          = 5
} BootState_t;

typedef enum {
    BOOT_OK              = 0,
    BOOT_ERR_WRONG_STATE = -1,
    BOOT_ERR_HOLD_ACTIVE = -2,
    BOOT_ERR_ALREADY_DONE= -3
} BootStatus_t;

/* ── Fixture ─────────────────────────────────────────────────────────────── */

void setUp(void)    {}
void tearDown(void) {}

/* ── Tests ───────────────────────────────────────────────────────────────── */

void test_initial_state_is_power_on(void)
{
    /* After boot_sequence_init(), state must be POWER_ON. */
    TEST_IGNORE_MESSAGE("TODO: implement when boot_sequence.c exists");
}

void test_hw_init_transitions_from_power_on(void)
{
    /* Running HW init from POWER_ON must advance to HW_INIT then HOLD. */
    TEST_IGNORE_MESSAGE("TODO: implement when boot_sequence.c exists");
}

void test_antenna_deploy_blocked_during_hold(void)
{
    /* Attempting deployment before the hold period expires must fail. */
    TEST_IGNORE_MESSAGE("TODO: implement when boot_sequence.c exists");
}

void test_antenna_deploy_allowed_after_hold_expires(void)
{
    /* Once the hold timer has elapsed, deployment must be permitted. */
    TEST_IGNORE_MESSAGE("TODO: implement when boot_sequence.c exists");
}

void test_antenna_deploy_not_repeated_if_already_deployed(void)
{
    /* Calling deploy a second time must return BOOT_ERR_ALREADY_DONE. */
    TEST_IGNORE_MESSAGE("TODO: implement when boot_sequence.c exists");
}

void test_state_reaches_nominal_after_successful_deploy(void)
{
    /* After a successful deployment the state must be NOMINAL. */
    TEST_IGNORE_MESSAGE("TODO: implement when boot_sequence.c exists");
}

void test_hw_init_failure_enters_error_state(void)
{
    /* A subsystem init failure during HW_INIT must set state to ERROR. */
    TEST_IGNORE_MESSAGE("TODO: implement when boot_sequence.c exists");
}

void test_hold_duration_is_at_least_30_minutes(void)
{
    /* The configured hold duration must be >= 1800 seconds. */
    TEST_IGNORE_MESSAGE("TODO: implement when boot_sequence.c exists");
}

/* ── Runner ──────────────────────────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_initial_state_is_power_on);
    RUN_TEST(test_hw_init_transitions_from_power_on);
    RUN_TEST(test_antenna_deploy_blocked_during_hold);
    RUN_TEST(test_antenna_deploy_allowed_after_hold_expires);
    RUN_TEST(test_antenna_deploy_not_repeated_if_already_deployed);
    RUN_TEST(test_state_reaches_nominal_after_successful_deploy);
    RUN_TEST(test_hw_init_failure_enters_error_state);
    RUN_TEST(test_hold_duration_is_at_least_30_minutes);

    return UNITY_END();
}