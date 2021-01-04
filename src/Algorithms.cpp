#include "Algorithms.h"

#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>

template<typename I, typename O, typename N>
void merge(I first_l, I last_l, I first_r, I last_r, N pred, O buffer)
{
    auto start_l = first_l;
    auto end_l = last_l;
    auto start_r = first_r;
    auto end_r = last_r;

    while (start_l != end_l && start_r != end_r) 
    {
        if (*start_l < *start_r)
        {
            buffer->push_back(*start_l);
            ++start_l;
        }
        else 
        {
            buffer->push_back(*start_r);
            ++start_r;
		}
    }

    while (start_l != end_l) 
    {
        buffer->push_back(*start_l);
        ++start_l;
    }

    while (start_r != end_r) 
    {
        buffer->push_back(*start_r);
        ++start_r;
    }

    //  TODO: std::copy(buffer->begin(), buffer->end(), first_l);
    while (first_l != last_r && !buffer->empty())
    {
        *first_l = buffer->back();
        buffer->pop_back();
        ++first_l;
    }
}

template<typename I, typename O, typename N>
void mergesort_helper(I first, I last, O buffer, N pred)
{
    auto size = std::distance(first, last);

    if (size <= 1) return;

    auto mid = std::next(first, size / 2);

    mergesort_helper(first, mid, buffer, pred);
    mergesort_helper(mid + 1, last, buffer, pred);
    merge(first, mid, mid + 1, last, pred, buffer);
}

template<typename I, typename N>
void Algorithms::mergesort(I first, I last, N pred)
{
    auto x = *first;
	std::vector<decltype(x)> buffer;

    mergesort_helper(first, last, &buffer, pred);
}

template<typename I, typename N>
void Algorithms::bubblesort(I first, I last, N pred)
{
}

template<typename I, typename N>
void Algorithms::quicksort(I first, I last, N pred)
{
}

template <typename I, typename N>  
void Algorithms::selectionsort(I first, I last, N pred)
{

}


