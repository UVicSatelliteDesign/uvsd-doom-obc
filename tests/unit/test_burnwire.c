/**
 * @file  test_burnwire.c
 * @brief Unit tests for OBC Requirement 5:
 *        "Enable burnwire to detach from balloon and deploy parachute when
 *         a command is received or predefined conditions are met."
 *
 * Two trigger mechanisms are tested:
 *   A) Command triggered  — deployment initiated by a command from TTC
 *   B) Condition triggered — deployment initiated by predefined conditions
 *                            (altitude or time since launch)
 * and how the burn ends:
 *   C) Burnwire switched off — by the long timer, or by the short timer once
 *                              the deployment switches release (issue #25)
 *
 * OPEN QUESTION: BURNWIRE_ALTITUDE_LIMIT and BURNWIRE_TIME_LIMIT in
 * burnwire.h are estimates (issue #25); confirm them with the mech team.
 *
 * burnwire.c is linked against the fakes below instead of obc_interface.c and
 * telemetry_sensors.c. Flight time follows the task.h mock's tick count, so
 * each 1 s vTaskDelay in the burnwire task advances the simulated flight 1 s.
 */

#include "../unity/unity.h"
#include "main.h"
#include "burnwire.h"
#include "obc_interface.h"
#include "telemetry_sensors.h"

/* ── Burnwire module state, cleared before every test ───────────────────── */

extern volatile uint8_t trigger_burnwire;
extern volatile uint8_t deployment_switches;
extern volatile uint8_t burnwire_started;
extern volatile uint8_t burnwire_done;

/* ── Simulated flight ────────────────────────────────────────────────────── */

static float    launch_altitude;   /* m   */
static float    ascent_rate;       /* m/s */

static uint8_t  command_pending;   /* deliver command_message at command_time */
static uint32_t command_time;      /* s   */
static uint32_t command_message;

/* What the burnwire task did */
static uint8_t  burnwire_pin;      /* 1 while the burnwire output is on */
static uint32_t pin_set_count;
static uint32_t long_timer_starts;
static uint32_t short_timer_starts;
static uint32_t pin_set_time;      /* flight time (s) when the pin was set */

static uint32_t elapsed_seconds(void)
{
    return mock_task_tick_count / configTICK_RATE_HZ;
}

/* Has the command task deliver `message` once the flight reaches `time` s. */
static void schedule_command(uint32_t time, uint32_t message)
{
    command_pending = 1;
    command_time    = time;
    command_message = message;
}

/* Runs the burnwire task until it deletes itself (the vTaskDelete mock returns). */
static void run_burnwire_task(void)
{
    burnwire_parachute(NULL);
}

/* ── Fakes for telemetry_sensors.c and obc_interface.c ───────────────────── */

float read_altimeter(void)
{
    return launch_altitude + ascent_rate * (float)elapsed_seconds();
}

uint32_t get_flight_time(void)
{
    uint32_t t = elapsed_seconds();

    /* Fail instead of hanging the run if the task never fires. */
    if (t > BURNWIRE_TIME_LIMIT + 60) {
        TEST_FAIL_MESSAGE("burnwire task still polling a minute past the time limit");
    }
    if (command_pending && t >= command_time) {
        command_pending = 0;
        burnwire_command_callback(command_message);
    }
    return t;
}

void set_burnwire_pin(void)
{
    burnwire_pin = 1;
    pin_set_count++;
    pin_set_time = elapsed_seconds();
}

void reset_burnwire_pin(void) { burnwire_pin = 0; }
void start_long_timer(void)   { long_timer_starts++; }
void start_short_timer(void)  { short_timer_starts++; }

/* ── Fixture ─────────────────────────────────────────────────────────────── */

void setUp(void)
{
    mock_task_reset();

    trigger_burnwire    = 0;
    deployment_switches = 0;
    burnwire_started    = 0;
    burnwire_done       = 0;

    launch_altitude   = 0.0f;
    ascent_rate       = 0.0f;
    command_pending   = 0;
    burnwire_pin      = 0;
    pin_set_count     = 0;
    long_timer_starts  = 0;
    short_timer_starts = 0;
    pin_set_time       = 0;
}

void tearDown(void) {}

/* ══ A) Command triggered ═════════════════════════════════════════════════ */

void test_deploy_command_triggers_burnwire(void)
{
    /* A valid deploy command received from TTC must activate the burnwire. */
    ascent_rate = 5.0f;                       /* 3000 m at 600 s, far below the limit */
    schedule_command(600, REQUEST | BURNWIRE);

    run_burnwire_task();

    TEST_ASSERT_EQUAL_UINT8(1, burnwire_pin);
    TEST_ASSERT_EQUAL_UINT32(1, long_timer_starts);
    /* Fired on the next 1 s poll, not at an altitude or time limit */
    TEST_ASSERT_UINT32_WITHIN(1, 600, pin_set_time);
}

void test_deploy_command_only_fires_once(void)
{
    /* A second deploy command after successful deployment must be ignored. */
    burnwire_command_callback(REQUEST | BURNWIRE);
    run_burnwire_task();
    long_timer_callback();                    /* burn finished, burnwire off */

    burnwire_command_callback(REQUEST | BURNWIRE);

    TEST_ASSERT_EQUAL_UINT32(1, pin_set_count);
    TEST_ASSERT_EQUAL_UINT32(1, long_timer_starts);
    TEST_ASSERT_EQUAL_UINT8(0, burnwire_pin);
}

void test_invalid_command_does_not_trigger_burnwire(void)
{
    /* A malformed or unrecognised command must not activate the burnwire. */
    const uint32_t invalid[] = {
        0,                                    /* empty message */
        REQUEST,                              /* no peripheral */
        BURNWIRE,                             /* no message type */
        INFO | BURNWIRE,                      /* burnwire status, not a request */
        WARNING | BURNWIRE,
        REQUEST | DEPLOYMENT_SWITCHES,        /* request for another peripheral */
    };
    for (size_t i = 0; i < sizeof invalid / sizeof invalid[0]; i++) {
        burnwire_command_callback(invalid[i]);
    }

    run_burnwire_task();                      /* altitude stays at 0 m */

    /* Only the time limit fired it; none of the commands did */
    TEST_ASSERT_EQUAL_UINT32(1, pin_set_count);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT32(BURNWIRE_TIME_LIMIT, pin_set_time);
}

/* ══ B) Condition triggered ═══════════════════════════════════════════════ */

void test_predefined_condition_triggers_burnwire(void)
{
    /* When predefined conditions are met, the burnwire must activate
     * automatically without a command from TTC. */
    ascent_rate = 10.0f;                      /* reaches the limit at 2310 s */

    run_burnwire_task();

    TEST_ASSERT_EQUAL_UINT8(1, burnwire_pin);
    TEST_ASSERT_EQUAL_UINT32(1, long_timer_starts);
    TEST_ASSERT_UINT32_WITHIN(1, BURNWIRE_ALTITUDE_LIMIT / 10, pin_set_time);
}

void test_burnwire_not_triggered_before_conditions_met(void)
{
    /* The burnwire must not activate while conditions are below threshold. */
    launch_altitude = 100.0f;
    ascent_rate     = 5.0f;                   /* 16600 m at the time limit */

    run_burnwire_task();

    /* Below both limits for the whole ascent, so the first burn is at the time limit */
    TEST_ASSERT_EQUAL_UINT32(1, pin_set_count);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT32(BURNWIRE_TIME_LIMIT, pin_set_time);
    TEST_ASSERT_LESS_OR_EQUAL_UINT32(BURNWIRE_TIME_LIMIT + 1, pin_set_time);
}

void test_condition_trigger_only_fires_once(void)
{
    /* Autonomous deployment must not re-trigger once already fired. */
    launch_altitude = BURNWIRE_ALTITUDE_LIMIT + 500.0f;   /* above the limit and staying there */

    run_burnwire_task();
    long_timer_callback();                    /* burn finished, burnwire off */
    burnwire_command_callback(REQUEST | BURNWIRE);

    TEST_ASSERT_EQUAL_UINT32(1, pin_set_count);
    TEST_ASSERT_EQUAL_UINT32(1, long_timer_starts);
    TEST_ASSERT_EQUAL_UINT8(0, burnwire_pin);
}

/* ══ C) Burnwire switched off ════════════════════════════════════════════ */

void test_switch_release_during_burn_ends_it_on_short_timer(void)
{
    /* Once the burn has started, the deployment switches releasing start the
     * short timer, which switches the burnwire off before the long timer. */
    burnwire_command_callback(REQUEST | BURNWIRE);
    run_burnwire_task();

    deployment_switches_callback();
    TEST_ASSERT_EQUAL_UINT32(1, short_timer_starts);
    short_timer_callback();

    TEST_ASSERT_EQUAL_UINT8(0, burnwire_pin);
}

void test_switch_release_before_burn_does_not_leave_burnwire_on(void)
{
    /* Switches releasing before the burn (e.g. the balloon bursts on its own)
     * must not stop the long timer switching the burnwire off later. */
    deployment_switches_callback();
    if (short_timer_starts > 0) {
        short_timer_callback();               /* a started short timer runs out */
    }

    burnwire_command_callback(REQUEST | BURNWIRE);
    run_burnwire_task();
    TEST_ASSERT_EQUAL_UINT8(1, burnwire_pin);

    long_timer_callback();

    TEST_ASSERT_EQUAL_UINT8(0, burnwire_pin);
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

    /* Burnwire switched off */
    RUN_TEST(test_switch_release_during_burn_ends_it_on_short_timer);
    RUN_TEST(test_switch_release_before_burn_does_not_leave_burnwire_on);

    return UNITY_END();
}
