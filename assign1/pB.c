#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *strrev(char *str)
{
    char *p1, *p2;

    if (! str || ! *str)
        return str;
    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
}

int main(int argc,char *argv[]){
    int pipefd[2];
    pid_t p2id;
    char buf[1024];

    if(argc !=2){
        fprintf(stderr,"Usage: %s <string>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    if(pipe(pipefd)==-1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    p2id=fork();//creating p2 in p1 process
    if(p2id==-1){
        perror("fork");
        exit(EXIT_FAILURE);

    }
    if(p2id==0){//which means we are in the p2 process
        //p2 read from pipe
        char buf[1024];

        close(pipefd[1]);
        int size=read(pipefd[0],buf,1024);
        if(size>=0){
            buf[size]=0;//terminate the string
            printf("p2,string recieved:%s, process id %d, parent id %d",buf,getpid(),getppid());}


        pid_t p3id=fork();//create process 3 in p2
        if(p3id==-1){
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if(p3id==0){//we are in p3 process we read the string from p2 and send it to p1
            char buf[1024];

            close(pipefd[1]);
            int size=read(pipefd[0],buf,1024);
            if(size>=0){
                buf[size]=0;//terminate the string
                printf("p3,string recieved:%s, process id %d, parent id %d",buf,getpid(),getppid());}

            close(pipefd[0]);
            open(pipefd[1]);
            write(pipefd[1], strrev(buf),strlen(buf));
            close(pipefd[1]); //Reader is seeing the EOF
            wait(NULL);       //WAIT FOR THE CHILDREN TO FINISH
            _exit(EXIT_SUCCESS);

        }else{//we are in p2 process and we write the string reversed to p3
            close(pipefd[0]);
            open(pipefd[1]);
            write(pipefd[1],strrev(buf),strlen(buf));
            close(pipefd[1]); //Reader is seeing the EOF
            wait(NULL);       //WAIT FOR THE CHILDREN TO FINISH
            _exit(EXIT_SUCCESS);
        }

    }else{
        //we are in p1 process and we write a string to p2 using the pipe
        close(pipefd[0]);
        write(pipefd[1],argv[1],strlen(argv[1]));
        close(pipefd[1]); //Reader is seeing the EOF


        wait(NULL);       //WAIT FOR THE CHILDREN TO FINISH
        open(pipefd[0]);
        int size=read(pipefd[0],buf,1024);
        if(size>=0){
            buf[size]=0;//terminate the string
            printf("p1,string recieved:%s, process id %d, parent id %d",buf,getpid(),getppid());}
        exit(EXIT_SUCCESS);


    }
}

