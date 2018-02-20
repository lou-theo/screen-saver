#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	int n,i;
	scanf("%d", &n);
	char **T;

	T=(char**)malloc(n*sizeof(char*));
	for(i=0;i<n;i++)
	{
		T[i]=(char*)malloc(20*sizeof(char));
	}

	sprintf(T[0], "lol0\n");
	sprintf(T[1], "lol1\n");
	sprintf(T[2], "\n");	
	sprintf(T[3], "\n");

	for (i = 0 ; i < 4 ; i++)
	{
		printf("%s", T[i]);
	}

	return 0;
}

