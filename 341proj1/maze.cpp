#include <stdexcept>
#include "maze.h"
#include <vector>

// Nicholas Klein
// CMSC 341 Section 01
// Project 1 maze.cpp

Maze::Maze(int ncell) {
  if (ncell < 1) {
    throw std::invalid_argument("Maze::Maze(): ncell must be >= 1");
  }

  _ncell = ncell;
  _maze = new cell_t[_ncell];
}

Maze::~Maze() {
  // As long as maze has cells, delete them all.
  if (_maze != nullptr) {
    delete[] _maze;
  }
}

void Maze::readFile(std::string fileName) {
  int numCells;
  int cell, n1, n2, n3, n4; 
  std::ifstream dataFile;
  
  dataFile.open(fileName);

  if (dataFile.is_open()) {
    dataFile >> numCells;
    this->reset(numCells);

    for (int i = 0; i < numCells; i++) {
      dataFile >> cell >> n1 >> n2 >> n3 >> n4;
      this->insert(cell_t(cell, n1, n2, n3, n4));
    }
  }
}

int Maze::getNcell() const {
  return _ncell;
}

void Maze::reset(int ncell) {
  if (ncell < 1) {
    throw std::invalid_argument("Maze::reset(): ncell must be >= 1");
  }

  if (_maze != nullptr) {
    delete [] _maze;
  }
  
  _ncell = ncell;
  _maze = new cell_t[_ncell];
}

void Maze::insert(cell_t cell) {
  if (_maze == nullptr) {
    throw std::domain_error("Maze::insert(): attempt to insert into uninitialized Maze object");
  }
  if (cell.cellNum < 0 || cell.cellNum >= _ncell) {
    throw std::invalid_argument("Maze:insert(): invalid cell number");
  }
  _maze[cell.cellNum] = cell;
}
  
cell_t Maze::retrieve(int nc) const {
  if (_maze == nullptr) {
    throw std::domain_error("Maze::retrieve(): attempt to retrieve from an uninitialized Maze object");
  }
  if (nc < 0 || nc >= _ncell) {
    throw std::invalid_argument("Maze:retrieve(): invalid cell index");
  }
  
  return _maze[nc];
}

void Maze::dump() const {

  std::cout << "[*] Dumping the maze cells..." << std::endl;
  
  for (int i = 0; i < _ncell; i++) {
    int nc = _maze[i].cellNum;
    if (nc != i) {
      std::cerr << "Warning: maze cell at index " << i
		<<" has cellNum of " << nc << std::endl;
    }
    std::cout << "Cell " << i << " has neighbor cells: ";
    neighbor_t nbs = _maze[i].neighbors;
    int n0, n1, n2, n3;
    if ( nbs[0] >= 0 ) std::cout << nbs[0] << " ";
    if ( nbs[1] >= 0 ) std::cout << nbs[1] << " ";
    if ( nbs[2] >= 0 ) std::cout << nbs[2] << " ";
    if ( nbs[3] >= 0 ) std::cout << nbs[3] << " ";
    std::cout << std::endl;
  }
}

std::vector<int> Maze::solve() const {
  // Initialize variables needed, including an instance of SolveStack, and
  // a copy of maze array that can be edited.
  std::cout << "[*] Start of call to solve()..." << std::endl;
  std::vector<int> path;
  SolveStack stack;
  cell_t *maze = new cell_t[_ncell];
  int next = VACANT;

  // Temporary integer pair to use while going through the maze.
  src_dest_t changePair;
  changePair.first = VACANT;
  changePair.second = 0;

  // Copy the maze.
  for (int i = 0; i < _ncell; i++) {
    maze[i] = _maze[i];
  }

  // If maze is empty, throw an error.
  if (maze == nullptr) {
    throw std::domain_error("Maze::solve(): attempt to retrieve from an uninitialized Maze object");
  }

  // Push the first cell (-1, 0) onto the stack, add it to the clear path.
  path.push_back(changePair.second);
  stack.push(changePair);
  int curr = stack.read().second;

  // As long as the maze hasn't been solved and the stack isn't empty (when it can't be solved).
  while((!stack.empty()) && (stack.read().second < (getNcell() - 1))) {
    curr = stack.read().second;
    next = VACANT;

    // Look for the last neighbor of current cell that isn't empty and doesn't go back to a previous cell.
    for (int i = 0; i < 4; i++) {
      if ((maze[curr].neighbors[i] != VACANT) && (maze[curr].neighbors[i] != stack.read().first)) {
	next = maze[curr].neighbors[i];
      }
    }

    // If no neighbors are available, backtrack.
    if (next == VACANT) {
      path.pop_back();
      stack.pop();
    } else {
      // Found a neighbor, so remove it from the list in case we need to backtrack later.
      for (int i = 0; i < 4; i++) {
	if (maze[curr].neighbors[i] == next) {
	  maze[curr].neighbors[i] = VACANT;
	}
      }
      // Edit the temp pair so we can keep track of where we are.
      // Push the pair onto the stack and the path to keep track of our way out of the maze.
      changePair.first = curr;
      changePair.second = next;
      stack.push(changePair);
      path.push_back(changePair.second);
    }
  }

  // Done with the maze, so delete it to get rid of memory leaks, and no seg faults.
  delete[] maze;

  // If the path isn't empty, we found the solution, otherwise, there is no solution.
  if (path.size() != 0) {
    std::cout << "[+] Solution found!" << std::endl;
  } else {
    std::cout << "[-] No solution found." << std::endl;
  }

  // Return the path out of the maze (if there isn't one, it's an empty vector).
  return path;
}

Maze::SolveStack::SolveStack() {
  _stack = nullptr;
}

Maze::SolveStack::~SolveStack() {
  // Until stack is empty, travel down the stack and pop each element off.
  while (_stack != nullptr) {
    src_dest_t tempPair = _stack -> src_dest;
    int prev = tempPair.first;
    
    // If at bottom of stack, set the stack pointer to NULL.
    if (prev == VACANT) {
      pop();
      _stack = nullptr;
    } else {
      pop();
    }
  }
}

bool Maze::SolveStack::empty() const {
  return _stack == nullptr;
}
				   
void Maze::SolveStack::push(src_dest_t src_dest) {
  // Adds a new element on top of the stack, sets it to point to the one below.
  entry_t *temp = new entry_t(src_dest);
  temp -> next = _stack;
  _stack = temp; 
}

src_dest_t Maze::SolveStack::pop() {
  if (_stack == nullptr) {
    throw std::domain_error("Maze::pop(): attempt to retrieve from an empty Stack object");
  }
  // Removes the top element, moves the stack pointer to the one below
  src_dest_t tempPair = _stack -> src_dest;
  entry_t *tempNext = _stack -> next;
  delete _stack;
  _stack = tempNext;
  return tempPair;
}

src_dest_t Maze::SolveStack::read() const {
  // If stack isn't empty, returns the values in the top element of the stack.
  if (_stack == nullptr) {
    throw std::domain_error("Maze::read(): attempt to retrieve from an empty Stack object");
  }
  return (_stack -> src_dest);
}
