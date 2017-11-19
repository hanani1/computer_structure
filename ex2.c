/*
 * ex2.c
 *
 *  Created on: Nov 19, 2017
 *      Author: hanani
 */

#include<stdio.h>

void copyFiles(char* source, char* destination)
{
	FILE *src, *dest;
	int check;
	char buffer[2];
	src = fopen(source, "rb");
	dest = fopen(destination, "wb");
	if((src == NULL) || (dest == NULL))
	{
		printf("problem opening source or dest file, exiting\n");
		return;
	}
	while((check = fread(buffer, 2, 1, src)) != 0)
	{
		printf("%s\n", buffer);
		if(fwrite(buffer, 2, 1, dest) != check)
		{
			printf("error writing to dest\n");
			return;                                           // or other action
	    }
	}
	fclose(src);
	fclose(dest);
	//return;
}


int main(int argc, char* argv[])
{
	switch(argc)
	{
		case 3:
			copyFiles(argv[1], argv[2]);
			break;
		default:
			printf("wrong amount of parameters");
			break;
	}
	return 0;
}
