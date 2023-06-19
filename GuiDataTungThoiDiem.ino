//NODEMCU -NRF24L01
//D2       CSN
//D4       CE
//3.3      V+
//GND      GND
//D7       MOSI
//D6       MISO
//D5       CSK o SCK


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>



RF24 radio(2, 4);
const byte address[6] = "00001";

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


#include "FirebaseESP8266.h"

#define FIREBASE_HOST "managehome-139b0-default-rtdb.firebaseio.com"

#define FIREBASE_AUTH "GPPC83128Qg0ctjCvGkV5ds7NjOlk2xNTHkt4E4K"



#define WIFI_SSID "My house"
#define WIFI_PASSWORD "11102001"

WiFiClient client;
FirebaseData firebaseData;

String temperatureData[4] = {"0", "0", "0", "0"};

void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Da ket noi WiFi!");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  static unsigned long delayStarttime;
  static unsigned long delayStarttime30;
  static unsigned long delayStarttime45;
  static unsigned long delayStarttime60;
  unsigned long currentTime = millis();
  unsigned long currentTimeOf30m = millis();
  unsigned long currentTimeOf45m = millis();
  unsigned long currentTimeOf60m = millis();

  if (radio.available()) {
    float data[3];
    radio.read(data, sizeof(data));
    int checkData = strncmp(data[0], "NAN"));
    
    Firebase.setString(firebaseData, "Data/checkConnect", int(0));
    Firebase.setString(firebaseData, "Data/temperature", String(data[0]));
    Firebase.setString(firebaseData, "Data/humidity", String(data[1]));
    Firebase.setString(firebaseData, "Data/alarmco2", String(data[2]));
    

    if (currentTime - delayStarttime >= 9000UL) {

      Serial.print(currentTime);
      Serial.println(";up load 15min");
      Firebase.setString(firebaseData, "Data/temp15", temperatureData[0]);
      temperatureData[0] = String(data[0]);
      delayStarttime = currentTime;
    }
    if (currentTimeOf30m - delayStarttime30 >= 18000UL) {

      Serial.print(currentTimeOf30m);
      Serial.println(";up load 30min");
      Firebase.setString(firebaseData, "Data/temp30", temperatureData[1]);
      temperatureData[1] = temperatureData[0];
      delayStarttime30 = currentTimeOf30m;
    }
    if (currentTimeOf45m - delayStarttime45 >= 27000UL) {

      Serial.print(currentTimeOf45m);
      Serial.println(";up load 45min");
      Firebase.setString(firebaseData, "Data/temp45", temperatureData[2]);
      temperatureData[2] = temperatureData[1];
      delayStarttime45 = currentTimeOf45m;
    }
    if (currentTimeOf60m - delayStarttime60 >= 36000UL) {

      Serial.print(currentTimeOf60m);
      Serial.println(";up load 60min");
      Firebase.setString(firebaseData, "Data/temp60", temperatureData[3]);
      temperatureData[3] = temperatureData[2];
      delayStarttime60 = currentTimeOf60m;
    }
    Serial.println(data[0]);
    Serial.println(data[1]);
    Serial.println(data[2]);
  } else {
    Firebase.setString(firebaseData, "Data/checkConnect", int(1));

  }
}
