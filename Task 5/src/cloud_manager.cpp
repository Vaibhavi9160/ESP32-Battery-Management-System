#include "cloud_manager.h"

#define BLYNK_TEMPLATE_ID "TMPL3Zcq7eI0X"
#define BLYNK_TEMPLATE_NAME "Battery Management System"
#define BLYNK_AUTH_TOKEN "j4aT5WqyT25c0wg32ZA3degHQdlhcTKW"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

void connectCloud()
{
    Serial.println("Connecting WiFi...");

    Blynk.begin("j4aT5WqyT25c0wg32ZA3degHQdlhcTKW", ssid, pass);

    Serial.println("Cloud Connected");
}

void sendTelemetry(String eventName)
{
    Serial.print("Blynk Status: ");
    Serial.println(Blynk.connected());

    if(Blynk.connected())
    {
        Serial.print("Cloud Event Sent: ");
        Serial.println(eventName);

        Blynk.virtualWrite(V0, eventName);
    }
}

void checkConnection()
{
    if(!Blynk.connected())
    {
        Serial.println("Reconnecting Cloud...");
        Blynk.connect();
    }

    Blynk.virtualWrite(V5, WiFi.RSSI());

    Serial.print("Sending RSSI: ");
    Serial.println(WiFi.RSSI());

    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());

    Blynk.run();
}

void sendDashboardData(
    float c1,
    float c2,
    float c3,
    float c4,
    String runtime,
    String risk,
    String diagnostics,
    String recommendation
)
{
    if(Blynk.connected())
    {
        Blynk.virtualWrite(V1, c1);
        Blynk.virtualWrite(V2, c2);
        Blynk.virtualWrite(V3, c3);
        Blynk.virtualWrite(V4, c4);

        Blynk.virtualWrite(V6, runtime);
        Blynk.virtualWrite(V7, risk);
        Blynk.virtualWrite(V8, diagnostics);
        Blynk.virtualWrite(V9, recommendation);

        Serial.println("Dashboard Data Sent");
    }
}
