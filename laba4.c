#include <stdio.h>
#include <stdlib.h>

int N;
int M;
int buffer[100]; 

void openSample()
{
	FILE *fptr;
	fptr = fopen("sample.dat","r");
	if(fptr == NULL)
	{
		printf("Error!");
		exit(1);
	}
    
	fscanf(fptr,"%d", &N);
	fscanf(fptr,"%d", &M);
	printf("Number of pages: %d \n", N);
	printf("Number of frames: %d \n", M);
	//fscanf(fptr,"%d", &M); //newline char
	int temp = 0;
	int counter = 0;
	while(temp != -1)
	{
		fscanf(fptr,"%d", &temp); //newline char
		//printf("Temp: %d", temp);
		buffer[counter] = temp;
		counter++;
	}
	printf("Size of refernce string: %d\n", counter-1);
	printf("Refernce String: \n");
	for(int i = 0; i < counter-1; i++)
	{
		printf("%d ", buffer[i]);
	}
	printf("\n\n");
}

void fifo()
{
	printf("\nFifo");
	
}

void optimal()
{
	printf("\nOptimal");
	
}


void leastUsed()
{
	printf("\nLeast");
	
}


int main()
{
	openSample();
	fifo();
	return 0;
}
