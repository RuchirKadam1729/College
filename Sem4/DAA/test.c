#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define LEN(A) sizeof(A)/sizeof(int)
#define RUN(A) {run(A, LEN(A));}

int cmp = 0;
int ex = 0;

void swap(int *a, int *b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int A[], int p, int r) 
{
    int i = p - 1;
    int x = A[r];
    for (int j = p; j < r; j++) {
        if (A[j] <= x) {
            cmp++;
            i++;
            swap(A+i,A+j);
        }
    }
    swap(A+i+1,A+r);
    ex++;
    return i+1;
}

void quickSort(int A[], int p, int r) 
{
    if(p < r) {
        int q = partition(A, p, r);
        quickSort(A,p,q-1);
        quickSort(A, q+1,r);
    }
}


void generate_worst_case(int arr[], int n) //Worst Case: sorted list
{
    for (int i = 0; i < n; i++) 
        arr[i] = i;
}

void best_case_helper(int arr[], int start, int end);
void generate_best_case(int arr[], int n) {
    for (int i = 0; i < n; i++)
        arr[i] = i;
    best_case_helper(arr, 0, n - 1);
}
void best_case_helper(int arr[], int start, int end) //Best Case: pivots are always median
{
    if (start >= end) return;

    int mid = (start + end) / 2;
    int temp = arr[mid];
    for (int i = mid; i <= end-1; i++)
        arr[i] = arr[i+1];
    
    arr[end] = temp;
    best_case_helper(arr, start, mid - 1);
    best_case_helper(arr, mid, end - 1);
}

void generate_average_case(int arr[], int n) 
{
    for (int i = 0; i < n; i++) 
        arr[i] = i;
    
    srand(time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void printArr(int A[], int n) 
{
	for (int i = 0; i < n; i += 1)
		printf("%d ", A[i]);
}

void run(int A[], int n) 
{
    cmp = 0; 
    ex = 0;
    printf("Original Arr.:\t");
    //printArr(A, n);
    printf("(%d elements)", n);
    quickSort(A, 0, n-1);
    printf("\nSorted Arr.:\t");
    //printArr(A, n);
    printf("\ncmp: %d   ex: %d\n", cmp, ex);
}

int main() 
{
    int n = 3000;
    int O[n];
    generate_worst_case(O, LEN(O));
    
    int W[n];
    generate_best_case(W, LEN(W));
    
    int T[n];
    generate_average_case(T, LEN(T));
    
    printf("Worst Case:\n");
    RUN(O);
    printf("\nBest Case:\n");
    RUN(W);
    printf("\nAvg Case:\n");
    RUN(T);
}
