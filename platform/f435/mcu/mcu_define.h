#ifndef MCU_DEFINE_H
#define MCU_DEFINE_H

typedef enum tag_work_mode_t
{
	rover_mode = 0,
	base_mode,
	single_mode,
	autobase_mode
} work_mode_t;

typedef struct tag_SystemInfo_t
{
    // 8-byte members
    double coordinate_lon;
    double coordinate_lat;
    double coordinate_alt;

    // 4-byte members
    float record_leftspace;
    uint8_t gprs_status;
    uint8_t NtripServer_status;
    uint8_t NtripClient_status;
    uint8_t radio_status;
    uint8_t wifi_status;
    work_mode_t work_mode;
    uint8_t NtripServer_IP[4];
    uint8_t NtripClient_IP[4];
    uint8_t wifi_ip[4];

    // Arrays
    char record_name[16]; //ascii
    uint8_t NtripServer_Mountpoint[32];
    uint8_t NtripClient_Mountpoint[32];
    uint8_t wifi_ssid[16];

    // 1-byte members
    unsigned char sw_reset_flag;
    unsigned char power_off_flag;
		bool isCharging;
    unsigned char record_status; // 0-off, 1-on
    uint8_t record_type;  //1-xyz 2-Rinex3.02
    uint8_t record_interval; // 0x00-15min, 0x01-60min, 0x02-120min, 0x04-240min, 0x18- 24hour
    unsigned char record_op;
    uint8_t coordinate_status;  // 0- none, 1-single, 4-fixed, 5- float, 7-fixedcoordinate
    uint8_t satellite_number_used;
    uint8_t satellite_number_track;
    uint8_t radio_mode;
    uint8_t radio_protocol;
    uint8_t radio_channel;
    uint8_t wifi_mode;
    uint8_t ExternalPower;
    uint8_t mcu_operation_flag;
    uint8_t linux_operation_flag;
    bool sync_flag;
} SystemInfo_t;

typedef enum chargeStatus_t
{
	notCharge = 0,
	normalCharge,
	fastCharge,
} chargeStatus_t;

typedef struct BatteryState
{
	uint16_t Percent;
	uint16_t Temp;
	uint16_t Voltage;
	chargeStatus_t chargeStatus;
	bool charge_time_record;
	uint32_t charge_start_time;
} BatteryState, *pBatteryState;


extern SystemInfo_t systemInfo;
extern BatteryState batteryState;
#endif // MCU_DEFINE_H