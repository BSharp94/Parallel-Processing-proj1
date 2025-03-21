#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <mpi.h>
#define ARRAY_SIZE 100
#define MATRIX_SIZE 1000

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
			matrix[i][j] = (rand() %10) + 1;	//random number between 1-100
		}
	}

}

/* End Generator funcions */

/* Begin Time Funcions */

//Constant function runs O(1) time. simply returns the first item in an array
void constant_master(int rank, int size, int arraySize, int array[arraySize], struct timeval *tvBegin, struct timeval *tvEnd,int *result){
	if(rank == 0){
		//start the time		
		gettimeofday(tvBegin, NULL);

		//send array
		MPI_Send(array, arraySize,MPI_INT,1,0,MPI_COMM_WORLD);

		//receive constatnt
		MPI_Recv(result,1,MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

		//stop the time
		gettimeofday(tvEnd, NULL);

	}else{
		printf("WARNING - constant_master being called by non master process");
	}	
}

void constant_slave(int rank, int size){
	if(rank == 1){
		//declare variable
		int constantResult;

		//Probe for arraySize
		int arraySize;
		MPI_Status status;
		MPI_Probe(0,0,MPI_COMM_WORLD,&status);
		MPI_Get_count(&status, MPI_INT, &arraySize);		

		//mallac array
		int * array = (int*)malloc(sizeof(int) * arraySize);
		
		//receive array
		MPI_Recv(array,arraySize,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

		//calculate constant
		constantResult = array[0];

		//send constant	
		MPI_Send(&constantResult,1,MPI_INT,0,0,MPI_COMM_WORLD);
	}
}


//Linear function runs O(n) time. Returns a linear search of the items in the array 
void linear_master(int rank, int size, int arraySize, int array[arraySize],int searchValue, struct timeval *tvBegin, struct timeval *tvEnd, int *result){
	if(rank == 0){
		int allocation = arraySize / (size-1);
		int sendArray[allocation];

		//start the time		
		gettimeofday(tvBegin, NULL);
		
		int i;
		for(i = 1; i<size;i++){
			
			//make sub array
			memcpy(&sendArray, &array[(i-1)*allocation ],allocation*sizeof(int));
			MPI_Send(sendArray,allocation,MPI_INT,i,0,MPI_COMM_WORLD);
			MPI_Send(&searchValue, 1, MPI_INT, i,0,MPI_COMM_WORLD);
		}

		//receive constatnt
		MPI_Recv(result,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

		//stop the time
		gettimeofday(tvEnd, NULL);

		
	}
}
void linear_slave(int rank, int size){
	if(rank != 0){
		//Probe for arraySize
		int arraySize;
		MPI_Status status;
		MPI_Probe(0,0,MPI_COMM_WORLD,&status);
		MPI_Get_count(&status, MPI_INT, &arraySize);		

		//mallac array
		int * array = (int*)malloc(sizeof(int) * arraySize);
		int searchValue;

		//receive array
		MPI_Recv(array,arraySize,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Recv(&searchValue, 1, MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		
		//run linear search
		int linearResults = -1;
		int i;
		for(i = 0; i < arraySize;i++){
			if(array[i] == searchValue){
				linearResults = i + (rank-1) * arraySize;
				MPI_Send(&linearResults, 1, MPI_INT,0,0,MPI_COMM_WORLD);
			}
		}		
	}
}
void bubble_master(int rank, int size, int arraySize, int array[arraySize],struct timeval *tvBegin, struct timeval *tvEnd){
	if(rank == 0){

		//start time
		gettimeofday(tvBegin, NULL);
			
		int allocation = (arraySize-1)  / (size - 1);
		int totalswaps = 0;
		int sorted = 0, phase = 0;
		while(sorted == 0){
			int i,sendArray[allocation];	
			for(i =1; i < size;i++){
				MPI_Send(&array[allocation*(i-1)+phase],allocation,MPI_INT,i,0,MPI_COMM_WORLD);		
			}
	
			//receive sections
			int numOfSwaps =  0;
			int recieveArray[allocation + 1];
			for(i=1; i < size; i++){
				MPI_Status status;
				MPI_Recv(recieveArray, allocation + 1, MPI_INT,i,0,MPI_COMM_WORLD,&status);	
				totalswaps += recieveArray[allocation];
				int j;
				for(j = 0; j < allocation;j++){
					array[(i-1)*allocation+phase + j] = recieveArray[j];
				}
			}
			//check for sorted
			if(phase == 1){
				if(totalswaps==0)break;
				totalswaps = 0;
			}
			//change phase		
			if(phase == 0)
				phase = 1;	
			else
				phase = 0;
		}				

		//stop time
		gettimeofday(tvEnd, NULL);
		//stop slaves
		int i,stopArray[allocation];
		
		for( i = 0; i<allocation;i++){
			stopArray[i]= -1;
		}
		for(i =1; i < size;i++){
			MPI_Send(stopArray,allocation,MPI_INT,i,0,MPI_COMM_WORLD);
		}
	}	
}
void bubble_slave(int rank, int size){
	if(rank > 0){
		while(1==1){
			
			//Probe for arraySize
			int arraySize;
			MPI_Status status;
			MPI_Probe(0,0,MPI_COMM_WORLD,&status);
			MPI_Get_count(&status, MPI_INT, &arraySize);		
			
			//mallac array
			int array[arraySize + 1];
	
			//receive array
			MPI_Recv(array,arraySize,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			
			if(array[0] == -1) {
				break;
			}		
			//run swap
			int i,numSwaps = 0;
			for(i = 0; i< arraySize; i=i+2){
				if(array[i] > array[i+1]){
					int swap = array[i];
					array[i] = array[i + 1];
					array[i + 1] = swap;
					numSwaps++;
				}	
			}
			array[arraySize] = numSwaps;
			//send array
			MPI_Send(array,arraySize + 1,MPI_INT,0,0,MPI_COMM_WORLD);
		}
	}
}
void matrix_master(int rank, int size, int dataSize,struct timeval *tvBegin, struct timeval *tvEnd,int result[dataSize][dataSize]){

	int matrixA[dataSize][dataSize];
	int matrixB[dataSize][dataSize];

	getRandMatrix(dataSize,matrixA,0);
	getRandMatrix(dataSize,matrixB,1000);

	//start time
	gettimeofday(tvBegin, NULL);
		
	//send out matrixB
	int i,j;
	int allocation = dataSize / (size-1) ;
	for(i = 1;i < size; i++){
		for(j =0; j<dataSize;j++){
			MPI_Send(&matrixB[j],dataSize,MPI_INT,i,0,MPI_COMM_WORLD);
		}
		for(j = 0; j < allocation; j++){
			MPI_Send(&matrixA[(i-1)*allocation + j],dataSize,MPI_INT,i,0,MPI_COMM_WORLD);
		}		
	}
	for(i = 1; i < size; i++){
		for(j = 0;j < allocation; j++){
			MPI_Recv(&result[(i-1)*allocation + j],dataSize, MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		} 
	}

	//end time
	gettimeofday(tvEnd, NULL);
		
}

void matrix_slave(int rank, int size,int dataSize){
	int allocation = dataSize / (size-1);
	int matrixB[dataSize][dataSize];
	int matrixA[allocation][dataSize];
	int result[allocation][dataSize];
	int i,j,k;
	for(i = 0; i < dataSize; i++){
		MPI_Recv(&matrixB[i],dataSize,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		
	}
	//recieve rows of matrix A
	for(i = 0; i < allocation; i++){
		MPI_Recv(&matrixA[i],dataSize,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}

	
	//calculate matrix 
	for(i = 0; i < allocation;i++){
		for(j = 0; j < dataSize; j++){
			int total = 0;
			for(k = 0; k < dataSize; k++)	
				total += matrixA[i][k] * matrixB[k][j];
			result[i][j] = total;
		}
	}
	
	for(i = 0; i < allocation; i++){
		MPI_Send(&result[i],dataSize,MPI_INT,0,0,MPI_COMM_WORLD);
	}
}
/*End Time Functions */


int main(int argc,char *argv){

	//init mpi environment
	MPI_Init(NULL,NULL);

	//get world rank and size
	int world_rank, world_size;
	MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&world_size);

	int dataSize = ARRAY_SIZE;
	dataSize = dataSize  - (dataSize % (world_size-1));
	
	//initialize time values
	struct timeval tvBegin, tvEnd, tvDiff;
	
	/*	Begin Constant Search	*/
	
	if(world_rank == 0){	//mater process

		//Generate array
		int array[dataSize];
		getRandArray(array,dataSize);		
		
		//run constant search
		int result;
		constant_master(world_rank,world_size,dataSize,array,&tvBegin,&tvEnd,&result);

		//record data
		time_diff(&tvDiff, &tvEnd, &tvBegin);
		printf("Constant Search: actual value a[0] = %d, calculated value a[0] = %d\n",array[0],result);
		printf("Constant Time:\t");
		printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
	}//end master process
	else if(world_rank == 1){
		//run constant_slave
		constant_slave(world_rank,world_size);

	}//end slave 1
	
	/*	End Constant Search	*/

	
	/*	Begin linear Search	*/
	if(world_rank == 0){

		//Generate array
		srand(time(NULL));
		int i,array[dataSize];
		for(i = 0; i< dataSize;i++){
			array[i] = (rand() % 100) + 1;
		}

		//gerneate search value
		int randIndex = (rand() %dataSize);
		int searchValue = array[randIndex];

		//run linear search
		int result;
		linear_master(world_rank,world_size,dataSize,array,searchValue,&tvBegin,&tvEnd,&result);

		//record data
		time_diff(&tvDiff, &tvEnd, &tvBegin);
		printf("Linear results a[%d] = %d, actual results a[%d] = %d\n",randIndex,searchValue,result,array[result]);
		printf("Linear Time:\t");
		printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
	

	}else{
		linear_slave(world_rank,world_size);
	}

	
	/*	End linear Search 	*/

	/*	Begin Bubble Search	*/
	dataSize = ARRAY_SIZE;
	dataSize = dataSize  - (dataSize % (world_size-1));
	
	if(world_rank == 0){
		int allocation = dataSize / (world_size -1);
		if(allocation % 2 >= 1){
			dataSize -= (world_size-1);
			allocation--;
		}
		dataSize += 1;
		//Generate array
		srand(time(NULL));
		int i,array[dataSize];
		for(i = 0; i< dataSize;i++){
			array[i] = (rand() % 100) + 1;
		}
		printf("before sorted [ %d, %d, %d , %d ...\n",array[0],array[1],array[2],array[3]);
		
		//run bubble sort
		bubble_master(world_rank, world_size, dataSize, array, &tvBegin, &tvEnd);
			
		//record data
		time_diff(&tvDiff, &tvEnd, &tvBegin);
		printf("Bubble sorted [ %d, %d, %d , %d ...\n",array[0],array[1],array[2],array[3]);
		printf("Bubble Sort Time:\t");
		printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
	
	}else{
		bubble_slave(world_rank,world_size);
	}
	
	/* 	End Bubble Search 	*/

	/*	Begin Matrix Mult	*/	
	dataSize = MATRIX_SIZE;
	dataSize = dataSize  - (dataSize % (world_size-1));
	
	if(world_rank == 0){
			
		int matrixResult[dataSize][dataSize];

		matrix_master(world_rank,world_size, dataSize, &tvBegin, &tvEnd,matrixResult);
			
		//record data
		time_diff(&tvDiff, &tvEnd, &tvBegin);
		printf("matrix mult  [ %d, %d, %d , %d ...\n",matrixResult[0][0],matrixResult[0][1],matrixResult[0][2],matrixResult[0][3]);
		printf("matrix mult Time:\t");
		printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
	

	}else{
		matrix_slave(world_rank,world_size,dataSize);
	}
		
	
	/*	End Matrix Mult		*/
	MPI_Finalize();	
	return 0; 	 
}

