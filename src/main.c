#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <pigpio.h>


#define WARNING_TEMP1 68
#define WARNING_TEMP2 76
#define PIN 17

void tempCheck(int n[],int size){
	for(int i = 0; i < size; i++){
		if(n[i] < 0){
			printf("\nNot a valid value");
		}else if( n[i] < WARNING_TEMP1 || n[i] > WARNING_TEMP2){
			printf("\nWARNING");
		}else{
			printf("\nNORMAL");
		}
	}
}

int avgTemp(int n[], int size){
	int sum = 0;
	for(int i = 0; i < size; i++){
		sum += n[i];
	}
	return sum / size;
}


void saveTemps(int n[], int size, int avg){
	FILE *file = fopen("../status.txt", "a");
	if(file == NULL){
		printf("\nCould not open file\n");
		return;
	}
	for(int i = 0; i < size; i++){
		if(n[i] < 0){
			fprintf(file, "Not a valid value\n");
		}else if( n[i] < WARNING_TEMP1 || n[i] > WARNING_TEMP2){
			fprintf(file, "%d WARNING\n", n[i]);
		}else{
			fprintf(file, "%d NORMAL\n", n[i]);
		}
	}
	fprintf(file, "Avg: %d\n", avg);
	
	fclose(file);
}

void singleTemp(int n){
	if( n < WARNING_TEMP1 || n > WARNING_TEMP2){
		gpioWrite(PIN,1);
		printf("\nWARNING");
	}else{
		gpioWrite(PIN,0);
		printf("\nNORMAL");
	}
}
	

int main(){
	gpioInitialise();
	gpioSetMode(PIN,PI_OUTPUT);
	int tp[5];
	printf("Enter the temperatures of the room: ");
	for(int i = 0; i < 5; i++){
		scanf("%d", &tp[i]);
	}
	tempCheck(tp, 5);
	int avg = avgTemp(tp, 5);
	printf("\nAverage Temperature is: %d", avg);
	saveTemps(tp, 5, avg);
	int single;
	printf("\nEnter a single temperature: ");
	scanf("%d", &single);
	singleTemp(single);
	gpioTerminate();
	return 0;
}
	
