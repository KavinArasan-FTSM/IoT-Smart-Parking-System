// secrets.h
// Rename from secrets_example.h after filling in your real values
// Keep this file private and add to .gitignore

#ifndef SECRETS_H
#define SECRETS_H

// Wi-Fi credentials
#define WIFI_SSID       "Your_WiFi_SSID"
#define WIFI_PASSWORD   "Your_WiFi_Password"

// AWS IoT
#define AWS_IOT_ENDPOINT    "Your_AWS_IoT_Endpoint"  // e.g., "abcdef123456.iot.us-east-1.amazonaws.com"
#define THINGNAME           "Your_Thing_Name"

// AWS Certificates (paste content of each cert as a multi-line string)
// CA Certificate
const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
YOUR_CA_CERT_HERE
-----END CERTIFICATE-----
)EOF";

// Device Certificate
const char AWS_CERT_CRT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
YOUR_DEVICE_CERT_HERE
-----END CERTIFICATE-----
)EOF";

// Private Key
const char AWS_CERT_PRIVATE[] PROGMEM = R"EOF(
-----BEGIN PRIVATE KEY-----
YOUR_PRIVATE_KEY_HERE
-----END PRIVATE KEY-----
)EOF";

#endif // SECRETS_H
