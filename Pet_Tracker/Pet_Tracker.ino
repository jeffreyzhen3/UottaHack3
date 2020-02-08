#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

//Arduino Pins           ADXL345 Pins
//
//SCL             ———->      SCL
//
//SDA              ———->      SDA
//
//5V                ———->      VIN
//
//GND             ———->      GND

//Arduino Pins           Bluetooth Pins
//
//RX (Pin 0)     ———->      TX
//
//TX (Pin 1)      ———->      RX
//
//5V                ———->      VCC
//
//GND             ———->      GND

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
int steps, flag;
int threshold = 60.0;

int xAxis, yAxis, zAxis;

int xAvg, yAvg, zAvg;

int xVal[15] = {0}, yVal[15] = {0}, zVal[15] = {0};

 
void setup() {
  Wire.begin();
  Serial.begin(9600);
  
}
void loop(void)
{ 
    
    if (Serial.available())
    {
    
      steps = ArduinoPedometer();
      Serial.println(steps);
      //It appears that delay is needed in order not to clog the port
      delay(100);
      Serial.read();  // clear the input buffer
    }
}
  
int ArduinoPedometer(){
  sensors_event_t accelEvent;
  accel.getEvent(&accelEvent);
  int totVect[15] = {0}, totAvg[15] = {0}, xAcc[15] = {0}, yAcc[15] = {0}, zAcc[15] = {0};
  for (int i=0;i<15;i++)
    {
      xAcc[i]= accelEvent.acceleration.x;
      delay(1);
      yAcc[i]= accelEvent.acceleration.y;
      delay(1);
      zAcc[i]= accelEvent.acceleration.z;
      delay(1);
      totVect[i] = sqrt(((xAcc[i]-xAvg)* (xAcc[i]-xAvg))+ ((yAcc[i] - yAvg)*(yAcc[i] - yAvg)) + ((zVal[i] - zAvg)*(zVal[i] - zAvg)));
      totAvg[i] = (totVect[i] + totVect[i-1]) / 2 ;
      delay(150);
  
      //cal steps 
      if (totAvg[i]>threshold && flag==0)
      {
         steps=steps+1;
         flag=1;
      }
      else if (totAvg[i] > threshold && flag==1)
      {
          //do nothing 
      }
      if (totAvg[i] < threshold  && flag==1)
      {
        flag=0;
      }
     // Serial.print("steps=");
     // Serial.println(steps);
     return(steps);
    }
} 
