#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *env[])
{
    int i;
    char *ptr = getenv("MYVAL");
    if (ptr == NULL) {
        printf("have no MYVAL\n");
    }else {
        printf("%s\n", ptr);
    }
    /*
    extern char **environ;
    for (i = 0; environ[i] != NULL; i++) {
        printf("env[%d]=[%s]\n", i, environ[i]);
    }
    */
    return 0;
}
