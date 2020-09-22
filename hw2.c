#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <windows.h>

struct SubArray
{
    int max;
    int startIndex;
    int endIndex;
};

typedef struct SubArray SUBARR;
typedef SUBARR* PTR_TO_SUBARR;


PTR_TO_SUBARR MaxSubBF(int* arr, int n)
{
    int maxSum = arr[0];
    int a = 0;
    int b = 0;

    for(int i = 0; i < n; i++)
    {
        int sum = 0;
        for(int j = i; j < n; j++)
        {
            sum += arr[j];
            if(sum > maxSum)
            {
                maxSum = sum;
                a = i;
                b = j;
            }
        }
    }

    PTR_TO_SUBARR subArr = (PTR_TO_SUBARR)calloc((b - a) + 1, sizeof(SUBARR));
    subArr->max = maxSum;
    subArr->startIndex = a;
    subArr->endIndex = b + 1;

    return subArr;
}

PTR_TO_SUBARR findMaxCrossing(int * arr, int low, int mid, int high)
{
    int leftSum = INT_MIN;
    int sum = 0;
    int a;

    for(int i = mid; i >= low; i--)
    {
        sum += arr[i];
        if(sum > leftSum)
        {
            leftSum = sum;
            a = i;
        }
    }

    int rightSum = INT_MIN;
    sum = 0;
    int b;

    for(int j = mid + 1; j <= high; j++)
    {
        sum += arr[j];
        if(sum > rightSum)
        {
            rightSum = sum;
            b = j;
        }
    }

    PTR_TO_SUBARR subArr = (PTR_TO_SUBARR)calloc((b - a) + 1, sizeof(SUBARR));
    subArr->max = leftSum + rightSum;
    subArr->startIndex = a;
    subArr->endIndex = b + 1;

    return subArr;
}

PTR_TO_SUBARR findMaxSub(int * arr, int low, int high)
{
    /*
    if(high < 200)
    {
        return MaxSubBF(arr, high + 1);
    }
    */

    if(high == low)
    {
        PTR_TO_SUBARR subArr = (PTR_TO_SUBARR)calloc((high - low) + 1, sizeof(SUBARR));
        subArr->max = arr[low];
        subArr->startIndex = low;
        subArr->endIndex = high + 1;
        return subArr;
    }


    int mid = (low + high) / 2;
    PTR_TO_SUBARR leftSubArr = findMaxSub(arr, low, mid);
    PTR_TO_SUBARR rightSubArr = findMaxSub(arr, mid + 1, high);
    PTR_TO_SUBARR crossSubArr = findMaxCrossing(arr, low, mid, high);

    if((leftSubArr->max >= rightSubArr->max) && (leftSubArr->max >= crossSubArr->max))
    {
        return leftSubArr;
    }
    else if((rightSubArr->max >= leftSubArr->max) && (rightSubArr->max >= crossSubArr->max))
    {
        return rightSubArr;
    }
    else
    {
        return crossSubArr;
    }
}

void fillList(int* arr, int n)
{
    srand(clock());
    for(int i = 0; i < n; i++)
    {
        arr[i] = (rand() % 1000) - 500;
    }
}

void printList(int* arr, int a, int b)
{
    for(int i = a; i < b; i++)
    {
        printf("[%d]", arr[i]);
    }
    printf("\n");
}

void printSubArr(int * arr, PTR_TO_SUBARR subArr)
{
    printf("Max Subarry is: [%d...%d]\n", subArr->startIndex, subArr->endIndex);
    printf("Max is: %d\n", subArr->max);
}

int main()
{
    int BFwins = 0;
    int DQwins = 0;
    int ties = 0;

    for(int runs = 0; runs < 10000; runs++)
    {
        int problemSize = 199;
        int arr[problemSize];
        fillList(arr, problemSize);;
        //printList(arr, 0, problemSize);

        printf("Problem size %d: \n\n", problemSize);


        clock_t start, end;

        start = clock();
        PTR_TO_SUBARR bf = MaxSubBF(arr, problemSize);
        end = clock();
        double BFtime = ((double)(end - start)) / CLOCKS_PER_SEC;

        printf("Brute Force:\n");
        printSubArr(arr, bf);
        printf("Time is: %f\n", BFtime);

        printf("\n");

        start = clock();
        PTR_TO_SUBARR rec = findMaxSub(arr, 0, problemSize - 1);
        end = clock();
        double DCtime = ((double)(end - start)) / CLOCKS_PER_SEC;

        printf("Divide and Conquer:\n");
        printSubArr(arr, rec);
        printf("Time is: %f\n\n", DCtime);

        if(BFtime < DCtime)
        {
            printf("Brute Force was faster\n");
            BFwins++;
        }
        else if(DCtime < BFtime)
        {
            printf("Divide and Conquer was faster\n");
            DQwins++;
        }
        else
        {
            printf("Both methods took equal time\n");
            ties++;
        }
    }

    printf("Brute Force:%d\nDivide and Conquer:%d\nTies:%d\n", BFwins, DQwins, ties);

    return 0;
}
