#include <stdio.h>
#include <stdlib.h>
FILE *ptr_source;

void SplitbySize(FILE *ptr_source, long int size) {
    
	char part_filename[200];
	FILE *ptr_part;
	size_t parts, i;
	unsigned long int part_size,written_bytes;
	int byte;

	fseek(ptr_source, 0, SEEK_END);
	parts= (size_t) size ;
	
	if (parts== 0) {
		printf("Invalid part size: %ld\n", parts);
		exit(1);
	}
	part_size = (size_t) ((double) ftell(ptr_source) / parts);
	rewind(ptr_source);

	printf("File will be split into %ld parts.\n", parts);

	for (i = 0; i < parts; i++) {
		printf("\nWriting part%2ld... ", i);
		sprintf(part_filename, "file_part%ld.txt", i);
		
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

void SplitbyLine(FILE *ptr_source,long int no_of_lines) {
    
	FILE *ptr_writefile;
	char line [128]; 
    long int filecounter=1, linecounter=1;
    long int no_of_linestosplit;
    no_of_linestosplit= no_of_lines;
	
	if (no_of_linestosplit== 0) {
		printf("Invalid part size: %ld\n",no_of_linestosplit );
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


int main() {
    char filename[100],c;
    int choice;
    long int size,no_of_lines;
    do
    {   
       printf("\n1.Split by Size\n2.Split by line\n3.Exit");
       choice='\0';
       printf("\nEnter your choice:");
       if (scanf("%d", &choice) != 1) {
           int c;
           while ((c = getchar()) != EOF && c != '\n')
              continue;
           if (c == EOF) {
               return 1;
           }
           choice = -1;
       }
      
       switch(choice) {
           case 1: printf("\nEnter the filename to split:\n");
                   scanf("%s",filename);
                   ptr_source = fopen(filename, "rb");
                   if (ptr_source== NULL) 
                   {
		              perror("Could not open file to split\n");
	               	  exit(1);
                  	}
                  	printf("Enter the no of units to split:");
                  	scanf("%ld",&size);
	                printf("Splitting by Size...\n");
                    SplitbySize(ptr_source,size);
                    break;
            case 2: printf("\nEnter the filename to split:\n");
                    scanf("%s",filename);
                    ptr_source = fopen(filename, "rb");
                    if (ptr_source== NULL) 
                    {
		              perror("Could not open file to split\n");
	               	  exit(1);
                  	}
                  	printf("Enter the no of lines to split:");
                  	scanf("%ld",&no_of_lines);
                  	printf("Splitting by Line...\n");
                  	SplitbyLine(ptr_source,no_of_lines);
                  	break;
            case 3: exit(1);
            default: printf("\nwrong input\n");
                     
       }
       printf("\nDo you want to split again?(y/n)\n");
       scanf("\n %c",&c);
    }while(c=='y');
    return 0;
     
}
