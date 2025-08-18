//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog.cpp
//
// Identification: src/primer/hyperloglog.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/hyperloglog.h"

namespace bustub {

/** @brief Parameterized constructor. */
template <typename KeyType>
HyperLogLog<KeyType>::HyperLogLog(int16_t n_bits) {
  cardinality_ = 0;
  b = n_bits;
  if (b >= 0){
    buckets = std::vector<uint64_t>(1ULL << b, 0);
  }
}

/**
 * @brief Function that computes binary.
 *
 * @param[in] hash
 * @returns binary of a given hash
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeBinary(const hash_t &hash) const -> std::bitset<BITSET_CAPACITY> {
  /** @TODO(student) Implement this function! */
  std::bitset<BITSET_CAPACITY> binaryRep(hash);
  return binaryRep;
}

/**
 * @brief Function that computes leading zeros.
 *
 * @param[in] bset - binary values of a given bitset
 * @returns leading zeros of given binary set
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::PositionOfLeftmostOne(const std::bitset<BITSET_CAPACITY> &bset) const -> uint64_t {
  /** @TODO(student) Implement this function! */
  for (int i = BITSET_CAPACITY - 1; i >= 0; i--){
    if (bset[i]) return BITSET_CAPACITY - i;
  }
  return BITSET_CAPACITY;
}

/**
 * @brief Adds a value into the HyperLogLog.
 *
 * @param[in] val - value that's added into hyperloglog
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::AddElem(KeyType val) -> void {
  /** @TODO(student) Implement this function! */
  if (b < 0) return;
  hash_t hashValue = CalculateHash(val);
  std::bitset<BITSET_CAPACITY> binaryRep(ComputeBinary(hashValue));
  uint64_t bucket_num = (binaryRep >> (BITSET_CAPACITY - b)).to_ullong();
  binaryRep <<= b;
  uint64_t leftmost = PositionOfLeftmostOne(binaryRep);
  if (bucket_num < buckets.size()) {
    buckets[bucket_num] = std::max(buckets[bucket_num], leftmost);
  }
}

/**
 * @brief Function that computes cardinality.
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeCardinality() -> void {
  /** @TODO(student) Implement this function! */
  if (b < 0) return;

  uint64_t m = buckets.size();
  double total = 0.0;
  for (uint64_t i = 0; i < m; i++){
    total += pow(2, -(double)buckets[i]);
  }
  cardinality_ = std::floor((double)(CONSTANT * m * m) / total);
}

template class HyperLogLog<int64_t>;
template class HyperLogLog<std::string>;

}  // namespace bustub
