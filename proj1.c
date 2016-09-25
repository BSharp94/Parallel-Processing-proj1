#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

int dataSize2 = 10;	//default size of array and matrix


/* Time functions */

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1){
	long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000*t1->tv_sec);
	result->tv_sec = diff / 1000000;
	result->tv_usec = diff % 1000000;

	return (diff<0);
}

void timeval_print(struct timeval *tv){

	char buffer[30];
	time_t curtime;
	
	printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
	curtime = tv->tv_sec;
	strftime(buffer, 30, "%m-%d-%Y %T", localtime(&curtime));
	printf(" = %s.%06ld\n", buffer, tv->tv_usec);

}
/* End Time Functions */

void constant(int array[],struct timeval *tvBegin,struct timeval *tvEnd){
	
	//start time
	gettimeofday(tvBegin, NULL);
	timeval_print(tvBegin);

	//find constant
	int value = array[1];
	
	//stop time
	gettimeofday(tvEnd, NULL);
	timeval_print(tvEnd);

}

void linear(int array[],int arraySize, int searchValue,struct timeval *tvBegin, struct timeval *tvEnd){
	

	//start time
	gettimeofday(tvBegin, NULL);
	timeval_print(tvBegin);

	//linear seach
	int success = 0;
	int i;
	for(i = 0; i < arraySize; i++){
		if(array[i] == searchValue){
			break;		
		}
	}
	
	//stop time
	gettimeofday(tvEnd, NULL);
	timeval_print(tvEnd);

}



void bubble(int array[],int dataSize, struct timeval *tvBegin, struct timeval *tvEnd){
	

	//start time
	gettimeofday(tvBegin, NULL);
	timeval_print(tvBegin);

	//perform bubble sort
	int c,d,swap;
	for(c = 0; c < (dataSize - 1); c++){
		for (d = 0; d < (dataSize - c - 1); d++){
			if(array[d] > array[d+1]){
				swap = array[d];
				array[d] = array[d+1];
				array[d+1] = swap;
			}		
		}
	}

	
	//stop time
	gettimeofday(tvEnd, NULL);
	timeval_print(tvEnd);

}

void matrix_mult(int (*matrixA)[dataSize], int (*matrixB)[dataSize],struct timeval *tvBegin, struct timeval *tvEnd){

	//start time
	gettimeofday(tvBegin, NULL);
	timeval_print(tvBegin);

	//compute matrix multiplication
	

	//stop time
	gettimeofday(tvEnd, NULL);
	timeval_print(tvEnd);

}


int main(){

	//get dataSize
	int dataSize = 10;
	printf("Please enter the data size: n = ");
	scanf("%d", &dataSize);
	dataSize2 = dataSize;
	//initialize time values
	struct timeval tvBegin, tvEnd, tvDiff;


	/*	Begin Constant Search	*/
	{
		//Generate array
		srand(time(NULL));
		int i,array[dataSize];
		for(i = 0; i< dataSize;i++){
			array[i] = (rand() % 100) + 1;
		}

		//run constant search
		constant(array,&tvBegin,&tvEnd);

		//record data
		timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
		printf("Constant Time: ");
		printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
	}
	/*	End Constant Search	*/

	
	/*	Begin linear Search	*/
	{
		//Generate array
		srand(time(NULL));
		int i,array[dataSize];
		for(i = 0; i< dataSize;i++){
			array[i] = (rand() % 100) + 1;
		}

		//gerneate search value
		int searchValue = array[rand() % dataSize];

		//run linear search
		linear(array,dataSize,searchValue,&tvBegin,&tvEnd);

		//record data
		timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
		printf("Linear Time: ");
		printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
	
	}
	/*	End linear Search 	*/

	/*	Begin Bubble Search	*/
	{
		//generate random array
		srand(time(NULL));
		int i,array[dataSize];
		for(i = 0; i< dataSize;i++){
			array[i] = (rand() % 100) + 1;
		}

		//run bubble sort
		bubble(array,dataSize,&tvBegin,&tvEnd);

		//record data
		timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
		printf("Bubble Time: ");
		printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
	
	}
	/* 	End Bubble Search 	*/

	/*	Begin Matrix Mult	*/	
	{
		//generate random array
		srand(time(NULL));
		int i,j;
		int **matrixA, **matrixB;

		//allocate for the matrix	
		for(i = 0; i< dataSize;i++){
			for(j = 0; j < dataSize; j++){
				matrixA[i][j] = (rand() % 100) + 1;
				matrixB[i][j] = (rand() % 100) + 1;
			}
		}

		//run  Matrix Mult
		matrix_mult(matrixA[dataSize2],matrixB[dataSize2],&tvBegin,&tvEnd);

		//record data
		//timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
		//printf("Matrix Time");
		//printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
	}
	/*	End Matrix Mult		*/
	
	return 0; 	 
}

