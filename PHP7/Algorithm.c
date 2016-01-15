/*
+----------------------------------------------------------------------+
| Author:                 YunDuan                                         |
+----------------------------------------------------------------------+
*/
#include <stdio.h>
#define LENTH 11


void exchangeNum(int *a, int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}


/**
 * 冒泡排序
 */
void BUBBLE_SORT(int * arr, int lenth)
{
	int i, j;
	for (i = 0; i < lenth; ++i) {
		for (j = i+1; j < lenth; ++j) {
			if (arr[i] > arr[j]){
				exchangeNum(&arr[i], &arr[j]);
			}
		}
	}
}



void SELECT_SORT(int *arr, int lenth)
{
	int i, j, minIndex;
	for (i = 0; i < lenth; ++i) {
		minIndex = i;
		for (j = i + 1; j < lenth; ++j) {
			if (arr[minIndex] > arr[j]) {
				minIndex = j;
			}
		}
		if (arr[minIndex] != arr[i]) exchangeNum(&arr[minIndex], &arr[i]);
	}
}


int main (void)
{

	int arr[] = {10,55,1,485,2,4,35,8,4,5,265};
	//BUBBLE_SORT(arr, LENTH);
	SELECT_SORT(arr, LENTH);
	for (int i = 0; i < LENTH; ++i) {
		printf("arr[%d] = %d \n", i , arr[i]);
	}

	return 0;

	int a = 1, b=2;
	printf("a = %d, b = %d \n", a , b);
	exchangeNum(&a, &b);
	printf("a = %d, b = %d \n", a , b);

	return 0;
}
