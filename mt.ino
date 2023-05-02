#include <Wire.h>
//definirea bibliotecilor și inițialiarea LCD-ului
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
int display_col = 16;
int display_row = 2;
//declararea obiectului LCD
hd44780_I2Cexp display(0x27,display_col,display_row); 
//definirea bibliotecilor și variabilelor pentru MQ-8
#include <MQUnifiedsensor.h>
#define board "Arduino UNO"
#define Voltage 5
#define AP_MQ8 A0 //Analog input 0 of your arduino
#define type "MQ-8" //MQ8
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ8CleanAir 70   //RS / R0 = 70 ppm  
#define calibration_button 13 //Pin to calibrate your sensor
MQUnifiedsensor MQ8(board, Voltage, ADC_Bit_Resolution, AP_MQ8, type);
//definirea pin-ului pentru KY-38
#define AP_OKY 1
int sum = 0;

const int SampleWindow = 50; unsigned int sample;
void setup() {
  Serial.begin(9600);
  display.begin(display_col,display_row);
  pinMode(AP_MQ8,INPUT);
  pinMode(AP_OKY,INPUT);
  MQ8.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ8.init();
  MQ8.setRL(10);
  display.print("Starting ...");delay(2500);
  /* OB: Se folosește doar la început pentru calibrare
  display.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ8.update(); // Update data, the arduino will read the voltage from the analog pin
    calcR0 += MQ8.calibrate(RatioMQ8CleanAir); Serial.print(".");
  }
  MQ8.setR0(calcR0/10);
  Serial.println("  done!.");*/
  MQ8.setR0(0.91);
}

void loop() {
  MQ8.update(); // Update data, the arduino will read the voltage from the analog pin
  display.clear();
 
  MQ8.setA(976.97); MQ8.setB(-0.688); // hidrogen
  float H2 = MQ8.readSensor();
  display.print("H2: ");
  display.setCursor(0,1);
  display.print(H2);display.print(" ppm");delay(3000);display.clear();

  MQ8.setA(10000000); MQ8.setB(-3.123); // GPL
  float LPG = MQ8.readSensor();
  display.print("LPG: ");
  display.setCursor(0,1);
  display.print(LPG);display.print(" ppm");delay(3000);display.clear();

  MQ8.setA(80000000000000); MQ8.setB(-6.666); // butan
  float CH4 = MQ8.readSensor();
  display.print("CH4: ");
  display.setCursor(0,1);
  display.print(CH4);display.print(" ppm");delay(3000);display.clear();

  MQ8.setA(2000000000000000000); MQ8.setB(-8.074); // monoxid de carbon
  float CO = MQ8.readSensor();
  display.print("CO: ");
  display.setCursor(0,1);
  display.print(CO);display.print(" ppm");delay(3000);display.clear();
  
  MQ8.setA(76101); MQ8.setB(-1.86); // alcool
  float Alcohol = MQ8.readSensor();
  display.print("Alcohol: ");
  display.setCursor(0,1);
  display.print(Alcohol);display.print(" ppm");delay(3000);display.clear();

  unsigned long startMillis = millis();
  float peakTopeak = 0;
  unsigned int signalMax = 0;unsigned int signalMin = 1024;
  
for(int i=0;i<128;i++){
  sum += analogRead(AP_OKY);
  }
float nr = sum/128;

  while( millis()-startMillis<SampleWindow ){
    sample = analogRead(AP_OKY);
    if(sample<1024){
      if(sample>signalMax) signalMax = sample;
      else if(sample<signalMin) signalMin = sample;
    }
  }
  peakTopeak = signalMax-signalMin;//diferenta de semnale
  float db = map(peakTopeak,10,900,35,65);
  display.print("Loudness: ");
  display.setCursor(0,1);
  if( db<=35 ) display.print("Average");
  else         display.print("Loud");
  delay(3000);display.clear();
  sum = 0;
  
  }