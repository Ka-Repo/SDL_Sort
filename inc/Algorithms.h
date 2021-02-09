#pragma once

#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>
#include <random>
#include <type_traits>

class Algorithms
{
public:
	template<typename I, typename N> static void mergesort(I begin, I end, N pred);
	template<typename I, typename N> static void bubblesort(I begin, I end, N pred);
	template<typename I, typename N> static void quicksort(I begin, I end, N pred);
	template<typename I, typename N> static void selectionsort(I begin, I end, N pred);
	template<typename I, typename N> static void bogosort(I begin, I end, N pred);
};

template<typename I, typename O, typename N>
void merge(I begin, I mid, I end, N pred, O buffer)
{
    I left = begin;
    I right = mid;

    while (left != mid && right != end)
    {
        if (*left < *right)
        {
            buffer.emplace_back(*left);
            ++left;
        }
        else
        {
            buffer.emplace_back(*right);
            ++right;
        }
    }

    buffer.insert(buffer.end(), left, mid);
    buffer.insert(buffer.end(), right, end);

    std::move(buffer.begin(), buffer.end(), begin);
}

template<typename I, typename O, typename N>
void mergesort_helper(I begin, I end, O &buffer, N pred)
{
    auto size = std::distance(begin, end);

    if (size <= 1) return;

    I mid = std::next(begin, size / 2);

    mergesort_helper(begin, mid, buffer, pred);
    mergesort_helper(mid, end, buffer, pred);
    merge(begin, mid, end, pred, buffer);
}

//************************************************************
// Description: Divide the unsorted list into n sublists,    *
//              each containing one element, this list is    *
//              considered sorted. Repeatedly merge sublists *
//              to produce new sorted sublists until there   *
//              is only one sublist remaining. This will be  *
//              the sorted list.                             *
//                                                           *
// Complexity: Ω(n log n), θ(n log n), O(n log n)            *                                                    
//************************************************************
template<typename I, typename N>
void Algorithms::mergesort(I begin, I end, N pred)
{
    using type = std::decay_t<decltype(*begin)>;
    std::vector<type> buffer;
    buffer.reserve(std::distance(begin, end));

    mergesort_helper(begin, end, buffer, pred);
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

//************************************************************
// Description: If U is the unsorted part of a list and S    *
//              the sorted part. Find the smallest element   *  
//              in U and swap it with the leftmost element   *
//              in U and move the boundaries of U one to     *
//              the right.                                   *
//                                                           *
// Complexity: Ω(n^2), θ(n^2), O(n^2)                        *                                                               
//************************************************************
template <typename I, typename N>
void Algorithms::selectionsort(I begin, I end, N pred)
{
    while (begin != end)
    {
        Swap(*begin, *Min(begin, end, pred));
        ++begin;
    }
}

//************************************************************
// Description: As long as the list is not sorted, step      *
//              through the list and swap adjacent elements  * 
//              if they are in wrong order.                  *
//                                                           *
// Complexity: Ω(n), θ(n^2), O(n^2)                          *                                                    
//************************************************************
template<typename I, typename N>
void Algorithms::bubblesort(I begin, I end, N pred)
{
    bool swapped = false;

    do {
        swapped = false;
        I it = begin;
        while ((it + 1) != end)
        {
            if (pred(*it, *(it + 1)))
            {
                Swap(*it, *(it + 1));
                swapped = true;
            }
            ++it;
        }
    } while (swapped);
}

template<typename I, typename N>
I Partition(I begin, I end, N pred)
{
    I pivot = std::prev(end, 1);
    I i = begin;
    I j = begin;

    while (j != pivot)
    {
        if (pred(*j, *pivot)) {
            Swap(*i, *j);
            ++i;
        }
        ++j;
    }

    Swap(*i, *pivot);
    return i;
}

//************************************************************
// Description: Select a pivot element from a list, devide   *
//              the list in two sublist according to         *
//              whether they are less or greater than the    *
//              pivot. Repeat this step for each sublist     *
//              recursivly.                                  *
//                                                           *
// Complexity: Ω(n log n), θ(n log n), O(n^2)                *                                                    
//************************************************************
template<typename I, typename N>
void Algorithms::quicksort(I begin, I end, N pred)
{
    if (std::distance(begin, end) <= 1) return;

    I pivot = Partition(begin, end, pred);

    Algorithms::quicksort(begin, pivot, pred);
    Algorithms::quicksort(pivot + 1, end, pred);
}

template<typename I, typename N>
bool Is_sorted(I begin, I end, N pred)
{
    I next = begin + 1;

    if (begin == end) return true;

    while (next != end)
    {
        if (!pred(*next, *begin)) return false;
        ++begin;
        ++next;
    }
    return true;
}

//************************************************************
// Description: Generate permutations of the input           *
//              until one turns out sorted.                  *
//                                                           *
// Complexity: Ω(n), θ((n+1)!) , O((n+1)!)                   *                                                    
//************************************************************
template<typename I, typename N>
void Algorithms::bogosort(I begin, I end, N pred)
{
    std::mt19937 generator{ std::random_device{}() };

    while (!Is_sorted(begin, end, pred)) {
        std::shuffle(begin, end, generator);
    }
}
