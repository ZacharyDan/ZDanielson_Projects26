/*
  Contains function implementation for USSIM project
*/
#include "setup.h"
#include "SAMDTimerInterrupt.h"


// global variables
SAMDTimer ITimer(SELECTED_TIMER);
uint32_t prevState = 0;
uint32_t rising = 0;
uint32_t currentState = 0;
uint32_t prevTime = 0;
double amplitude = 1.0;
double antenaVals[] = {1.0, 0.5, 0.25, 0.125, 2.0, 0.0}; 
int tInd = 0;
bool ampFlag = false;
bool enabledFlag = false;

uint8_t ant0 = 0;
uint8_t ant1 = 0;
uint8_t ant2 = 0;

void syncTimer() {
  ITimer.restartTimer();
}


void setupTimer() {
  if (ITimer.attachInterruptInterval(BIT_WIDTH * K, TimerHandler)) {
    Serial.println("Starting  ITimer OK, millis() = " + String(millis()));
    digitalWrite(STATUSLED_PIN, HIGH);
  }
  else {
    Serial.println("Can't set ITimer. Select another freq. or timer");

    while (true) {
      digitalWrite(STATUSLED_PIN, HIGH);
      delay(1000);
      digitalWrite(STATUSLED_PIN, LOW);
      delay(1000);
    }
  }
}


void TimerHandler() {
  ampFlag = true;
}


void setAmplitude() {
  while (DAC->SYNCBUSY.bit.DATA0);
  if (amplitude < 0.0) {
    DAC->DATA[0].reg = (abs(amplitude*.7) * VI_SCALE) * 4095.0 / 3.3;
  } else {
    DAC->DATA[0].reg = 0;
  }
  while (DAC->SYNCBUSY.bit.DATA0);  

  while (DAC->SYNCBUSY.bit.DATA1);
  DAC->DATA[1].reg = (abs(amplitude*.7) * VOFF_SCALE) * 4095.0 / 3.3;
  while (DAC->SYNCBUSY.bit.DATA1);  

  ampFlag = false;
}


void DPSKHandler() {
  amplitude *= -1;
}


void ANTSELHandler(uint8_t pos2, uint8_t pos1, uint8_t pos0) {
  uint8_t antSelIndex = 0;
  antSelIndex |= (pos0 << 0);
  antSelIndex |= (pos1 << 1);
  antSelIndex |= (pos2 << 2);

  amplitude = antenaVals[antSelIndex];
}


void TOHandler(uint8_t pos2, uint8_t pos1, uint8_t pos0) {
  // determine what station is being used, then do correct thingy
  ITimer.stopTimer();
  digitalWrite(13, HIGH);
  int prevTime = micros();
  if (pos0) {
    for (tInd = 0; tInd < AZ_POINTS; tInd++) {

      while(micros() - prevTime < 10*K);
      prevTime = micros();

      while (DAC->SYNCBUSY.bit.DATA0);
      DAC->DATA[0].reg = AZScanA0[tInd];
      while (DAC->SYNCBUSY.bit.DATA0);  
      
      while (DAC->SYNCBUSY.bit.DATA1);
      DAC->DATA[1].reg = AZScanA1[tInd];
      while (DAC->SYNCBUSY.bit.DATA1);  
    }
  } else if (pos2) {
    for (tInd = BAZ_POINTS-1; tInd > -1; tInd--) {

      while(micros() - prevTime < 10*K);
      prevTime = micros();

      while (DAC->SYNCBUSY.bit.DATA0);
      DAC->DATA[0].reg = BAZScanA0[tInd];
      while (DAC->SYNCBUSY.bit.DATA0);  
      
      while (DAC->SYNCBUSY.bit.DATA1);
      DAC->DATA[1].reg = BAZScanA1[tInd];
      while (DAC->SYNCBUSY.bit.DATA1);  
    }
  } else if (pos1) {
    for (tInd = 0; tInd < EL_POINTS; tInd++) {

      while(micros() - prevTime < 10*K);
      prevTime = micros();

      while (DAC->SYNCBUSY.bit.DATA0);
      DAC->DATA[0].reg = ELScanA0[tInd];
      while (DAC->SYNCBUSY.bit.DATA0);  
      
      while (DAC->SYNCBUSY.bit.DATA1);
      DAC->DATA[1].reg = ELScanA1[tInd];
      while (DAC->SYNCBUSY.bit.DATA1);  
    }
  }
  digitalWrite(13, LOW);
  ITimer.restartTimer();
}


void FROHandler(uint8_t pos2, uint8_t pos1, uint8_t pos0) {
  ITimer.stopTimer();
  digitalWrite(13, HIGH);
  // determine what station is being used, then do correct thingy
  int prevTime = micros();
  if (pos0) {
    for (tInd = AZ_POINTS-1; tInd > -1; tInd--) {
      while(micros() - prevTime < 10*K);
      prevTime = micros();

      while (DAC->SYNCBUSY.bit.DATA0);
      DAC->DATA[0].reg = AZScanA0[tInd];
      while (DAC->SYNCBUSY.bit.DATA0);  
      
      while (DAC->SYNCBUSY.bit.DATA1);
      DAC->DATA[1].reg = AZScanA1[tInd];
      while (DAC->SYNCBUSY.bit.DATA1);  
    }
  } else if (pos2) {
    for (tInd = 0; tInd < BAZ_POINTS; tInd++) {
      while(micros() - prevTime < 10*K);
      prevTime = micros();

      while (DAC->SYNCBUSY.bit.DATA0);
      DAC->DATA[0].reg = BAZScanA0[tInd];
      while (DAC->SYNCBUSY.bit.DATA0);  
      
      while (DAC->SYNCBUSY.bit.DATA1);
      DAC->DATA[1].reg = BAZScanA1[tInd];
      while (DAC->SYNCBUSY.bit.DATA1);  
    }
  } else if (pos1) {
    for (tInd = EL_POINTS-1; tInd > -1; tInd--) {
      while(micros() - prevTime < 10*K);
      prevTime = micros();

      while (DAC->SYNCBUSY.bit.DATA0);
      DAC->DATA[0].reg = ELScanA0[tInd];
      while (DAC->SYNCBUSY.bit.DATA0);  
      
      while (DAC->SYNCBUSY.bit.DATA1);
      DAC->DATA[1].reg = ELScanA1[tInd];
      while (DAC->SYNCBUSY.bit.DATA1);  
    }
  }
  digitalWrite(13, LOW);
  ITimer.restartTimer();
}
