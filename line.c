#include <stdio.h>
#include <stdlib.h>

FILE *openforwrite(int filecounter) {
	char fileoutputname[15];
	sprintf(fileoutputname, "file_part%d.txt", filecounter);
	return fopen(fileoutputname, "w");
}

int main(int argc, char *argv[]) {
	FILE *ptr_readfile;
	FILE *ptr_writefile;
	char line [128]; 
    long int filecounter=1, linecounter=1;
    long int no_of_linestosplit;
    no_of_linestosplit=atol(argv[1]);
	
	if (no_of_linestosplit== 0) {
		printf("Invalid part size: %s\n", argv[1]);
		exit(1);
	}
	
	if ((ptr_readfile=fopen(argv[2], "rb")) == NULL) {
        perror("Could not open file to split");
		return 1;
    }
  
	ptr_writefile = openforwrite(filecounter);

	while (fgets(line, sizeof line, ptr_readfile)!=NULL) {
		if (linecounter ==  no_of_linestosplit   ) {
		    printf("File part %ld written\n",filecounter);
			fclose(ptr_writefile);
			linecounter = 1;
			filecounter++;
			ptr_writefile = openforwrite(filecounter);
			if (!ptr_writefile)
			return 1;
		}
		fprintf(ptr_writefile,"%s\n", line);
		linecounter++;
		}
	fclose(ptr_readfile);
	return 0;
}

