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

char file[50]; /* source file name */
char fileout[50]; /*target file name */

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
	pthread_t 	t[numb]; //treads to share

	if(argc < 3){
                puts("Not enough arguments.");
		puts("parallelCopy sourceFile targetFile");
                return -1;
        }
        strcpy(file, argv[1]);
        stat(file, &f_stat); // getting the meta info of file
        strcpy(fileout, argv[2]); 
	//strcpy(&numb, argv[3]);
	//scanf("%d how many threads would you like to use:", &numb);

	printf("Size of file is %lu \n", f_stat.st_size);
	part->offset = 0;

	//diviging the siz of file in 5 parts
	//part1.size = f_stat.st_size / 5;
        //part2.offset = part1.size;
        //part3.size = part1.size;
        //part4.offset = part2.offset + part3.size;
        //part5.size = f_stat.st_size - part4.offset;

	part[0].offset = 0;
        part[0].size = f_stat.st_size /numb;
        for(j =1; j<numb; j++){
                offset_size+=part[j-1].size;
                part[j].offset=offset_size;
                part[j].size=part[0].size;
                }
	
	/* creating part1 by main thread */
	fin1 = open(file, O_RDONLY);
	fout1 = open(fileout, O_WRONLY|O_CREAT, 0666);

	/*creating 3 threads to copy 2 n 3rd part of file */
        //pthread_create(&t1, NULL, func1, &part2);
        //pthread_create(&t2, NULL, func1, &part3);
        //pthread_create(&t3, NULL, func1, &part4);
        //pthread_create(&t4, NULL, func1, &part5);
	while(i < part->size){	
		x = read(fin1, data, SIZE);
		write(fout1, data, x);
		i += x;
	}

	//pthread_join(t1, NULL); //making main to wait for t1
	//pthread_join(t2, NULL); //making main to wait for t2
	//pthread_join(t3, NULL); //making main to wait for t3
        //pthread_join(t4, NULL); //making main to wait for t4	
	printf("file is copied\n");
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
        printf("thread is done.\n");
        close(fout);
        close(fin);
}
