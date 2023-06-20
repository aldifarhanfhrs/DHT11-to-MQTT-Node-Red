#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTSensorPin 14

#define TIPEDHT DHT11 //terserah mau diisi apa, untuk mendeklarasikan sensor DHT

//reset pin, tidak digunakan, tapi dibutuhkan untuk library
//#define OLED_RESET 4
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DHT dht(DHTSensorPin, TIPEDHT);
//#define TIPEDHT   DHT22     // DHT 22 (AM2302)
//#define TIPEDHT    DHT21     // DHT 21 (AM2301)

float kelembaban;
float suhuC;
float suhuF;

void setup() {
  Serial.begin(9600);

  dht.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop() {
  delay(2000); //delay sebelum pengukuran, agar lebih stabil
  kelembaban = dht.readHumidity(); //membaca kelembaban
  suhuC      = dht.readTemperature(); // membaca suhu default, Celcius
  suhuF      = dht.readTemperature(true); // membaca suhu Fahrenheit, isFahrenheit = true

  //Ngecek jika ada pembacaan yang gagal dan keluar secepatnya (lalu mencoba ulang)
  if (isnan(kelembaban) || isnan(suhuC) || isnan(suhuF)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
    
    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(suhuF, kelembaban);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(suhuC, kelembaban, false);
    

  Serial.print("Kelembaban:");
  Serial.print(kelembaban);
  Serial.print("%\t");
  
  Serial.print("Temperatur (C):");
  Serial.print(suhuC);
  Serial.print("*C  ");
  Serial.print(" ~ (F):");
  Serial.print(suhuF);
  Serial.print("*F\t");
  
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");

  // clear display
  display.clearDisplay();
  
  // display temperature
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.setTextSize(1);
  display.setCursor(0,10);
  display.print(suhuC);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(1);
  display.print("C");
  
  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Humidity: ");
  display.setTextSize(1);
  display.setCursor(0, 45);
  display.print(kelembaban);
  display.print(" %"); 
  
  display.display(); 
}
