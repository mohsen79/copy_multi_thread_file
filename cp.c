#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#define SIZE 4096

typedef struct{
	unsigned long offset;
	unsigned long size;
}PART;

char file[50]; 
char fileout[50];

void* func1(void *arg);

int main(int argc, char *argv[]){
	char 		data[SIZE];
	struct stat 	f_stat;
	int 		fin1, fout1, numb, x, chk, i=0,j,th;
	unsigned long offset_size=0;
	
	if(argv[3] != NULL){
		 numb = atoi(argv[3]);
	} else{
		 numb = 4;
	}

	PART 		part[numb];
	pthread_t 	t[numb];

	if(argc < 3){
                puts("too few arguments");
                return -1;
        }
        strcpy(file, argv[1]);
        stat(file, &f_stat);  
        strcpy(fileout, argv[2]); 

	printf("Size%lu \n", f_stat.st_size);
	part->offset = 0;

	part[0].offset = 0;
        part[0].size = f_stat.st_size /numb;
        for(j =1; j<numb; j++){
                offset_size+=part[j-1].size;
                part[j].offset=offset_size;
                part[j].size=part[0].size;
                }

	fin1 = open(file, O_RDONLY);
	fout1 = open(fileout, O_WRONLY|O_CREAT, 0666);

		while(i < part->size){	
		x = read(fin1, data, SIZE);
		write(fout1, data, x);
		i += x;
	}

	printf("file copied\n");
	for(j = 0; j < numb; j++){
		pthread_create(&t[j], NULL, func1, &part[j]);
		pthread_join(t[j], NULL);
	}
	close(fout1);
	close(fin1);
	return 0;
}

void* func1(void *arg){
	int fin, fout, x, i;
	PART *part;
	char data[SIZE];
	
	part = (PART *)arg;
	fin = open(file, O_RDONLY);
        fout = open(fileout, O_WRONLY);
	lseek(fin, part->offset, SEEK_SET);
	lseek(fout, part->offset, SEEK_SET);
        while(i < part->size){
                x = read(fin, data, SIZE);
                write(fout, data, x);
                i += x;
        }
        printf("thread is done\n");
        close(fout);
        close(fin);
}
