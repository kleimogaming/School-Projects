#include <iostream>
#include "heap.h"
#include "donut.h"

int main(){

  vector<Donut> donuts;
  donuts.push_back(Donut("glazed", 5, "super donut"));
  donuts.push_back(Donut("glazed", 12, "mega donut"));
  donuts.push_back(Donut("glazed", 4, "shack of donuts"));
  donuts.push_back(Donut("boston cream", 10, "mega donut"));
  donuts.push_back(Donut("boston cream", 5, "donut world"));
  donuts.push_back(Donut("sprinkle", 7, "donut world"));
  donuts.push_back(Donut("sprinkle", 15, "super donut"));
  donuts.push_back(Donut("sprinkle", 8, "shack of donuts"));
  donuts.push_back(Donut("boston cream", 3, "donut joint"));
  donuts.push_back(Donut("cinnamon", 3, "shack of donuts"));
  donuts.push_back(Donut("bavarian cream", 28, "mega donut"));
  donuts.push_back(Donut("cinnamon", 11, "super donut"));

  Heap<Donut> heap;
  for(auto d: donuts){
    heap.insert(d);
  }

  heap.dump();
  cout << endl;

  heap.removeTop();
  heap.removeTop();
  heap.removeTop();
  heap.removeTop();

  // cout << endl << heap.readTop() << endl << endl;
  heap.dump();

  return 0;
}
