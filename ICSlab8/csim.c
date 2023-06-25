//521021910760 qianweike
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#define MAXLINE 512
#define MAXADDR 128
void printSummary(int hits, int misses, int evictions)
{
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
    FILE* output_fp = fopen(".csim_results", "w");
    assert(output_fp);
    fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
    fclose(output_fp);
}


int s,E,b,verbose;
int MISS=0,HIT=0,EVIC=0;
int S,B;
int TIMETAG=1;
extern char* optarg;
char* fileaddr;
int argflag=15,addrlen=0;
bool addr[MAXADDR];

int Pow2(int x)
{
	int ret=1,bas=2;
	while(x)
	{
		if(x&1)ret*=bas;
		bas*=bas,x>>=1;
	}
	return ret;
};
struct Line
{
	bool avai;
	unsigned long long tag;
	unsigned char * data;
	int ref;
};
struct Block
{
	struct Line* line;
};
struct Block* block;
void Init()
{
	block=(struct Block*)malloc(sizeof(struct Block*)*S);
	for(int i=0;i<S;i++)
	{
		block[i].line=(struct Line*)malloc(sizeof(struct Line*)*E);
		for(int j=0;j<E;j++)
		{
			block[i].line[j].data=(unsigned char *)malloc(sizeof(unsigned char *)*B);
			block[i].line[j].avai=false;
			block[i].line[j].tag=0;
			block[i].line[j].ref=0;
		}
	}
}
void Rest()
{
	// for(int i=0;i<S;i++)
	// {
	// 	for(int j=0;j<E;j++)
	// 		free(block[i].line[j].data);
	// 	free(block[i].line); 
	// }
	free(block);
}



void work(int w)
{
	unsigned long long _S=0,_B=0,_T=0;
    unsigned long long tmp=1;
    for(int i=0;i<b;i++)
    {
    	_B+=addr[i]*tmp;
        tmp<<=1;
	}
    tmp=1;
    for(int i=b;i<b+s;i++)
    {
    	_S+=addr[i]*tmp;
        tmp<<=1;
	}
    tmp=1;
    for(int i=b+s;i<addrlen;i++)
    {
    	_T+=addr[i]*tmp;
        tmp<<=1;
	}
    if(verbose)printf("t=%lld s=%lld b=%lld\n",_T,_S,_B);
	int eflag=-1,eref=1<<29;
	for(int i=0;i<E;i++)
	{
		if(block[_S].line[i].avai&&block[_S].line[i].tag==_T)
		{
			//hit
			if(verbose)printf("hit ");
			HIT++;
			block[_S].line[i].ref=TIMETAG++;
			return;
		}
		else if(block[_S].line[i].ref<eref)
		{
			eref=block[_S].line[i].ref;
			eflag=i;
		}
	}
	//miss
	if(verbose)printf("miss ");
	MISS++;
	if(eref==0)
	{
		block[_S].line[eflag].avai=1;
		block[_S].line[eflag].tag=_T;
		block[_S].line[eflag].ref=TIMETAG++;
		return;
	}
	//eject
	if(verbose)printf("eviction ");
	EVIC++;
	block[_S].line[eflag].tag=_T;
	block[_S].line[eflag].ref=TIMETAG++;
}
void parseline(char* cmdline)
{
	if(verbose)
    {
        printf("parsing:%s\n",cmdline);
    }
    char instr=cmdline[1];
    int index1,index2;
    int i=3;
    while(1)
    {
        if(cmdline[i]==',')index1=i;
        if(cmdline[i]=='\0')
        {
            index2=i;
            break;
        }
        i++;
    }
    addrlen=0;
    for(i=index1-1;i>=3;i--)
    {
        int tmp=(cmdline[i]>='0'&&cmdline[i]<='9'?cmdline[i]-'0':cmdline[i]-'a'+10);
        addr[addrlen++]=tmp&1;
        addr[addrlen++]=tmp&2;
        addr[addrlen++]=tmp&4;
        addr[addrlen++]=tmp&8;
    }
    int w=0;
    for(i=index1+1;i<index2;i++)
        w*=10,w+=cmdline[i]-'0';
    switch(instr)
    {
        case 'L':
            work(w);
            break;
        case 'S':
            work(w);
            break;
        case 'M':
            work(w),work(w);
            break;
        default:
            printf("WRONG COMMAND!\n");
            exit(0);

    }
    if(verbose)printf("\n");
}
int main(int argc, char **argv)
{
	verbose=0;
	char cmdline[MAXLINE];
    char c;
	while ((c = getopt(argc, argv, "vs:E:b:t:")) != -1) {
        switch (c) {
	        case 'v':
	        	verbose=1;
		    break;
	        case 's':
	        	s=atoi(optarg);
	        	S=Pow2(s);
	        	argflag-=(argflag&1);
		    break;
	        case 'E':
	            E=atoi(optarg);
	        	argflag-=(argflag&2);
		    break;
	        case 'b':
	            b=atoi(optarg);
	            B=Pow2(b);
	        	argflag-=(argflag&4);
		    break;
	        case 't':
	        	fileaddr=optarg;
	        	argflag-=(argflag&8);
		    break;
		}
    }
    if(argflag)
    {
    	printf("ARGUMENT WRONG!\n");
    	exit(0);
	}
	Init();
	
    FILE * optfile=fopen(fileaddr,"r");
	while(1)
	{
		fgets(cmdline,MAXLINE,optfile);
		if(feof(optfile))break;
		if(cmdline[0]=='I')
        {
            if(verbose)
                printf("Get Instr:%s\n",cmdline);
            continue;
        }
		parseline(cmdline);
	}
    printSummary(HIT, MISS, EVIC);
    Rest();
    return 0;
}
