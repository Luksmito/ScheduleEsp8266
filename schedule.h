
#include <EEPROM.h>

typedef struct schedule {
  uint8_t hour;
  uint8_t minute;
} ScheduleStruct;

class Schedule {
public:
  static ScheduleStruct schedule[7];
  static uint8_t memoryAddressForEachDay[7];

  Schedule();

  static void initializeSchedule() {
    memoryAddressForEachDay[0] = 0x01;
    memoryAddressForEachDay[1] = 0x23;
    memoryAddressForEachDay[2] = 0x34;
    memoryAddressForEachDay[3] = 0x56;
    memoryAddressForEachDay[4] = 0x78;
    memoryAddressForEachDay[5] = 0x9A;
    memoryAddressForEachDay[6] = 0xBC;
    for (int i = 0; i < 7; i++) {
      uint8_t hourAddress = (memoryAddressForEachDay[i] >> 4) & 0x0F;
      uint8_t minuteAddress = memoryAddressForEachDay[i] & 0x0F;

      schedule[i].hour = EEPROM.read(hourAddress);
      schedule[i].minute = EEPROM.read(minuteAddress);
    }
  }
  static void setHourandMinuteForDay(int day, uint8_t hour, uint8_t minute) {
    schedule[day].hour = hour;
    schedule[day].minute = minute;

    uint8_t hourAddress = (memoryAddressForEachDay[day] >> 4) & 0x0F;
    uint8_t minuteAddress = memoryAddressForEachDay[day] & 0x0F;

    EEPROM.write(hourAddress, hour);
    EEPROM.commit();
    

    EEPROM.write(minuteAddress, minute);
    EEPROM.commit();
  
  }
};

ScheduleStruct Schedule::schedule[7];
uint8_t Schedule::memoryAddressForEachDay[7];

