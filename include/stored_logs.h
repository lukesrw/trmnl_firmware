#include <Preferences.h>

void storeLog(const char* log_buffer, size_t size, Preferences& preferences);

void getStoredLogs(String& log, Preferences& preferences);

void removeStoredLogs(Preferences& preferences);
