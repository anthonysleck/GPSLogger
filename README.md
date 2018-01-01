# GPSLogger
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
