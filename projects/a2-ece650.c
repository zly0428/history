#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int nsize=0;//V value
char c;
int statu=0;// start

int i=0;
int j=0;
int nstart=0;
int nend=0;
int lstart=0;
int lend=0;
int **metrixD,**metrixX;

void floyd(int **A,int **C,int s,int e);
int main(int argc, char *argv[])
{
    char number[10];
	number[0]='\0';
	char standnumber[10];
	standnumber[0]='\0';
    int xxxxx=atoi(argv[1]);
    int printchar=1;
	while(!feof(stdin)){
		c=getc(stdin);
		switch(c){
			case 'V':
            case 'v':statu=1; if(printchar){fprintf(stdout,"%c",c);}break;
			case 'E':
			case 'e':if(statu!=-1){statu=5;} if(printchar){fprintf(stdout,"%c",c);}break;
			case 'S':
            case 's':if(statu!=-1){statu=14;printchar=0;} if(printchar){fprintf(stdout,"%c",c);}break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if(statu==2||statu==8||statu==10||statu==15||statu==17) {
					i=0 ;
					while(number[i]!='\0'){i++;}
					number[i]=c;
					number[i+1]='\0';
					statu=statu+1;
				}
				else if(statu==3||statu==9||statu==11||statu==16||statu==18) {
					i=0 ;
					while(number[i]!='\0'){i++;}
					number[i]=c;
					number[i+1]='\0';
				}
				if(printchar){fprintf(stdout,"%c",c);}break;
			case ' ':
			case '=': 
				if(statu==1||statu==5||statu==14) {
					statu=statu+1;
				}
				else if(statu==16){
					lstart=atoi(number);
					strcpy(number,standnumber);
					statu=17;
				}
				if(printchar){fprintf(stdout,"%c",c);}break;
			case '\n':
				if(statu==3){
					nsize=atoi(number);
					metrixD=(int**)malloc(nsize*sizeof(int*));
					metrixX=(int**)malloc(nsize*sizeof(int*));
					for(i=0;i<nsize;i++) {
						metrixD[i]=(int*)malloc(nsize*sizeof(int));
						metrixX[i]=(int*)malloc(nsize*sizeof(int));
						for(j=0;j<nsize;j++){
							if(i==j){
								metrixD[i][j]=0;
								metrixX[i][j]=0;
							}
							else{
								metrixD[i][j]=nsize+1;
								metrixX[i][j]=nsize+1;
							}
						}
					}
					strcpy(number,standnumber);
					statu=4;
                    break;
				}
				else if(statu==12){
					statu=13;
                    if (dup2(atoi(argv[1]), STDIN_FILENO) < 0) {
                        perror("Error: Can not dup2 to keyboard!");
                    }
				}
				else if(statu==18){
					lend=atoi(number);
					strcpy(number,standnumber);
					if(lstart > nsize-1 || lend > nsize-1){
						fprintf(stdout,"Error: The point is out of range!\n");
						break;
					}
					floyd(metrixX,metrixD, lstart, lend);
					statu=13;
				}
				if(printchar){fprintf(stdout,"\n");}break;
			case '{':
				if(statu==6){
					statu=7;
				}if(printchar){fprintf(stdout,"%c",c);}break;
			case '}':
				if(statu==12)
				{
					statu=12;
				}
				if(printchar){fprintf(stdout,"%c",c);}break;
			case '<':if(statu==7){
				statu=8;
            }
				if(printchar){fprintf(stdout,"%c",c);}break;
			case '>':
				if(statu==11){
					nend=atoi(number);
					strcpy(number,standnumber);
					if(nstart > nsize-1 || nend > nsize-1){
						fprintf(stdout,"Error: <%d,%d> is out of range V %d\n",nstart,nend,nsize);
                        statu=-1;
                        break;
					}
					else{
						metrixD[nstart][nend]=1;
						metrixD[nend][nstart]=1;
					}
					statu=12;
				}
				if(printchar){fprintf(stdout,"%c",c);}break;
			case ',':
				if(statu==9){
					nstart=atoi(number);
					strcpy(number,standnumber);
					statu=10;
				}
				else if(statu==12){
					statu=8;
				}
				if(printchar){fprintf(stdout,"%c",c);}break;
			default:
                break;
		}
        
    }//end while
	return 0;
}
void floyd(int **A,int **C,int s,int e)  //A是路径长度矩阵，C是有向网络G的带权邻接矩阵
{
    int i,j,k,next;
    int max=nsize+1;
    int n=nsize;
    int path[nsize][nsize];
    for(i=0;i<n;i++)//设置A和path的初值
    {
        for(j=0;j<n;j++)
        {
            if(C[i][j]!=max)
                path[i][j]=j;   //j是i的后继
            else
                path[i][j]=0;
            A[i][j]=C[i][j];
        }
    }
    for(k=0;k<n;k++)
        //做n次迭代，每次均试图将顶点k扩充到当前求得的从i到j的最短路径Pij上
    {
        for(i=0;i<n;i++)
        {
            for(j=0;j<n;j++)
            {
                if(A[i][j]>(A[i][k]+A[k][j]))
                {
                    A[i][j]=A[i][k]+A[k][j];  //修改长度
                    path[i][j]=path[i][k];    //修改路径
                }
            }
        }
    }
    next=path[s][e];        //next为起点i的后继顶点
    if(next==0)
        fprintf(stdout,"Error: From Point %d can not get Point %d\n",s,e);
    else       //Pij存在
    {
        fprintf(stdout,"%d",s);
        while(next!=e)
        {
            fprintf(stdout,"-%d",next);  //打印后继点
            next=path[next][e];        //继续找下一个后继点
        }
        fprintf(stdout,"-%d\n",e);       //打印终点
    }
    
}
