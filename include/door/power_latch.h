/* Ideally this would be a class, however, since
 * the power must be latched before the capacitor
 * charges to the P-MOS's threshold voltage, the
 * task is quite time critical. To prevent any uneceassary
 * delays (ex. through object initialization), power 
 * latching is primitively implemented through macros 
 * simply calling digitalWrite().
 */

#include <Arduino.h>

#include <config.h>

#define LATCH_POWER() pinMode(DOOR_POWER_LATCH, OUTPUT); digitalWrite(DOOR_POWER_LATCH, HIGH)
#define UNLATCH_POWER() digitalWrite(DOOR_POWER_LATCH, LOW)