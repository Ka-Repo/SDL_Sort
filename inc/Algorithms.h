#pragma once

#include <vector>

class Algorithms
{
private:
public:
	template <typename I, typename N> static void mergesort(I first, I last, N pred);
	template <typename I, typename N> static void bubblesort(I first, I last, N pred);
	template <typename I, typename N> static void quicksort(I first, I last, N pred);
	template <typename I, typename N> static void selectionsort(I first, I last, N pred);
};
