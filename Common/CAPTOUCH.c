#include "CAPTOUCH.h"
#include "timers.h"
#include "AD.h"
#include <stdio.h>
#include <stdlib.h>

volatile int touch = 0;
volatile unsigned int prevtime = 0;
volatile unsigned int currenttime = 0;
char CAPTOUCH_Init(void) {
    // following block inits the timer
    T2CON = 0;
    T2CONbits.TCKPS = 0b011;
    PR2 = 0xFFFF;
    T2CONbits.ON = 1;

    //this block inits input capture
    IC4CON = 0;
    IC4CONbits.ICTMR = 1;
    IC4CONbits.ICM = 0b010;
    touch = 0;
    prevtime = 0;
    currenttime = 0;
    IFS0bits.IC4IF = 0;
    IPC4bits.IC4IP = 7;
    IEC0bits.IC4IE = 1;
    IC4CONbits.ON = 1;
    return SUCCESS;
    // whatever else you need to do to initialize your module
}

void __ISR(_INPUT_CAPTURE_4_VECTOR) InputCapture_Handler(void) {
    IFS0bits.IC4IF = 0;
    
    // IC4BUF contains the timer value when the rising edge occurred.
}

char CAPTOUCH_IsTouched(void){
    if (touch == 1){
        return 1;
    }
    return 0;
}

