/* use PID gains of Kp = 80, Ki = 25 for LSU 4.2

 DIAG_REG = 0x78 = 01111000
 IDENT_REG = 0x48 = 01001000
 INIT_REG1 RD = 0x6C = 01101100
 INIT_REG1 WR = 0x56 = 01010110
 INIT_REG2 RD = 0x7E = 01111110
 INIT_REG2 WR = 0x5A = 01011010

TODO

*/

#include <stm32f37x.h>
#include <stm32f37x_spi.h>
#include <stm32f37x_gpio.h>
#include "reuse.h"

// define constants that won't change, such as assigning names to pin numbers

const int UR_pin = A0;
const int UA_pin = A1;
const int heater_pin = 3;
const byte chip_select = 10;

// define sampling timers

unsigned long fault_current_millis = 0;
unsigned long fault_previous_millis = 0;
unsigned long fault_interval_millis = 2000;
unsigned long pumpprint_current_millis = 0;
unsigned long pumpprint_previous_millis = 0;
unsigned long pumpprint_interval_millis = 100;

// define variables that will change

double UR_c = 0;
double UA_c = 0;
double UR_m = 0;
double UA_m = 0;
double heater_duty = 0;
int cold_threshold = 990;
double IP = 0;
bool fault = false;
bool cold_start = false;
byte ident_reg_contents = B00000000;
byte diag_reg_contents = B00000000;

PID heater_PID(&UR_m, &heater_duty, &UR_c, 80, 25, 0, DIRECT);

void setup()
{

 /*read calibration values from EEPROM to allow heater PID and pump current
  calculations to function without recalibration or from hot start */
     GPIO_ResetBits( GPIOF, GPIO_Pin_10);
     GPIO_ResetBits( GPIOA, GPIO_Pin_11);

 // identify CJ125
 SPIByte(0x48);
 ident_reg_contents = SPIByte(0);
 if (ident_reg_contents != 0)
 {
   //Serial.println("");
  // Serial.println("");
   //Serial.println("SPI comms successful!");
   //Serial.print("CJ125 Device ID - ");
   //Serial.println(ident_reg_contents, BIN);
 }
 else 
 {
   //Serial.println("SPI comms failed!");
   //Serial.print(ident_reg_contents, BIN);
 }
 
 // enable diagnostics and set reference pump current
 
 SPIByte(0x5a);
 SPIByte(0x12); 

 // read diagnoses registers
 SPIByte(0x78);
 diag_reg_contents = SPIByte(0x00);

 // is there a fault? set fault boolean and print to serial
 if (diag_reg_contents != 0xFF)
 {
   fault = true;
 }

 // deselect chip
     GPIO_SetBits( GPIOF, GPIO_Pin_10);
     GPIO_SetBits( GPIOA, GPIO_Pin_11);

 // check probe temperature, set cold start boolean
 UR_m = analogRead(UR_pin);
 if (UR_m >= cold_threshold)
 {
   cold_start = true;
 }

 // enable heater PID, define sample time and set output limits
 heater_PID.SetMode(AUTOMATIC);
 heater_PID.SetSampleTime(100);
 heater_PID.SetOutputLimits(0, 255);
}

void probe_warmup()
{
 // heat up probe (assumes 20degC cold start, ramps from 8v effective duty at 0.4v/s to 100% duty. ~ 15sec)
 Serial.println("Probe Heating");
 for (heater_duty = 145; heater_duty <= 255; heater_duty++)
 {
   analogWrite(heater_pin, heater_duty);
   delay(138);
 }
 Serial.println("Probe Heat Complete");
 // set cold start flag to false
 cold_start = false;
}

void loop()
{
 if (fault == false && cold_start == true)
 {
   // enter calibration mode
   SPI.transfer(B01010110); // INIT_REG1 WR
   SPI.transfer(B00010100); // sets RA & LA to 1

   // sample calibration values of UR & UA
   UR_c = analogRead(UR_pin);
   UR_c = map(UR_c, 0, 5, 0, 255);
   UA_c = analogRead(UA_pin);
   UA_c = map(UA_c, 0, 5, 0, 255);

   // save UR_c & UA_c to EEPROM (flash in actual board, EEPROM for test)
   EEPROM_writeAnything(0, UR_c);
   EEPROM_writeAnything(8, UA_c);

   // warm up probe
   probe_warmup();

   // enter measurement mode - LA bit controls pump current offset cal, PA controls whether pump current can flow
   SPI.transfer(B01010110); // INIT_REG1 WR
   SPI.transfer(B00000000); // sets RA & LA to 0
 }
 else if (fault == false)
 {
   // run loop code here to update heater PID and calculate pump current periodically
         UR_m = analogRead(UR_pin);
     UR_m = map(UR_m, 0, 5, 0, 255);
     heater_PID.Compute();
     analogWrite(3, heater_duty);

     UA_m = analogRead(UA_pin);
     UA_m = map(UA_m, 0, 5, 0, 255);

unsigned long pumpprint_current_millis = millis();
   if (pumpprint_current_millis - pumpprint_previous_millis > pumpprint_interval_millis)
   {
     pumpprint_previous_millis = pumpprint_current_millis;

     IP = (UA_m - UA_c) / (8 * 61.9); // check this will actually conform BODMAS style

     Serial.print("Pump Current = ");
     Serial.print(IP);
     Serial.println("");
   }
 }
 else
 {
   // print fault flag to serial every 2 sec
   unsigned long fault_current_millis = millis();
   if (fault_current_millis - fault_previous_millis > fault_interval_millis)
   {
     fault_previous_millis = fault_current_millis;
     Serial.println("");
     Serial.print("Fault Code = ");
     Serial.print(diag_reg_contents, BIN);
   }
 }
}
