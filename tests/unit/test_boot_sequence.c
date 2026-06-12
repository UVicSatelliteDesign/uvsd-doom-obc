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
 *
 *   HW_INIT    Peripheral and subsystem initialisation is running.
 *
 *   NOMINAL    Boot complete. OBC is operating normally.
 *
 *   ERROR      A failure occurred during HW_INIT. Normal boot cannot
 *              proceed. A reboot is required to recover.
 *
 * Transitions
 * -----------
 *   POWER_ON --> HW_INIT  on boot_sequence_init() called
 *   HW_INIT  --> NOMINAL  on successful hardware initialisation
 *   HW_INIT  --> ERROR    on any subsystem init failure
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
    BOOT_STATE_HW_INIT  = 1,
    BOOT_STATE_NOMINAL  = 2,
    BOOT_STATE_ERROR    = 3
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

void test_hw_init_transitions_from_power_on(void)
{
    /* Running HW init from POWER_ON must advance to HW_INIT. */
    TEST_IGNORE_MESSAGE("TODO: implement when boot_sequence.c exists");
}

void test_state_reaches_nominal_after_successful_init(void)
{
    /* After successful HW init the state must be NOMINAL. */
    TEST_IGNORE_MESSAGE("TODO: implement when boot_sequence.c exists");
}

void test_hw_init_failure_enters_error_state(void)
{
    /* A subsystem init failure during HW_INIT must set state to ERROR. */
    TEST_IGNORE_MESSAGE("TODO: implement when boot_sequence.c exists");
}

/* ── Runner ──────────────────────────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_initial_state_is_power_on);
    RUN_TEST(test_hw_init_transitions_from_power_on);
    RUN_TEST(test_state_reaches_nominal_after_successful_init);
    RUN_TEST(test_hw_init_failure_enters_error_state);

    return UNITY_END();
}