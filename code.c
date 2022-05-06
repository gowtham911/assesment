#include <stdio.h>
#include <stdlib.h> // exit()
FILE *ptr_source;

void SplitbySize(FILE *ptr_source, char *argv[]) {
    
	char part_filename[200];
	FILE *ptr_part;
	size_t parts, i;
	unsigned long int part_size,written_bytes;
	int byte;

	fseek(ptr_source, 0, SEEK_END);
	parts= atol(argv[2]) ;
	
	if (parts== 0) {
		printf("Invalid part size: %ld\n", parts);
		exit(1);
	}
	part_size = (size_t) ((double) ftell(ptr_source) / parts);
	rewind(ptr_source);

	printf("\nFile will be split into %lu parts.\n", parts);

	for (i = 0; i < parts; i++) {
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
	char line [128]; 
    long int filecounter=1, linecounter=1;
    long int no_of_linestosplit;
    no_of_linestosplit=atol(argv[3]);
	
	if (no_of_linestosplit== 0) {
		printf("Invalid part size: %s\n", argv[3]);
		exit(1);
	}
	
	ptr_writefile = openforwrite(filecounter);

	while (fgets(line, sizeof line, ptr_source)!=NULL) {
		if (linecounter ==  no_of_linestosplit   ) {
		    printf("File part %ld written\n",filecounter);
			fclose(ptr_writefile);
			linecounter = 1;
			filecounter++;
			ptr_writefile = openforwrite(filecounter);
			if (!ptr_writefile)
			printf("could not open file to write");
		}
		fprintf(ptr_writefile,"%s\n", line);
		linecounter++;
		}
	fclose(ptr_source);
	
}


int main(int argc, char *argv[]) {
    if (argc != 4) {
		printf("please provid valid input");
		exit(1);
	}
  
    ptr_source = fopen(argv[1], "rb");
    if (ptr_source== NULL) {
		perror("Could not open file to split");
		exit(1);
	}
	printf("Splitting by Size...\n");
    SplitbySize(ptr_source,argv);
     ptr_source = fopen(argv[1], "rb");
    if (ptr_source== NULL) {
		perror("Could not open file to split");
		exit(1);
	}
	printf("\nSplitting by Line...\n\n");
    SplitbyLine(ptr_source,argv);
    return 0;
    
}
