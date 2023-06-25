#include<stdio.h>
#include<stdlib.h>
int main()
{
	char *c="0xfffffffffffffaaa";
	puts(c);
	long long num=strtoull(c,NULL,0);
	printf("%lld\n0x%llx\n",num,num);
}
