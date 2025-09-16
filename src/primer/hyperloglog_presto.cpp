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
  if (b >= 0) {
    dense_bucket_.resize(1ULL << b, 0);
  }
}

/** @brief Element is added for HLL calculation. */
template <typename KeyType>
auto HyperLogLogPresto<KeyType>::AddElem(KeyType val) -> void {
  /** @TODO(student) Implement this For calculating hash, you can use the given function:

function! */
  if (b < 0) return;
  int16_t lsb = 0;
  hash_t hashVal = CalculateHash(val);
  std::bitset<64> binaryRep(hashVal);
  uint16_t num = (binaryRep >> (64 - b)).to_ullong();
  binaryRep = (binaryRep << b) >> b;
  

  while (!binaryRep[lsb] and lsb < 64 - b) {
    lsb++;
  }

  std::bitset<TOTAL_BUCKET_SIZE> total(lsb);
  std::bitset<DENSE_BUCKET_SIZE> dense;
  std::bitset<OVERFLOW_BUCKET_SIZE> overflow;
  for (uint16_t i = 0; i < DENSE_BUCKET_SIZE; i++){
    dense[i] = total[i];
  }
  for (uint16_t i = DENSE_BUCKET_SIZE; i < TOTAL_BUCKET_SIZE; i++){
    overflow[i - DENSE_BUCKET_SIZE] = total[i];
  }

  unsigned long long current_val = (overflow_bucket_[num].to_ullong() << DENSE_BUCKET_SIZE) | dense_bucket_[num].to_ullong();
  if (total.to_ullong() > current_val){
    dense_bucket_[num] = dense;
    overflow_bucket_[num] = overflow;
  }
}

/** @brief Function to compute cardinality. */
template <typename T>
auto HyperLogLogPresto<T>::ComputeCardinality() -> void {
  /** @TODO(student) Implement this function! */
  if (b < 0) return;

  double total = 0;
  double m = dense_bucket_.size();
  for (uint16_t i = 0; i < (uint16_t)m; i++) {
    unsigned long long dense_val = dense_bucket_[i].to_ullong();
    unsigned long long overflow_val = overflow_bucket_[i].to_ullong();
    unsigned long long full_val = (overflow_val << DENSE_BUCKET_SIZE) | dense_val;
    
    total += pow(2.0, -(double)full_val);
  }

  if (total == 0) return;
  cardinality_ = std::floor((CONSTANT * m * m) / total);
}

template class HyperLogLogPresto<int64_t>;
template class HyperLogLogPresto<std::string>;
}  // namespace bustub
