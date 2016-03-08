<?php
/*
+----------------------------------------------------------------------+
| Author:                 YunDuan                                         |
+----------------------------------------------------------------------+
*/



/** **********************最大连续子数组****************************************************************/
function FIND_MAX_CROSSING_SUBARRAY($A, $low, $mid, $high){
	
	$max_left_index = 0;
	$leftSum = 0;
	$sum = 0;
	for ($i = $mid; $i >= $low; $i--) {
		$sum += $A[$i];
		if ($sum > $leftSum) {
			$leftSum = $sum;
			$max_left_index = $i;
		}
	}
	
	$max_right_index = 0;
	$rightSum = 0;
	$sum = 0;
	for ($i = $mid+1; $i <= $high; $i++) {
		$sum += $A[$i];
		if ($sum > $rightSum) {
			$rightSum = $sum;
			$max_right_index = $i;
		}
	}
	
	return array($max_left_index, $max_right_index, $leftSum + $rightSum);
}


function FIND_MAXMUN_SUBARRAY($A, $low, $high) {
	
	if ($low == $high) {
		return  array($low, $high, $A[$low]);
	}else {
		$mid = floor(($low+$high)/2);
		list($low_left_index, $high_left_index, $leftSum) = FIND_MAXMUN_SUBARRAY($A, $low, $mid);
		list($low_right_index, $high_right_index, $rightSum) = FIND_MAXMUN_SUBARRAY($A, $mid+1, $high);
		list($low_cross_index, $high_cross_index, $crossSum) = FIND_MAX_CROSSING_SUBARRAY($A, $low, $mid, $high);
		
		if ($leftSum > $rightSum && $leftSum > $crossSum) {
			return  array($low_left_index, $high_left_index, $leftSum);
		}elseif ($rightSum > $leftSum && $rightSum > $crossSum){
			return  array($low_right_index, $high_right_index, $rightSum);
		}else {
			return  array($low_cross_index, $high_cross_index, $crossSum);
		}
	}
}


/** **********************动态规划**********************************************************************/
/**
 * $priceArr 对应价格
 * $n 问题规模
 */
function EXTENDED_BOTTOM_UP_CUT_ROD($priceArr, $n) {
	$r = array();//长度为J的最大收益
	$s = array();//长度为J的第一段切割
	
	$r[0] = 0;
	for ($j = 1; $j <= $n; $j++) {
		$sumPrice = 0;
		for ($i = 1; $i <= $j; $i++) {
			if ($sumPrice < $priceArr[$i] + $r[$j-$i]) {
				$sumPrice = $priceArr[$i] + $r[$j-$i];
				$s[$j] = $i;
			}
		}
		$r[$j] = $sumPrice;
	}
	
	echo 'array(';
	foreach ($priceArr as $key => $value) {
		echo $key,'=>',$value,', ';
	}
	echo ')';
	
	echo 'array(';
	foreach ($r as $key => $value) {
		echo $key,'=>',$value,', ';
	}
	echo ')';
	
	echo 'array(';
	foreach ($s as $key => $value) {
		echo $key,'=>',$value,', ';
	}
	echo ')';
	
	while ($n > 0) {
		echo $s[$n],',';
		$n = $n - $s[$n];
	}
}
//EXTENDED_BOTTOM_UP_CUT_ROD(array(1=>1,2=>5,3=>8,4=>9,5=>10,6=>17,7=>17,8=>20,9=>24,10=>30),8);


/*************************************** 基数排序 ******************************************************/

function RADIX_SORT(&$arr, $maxDigit) {
	for ($i = 1; $i <= $maxDigit; $i++) {
		for ($k = 0; $k <= 10; $k++) $C[$k] = 0;
		for ($j = 0; $j < count($arr); $j++) {
			$C[floor($arr[$j]/pow(10, $i-1))%10] = $C[floor($arr[$j]/pow(10, $i-1))%10] + 1;
		}
	}
}


/*************************************** 计数排序 ******************************************************/
/**
 * 计数排序
 * @param array $arr
 * @return array
 */
function COUNT_SORT($arr) {
	$index = count($arr);
	$maxIndex = 0;
	for ($i = 1; $i < $index; $i++) if($arr[$i] > $arr[$maxIndex]) $maxIndex = $i;//最大值
	for ($i = 0; $i <= $arr[$maxIndex]; $i++) $C[$i] = 0;//初始化Count数组
	for ($i = 0; $i < $index; $i++) $C[$arr[$i]] = $C[$arr[$i]] + 1;//计数
	for ($i = 1; $i <= $arr[$maxIndex]; $i++) $C[$i] = $C[$i] + $C[$i-1];//累计计数
	var_dump($C);
	$B = array();
	for ($i = $index-1; $i >=0; --$i) {
		$B[$C[$arr[$i]]] = $arr[$i];//排序
		$C[$arr[$i]] = $C[$arr[$i]] - 1;
	}
	return $B;
}


/***************************************快速排序 尾递归 随机化 优化版******************************************************/
function OPTIMIZE_TAIL_RECURSION_RANDOMIZED_QUICK_SORT(&$arr, $startIndex, $endIndex) {
	while ($startIndex < $endIndex) {
		$midlleIndex = RANDOMIZED_PARTITION($arr, $startIndex, $endIndex);
		if ($midlleIndex - $startIndex < $endIndex - $midlleIndex) {//将分得的元素数量较少的使用递归排序 较多的使用迭代
			OPTIMIZE_TAIL_RECURSION_RANDOMIZED_QUICK_SORT($arr, $startIndex, $midlleIndex - 1);
			$startIndex = $midlleIndex + 1;//递增
		}else {
			OPTIMIZE_TAIL_RECURSION_RANDOMIZED_QUICK_SORT($arr, $midlleIndex+1, $endIndex);
			$endIndex = $midlleIndex - 1;//递减
		}
	}
}

/***************************************快速排序 尾递归 随机化版******************************************************/
/**
 * 尾递归 随机化版
 * @param array $arr
 * @param long $startIndex
 * @param long $endIndex
 */
function TAIL_RECURSION_RANDOMIZED_QUICK_SORT(&$arr, $startIndex, $endIndex) {
	while ($startIndex < $endIndex) {
		$midlleIndex = RANDOMIZED_PARTITION($arr, $startIndex, $endIndex);
		RANDOMIZED_QUICK_SORT($arr, $startIndex, $midlleIndex-1);
		//RANDOMIZED_QUICK_SORT($arr, $midlleIndex+1, $endIndex);
		$startIndex = $midlleIndex + 1;
	}
}

/***************************************快速排序 随机化版******************************************************/
function RANDOMIZED_QUICK_SORT(&$arr, $startIndex, $endIndex) {
	if ($startIndex < $endIndex) {
		$midlleIndex = RANDOMIZED_PARTITION($arr, $startIndex, $endIndex);
		RANDOMIZED_QUICK_SORT($arr, $startIndex, $midlleIndex-1);
		RANDOMIZED_QUICK_SORT($arr, $midlleIndex+1, $endIndex);
	}
}

function RANDOMIZED_PARTITION(&$arr, $startIndex, $endIndex) {
	$randomizedIndex = mt_rand($startIndex, $endIndex);
	exchangeNum($arr[$randomizedIndex], $arr[$endIndex]);
	return PARTITION($arr, $startIndex, $endIndex);
}


/***************************************快速排序******************************************************/
/**
 * 快速排序
 * @param array $arr
 * @param long $startIndex
 * @param long $endIndex
 * @return boolean
 */
function QUICK_SORT(&$arr, $startIndex, $endIndex) {
	if ($startIndex < $endIndex) {
		$middleIndex = PARTITION($arr, $startIndex, $endIndex);
		QUICK_SORT($arr, $startIndex, $middleIndex-1);
		QUICK_SORT($arr, $middleIndex+1, $endIndex);
	}else return false;
}


/**
 * 拆分
 * @param array $arr
 * @param long $startIndex
 * @param long $endIndex
 * @return number
 */
function PARTITION(&$arr, $startIndex, $endIndex) {
	$minIndexLimit = $startIndex - 1;
	for ($notSortIndex = $startIndex; $notSortIndex < $endIndex; $notSortIndex++) 
		if ($arr[$notSortIndex] <= $arr[$endIndex]) 
			exchangeNum($arr[++$minIndexLimit], $arr[$notSortIndex]);
	
	exchangeNum($arr[++$minIndexLimit], $arr[$endIndex]);
	return $minIndexLimit;
}


/*******************************************堆排序***************************************************/

/**
 * 堆排序
 * @param array $arr
 */
function HEAP_SORT(&$arr) {
	$heapSize = count($arr);
	for ($i = ($heapSize>>1)-1; $i >= 0; --$i) MAX_HEAPIFY($arr, $i, $heapSize);//BUILD_MAX_HEAP($arr);//建堆
	for ($i = $heapSize-1; $i > 0; --$i) {
		exchangeNum($arr[0], $arr[$i]);
		--$heapSize;
		MAX_HEAPIFY($arr, 0, $heapSize);
	}
}


/**
 * 保持堆特性
 * @param array $arr
 * @param long $parentIndex
 * @param long $heapSize
 */
function MAX_HEAPIFY(&$arr, $parentIndex, $heapSize) {
	$leftNodeIndex = ($parentIndex << 1)+1;
	if($leftNodeIndex >= $heapSize) return ;
	$rightNodeIndex = ($parentIndex << 1) + 2;
	//echo $heapSize,' ',$parentIndex,' ',$leftNodeIndex,' ',$rightNodeIndex,'<br>';
	$largestIndex = $parentIndex;
	if ($arr[$leftNodeIndex] > $arr[$parentIndex]) $largestIndex = $leftNodeIndex;
	if ($rightNodeIndex < $heapSize && $arr[$rightNodeIndex] > $arr[$largestIndex]) $largestIndex = $rightNodeIndex;
	if ($largestIndex != $parentIndex) {
		exchangeNum($arr[$largestIndex], $arr[$parentIndex]);
		MAX_HEAPIFY($arr, $largestIndex, $heapSize);
	}else return ;
}


/*******************************************归并排序*************************************************/
/**
 * 归并排序
 * @param array $arr
 * @param long $startIndex
 * @param long $endIndex
 */
function MERGE_SORT(&$arr, $startIndex, $endIndex) {
	
	if($startIndex >= $endIndex) return ;
	else {
		$middleIndex = floor(($startIndex + $endIndex)/2);
		MERGE_SORT($arr, $startIndex, $middleIndex);
		MERGE_SORT($arr, $middleIndex+1, $endIndex);
		MERGE($arr, $startIndex, $middleIndex, $endIndex);
	}
	return ;
}


/**
 * 合并
 * @param array $arr
 * @param long $startIndex
 * @param long $middleIndex
 * @param long $endIndex
 */
function MERGE(&$arr, $startIndex, $middleIndex, $endIndex) {
	$L = array();
	$R = array();
	$countL = $middleIndex - $startIndex + 1;
	$countR = $endIndex - $middleIndex;
	for ($i = $startIndex; $i <= $middleIndex; $i++) $L[] = $arr[$i];
	for ($i = $middleIndex + 1; $i <= $endIndex; $i++) $R[] = $arr[$i];
	//echo 'L:';var_dump($L);echo '<br>R:';var_dump($R);echo '-----<br>';
	for ($index = $startIndex, $i = 0, $j = 0; $index <= $endIndex; ++$index) {
		if($i >= $countL && $j < $countR) {
			$arr[$index] = $R[$j];
			++$j;
			continue;
		}else if($j >= $countR && $i < $countL) {
			$arr[$index] = $L[$i];
			++$i;
			continue;
		}
		if ($L[$i] <= $R[$j]) {
			$arr[$index] = $L[$i];
			++$i;
		}else {
			$arr[$index] = $R[$j];
			++$j;
		}
	}
}


/********************************************插入排序************************************************/
/**
 * 插入排序
 * @param array $arr
 */
function INSERTION_SORT(&$arr){
	$count = count($arr);
	if($count <= 1) return ;
	for ($i = 1; $i < $count; $i++) {//第二张开始
		$currentPoker = $arr[$i];// 即将插入的值 备份此值 O(1)
		$j = $i;
		while ($j > 0 && ($currentPoker < $arr[$j-1])) {//若是小于 则$arr[$j-1]往后移一位
			$arr[$j] = $arr[$j-1];
			--$j;
		}
		$arr[$j] = $currentPoker;//合适的位置插入
	}
}


/**
 * 交换变量
 * @param long $firstNum
 * @param long $secondNum
 */
function exchangeNum(&$firstNum, &$secondNum) {//用于同为整数和同为字符串 其他类型可转为字符后交换
	/*$firstNum ^= $secondNum;
	$secondNum ^= $firstNum;
	$firstNum ^= $secondNum;*/
	if($firstNum == $secondNum) return ;
	$temp = $firstNum;
	$firstNum = $secondNum;
	$secondNum = $temp;
}


/*********************************************选择排序*******************************************/

/**
 * 随机化 选择第几小的数
 * @param array $arr
 * @param long $startIndex
 * @param long $endIndex
 * @param long $index
 * @return long
 * 
 * $arr = [-1, -2, -3, 5, 1, 2];
 * echo RANDOMIZED_SELECT($arr, 0, 5, 3);die;
 */
function RANDOMIZED_SELECT(&$arr, $startIndex, $endIndex, $index) {
	if($startIndex == $endIndex) return $arr[$startIndex];
	$midlleIndex = RANDOMIZED_PARTITION($arr, $startIndex, $endIndex);
	$k = $midlleIndex - $startIndex +1;
	if ($index == $k) return $arr[$midlleIndex];
	elseif ($index < $k) return RANDOMIZED_SELECT($arr, $startIndex, $midlleIndex-1, $index);
	else return RANDOMIZED_SELECT($arr, $midlleIndex+1, $endIndex, $index-$k);
}


/*********************************************选择排序*******************************************/
/**
 * 选择排序
 * @param array $arr
 */
function SELECT_SORT(&$arr) {
	for ($i = 0,$index = count($arr); $i < $index-1; $i++) {
		$minIndex = $i;
		for ($j = $i+1; $j < $index; $j++) {
			if ($arr[$minIndex] > $arr[$j]) $minIndex = $j;//选出最小值的下标
		}
		if($arr[$minIndex] != $arr[$i]) exchangeNum($arr[$minIndex], $arr[$i]);
	}
}


/*******************************************冒泡排序*******************************************/
/**
 * 冒泡排序
 * @param array $arr
 */	
function BUBBLE_SORT(&$arr) {
	for ($i = 1,$index=count($arr); $i < $index; $i++) {//循环次数
		for ($j = 0; $j <= $index - $i; $j++) {//每轮循环
			if($arr[$j] > $arr[$j + 1]) exchangeNum($arr[$j], $arr[$j + 1]);
		}
	}
}

