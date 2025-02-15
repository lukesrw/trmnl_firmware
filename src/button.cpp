#include "trmnl_log.h"
#include <config.h>
#include "button.h"

ButtonPressResult onWakeByButton() {
    unsigned long msOfFirstPress = millis();
    bool wasReleased = false;

    Log_info(
        "onWakeByButton: Wake Time (%d)",
        msOfFirstPress
    );

    while (true) {
        int isReleased = digitalRead(PIN_INTERRUPT);
        unsigned long msFromFirstPress = millis() - msOfFirstPress;

        if (isReleased) {
            if (!wasReleased) {
                wasReleased = true;
            } else if (msFromFirstPress > 2e3) {
                Log_info(
                    "onWakeByButton: Position (%d), Time From First Press (%d), Was Released (%d) = No Action",
                    isReleased, msFromFirstPress, wasReleased
                );

                return NoAction;
            }
        } else {
            if (msFromFirstPress > BUTTON_HOLD_TIME) {
                Log_info(
                    "onWakeByButton: Position (%d), Time From First Press (%d), Was Released (%d) = Long Press",
                    isReleased, msFromFirstPress, wasReleased
                );

                return LongPress;
            }

            if (wasReleased) {
                Log_info(
                    "onWakeByButton: Position (%d), Time From First Press (%d), Was Released (%d) = Double Click",
                    isReleased, msFromFirstPress, wasReleased
                );

                return DoubleClick;
            }
        }
    }
}