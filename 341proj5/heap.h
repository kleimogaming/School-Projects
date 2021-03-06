// CMSC 341 - Fall 2019 - Project 5
// heap.h
// Templated, vector-based heap implementation

// To work with Heap, the template class T must provide the following:
//  (a) Default constructor
//  (b) priority function (unsigned priority())
//  (c) overloaded insertion operator
// To work with Heap *and* HashTable, it also needs:
//  (d) key function (string key())

#ifndef _HEAP_H
#define _HEAP_H

#include <iostream>
#include <vector>
#include <exception>
#include <utility>   // swap

using std::vector;
using std::endl;
using std::cout;
using std::swap;
using std::range_error;

// To work with Heap and HashTable, class T must provide the
// following:
//  (a) Default constructor
//  (b) priority function (unsigned priority())
//  (c) key function (string key())
//  (d) overloaded insertion operator

class Grader;

template <class T>
class Heap {

  friend Grader;
  
 public:
  // Constructor
  Heap();

  // Inline functions: size, empty, used 
  unsigned size() const { return _heap.size() - 1 ; }
  bool empty() const { return _heap.size() <= 1 ; }
  bool used() const { return _used ; }

  // Main heap operations: insert, read, remove
  void insert(const T& object);
  T readTop() const;
  void removeTop();

  // Dump the entire heap
  void dump() const;

  // Root node always has index 1
  static const unsigned ROOT = 1;
  
 private:
  vector<T> _heap;   // Vector-based heap
  bool _used;        // Has the heap ever been used?  Needed for
		     // linear probing.

  // *********************************************
  // Private helper function declarations go here!
  // *********************************************

  void insertHelp(int index);
  void removeHelp(int index);
};

// Helper function for insert, figures out if new value is 
template <class T>
void Heap<T>::insertHelp(int index) {
  int parent = index / 2;
  if (parent == 0) {
    return;
  }

  // Fix max-heap order
  if (_heap[index].priority() > _heap[parent].priority()) {
    swap(_heap[index], _heap[parent]);
    insertHelp(parent);
  }

}

// Helper function for remove, figures out if they need to swap anything based off priorities
template <class T>
void Heap<T>::removeHelp(int index) {
  // Calculate the locations of the three elements to compare
  int leftIndex = index * 2;
  int rightIndex = (index * 2) + 1;
  int parentIndex = index;

  // If left child is greater than parent
  if ((leftIndex <= (int)size()) && (_heap[leftIndex].priority() > _heap[parentIndex].priority())) {
    parentIndex = leftIndex;
  }

  // If right child is greater than parent
  if ((rightIndex <= (int)size()) && (_heap[rightIndex].priority() > _heap[parentIndex].priority())) {
    parentIndex = rightIndex;
  }

  // If the original index was changed, swap the values
  if (parentIndex != index) {
    swap(_heap[index], _heap[parentIndex]);
    removeHelp(parentIndex);
  }
  
}

// Initializes the heap, marks it as unused
template <class T>
Heap<T>::Heap() {
  _used = false;
  T empty;
  _heap.push_back(empty);
}

// Inserts a new object into the heap, then puts it in the proper place using the max-heap property
template <class T>
void Heap<T>::insert(const T& object) {
  // This heap has now had an object put in it, so mark the heap as used
  _used = true;
  _heap.push_back(object);

  insertHelp(int(size()));
}

// If heap is empty, there's nothing to read so throw error
// Otherwise, return the root (top) element
template <class T>
T Heap<T>::readTop() const {
  // If heap is empty, can't read anything
  if (empty()) {
    throw range_error("Heap is empty");
  }
  // Retruns the highest priority element
  return _heap[ROOT];
}

//Removes the highest priority element in this heap, then reorders to keep max-heap property
template <class T>
void Heap<T>::removeTop() {
  // If heap is empty, can't remove anything
  if (empty()) {
    throw range_error("Heap is empty");
  }

  swap(_heap[ROOT], _heap[int(size())]);
  _heap.pop_back();
  removeHelp(ROOT);
}

// Prints out the contents of the data in the heap
// If more than one objects are stored, iterates through all of them
template <class T>
void Heap<T>::dump() const {
  for (unsigned int i = ROOT; i <= size(); i++) {
    cout << _heap[i] << endl;
  }
}
// ***************************************
// Templated function definitions go here!
// ***************************************

#endif
