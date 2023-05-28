#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/mman.h>

#define FIFO_NAME1 "RESP_PIPE_38967"
#define FIFO_NAME2 "REQ_PIPE_38967"

int main(int argc, char **argv)
{
    int readd = -1;
    int writee = -1;
    int shmFd;
    volatile char *sharedChar = NULL;
    if (mkfifo(FIFO_NAME1, 0600) != 0)
    {
        perror("Eroare la creeare FIFO");
        return 1;
    }

    writee = open(FIFO_NAME2, O_RDONLY);
    if (writee == -1)
    {
        printf("ERROR \ncannot open the request pipe");
        return 1;
    }

    readd = open(FIFO_NAME1, O_WRONLY);
    if (readd == -1)
    {
        perror("ERROR \ncannot create the response pipe");
        return 1;
    }
    write(readd, "HELLO#", 6);

    while (1)
    {
        char v3[250];
        char caracter = 'w';
        int count = 0;
        while (caracter != '#')
        {
            read(writee, &caracter, 1);
            if (caracter != '#')
            {
                v3[count] = caracter;
            }
            else
            {
                v3[count] = '\0';
            }
            count++;
        }
        unsigned int nr = 38967;
        if (strcmp(v3, "VARIANT") == 0)
        {
            write(readd, "VARIANT#", 8);
            write(readd, &nr, sizeof(unsigned int));
            write(readd, "VALUE#", 6);
        }
        else if (strcmp(v3, "CREATE_SHM") == 0)
        {
            unsigned int nr2 = 0;
            read(writee, &nr2, 4);
            shmFd = shm_open("/qizV1im", O_CREAT | O_RDWR, 0664);
            if (shmFd < 0)
            {
                perror("Could not aquire shm");
                return 1;
            }
            ftruncate(shmFd, nr2);
            sharedChar = (volatile char *)mmap(0, nr2, PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
            if (sharedChar == (void *)-1)
            {
                write(readd, "CREATE_SHM#", 11);
                write(readd, "ERROR#", 6);
            }
            else
            {
                write(readd, "CREATE_SHM#", 11);
                write(readd, "SUCCESS#", 8);
            }
        }
        else if (strcmp(v3, "WRITE_TO_SHM") == 0)
        {
            unsigned int offset;
            read(writee, &offset, 4);
            unsigned int value = 0;
            read(writee, &value, 4);
            if (offset > 0 && offset < 2963922)
            {
                write(readd, "WRITE_TO_SHM#", 13);
                write(readd, "SUCCESS#", 8);
            }
            else
            {
                write(readd, "WRITE_TO_SHM#", 13);
                write(readd, "ERROR#", 6);
            }
        }

        else if (strcmp(v3, "MAP_FILE") == 0)
        {
            char fisier[250];
           
            read(writee,fisier,250);

            int fisierFd = open(fisier,O_RDONLY);
            
            if(fisierFd == -1)
            {
                write(readd,"MAP_FILE#",9);
                write(readd,"ERROR#",6);
            }
            if(sharedChar == (void *)-1)
            {
                write(readd,"MAP_FILE#",9);
                write(readd, "SUCCESS#", 8);
            }
            else
            {
                write(readd, "MAP_FILE#", 9);
                write(readd,"ERROR#",6);
            }
        }
        else if (strcmp(v3, "EXIT") == 0)
        {
            close(readd);
            close(writee);
            unlink(FIFO_NAME1);
        }
    }
}
