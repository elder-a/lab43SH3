#include <stdio.h>
#include <stdlib.h>

int N; //Number of virtual pages 
int M; //Number of actual pages (page frame)
int buffer[100]; 
int refStringSize = 0; //give you the size of the refernce string 
long int lruCounter = 0;



typedef struct Page 
{
	int refernce;
	int time;
	struct Page* next;
	struct Page* prev;
} Page;

Page* pageList = NULL;


//opens the sample data files and saves the data into the global varibles (N, M and buffer)
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

//init for the starting list, starts will all refernces -1
void initList ()
{
	Page* pageListRef = NULL;
	pageList = malloc(sizeof(Page));
	pageList->refernce = -1;
	pageList->time = 0;
	pageList->prev = NULL;
	pageList->next = pageListRef;
	
	Page* pageListPrev = pageList;
	
	for(int i = 1; i < M; i++)
	{
		pageListRef = malloc(sizeof(Page));
		pageListRef->refernce = -1;
		pageListRef->time = 0; 
		pageListRef->prev = pageListPrev;
		pageListRef->next = NULL;
		
		pageListPrev->next = pageListRef;
		pageListPrev = pageListRef;
	}
}

//frees list of memory 
void freeList()
{
	Page* curr = pageList;
	Page* prev;
	while(curr->next != NULL)
	{
		prev = curr;
		curr = curr->next;
		free(prev);
	}
}

//prints out the list 
void ptrList()
{
	Page* curr = pageList;
	//printf("Page Refernce: %d \n", pageList->refernce);
	while(curr->next != NULL)
	{
		printf("Refernce: %d   ", curr->refernce);
		//printf("Time: %d |", curr->time);//delete after
		curr = curr->next;
	}
	printf("Refernce: %d   ", curr->refernce);
	//printf("Time: %d |", curr->time);//delete after
	printf("\n");
}

//returns 1 if in list otherwise 0
int inList(int ref)
{
	Page* curr = pageList;
	if(curr->refernce == ref){
		return 1;
	}
	while(curr->next != NULL)
	{
		curr = curr->next;
		if(curr->refernce == ref){
			return 1;
		}
	}
	
	return 0;
}



//adds the paramter to the last value in the list, pops off the top values and replaces it with the second value in the list 
void addPageFiFo(int ref)
{
	//pop off the top page and replace it
	Page* curr = pageList;
	if(curr->next == NULL){
		printf("List to small");
		return; 
	}
	curr = curr->next;
	curr->prev = NULL;
	Page* toFree = pageList;
	pageList = curr;
	free(toFree); //frees up the old link 
	//replace the last page with the new one 
	while(curr->next != NULL) //goto the last node
	{
		curr = curr->next;
	}
	
	//now at the last space in the list 
	Page* pageListRef = NULL;
	pageListRef = malloc(sizeof(Page));
	pageListRef->refernce = ref;
	//printf("Ref num: %d", pageListRef->refernce);
	pageListRef->prev = curr;
	pageListRef->next = NULL;
	curr->next = pageListRef;
	return;
}

//returns the size of the list 
int sizeList()
{
	int currSize = 0;
	Page* curr = pageList;
	if(curr->refernce == -1){
		return 0; 
	}
	
	while(curr->next != NULL && curr->refernce != -1)
	{
		curr = curr->next;
		currSize++;
	}
	if(curr->refernce != -1){
		currSize++;
	}
	return currSize;
}

void addToBack(int ref)
{
	Page* curr = pageList;
	while(curr->next != NULL && curr->refernce != -1) //goto the last node
	{
		curr = curr->next;
	}
	curr->refernce = ref;
	curr->time = lruCounter;
}

void replaceLRU(int ref)
{
	Page* curr = pageList;
	int oldestTime = 1000;
	Page* toChange;
	while(curr->next != NULL ) //goto the last node
	{
		if(oldestTime > curr->time){
			oldestTime = curr->time;
			toChange = curr; 
		}
		curr = curr->next;
	}
	//have to check the last value
	if(oldestTime > curr->time){
		oldestTime = curr->time;
		toChange = curr; 
	}
	toChange->refernce = ref;
	toChange->time = lruCounter; 
}

void updateTime(int ref)
{
	Page* curr = pageList;
	if(curr->refernce == ref){
		curr->time = lruCounter;
		return;
	}
	while(curr->next != NULL ) //goto the last node
	{
		curr = curr->next;
		if(curr->refernce == ref){
			curr->time = lruCounter;
			return;
		}
	}
}

void addNewPageLRU(int ref)
{
	if(sizeList() < M)
	{
		//just stick the value at end of list
		addToBack(ref);
	} else {
		//replace an old value 
		replaceLRU(ref);
		
	}
}

void fifo()
{
	printf("\nFIFO\n");
	printf("------------------------------------------------------------\n");
	int totalFaults = 0;
	for(int i = 0; i < refStringSize; i++)
	{
		if(inList(buffer[i]))
		{
			//no fault and do not add to list 
		} else {
			//fault and add to list 
			addPageFiFo(buffer[i]);
			totalFaults++;
		}
		ptrList();
	}
	printf("------------------------------------------------------------\n");
	printf("%d page-faults\n", totalFaults);
}

void optimal()
{
	printf("\nOptimal");
	
}

void leastUsed()
{
	printf("\nLeast Used\n");
	printf("------------------------------------------------------------\n");
	int totalFaults = 0;
	
	for(int i = 0; i < refStringSize; i++)
	{
	
		if(inList(buffer[i]))
		{
			//no fault just update the timer in the stored list 
			updateTime(buffer[i]);
		} else {
			//fault and add to list 
			addNewPageLRU(buffer[i]);
			totalFaults++;
		}
		
		ptrList();
		lruCounter++;
		
	}
	printf("------------------------------------------------------------\n");
	printf("%d page-faults\n", totalFaults);	
}


int main()
{
	openSample();
	initList();
	ptrList();
	fifo();
	freeList();
	initList();
	leastUsed();
	
	return 0;
}
