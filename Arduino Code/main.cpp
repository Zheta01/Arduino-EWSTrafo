#include <PZEM004Tv30.h>
#include <max6675.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Arduino.h>
#include <MQUnifiedsensor.h>

int MAX6675_MISO = 50;
int MAX6675_SCK = 52;
int chipSelect1 = 2;
int chipSelect2 = 3;
int chipSelect3 = 4;
int chipSelect4 = 5;
int chipSelect5 = 6;

float temp1;
float temp2;
float temp3;
float temp4;
float temp5;

float voltage1;
float current1;
float voltage2;
float current2;
float voltage3;
float current3;

MAX6675 thermo1(MAX6675_SCK, chipSelect1, MAX6675_MISO);
MAX6675 thermo2(MAX6675_SCK, chipSelect2, MAX6675_MISO);
MAX6675 thermo3(MAX6675_SCK, chipSelect3, MAX6675_MISO);
MAX6675 thermo4(MAX6675_SCK, chipSelect4, MAX6675_MISO);
MAX6675 thermo5(MAX6675_SCK, chipSelect5, MAX6675_MISO);

PZEM004Tv30 pzem1(Serial1);
PZEM004Tv30 pzem2(Serial2);
SoftwareSerial pzemSWSerial(11,13);
PZEM004Tv30 pzem3;

#define Board                     ("Arduino MEGA")
#define MQ4_Pin                   (A0)
#define MQ8_Pin                   (A1)

#define RatioMQ4CleanAir          (4.4)
#define RatioMQ8CleanAir          (70)

#define ADC_Bit_Resolution        (10) 
#define Voltage_Resolution        (5) 
#define Type                      ("Arduino MEGA")


MQUnifiedsensor MQ4(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ4_Pin, Type);
MQUnifiedsensor MQ8(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ8_Pin, Type);

String kirimesp;


void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(115200);
  pzem3 = PZEM004Tv30(pzemSWSerial);
  pzemSWSerial.begin(9600);
  MQ4.init();
  MQ4.setRegressionMethod(1);
  MQ4.setR0(14.23);
  MQ8.init();
  MQ8.setRegressionMethod(1);
  MQ8.setR0(0.91);

  Serial.print("Calibrating please wait.");
  float MQ4calcR0 = 0;
  float MQ8calcR0 = 0;
         
  for (int i = 1; i <= 10; i ++){
      MQ4.update();
      MQ8.update();
      MQ4calcR0 += MQ4.calibrate(RatioMQ4CleanAir);
      MQ8calcR0 += MQ8.calibrate(RatioMQ8CleanAir);
      Serial.print(".");
    }
  MQ4.setR0(MQ4calcR0 / 10);
  MQ8.setR0(MQ8calcR0 / 10);
  Serial.println("  done!.");

  Serial.print("(MQ4 & MQ8):");
  Serial.print(MQ4calcR0 / 10); Serial.print(" | ");
  Serial.print(MQ8calcR0 / 10); Serial.print(" | \n");
}


void loop(){
  temp1 = thermo1.readCelsius();
  temp2 = thermo2.readCelsius();
  temp3 = thermo3.readCelsius();
  temp4 = thermo4.readCelsius();
  temp5 = thermo5.readCelsius();


        Serial.print("Suhu Celcius1: ");
        Serial.println(temp1);
        kirimesp += String(""); //thermocouple1
        kirimesp += String(temp1);
        Serial3.print(kirimesp); kirimesp = "";
        //
        Serial.print("Suhu Celcius2: ");
        Serial.println(temp2);
        kirimesp += String(""); //thermocouple2
        kirimesp += String(temp1);
        Serial3.print(kirimesp); kirimesp = "";
        //
        Serial.print("Suhu Celcius3: ");
        Serial.println(temp2);
        kirimesp += String(""); //thermocouple3
        kirimesp += String(temp3);
        Serial3.print(kirimesp); kirimesp = "";
        //
        Serial.print("Suhu Celcius4: ");
        Serial.println(temp4);
        kirimesp += String(""); //thermocouple4
        kirimesp += String(temp4);
        Serial3.print(kirimesp); kirimesp = "";
        //
        Serial.print("Suhu Celcius5: ");
        Serial.println(temp5);
        kirimesp += String(""); //thermocouple5
        kirimesp += String(temp5);
        Serial3.print(kirimesp); kirimesp = "";
     
      MQ4.update();
      MQ8.update();

      MQ4.setA(1012.7); MQ4.setB(-2.786); // set gas CH4
      float CH4 = MQ4.readSensor(); 

      MQ4.setA(30000000); MQ4.setB(-8.308); //set Asap 
      float smoke = MQ4.readSensor(); 

      MQ4.setA(200000000000000); MQ4.setB(-19.05); //set Karbon
      float CO = MQ4.readSensor();

      MQ8.setA(976.97); MQ8.setB(-0.688); //set Hidrogen
      float H2 = MQ8.readSensor();

      //metana MQ4
      Serial.print("Metana: "); Serial.println(CH4);
      kirimesp += String(""); //metana
      kirimesp += String(CH4); Serial3.print(kirimesp);
      kirimesp = "";

      //asap MQ4
      Serial.print("Smoke: "); Serial.println(smoke);
      kirimesp += String(""); //asap
      kirimesp += String(smoke); Serial3.print(kirimesp);
      kirimesp = "";

      //gas karbon MQ4
      Serial.print("Karbon: "); Serial.println(CO);
      kirimesp += String(""); //karbon
      kirimesp += String(CO); Serial3.print(kirimesp);
      kirimesp = "";

      //Hidrogen MQ8
      Serial.print("H2: "); Serial.println(H2);
      kirimesp += String(""); //hidrogen
      kirimesp += String(H2); Serial3.print(kirimesp);
      kirimesp = "";
  

    voltage1 = pzem1.voltage();
      if(!isnan(voltage1)){
        Serial.print("Tegangan1: ");
        Serial.print(voltage1);
        Serial.println("V");
        }
      else{
        Serial.println("Error membaca tegangan1");
      }
      kirimesp += String(""); //tegangan1
      kirimesp += String(voltage1); Serial3.print(kirimesp);
      kirimesp = "";

    current1 = pzem1.current();
    if(!isnan(current1)){
        Serial.print("Arus1: ");
        Serial.print(current1);
        Serial.println("A");
        } 
      else{
        Serial.println("Error membaca arus1");
      }
      kirimesp += String(""); //arus1
      kirimesp += String(current1); Serial3.print(kirimesp);
      kirimesp = "";

    voltage2 = pzem2.voltage();
      if(!isnan(voltage2)){
        Serial.print("Tegangan2: ");
        Serial.print(voltage2);
        Serial.println("V");
      }
      else{
        Serial.println("Error membaca tegangan2");
      }
      kirimesp += String(""); //tegangan2
      kirimesp += String(voltage2); Serial3.print(kirimesp);
      kirimesp = "";

    current2 = pzem2.current();
    if(!isnan(current2)){
        Serial.print("Arus2: ");
        Serial.print(current2);
        Serial.println("A");
        } 
        
    else {
        Serial.println("Error membaca arus2");
        }
      kirimesp += String(""); //arus2
      kirimesp += String(current2); 
      Serial3.print(kirimesp);
      kirimesp = "";


    voltage3 = pzem3.voltage();
      if(!isnan(voltage3)){
        Serial.print("Tegangan3: ");
        Serial.print(voltage3);
        Serial.println("V");
      }
      else{
        Serial.println("Error membaca tegangan3");
      }
      kirimesp += String(""); //tegangan3
      kirimesp += String(voltage3); Serial3.print(kirimesp);
      kirimesp = "";

    current3 = pzem3.current();
      if(!isnan(current3)){
        Serial.print("Arus3: ");
        Serial.print(current3);
        Serial.print("A");
      }
      else{
        Serial.print("Error membaca arus3\n");
      }
      kirimesp += String(""); //arus3 
      kirimesp += String(current3); Serial3.print(kirimesp);
      kirimesp = "";
  
  delay(3000);
}