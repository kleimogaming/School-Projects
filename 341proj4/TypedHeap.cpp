// cpp for TypedHeap

#include "TypedHeap.h"

//Insert the strings into the corresponding heaps
void TypedHeap::insertToHeaps(vector<string> vec) {

  //For each string in the vector provided
  for (int i = 0; i < vec.size(); i++) {
    //Marker for whether the string can be an integer
    bool checker = false;
    //For each character in the selected string
    for (int j = 0; j < vec[i].size(); j++) {
      //See if the character is an integer
      if (!isdigit(vec[i][j])) {
	checker = true;
      }
    }
    //Insert into the correct heap
    if (checker) {
      stringHeap.insert(vec[i]);
    } else {
      intHeap.insert(stoi(vec[i]));
    }
  }
}

//Merge the integer and string heaps into the total heap
void TypedHeap::combineHeaps() {
  intHeap.skewHeapMerge(stringHeap);
  totalHeap.skewHeapMerge(intHeap);
}

//Dump the contents of all three heaps
void TypedHeap::printHeaps() const {
  intHeap.dump();
  stringHeap.dump();
  totalHeap.dump();
}

//Change the priority of each of the heaps to the given priority function
void TypedHeap::changePriority(pri_fn pri) {
  intHeap.setPriFn(pri);
  stringHeap.setPriFn(pri);
  totalHeap.setPriFn(pri);
}
