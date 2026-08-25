#ifndef SETUP_H
#define SETUP_H

#include <Arduino.h>

// pins and ports
#define SEQSTRT_PORTBIT 6 
#define SEQSTRT_PIN 19
#define TXEN_PORTBIT 14
#define TXEN_PIN 4
#define DPSK_PORTBIT 16
#define DPSK_PIN 0
#define TO_PORTBIT 17
#define TO_PIN 1
#define FRO_PORTBIT 18
#define FRO_PIN 7
#define ANTSELRD_PORTBIT 19
#define ANTSELRD_PIN 9
#define ANTSEL0_PORTBIT 20
#define ANTSEL0_PIN 10
#define ANTSEL1_PORTBIT 21
#define ANTSEL1_PIN 11
#define ANTSEL2_PORTBIT 23
#define ANTSEL2_PIN 12

#define STATUSLED_PIN 13

// opamp scaling factors
#define VI_SCALE 33.0/20.0
#define VOFF_SCALE 33.0/73.0

// scanning beam values
#define AZ_POINTS 620
#define BAZ_POINTS 420
#define EL_POINTS 155

// timer implementation
#define BIT_WIDTH 64
#define K 4
#define SELECTED_TIMER TIMER_TC3


// function prototypes
void setupTimer();
void syncTimer();
void DPSKHandler();
void ANTSELHandler(uint8_t, uint8_t, uint8_t);
void TOHandler(uint8_t pos2, uint8_t pos1, uint8_t pos0);
void FROHandler(uint8_t pos2, uint8_t pos1, uint8_t pos0);
void TimerHandler();
void setAmplitude();

// global variables
extern uint32_t prevState;
extern uint32_t rising;
extern uint32_t currentState;
extern uint32_t prevTime;
extern double amplitude;
extern double antenaVals[];
extern bool ampFlag;
extern bool enabledFlag;

extern uint8_t ant0;
extern uint8_t ant1;
extern uint8_t ant2;

// scanning beam arrays
extern int AZScanA0[];
extern int AZScanA1[];

extern int BAZScanA0[];
extern int BAZScanA1[];

extern int ELScanA0[];
extern int ELScanA1[];
#endif
