/*
 * WebSocketServer.ino
 *
 *  Created on: 22.05.2015
 *
 */

#include <Arduino.h>

#ifdef ESP32
 #include <WiFi.h>
 #include <WiFiMulti.h>
 #include <WiFiClientSecure.h>
 WiFiMulti WiFiMulti;
#else 
 #include <ESP8266WiFi.h>
 #include <ESP8266WiFiMulti.h>
 ESP8266WiFiMulti WiFiMulti;
#endif

#include <WebSocketsServer.h>
#include <Hash.h>
#include <ArduinoJson.h>

WebSocketsServer webSocket = WebSocketsServer(81);

#define USE_SERIAL Serial

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    int t = type;
    switch (t) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
				
				// send message to client
				webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
            USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);
            {
                StaticJsonDocument<512> doc;
                DeserializationError err = deserializeJson(doc, payload);
                if (!err) {
                    String sJson;
                    serializeJsonPretty(doc, sJson);
                    webSocket.sendTXT(num, sJson);
              
                    serializeJsonPretty(doc, USE_SERIAL);          
                    }
            // send message to client
            // webSocket.sendTXT(num, "message here");
                webSocket.sendTXT(num, payload);
            }
            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;

        case WStype_BIN:
            USE_SERIAL.printf("[%u] get binary length: %u", num, length);
            hexdump(payload, length);

            // send message to client
            webSocket.sendBIN(num, payload, length);
            break;
    }

}

void setup() {
    // USE_SERIAL.begin(921600);
    USE_SERIAL.begin(115200);

    //Serial.setDebugOutput(true);
    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t =3; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("BreezeHill_50", "0932902190");

    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    webSocket.loop();
}