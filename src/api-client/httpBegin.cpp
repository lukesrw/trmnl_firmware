#pragma once

#include "api-client/httpBegin.h"

/**
 * Automatically switch between HTTP and HTTPS depending on the URL protocol.
 */
bool httpBegin(HTTPClient& https, WiFiClientSecure& client, const String& url) {
    if (url.startsWith("https:")) {
        return https.begin(client, url);
    }

    return https.begin(url);
}