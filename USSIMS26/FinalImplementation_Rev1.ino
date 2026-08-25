/*
  Main file for USSIM project. Implemented on an ItsyBitsy M4 MCU

  Last updated: 5/14
*/

#include "setup.h"

void setup() {
  delay(3000);

  //Serial.begin(9600);

  // sets up pins 
  pinMode(SEQSTRT_PIN, INPUT);
  pinMode(TXEN_PIN, INPUT);
  pinMode(DPSK_PIN, INPUT);
  pinMode(TO_PIN, INPUT);
  pinMode(FRO_PIN, INPUT);
  pinMode(ANTSELRD_PIN, INPUT);
  pinMode(ANTSEL0_PIN, INPUT);
  pinMode(ANTSEL1_PIN, INPUT);
  pinMode(ANTSEL2_PIN, INPUT);

  setupTimer();

  analogWrite(A0, 0);
  analogWrite(A1, 0);
}

void loop() {
  if (digitalRead(TXEN_PIN)) {
    if (!enabledFlag) {
      amplitude = 1.0;
      enabledFlag = true;
    }

    // check for rising edge
    currentState = PORT->Group[0].IN.reg;
    rising = currentState & ~prevState;
    prevState = currentState;

    if ((rising >> SEQSTRT_PORTBIT) & 1) {
      syncTimer();
    }

    // run appropriate function depending on where the rising edge is
    if ((rising >> DPSK_PORTBIT) & 1) {
      DPSKHandler();
    }

    // reads current antenna values, I think this could go into the conditionals...
    ant0 = (currentState >> ANTSEL0_PORTBIT) & 1;
    ant1 = (currentState >> ANTSEL1_PORTBIT) & 1;
    ant2 = (currentState >> ANTSEL2_PORTBIT) & 1;
    // ant0 = 1;

    if ((rising >> TO_PORTBIT) & 1) {
      TOHandler(ant2, ant1, ant0);
    }

    if ((rising >> FRO_PORTBIT) & 1) {
      FROHandler(ant2, ant1, ant0);
    }

    if ((rising >> ANTSELRD_PORTBIT) & 1) {
      ANTSELHandler(ant2, ant1, ant0);
    }
    
  } else {
    amplitude = 0.0;
    enabledFlag = false;
  }

  if (ampFlag) {
    setAmplitude();
  }
}
