#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include<sys/wait.h>
#include "a2_helper.h"
int main(int argc,char **argv)
{
    init();
    pid_t P2,P3,P4,P5,P6,P7;
    info(BEGIN,1,0);
    P2=fork();

    if(P2 == 0)
    {
        info(BEGIN,2,0);
        info(END,2,0);
    }
    else
    {
        P3=fork();
        
        if(P3 == 0)
        {
            info(BEGIN,3,0);
            info(END,3,0);
        }
        else 
        {
            P4=fork();

            if(P4==0)
            {
                info(BEGIN,4,0);
                info(END,4,0);
            }
            else
            {
                P5=fork();

                if(P5 == 0)
                {
                    info(BEGIN,5,0);
                    info(END,5,0);
                }
                else
                {
                    P6 = fork();

                    if(P6 == 0)
                    {
                        info(BEGIN,6,0);
                        P7 = fork();

                        if(P7 == 0){
                            info(BEGIN, 7, 0);
                            info(END, 7, 0);
                        }else{
                            wait(NULL);
                            info(END, 6, 0);
                        }
                    }else{
                        wait(NULL);
                        wait(NULL);
                        wait(NULL);
                        wait(NULL);
                        wait(NULL);
                        info(END, 1, 0);
                    }
                }
            }
        }
    } 
    return 0;
}