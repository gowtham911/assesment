#include <stdio.h>
#include <stdlib.h> // exit()
#include <string.h>
FILE *ptr_source;

void SplitbySize(FILE *ptr_source, char *argv[]) {
    
	char part_filename[200];
	FILE *ptr_part;
	size_t part_size, i;
	unsigned long int parts,written_bytes,file_size;
	int byte;
	fseek(ptr_source, 0, SEEK_END);
	part_size= (size_t) (atol(argv[3])*1024) ;
	file_size=ftell(ptr_source);
	if (part_size== 0) {
		printf("Invalid part size: %ld\n", part_size);
		exit(1);
	}
	else if(file_size<part_size)
	{
	    printf("File size is less than part size. please provide large file");
	    exit(1);
	}
	parts= (size_t) ((double) file_size / part_size);
	if (file_size%part_size!=0)
       {
           parts=parts+1;
       }
	rewind(ptr_source);
	printf("File will be split into %lu parts.\n", parts);
	for (i = 1; i <= parts; i++) {
		printf("Writing part%2ld... ", i);
		sprintf(part_filename, "%s.part%02ld", argv[1], i);
		
		if ((ptr_part = fopen(part_filename, "wb")) == NULL) {
			perror("Could not open part for writing");
			exit(1);
		}

		for (written_bytes = 0; written_bytes < part_size &&
		    (byte = fgetc(ptr_source)) != EOF; written_bytes++)
		{
			fputc(byte, ptr_part);
		}

		printf("(%lu bytes)\n", written_bytes);
		fclose(ptr_part);
	}
	
	fclose(ptr_source);

}

FILE *openforwrite(int filecounter) {
	char fileoutputname[15];
	sprintf(fileoutputname, "file_part%d.txt", filecounter);
	return fopen(fileoutputname, "w");
}

void SplitbyLine(FILE *ptr_source,char *argv[]) {
    
	FILE *ptr_writefile;
    long int filecounter=1, linecounter=0;
    char c;
    long int no_of_linestosplit;
    no_of_linestosplit=atol(argv[3]);
	
	if (no_of_linestosplit== 0) {
		printf("Invalid part size: %s\n", argv[3]);
		exit(1);
	}
	
	ptr_writefile = openforwrite(filecounter);

    for (c = getc(ptr_source); c != EOF; c = getc(ptr_source))
    {
		if (linecounter ==  no_of_linestosplit   ) {
		    printf("File part %ld written\n",filecounter);
			fclose(ptr_writefile);
			linecounter = 0;
			filecounter++;
			ptr_writefile = openforwrite(filecounter);
			if (!ptr_writefile)
			printf("could not open file to write");
		}
		fprintf(ptr_writefile,"%c", c);
		if (c == '\n')
            linecounter = linecounter + 1;
	}
        printf("File part %ld written\n",filecounter);
	fclose(ptr_source);
	
}


int main(int argc, char *argv[]) {
    if (argc != 4) {
		printf("please provid valid input");
		exit(1);
	}
	int res1,res2;
    res1=strcmp(argv[2], "-s");
    res2=strcmp(argv[2], "-n");
    if(res1==0)
    {
        ptr_source = fopen(argv[1], "rb");
        if (ptr_source== NULL) {
            perror("Could not open file to split");
		    exit(1);
            
        }
        printf("Splitting by Size...\n");
        SplitbySize(ptr_source,argv);
    } 
    else if(res2==0)
    {
        ptr_source = fopen(argv[1], "rb");
        if (ptr_source== NULL) {
            perror("Could not open file to split");
		    exit(1);
            
        }
        printf("Splitting by Line...\n");
        SplitbyLine(ptr_source,argv);
    }
    else 
    {
        printf("Please provide the second argument in correct format ('-s'/'-n')");
    }
    return 0;
    
}
