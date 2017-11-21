/*
 * ex2.c
 *
 *  Created on: Nov 19, 2017
 *      Author: hanani
 */

#include<stdio.h>
#include<string.h>

typedef enum {
	WIN = 1,
	UNIX,
	MAC,
}OS_TYPE;

char* findNewLineChar(OS_TYPE* osType, const char* srcOs,
		const char* destOs)
{
	if(strcmp(srcOs, "-unix") == 0)
	{
		*osType = UNIX;
	}
	else if(strcmp(srcOs, "-win") == 0)
	{
		*osType = WIN;
	}
	else if(strcmp(srcOs, "-mac") == 0)
	{
		*osType = MAC;
	}
	else
		return NULL;

	if(strcmp("-unix", destOs) == 0)
		return "\n";
	else if(strcmp("-mac", destOs) == 0)
		return "\r";
	else if(strcmp("-win", destOs) == 0)
		return "\r\n";
	else
		return NULL;
}

void copyFiles(char* source, char* destination,
		char* srcOs, char* destOs, char* swapFlag)
{
	FILE *src, *dest;
	int check = 0, isNewLine = 0;
	OS_TYPE osType;
	char *newLine, buffer[2];
	newLine = findNewLineChar(&osType, srcOs, destOs);
	src = fopen(source, "rb");
	dest = fopen(destination, "wb");
	if((src == NULL) || (dest == NULL))
	{
		printf("problem opening source or dest file, exiting\n");
		return;
	}
	//copy loop
	while((check = fread(buffer, 2, 1, src)) != 0)
	{
		switch(osType)
		{
			case WIN:
				if(buffer[0] == '\r')
				{
					//if check equals 0, it happens because we read a \r
					//and then the file ended - meaning it is not a windows file
					if((check = fread(buffer, 2, 1, src)) == 0)
					{
						printf("wrong type of source file\n");
						return;
					}
					isNewLine = 1;
				}
				break;
			case UNIX:
				if(buffer[0] == '\n')
				{
					isNewLine = 1;
				}
				break;
			case MAC:
				if(buffer[0] == '\r')
				{
					isNewLine = 1;
				}
				break;
			default:
				break;
		}

		if(isNewLine)
		{
			if(strcmp(destOs,"-win") == 0)
			{
				buffer[0] = '\r';
				fwrite(buffer, 1, sizeof(buffer), dest);
				buffer[0] = '\n';
				fwrite(buffer, 1, sizeof(buffer), dest);
				continue;
			}
			else
				strcpy(buffer, newLine);
			isNewLine = 0;
		}

		if(fwrite(buffer, 1, sizeof(buffer), dest) < check)
		{
			printf("error writing to dest\n");
			return;                                           // or other action
	    }
	}
	fclose(src);
	fclose(dest);
}


int main(int argc, char* argv[])
{
	switch(argc)
	{
		case 3:
			copyFiles(argv[1], argv[2], "0", "0", "0");
			break;
		case 5:
			copyFiles(argv[1], argv[2], argv[3], argv[4], "0");
			break;
		default:
			printf("wrong amount of parameters");
			break;
	}
	return 0;
}
