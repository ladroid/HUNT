#pragma once

#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdint.h>

template<typename T, size_t Size>
class List
{ 
public:
  using ItemType = T;
  using IndexType = uint8_t;
  static constexpr size_t _size = Size;
  static constexpr IndexType firstIndex = 0;
  static constexpr IndexType lastIndex = _size - 1;
private:
  ItemType items[_size];
  IndexType next;
public:
  List() : items { }, next { 0 }
  {  }
  
  bool isFull() {
    return (this->next == this->get_size());
  }
  
  bool isEmpty() {
    return (this->next == firstIndex);
  }
  
  bool add(const ItemType& item) {
    if (this->isFull()) {
      return false;
    }
  
    this->items[this->next] = item;
    this->next++;
    return true;
  }
  
  bool _remove(const ItemType& item) {
    for (IndexType i = 0; i < this->next; i++) {
      if (this->items[i] != item) {
        continue;
      }
      this->next--;
      while (i < this->next) {
        this->items[i] = this->items[i++];
        i++;
      }
      this->items[this->next].~ItemType();
      return true;
    }
    return false;
  }
  
  void removeAt(IndexType index) {
    this->next--;
    for(IndexType i = index; i < this->next; i++) {
      this->items[i] = this->items[i+1];
    }
    this->items[this->next].~ItemType();
  }
  
  ItemType& operator [] (IndexType index) {
    return this->items[index];
  }
  
  const ItemType& operator [] (IndexType index) const {
    return this->items[index];
  }
  
  IndexType get_size() const {
    return static_cast<IndexType>(_size);
  }
  
  void clear_list() {
    for(IndexType i = 0; i < this->next; i++) {
      this->items[i].~ItemType();
    }
    this->next = 0;
  }
};

#endif
