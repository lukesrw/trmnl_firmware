#pragma once

#include <HTTPClient.h>

bool httpBegin(HTTPClient& https, WiFiClientSecure& client, const String& url);