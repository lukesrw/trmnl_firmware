#include <HTTPClient.h>

/**
 * Automatically switch between HTTP and HTTPS depending on the URL protocol.
 */
bool httpsBegin(HTTPClient& https, WiFiClientSecure& client, const String& url) {
    if (url.startsWith("https:")) {
        return https.begin(client, url);
    }

    return https.begin(url);
}