#ifndef MYARRAY_H
#define MYARRAY_H

using namespace std;

class my_array {

public:
  //Default constructor
  my_array( unsigned int size = 1 );

  // Non-default constructor copies data from array to my_array  
  my_array( int* data, int len );

  // Copy constructor
  my_array(const my_array& arr);

  // Destructor
  ~my_array();

  // Assignment operator
  my_array& operator=(const my_array& arr);
  
  // Return number of items in my_array
  int size() const;

  // Get element at indx position
  int& at( unsigned int indx );

private:
  unsigned int m_size;  // size of my array
  int *m_data;          // storage array
};

#endif
