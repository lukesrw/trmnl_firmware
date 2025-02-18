#pragma once

#include "api-client/httpBegin.h"
#include <WiFiClientSecure.h>

/**
 * Automatically switch between HTTP and HTTPS depending on the URL protocol.
 */
bool httpBegin(HTTPClient& httpClient, const String& url) {
    if (url.startsWith("https://")) {
        WiFiClientSecure clientSecure;

        /**
         * Skip checking certificate chains
         *
         * @todo Remove when TRMNL supports checks
         */
        clientSecure.setInsecure();

        return httpClient.begin(clientSecure, url);
    }

    WiFiClient clientInsecure;

    return httpClient.begin(clientInsecure, url);
}