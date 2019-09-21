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

void funcGPIOWrite(int pin, int v){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, v);
    USE_SERIAL.printf("\nPin%d=%d",pin,v);
    }
int funcGPIORead(int pin){
    int v = digitalRead(pin);
    USE_SERIAL.printf("\nPin%d:%d",pin,v);
    return v;
    }

void parseCmd(uint8_t num, JsonObject obj) {
     String sJson;
     //serializeJson(obj, Serial);
     bool bStatus=true;    //default status true 
     //------------------------------------------------------------------------
     if (obj.containsKey("gpio") && obj.containsKey("v") ) { //{"gpio":2,"v":1} 
        int p = obj["gpio"];
        int v = obj["v"];      
        funcGPIOWrite(p,v); 
        }
     else if (obj.containsKey("gpio") && !obj.containsKey("v") ) { //{"gpio":2}  
        int p = obj["gpio"];
        int v = funcGPIORead(p); 
        obj["val"]=v;
        }
     else if (obj.containsKey("gpios") && obj.containsKey("v") ) {//{"gpios":[2,3,4,5], "v":[0,1,0,1]}
        int size = obj["gpios"].size(); 
        for (int i=0;i<size;i++) {
            int p = obj["gpios"][i];
            int v = obj["v"][i];
            funcGPIOWrite(p,v);    
            }    
        }
     else if (obj.containsKey("gpios") && !obj.containsKey("v") ) {//{"gpios":[2,3,4,5]}
        int size = obj["gpios"].size(); 
        JsonArray vals = obj.createNestedArray("vals");
        for (int i=0;i<size;i++) {
            int p = obj["gpios"][i]; 
            int v = funcGPIORead(p); 
            vals.add(v); 
            }    
        }   
     else if (obj.containsKey("echo") ) { //{"echo":"say something!"} ------------
        //bStatus = true;  
        }
     else 
        bStatus = false;
  
    obj["status"]=bStatus;
    serializeJson(obj, sJson);
    webSocket.sendTXT(num, sJson);  
 }

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    int t = type;
    switch (t) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("\n[%u] Disconnected!\n\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);       
                USE_SERIAL.printf("\n[%u] Connected from %d.%d.%d.%d url: %s", num, ip[0], ip[1], ip[2], ip[3], payload);
				// send message to client
                String sLocalIP = String(WiFi.localIP().toString().c_str());
				webSocket.sendTXT(num, sLocalIP+" Connected!");
                //webSocket.sendPing(num);
            }
            break;
        case WStype_PONG:
            USE_SERIAL.printf("\n[%u] Pong", num);
            break;
        case WStype_TEXT:
            USE_SERIAL.printf("\n[%u] got Text: %s", num, payload);
            {
                StaticJsonDocument<512> doc;
                DeserializationError err = deserializeJson(doc, payload);
                if (err) {
                    String sErr = "Json Err:";
                    sErr += err.c_str();
                    webSocket.sendTXT(num, sErr.c_str());
                    return;
                    }    
                JsonObject root = doc.as<JsonObject>();
                parseCmd(num, root);
            // send message to client
            // webSocket.sendTXT(num, "message here");
            }
            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;

        case WStype_BIN:
            USE_SERIAL.printf("\n[%u] get binary length: %u", num, length);
            hexdump(payload, length);
            // send message to client
            //webSocket.sendBIN(num, payload, length);
            break;
    }
}
/*
struct myfunctions
{ 
  char name[16];
  void (*fn)();
};
// Declare the functions here, or use prototyping
void func_led();
void func_leds();
myfunctions fntable[] = 
{  //123456789ABCDEF
  { "led",  func_led },
  { "leds", func_leds },
};
*/
byte hwaddr[6];
void setup() {
    // USE_SERIAL.begin(921600);
    USE_SERIAL.begin(115200);

    //Serial.setDebugOutput(true);
    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();
#ifdef ESP8266
    int chipid=ESP.getChipId();
    Serial.printf("ESP8266 Chip ID = 0x%0X(...%d)\n", (chipid), chipid%1000);

    WiFi.macAddress(hwaddr);
    uint32_t nodeId = 0;
    nodeId |= hwaddr[2] << 24;  // Big endian (aka "network order"):
    nodeId |= hwaddr[3] << 16;
    nodeId |= hwaddr[4] << 8;
    nodeId |= hwaddr[5];
    Serial.printf("ESP8266 MAC Address(NodeId) = %s(%d) \n", WiFi.macAddress().c_str(), nodeId );
    
#else
    int chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
	Serial.printf("ESP32 Chip ID = 0x%04X\n",(uint16_t)(chipid>>32));//print High 2 byteschip
#endif

    WiFiMulti.addAP("BreezeHill_50", "0932902190");

    int t=0;
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(1000);
        USE_SERIAL.printf("[BOOT] WiFi WAIT %d...\n", t++);        
        }
    USE_SERIAL.printf("\nLocal IP Address: %s\n", WiFi.localIP().toString().c_str());

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    webSocket.loop();
}