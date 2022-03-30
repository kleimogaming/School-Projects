//
// test0.cpp : simple test driver for the my_array class
//
// Compile with:
//    g++ test0.cpp my_array.cpp -o test0
//

#include <iostream>
#include "my_array.h"

using namespace std;

int main() {

  const int ARRAY_SIZE = 20;
  
  // Create an array 
  my_array x(ARRAY_SIZE);

  // Write some values to the array
  for (int i = 1; i <= ARRAY_SIZE; i++ ) {
    x.at(i-1) = (i*i) % 19;
  }

  // Print the values in the array
  cout << "my_array x:" << endl;
  for (int i = 0; i < ARRAY_SIZE; i++) {
    cout << x.at(i) << " ";
  }
  cout << endl << endl;

  // This code should result in a range_error exception
  cout << "Using an index that is too large causes an "
       << "exception to be thrown." << endl;
  try {
    x.at(ARRAY_SIZE) = -1;
  } catch (exception& e) {
    cerr << "Error: " << e.what() << endl;
  }
  
  return 0;
}
