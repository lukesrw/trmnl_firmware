#include "stored_logs.h"
#include "config.h"
#include "trmnl_log.h"

void storeLog(const char* log_buffer, size_t size, Preferences& preferences) {
    /**
     * Attempt to write to the next available preference key.
     */
    for (uint8_t index = 0; index < LOG_MAX_NOTES_NUMBER; index++) {
        const char* preferenceKey = (PREFERENCES_LOG_KEY + String(index)).c_str();
        if (preferences.isKey(preferenceKey)) {
            continue;
        }

        size_t sizeWritten = preferences.putString(preferenceKey, log_buffer);
        Log_info(
            "Writing [%s], Size [%d]: [%s]",
            preferenceKey, size, sizeWritten == size ? "Success" : "Fail"
        );
        return;
    }

    /**
     * Get the next available overwrite index.
     */
    uint8_t overwriteHead = 0;
    if (preferences.isKey(PREFERENCES_LOG_BUFFER_HEAD_KEY)) {
        overwriteHead = preferences.getUChar(PREFERENCES_LOG_BUFFER_HEAD_KEY, 0);
    }

    /**
     * Overwrite the next available preference key.
     */
    const char* preferenceKey = (PREFERENCES_LOG_KEY + String(overwriteHead)).c_str();
    size_t sizeWritten = preferences.putString(preferenceKey, log_buffer);
    Log_info(
        "Overwriting [%s], Size [%d]: [%s]",
        preferenceKey, size, sizeWritten == size ? "Success" : "Fail"
    );

    /**
     * Update the next available overwrite index.
     */
    overwriteHead++;
    if (overwriteHead >= LOG_MAX_NOTES_NUMBER) {
        overwriteHead = 0;
    }
    uint8_t sizeWritten = preferences.putUChar(PREFERENCES_LOG_BUFFER_HEAD_KEY, overwriteHead);
    Log_info(
        "Head Writing: [%s]",
        sizeWritten ? "Success" : "Fail"
    );
}

void getStoredLogs(String& concatenatedLogs, Preferences& preferences) {
    for (uint8_t index = 0; index < LOG_MAX_NOTES_NUMBER; index++) {
        const char* preferenceKey = (PREFERENCES_LOG_KEY + String(index)).c_str();

        /**
         * Skip logs that don't exist or are empty.
         */
        if (!preferences.isKey(preferenceKey)) {
            continue;
        }
        String log = preferences.getString(preferenceKey, "");
        if (log.length() == 0) {
            continue;
        }

        /**
         * Append to the concatenated logs, prefix "," if needed.
         */
        if (concatenatedLogs.length() > 0) {
            concatenatedLogs += ",";
        }
        concatenatedLogs += log;
    }
}

void removeStoredLogs(Preferences& preferences) {
    for (uint8_t i = 0; i < LOG_MAX_NOTES_NUMBER; i++) {
        const char* preferenceKey = (PREFERENCES_LOG_KEY + String(i)).c_str();
        if (!preferences.isKey(preferenceKey)) {
            continue;
        }

        bool isNoteRemoved = preferences.remove(preferenceKey);
        if (!isNoteRemoved) {
            Log_info(
                "Removing [%s]: Fail",
                preferenceKey
            );
        }
    }
}