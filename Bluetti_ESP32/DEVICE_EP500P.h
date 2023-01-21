#ifndef DEVICE_EP500P_H
#define DEVICE_EP500P_H
#include "Arduino.h"

/* Not implemented yet
enum output_mode {
    STOP = 0,
    INVERTER_OUTPUT = 1,
    BYPASS_OUTPUT_C = 2,
    BYPASS_OUTPUT_D = 3,
    LOAD_MATCHING = 4
};

enum ups_mode {
    CUSTOMIZED = 1,
    PV_PRIORITY = 2,
    STANDARD = 3,
    TIME_CONTROl = 4  
};

enum auto_sleep_mode {
  THIRTY_SECONDS = 2,
  ONE_MINNUTE = 3,
  FIVE_MINUTES = 4,
  NEVER = 5  
};
*/

// { FIELD_NAME, PAGE, OFFSET, SIZE, SCALE (if scale is needed e.g. decimal value, defaults to 0) , ENUM (if data is enum, defaults to 0) , FIELD_TYPE }
static device_field_data_t bluetti_device_state[] = {
  /*Page 0x00 Core */
  {DEVICE_TYPE,               0x00, 0x0A, 7, 0, 0, STRING_FIELD},
  {SERIAL_NUMBER,             0x00, 0x11, 4, 0 ,0, SN_FIELD},
  {ARM_VERSION,               0x00, 0x17, 2, 0, 0, VERSION_FIELD},
  {DSP_VERSION,               0x00, 0x19, 2, 0, 0, VERSION_FIELD},
  {DC_INPUT_POWER,            0x00, 0x24, 1, 0, 0, UINT_FIELD},
  {AC_INPUT_POWER,            0x00, 0x25, 1, 0, 0, UINT_FIELD},
  {AC_OUTPUT_POWER,           0x00, 0x26, 1, 0, 0, UINT_FIELD},
  {DC_OUTPUT_POWER,           0x00, 0x27, 1, 0, 0, UINT_FIELD},
  {POWER_GENERATION,          0x00, 0x29, 1, 1, 0, DECIMAL_FIELD},
  {TOTAL_BATTERY_PERCENT,     0x00, 0x2B, 1, 0, 0, UINT_FIELD},
  {AC_OUTPUT_ON,              0x00, 0x30, 1, 0, 0, BOOL_FIELD},
  {DC_OUTPUT_ON,              0x00, 0x31, 1, 0, 0, BOOL_FIELD},

  /*Page 0x00 Details  //for all scale check needed
  {AC_OUTPUT_MODE,            0x00, 0x46, 1, 0, 0, ENUM_FIELD}, //check size needed!!
  {INTERNAL_AC_VOLTAGE,       0x00, 0x47, 1, 1, 0, DECIMAL_FIELD},
  {INTERNAL_CURRENT_ONE,      0x00, 0x48, 1, 1, 0, DECIMAL_FIELD},
  {INTERNAL_POWER_ONE,        0x00, 0x49, 1, 0, 0, UINT_FIELD},
  {INTERNAL_AC_FREQUENCY,     0x00, 0x4A, 2, 0, 0, DECIMAL_FIELD},
  {INTERNAL_CURRENT_TWO,      0x00, 0x4B, 1, 0, 0, DECIMAL_FIELD},
  {INTERNAL_POWER_TWO,        0x00, 0x4C, 1, 0, 0, UINT_FIELD},
  {AC_INPUT_VOLTAGE,          0x00, 0x4D, 1, 0, 0, DECIMAL_FIELD},
  {INTERNAL_CURRENT_THREE,    0x00, 0x4E, 1, 0, 0, DECIMAL_FIELD},
  {INTERNAL_POWER_THREE,      0x00, 0x4F, 1, 0, 0, UINT_FIELD},
  {AC_INPUT_FREQUENCY,        0x00, 0x50, 1, 2, 0, DECIMAL_FIELD},
  {INTERNAL_DC_INPUT_VOLTAGE, 0x00, 0x56, 1, 1, 0, DECIMAL_FIELD},
  {INTERNAL_DC_INPUT_POWER,   0x00, 0x57, 1, 0, 0, UINT_FIELD},
  {INTERNAL_DC_INPUT_CURRENT, 0x00, 0x88, 1, 1, 0, DECIMAL_FIELD},
  */
  
  /*
  //Page 0x00 Battery Details
  {PACK_NUM_MAX,              0x00, 0x5B, 1, 0, 0, UINT_FIELD },
  */

  /*
  //Page 0x00 Battery Data 
  {PACK_VOLTAGE,              0x00, 0x5C, 1, 2 ,0, DECIMAL_FIELD},
  {PACK_BATTERY_PERCENT,      0x00, 0x5E, 1, 0, 0, UINT_FIELD},
  {PACK_NUM,                  0x00, 0x60, 1, 0, 0, UINT_FIELD},
  {CELL_VOLTAGE,              0x00, 0x69, 16, 2 ,0, DECIMAL_FIELD},
  */

  /*Page 0x00 - Controls 
  {UPS_MODE,                  0x0B, 0xB9, 1, 0, 0, ENUM_FIELD}, //check size needed!!
  {SPLIT_PHASE_ON,            0x0B, 0xBC, 1, 0, 0, BOOL_FIELD},
  {SPLIT_MACHINE_MODE,        0x0B, 0xBD, 1, 0, 0, ENUM_FIELD}, //check size needed!!
  {PACK_NUM,                  0x0B, 0xBE, 1, 0, 0, UINT_FIELD},
  {AC_OUTPUT_ON,              0x0B, 0xBF, 1, 0, 0, BOOL_FIELD},
  {DC_OUTPUT_ON,              0x0B, 0xC0, 1, 0, 0, BOOL_FIELD},
  {GRID_CHARGE_ON,            0x0B, 0xC3, 1, 0, 0, BOOL_FIELD},
  {TIME_CONTROL_ON,           0x0B, 0xC5, 1, 0, 0, BOOL_FIELD},
  {BATTERY_RANGE_START,       0x0B, 0xC7, 1, 0, 0, UINT_FIELD},
  {BATTERY_RANGE_END,         0x0B, 0xC8, 1, 0, 0, UINT_FIELD},
  // 0xD7-0xD9 is the current device time & date without a timezone
  {BLUETOOTH_CONNECTED,       0x0B, 0xDC, 1, 0, 0, BOOL_FIELD},
  self.struct.add_bool_field('bluetooth_connected', 0x0B, 0xDC)
  // 0xDF-0xF0 is the time control programming
   {AUTO_SLEEP_MODE,          0x0B, 0xF5, 1, 0, 0, ENUM_FIELD}, //check size needed!!
  */
  
};

static device_field_data_t bluetti_device_command[] = {
  /*Page 0x00 Core */
  {AC_OUTPUT_ON,      0x0B, 0xBF, 1, 0, 0, BOOL_FIELD}, 
  {DC_OUTPUT_ON,      0x0B, 0xC0, 1, 0, 0, BOOL_FIELD}
};

static device_field_data_t bluetti_polling_command[] = {
  {FIELD_UNDEFINED, 0x00, 0x0A, 0x28 ,0 , 0, TYPE_UNDEFINED},
  {FIELD_UNDEFINED, 0x00, 0x46, 0x15 ,0 , 0, TYPE_UNDEFINED},
  {FIELD_UNDEFINED, 0x0B, 0xB9, 0x3D ,0 , 0, TYPE_UNDEFINED}
};

/*
static device_field_data_t bluetti_polling_command[] = {
  //polling_commands
  {FIELD_UNDEFINED, 0x00, 0x0A, 0x28 ,0 , 0, TYPE_UNDEFINED},
  {FIELD_UNDEFINED, 0x00, 0x46, 0x15 ,0 , 0, TYPE_UNDEFINED},
  {FIELD_UNDEFINED, 0x0B, 0xB9, 0x3D ,0 , 0, TYPE_UNDEFINED},
  //pack_polling_commands
  {FIELD_UNDEFINED, 0x00, 0x5B, 0x25 ,0 , 0, TYPE_UNDEFINED}
};

*/

/*
     # Page 0x00 - Core
        self.struct.add_string_field('device_type', 0x00, 0x0A, 6)
        self.struct.add_sn_field('serial_number', 0x00, 0x11)
        self.struct.add_version_field('arm_version', 0x00, 0x17)
        self.struct.add_version_field('dsp_version', 0x00, 0x19)
        self.struct.add_uint_field('dc_input_power', 0x00, 0x24)
        self.struct.add_uint_field('ac_input_power', 0x00, 0x25)
        self.struct.add_uint_field('ac_output_power', 0x00, 0x26)
        self.struct.add_uint_field('dc_output_power', 0x00, 0x27)
        self.struct.add_decimal_field('power_generation', 0x00, 0x29, 1)  # Total power generated since last reset (kwh)
        self.struct.add_uint_field('total_battery_percent', 0x00, 0x2B)
        self.struct.add_bool_field('ac_output_on', 0x00, 0x30)
        self.struct.add_bool_field('dc_output_on', 0x00, 0x31)

        # Page 0x00 - Details
        self.struct.add_enum_field('ac_output_mode', 0x00, 0x46, OutputMode)
        self.struct.add_decimal_field('internal_ac_voltage', 0x00, 0x47, 1)
        self.struct.add_decimal_field('internal_current_one', 0x00, 0x48, 1)
        self.struct.add_uint_field('internal_power_one', 0x00, 0x49)
        self.struct.add_decimal_field('internal_ac_frequency', 0x00, 0x4A, 2)
        self.struct.add_decimal_field('internal_current_two', 0x00, 0x4B, 1)
        self.struct.add_uint_field('internal_power_two', 0x00, 0x4C)
        self.struct.add_decimal_field('ac_input_voltage', 0x00, 0x4D, 1)
        self.struct.add_decimal_field('internal_current_three', 0x00, 0x4E, 1)
        self.struct.add_uint_field('internal_power_three', 0x00, 0x4F)
        self.struct.add_decimal_field('ac_input_frequency', 0x00, 0x50, 2)
        self.struct.add_decimal_field('internal_dc_input_voltage', 0x00, 0x56, 1)
        self.struct.add_uint_field('internal_dc_input_power', 0x00, 0x57)
        self.struct.add_decimal_field('internal_dc_input_current', 0x00, 0x58, 1)

        # Page 0x00 - Battery Data
        self.struct.add_uint_field('pack_num_max', 0x00, 0x5B)
        self.struct.add_decimal_field('pack_voltage', 0x00, 0x5C, 1)  # Full pack voltage
        self.struct.add_uint_field('pack_battery_percent', 0x00, 0x5E)
        self.struct.add_uint_field('pack_num', 0x00, 0x60)
        self.struct.add_decimal_array_field('cell_voltages', 0x00, 0x69, 16, 2)

        # Page 0x0B - Controls
        self.struct.add_enum_field('ups_mode', 0x0B, 0xB9, UpsMode)
        self.struct.add_bool_field('split_phase_on', 0x0B, 0xBC)
        self.struct.add_enum_field('split_phase_machine_mode', 0x0B, 0xBD, MachineAddress)
        self.struct.add_uint_field('pack_num', 0x0B, 0xBE)
        self.struct.add_bool_field('ac_output_on', 0x0B, 0xBF)
        self.struct.add_bool_field('dc_output_on', 0x0B, 0xC0)
        self.struct.add_bool_field('grid_charge_on', 0x0B, 0xC3)
        self.struct.add_bool_field('time_control_on', 0x0B, 0xC5)
        self.struct.add_uint_field('battery_range_start', 0x0B, 0xC7)
        self.struct.add_uint_field('battery_range_end', 0x0B, 0xC8)
        # 0xD7-0xD9 is the current device time & date without a timezone
        self.struct.add_bool_field('bluetooth_connected', 0x0B, 0xDC)
        # 0xDF-0xF0 is the time control programming
        self.struct.add_enum_field('auto_sleep_mode', 0x0B, 0xF5, AutoSleepMode)

        super().__init__(address, 'EP500P', sn)

    @property
    def polling_commands(self) -> List[QueryRangeCommand]:
        return [
            QueryRangeCommand(0x00, 0x0A, 0x28),
            QueryRangeCommand(0x00, 0x46, 0x15),
            QueryRangeCommand(0x0B, 0xB9, 0x3D),
        ]

    @property
    def pack_polling_commands(self) -> List[QueryRangeCommand]:
        return [QueryRangeCommand(0x00, 0x5B, 0x25)]

    @property
    def logging_commands(self) -> List[QueryRangeCommand]:
        return [
            QueryRangeCommand(0x00, 0x00, 0x46),
            QueryRangeCommand(0x00, 0x46, 0x15),
            QueryRangeCommand(0x0B, 0xB9, 0x3D),
        ]

    @property
    def pack_logging_commands(self) -> List[QueryRangeCommand]:
        return [QueryRangeCommand(0x00, 0x5B, 0x77)]
*/
#endif
