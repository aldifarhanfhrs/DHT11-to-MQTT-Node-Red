#include <Wire.h> //library buat i2C
#include "DHT.h" //library buat DHT

/*library buat grafik sama OLED*/
//#include <Adafruit_GFX.h> 
//#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);

#define DHTSensorPin 14
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
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
char str[10];

void drawTest(void) {
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr( 0, 15, "Monitoring Kelembaban");
  u8g.drawStr( 0, 30, "dan Suhu Ruangan  ");
  u8g.drawStr( 0, 45, "Loading...  ");
}


void setup() 
{
  Serial.begin(9600);
  dht.begin();
  Serial.println("DHT11 Test");

  dht.begin();
  u8g.firstPage();  
  do {
    drawTest();
  } while( u8g.nextPage() );
   
}

void loop() 
{
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

    // picture loop
  u8g.firstPage();  
  do {
    u8g.setFont(u8g_font_helvB08);
    
    u8g.drawStr( 0, 15, "Kelembaban:");
    u8g.drawStr( 80, 15, dtostrf(kelembaban, 5, 2, str));
    u8g.drawStr( 120, 15, "%");
    
    u8g.drawStr( 0, 30, "Temperatur:");
    u8g.drawStr( 80, 30, dtostrf(suhuC, 5, 2, str));
    u8g.drawStr( 120, 30, "\260C");
    
    u8g.drawStr( 80, 45, dtostrf(suhuF, 5, 2, str));
    u8g.drawStr( 120, 45, "\260F");
    
    u8g.drawStr( 0, 60, "Heat index:");
    u8g.drawStr( 80, 60, dtostrf(hic, 5, 2, str));
    u8g.drawStr( 120, 60, "\260C");
    
    
  } while( u8g.nextPage() );

}
