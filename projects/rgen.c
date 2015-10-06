#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int geran(int,int,int);
int line_coin(int,int, int, int ,int ,int,int, int);
int main(int argc,char *argv[])
{
    int i,j,k;
    int s=10,n=5,l=5,c=20;
    char opt;
    //KEEP READING UNTIL EOF or \N
    while((opt=getopt(argc, argv, "s:l:c:n:"))!=-1){
        switch(opt){
            case 's':s=atoi(optarg);break;
            case 'n':n=atoi(optarg);break;
            case 'l':l=atoi(optarg);break;
            case 'c':c=atoi(optarg);break;
            default: break;
        }
    }
    
    int num_street;
    int *name_street,*x_street,*y_street;
    int num_segments;
    int range_xy;
    int x,y;
    int serror;
    serror=0;
    if(s<2||n<1||l<5||c<1){
        perror("Error: wrong input of s/n/l/c!");
    }else{
        int fd= -1;
        if ((fd = open("/dev/urandom", O_RDONLY)) < 0)
        {
            perror("Error: Can not open urandom!");
            return 0;
        }
        fprintf(stdout,"r\n");fflush(stdout);
        range_xy=geran(1,c,fd);//generate xy range
        sleep(geran(5,l,fd));
        num_street=geran(2,s,fd);// generate street
        sleep(geran(5,l,fd));
        name_street=(int*)malloc(num_street*sizeof(int*));
        for(i=0;i<num_street;i++){
            name_street[i]=i+1;//define each street name
            num_segments=geran(1,n,fd); //generate each street segments
            sleep(geran(5,l,fd));
            x_street=(int*)malloc((num_segments+1)*sizeof(int*));
            y_street=(int*)malloc((num_segments+1)*sizeof(int*));
            fprintf(stdout,"a \"Street%d\"",name_street[i]);fflush(stdout);
            for(j=0;j<num_segments+1;j++){
                int cycle=0;
                while (cycle<25) {
                    int statu=1;
                    x=geran(-range_xy,range_xy,fd);
                    sleep(geran(5,l,fd));
                    y=geran(-range_xy,range_xy,fd);
                    sleep(geran(5,l,fd));
                    for (k=0; k<j; k++) {
                        if (x==x_street[k]&&y==y_street[k]) {
                            statu=0;
                            break;
                        }
                        else if(j>=2){
                            if(line_coin(x_street[k],y_street[k],x_street[k+1],y_street[k+1],x_street[j-1],y_street[j-1],x,y)==0){
                                statu=0;break;
                            }
                        }
                    }
                    if (statu==1) {
                        x_street[j]=x;
                        y_street[j]=y;
                        break;
                    } else{
                        cycle++;
                    }
                }
                
                if (cycle==25) {
                    serror=1;
                    break;
                } else {
                    fprintf(stdout," (%d,%d)",x_street[j],y_street[j]);fflush(stdout);
                }
            }
            if (serror==1) {
                fprintf(stdout, "\n");
                fprintf(stderr, "Error: Failed to generate valid input for 25 simultaneous attempts!");fflush(stdout);
                break;
            } else {
                fprintf(stdout,"\n");fflush(stdout);
            }
        }
        if (serror==0) {
            fprintf(stdout,"g\n%d",EOF);fflush(stdout);
        }
    }
    
    return 0;
}

int line_coin(int x11,int y11, int x12, int y12,int x21,int y21,int x22, int y22){
    int x=1;
    if((y22-y21)*(x12-x11)==(y12-y11)*(x22-x21)){
        if((y22-y12)*(y21-y12)<0||(x22-x12)*(x21-x12)<0||(y22-y11)*(y21-y11)<0||(x22-x11)*(x21-x11)<0||(y12-y22)*(y11-y22)<0||(x12-x22)*(x11-x22)<0||(y12-y21)*(y11-y21)<0||(x12-x21)*(x11-x21)<0){
            x=0;
        }
    }
    return x;
}

int geran(int a, int b, int fd){
    int in=1;
    ssize_t read_ret;
    unsigned char *buf = NULL;
    buf = (unsigned char *)malloc(in*sizeof(unsigned char));
    read_ret = read(fd, buf, in);
    if (read_ret < 1){
        perror("Error: Can not read random");
        return b+1;
    }
    free(buf);
    return buf[0]%(b-a+1)+a;
}
