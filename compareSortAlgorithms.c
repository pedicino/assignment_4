#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

// global variable to track extra memory
int extraMemoryAllocated;

// supplementary merge halves function for mergeSort
int merge(int pData[], int l, int m, int r)
{
    int extraMemoryAllocated = 0;
    
    // left and right side length
    int nLeft = m-l + 1;
    int nRight = r-m;

    // tmp left and right arrays
    int tmpLeft[nLeft], tmpRight[nRight];
    
    // copy values from pData array into left and right tmp arrays
    for(int i = 0; i < nLeft; ++i) tmpLeft[i] = pData[l+i];
    for(int i = 0; i < nRight; ++i) tmpRight[i] = pData[m+1 + i];

    // k serves as counter for pData itself (i for left, j for right)
    int i = 0, j = 0, k = l;

    // Iterate through both arrays simultaneously
    while(i < nLeft && j < nRight)
    {
        // If cur value left arr <= cur val right arr
        // assign cur pData to left's value, iterate left arr
        if(tmpLeft[i] <= tmpRight[j]) {pData[k] = tmpLeft[i]; ++i;}

        // otherwise, perform for right arr
        else {pData[k] = tmpRight[j]; ++j;}
        
        ++k;
    }

    // copy leftovers
    while (i < nLeft || j < nRight) {

        // assuming left finished first
        if (i == nLeft)
        {
            // copy vals from right into pData
            pData[k] = tmpRight[j];
            k++;
            j++;
        }
        // assuming right finished first
        else if (j == nRight)
        {
            // copy vals from left into pData
            pData[k] = tmpLeft[i];
            k++;
            i++;
        }
        // if neither finish completely first...
        else if (tmpLeft[i] <= tmpRight[j])
        {
            // if the left value is smaller, copy into pData
            pData[k] = tmpLeft[i];
            k++;
            i++;
        }
        // same for right, respectively
        else
        {
            pData[k] = tmpRight[j];
            k++;
            j++;
        }

    }

    // extra memory
    extraMemoryAllocated += nLeft * sizeof(int) + nRight * sizeof(int);
    return extraMemoryAllocated;
}

// implement merge sort (recursive portion)
void mergeSort(int pData[], int l, int r)
{
	if (l < r) 
    {
        // middle index
        int m = l + (r - l) / 2;
  
		// left side
        mergeSort(pData, l, m);
		// right side
        mergeSort(pData, m + 1, r);
	
		//combine halves
        extraMemoryAllocated = merge(pData, l, m, r);
    }
}


// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	extraMemoryAllocated = 0;

	// consider each element individually (first sorted by default)
	for(int i = 1; i < n; ++i)
	{
		// Save current val to be inserted at designated pos
		int cur = pData[i];

		// Begin at end of sorted portion
		// If the element at j exceeds cur, continue to iterate in reverse
		int j;
		for(j = i - 1; j >= 0 && pData[j] > cur; --j)
		{
			// shift for space
			pData[j+1] = pData[j];
			// track memory
			extraMemoryAllocated += sizeof(int);
		}
		// insert the val being assessed into its pos
		pData[j+1] = cur;
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	extraMemoryAllocated = 0;

	// Loop n times, once for each int in array
	for(int i = 0; i < n; ++i)
	{
		// Comparing each adjacent int 
		// (last element doesn't have one to the right)
		for(int j = 0; j < n-1; ++j)
		{
			// ints are swapped if wrong order
			if(pData[j] > pData[j+1])
			{
				int tmp = pData[j];
				pData[j] = pData[j+1];
				pData[j+1] = tmp;

				//track memory for 3 vars
				extraMemoryAllocated += sizeof(int) * 3;
			}
		}
	}
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	extraMemoryAllocated = 0;

	// Loop until n-1 (last element sorted by default)
	for(int i = 0; i < n-1; ++i)
	{
		// Define index of minimum value in array
		int minIdx = i;
		
		// Begin one index ahead, assuming that is unsorted portion
		for(int j = i+1; j < n; ++j)
		{
			// define new index of min
			// when val at cur index < val at known min index
			if (pData[j] < pData[minIdx]) minIdx = j;
		}

		// swap use case
		if(minIdx != i)
		{
			int tmp = pData[i];
			pData[i] = pData[minIdx];
			pData[minIdx] = tmp;

			//track memory for 3 vars
			extraMemoryAllocated += sizeof(int) * 3;
		}
	}
}


// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);

		// Implement parse data block
		for(int i = 0; i < dataSz; ++i)
		{
			// Scan address of current integer
			fscanf(inFile, " %d", (*ppData)+i);
		}
	}

	fclose(inFile);
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}