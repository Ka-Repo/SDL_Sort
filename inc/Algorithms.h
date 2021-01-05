#pragma once

#include <vector>

class Algorithms
{
private:
public:
	template <typename I, typename N> static void mergesort(I begin, I end, N pred);
	template <typename I, typename N> static void bubblesort(I begin, I end, N pred);
	template <typename I, typename N> static void quicksort(I begin, I end, N pred);
	template <typename I, typename N> static void selectionsort(I begin, I end, N pred);
	template <typename I, typename N> static void bogo_sort(I begin, I end, N pred);
};
