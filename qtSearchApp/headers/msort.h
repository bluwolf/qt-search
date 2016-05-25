#ifndef MSORT_H
#define MSORT_H


#include <deque>


using namespace std;


template <class T, class Comparator>
void mergeSort (deque<T> & myArray, Comparator comp)
{
	msortHelper (myArray, 0, (myArray.size()) - 1, comp);
}

template <class T, class Comparator>
void msortHelper (deque<T> & myArray, int start, int end, Comparator comp)
{
	if (start < end)
	// the list's size is greater than 1
	{
		int midpoint = (start + end)/2;
		msortHelper (myArray, start, midpoint, comp);
		msortHelper (myArray, midpoint + 1, end, comp);
		merge (myArray, start, midpoint, midpoint + 1, end, comp);
	}
}

template <class T, class Comparator>
void merge (deque<T> & myArray, int s1, int e1, int s2, int e2, Comparator comp)
{
	int begin = s1;
	deque<T> temp;
	// push the values into temp in order

	while (s1 <= e1 || s2 <= e2)
	// when the start is equal to the end, that section has been looked at
	{
		if ((s1 <= e1) && ((s2 > e2) || comp(myArray[s1], myArray[s2])))
		// if the start is less than the end, there is more to be checked in this section
		// if s2 > e2, the entire second section has been checked and the remaining is filled in by the first section
		// if the value at s1 is less than the value at s2, then s1 comes before s2
		{
			temp.push_back (myArray[s1]);
			++s1;
		}
		else
		{
			temp.push_back (myArray[s2]);
			++s2;
		}
	}

	for (int k = 0; k < temp.size(); k++)
	// copy the ordered values from temp into myArray
	{
		myArray[begin+k] = temp[k];
	}
}

// NOTES
// Change the copying over to go from s1 to e2
// Change values of the deque
// Make functor that takes in set (?) and compares that
// Put the functor structs in the class definition or in main

#endif
