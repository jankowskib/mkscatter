/*   mkscatter.c
 *      
 *   Copyright 2013 Bartosz Jankowski
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */
 
#include <stdio.h>
#include <stdlib.h>

static void usage(char **argv)
{
	printf("==================================================\n"
		   "MTK dumchar_info -> scatter.txt Converter by lolet\n"
		   "==================================================\n");
	printf("Usage: %s input output\n", argv[0]);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	FILE * f, *o;
	char verbose = 0;
	char *fname = 0, *oname = 0, syspart[200] = {0};
	
	if (argc == 3 )
	{
		f = fopen(argv[1], "r");
		if(!f)
		{
			printf("Input file %s doesn't exists!\n", argv[1]);
			exit(EXIT_FAILURE);
		}
		o = fopen(argv[2], "wb");
		if(!o)
		{
			printf("Output file %s cannot be created!\n", argv[2]);
			fclose(f);
			exit(EXIT_FAILURE);
		}
		fname = argv[1];
		oname = argv[2];
	}
	else	
		usage(argv);

	if(verbose) {
	
	printf("==================================================\n"
		   "MTK dumchar_info -> scatter.txt converter by lolet\n"
		   "==================================================\n");
	printf ("Input file: %s\n", fname);
	}
	char head[1024] = {0};
	char buffer[1024] = {0};
	fscanf(f,"%s\t",&head);
	fgets(buffer, 1024, f);
	
	if(strcmp(head,"Part_Name"))
	{
		printf("ERROR: Unknown file header (%s)", head);
		fclose(f);
		return EXIT_FAILURE;
	}
	int offset = 0;
	while(fgets(buffer, 1024, f))
	{
	char * part = strtok(buffer, " ");
	char * size = strtok(NULL, " ");
	
	int i;
	for(i=0; part[i]; ++i)
		part[i] = toupper(part[i]);
	
	size+=2;
	sscanf(size,"%x",&i);
	
	if(!strcmp(part, "BMTPOOL"))
	{
		if(verbose)
		printf("Skipping: %s 0x%x\n", part, offset);
		continue;
	}
	if(!strcmp(part, "ANDROID") && !syspart[0])
	{
	if(verbose)
		printf("Found system partition..\n");
	strtok(NULL, "/");
	//strtok(NULL, " ");
	size = strtok(NULL, " \n");
	syspart[0]='/';
	strcpy(&syspart[1], size);
	}
	if(!strcmp(part,"PART_NAME:PARTITION"))
	{
		if(verbose)
		printf("Finishing...\n");
		break;
	}
	fprintf(o,"%s 0x%x\xA", part, offset);
	if(verbose)
	printf("Writting: %s 0x%x\n", part, offset);
	offset+=i;
	}
	
	fprintf(o,"\xA\xA");
	fclose(f);
	fclose(o);
	printf("%s",syspart);
	return EXIT_SUCCESS;
}