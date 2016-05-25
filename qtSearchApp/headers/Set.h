#ifndef SET_H
#define SET_H

#include <set>
#include <stdexcept>

template <class T>
class Set : public std::set<T> {
	public:
		Set ();
		Set (const Set<T> & other);
		~Set ();
		// returns true if Set contains the item; otherwise false
		bool contains (const T & item) const;
		// returns Set that is intersection of current Set and other
		Set<T> setIntersection (const Set<T> & other) const;
		// returns Set that is union of current Set and other
		Set<T> setUnion (const Set<T> & other) const;
};

template <class T>
Set<T>::Set () : std::set<T>::set()
{}

template <class T>
Set<T>::Set (const Set<T> & other) : std::set<T>::set(other)
{}

template <class T>
Set<T>::~Set ()
{}

template <class T>
bool Set<T>::contains (const T & item) const
{
	// check every value
	typename std::set<T>::iterator it = std::set<T>::find(item);
	if (it != std::set<T>::end())
		return true;

	// if no matches found, return false
	return false;
}


template <class T>
Set<T> Set<T>::setIntersection (const Set<T> & other) const
{
	// initialize new set to be returned
	Set<T> newSet;

	// go through this set and determine if other contains the same items
	// if so, add those items to newSet
	for (typename std::set<T>::iterator it = std::set<T>::begin(); it != std::set<T>::end(); ++it)
	{
		if (other.contains(*it))
			newSet.insert(*it);
	}

	return newSet;
}

template <class T>
Set<T> Set<T>::setUnion (const Set<T> & other) const
{
	// initialize new set by copying other
	Set<T> newSet(other);

	// add all items of this set that are not already in newSet
	for (typename std::set<T>::iterator it = std::set<T>::begin(); it != std::set<T>::end(); ++it)
	{
		if (!newSet.contains(*it))
			newSet.std::set<T>::insert(*it);
	}

	return newSet;
}

#endif
