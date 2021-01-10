#include "Algorithms.h"

#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>
#include <random>

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
void mergesort_helper(I begin, I end, O buffer, N pred)
{
    auto size = std::distance(begin, end);

    if (size <= 1) return;

    auto mid = std::next(begin, size / 2);

    mergesort_helper(begin, mid, buffer, pred);
    mergesort_helper(mid, end, buffer, pred);
    merge(begin, mid, end, pred, buffer);
}

template<typename I, typename N>
void Algorithms::mergesort(I begin, I end, N pred)
{
    auto x = *begin;
	std::vector<decltype(x)> buffer;

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

template <typename I, typename N>  
void Algorithms::selectionsort(I begin, I end, N pred)
{
    while (begin != end)
    {
        Swap(*begin, *Min(begin, end, pred));
        ++begin;
    }
}

template<typename I, typename N>
void Algorithms::bubblesort(I begin, I end, N pred)
{
    bool swapped = false;

    do {
        swapped = false;
        auto it = begin;
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
    auto pivot = std::prev(end, 1);
    auto i = begin;

    for (auto j = begin; j != pivot; ++j) {
        if (pred(*j, *pivot)) {
            Swap(*i++, *j);
        }
    }

    Swap(*i, *pivot);
    return i;
}

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
    while (begin != end && next != end)
    {
        if (!pred(*next, *begin)) return false;
        ++begin;
        ++next;
    }
    return true;
}

template<typename I, typename N>
void Algorithms::bogosort(I begin, I end, N pred)
{
    std::mt19937 generator{ std::random_device{}() };

    while (!Is_sorted(begin, end, pred)) {
        std::shuffle(begin, end, generator);
    }
}