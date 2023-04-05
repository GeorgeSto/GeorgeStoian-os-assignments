#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void listareIterativa(const char *cale)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(cale);
    if (dir == NULL)
    {
        printf("ERROR\ninvalid directory path");
        return;
    }
    printf("SUCCESS\n");
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", cale, entry->d_name);
            if (lstat(fullPath, &statbuf) == 0)
            {
                printf("%s\n", fullPath);
            }
        }
    }
    closedir(dir);
}

void listareRecursiva(const char *cale)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(cale);
    if (dir == NULL)
    {
        printf("ERROR\ninvalid directory path");
        return;
    }
    
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", cale, entry->d_name);
            if (lstat(fullPath, &statbuf) == 0)
            {
                if (S_ISDIR(statbuf.st_mode))
                {
                    printf("%s\n", fullPath);
                    listareRecursiva(fullPath);
                }
                else if (S_ISREG(statbuf.st_mode))
                {
                    printf("%s\n", fullPath);
                }
            }
        }
    }
    closedir(dir);
}

int main(int argc, char **argv)
{

    char *cale;
    if (argc >= 2)
    {
        if (strcmp(argv[1], "variant") == 0)
        {
            printf("38967");
        }

        if (strcmp(argv[1], "list") == 0)
        {
            if (strcmp(argv[2], "recursive") == 0)
            {
                cale = strtok(argv[3], "="); 
                cale = strtok(NULL, "");     
                printf("SUCCESS\n");
                listareRecursiva(cale);
            }
            else
            {
                // aici extragem path
                cale = strtok(argv[2], "="); // extragem path separand "path" de "dir_path"
                cale = strtok(NULL, "");     // se scoate calea introdusa dupa path
                listareIterativa(cale);
            }
        }
        if (strcmp(argv[1], "path") == 0)
        {
            printf("Path");
        }
    }
    return 0;
}