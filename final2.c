
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

#define ARRAY_SIZE 8

int copy3[ARRAY_SIZE] = {1, 1, 1, 0, 0, 0, 0, 0}; // initialize array with 3 values set to 1
int copy2[ARRAY_SIZE] = {1, 1, 0, 0, 0, 0, 0, 0}; // initialize array with 2 values set to 1
int copy1[ARRAY_SIZE] = {1,0, 0, 0, 0, 0, 0, 0, 0}; // initialize array with 1 values set to 1


int arr3[ARRAY_SIZE] = {1, 1, 1, 0, 0, 0, 0, 0}; // initialize array with 3 values set to 1
int arr2[ARRAY_SIZE] = {1, 1, 0, 0, 0, 0, 0, 0}; // initialize array with 2 values set to 1
int arr1[ARRAY_SIZE] = {1,0, 0, 0, 0, 0, 0, 0}; // initialize array with 1 values set to 1

int gameArray[8][6];

int level = 0;
int lives = 3;

int start_idx = 0;  // index of first 1
int end_idx = 0;    // index of last 1


int start_idx1 = 0;  // index of first 1
int end_idx1 = 0;    // index of last 1

int start_idx2 = 0;  // index of first 1
int end_idx2 = 1;    // index of last 1

int start_idx3 = 0;  // index of first 1
int end_idx3 = 2;    // index of last 1

int direction = 1;  // 1 for forward, -1 for backward

int x = 0;

bool fallFlag = false;
int tempArr[ARRAY_SIZE];

//MAYBE do SET ARRAY 
void setRow(int row[],  int numlevels){
    
    for(int j = 0; j < numlevels ; j++){

        for(int i = 0; i < ARRAY_SIZE; i ++){
            if(i == 0)){
                if (row[i] ==1){
                    LATEbits.LATF4 = 1;
                }else{
                    LATEbits.LATF4 = 0;
                }
                TRISEbits.TRISF5 = 0;
                TRISEbits.TRISF5 = 1;  //srclk set
                TRISDbits.TRISB1 = 0;
                TRISDbits.TRISB1 = 1  //rclk   turn on row 
                 
            } else if (i == 1){
                if (row[i] ==1){
                    LATEbits.LATF4 = 1;
                }else{
                    LATEbits.LATF4 = 0;
                }
                TRISEbits.TRISF5 = 0;
                TRISEbits.TRISF5 = 1;  //srclk set
                TRISDbits.TRISB1 = 0;
                TRISDbits.TRISB1 = 1  //rclk   turn on row 
                 

            } else if (i == 2){

            }else if (i == 3){

            }else if (i == 4){

            }else if (i == 5){

            }
            
            
        }
            
    }
}


void fall(){
    int templvl = level + 1;
    //go through each level and switch the falling bits with the ones below to show falling  
    
    for(int a = 0; a< templvl; a++){
        
        //templvl = templvl-1;
        //print adjusted array
        for (int b = 0; b< templvl; b ++){
            //go through all array values and switch bits for one falling frame
            for (int c = 0; c < ARRAY_SIZE; c++) {
                
                if(gameArray[c][templvl-b] ==1 && gameArray[c][templvl-b-1] ==0){
                    // if lives == 1 
                    //set 4 pins for to mux2
                    
                    
                    //if lives >1 
                    //set 4 pins to mux1
                    
                    gameArray[c][templvl-b] =0;
                    gameArray[c][templvl-b-1] =1;// or other way              
                }
            }
            printf("\e[1;1H\e[2J");
            //print whole updated arrya from one fall frame
            for (int i =0; i< templvl ; i ++){
                for (int j = 0; j < ARRAY_SIZE; j++) {
                    printf("%d ",  gameArray[j][i]);
                }
                printf("\n");
            }
            usleep(200000);
            
        }
       
    }
    
    //wipe console
    
    //print final game array
    
    fallFlag = false;
    
    
}

//result state
void results() {
    if(lives ==0){  // if lives == 0 lose go to result state
        usleep(1200000);
        printf("\n GAME OVER \n ");
        

    }



    if(level ==5){  // if lives == 9 win go to result state
        printf("YOU WIN\n");
    }
}

void off(){
    //while not on 
//        //Get reading from ping sensor
//        if (distance){
//            state = On;
//        }
//    } else if (state == On){
}

int main(int argc, char *argv[])
{
    // row 0
    TRISDbits.TRISB1 = 0; //set output on pin 41 (reg D0) ->RCLK (refresh output)
    LATDbits.LATB1 = 0;
    
    TRISEbits.TRISF5 = 0; //set output on pin 40 (reg D0)-> SRCLK (normal clk)
    LATEbits.LATF5 = 0;

    TRISEbits.TRISF4 = 0; //set output on pin 39 (reg D0)-> SER (serial line)
    LATEbits.LATF4 = 0;
    
    
    //row 1
    TRISDbits.TRISF6 = 0; //set output on pin 38 (reg D0) ->RCLK (refresh output)
    LATDbits.LATF6 = 0;
    
    TRISEbits.TRISD7 = 0; //set output on pin 37 (reg D0)-> SRCLK (normal clk)
    LATEbits.LATD7 = 0;

    TRISEbits.TRISD6 = 0; //set output on pin 36 (reg D0)-> SER (serial line)
    LATEbits.LATD6 = 0;
    
    //row 2
    TRISDbits.TRISG6 = 0; //set output on pin 13 (reg D0) ->RCLK (refresh output)
    LATDbits.LATG6 = 0;
    
    TRISEbits.TRISG7 = 0; //set output on pin 12 (reg D0)-> SRCLK (normal clk)
    LATEbits.LATG7 = 0;

    TRISEbits.TRISG8 = 0; //set output on pin 11 (reg D0)-> SER (serial line)
    LATEbits.LATG8 = 0;
    
    //row 3
    TRISDbits.TRISD4 = 0; //set output on pin 10 (reg D0) ->RCLK (refresh output)
    LATDbits.LATD4 = 0;
    
    TRISEbits.TRISD3 = 0; //set output on pin 9 (reg D0)-> SRCLK (normal clk)
    LATEbits.LATD3 = 0;

    TRISEbits.TRISD2 = 0; //set output on pin 8 (reg D0)-> SER (serial line)
    LATEbits.LATD2 = 0;
    
    //row 4
    TRISDbits.TRISE7 = 0; //set output on pin 33 (reg D0) ->RCLK (refresh output)
    LATDbits.LATE7 = 0;
    
    TRISEbits.TRISE6 = 0; //set output on pin 32 (reg D0)-> SRCLK (normal clk)
    LATEbits.LATE6 = 0;

    TRISEbits.TRISE5 = 0; //set output on pin 31 (reg D0)-> SER (serial line)
    LATEbits.LATE5 = 0;
    
    //row 5
    TRISDbits.TRISE4 = 0; //set output on pin 30 (reg D0) ->RCLK (refresh output)
    LATDbits.LATE4 = 0;
    
    TRISEbits.TRISE3 = 0; //set output on pin 29 (reg D0)-> SRCLK (normal clk)
    LATEbits.LATE3 = 0;

    TRISEbits.TRISE2 = 0; //set output on pin 28 (reg D0)-> SER (serial line)
    LATEbits.LATE2 = 0;
    
    
    
    //game state
    while(1){
        
        //check ping sensor
        
        //while not on if something in front of ping
        
        while (!captouch ) {
            // print array
            for (int i = 0; i < ARRAY_SIZE; i++) {
                if (lives ==3){
                    printf("%d ", arr3[i]);
                   
                }else if (lives ==2){
                    printf("%d ", arr2[i]);
                }else if (lives ==1){
                    printf("%d ", arr1[i]);
                }
            }
            //printf("\n");
            printf("\r");
            fflush(stdout);
            usleep(400000);
           
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
            if (start_idx < 0 || end_idx >= ARRAY_SIZE) {
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
       
        if(level != 0){
            if (lives ==3){
                for (int j = 0; j < ARRAY_SIZE; j++) {
                    //if current level and prev level both 1 then add to temp array
                    if(arr3[j] == 1 && gameArray[j][level-1] == 1){
                        tempArr[j] = 1;
                    // drop off case
                    }else if(arr3[j] == 1 && gameArray[j][level-1] == 0){
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
                    if(arr2[j] == 1 && gameArray[j][level-1] == 1){
                        tempArr[j] = 1;
                    // drop off case
                    }else if(arr2[j] == 1 && gameArray[j][level-1] == 0){
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
                    if(arr1[j] == 1 && gameArray[j][level-1] == 1){
                        tempArr[j] = 1;
                    // drop off case
                    }else if(arr1[j] == 1 && gameArray[j][level-1] == 0){
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
                gameArray[j][level] = tempArr[j];
        }
          
        //print latests row 
        for (int j = 0; j < ARRAY_SIZE; j++) {
            // if lives == 1 
            //set 4 pins for to mux2
            
            
            //if lives >1 
            //set 4 pins to mux1
            printf("%d ",  gameArray[j][level]);
        }
        printf("\n");
        
        if(fallFlag ==true){
            
            fall();
            fallFlag == false;
        }
        
       
        if(lives ==0 || lives ==5){  // if lives == 0 lose go to result state
            results();
            //back to on state 
            break;
           
        }
       
        level +=1;
       
        // reset arrays to come from right side    
        for (int j = 0; j < ARRAY_SIZE; j++) {
            arr3[j]= copy3[j];
            arr2[j]= copy2[j];
            arr1[j]= copy1[j];
        }
        //reset indexes
        start_idx1 = 0;  // index of first 1
        end_idx1 = 0;    // index of last 1
        start_idx2 = 0;  // index of first 1
        end_idx2 = 1;    // index of last 1
        start_idx3 = 0;  // index of first 1
        end_idx3 = 2;    // index of last 1
       
       
        
        usleep(1200000);
    }
   
   
   
}