#include <filesystem.h>
#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoLog.h>

/**
 * @brief Function to init the filesystem
 * @param none
 * @return bool result
 */
bool fileSystemInit(void) {
    if (SPIFFS.begin(true)) {
        return true;
    }

    Log.fatal(
        "%s [%d]: Failed to mount SPIFFS\r\n",
        __FILE__, __LINE__
    );
    ESP.restart();

    return false;
}

/**
 * @brief Function to de-init the filesystem
 * @param none
 * @return none
 */
void fileSystemDeInit(void) {
    SPIFFS.end();
}

/**
 * @brief Function to read data from file
 * @param name filename
 * @param out_buffer pointer to output buffer
 * @return result - true if success; false - if failed
 */
bool fileSystemReadFile(const char* fileName, uint8_t* out_buffer, size_t size) {
    if (!SPIFFS.exists(fileName)) {
        Log.error(
            "%s [%d]: File [%s] doesn't exists\r\n",
            __FILE__, __LINE__, fileName
        );
        return false;
    }

    File file = SPIFFS.open(fileName, FILE_READ);
    if (!file) {
        Log.error(
            "%s [%d]: File [%s] Open Error\r\n",
            __FILE__, __LINE__, fileName
        );
        return false;
    }

    file.readBytes((char*)out_buffer, size);
    return true;
}

/**
 * @brief Function to write data to file
 * @param name filename
 * @param in_buffer pointer to input buffer
 * @param size size of the input buffer
 * @return size of written bytes
 */
size_t fileSystemWriteFile(const char* fileName, uint8_t* in_buffer, size_t size) {
    uint32_t SPIFFS_freeBytes = (SPIFFS.totalBytes() - SPIFFS.usedBytes());
    Log.info(
        "%s [%d]: SPIFFS Free Bytes - [%d]\r\n",
        __FILE__, __LINE__, SPIFFS_freeBytes
    );

    if (SPIFFS.exists(fileName)) {
        Log.info(
            "%s [%d]: file %s exists. Deleting...\r\n",
            __FILE__, __LINE__, fileName
        );

        bool isFileRemoved = SPIFFS.remove(fileName);
        if (!isFileRemoved) {
            Log.info(
                "%s [%d]: file %s deleting failed\r\n",
                __FILE__, __LINE__, fileName
            );
        }
    } else {
        Log.info(
            "%s [%d]: file %s not exists.\r\n",
            __FILE__, __LINE__, fileName
        );
    }

    delay(100);

    Log.error(
        "%s [%d]: Pre-Open - Free Heap [%d], Free Alloc Heap [%d]\r\n",
        __FILE__, __LINE__, ESP.getFreeHeap(), ESP.getMaxAllocHeap()
    );

    File file = SPIFFS.open(fileName, FILE_WRITE);
    if (!file) {
        Log.error(
            "%s [%d]: File open ERROR\r\n",
            __FILE__, __LINE__
        );
        return 0;
    }

    Log.error(
        "%s [%d]: Post-Open - Free Heap [%d], Free Alloc Heap [%d]\r\n",
        __FILE__, __LINE__, ESP.getFreeHeap(), ESP.getMaxAllocHeap()
    );

    // Write the buffer in chunks
    size_t bytesWritten = 0;
    while (bytesWritten < size) {
        size_t diff = size - bytesWritten;
        size_t chunkSize = _min(4096, diff);
        uint16_t res = file.write(in_buffer + bytesWritten, chunkSize);

        if (res != chunkSize) {
            file.close();

            bool isFormatted = SPIFFS.format();

            Log.error(
                "%s [%d]: Erasing SPIFFS: [%s]\r\n",
                __FILE__, __LINE__, isFormatted ? "Success" : "Fail"
            );

            return bytesWritten;
        }

        bytesWritten += chunkSize;
    }

    Log.info(
        "%s [%d]: Writing File [%s]: Success [%d] Bytes\r\n",
        __FILE__, __LINE__, fileName, bytesWritten
    );
    file.close();

    return bytesWritten;
}

/**
 * @brief Function to check if file exists
 * @param name filename
 * @return result - true if exists; false - if not exists
 */
bool fileSystemExists(const char* name) {
    if (!SPIFFS.exists(name)) {
        Log.error(
            "%s [%d]: File [%s]: Doesn't Exist.\r\n",
            __FILE__, __LINE__, name
        );
        return false;
    }

    return true;
}

/**
 * @brief Function to delete the file
 * @param name filename
 * @return result - true if success; false - if failed
 */
bool fileSystemDeleteFile(const char* fileName) {
    if (!SPIFFS.exists(fileName)) {
        return true;
    }

    if (!SPIFFS.remove(fileName)) {
        Log.error(
            "%s [%d]: File %s Deleting Failed.\r\n",
            __FILE__, __LINE__, fileName
        );
        return false;
    }

    return true;
}

/**
 * @brief Function to rename the file
 * @param oldFileName old filename
 * @param newFileName new filename
 * @return result - true if success; false - if failed
 */
bool fileSystemRenameFile(const char* oldFileName, const char* newFileName) {
    if (!SPIFFS.exists(oldFileName)) {
        Log.error(
            "%s [%d]: file %s not exists.\r\n",
            __FILE__, __LINE__, oldFileName
        );
        return false;
    }

    bool isRenamed = SPIFFS.rename(oldFileName, newFileName);
    if (!isRenamed) {
        Log.error(
            "%s [%d]: file %s wasn't renamed.\r\n",
            __FILE__, __LINE__, oldFileName
        );
        return false;
    }

    return true;
}