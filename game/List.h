#pragma once

#include <stddef.h>
#include <stdint.h>

template<typename T, size_t size>
class List
{
private:
  uint8_t next;
  T arr[size];
public:
  using ItemType = T;
  using ItemTypeRef = T&;
  using ConstItemTypeRef = const T&;
  using IndexType = uint8_t;
  static constexpr size_t _size = size;
  static constexpr IndexType firstIndex = 0;
  static constexpr IndexType lastIndex = _size - 1;
  List() : arr {}
  {
    next = 0;
  }

  // Returns the number of Ts currently in the list
  IndexType get_size() const
  {
    return static_cast<IndexType>(_size);
  }

  // Returns true if the list is full
  bool isFull(void) const
  {
    return next == get_size();
  }

  // Returns true if the list is empty
  bool isEmpty(void) const
  {
    return next == firstIndex;
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
  ItemTypeRef operator[](uint8_t index)
  {
    return arr[index];
  }
  ConstItemTypeRef operator[](IndexType index) const
  {
    return arr[index];
  }
};
