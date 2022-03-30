// CMSC 341 - Fall 2019 - Project 3
// File: treap.cpp

using namespace std ;

#include "treap.h"

Treap::Treap() {
  _nptr = nullptr ;
}

// Initializes nptr, then makes this treap a copy of the passed treap
Treap::Treap(const Treap& other) {
  _nptr = nullptr;
  copyHelper(other);
}

// If the tree isn't empty, deletes it
Treap::~Treap() {
  if (!empty()) {
    delete _nptr;
    _nptr = nullptr;
  }
}

// Deletes this treap, then assigns it to be equal to the passed treap
const Treap& Treap::operator=(const Treap& rhs) {
  if (!empty()) {
    delete _nptr;
    _nptr = nullptr;
  }
  copyHelper(rhs);
  return *this;
}

// Helper function to make this treap a deep copy of the passed treap
void Treap::copyHelper(const Treap& other) {
  if (other.empty()) {
    return;
  }
  // Follow inorder traversal to insert everything in the other treap into this one
  copyHelper(other._nptr->_left);
  
  insert(other._nptr->_data, other._nptr->_pri);
  
  copyHelper(other._nptr->_right);
}

// Return -1 if the treap is empty; otherwise, return value in _height.
int Treap::height() const {
  return _nptr ? _nptr->_height : -1 ;
}

// Return the priority of the root node of a Treap.
// Note: numeric_limits<priority_t>::min() returns the minimum value
// of the priority type.  We will only use int priorities, so this is
// the minimum integer value.
priority_t Treap::priority() const {
  return _nptr ? _nptr->_pri : numeric_limits<priority_t>::min() ;
}

const data_t* Treap::find(const data_t& x) {
  // If treap is empty, the data can't be in it
  if (empty()) {
    return nullptr;
  }

  // Use the BST property to search through the treap until you find the data
  // or find that it's not in the treap (reaches the base case)
  if (x < _nptr->_data) {
    return(_nptr->_left.find(x));
  } else if (x > _nptr->_data) {
    return(_nptr->_right.find(x));
  } else {
    // If x is equal to the data in _nptr, return the address of the data
    return &(_nptr->_data);
  }
}

// Inorder traversal.  Recommend you NOT change this!!!
void Treap::inorder() {

  if ( empty() ) return ;

  cout << "(" ;
  _nptr->_left.inorder() ;

  cout << _nptr->_data << ":" << priority() << ":" << height() ;

  _nptr->_right.inorder() ;
  cout << ")" ;

  return  ;
}

// Used for debugging and grading.  Interprets the characters in
// position[] as directions in the tree.  E.g., "RLR" means, "starting
// from root, go right, then left, then right."  If position[] is an
// empty string, then it just locats the value a the root.
// See project description for details.
bool Treap::locate (const char position[], data_t& x, priority_t& p, int& h, int offset) {

  if (_nptr == nullptr) return false ;

  if (position[offset] == '\0') {
    x = _nptr->_data ;
    p = _nptr->_pri ;
    h = _nptr->_height ;
    return true ;
  }

  if (position[offset] == 'L') {
    return _nptr->_left.locate(position, x, p, h, offset+1) ;
  }

  if (position[offset] == 'R') {
    return _nptr->_right.locate(position, x, p, h, offset+1) ;
  }

  cerr << "Bad position character!\n" ;
  exit(17) ;

  return false ; 
}


// Basic BST insertion.  Does not allow duplicate values.
//
// Complete this function to do Treap insertion.
//
void Treap::insert(const data_t& x, const priority_t& p) {
  if ( empty() ) {
    _nptr = new TreapNode(x, p) ;
  }

  // If the data you're looking for is larger than the current data,
  // recurse to the right.

  if (_nptr->_data < x) { 
    _nptr->_right.insert(x, p) ;

    // If the priority of this node's child is greater, rotate
    if (_nptr->_pri < _nptr->_right._nptr->_pri) {
      TreapNode *temp = _nptr;
      TreapNode *switchChild = _nptr->_right._nptr->_left._nptr;
      _nptr = _nptr->_right._nptr;
      _nptr->_left._nptr = temp;
      _nptr->_left._nptr->_right._nptr = switchChild;

      //Update height of node that was rotated down
      int lcHeight = _nptr->_left._nptr->_left.height();
      int rcHeight = _nptr->_left._nptr->_right.height();
      _nptr->_left._nptr->_height = 1 + (lcHeight > rcHeight ? lcHeight : rcHeight);

      temp = nullptr;
      switchChild = nullptr;
    }

    // If the data you're looking for is smaller than the current data,
    // recurse to the left.
  } else if (x < _nptr->_data ) {
    _nptr->_left.insert(x, p) ;

    // If te priority of this node's child is smaller, rotate
    if (_nptr->_pri < _nptr->_left._nptr->_pri) {
      TreapNode *temp = _nptr;
      TreapNode *switchChild = _nptr->_left._nptr->_right._nptr;
      _nptr = _nptr->_left._nptr;
      _nptr->_right._nptr = temp;
      _nptr->_right._nptr->_left._nptr = switchChild;

      //Update height of node that was rotated down
      int lcHeight = _nptr->_right._nptr->_left.height();
      int rcHeight = _nptr->_right._nptr->_right.height();
      _nptr->_right._nptr->_height = 1 + (lcHeight > rcHeight ? lcHeight : rcHeight);

      temp = nullptr;
      switchChild = nullptr;
    }
  }

  // Update height. Maybe this should be a helper function??
  int leftHeight = _nptr->_left.height();
  int rightHeight = _nptr->_right.height();
  _nptr->_height = 1 + ( leftHeight > rightHeight ? leftHeight : rightHeight );

}

bool Treap::remove(const data_t& x) {

  // If the data is not in the tree, you can't remove it
  if (find(x) == nullptr) {
    return false;
  }
  
  if (x == _nptr->_data) {
    
    if ((_nptr->_left._nptr == nullptr) && (_nptr->_right._nptr == nullptr)) {
      // If the node has no non-null children, it's a leaf, so just delete it
      delete _nptr;
      _nptr = nullptr;
      
    } else if ((_nptr->_left._nptr != nullptr) && (_nptr->_right._nptr == nullptr)) {
      // If the node has a left child with data, but a NULL right child
      TreapNode *switchChild = _nptr->_left._nptr->_right._nptr;
      TreapNode *temp = _nptr;
      _nptr = _nptr->_left._nptr;
      _nptr->_right._nptr = temp;
      _nptr->_right._nptr->_left._nptr = switchChild;
      
      _nptr->_right.remove(x);
      int leftHeight = _nptr->_left.height();
      int rightHeight = _nptr->_right.height();
      _nptr->_height = 1 + ( leftHeight > rightHeight ? leftHeight : rightHeight );
      
      
    } else if ((_nptr->_right._nptr != nullptr) && (_nptr->_left._nptr == nullptr)) {
      // If the node has a right child with data, but a NULL left child
      TreapNode *temp = _nptr;
      TreapNode *switchChild = _nptr->_right._nptr->_left._nptr;
      _nptr = _nptr->_right._nptr;
      _nptr->_left._nptr = temp;
      _nptr->_left._nptr->_right._nptr = switchChild;
    
      _nptr->_left.remove(x);
      int leftHeight = _nptr->_left.height();
      int rightHeight = _nptr->_right.height();
      _nptr->_height = 1 + ( leftHeight > rightHeight ? leftHeight : rightHeight );
      
    } else if ((_nptr->_left._nptr != nullptr) && (_nptr->_right._nptr != nullptr)) {
      // If the node has two non-null children
      if (_nptr->_left._nptr->_pri > _nptr->_right._nptr->_pri) {
	// Left child priority is greater, so rotate same way as above when only left child exists
	TreapNode *temp = _nptr;
	TreapNode *switchChild = _nptr->_left._nptr->_right._nptr;
	_nptr = _nptr->_left._nptr;
	_nptr->_right._nptr = temp;
	_nptr->_right._nptr->_left._nptr = switchChild;

	_nptr->_right.remove(x);
	int leftHeight = _nptr->_left.height();
	int rightHeight = _nptr->_right.height();
	_nptr->_height = 1 + ( leftHeight > rightHeight ? leftHeight : rightHeight );
      } else if (_nptr->_left._nptr->_pri < _nptr->_right._nptr->_pri) {
	// Right child priority is greater, so rotate same way as above when only right child exists
	TreapNode *temp = _nptr;
	TreapNode *switchChild = _nptr->_right._nptr->_left._nptr;
	_nptr = _nptr->_right._nptr;
	_nptr->_left._nptr = temp;
	_nptr->_left._nptr->_right._nptr = switchChild;
	
	_nptr->_left.remove(x);
	int leftHeight = _nptr->_left.height();
	int rightHeight = _nptr->_right.height();
	_nptr->_height = 1 + ( leftHeight > rightHeight ? leftHeight : rightHeight );
      }
    }
    // If this is not the data you're looking for, either recurse left or right, depending
    // on which of the data values are greater (according to the BST logic)
  } else if (x <_nptr->_data) {
    _nptr->_left.remove(x);
  } else if (x >_nptr->_data) {
    _nptr->_right.remove(x);
  }

  // Update the height if not on a NULL nptr
  if (_nptr != nullptr) {
    int leftHeight = _nptr->_left.height();
    int rightHeight = _nptr->_right.height();
    _nptr->_height = 1 + ( leftHeight > rightHeight ? leftHeight : rightHeight );
  }
  return true;

}


// Treap::dump() just calls TreapNode::dump()
void Treap::dump() {
  if ( !empty() ) _nptr->dump() ;
}

ostream& operator<< (ostream& os, const Treap& T) {
  os << T._nptr ; 
  return os ;
}


TreapNode::TreapNode() : _height(0) {
  // Empty constructor body
}

TreapNode::TreapNode(const data_t& x, const priority_t& p) 
  : _data(x), _pri(p), _height(0) { 
  // Empty constructor body
}

TreapNode::~TreapNode() {
  // Empty destructor
  // Retained for debugging
}

// Dump the contents of a node
void TreapNode::dump() {
  cout << "===== TreapNode::dump()\n" ;
  cout << "      this = " << this << endl ; 
  cout << "      left = " << _left << endl ; 
  cout << "      right= " << _right << endl ; 
  cout << "      data = " << _data << endl ;
  cout << "      height = " << _height << endl ;
  _left.dump() ;
  _right.dump() ;
}

