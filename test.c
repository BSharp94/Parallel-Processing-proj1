#include <stdio.h>

int main(){

	int np = 8;
	int dataSize = 18;
	int dataAllocation[dataSize];
	int overAllocate = dataSize % 8;

	int i;
	for(i = 0;i < np; i++){
	
		if(i < overAllocate){
			dataAllocation[i] = dataSize / np +1;
		}else{
			dataAllocation[i] = dataSize / np;
		}
	}
	for (i=0; i<np; i++){
		printf("%d -", dataAllocation[i]);
	}

}
