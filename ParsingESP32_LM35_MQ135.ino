#include <SoftwareSerial.h>

SoftwareSerial Arduino(34, 35); //RX tX

String dataMonitoring[10];
String infoData;

bool parsing = false;

void setup(){
  Serial.begin(9600);
  Arduino.begin(9600);
}
void loop(){
  //String dataMonitoring ="";
  if (Arduino.available() > 0){
    infoData = Arduino.readString();
    parsing = true;
  }
  if(parsing){
    Arduino.println("Data Monitoring: " + infoData);

    int sensor = 0;

    for(int i = 0; i < 2; i++){
      dataMonitoring[i] = "";
    }

    for(int i = 0; i < infoData.length(); i++){
      if(infoData[i] == '#'){
        sensor++; dataMonitoring[sensor] = "";
      } else{
        dataMonitoring[sensor] += infoData[i];
      }
    }
    Serial.println("Pembacaan MQ135: "  + dataMonitoring[0]);
    Serial.println("Pembacaan LM35 : "  + dataMonitoring[1]);
    parsing = false;
  }
}