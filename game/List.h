#pragma once

#include <stddef.h>
#include <stdint.h>

template<typename T, size_t size>
class List
{
private:
  uint8_t next;
  T* arr;
public:
  using ItemType = T;
  using ItemTypeRef = T&;
  using ConstItemTypeRef = const T&;
  using IndexType = uint8_t;
  static constexpr size_t _size = size;
  static constexpr IndexType firstIndex = 0;
  static constexpr IndexType lastIndex = _size - 1;

  List() : next{0}
  {
    arr = new ItemType[_size]{};
  }
  
  // Returns the number of Ts currently in the list
  uint8_t get_size(void) const
  {
    return next;
  }

  // Returns the maximum number of Ts the list can hold
  uint8_t getCapacity(void) const 
  { 
    return _size; 
  }

  // Returns true if the list is full
  bool isFull()
  {
    return (getCapacity() == get_size());
  }

  // Returns true if the list is empty
  bool isEmpty()
  {
    return (getCapacity() == firstIndex);
  }

  // Clears the list (by cheating)
  void clear_list(void)
  {
    next = 0;
  }

  // Returns true if the T was added
  // Returns false if the list is already full
  bool add(ConstItemTypeRef item)
  {
    if (isFull())
    {
      return false;
    }
    arr[next] = item; // put the T in the array
    ++next; // increment the next index
    return true;
  }

  bool _remove(const ItemType& item)
  {
    for(IndexType i = 0; i < next; ++i)
    {
      if(arr[i] != item)
      {
        continue;
      }
      next--;
      while(i < next)
      {
        arr[i] = arr[++i];
        ++i;
      }
      arr[next].~ItemType();
      return true;
    }
    return false;
  }
  
  // Returns true if the T was removed
  // Returns false if the index was invalid
  bool removeAt(IndexType index)
  {
    if (index >= next)
    {
      return false;
    }
    --next; // decrement next index
    for (IndexType i = index; i < next; ++i) // shuffle everything down
    {
      arr[i] = arr[i + 1];
      return true;
    }
  }

  // These are for indexing the list
  // Be careful, these don't check if the index is valid
  ItemTypeRef operator[](IndexType index)
  {
    return arr[index];
  }
  ConstItemTypeRef operator[](IndexType index) const
  {
    return arr[index];
  }

  ~List()
  {
    next = 0;
    delete[] arr;
  }
};
