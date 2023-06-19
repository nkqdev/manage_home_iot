//8 CSN
//7 CE
//13 SCK
//11 MOSI
//12 MISO
#include <DHT.h>

#define DHTPIN 3

#define DHTTYPE DHT11
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);
const byte address[6] = "00001";
DHT dht(DHTPIN, DHTTYPE);
float data[3];

void setup() {

  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  dht.begin();
}

void loop() {
  // Serial.println("Hello");
  // const char text[]="Hello";
  // radio.write(&text,sizeof(text));

  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float valueMQ2 = analogRead(A0);  //đọc giá trị điện áp ở chân A0 - chân cảm biến
  float checkGas = 0;
  if (valueMQ2 > 500) {
    checkGas = 1;
  } else {
    checkGas = 0;
  }
  if (isnan(t) || isnan(h)) {
    data[0] = 0;
    data[1] = 0;
  } else {
    data[0] = t;
    data[1] = h;
  }

  data[2] = checkGas;
  radio.write(data, sizeof(data));
  Serial.println(checkGas);  //(value luôn nằm trong khoảng 0-1023)
  Serial.println(String(t));
  Serial.println(String(h));
  delay(1000);
}
