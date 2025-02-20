#include "api-client/submit_log.h"
#include "api-client/httpBegin.h"
#include "config.h"

bool submitLogToApi(LogApiInput& input) {
    String url = API_LOG_URL + String("/log/");
    bool isHttps = String(API_LOG_URL).startsWith("https://");

    HTTPClient http;
    if (!httpBegin(http, url)) {
        return false;
    }

    const String payload = "{\"log\":{\"logs_array\":[" + String(input.log_buffer) + "]}}";
    http.addHeader("Accept", "application/json");
    http.addHeader("Access-Token", input.api_key);
    http.addHeader("Content-Length", String(payload.length()));
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.POST(payload);
    http.end();

    switch (httpCode) {
        case HTTP_CODE_OK:
        case HTTP_CODE_MOVED_PERMANENTLY:
        case HTTP_CODE_NO_CONTENT:
            return true;

        default:
            return false;
    }
}
