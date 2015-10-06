#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[])
{
    int pipe_fd[2];
    int rtn1,rtn2;
    int pid1,pid2;
    if (pipe(pipe_fd)<0) {
        perror("Error: Can not pipe!");return 0;
    }
    if (!(pid1=fork()))//first child
    {
        //for rgen
        if(!(pid2=fork())){
            close(pipe_fd[0]);//close read
            if (dup2(pipe_fd[1], STDOUT_FILENO) < 0) {
                perror("Error: Can not dup2!");
            }
            if (execvp("./rgen",argv)<0) {
                perror("Error: Can not exec rgen!");return 0;
            }
        }
        else{
            wait(&pid2);
            //close(pipe_fd[1]);
            if (dup2(pipe_fd[0], STDIN_FILENO)<0)
            {
                perror("Error: a1-ece650 can not read from pipe!");return 0;
            }
            
            if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
            {
                perror("Error: Can not dup2!");
            }
            if(execl("/usr/bin/python", "/usr/bin/python", "./a1-ece650.py", STDIN_FILENO)<0)
            {
                perror("Error: Can not exec a1-ece650!");return 0;
            }
        }
    }
    else
    {
        wait(&pid1);
        //char r_buf[10000];
        //int r_num;
        //r_num=read(pipe_Fd[0],r_buf,10000);
        int fid;
        fid=dup(STDIN_FILENO);
        close(pipe_fd[1]);
        if (dup2(pipe_fd[0], STDIN_FILENO)<0)
        {
            perror("Error: a2-ece650 can not read from pipe!");return 0;
        }
        //close(pipe_fd[0]);
        //int i=0;
        //char c;
        //while(!feof(stdin)){
        //    c=getc(stdin);
        //    fprintf(stdout,"%c",c);
        //    i++;
        //}
        //freopen("CON","w",stdin);
        //if (dup2(oldstdout, STDIN_FILENO) < 0) {
        //    perror("Error: Can not dup2!");
        //}
        //close(pipe_fd[0]);
        char s[10];
        sprintf(s,"%d",fid);
        argv[1]=s;
        if (execvp("./a2-ece650",argv)<0) {
            perror("Error: Can not exec a2-ece650!");return 0;
        }
    }
    return 0;
}