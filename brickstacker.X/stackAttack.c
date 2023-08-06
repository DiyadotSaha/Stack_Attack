/* 
 * File:   stackAttack.c
 * Author: robertgaines
 *
 * Created on June 3, 2023, 3:32 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "BOARD.h"
#include "serial.h"
#include "Buttons.h"
#include <stdbool.h>
#include <unistd.h>
#include <sys/select.h>
#include "PING.h"
#include "CAPTOUCH.h"
#include "timers.h"
#include "AD.h"
#include "pwm.h"
#include "ToneGeneration.h"


#define ARRAY_SIZE 8
/*
 * 
 */
#define DELAY(x) {int wait; for(wait = 0; wait <= x; wait++){asm("nop");}}

int copy3[ARRAY_SIZE] = {1, 1, 1, 0, 0, 0, 0}; // initialize array with 3 values set to 1
int copy2[ARRAY_SIZE] = {1, 1, 0, 0, 0, 0, 0}; // initialize array with 2 values set to 1
int copy1[ARRAY_SIZE] = {1, 0, 0, 0, 0, 0, 0, 0}; // initialize array with 1 values set to 1


int arr3[ARRAY_SIZE] = {1, 1, 1, 0, 0, 0, 0}; // initialize array with 3 values set to 1
int arr2[ARRAY_SIZE] = {1, 1, 0, 0, 0, 0, 0}; // initialize array with 2 values set to 1
int arr1[ARRAY_SIZE] = {1, 0, 0, 0, 0, 0, 0}; // initialize array with 1 values set to 1

int gameArray[6][8];

int level = 0;
int lives = 3;

int captouched = 0;
int piezotouched = 0;

int start_idx = 0; // index of first 1
int end_idx = 0; // index of last 1


int start_idx1 = 1; // index of first 1
int end_idx1 = 1; // index of last 1

int start_idx2 = 1; // index of first 1
int end_idx2 = 2; // index of last 1

int start_idx3 = 1; // index of first 1
int end_idx3 = 3; // index of last 1

int direction = 1; // 1 for forward, -1 for backward

int x = 0;

bool fallFlag = false;
int tempArr[ARRAY_SIZE];

int blank [6][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

int arrow [6][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 0}
};


int letterC [6][8] = {
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0}
};

int letterW [6][8] = {
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 1, 0, 1, 1, 0, 1, 0},
    {0, 1, 0, 1, 1, 0, 1, 0},
    {0, 1, 0, 1, 1, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0}
};

int letterL [6][8] = {
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 0, 0}
};
int letterE [6][8] = {
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0}
};

int numberOne[6][8] = {
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0}
};
int numberSix[6][8] = {
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0}
};
int numberSeven[6][8] = {
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0}
};

void loadArray(int arr[6][8], int numlevels) {
    numlevels+=1;
    for (int j = 0; j < numlevels; j++) {

        for (int i = ARRAY_SIZE; i >= 0; i--) {
            //printf("%d ", arr[j][i]);
            if (j == 1) {
                //printf("in row 4\n");
                if (arr[j][i] == 1) {
                    //printf("1 detected in row 4\n");
                    LATBbits.LATB1 = 1;
                } else {
                    LATBbits.LATB1 = 0;
                }
                LATFbits.LATF4 = 0;
                LATFbits.LATF4 = 1; //srclk set


            } else if (j == 0) {
                //printf("in row 5\n");
                if (arr[j][i] == 1) {

                    LATFbits.LATF6 = 1;
                } else {
                    LATFbits.LATF6 = 0;
                }
                LATDbits.LATD6 = 0;
                LATDbits.LATD6 = 1;

            } else if (j == 2) {
                //printf("in row 3\n");

                if (arr[j][i] == 1) {
                    LATGbits.LATG6 = 1;
                } else {
                    LATGbits.LATG6 = 0;
                }
                LATGbits.LATG8 = 0;
                LATGbits.LATG8 = 1;



            } else if (j == 3) {
                //printf("in row 2\n");
                if (arr[j][i] == 1) {
                    LATDbits.LATD11 = 1;
                } else {
                    LATDbits.LATD11 = 0;
                }
                LATFbits.LATF1 = 0;
                LATFbits.LATF1 = 1;



            } else if (j == 4) {
                //printf("in row 1\n");
                if (arr[j][i] == 1) {
                    //printf("1 detected in row 1\n");
                    LATEbits.LATE7 = 1;
                } else {
                    LATEbits.LATE7 = 0;
                }
                LATEbits.LATE5 = 0;
                LATEbits.LATE5 = 1;



            } else if (j == 5) {
                //printf("in row 0\n");
                if (arr[j][i] == 1) {
                    //printf("1 detected in row 0\n");
                    LATEbits.LATE4 = 1;
                } else {
                    LATEbits.LATE4 = 0;
                }

                LATEbits.LATE2 = 0;
                LATEbits.LATE2 = 1;


            }

        }
        //printf("\n");
        //DELAY(1000000);
        if (j == 0) {
            LATDbits.LATD7 = 0;
            LATDbits.LATD7 = 1; //rclk   turn on row 
        } else if (j == 1) {
            LATFbits.LATF5 = 0;
            LATFbits.LATF5 = 1; //rclk   turn on row 

        } else if (j == 2) {
            LATGbits.LATG7 = 0;
            LATGbits.LATG7 = 1;
        } else if (j == 3) {
            LATEbits.LATE1 = 0;
            LATEbits.LATE1 = 1;
        } else if (j == 4) {
            LATEbits.LATE6 = 0;
            LATEbits.LATE6 = 1;
        } else if (j == 5) {
            LATEbits.LATE3 = 0;
            LATEbits.LATE3 = 1;
        }



    }
    //    

    //                
    //    TRISEbits.TRISE3 = 0;
    //    TRISEbits.TRISE3 = 1;  //rclk set+
}

void pingFunction() {
    unsigned int tick = 0;

    while (1) {
        if (TIMERS_GetMilliSeconds() - tick >= 10) {
            tick = TIMERS_GetMilliSeconds();
            int dist = PING_GetDistance();
            if (dist > 0) {
                if (dist < 40) {
                    return;
                }
                printf("distance = %d cm \r\n", dist);
            }
            DELAY(10000);
        }
    }
}

void capFunction() {
    unsigned int tick = 0;
    unsigned int cap = 0;
    while(1){
        if (TIMERS_GetMilliSeconds() - tick >= 10) {
        tick = TIMERS_GetMilliSeconds();
        cap = AD_ReadADPin(AD_A1);
        printf("Cap touch: %d \r\n", cap);
        if (cap > 15){
            return;
            //captouched = 1;
        } 
//        else{
//            captouched = 0;
//        }
        DELAY(10000);
        }
    
    }
    
//    return captouched;
}


int piezoFunction(){
//    unsigned int piezo = AD_ReadADPin(AD_A2);
//    printf("piez:%u\r\n", piezo);
//    if(piezo >50){
//        return 1;
//    }else{
//        return 0;
//    }
    
    unsigned int tick = 0;
    unsigned int cap = 0;
    if (TIMERS_GetMilliSeconds() - tick >= 10) {
        tick = TIMERS_GetMilliSeconds();
        int dist = PING_GetDistance();
        //printf("distance = %d cm \r\n", dist);
        if (dist > 0) {
            if (dist < 50) {
                return 0;
            }else{
                return 1;
            }

        }
        DELAY(10000);
//        DELAY(10000);
    }else{
        return 1;
    }    
//        else{
//            captouched = 0;
//        }
        //DELAY(10000);
        
    return 1;
    
    
    
}
//result state

void results() {
    
    if (lives == 0) { // if lives == 0 lose go to result state
        
        printf("\n GAME OVER \n ");
        
         
        int freq = 800;
        
       
        //play sounds
        for(int i =0 ; i < 3; i ++){
            loadArray(letterL, 6);
            ToneGeneration_ToneOn(); 
            DELAY(100000);
            loadArray(blank, 6);
            freq -= 100; 
            ToneGeneration_SetFrequency(freq); 
            DELAY(100000);
        }
        
        ToneGeneration_ToneOff(); 
        
        return;
    }
    if (level ==6) { // if lives == 9 win go to result state
        printf("YOU WIN\n");
        int freq = 200;
        //play sounds
        for(int i =0 ; i < 3; i ++){
            loadArray(letterW, 6);
            ToneGeneration_ToneOn(); 
            DELAY(100000);
            loadArray(blank, 6);
            freq += 100; 
            ToneGeneration_SetFrequency(freq); 
            loadArray(blank, 6);
            DELAY(100000);
        }
        ToneGeneration_ToneOff(); 
        return;
    }
    return;
}

void fall2(){
    int templvl = level + 1;
    //go through each level and switch the falling bits with the ones below to show falling  

    for (int a = 0; a < ARRAY_SIZE; a++) {
        if(gameArray[templvl][a] ==1 && gameArray[templvl-1][a]==0){
            //for every level under the falling block initial 
            for(int i =templvl; i <0; i--){
                if(gameArray[i][a] ==1 && gameArray[i-1][a]==0){
                    gameArray[i][a]=0;
                    gameArray[i-1][a] =1;
                    loadArray(gameArray, level);
                    DELAY(100000);
                }
            }
        }
    }
    fallFlag = false;
    return;
}

void fall() {
    int templvl = level + 1;
    //go through each level and switch the falling bits with the ones below to show falling  
    int fallTone =900;
    //for (int a = 0; a < level; a++) {
       
        //templvl = templvl-1;
        //print adjusted array
        for (int b = 5; b > 0; b--) {
            //go through all array values and switch bits for one falling frame
            for (int c = 0; c < ARRAY_SIZE; c++) {
                
                if (gameArray[b][c] == 1 && gameArray[b - 1][c] == 0) { // NOTETENOENTET switch
                     
                    printf("found a fallign block \n");
                    // if lives == 1 
                    //set 4 pins for to mux2
//                    for(int j = 0; j < templvl){
//                        
//                    }

                    //if lives >1 
                    //set 4 pins to mux1
                    if(b==1){
                       gameArray[b][c] = 0; // NOTETENOENTET  swith
                        gameArray[ b - 1][c] = 0; // NOTETENOENTET  swithc  
                    }else{
                        gameArray[b][c] = 0; // NOTETENOENTET  swith
                        gameArray[ b - 1][c] = 1; // NOTETENOENTET  swithc 
                    }
                     
                    
                }
            }
            //            printf("\e[1;1H\e[2J");
            //print whole updated arrya from one fall frame
            //            for (int i =0; i< templvl ; i ++){
            //                for (int j = 0; j < ARRAY_SIZE; j++) {
            //                    printf("%d ",  gameArray[i][j]);  // NOTETENOENTET change to i j
            //                }
            //                printf("\n");
            //            }
            if(b >2){
                fallTone-=120;
                ToneGeneration_SetFrequency(fallTone);
                ToneGeneration_ToneOn();
            }else{
                 ToneGeneration_ToneOff();
            }
            
            loadArray(gameArray, level);
            DELAY(100000);

        }
        
    ToneGeneration_ToneOff();
    //}
    fallFlag = false;
    return;
}

void printECE() {
    
   ToneGeneration_ToneOn();
   ToneGeneration_SetFrequency(200); 
   loadArray(letterE, 6);
   
   DELAY(100000);
   ToneGeneration_ToneOn();
   ToneGeneration_SetFrequency(400); 
   loadArray(letterC, 6);
   
   DELAY(100000);
   ToneGeneration_ToneOn();
   ToneGeneration_SetFrequency(200);
   
   loadArray(letterE, 6);
   
   DELAY(200000);
   ToneGeneration_ToneOn();
   ToneGeneration_SetFrequency(400); 
   loadArray(numberOne, 6);
   
   DELAY(100000);
   ToneGeneration_ToneOn();
   ToneGeneration_SetFrequency(600); 
   loadArray(numberSix, 6);
   
   DELAY(100000);
   ToneGeneration_ToneOn();
   ToneGeneration_SetFrequency(800); 
   loadArray(numberSeven, 6);
 
   DELAY(100000); 
   
   ToneGeneration_ToneOff();
   loadArray(arrow, 6);
}

int main(int argc, char** argv) {

    //FORMAT FOR ALL PINS FOR EACH ROW
    //1st pin = ser 2nd = rclk  3rd = srclk

    // row 5
    TRISBbits.TRISB1 = 0; //set output on pin 41 (reg D0) ->SER (refresh output)
    LATBbits.LATB1 = 0;

    TRISFbits.TRISF5 = 0; //set output on pin 40 (reg D0)-> RCLK (normal clk)
    LATFbits.LATF5 = 0;

    TRISFbits.TRISF4 = 0; //set output on pin 39 (reg D0)-> SRCLK (serial line)
    LATFbits.LATF4 = 0;


    //row 6 index 5
    TRISFbits.TRISF6 = 0; //set output on pin 38 
    LATFbits.LATF6 = 0;

    TRISDbits.TRISD7 = 0; //set output on pin 37 
    LATDbits.LATD7 = 0;

    TRISDbits.TRISD6 = 0; //set output on pin 36 
    LATDbits.LATD6 = 0;

    //row 4
    TRISGbits.TRISG6 = 0; //set output on pin 13 
    LATGbits.LATG6 = 0;

    TRISGbits.TRISG7 = 0; //set output on pin 12 
    LATGbits.LATG7 = 0;

    TRISGbits.TRISG8 = 0; //set output on pin 11 
    LATGbits.LATG8 = 0;

    //row ?   supposed to be 3
    TRISDbits.TRISD11 = 0; //set output on pin 35 
    LATDbits.LATD11 = 0;

    TRISEbits.TRISE1 = 0; //set output on pin 27 
    LATEbits.LATE1 = 0;

    TRISFbits.TRISF1 = 0; //set output on pin 34 
    LATFbits.LATF1 = 0;

    //row 2
    TRISEbits.TRISE7 = 0; //set output on pin 33 
    LATEbits.LATE7 = 0;

    TRISEbits.TRISE6 = 0; //set output on pin 32 
    LATEbits.LATE6 = 0;

    TRISEbits.TRISE5 = 0; //set output on pin 31 
    LATEbits.LATE5 = 0;

    //row 1
    TRISEbits.TRISE4 = 0; //set output on pin 30 
    LATEbits.LATE4 = 0;

    TRISEbits.TRISE3 = 0; //set output on pin 29 
    LATEbits.LATE3 = 0;

    TRISEbits.TRISE2 = 0; //set output on pin 28 
    LATEbits.LATE2 = 0;


    BOARD_Init();
    TIMERS_Init();
    SERIAL_Init();
    AD_Init();
    AD_AddPins(AD_A1);
    AD_AddPins(AD_A2);
    PWM_Init();
    PWM_AddPins(PWM_PORTZ06);
    PING_Init();
    CAPTOUCH_Init();
    ToneGeneration_Init();
    //ToneGeneration_SetFrequency(500);
    //ToneGeneration_ToneOn();
    
//    uint32_t time1 = 0;
//    uint32_t time2 = 0;
//
//    int shifted = 0;
//
//    int SPEED = 500;
//    int lives = 3;
    int delayVal = 100000;
    int flag = 0;
    while (1) {
        printf("Games start\n");
        delayVal =100000;
        flag =0;
        level =0;
        lives =3;
        direction = 1;
        for(int z =0; z < 6; z++){
            for(int g = 0; g < ARRAY_SIZE ; g++){
                gameArray[z][g] = 0;
            }
        }
        loadArray(blank, 6);
        pingFunction();   //fucntion will pass once in detection of game
        printECE();   //print ece animation once
        capFunction();   //funciton will pass once cap is touched
        printf("Works \n");
        loadArray(blank, 6);
        

        ///game state macine
        while(1){
            //check for captoutch
            
            while (piezoFunction()) {
                printf("not tapped\n");
                // print array
                for (int i = 0; i < ARRAY_SIZE; i++) {
                    if (lives ==3){
                        gameArray[level][i] = arr3[i];
                        //printf("%d ", arr3[i]);

                    }else if (lives ==2){
                        gameArray[level][i] = arr2[i];
                        //printf("%d ", arr2[i]);
                    }else if (lives ==1){
                        gameArray[level][i] = arr1[i];
                        //printf("%d ", arr1[i]);
                    }
                }
                loadArray(gameArray, level);
                if(flag==1){
                    ToneGeneration_ToneOn();
                    ToneGeneration_SetFrequency(600);
                    DELAY(20000);
                    ToneGeneration_ToneOff();
                }else if (flag ==2){
                    ToneGeneration_ToneOn();
                    ToneGeneration_SetFrequency(200);
                    DELAY(20000);
                    ToneGeneration_ToneOff();
                }
                flag=0;
                //printf("\n");
//                printf("\r");
//                fflush(stdout);
                DELAY(delayVal);
                

                if (lives ==3){
                    // update indices
                    start_idx3 += direction;
                    end_idx3 += direction;
                    start_idx = start_idx3;
                    end_idx = end_idx3;

                }else if (lives ==2){
                    // update indices
                    start_idx2 += direction;
                    end_idx2 += direction;
                    start_idx = start_idx2;
                    end_idx = end_idx2;
                }else if (lives ==1){
                    // update indices
                    start_idx1 += direction;
                    end_idx1 += direction;
                    start_idx = start_idx1;
                    end_idx = end_idx1;
                }
                // check if indices hit array boundaries
                if (start_idx < 1 || end_idx >= ARRAY_SIZE-1) {
                    
                    if(start_idx < 1 ){
                        flag =1;
                    }else{
                        flag =2;
                    }
                    
                    
                    direction *= -1;  // reverse direction
                    start_idx+= 2 * direction;
                    end_idx += 2 * direction;
                }

                // reset array
                for (int i = 0; i < ARRAY_SIZE; i++) {
                    if (lives ==3){
                        arr3[i] = (i >= start_idx3 && i <= end_idx3) ? 1 : 0;
                    }else if (lives ==2){
                        arr2[i] = (i >= start_idx2 && i <= end_idx2) ? 1 : 0;
                    }else if (lives ==1){
                        arr1[i] = (i >= start_idx1 && i <= end_idx1) ? 1 : 0;
                    }

                }
            }
            delayVal-=20000;
            printf("tapped\n");
            if(level != 0){
                if (lives ==3){
                    for (int j = 0; j < ARRAY_SIZE; j++) {
                        //if current level and prev level both 1 then add to temp array
                        if(arr3[j] == 1 && gameArray[level-1][j]== 1){
                            tempArr[j] = 1;
                        // drop off case
                        }else if(arr3[j] == 1 && gameArray[level-1][j] == 0){
                            tempArr[j] = 1;                                                     //CHANGE TO 1 LATER FOR FALL TO WORK
                            lives -=1;
                        //else temparray is 0
                        }else{
                            tempArr[j] = 0;
                        }
                    }
                    if(lives <3){
                        fallFlag =true;
                    }
                } else if (lives ==2){
                    for (int j = 0; j < ARRAY_SIZE; j++) {
                        //if current level and prev level both 1 then add to temp array
                        if(arr2[j] == 1 && gameArray[level-1][j] == 1){
                            tempArr[j] = 1;
                        // drop off case
                        }else if(arr2[j] == 1 && gameArray[level-1][j] == 0){
                            tempArr[j] = 1;                                                
                            lives -=1;
                        //else temparray is 0
                        }else{
                            tempArr[j] = 0;
                        }
                    }
                    if(lives <2){
                        fallFlag=true;
                    }
                }else if (lives ==1){
                    for (int j = 0; j < ARRAY_SIZE; j++) {
                        //if current level and prev level both 1 then add to temp array
                        if(arr1[j] == 1 && gameArray[level-1][j] == 1){
                            tempArr[j] = 1;
                        // drop off case
                        }else if(arr1[j] == 1 && gameArray[level-1][j] == 0){
                            tempArr[j] = 1;                                                 
                            lives -=1;
                        //else temparray is 0
                        }else{
                            tempArr[j] = 0;
                        }
                    }
                    if(lives <1){
                        fallFlag = true;
                    }
                }   
            }else{
                for (int j = 0; j < ARRAY_SIZE; j++) {
                    tempArr[j] = arr3[j];
                }
            }

            //copy to game array
            for (int j = 0; j < ARRAY_SIZE; j++) {
                    gameArray[level][j] = tempArr[j];
            }

            //print latests row 
//                    for (int j = 0; j < ARRAY_SIZE; j++) {
//                        loadArray(gameArray, level);            ///possible need to add one to level
//                        printf("%d ",  gameArray[level][j]);
//                    }
//                    printf("\n");
            loadArray(gameArray, level);
            ToneGeneration_ToneOn();
            ToneGeneration_SetFrequency(200); 
            DELAY(80000);
            ToneGeneration_ToneOff();
            if(fallFlag ==true){

                fall();
                fallFlag = false;
            }
            
            level +=1;

            if(lives ==0 || level ==6){  // if lives == 0 lose go to result state
                printf("triggered results\n");
                results();
                //back to on state 
                break;

            }

            

            // reset arrays to come from right side    
            for (int j = 0; j < ARRAY_SIZE; j++) {
                arr3[j]= copy3[j];
                arr2[j]= copy2[j];
                arr1[j]= copy1[j];
            }
            //reset indexes
            

            start_idx1 = 1; // index of first 1
            end_idx1 = 1; // index of last 1

            start_idx2 = 1; // index of first 1
            end_idx2 = 2; // index of last 1

            start_idx3 = 1; // index of first 1
            end_idx3 = 3; // index of last 1



//            DELAY(1200000);
        }
        printf("restarting game \n");
        
        
    }
    //        
    //        


    //        time1 = TIMERS_GetMilliSeconds();
    //        
    //        if(time1 % SPEED == 0){
    //            //set clocks low
    //        LATDbits.LATD2 = 0;
    //        LATDbits.LATD3 = 0;
    //        printf("shift loop");
    //        //shift registers
    //        shifted++;
    //        if(shifted >= 8){shifted = 0; lives = 3;}
    //        if(lives > 0){
    //            LATDbits.LATD1 = 1;
    //            lives --;
    //        }
    //        else LATDbits.LATD1 = 0;
    //
    //        //serial clock high will shift shift register one space
    //        LATDbits.LATD2 = 1;
    //
    //        //refresh output clock set high to
    //        if(! lives > 0)
    //        LATDbits.LATD3 = 1;
    //        DELAY(100000);
    //        }

    //        
    //        time1 = TIMERS_GetMilliSeconds();
    //        
    //        if(time1 % SPEED == 0){
    //            //set clocks low
    //        LATDbits.LATD7 = 0;
    //        LATDbits.LATD6 = 0;
    //        printf("shift loop");
    //        //shift registers
    //        shifted++;
    //        if(shifted >= 8){shifted = 0; lives = 3;}
    //        if(lives > 0){
    //            LATFbits.LATF6 = 1;
    //            lives --;
    //        }
    //        else LATFbits.LATF6 = 0;
    //
    //        //serial clock high will shift shift register one space
    //        LATDbits.LATD6 = 1;
    //
    //        //refresh output clock set high to
    //        if(! lives > 0)
    //        LATDbits.LATD7 = 1;
    //        DELAY(100000);
    //        }
    //        
    //        



}


