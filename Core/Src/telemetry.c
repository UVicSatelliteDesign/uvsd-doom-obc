/**
 * @file		: telemetry.c
 * @brief		: Implementation of telemetry functions
 */

#include "main.h"
#include "cmsis_os.h"
#include "telemetry_sensors.h"


// Sampling periods (ms) - please review/might change/open to feedback
#define TELEMETRY_BASE_PERIOD_MS		10		// loop tick
#define TELEMETRY_IMU_PERIOD_MS			10		// gyro + accel
#define TELEMETRY_ALT_PERIOD_MS			100		// altimeter
#define TELEMETRY_POWER_PERIOD_MS		100		// currents + voltages
#define TELEMETRY_TEMP_PERIOD_MS		1000	// regulator temps
#define TELEMETRY_SD_FLUSH_PERIOD_MS	1000	// SD card sync

// Number of loop cycles per period
#define TELEMETRY_CYCLES(period_ms)		((period_ms) / TELEMETRY_BASE_PERIOD_MS)

// Every period must be a whole multiple of the base period at compile time
// or else the build fails, this protects against accidental macro changes throughout development
_Static_assert(TELEMETRY_IMU_PERIOD_MS % TELEMETRY_BASE_PERIOD_MS == 0, "IMU period must be a multiple of base period");
_Static_assert(TELEMETRY_ALT_PERIOD_MS % TELEMETRY_BASE_PERIOD_MS == 0, "ALT period must be a multiple of base period");
_Static_assert(TELEMETRY_POWER_PERIOD_MS % TELEMETRY_BASE_PERIOD_MS == 0, "POWER period must be a multiple of base period");
_Static_assert(TELEMETRY_TEMP_PERIOD_MS % TELEMETRY_BASE_PERIOD_MS == 0, "TEMP period must be a multiple of base period");
_Static_assert(TELEMETRY_SD_FLUSH_PERIOD_MS % TELEMETRY_BASE_PERIOD_MS == 0, "SD flush period must be a multiple of base period");

// Latest readings per sensor group
typedef struct {
	vector3f_t gyro;
	vector3f_t accel;
} imu_data_t;

typedef struct {
	float obc_current;
	float nrf_current;
	float slipttc_current;
	float orcattc_current;
	float burnwire_current;
	float obc_voltage;
	float nrf_voltage;
	float slipttc_voltage;
	float orcattc_voltage;
	float burnwire_voltage;
} power_data_t;

typedef struct {
	float reg3v3;
	float reg5v;
	float reg7v7;
} temp_data_t;

static imu_data_t imu_data;
static power_data_t power_data;
static temp_data_t temp_data;
static float altitude;
static uint32_t telemetry_overruns;	// cycles where the loop missed its deadline

static void read_imu(void) {
	imu_data.gyro = read_gyroscope();
	imu_data.accel = read_accelerometer();
}

static void read_power(void) {
	power_data.obc_current = read_obc_current();
	power_data.nrf_current = read_nrf_current();
	power_data.slipttc_current = read_slipttc_current();
	power_data.orcattc_current = read_orcattc_current();
	power_data.burnwire_current = read_burnwire_current();
	power_data.obc_voltage = read_obc_voltage();
	power_data.nrf_voltage = read_nrf_voltage();
	power_data.slipttc_voltage = read_slipttc_voltage();
	power_data.orcattc_voltage = read_orcattc_voltage();
	power_data.burnwire_voltage = read_burnwire_voltage();
}

static void read_temps(void) {
	temp_data.reg3v3 = read_3v3reg_temperature();
	temp_data.reg5v = read_5vreg_temperature();
	temp_data.reg7v7 = read_7v7reg_temperature();
}

void handle_telemetry(void *vpParameters) {
	// Init/ configure sensors (if not done somewhere lese + log any failures)
		
	// Mount + verify SD card (check FRESULT, retry later on failure)

	// Get start tick for vTaskDelayUntil
	TickType_t last_wake = xTaskGetTickCount();
	uint32_t cycle = 0;

	for(;;) {
		// Read sensors whose rate group is due this cycle
		if (cycle % TELEMETRY_CYCLES(TELEMETRY_IMU_PERIOD_MS) == 0) {
			read_imu();
		}
		if (cycle % TELEMETRY_CYCLES(TELEMETRY_ALT_PERIOD_MS) == 0) {
			altitude = read_altimeter();
		}
		if (cycle % TELEMETRY_CYCLES(TELEMETRY_POWER_PERIOD_MS) == 0) {
			read_power();
		}
		if (cycle % TELEMETRY_CYCLES(TELEMETRY_TEMP_PERIOD_MS) == 0) {
			read_temps();
		}

		// format for SD card writes -> not entirely sure might ask artyom
		// Planned layout: one CSV line per group that sampled this cycle, <TAG>,<tick_ms>,<values...>
		//   IMU,<ms>,gx,gy,gz,ax,ay,az                     (deg/s, m/s^2)
		//   ALT,<ms>,altitude                              (m)
		//   PWR,<ms>,obc_i,nrf_i,slip_i,orca_i,bw_i,obc_v,nrf_v,slip_v,orca_v,bw_v   (A, V)
		//   TMP,<ms>,reg3v3,reg5v,reg7v7                   (degC)

		// write lines to SD telemetry file, flush every TELEMETRY_SD_FLUSH_PERIOD_MS
		//    (on FRESULT error, flag + remount next cycle)

		// Detect overrun (work took longer than one cycle), then wait for next cycle
		if (xTaskGetTickCount() - last_wake >= pdMS_TO_TICKS(TELEMETRY_BASE_PERIOD_MS)) {
			telemetry_overruns++;
		}
		vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(TELEMETRY_BASE_PERIOD_MS));
		cycle++;
	}
}
