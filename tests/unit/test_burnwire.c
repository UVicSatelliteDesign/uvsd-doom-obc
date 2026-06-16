/**
 * @file  test_burnwire.c
 * @brief Unit tests for OBC Requirement 5:
 *        "Enable burnwire to detach from balloon and deploy parachute when
 *         a command is received or predefined conditions are met."
 *
 * Two trigger mechanisms are tested:
 *   A) Command triggered  — deployment initiated by a command from TTC
 *   B) Condition triggered — deployment initiated by predefined conditions
 *                            (e.g. altitude, time, sensor threshold)
 *
 * OPEN QUESTION: Confirm the predefined conditions for autonomous deployment
 * with the electrical and systems teams before implementing burnwire.c.
 *
 * NOTE: Placeholder types should be defined in burnwire.h and included
 * here once that file is written.
 *
 * TODO: #include "burnwire.h" once the module is written.
 */

#include "../unity/unity.h"

/* ── Fixture ─────────────────────────────────────────────────────────────── */

void setUp(void)    {}
void tearDown(void) {}

/* ══ A) Command triggered ═════════════════════════════════════════════════ */

void test_deploy_command_triggers_burnwire(void)
{
    /* A valid deploy command received from TTC must activate the burnwire. */
    TEST_IGNORE_MESSAGE("TODO: implement when burnwire.c exists");
}

void test_deploy_command_only_fires_once(void)
{
    /* A second deploy command after successful deployment must be ignored. */
    TEST_IGNORE_MESSAGE("TODO: implement when burnwire.c exists");
}

void test_invalid_command_does_not_trigger_burnwire(void)
{
    /* A malformed or unrecognised command must not activate the burnwire. */
    TEST_IGNORE_MESSAGE("TODO: implement when burnwire.c exists");
}

/* ══ B) Condition triggered ═══════════════════════════════════════════════ */

void test_predefined_condition_triggers_burnwire(void)
{
    /* When predefined conditions are met, the burnwire must activate
     * automatically without a command from TTC.
     * OPEN QUESTION: confirm exact conditions with systems team. */
    TEST_IGNORE_MESSAGE("TODO: implement when burnwire.c exists");
}

void test_burnwire_not_triggered_before_conditions_met(void)
{
    /* The burnwire must not activate while conditions are below threshold. */
    TEST_IGNORE_MESSAGE("TODO: implement when burnwire.c exists");
}

void test_condition_trigger_only_fires_once(void)
{
    /* Autonomous deployment must not re-trigger once already fired. */
    TEST_IGNORE_MESSAGE("TODO: implement when burnwire.c exists");
}

/* ── Runner ──────────────────────────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    /* Command triggered */
    RUN_TEST(test_deploy_command_triggers_burnwire);
    RUN_TEST(test_deploy_command_only_fires_once);
    RUN_TEST(test_invalid_command_does_not_trigger_burnwire);

    /* Condition triggered */
    RUN_TEST(test_predefined_condition_triggers_burnwire);
    RUN_TEST(test_burnwire_not_triggered_before_conditions_met);
    RUN_TEST(test_condition_trigger_only_fires_once);

    return UNITY_END();
}