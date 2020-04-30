#pragma once

#include <iostream>

template <typename T>
class bit_container;

template <typename T>
class bit_const_iterator {
public:
  inline bool operator==(const bit_const_iterator<T>& other) const { 
    return index == other.index;
  }
  inline bool operator!=(const bit_const_iterator<T>& other) const { return !(*this == other); }
  
  template <typename U>
  inline bit_const_iterator& operator+=(U diff) {
    index += diff;
    return *this;
  }

  inline bit_const_iterator& operator++() {
    ++index;
    return *this;
  }

  inline bit_const_iterator& operator--() {
    --index;
    return *this;
  }

  inline bit_const_iterator operator++(int) {
    bit_const_iterator old = *this;
    ++index;
    return old;
  }

  inline bit_const_iterator operator--(int) {
    bit_const_iterator old = *this;
    --index;
    return old;
  }

  inline bool operator*() const { 
    return container[index];
  }

private:
  size_t index;
  const bit_container<T>& container;

  bit_const_iterator(size_t index, const bit_container<T>& container): 
    index(index), container(container) {}

  friend class bit_container<T>;
};

template <typename T>
class bit_container {
public:
  typedef bit_const_iterator<T> const_iterator;
  
  inline const_iterator begin() const {
    return const_iterator(0, val);
  }

  inline const_iterator end() const {
    return const_iterator(sizeof(T) * 8, *this);
  }
  
  template <typename U>
  inline bool operator[](U index) const {
    return val & (1 << index);
  }

  bit_container(T val): val(val) {}

private:
  T val;
};

