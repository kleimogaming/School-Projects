// cpp file for SkewHeap

#include "SkewHeap.h"

//Constructor for SkewHeap, sets the priority
SkewHeap::SkewHeap(pri_fn pri) {
  priority = pri;
  m_heap = nullptr;
}

//Destructor, removes the top node until the heap is empty
SkewHeap::~SkewHeap(){
  while (!empty()) {
    removeTop();
  }
  m_heap = nullptr;
}

//Copy Constructor, makes this heap a copy of rhs
SkewHeap::SkewHeap(const SkewHeap& rhs) {
  priority = rhs.priority;
  m_heap = copyHelp(rhs.m_heap);
}

//Overloaded assignment operator, sets this heap equal to rhs
const SkewHeap& SkewHeap::operator=(const SkewHeap& rhs) {
  //Deletes the current heap
  while (!empty()) {
    removeTop();
  }
  m_heap = nullptr;

  //Makes this heap a copy of rhs
  priority = rhs.priority;
  m_heap = copyHelp(rhs.m_heap);

  //Return the new heap
  return *this;
}

//Helper function for copy constructor and assignment operator
Node* SkewHeap::copyHelp(Node* copyThis) {
  if (copyThis == nullptr) {
    return nullptr;
  }

  //Depending on if the data is a string or int, make a corresponding node
  Node *thisNode = nullptr;
  if (copyThis -> tagged_union == ISINT) {
    thisNode = new Node(copyThis -> data_int);
  } else {
    thisNode = new Node(copyThis -> data_string);
  }

  //Recurse for the children
  thisNode -> left = copyHelp(copyThis -> left);
  thisNode -> right = copyHelp(copyThis -> right);

  return thisNode;
}

//Return the priority function
pri_fn SkewHeap::getPriFn() const {
  return priority;
}

//Resets the priority function to a new one
void SkewHeap::setPriFn(pri_fn pri) {
  priority = pri;

  if (m_heap == nullptr) {
    return;
  }

  SkewHeap tempHeap(pri);

  Node *temp = m_heap;

  //As long as the heap isn't empty, insert the data as a new node into
  //the new heap.
  while (temp != nullptr) {
    if (temp -> tagged_union == ISINT) {
      tempHeap.insert(temp -> data_int);
    } else {
      tempHeap.insert(temp -> data_string);
    }
    //Remove from the old heap to get the next node
    removeTop();
    temp = m_heap;
  }

  //Copy the temp heap to this one
  m_heap = nullptr;
  m_heap = copyHelp(tempHeap.m_heap);
}

//Returns if m_heap is nullptr
bool SkewHeap::empty() const {
  if (m_heap == nullptr) {
    return true;
  } else {
    return false;
  }
}

//Insert the string data
void SkewHeap::insert(string data) {
  Node *tempNew = new Node(data);
  Node *tempOld = m_heap;
  m_heap = mergeHelp(tempOld, tempNew);
}

//Insert the integer data
void SkewHeap::insert(int data) {
  Node *tempNew = new Node(data);
  Node *tempOld = m_heap;
  m_heap = mergeHelp(tempOld, tempNew);

}

//Returns m_heap, the top of the heap
Node* SkewHeap::front() const {
  return m_heap;
}

//Removes the top element 
void SkewHeap::removeTop() {
  Node *leftNode = m_heap -> left;
  Node *rightNode = m_heap -> right;

  //Clear the current heap
  delete m_heap;
  m_heap = nullptr;

  //Merge the children into this node
  m_heap = mergeHelp(leftNode, rightNode);
}

//Merges this heap with sh
void SkewHeap::skewHeapMerge(SkewHeap& sh) {
  //If they are the same heap. don't do anything
  if (m_heap == sh.m_heap) {
    return;
  }

  pri_fn temp = sh.priority;

  //If priorities are different, can't merge
  if (temp != priority) {
    throw domain_error("Priorities are not the same!");
  }

  //Merge the old and new heaps
  m_heap = mergeHelp(m_heap, sh.m_heap);
  sh.m_heap = nullptr;
}

//Helper function for merging two heaps
Node* SkewHeap::mergeHelp(Node* greater, Node* lesser) {
  //If one heap is empty, just, return the other
  if (greater == nullptr) {
    return lesser;
  } else if (lesser == nullptr) {
    return greater;
  }

  //If priorities are in wrong order, swap them
  if (priority(greater) < priority(lesser)) {
    swap(greater, lesser);
  }

  //Swap two nodes
  swap(greater -> left, greater -> right);

  //Set next merged heap to the left child
  greater -> left = mergeHelp(greater -> left, lesser);

  return greater;
}

//Prints out elements inorder
void SkewHeap::inorder() const {
  inorderHelp(m_heap);
}

//Helper function for inorder traversal
void SkewHeap::inorderHelp(Node *printThis) const {
  if (printThis == nullptr) {
    return;
  }

  cout << "(";

  //Recursive call for left child
  inorderHelp(printThis->left);

  //Print the data, depending on if it's an integer or string
  if (printThis -> tagged_union == ISINT) {
    cout << printThis -> data_int;
  } else {
    cout << printThis -> data_string;
  }

  //Recursive call for right child
  inorderHelp(printThis -> right);

  cout << ")";
}

//Prints the data and priority of each node in the heap
void SkewHeap::dump() const {
  //Create a copy of the heap to specifically dump
  SkewHeap dumpThis(*this);

  //While the copy heap still has nodes, print the data, then the priority
  while (!dumpThis.empty()) {
    cout << "Data is:" << setw(8);
    if (dumpThis.m_heap -> tagged_union == ISINT) {
      cout << dumpThis.m_heap -> data_int;
    } else {
      cout << dumpThis.m_heap -> data_string;
    }
    cout << setw(25) << "Priority is:" << setw(8) << priority(dumpThis.m_heap) << endl;

    //Remove the node after printing its contents
    dumpThis.removeTop();
  }
}
