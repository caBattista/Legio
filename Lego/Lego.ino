#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include "web.cpp"
#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server (80);
WebSocketsServer webSocket = WebSocketsServer(85);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

    switch(type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            webSocket.sendTXT(num, "Connected");
        }
            break;
        case WStype_TEXT:
            USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);
            // decode data packet
            uint32_t pwm = (uint32_t) strtol((const char *) &payload[0], NULL, 16);
            USE_SERIAL.println(pwm);
            analogWrite(2, pwm*2);
            break;
    }
}

void setup() {
  pinMode(2, OUTPUT);
  USE_SERIAL.begin(74880);
    delay(500);//Serial will not work if no delay
    //USE_SERIAL.setDebugOutput(true);
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println("Trying to connect to Router");
    
    WiFiMulti.addAP("TP-LINK_8B1E", "94077278");
    //WiFiMulti.addAP("WLAN-B52287", "4294206527437190");
    WiFi.mode(WIFI_STA);
    //WiFi.softAP("Lego");
    //IPAddress myIP = WiFi.softAPIP();
    
    uint8_t i = 0;
    while((WiFiMulti.run() != WL_CONNECTED)&&(i<5)) {
        delay(1000);
        USE_SERIAL.println(i);
        i++;
    }

    if(i>4){
      USE_SERIAL.println("Could not connect to Router");
      }

    // start webSocket server
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    // handle index

    server.on("/", []() {
        // send index.html
        server.send(200, "text/html", _index);//html code is sent here
    });

    server.begin();

    String ip;
    
    if(i>4){
      ip = WiFi.softAPIP().toString();
      }
    else{
      ip = WiFi.localIP().toString();
      }
    Serial.println(ip);

}

void loop() {
  webSocket.loop();
  server.handleClient();
}
