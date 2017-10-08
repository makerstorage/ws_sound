#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <WebSocketsServer.h>
#include <Hash.h>
//#include <WiFiUdp.h>
#include "ws2812_i2s.h"

// Set to the number of LEDs in your LED strip
#define NUM_LEDS 12
// Maximum number of packets to hold in the buffer. Don't change this.
#define BUFFER_LEN 1024
// Toggles FPS output (1 = print FPS over serial, 0 = disable output)
#define PRINT_FPS 1

// Wifi and socket settings

const char* ssid = "Tilgin-4sATNx42WS4H";
const char* password = "AKpQwcUhvJ6Sk";
 
char* topic = "msled";     //  using wildcard to monitor all traffic from mqtt server
char* server = "192.168.1.16";  // Address of my server on my network, substitute yours!

char message_buff[BUFFER_LEN];   // initialise storage buffer (i haven't tested to this capacity.)
 
WiFiClient wifiClient;
PubSubClient client(wifiClient);


char packetBuffer[BUFFER_LEN];

// LED strip
static WS2812 ledstrip;
static Pixel_t pixels[NUM_LEDS];

String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}


void setup() {
  Serial.begin(115200);
  delay(10);

  client.setServer(server, 1883);
  client.setCallback(callback);  

Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  if (client.connect("ard2")) {
   // client.publish("outTopic","hello world");
    client.subscribe(topic);   // i should probably just point this to the varialbe above.
  }
 
    ledstrip.init(NUM_LEDS);
}

uint8_t N = 0;
#if PRINT_FPS
    uint16_t fpsCounter = 0;
    uint32_t secondTimer = 0;
#endif

void loop() {

  client.loop();
 
}



void callback(char* topic, byte* payload, unsigned int length) {

  // Serial.println("N: " + String(payload[0],HEX));
   
  
  for(int i=0; i<length; i+=4) {
   // message_buff[i] = payload[i];
  //       message_buff[packetSize] = 0;
         N = (uint8_t)payload[i];
 //        Serial.println("N: " + String(N,HEX));
         pixels[N].R = (uint8_t)payload[i+1];
         pixels[N].G = (uint8_t)payload[i+2];
         pixels[N].B = (uint8_t)payload[i+3];
  }
       ledstrip.show(pixels);
 // message_buff[i] = '\0';
  
//  String msgString = String(message_buff);
  

}
