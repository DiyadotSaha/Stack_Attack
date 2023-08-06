#include <stdio.h>
#include <stdlib.h>
#include "Buttons.h"
#include "AD.h"
//AD_A2 = Piezoelectric
//AD_A1 = Capacitive Touch
//Pin 2 = Trigger for Ping
//Pin 34 = Echo for Ping
int distance;
int result;
int row;
int lives;
enum SM{
    Off,
    On,
    Game,
    Result
}state;

//int GameSM(void){
//    if (state == Off){
//        //Get reading from ping sensor
//        if (distance){
//            state = On;
//        }
//    } else if (state == On){
//        
//    } else if (state == Game){
//        if (lives == 0){
//            state = Result;
//            result = 0;
//        } else if (row == 6 && lives > 0){
//            state = Result;
//            result = 1;
//        } 
//        
//    } else if (state == Result){
//        if (result == 1){
//            
//            return 1;
//        }
//        else if (result == 0){
//            return 0;
//        }
//    }
//}

