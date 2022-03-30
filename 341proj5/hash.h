// CMSC 341 - Fall 2019 - Project 5
// hash.h

// Templated, hash table implementation.  Each buckets is a heap.  A
// bucket contains objects with the same key values stored as a
// max-heap based on priority.  Collisions are resolved by linear
// probing.

// To work with Heap and HashTable, the template class T must provide
// the following:
//  (a) Default constructor
//  (b) priority function (unsigned priority())
//  (c) key function (string key())
//  (d) overloaded insertion operator

#ifndef _HASH_H
#define _HASH_H

#include <string>
#include "heap.h"

using std::string;

// Hash function typedef.  The hash function must return an 'unsigned
// int' value.  Reduction mod N is the responsiblity of the caller,
// not the hash function.
typedef unsigned (*hash_fn)(string);

class Grader;

template <class T>
class HashTable {

  friend Grader;
  
 public:
  // Constructor.  Requires table size and hash function.
  HashTable(unsigned size, hash_fn hash);

  // Destructor, copy, and assignment
  ~HashTable();
  HashTable(const HashTable<T>& ht);
  const HashTable<T>& operator=(const HashTable<T>& ht);

  // In-line functions
  
  // Table size; set by constructor
  unsigned tableSize() const { return _N; }
  // Number of entries in the table
  unsigned numEntries() const { return _n; }
  // Load factor
  float lambda() const { return ((float) _n) / _N; }

  // Main hash table functions
  
  // insert returns 'true' if successful; 'false' otherwise
  bool insert(const T& object);
  // getNext retrieves **and removes** the highest priority order of
  // type indicated by key.  It returns 'true' if successful; 'false'
  // otherwise.
  bool getNext(string key, T& obj);

  // Dump the entire hash table
  void dump() const;
  
 private:
  unsigned _N;       // hash table size
  unsigned _n;       // current number of entries
  hash_fn _hash;     // hash function
  Heap<T> *_table;   // array of heaps

  // ***********************************************
  // Private helper function declarations go here! *
  // ***********************************************
  
};

// Constructor for hash table, initializes variables including size and the table itself
template <class T>
HashTable<T>::HashTable(unsigned size, hash_fn hash) {
  _N = size;
  _hash = hash;
  _n = 0;
  _table = nullptr;
  _table = new Heap<T>[_N];
}

// Destructor for hash table, deletes the actual table
template <class T>
HashTable<T>::~HashTable() {
  // Don't bother deleting hash table if there's nothing to delete
  if (_n != 0) {
    delete[] _table;
  }
}

// Copy constructor for the hash table, making this a copy of the hash table that was passed as a parameter
template <class T>
HashTable<T>::HashTable(const HashTable<T>& ht) {
  _N = ht._N;
  _hash = ht._hash;
  _n = ht._n;
  _table = nullptr;
  _table = new Heap<T>[_N];

  for (unsigned int i = 0; i < _N; i++) {
    _table[i] = ht._table[i];
  }
}

// Overloaded assignment operator, clears this hash table and then makes it a copy of the hash table
// that was passed as a parameter
template <class T>
const HashTable<T>& HashTable<T>::operator=(const HashTable<T>& ht) {
  // Delete the current hash table
  if (_n != 0) {
    delete[] _table;
  }

  // Make this a copy of the passed hash table
  _N = ht._N;
  _hash = ht.hash;
  _n = ht._n;
  _table = nullptr;
  _table = new Heap<T>[_N];

  for (unsigned int i = 0; i < _N; i++) {
    _table[i] = ht._table[i];
  }

  return *this;
}

// Inserts a new object into the hash table, uses the hash function to find where to put it
template <class T>
bool HashTable<T>::insert(const T& object) {
  // If table is full, can't insert.
  if (_n == _N) {
    return false;
  }

  // Keep the hash value of the key, keep track if you inserted
  unsigned temp = _hash(object.key());
  unsigned place = temp % _N;
  unsigned original = place;
  bool inserted = false;

  // As long as you haven't reached the end, and you haven't inserted
  while ((place < _N) && !inserted) {
    // If the key has been used
    if (_table[place].used()) {
      // If the spot is not empty but the keys match, insert there
      if (!(_table[place].empty()) && (_table[place].readTop().key() == object.key())) {
	_table[place].insert(object);
	inserted = true;
      }
    } else {
      // If the spot isn't used, insert there
      _table[place].insert(object);
      _n++;
      inserted = true;
    }

    // Increment index
    place++;

    // Loop around the table if needed
    if (place >= _N) {
      place = 0;
    }
  }

  // If the first spot checked is used but is empty and you haven't inserted yet, insert there
  if ((_table[original].used()) && (_table[original].empty()) && !inserted) {
    _table[original].insert(object);
  }
  
  return inserted;
}

// Reads and removes the highest priority object at that key, returns the object through the reference
// parameter passed to it
template <class T>
bool HashTable<T>::getNext(string key, T& obj) {
  // If nothing has been entered, you can't get anything
  if (_n == 0) {
    return false;
  }

  // Keep track of the hash value and whether or not you have found it
  unsigned temp = _hash(key);
  unsigned place = temp % _N;
  bool found = false;
  int counter = 0;

  // As long as you haven't checked each element and it hasn't been found yet
  while ((counter < _N) && !found) {

    // If the spot has been used
    if (_table[place].used()) {
      // If the spot isn't empty and the keys match, retrieve the top element and remove it.
      if (!(_table[place].empty()) && (_table[place].readTop().key() == key)) {
	obj = _table[place].readTop();
	_table[place].removeTop();
	found = true;
      }
    }
    // Incremenet the index and the counter
    place++;
    counter++;

    // Loop the index to the start of the hash table if need be
    if (place >= _N) {
      place = 0;
    }
  }

  return found;
}

// Dumps the contents of the hash table by iterating through each slot in the table and
// calling the dump function of each of the heaps
template <class T>
void HashTable<T>::dump() const {
  for (unsigned int i = 0; i < _N; i++) {
    cout << "[" << i << "]" << endl;
    _table[i].dump();
  }
}

// *****************************************
// Templated function definitions go here! *
// *****************************************

#endif
