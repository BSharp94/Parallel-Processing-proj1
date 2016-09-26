#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>


/* Time functions */

int time_diff(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
	long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000*t1->tv_sec);
	result->tv_sec = diff / 1000000;
	result->tv_usec = diff % 1000000;

	return (diff<0);
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

void getRandMatrix(int dataSize, int matrix[dataSize][dataSize],int offset){

	srand(time(NULL) + offset);	//seed rand with time
	
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

	//find constant
	int value = array[1];
	
	//stop time
	gettimeofday(tvEnd, NULL);

}

//Linear function runs O(n) time. Returns a linear search of the items in the array 
void linear(int array[],int arraySize, int searchValue,struct timeval *tvBegin, struct timeval *tvEnd){
	

	//start time
	gettimeofday(tvBegin, NULL);

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

}


//Bubble sort runs at O(n^2) time. 
void bubble(int array[],int dataSize, struct timeval *tvBegin, struct timeval *tvEnd){
	

	//start time
	gettimeofday(tvBegin, NULL);

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

}

//Matrix Multiplication simulates O(n^3) time. 
void matrix_mult(int dataSize,int matrixA[dataSize][dataSize],int matrixB[dataSize][dataSize],struct timeval *tvBegin, struct timeval *tvEnd){

	

	//generate Result matrix
	int matrixResult[dataSize][dataSize];

	int i,j,k;
	for(i = 0; i<dataSize;i++){
		for(j = 0; j< dataSize; j++){
			matrixResult[i][j] = 0;
		}
	}

	//start time
	gettimeofday(tvBegin, NULL);

	//compute matrix multiplication
	
	for(i = 0; i < dataSize;i++){
		for(j = 0; j < dataSize;j++){
			//for each cell in the result matrix
			for(k = 0; k <dataSize; k++){
				matrixResult[i][j] += matrixA[i][k] * matrixB[k][j];			
			}
		}
	}
	//stop time
	gettimeofday(tvEnd, NULL);
}

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
		time_diff(&tvDiff, &tvEnd, &tvBegin);
		printf("Constant Time:\t");
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
		time_diff(&tvDiff, &tvEnd, &tvBegin);
		printf("Linear Time:\t");
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
		time_diff(&tvDiff, &tvEnd, &tvBegin);
		printf("Bubble Time:\t");
		printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
	
	}
	/* 	End Bubble Search 	*/

	/*	Begin Matrix Mult	*/	
	{
		
		//generate random array
		int matrixA[dataSize][dataSize];
		int matrixB[dataSize][dataSize];

		getRandMatrix(dataSize,matrixA,0);
		getRandMatrix(dataSize,matrixB,1000);

		//run  Matrix Mult
		matrix_mult(dataSize,matrixA,matrixB,&tvBegin,&tvEnd);

		
		//record data
		time_diff(&tvDiff, &tvEnd, &tvBegin);
		printf("Matrix Time:\t");
		printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
		
	}
	/*	End Matrix Mult		*/
	
	return 0; 	 
}

