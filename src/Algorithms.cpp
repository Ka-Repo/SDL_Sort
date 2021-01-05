#include "Algorithms.h"

#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>

template<typename I, typename O, typename N>
void merge(I begin, I mid, I end, N pred, O buffer)
{
    I begin_l = begin;
    I end_l = mid;
    I begin_r = mid;
    I end_r = end;

    while (begin_l != end_l && begin_r != end_r)
    {
        if (*begin_l < *begin_r)
        {
            buffer->emplace_back(*begin_l);
            ++begin_l;
        }
        else 
        {
            buffer->emplace_back(*begin_r);
            ++begin_r;
		}
    }

    while (begin_l != end_l)
    {
        buffer->push_back(*begin_l);
        ++begin_l;
    }

    while (begin_r != end_r)
    {
        buffer->push_back(*begin_r);
        ++begin_r;
    }
}

template<typename I, typename O, typename N>
void mergesort_helper(I begin, I end, O buffer, N pred)
{
    auto size = std::distance(begin, end);

    if (size <= 1) return;

    auto mid = std::next(begin, size / 2);

    mergesort_helper(begin, mid, buffer, pred);
    mergesort_helper(mid, end, buffer, pred);
    merge(begin, mid, end, pred, buffer);

    std::move(buffer->begin(), buffer->begin(), begin);
}

template<typename I, typename N>
void Algorithms::mergesort(I begin, I end, N pred)
{
    auto x = *begin;
	std::vector<decltype(x)> buffer;

    mergesort_helper(begin, end, &buffer, pred);
}

template<typename I, typename N>
void Algorithms::bubblesort(I begin, I end, N pred)
{
}

template<typename I, typename N>
void Algorithms::quicksort(I begin, I end, N pred)
{
}


template <typename T>
void Swap(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

template <typename I, typename N>
I Min(I begin, I end, N pred)
{
    I min_adr = begin;

    while (begin != end)
    {
        if (pred(*begin, *min_adr))
        {
            min_adr = begin;
        }
        ++begin;
    }

    return min_adr;
}

template <typename I, typename N>  
void Algorithms::selectionsort(I begin, I end, N pred)
{
    while (begin != end)
    {
        Swap(*begin, *Min(begin, end, pred));
        ++begin;
    }
}


