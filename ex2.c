/*
 * ex2.c
 *
 *  Created on: Nov 19, 2017
 *      Author: hanani
 */
#include<stdio.h>
#include<string.h>

#define OS_ERROR 'x'

typedef enum {
	WIN = 1,
	UNIX,
	MAC,
}OS_TYPE;

void swapBytes(char* a, char* b)
{
	char temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

char findNewLineChar(OS_TYPE* osType, const char* srcOs,
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
	//should not get here as it stated in the instructions there will be no faulty arguments
	else
	{
		return OS_ERROR;
	}

	if(strcmp("-unix", destOs) == 0)
	{
		return '\n';
	}
	else if(strcmp("-mac", destOs) == 0)
	{
		return '\r';
	}
	else if(strcmp("-win", destOs) == 0)
	{
		return '\r';
	}
	//should not get here as it stated in the instructions there will be no faulty arguments
	else
	{
		return OS_ERROR;
	}
}


/*
 * copies one file to another upon given flags,
 * two flags for source and destination file format -win/-mac/-unix
 * the last flag is -swap/-keep, deciding wether or not to swap bytes of file.
 */
void copyFiles(char* source, char* destination,
		char* srcOs, char* destOs, char* swapFlag)
{
	FILE *src, *dest;
	int check = 0, isNewLine = 0, endian = 1, swap = 0;
	OS_TYPE osType;
	char newLine, buffer[2];

	newLine = findNewLineChar(&osType, srcOs, destOs);
	if(newLine == OS_ERROR)
	{
		//error
		return;
	}

	if(strcmp(swapFlag, "-swap") == 0)
	{
		swap = 1;
	}

	src = fopen(source, "rb");
	dest = fopen(destination, "wb");
	if((src == NULL) || (dest == NULL))
	{
		//error
		return;
	}
	//copy loop
	while((check = fread(buffer, sizeof(buffer), sizeof(char), src)) != 0)
	{
		switch(osType)
		{
			case WIN:
				if((buffer[0] == '\r') || (buffer[1] == '\r'))
				{
					//if check equals 0, it happens because we read a \r
					//and then the file ended - meaning it is not a windows file
					if((check = fread(buffer, sizeof(buffer), sizeof(char), src)) == 0)
					{
						//error
						return;
					}
					isNewLine = 1;
				}
				break;
			case UNIX:
				if((buffer[0] == '\n') || (buffer[1] == '\n'))
				{
					isNewLine = 1;
				}
				break;
			case MAC:
				if((buffer[0] == '\r') || (buffer[1] == '\r'))
				{
					isNewLine = 1;
				}
				break;
			default:
				break;
		}

		if(isNewLine)
		{
			//checking if big or little endian
			if((buffer[0] == '\0'))
			{
				endian = 1;
			}
			else if((buffer[1] == '\0'))
			{
				endian = 0;
			}

			isNewLine = 0;
			//only destination windows has a special need to deal,
			//the rest are dealt with coppying newLine
			if(strcmp(destOs,"-win") == 0)
			{
				buffer[endian] = newLine;
				if(swap)
				{
					swapBytes(&buffer[0], &buffer[1]);
				}
				fwrite(buffer, sizeof(buffer), sizeof(char), dest);

				buffer[endian] = '\n';
				//resetting the other buffer slot
				buffer[(endian + 1) % 2] = '\0';
				if(swap)
				{
					swapBytes(&buffer[0], &buffer[1]);
				}
				fwrite(buffer, sizeof(buffer), sizeof(char), dest);
				continue;
			}
			else
			{
				buffer[endian] = newLine;
			}
		}

		if(swap)
		{
			swapBytes(&buffer[0], &buffer[1]);
		}

		if(fwrite(buffer, sizeof(buffer), sizeof(char), dest) < check)
		{
			//error
			return;
	    }
	}
	fclose(src);
	fclose(dest);
}

/*
 * coppies one file to another, regardless of os type
 */
void copyFilesSimple(char* source, char* destination)
{
	FILE *src, *dest;
	char buffer[2], check;
	src = fopen(source, "rb");
	dest = fopen(destination, "wb");
	if((src == NULL) || (dest == NULL))
	{
		//error
		return;
	}
	while((check = fread(buffer, 2, 1, src)) != 0)
	{
		if(fwrite(buffer, 2, 1, dest) != check)
		{
			//error
			return;                                           // or other action
	    }
	}
	fclose(src);
	fclose(dest);
}

/*
 * the main function that runs the software
 */
int main(int argc, char* argv[])
{
	switch(argc)
	{
		case 3:
			copyFilesSimple(argv[1], argv[2]);
			break;
		case 5:
			copyFiles(argv[1], argv[2], argv[3], argv[4], "0");
			break;
		case 6:
			copyFiles(argv[1], argv[2], argv[3], argv[4], argv[5]);
			break;
		default:
			//error;
			break;
	}
	return 0;
}
