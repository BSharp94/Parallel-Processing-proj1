#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>


/* Time functions */

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
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

/* Begin Generator functions */

void getRandArray(int array[],int dataSize){
	
	srand(time(NULL));	//seed rand with time
	
	int i;
	for(i = 0; i< dataSize;i++){
		array[i] = (rand() % 100) + 1;	//random number between 1-100
	}
	
}

void getRandMatrix(int dataSize, int matrix[dataSize][dataSize]){

	srand(time(NULL));	//seed rand with time
	
	int i,j;
	for(i = 0; i < dataSize;i++){
		for(j = 0;j < dataSize;j++){
			matrix[i][j] = (rand() %100) + 1;	//random number between 1-100
		}
	}

}

/* End Generator funcions */

/* Begin Time Funcions */

//Constant function runs O(1) time. simply returns the first item in an array

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

//Linear function runs O(n) time. Returns a linear search of the items in the array 
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


//Bubble sort runs at O(n^2) time. 
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

//Matrix Multiplication simulates O(n^3) time. 
/*void matrix_mult(int dataSize,struct timeval *tvBegin, struct timeval *tvEnd){

	//get matricies

	//start time
	gettimeofday(tvBegin, NULL);
	timeval_print(tvBegin);

	//compute matrix multiplication
	

	//stop time
	gettimeofday(tvEnd, NULL);
	timeval_print(tvEnd);

}
*/
/*End Time Functions */


int main(){

	//get dataSize
	int dataSize = 10;
	printf("Please enter the data size: n = ");
	scanf("%d", &dataSize);
	
	//initialize time values
	struct timeval tvBegin, tvEnd, tvDiff;


	/*	Begin Constant Search	*/
	{
		//Generate array
		int array [dataSize];
		getRandArray(array,dataSize);		

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
		/*
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
		*/
	}
	/*	End Matrix Mult		*/
	
	return 0; 	 
}

