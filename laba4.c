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
	int label;
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
	printf("Size of reference string: %d\n", counter-1);
	printf("Reference String: \n");
	for(int i = 0; i < counter-1; i++)
	{
		printf("%2.1d", buffer[i]);
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
	while(curr != NULL)
	{
		prev = curr;
		curr = curr->next;
		free(prev);
	}
}

//returns 1 if in list otherwise 0
int inList(int ref)
{
	Page* curr = pageList;
	while (curr != NULL) {
		if(curr->refernce == ref) return 1;
		curr = curr->next;
	}

	return 0;
}

void writePagesToArray(int (*pageStates)[M+1], int index) {
	Page* curr = pageList;
	for (int i = 0; i < M; i++) {
		pageStates[index][i] = curr->refernce;
		curr = curr->next;
	}
}

void printPageStates(int (*pageStates)[M+1]) {
	for (int i = 0; i < refStringSize; i++) printf("%3.1d", buffer[i]);
	printf("\n");
	for (int i = 0; i < refStringSize; i++) printf("---");
	printf("\n");

	for (int j = 0; j < M; j++) {
		for (int i = 0; i < refStringSize; i++) {
			printf("%3.1d", pageStates[i][j]);
		}
		printf("\n");
	}

	for (int i = 0; i < refStringSize; i++) {
		if (pageStates[i][3]) printf("  p");
		else printf("   ");
	}
	printf("\n");

	for (int i = 0; i < refStringSize; i++) printf("---");
	printf("\n");
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
	int totalFaults = 0;
	int pageStates[refStringSize][M+1];
	for(int i = 0; i < refStringSize; i++)
	{
		if(inList(buffer[i]))
		{
			//no fault and do not add to list
			pageStates[i][3] = 0;
		} else {
			//fault and add to list
			addPageFiFo(buffer[i]);
			totalFaults++;
			pageStates[i][3] = 1;
		}
		writePagesToArray(pageStates, i);
	}
	printPageStates(pageStates);
	printf("%d page-faults\n", totalFaults);
}

void calculatePageLabels(int currentReferenceIndex) {
	Page* curr = pageList;
	while (curr != NULL) {
		curr->label = -1;
		for (int i = currentReferenceIndex; i < refStringSize; i++) {
			if (buffer[i] == curr->refernce) {
				curr->label = i - currentReferenceIndex;
				break;
			}
		}
		curr = curr->next;
	}
}

void swapOutHighestLabelPage(int pageReference) {
	Page* curr = pageList;
	Page* highestLabelPage = NULL;
	while (curr != NULL) {
		if (curr->label == -1) {
			highestLabelPage = curr;
			break;
		} else if (highestLabelPage == NULL || curr->label > highestLabelPage->label) {
			highestLabelPage = curr;
		}
		curr = curr->next;
	}
	highestLabelPage->refernce = pageReference;
}


void optimal()
{
	printf("\nOptimal\n");

	int totalFaults = 0;
	int pageStates[refStringSize][M+1];
	for (int i = 0; i < refStringSize; i++) {
		if (!inList(buffer[i])) {
			totalFaults++;
			calculatePageLabels(i);
			swapOutHighestLabelPage(buffer[i]);
			pageStates[i][3] = 1;
		} else {
			pageStates[i][3] = 0;
		}
		writePagesToArray(pageStates, i);
	}

	printPageStates(pageStates);
	printf("%d page-faults\n", totalFaults);

}

void leastUsed()
{
	printf("\nLeast Used\n");
	int totalFaults = 0;
	int pageStates[refStringSize][M+1];

	for(int i = 0; i < refStringSize; i++)
	{

		if(inList(buffer[i]))
		{
			//no fault just update the timer in the stored list
			updateTime(buffer[i]);
			pageStates[i][3] = 0;
		} else {
			//fault and add to list
			addNewPageLRU(buffer[i]);
			totalFaults++;
			pageStates[i][3] = 1;
		}
		writePagesToArray(pageStates, i);
		lruCounter++;
	}
	printPageStates(pageStates);
	printf("%d page-faults\n", totalFaults);
}


int main()
{
	openSample();

	initList();
	fifo();
	freeList();

	initList();
	optimal();
	freeList();

	initList();
	leastUsed();
	freeList();

	return 0;
}
