#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

int main ( int argc, char *argv[] )
{
    //-----------------------go to upper directory-------------------
    char *directory="..";
    int ret;
    ret=chdir(directory);


    //------------------------showing current directory--------------
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        fprintf(stdout, "Current working dir: %s\n", cwd);
    }
    else
    {
        perror("getcwd() error");
        return 0;
    }
    //----------------------------making new directory---------------
    struct stat st = {0};
    char newDir[80];
    strcpy(newDir,cwd);
    strcat(newDir,"/newDir");
    //printf(newDir);
    if (stat(newDir, &st) == -1) {
        mkdir(newDir, 0700);
    }

    //-----------------------List everything in the current directory
    DIR * d;
    char * dir_name = ".";

    /* Open the current directory. */

    d = opendir (dir_name);

    if (! d) {
        fprintf (stderr, "Cannot open directory '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
    while (1) {
        struct dirent * entry;

        entry = readdir (d);
        if (! entry) {
            break;
        }
        printf ("%s\n", entry->d_name);
    }
    /* Close the directory. */
    if (closedir (d)) {
        fprintf (stderr, "Could not close '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }

    return 0;
    //printf("result:%f",ret);

    //    if ( argc != 2 ) /* argc should be 2 for correct execution */
    //    {
    //        /* We print argv[0] assuming it is the program name */
    //        printf( "usage: %s filename", argv[0] );
    //    }
}
