#pragma once

#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdint.h>

template<typename T, uint8_t N>
struct List
{
private:
  T array[N];
  uint8_t next;

public: 
  // Returns the number of Ts currently in the list
  uint8_t get_size(void) const { return next; }
  
  // Returns the maximum number of Ts the list can hold
  uint8_t getCapacity(void) const { return N; }
  
  // Returns true if the list is full
  bool isFull(void) const { return get_size() == getCapacity(); }
  
  // Returns true if the list is empty
  bool isEmpty(void) const { return get_size() == 0; }
  
  // Clears the list (by cheating)
  void clear_list(void) { next = 0; }
  
  // Returns true if the T was added
  // Returns false if the list is already full
  bool add(const T & item)
  {
    if(this->isFull())
      return false;
    
    array[next] = item; // put the T in the array
    ++next; // increment the next index
    return true;
  }
  
  // Returns true if the T was removed
  // Returns false if the index was invalid
  bool removeAt(uint8_t index)
  {
    if(index >= next)
      return false;
    
    --next; // decrement next index
    for(size_t i = index; i < next; ++i) // shuffle everything down
      array[i] = array[i + 1];
    return true;
  }
  
  // These are for indexing the list
  // Be careful, these don't check if the index is valid
  T & operator[](uint8_t index) { return array[index]; }  
  const T & operator[](uint8_t index) const { return array[index]; }
};

#endif
