#include <stdio.h>
#include <stdlib.h>

int N; //Number of virtual pages 
int M; //Number of actual pages (page frame)
int buffer[100]; 
int refStringSize = 0;

typedef struct Page 
{
	int refernce;
	struct Page* next;
	struct Page* prev;
} Page;

Page* pageList = NULL;

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
	refStringSize = counter-1;
	printf("Size of refernce string: %d\n", counter-1);
	printf("Refernce String: \n");
	for(int i = 0; i < counter-1; i++)
	{
		printf("%d ", buffer[i]);
	}
	printf("\n\n");
}

void initList ()
{
	Page* pageListRef = NULL;
	pageList = malloc(sizeof(Page));
	pageList->refernce = -1;
	pageList->prev = NULL;
	pageList->next = pageListRef;
	
	Page* pageListPrev = pageList;
	
	for(int i = 0; i < M; i++)
	{
		pageListRef = malloc(sizeof(Page));
		pageListRef->refernce = -1;
		pageListRef->prev = pageListPrev;
		pageListRef->next = NULL;
		
		pageListPrev->next = pageListRef;
		pageListPrev = pageListRef;
	}
}

void ptrList()
{
	Page* curr = pageList;
	while(curr->next != NULL)
	{
		printf("Refernce: %d \n", curr->refernce);
		curr = curr->next;
	}
}

void fifo()
{
	//printf("\nFifo");
	for(int i = 0; i < refStringSize; i++)
	{
	
	
	
	
	
	}
	
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
	initList();
	ptrList();
	
	fifo();
	return 0;
}
