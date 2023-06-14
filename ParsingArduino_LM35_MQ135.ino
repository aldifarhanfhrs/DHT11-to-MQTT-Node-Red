#include <SoftwareSerial.h>
#include <MQ135.h>

#define LM35 A2
#define MQ135Pin A0
#define RZERO 206.85

float nilaiMQ;
int nilaiLM35;
float temperature;
float voltageLM35;

MQ135 gasSensor = MQ135(MQ135Pin);
SoftwareSerial Arduino(2, 3); //RX TX

void setup(){
  pinMode (MQ135Pin, INPUT);
  pinMode (LM35, INPUT);

  float rzero = gasSensor.getRZero();

  Serial.begin(9600);
  Arduino.begin(9600);
}

void loop(){

  float nilaiMQ = gasSensor.getPPM();
  nilaiLM35 = analogRead(LM35); //data 1
  voltageLM35 = nilaiLM35 * (5000 / 1023);
  temperature = voltageLM35/10;

  String dataSend = String(nilaiMQ) + "#" + String(temperature);

  Serial.println(dataSend);
  Arduino.println(dataSend);
  delay(1000);
}