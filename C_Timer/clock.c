#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <ctype.h>
#include "lib_functions.h"

struct stTime{
	int seconds;
	int minutes;
	int hours;
};

struct stTime main_time={0,0,0};
struct stTime stopping_time={-1,-1,-1};
char mode = 'n';
int flag = 0;



void ExtractTimeAsStrings(char sec[3], char min[3], char h[3], char str_main[8]){
	sec[0] = str_main[0];
	sec[1] = str_main[1];
	sec[3] = '\0';
	min[0] = str_main[3];
	min[1] = str_main[4];
	min[3] = '\0';
	h[0] = str_main[6];
	h[1] = str_main[7];
	h[3] = '\0';
}
void ExtractTimeAsNumbers(int *sec_number, int *min_number, int *h_number, char str_main[8]){
	char sec[3], min[3], h[3];

	sec[0] = str_main[0];
	sec[1] = str_main[1];
	sec[3] = '\0';
	*sec_number = atoi(sec);

	min[0] = str_main[3];
	min[1] = str_main[4];
	min[3] = '\0';
	*min_number = atoi(min);

	h[0] = str_main[6];
	h[1] = str_main[7];
	h[3] = '\0';
	*h_number = atoi(h);
}

void PrintError(){
	printf("Error\n\a");
	printf("To get the executable file from the clock.c file -> `gcc clock.c -o clock`\n");
	printf("To start from 00:00:00 -> `./clock`\n");
	printf("To start from a specific time -> `./clock your_time_here` example: `./clock 10:20:01`\n");
	printf("To start from 00:00:00 and stop at a specific time -> `./clock s your_time_here` example: `./clock s 00:25:00`\n");
	printf("To stop the program -> `ctrl \\` or `ctrl c`\n");
}

void ValidateArgs(int argc, char *argv[]){
	switch(argc){
		case 1:{break;}
		case 2:{
			if(strlen(argv[1]) != 8){
				PrintError();
				exit(1);
			}
			else if(!(isdigit(argv[1][0]) &&
				isdigit(argv[1][1]) &&
				(argv[1][2] == ':') &&
				isdigit(argv[1][3]) &&
				isdigit(argv[1][4]) &&
				(argv[1][5] == ':') &&
				isdigit(argv[1][6]) &&
				isdigit(argv[1][7])
				)){
					PrintError();
					exit(0);
				}
			break;
		}
		case 3:{
			if(strlen(argv[2]) != 8 || strlen(argv[1]) != 1 || argv[1][0] != 's'){
				PrintError();
				exit(1);
			}
			else if(!(isdigit(argv[2][0]) &&
				isdigit(argv[2][1]) &&
				(argv[2][2] == ':') &&
				isdigit(argv[2][3]) &&
				isdigit(argv[2][4]) &&
				(argv[2][5] == ':') &&
				isdigit(argv[2][6]) &&
				isdigit(argv[2][7])
				)){
					PrintError();
					exit(0);
				}
			break;
		default: {break;}
		}
	}
}

void InitializeVariables(int argc, char* argv[]){

	ValidateArgs(argc, argv);

	switch(argc){
		case 2:{
				ExtractTimeAsNumbers(&main_time.seconds, &main_time.minutes, &main_time.hours, argv[1]);	
				break;
			}
		case 3:{
				ExtractTimeAsNumbers(&stopping_time.seconds, &stopping_time.minutes, &stopping_time.hours, argv[2]);
				mode = *(argv[1]);
			}
		default:{break;}
	}	
}



void UpdateTime(int *sec, int* min, int* h){
	if(main_time.hours == stopping_time.hours && main_time.minutes == stopping_time.minutes && main_time.seconds == stopping_time.seconds){
		while(1){
			fflush(stdout);
			printf("\a");
			sleep(1);
		}
	}
	else if((*sec) >= 60){
		(*sec) = 0;
		(*min)++;
		if((*min) >= 60){
			(*min) = 0;
			(*h)++;
			if((*h)>=99){
				(*sec) = 0;
				(*min) = 0;
				(*h) = 0;
			}
		}
	}
	else{
		(*sec)++;
	}
}

void Handler(int signal_number){
	fflush(stdout);
	printf("\r%02d:%02d:%02d", main_time.seconds, main_time.minutes, main_time.hours);
	UpdateTime(&main_time.seconds, &main_time.minutes, &main_time.hours);
	alarm(1);
}

void main(int argc, char* argv[])
{
	InitializeVariables(argc, argv);
	signal(SIGALRM, Handler);
	alarm(1);
	while(1);
}
