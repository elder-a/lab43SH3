#include <stdio.h>
#include <stdlib.h>

int N; //Number of virtual pages
int M; //Number of actual pages (page frame)
int referenceString[100];
int refStringSize; //give you the size of the reference string
long int lruCounter = 0;



typedef struct PageFrame
{
	int pageNumber;
	int time;
	int label;
	struct PageFrame* next;
	struct PageFrame* prev;
} PageFrame;

PageFrame* pageFrameList = NULL;


//opens the sample data files and saves the data into the global varibles (N, M and referenceString)
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
	int temp = 0;
	int counter = 0;
	while(temp != -1)
	{
		fscanf(fptr,"%d", &temp); //newline char
		referenceString[counter] = temp;
		counter++;
	}
	fclose(fptr);
	refStringSize = counter-1;
	printf("Size of reference string: %d\n", counter-1);
	printf("Reference String: \n");
	for(int i = 0; i < counter-1; i++)
	{
		printf("%2.1d", referenceString[i]);
	}
	printf("\n\n");
}

//init for the starting list, starts will all refernces -1
void initList ()
{
	PageFrame* pageListRef = NULL;
	pageFrameList = malloc(sizeof(PageFrame));
	pageFrameList->pageNumber = -1;
	pageFrameList->time = 0;
	pageFrameList->prev = NULL;
	pageFrameList->next = pageListRef;

	PageFrame* pageListPrev = pageFrameList;

	for(int i = 1; i < M; i++)
	{
		pageListRef = malloc(sizeof(PageFrame));
		pageListRef->pageNumber = -1;
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
	PageFrame* curr = pageFrameList;
	PageFrame* prev;
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
	PageFrame* curr = pageFrameList;
	while (curr != NULL) {
		if(curr->pageNumber == ref) return 1;
		curr = curr->next;
	}

	return 0;
}

void writePagesToArray(int (*pageStates)[M+1], int index) {
	PageFrame* curr = pageFrameList;
	for (int i = 0; i < M; i++) {
		pageStates[index][i] = curr->pageNumber;
		curr = curr->next;
	}
}

void printPageStates(int (*pageStates)[M+1]) {
	for (int i = 0; i < refStringSize; i++) printf("%3.1d", referenceString[i]);
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
		if (pageStates[i][M]) printf("  p");
		else printf("   ");
	}
	printf("\n");

	for (int i = 0; i < refStringSize; i++) printf("---");
	printf("\n");
}

//adds the paramter to the last value in the list, pops off the top values and replaces it with the second value in the list
void addPageFiFo(int ref)
{
	//puch a new page into the list
	PageFrame* pageListRef = NULL;
	pageListRef = malloc(sizeof(PageFrame));
	pageListRef->pageNumber = ref;
	pageListRef->prev = NULL;
	pageListRef->next = pageFrameList;

	PageFrame* curr = pageFrameList;
	pageFrameList = pageListRef;

	PageFrame* pagePrev = NULL;
	//replace the last page with the new one
	while(curr->next != NULL) //goto the last node
	{
		pagePrev = curr;
		curr = curr->next;
	}
	pagePrev->next = NULL;
	free(curr);

	return;
}

//returns the size of the list
int sizeList()
{
	int currSize = 0;
	PageFrame* curr = pageFrameList;

	while(curr != NULL)
	{
		if (curr->pageNumber != -1) currSize++;
		curr = curr->next;
	}

	return currSize;
}

void addToBack(int ref)
{
	PageFrame* curr = pageFrameList;
	while(curr->next != NULL && curr->pageNumber != -1) //goto the last node
	{
		curr = curr->next;
	}
	curr->pageNumber = ref;
	curr->time = lruCounter;
}

void replaceLRU(int ref)
{
	PageFrame* curr = pageFrameList;
	int oldestTime = 1000;
	PageFrame* toChange;
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
	toChange->pageNumber = ref;
	toChange->time = lruCounter;
}

void updateTime(int ref)
{
	PageFrame* curr = pageFrameList;
	if(curr->pageNumber == ref){
		curr->time = lruCounter;
		return;
	}
	while(curr->next != NULL ) //goto the last node
	{
		curr = curr->next;
		if(curr->pageNumber == ref){
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
		if(inList(referenceString[i]))
		{
			//no fault and do not add to list
			pageStates[i][M] = 0;
		} else {
			//fault and add to list
			addPageFiFo(referenceString[i]);
			totalFaults++;
			pageStates[i][M] = 1;
		}
		writePagesToArray(pageStates, i);
	}
	printPageStates(pageStates);
	printf("%d page-faults\n", totalFaults);
}

void calculatePageLabels(int currentReferenceIndex) {
	PageFrame* curr = pageFrameList;
	while (curr != NULL) {
		curr->label = -1;
		for (int i = currentReferenceIndex; i < refStringSize; i++) {
			if (referenceString[i] == curr->pageNumber) {
				curr->label = i - currentReferenceIndex;
				break;
			}
		}
		curr = curr->next;
	}
}

void swapOutHighestLabelPage(int pageNumber) {
	PageFrame* curr = pageFrameList;
	PageFrame* highestLabelPage = NULL;
	// Look for free page first
	while (curr != NULL) {
		if (curr->pageNumber == -1) {
			curr->pageNumber = pageNumber;
			return;
		}
		curr = curr->next;
	}

	curr = pageFrameList;
	while (curr != NULL) {
		if (curr->label == -1) {
			highestLabelPage = curr;
			break;
		} else if (highestLabelPage == NULL || curr->label > highestLabelPage->label) {
			highestLabelPage = curr;
		}
		curr = curr->next;
	}
	highestLabelPage->pageNumber = pageNumber;
}


void optimal()
{
	printf("\nOptimal\n");

	int totalFaults = 0;
	int pageStates[refStringSize][M+1];
	for (int i = 0; i < refStringSize; i++) {
		if (!inList(referenceString[i])) {
			totalFaults++;
			calculatePageLabels(i);
			swapOutHighestLabelPage(referenceString[i]);
			pageStates[i][M] = 1;
		} else {
			pageStates[i][M] = 0;
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

		if(inList(referenceString[i]))
		{
			//no fault just update the timer in the stored list
			updateTime(referenceString[i]);
			pageStates[i][M] = 0;
		} else {
			//fault and add to list
			addNewPageLRU(referenceString[i]);
			totalFaults++;
			pageStates[i][M] = 1;
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
