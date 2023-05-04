#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <pthread.h>

#define nr_threads_2 5
#define nr_threads_7 45
#define nr_threads_3 4

typedef struct
{
    int id;
} structura;

void *thread_function_2(void *arg)
{
    structura *a = (structura *)arg;
    info(BEGIN, 2, a->id);
    info(END, 2, a->id);
    return NULL;
}

void *thread_function_7(void *arg)
{
    structura *b = (structura *)arg;
    info(BEGIN, 7, b->id);
    info(END, 7, b->id);
    return NULL;
}

void *thread_function_3(void *arg)
{
    structura *c = (structura *)arg;
    info(BEGIN,3,c->id);
    info(END,3,c->id);
    return NULL;
}

int main(int argc, char **argv)
{

    init();
    pid_t P2, P3, P4, P5, P6, P7;
    info(BEGIN, 1, 0);
    P2 = fork();

    pthread_t tid_2[nr_threads_2];
    structura params_2[nr_threads_2];

    pthread_t tid_7[nr_threads_7];
    structura params_7[nr_threads_7];

    pthread_t tid_3[nr_threads_3];
    structura params_3[nr_threads_3];

    if (P2 == 0)
    {
        info(BEGIN, 2, 0);

        for (int i = 0; i < nr_threads_2; i++)
        {
            params_2[i].id = i + 1;
            pthread_create(&tid_2[i], NULL, thread_function_2, &params_2[i]);
        }
        for (int i = 0; i < nr_threads_2; i++)
        {
            pthread_join(tid_2[i], NULL);
        }
        info(END, 2, 0);
    }
    else
    {
        P3 = fork();

        if (P3 == 0)
        {
            info(BEGIN, 3, 0);
            for(int i=0;i<nr_threads_3;i++)
            {
                params_3[i].id= i+1;
                pthread_create(&tid_3[i],NULL,thread_function_3,&params_3[i]);
            }
            for(int i=0;i<nr_threads_3;i++)
            {
                pthread_join(tid_3[i],NULL);
            }
            info(END, 3, 0);
        }
        else
        {
            P4 = fork();

            if (P4 == 0)
            {
                info(BEGIN, 4, 0);
                info(END, 4, 0);
            }
            else
            {
                P5 = fork();

                if (P5 == 0)
                {
                    info(BEGIN, 5, 0);
                    info(END, 5, 0);
                }
                else
                {
                    P6 = fork();

                    if (P6 == 0)
                    {
                        info(BEGIN, 6, 0);
                        P7 = fork();

                        if (P7 == 0)
                        {
                            info(BEGIN, 7, 0);
                            for(int i=0;i<nr_threads_7;i++)
                            {
                                params_7[i].id =i+1;
                                pthread_create(&tid_7[i],NULL,thread_function_7,&params_7[i]);
                            }
                            for(int i=0;i<nr_threads_7;i++)
                            {
                                pthread_join(tid_7[i],NULL);
                            }

                            info(END, 7, 0);
                        }
                        else
                        {
                            wait(NULL);
                            info(END, 6, 0);
                        }
                    }
                    else
                    {
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