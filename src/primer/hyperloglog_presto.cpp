//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog_presto.cpp
//
// Identification: src/primer/hyperloglog_presto.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/hyperloglog_presto.h"

namespace bustub {

/** @brief Parameterized constructor. */
template <typename KeyType>
HyperLogLogPresto<KeyType>::HyperLogLogPresto(int16_t n_leading_bits) {
  cardinality_ = 0;
  b = n_leading_bits;
  if (b >= 0){
    dense_bucket_ = std::vector<std::bitset<DENSE_BUCKET_SIZE>>(1ULL << b, 0);
  }
}

/** @brief Element is added for HLL calculation. */
template <typename KeyType>
auto HyperLogLogPresto<KeyType>::AddElem(KeyType val) -> void {
  /** @TODO(student) Implement this function! */
  if (b < 0) return;
  uint16_t lsb = 0;
  hash_t hashVal = CalculateHash(val);
  std::bitset<64> binaryRep(hashVal);
  uint16_t num = (binaryRep >> (64 - b)).to_ullong();


  while (!binaryRep[lsb] and lsb < 64 - b){
    lsb++;
  }



  std::bitset<TOTAL_BUCKET_SIZE> rightmost(lsb);
  std::bitset<OVERFLOW_BUCKET_SIZE> overflow ((rightmost >> DENSE_BUCKET_SIZE).to_ullong());


  if (overflow.to_ullong() != 0){
    if (overflow_bucket_.count(num)){
      if (overflow_bucket_[num].to_ullong() < overflow.to_ullong())
        overflow_bucket_[num] = overflow;
    } else {
      overflow_bucket_[num] = overflow;
    }
    std::bitset<TOTAL_BUCKET_SIZE> tmp(overflow.to_ullong());
    rightmost = rightmost ^ (tmp << DENSE_BUCKET_SIZE);
  }

  std::bitset<DENSE_BUCKET_SIZE> base(rightmost.to_ullong());
  if (num < dense_bucket_.size()){
    if (dense_bucket_[num].to_ullong()){
      if (dense_bucket_[num].to_ullong() < base.to_ullong())
        dense_bucket_[num] = base;
    } else {
      dense_bucket_[num] = base;
    }
  }

}

/** @brief Function to compute cardinality. */
template <typename T>
auto HyperLogLogPresto<T>::ComputeCardinality() -> void {
  /** @TODO(student) Implement this function! */
  if (b < 0) return;


  double total = 0;
  double m = dense_bucket_.size();
  for (uint16_t i = 0; i < dense_bucket_.size(); i++){
    total += pow(2, -(double)dense_bucket_[i].to_ullong());
  }

  if (total == 0) return;
  cardinality_ = std::floor((CONSTANT * m * m) / total);
}

template class HyperLogLogPresto<int64_t>;
template class HyperLogLogPresto<std::string>;
}  // namespace bustub
