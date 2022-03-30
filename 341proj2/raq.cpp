// Nicholas Klein
// CMSC 341 Section 01
// Project 2 raq.cpp

#include "raq.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

// Constructor for RAQ
RAQ::RAQ(std::vector<float> data) {
  float sum = 0.0;
  
  for (int i = 0; i < data.size(); i++) {

    for (int j = i; j < data.size(); j++) {
      // If taking the sum of one number, just input the data.
      if (i == j) {
	sum = data[j];
	m_storage.push_back(sum);
	// Otherwise add the current data to the previous sum
      } else {
	sum = data[j] + sum;
	m_storage.push_back(sum);
      }
      
    }

    // Put the new vector onto the 2D vector sum table
    // Clear the current temporary storage
    m_sumTable.push_back(m_storage);
    m_storage.clear();
  }
}

// Query the average between two elements of the data.
float RAQ::query(int i, int j) const {
  // Throw errors if ranges are not acceptable
  if ((j < i) || (i < 0) || (i > m_data.size()) || (j > m_data.size())) {
    throw std::domain_error("Given ranges are not acceptable.");
  }

  // Get the true index of j, since further elements of i will have less elements of j.
  int actualIndex = j - i;

  // Get the corresponding sum from the table and take the average of it.
  float sum = m_sumTable[i][actualIndex];
  float average = sum / (j - i + 1);
  return average;
}

// Dump the contents of the 2D table, showing the sums of every possible pair.
void RAQ::dump() const {
  for (int i = 0; i < m_sumTable.size(); i++) {

    for (int j = 0; j < m_sumTable[i].size(); j++) {

      // Get the true index since further elements in have a smaller number of elements
      int actualIndex = i + j;
      std::cout << "Sum of (" << i << "," << actualIndex << "): " << m_sumTable[i][j];

      // If end of current i, start a new line, otherwise tab over for formatting.
      if (j == m_sumTable[i].size() - 1) {
	std::cout << std::endl;
      } else {
	std::cout << "\t";
      }
    }
  }
}

// Constructor to BlockRAQ
BlockRAQ::BlockRAQ(std::vector<float> data) {
  // Store the passed data into a class vector to keep it.
  for (int i = 0; i < data.size(); i++) {
    m_data.push_back(data[i]);
  }

  // Blocks should be root of n in size
  m_blockSize = (int) sqrt((float) m_data.size());

  // Throw error if passed an empty array, so that you don't divide by 0.
  if (m_blockSize == 0) {
    throw std::domain_error("Size of data is 0, can't perform averages of nothing.");
  }

  // The remainder from the blocks until the end of data is the last special block.
  m_specialSize = (m_data.size() % m_blockSize);

  // If no remainder, make it same size as a regular block
  if (m_specialSize == 0) {
    m_specialSize = m_blockSize;
  }

  // Keep track of block summations.
  int blockCounter = 0;
  m_blocks.push_back(0);

  // Put the sum in each group of data into an element of the block vector.
  for (int i = 0; i < m_data.size(); i++) {
    if ((i != 0) && (i % m_blockSize == 0)) {
      m_blocks.push_back(0);
      blockCounter++;
    }
    m_blocks[blockCounter] += m_data[i];
  }
}

// Query to find the average in a range, using the block structure to assist.
float BlockRAQ::query(int i, int j) const {
  // Throw error is ranges are not acceptable
  if ((j < i) || (i < 0) || (i > m_data.size()) || (j > m_data.size())) {
    throw std::domain_error("Given ranges are not acceptable.");
  }

  // Initialize variables to keep track of how far we are in blocks / data
  float average = 0.0;
  float dataCounter = 0;
  int blockCounter = 0;

  // While still in the range, add the total sum to the tracker variable.
  do {
    // If at the start of the block and you go through the entire block.
    if ((i % m_blockSize == 0) && (i + m_blockSize <= j)) {
      blockCounter = i / m_blockSize;
      average += m_blocks[blockCounter];
      dataCounter += m_blockSize;
      i += m_blockSize;
      // else if at the last special block and you go until the end of data.
    } else if ((i == (m_data.size() - m_specialSize)) && (j == m_data.size() - 1)) {
      blockCounter = m_blocks.size() - 1;
      average += m_blocks[blockCounter];
      dataCounter += m_specialSize;
      i = j + 1;
      // otherwise get individual data (outside of the start of the block or not going until end.
    } else {
      average += m_data[i];
      dataCounter++;
      i++;
    }
  } while (i <= j);

  // Calculate the average based on how many data slots
  average = average / dataCounter;

  return average;
}

// Dump the contents of the blocks, including block number, size, and sum
void BlockRAQ::dump() const {
  std::cout << "Number of blocks: " << m_blocks.size() << std::endl;
  for (int i = 0; i < m_blocks.size(); i++) {
    if (i < m_blocks.size() - 1) {
      std::cout << "Block " << i << "- " << "Block Size: " << m_blockSize << std::endl;
      std::cout << "Block " << i << " Sum: " << m_blocks[i] << std::endl;
    } else if (i == m_blocks.size() - 1) {
      // If last block, use special size.
      std::cout << "Block " << i << "- " << "Block Size: " << m_specialSize << std::endl;
      std::cout << "Block " << i << " Sum: " << m_blocks[i] << std::endl;
    }
  }
}
