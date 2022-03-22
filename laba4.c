#include <stdio.h>
#include <stdlib.h>

int N;
int M;
int buffer[100]; 

void openSample()
{
	//buffer = (int*)malloc(100*sizeof(int));
	FILE *fptr;
	fptr = fopen("sample.dat","r");
	printf("Hello world!\n");

	if(fptr == NULL)
	{
		printf("Error!");
		exit(1);
	}
    
	fscanf(fptr,"%d", &N);
	fscanf(fptr,"%d", &M);
	printf("Value of N=%d", N);
	printf("Value of M=%d", M);
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
	printf("\n Refernce String: \n");
	for(int i = 0; i < counter-1; i++)
	{
		printf("%d ", buffer[i]);
	}
}


int main()
{
	openSample();
	return 0;
}
