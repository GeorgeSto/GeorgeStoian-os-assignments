#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct
{
    char *list;
    char *recursiv;
    char *filtering;
    char *path;
    char *parse;
} filtrare;

typedef struct
{
    char name[10];
    int size;
    int offset;
    int type;
} hSectiune;

filtrare *cautareArgumente(char **arg, int cnt)
{
    filtrare *a = (filtrare *)malloc(sizeof(filtrare));
    a->list = NULL;
    a->recursiv = NULL;
    a->filtering = NULL;
    a->path = NULL;
    for (int i = 1; i < cnt; i++)
    {
        if (strcmp(arg[i], "list") == 0) // salvam in stringurile din structura parametrii care ar trebui sa fie in linie de comanda intr-un for pentru a putea lua in orice ordine
        {
            a->list = "list";
        }
        if (strcmp(arg[i], "recursive") == 0)
        {
            a->recursiv = "recursiv";
        }
        if (strncmp(arg[i], "name_starts_with=", 17) == 0) // daca vrem sa filtram dupa name_starts _with punem in filtering numele cu care trebuie sa inceapa rezultatul
        {
            char *p = strtok(arg[i], "=");
            p = strtok(NULL, "");
            a->filtering = p;
        }
        if (strncmp(arg[i], "has_perm_write", 14) == 0) // daca dorim sa filtram dupa permisiunea de owner punem in filtering string ul "perm"
        {
            a->filtering = "perm";
        }
        if (strncmp(arg[i], "path=", 5) == 0) // salvam path ul in variabila path din structura
        {

            char *p = strtok(arg[i], "=");
            p = strtok(NULL, "");
            a->path = p;
        }
        if (strcmp(arg[i], "parse") == 0)
        {
            a->parse = "parse";
        }
    }
    return a;
}

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
    // citim fiecare element din director
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

    // listam recursiv fiecare subfolder al folderului
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

void permisiuni(const char *cale)
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
                if ((statbuf.st_mode & 0200)) // bitul 1:read ; bitul 2:Write ; bitul 3:Execute
                {
                    printf("%s\n", fullPath);
                }
            }
        }
    }
    closedir(dir);
}

void startWith(const char *cale, const char *p)
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
                // preluat : https://www.tutorialkart.com/c-programming/c-program-check-if-string-starts-with-a-specific-substring/
                int check = 0;
                for (int i = 0; p[i] != '\0'; i++)
                {
                    if (entry->d_name[i] != p[i])
                    {
                        check = 0;
                        break;
                    }
                    check = 1;
                }

                if (check == 1)
                {
                    printf("%s\n", fullPath);
                }
            }
        }
    }
    closedir(dir);
}

void permisiuniRecursiva(const char *path)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char filePath[512];
    struct stat statbuf;

    dir = opendir(path);
    if (dir == NULL)
    {
        printf("ERROR\ninvalid directory path");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(filePath, 512, "%s%s", path, entry->d_name);
            if (lstat(filePath, &statbuf) == 0)
            {
                if ((statbuf.st_mode & 0200))
                {
                    printf("%s", filePath);
                }
                else if (S_ISDIR(statbuf.st_mode)) // se verifica directorul pentru a se putea face recursivitatea
                {
                    permisiuniRecursiva(filePath);
                }
            }
        }
    }
    closedir(dir);
}

void startWithRecursiv(const char *cale, const char *p)
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
                // preluat : https://www.tutorialkart.com/c-programming/c-program-check-if-string-starts-with-a-specific-substring/
                int check = 0;
                for (int i = 0; p[i] != '\0'; i++)
                {
                    if (entry->d_name[i] != p[i])
                    {
                        check = 0;
                        break;
                    }
                    check = 1;
                }

                if (check == 1)
                {
                    printf("%s\n", fullPath);
                }
            }
            else if (S_ISDIR(statbuf.st_mode))
            {
                startWithRecursiv(fullPath, p);
            }
        }
    }
    closedir(dir);
}

void parse(const char *cale)
{
    int fd = -1;
    char magic[5];
    int header_size = 0;
    int version = 0;
    int no_of_section = 0;
    fd = open(cale, O_RDONLY);
    if (fd == -1)
    {
        perror("Nu se poate deschide");
        return;
    }
    lseek(fd, -6, SEEK_END);
    read(fd, &header_size, 2);         // am scos valoarea lui header_size
    read(fd, &magic, 4);               // am scos valoarea lui magic
    lseek(fd, -header_size, SEEK_END); // am aflat valoarea header-ului dar fara magic si header_size , am pus valoare negativa la header_size pt ca cursorul sa merge din dreapta in stanga
    read(fd, &version, 1);             // scoatem valoarea lui version
    read(fd, &no_of_section, 1);       // valoarea lui no_of_section
    hSectiune *sectiuni = (hSectiune *)calloc(no_of_section, sizeof(hSectiune));
    magic[4] = '\0';

    for (int i = 0; i < no_of_section; i++)
    {
        read(fd, &sectiuni[i].name, 9);
        read(fd, &sectiuni[i].type, 1);
        read(fd, &sectiuni[i].offset, 4);
        read(fd, &sectiuni[i].size, 4);
        sectiuni[i].name[9] = '\0';
    }

    if (strcmp(magic, "rlSa") != 0)
    {
        printf("ERROR\nwrong magic\n");
        return;
    }
    if (version < 78 || version > 98)
    {
        printf("ERROR\nwrong version\n");
        return;
    }
    if (no_of_section < 8 || no_of_section > 18)
    {
        printf("ERROR\nwrong sect_nr\n");
        return;
    }
    for (int i = 0; i < no_of_section; i++)
    {
        if (sectiuni[i].type != 58 && sectiuni[i].type != 44)
        {
            printf("ERROR\nwrong sect_types\n");
            return;
        }
    }
    printf("SUCCESS\n");
    printf("version=%d\n", version);
    printf("nr_sections=%d\n", no_of_section);
    for (int i = 0; i < no_of_section; i++)
    {
        printf("section%d: %s %d %d\n", i + 1, sectiuni[i].name, sectiuni[i].type, sectiuni[i].size);
    }
    close(fd);
    free(sectiuni);
}

hSectiune *parse2(const char *cale)
{
    int fd = -1;
    char magic[5];
    int header_size = 0;
    int version = 0;
    int no_of_section = 0;
    fd = open(cale, O_RDONLY);
    if (fd == -1)
    {
        perror("Nu se poate deschide");
        return NULL;
    }
    lseek(fd, -6, SEEK_END);
    read(fd, &header_size, 2);         // am scos valoarea lui header_size
    read(fd, &magic, 4);               // am scos valoarea lui magic
    lseek(fd, -header_size, SEEK_END); // am aflat valoarea header-ului dar fara magic si header_size
    read(fd, &version, 1);             // scoatem valoarea lui version
    read(fd, &no_of_section, 1);       // valoarea lui no_of_section
    hSectiune *sectiuni = (hSectiune *)calloc(no_of_section, sizeof(hSectiune));
    magic[4] = '\0';

    for (int i = 0; i < no_of_section; i++)
    {
        read(fd, &sectiuni[i].name, 9);
        read(fd, &sectiuni[i].type, 1);
        read(fd, &sectiuni[i].offset, 4);
        read(fd, &sectiuni[i].size, 4);
        sectiuni[i].name[9] = '\0';
    }
    if (strcmp(magic, "rlSa") != 0)
    {
        printf("ERROR\nwrong magic\n");
        return NULL;
    }
    if (version < 78 || version > 98)
    {
        printf("ERROR\nwrong version\n");
        return NULL;
    }
    if (no_of_section < 8 || no_of_section > 18)
    {
        printf("ERROR\nwrong sect_nr\n");
        return NULL;
    }
    for (int i = 0; i < no_of_section; i++)
    {
        if (sectiuni[i].type != 58 && sectiuni[i].type != 44)
        {
            printf("ERROR\nwrong sect_types\n");
            return NULL;
        }
    }
    close(fd);
    return sectiuni;
}

void extract(const char *cale, int sectiune, int linie)
{
    int fd = -1;
    fd = open(cale, O_RDONLY);
    if (fd == -1)
    {
        printf("ERROR\ninvalid file");
        return;
    }
    hSectiune *y = parse2(cale);                 // am scos cele 4 argumente din header section
    lseek(fd, y[sectiune - 1].offset, SEEK_SET); // am setat cursorul la inceputul sectiunii
    char c, d;
    int nrlinii = 1;
    if (sectiune > y->size)
    {
        printf("ERROR\ninvalid section");
        return;
    }
    for (int i = 0; i < y[sectiune - 1].size; i++) // aflam cate linii are o sectiune
    {
        read(fd, &c, 1); // citim bit cu bit cate un caracter
        if (d == '\x0D' && c == '\x0A')
        {
            nrlinii++;
        }
        d = c;
    }
    if (nrlinii < linie)
    {
        printf("ERROR\ninvalid line");
        return;
    }
    lseek(fd, y[sectiune - 1].offset, SEEK_SET); // mutam cursorul din nou la inceputul sectiunii
    int count = 1;
    printf("SUCCESS\n");
    for (int i = 0; i < y[sectiune - 1].size; i++) // afisam linia dorita
    {
        read(fd, &c, 1);
        if (linie == nrlinii - count + 1)
        {
            printf("%c", c); // afisam caracter cu caracter linia dorita
        }
        if (d == '\x0D' && c == '\x0A')
        {
            count++;
        }
        d = c;
    }
    close(fd);
    free(y);
}

int main(int argc, char **argv)
{
    if (argc >= 2)
    {
        filtrare *op = cautareArgumente(argv, argc);
        if (strcmp(argv[1], "variant") == 0)
        {
            printf("38967");
        }
        else
        {
            if (op->list != NULL)
            {
                if (op->recursiv != NULL)
                {
                    if (op->filtering == NULL)
                    {
                        printf("SUCCESS\n");
                        listareRecursiva(op->path);
                    }
                    else
                    {
                        if (strcmp(op->filtering, "perm") == 0)
                        {
                            // afisam pentru has_perm_write
                            printf("SUCCES\n");
                            permisiuniRecursiva(op->path);
                        }
                        else
                        {
                            // afisam pentru starts_with=string,string este salvat in op->filtering
                            startWithRecursiv(op->path, op->filtering);
                        }
                    }
                }
                else
                {
                    if (op->filtering == NULL)
                    {
                        listareIterativa(op->path);
                    }
                    else
                    {
                        if (strcmp(op->filtering, "perm") == 0)
                        {
                            // afisam pentru has_perm_write
                            permisiuni(op->path);
                        }
                        else
                        {
                            // afisam pentru starts_with=string,string este salvat in op->filtering
                            startWith(op->path, op->filtering);
                        }
                    }
                }
            }
            else if (op->parse != NULL)
            {
                parse(op->path);
            }
            else if (strcmp(argv[1], "extract") == 0)
            {
                if (op->path != NULL)
                {
                    if (strncmp(argv[3], "section=", 8) == 0)
                    {
                        char *a = strtok(argv[3], "=");
                        a = strtok(NULL, "");
                        int b;
                        b = atoi(a); // transformam un string intr-un int
                        if (strncmp(argv[4], "line=", 5) == 0)
                        {
                            char *c = strtok(argv[4], "=");
                            c = strtok(NULL, "");
                            int d;
                            d = atoi(c); // transformare string in int
                            extract(op->path, b, d);
                        }
                    }
                }
            }
        }
        free(op);
    }
    return 0;
}