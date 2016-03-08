/*
+----------------------------------------------------------------------+
| Author:                 YunDuan                                         |
+----------------------------------------------------------------------+
*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>

#define RAND_RANGE(__n, __min, __max, __tmax) \
    (__n) = (__min) + (long) ((double) ( (double) (__max) - (__min) + 1.0) * ((__n) / ((__tmax) + 1.0)))

#define LENTH 10


void exchangeNum(int *a, int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}


/**
 * 冒泡排序
 */
int BUBBLE_SORT(int * arr, int lenth)// O(N*N)
{
	if (lenth < 2) return -1;
	int i, j;
	for (i = 1; i < lenth; i++) {//i 控制循环次数
		for (j = 0; j < lenth - i; j++) {//j 每次循环 找出最大值  冒泡
			if (arr[j] > arr[j+1]){
				exchangeNum(&arr[j], &arr[j+1]);
			}
		}
	}
	return 0;
}


/**
 * 选择排序
 */
int SELECT_SORT(int *arr, int lenth)
{
	if (lenth < 2) return -1;
	int i, j, minIndex;
	for (i = 0; i < lenth; ++i) {// i 控制循环次数
		minIndex = i;
		for (j = i + 1; j < lenth; ++j) {// 一次循环找出最小值下标  当次循环完了即找出最小值 交换位置
			if (arr[minIndex] > arr[j]) {
				minIndex = j;
			}
		}
		if (arr[minIndex] != arr[i]) exchangeNum(&arr[minIndex], &arr[i]);
	}
	return 0;
}


/**
 * 插入排序
 */
int INSERTION_SORT(int *arr, int lenth)
{
	if (lenth < 2) return -1;
	int i, j, currentVal;
	for (i = 1; i < lenth; ++i) { //第二个数开始
		currentVal = arr[i];
		j = i;
		while (j > 0 && arr[j-1] > currentVal)  //while 作用是 寻找currentVal插入的合适位置
		{
			arr[j] = arr[j-1];//大元素往后移动  空出一个空间
			j--;
		}
		arr[j] = currentVal;
	}
	return 0;
}


/**
 * 桶排序
 */
int BUCKET_SORT(int *arr, int lenth)
{
	int bucket[lenth] = calloc(lenth, sizeof(int)), i, j;

}




int PARTITION(int *arr, int startIndex, int endIndex) {
	int minIndexLimit = startIndex - 1, notSortIndex;
	for (notSortIndex = startIndex; notSortIndex < endIndex; notSortIndex++)
		if (arr[notSortIndex] <= arr[endIndex])
			exchangeNum(&arr[++minIndexLimit], &arr[notSortIndex]);

	exchangeNum(&arr[++minIndexLimit], &arr[endIndex]);
	return minIndexLimit;
}


int createRandNum(int startIndex, int endIndex)
{
	struct timeval t_val;
	gettimeofday(&t_val, NULL);
	srand((unsigned)t_val.tv_usec);
	int randomizedIndex =  (rand() % (endIndex - startIndex)) + startIndex;

	/*RAND_RANGE(randomizedIndex, startIndex, endIndex , RAND_MAX);
	return randomizedIndex;*/

}


int  RANDOMIZED_PARTITION(int *arr, int startIndex, int endIndex) {
	int randomizedIndex = createRandNum(startIndex, endIndex);//startIndex;//
	printf("randomizedIndex = %d \n", randomizedIndex);
	exchangeNum(&arr[randomizedIndex], &arr[endIndex]);
	return PARTITION(arr, startIndex, endIndex);
}


/*
 * 选择第几小的数
 * index:第几小的数
 */
int RANDOMIZED_SELECT(int *arr, int startIndex, int endIndex, int index)
{
	int midlleIndex, k;
	if (startIndex == endIndex) return arr[endIndex];
	midlleIndex = RANDOMIZED_PARTITION(arr, startIndex, endIndex);
	k = midlleIndex -startIndex + 1;
	if (index == k) return arr[midlleIndex];
	else if (index < k) return RANDOMIZED_SELECT(arr, startIndex, midlleIndex-1, index);
	else return RANDOMIZED_SELECT(arr, midlleIndex+1, endIndex, index-k);
}

int main (void)
{


	int arr[] = {10,55,1,485,2,4,35,8,5,265};
	//printf("第%d小的数是： %d \n", 10, RANDOMIZED_SELECT(arr, 0, LENTH - 1, 10));
	//BUBBLE_SORT(arr, LENTH);
	//SELECT_SORT(arr, LENTH);
	INSERTION_SORT(arr, LENTH);
	for (int i = 0; i < LENTH; ++i) {
		printf("arr[%d] = %d \n", i , arr[i]);
	}

	return 0;

	printf("%d --- %d === %d \n", 100000, 200000, createRandNum(100000, 200000));
	return 0;

	int a = 1, b=2;
	printf("a = %d, b = %d \n", a , b);
	exchangeNum(&a, &b);
	printf("a = %d, b = %d \n", a , b);

	return 0;
}
