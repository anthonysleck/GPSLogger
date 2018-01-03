/*
   SLECK GPS DataLogger v5.0

   This is a simple GPS Datalogger using an ESP8266 controller, an
   SDCard reader, and a NEO6M GPS board to record GPS positions to
   the SDCard.

   Notes:
    - Be sure to include the LOG.CSV file in the root of the SDCard.
    - Only make changes to the "DIGITAL PIN ASSIGNMENTS"
    - be sure to check https://acrobotic.com/media/wysiwyg/products/esp8266_devkit_horizontal-01.png
   for pin outs of ESP8266-12E board.

   Components
    - ESP8266-12E dev board
    - NMEA6 GPS
    - MICROSDCard breakout board
    - MICROSDCard > 1GB

   Connections
    GPS
      - GPS GND to ESP8266 GND
      - GPS 5V to ESP8266 VIN AKA 5V
      - GPS TX to ESP8266 GPIO0
      - GPS RX to ESP8266 GPIO2
    SDCard Reader
      - Card Reader GND to ESP8266 GND
      - Card Reader VCC to ESP8266 VIN AKA 5v
      - Card Reader MISO to ESP8266 GPIO12
      - Card Reader MOSI to ESP8266 GPIO13
      - Card Reader SCK to ESP8266 GPIO14
      - Card Reader CS to ESP8266 GPIO15
    Accelerometer
      - Accelerometer GND to ESP8266 GND
      - Accelerometer VCC to ESP8266 3V3
      - Accelerometer SCL to ESP8266 GPIO5
      - Accelerometer SDA to ESP8266 GPIO4

    Contact Info
      email anthony.sleck@gmail.com
      web   anthony.sleckconsulting.com

    Changelog
      5.0 - ARS - rewrote sketch to include new sub-routines for reads and writes
      of GPS Sensor data; pdated pin assignments for ESP8266-12E
*/

// includes
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <TinyGPS++.h>
#include <WiFiClient.h>

// digital pin assignments
#define CS          15
#define GPS_RX      0
#define GPS_TX      2
#define GPS_BAUD    9600
#define SERIAL_BAUD 115200

// constants and variables
const char *ssid = "ESPAP";
ESP8266WebServer server(80);
File myDataFile;
SoftwareSerial ss (GPS_RX, GPS_TX);
String webString = "";
TinyGPSPlus gps;

void handleRoot() {
  server.send(200, "text.html", "<h1>You are conected to the Datalogger Server</h1>");
}

void setup() {
  unsigned long timeOut = millis() + 60000;
  while (millis() <= timeOut)
    // serial interface initialization
    Serial.begin(SERIAL_BAUD);

  // enable and configure WiFi access point and server
  Serial.print("Configuring Access Point...");
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Server started...");

  // SDCard initialization
  Serial.print("Initializing SDCard...");
  if (!SD.begin(CS))
  {
    Serial.println("SDCard initialization failed... Please check wiring.");
    return;
  }
  Serial.println("SDCard initialized sucessfully!");

  ss.begin(GPS_BAUD);
}

void loop() {
  // start GPS data Read/Write
  while (ss.available() > 0)
    gpsInfo(); // read/write GPS sensor data

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected; check wiring!"));
    while (true);
  }
}

void gpsInfo() {
  // read/write GPS sensor data
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    Serial.print(F(","));
    Serial.print(gps.altitude.feet(), 2);
    Serial.print(F(","));
    Serial.print(gps.speed.mph());
    Serial.print(F(","));
    Serial.print(gps.satellites.value());

    Serial.print("Attempting to open the LOG.CSV file from the SDCard...");
    myDataFile = SD.open("log.csv", FILE_WRITE);
    if (myDataFile)
    {
      // write GPS Sensor data to SDCard
      Serial.println();
      Serial.print("writing Data point to LOG.CSV...");
      myDataFile.print(gps.location.lat(), 6);
      myDataFile.print(",");
      myDataFile.print(gps.location.lng(), 6);
      myDataFile.print(",");
      myDataFile.print(gps.altitude.feet(), 2);
      myDataFile.print(",");
      myDataFile.print(gps.speed.mph());
      myDataFile.print(",");
      myDataFile.print(gps.satellites.value());
      myDataFile.println();
      myDataFile.close();
    }
    else
    {
      Serial.println("Opening LOG.CSV failed to open from SDCard...");
    }
  }
  else
  {
    Serial.print(F("INVALID DATA or GPS cannot see sky!"));
  }

}
