/**
 * @file  test_boot_sequence.c
 * @brief Unit tests for OBC Requirement 2:
 *        "Boot sequence and deployment (antennas)."
 *
 * =============================================================================
 * PROPOSED Boot Sequence State Machine
 * =============================================================================
 * NOTE: This state machine is proposed based on standard CubeSat practice and
 * has not yet been validated with the electrical team. Confirm states and
 * transitions before implementing boot_sequence.c.
 *
 * NOTE: Antenna deployment is not required for the DOOM balloon project as
 * antennas are already deployed at launch. This functionality is intentionally
 * omitted but could be added in the future if this code is adapted for a
 * satellite project.
 *
 * States
 * ------
 *   POWER_ON   Initial state on startup. Hardware has power but nothing
 *              has been initialised yet.
 * *
 *   NOMINAL    Boot complete. OBC is operating normally.
 *
 *   ERROR      A failure occurred during HW_INIT. Normal boot cannot
 *              proceed. A reboot is required to recover.
 *
 * Transitions
 * -----------
 *   POWER_ON on boot_sequence_init() called
 *   POWER_ON  --> NOMINAL  on successful hardware initialisation
 *   POWER_ON  --> ERROR    on any subsystem init failure
 *
 * Rules
 * -----
 *   - ERROR is a terminal state — reboot required to recover
 * =============================================================================
 *
 * TODO: #include "boot_sequence.h" once the module is written.
 */

#include "../unity/unity.h"

/* ── Placeholder types ───────────────────────────────────────────────────── */

typedef enum {
    BOOT_STATE_POWER_ON = 0,
    BOOT_STATE_NOMINAL  = 1,
    BOOT_STATE_ERROR    = 2
} BootState_t;

typedef enum {
    BOOT_OK              =  0,
    BOOT_ERR_WRONG_STATE = -1
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

void test_state_reaches_nominal_after_successful_boot(void)
{
    /* After successful boot the state must be NOMINAL. */
    TEST_IGNORE_MESSAGE("TODO: implement when boot_sequence.c exists");
}

void test_power_on_failure_enters_error_state(void)
{
    /* A subsystem init failure during POWER_ON must set state to ERROR. */
    TEST_IGNORE_MESSAGE("TODO: implement when boot_sequence.c exists");
}

/* ── Runner ──────────────────────────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_initial_state_is_power_on);
    RUN_TEST(test_state_reaches_nominal_after_successful_boot);
    RUN_TEST(test_power_on_failure_enters_error_state);

    return UNITY_END();
}