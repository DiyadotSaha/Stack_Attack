#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#define ARRAY_SIZE 9

struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

int copy3[ARRAY_SIZE] = {1, 1, 1, 0, 0, 0, 0, 0, 0}; // initialize array with 3 values set to 1
int copy2[ARRAY_SIZE] = {1, 1, 0, 0, 0, 0, 0, 0, 0}; // initialize array with 2 values set to 1
int copy1[ARRAY_SIZE] = {1,0, 0, 0, 0, 0, 0, 0, 0}; // initialize array with 1 values set to 1


int arr3[ARRAY_SIZE] = {1, 1, 1, 0, 0, 0, 0, 0, 0}; // initialize array with 3 values set to 1
int arr2[ARRAY_SIZE] = {1, 1, 0, 0, 0, 0, 0, 0, 0}; // initialize array with 2 values set to 1
int arr1[ARRAY_SIZE] = {1,0, 0, 0, 0, 0, 0, 0, 0}; // initialize array with 1 values set to 1

int gameArray[9][10];

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


void fall(){
    int templvl = level + 1;
    // printf("\e[1;1H\e[2J");
    // printf("\n");
    // printf("\n");
    // printf("\n");
    // printf("\n");
    // printf("levels:%d \n", templvl );
    
    // //print last line for array
    // for (int j = 0; j < ARRAY_SIZE; j++) {
    //     printf("%d ",  gameArray[j][level]);
    // }
    // printf("\n");
    
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
                
    
                
                
                // printf("%d ",  gameArray[c][b]);
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
            
            // printf("\n");
        }
        
        //delay a bit so it not so fast 
        // usleep(2000000);
           
        
        // printf("\n");
        // printf("\n");
        
        //wipe console
        //printf("\e[1;1H\e[2J");
        
        
    }
    
    //wipe console
    
    //print final game array
    
    
    //POSSIBLE PROBLEMS:   Game array may be incorrect at the end. SAVE game aray to temp array and do falling animation with temp array
    // 
    fallFlag = false;
    
    
    
    //printf("end fall\n");
    
}


int main(int argc, char *argv[])
{
    
   
    while(1){
        set_conio_terminal_mode();

        while (!kbhit()) {
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
                        tempArr[j] = 1;                                                 //CHANGE TO 1 LATER FOR FALL TO WORK
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
        
       
        if(lives ==0){
            usleep(1200000);
            printf("\n GAME OVER \n ");
            // for (int j = 0; j < ARRAY_SIZE; j++) {
            //     printf("%d ",  gameArray[j][level]);
            // }
            // printf("\n prev\n");
           
            // for (int j = 0; j < ARRAY_SIZE; j++) {
            //     printf("%d ",  gameArray[j][level-1]);
            // }
            printf("\n");
            
            break;
           
        }
       
       
       
        if(level ==9){
            printf("YOU WIN\n");
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
       
       
        (void)getch(); /* consume the character */
        usleep(1200000);
    }
   
   
   
}