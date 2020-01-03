#ifndef SET_OPERATIONS_HPP
#define SET_OPERATIONS_HPP

#include <vector>
#include <algorithm>
#include <utility>
#include "../../common/utility.hpp"
#include "../core/radix_sort.hpp" // for reverse
#include "../core/lower_bound.hpp"
#include "../core/upper_bound.hpp"


#define SET_VLEN 768
#define SET_VLEN_EACH 256

namespace vstl {
namespace seq {

#if !(defined(_SX) || defined(__ve__))
template <class T>
std::vector<T> set_intersection(const std::vector<T>& left,
                                const std::vector<T>& right) {
  std::vector<T> ret;
  std::set_intersection(left.begin(), left.end(),
                        right.begin(), right.end(),
                        std::back_inserter(ret));
  return ret;
}

template <class T>
std::vector<T> set_union(const std::vector<T>& left,
                         const std::vector<T>& right) {
  std::vector<T> ret;
  std::set_union(left.begin(), left.end(),
                 right.begin(), right.end(),
                 std::back_inserter(ret));
  return ret;
}

template <class T>
std::vector<T> set_difference(const std::vector<T>& left,
                              const std::vector<T>& right) {
  std::vector<T> ret;
  std::set_difference(left.begin(), left.end(),
                      right.begin(), right.end(),
                      std::back_inserter(ret));
  return ret;
}

template <class T>
std::vector<T> set_merge(const std::vector<T>& left,
                         const std::vector<T>& right) {
  std::vector<T> ret;
  std::merge(left.begin(), left.end(),
             right.begin(), right.end(),
             std::back_inserter(ret));
  return ret;
}

template <class T>
std::vector<T> set_merge_desc(const std::vector<T>& left,
                              const std::vector<T>& right) {
  std::vector<T> ret;
  std::merge(left.begin(), left.end(),
             right.begin(), right.end(),
             std::back_inserter(ret), std::greater<T>());
  return ret;
}

template <class K, class V>
struct set_merge_pair_helper {
  bool operator()(const std::pair<K,V>& left,
                  const std::pair<K,V>& right) {
    return left.first < right.first;
  }
};

template <class T, class K>
void set_merge_pair(const std::vector<T>& left,
                    const std::vector<K>& left_val,
                    const std::vector<T>& right,
                    const std::vector<K>& right_val,
                    std::vector<T>& out,
                    std::vector<K>& out_val) {
  size_t left_size = left.size();
  size_t right_size = right.size();
  std::vector<std::pair<T,K>> left_pair(left_size);
  std::vector<std::pair<T,K>> right_pair(right_size);
  std::vector<std::pair<T,K>> out_pair;
  for(size_t i = 0; i < left_size; i++) {
    left_pair[i].first = left[i];
    left_pair[i].second = left_val[i];
  }
  for(size_t i = 0; i < right_size; i++) {
    right_pair[i].first = right[i];
    right_pair[i].second = right_val[i];
  }
  std::merge(left_pair.begin(), left_pair.end(),
             right_pair.begin(), right_pair.end(),
             std::back_inserter(out_pair),set_merge_pair_helper<T,K>());
  out.resize(left_size + right_size);
  out_val.resize(left_size + right_size);
  for(size_t i = 0; i < left_size + right_size; i++) {
    out[i] = out_pair[i].first;
    out_val[i] = out_pair[i].second;
  }
}

template <class K, class V>
struct set_merge_pair_desc_helper {
  bool operator()(const std::pair<K,V>& left,
                  const std::pair<K,V>& right) {
    return left.first > right.first;
  }
};

template <class T, class K>
void set_merge_pair_desc(const std::vector<T>& left,
                         const std::vector<K>& left_val,
                         const std::vector<T>& right,
                         const std::vector<K>& right_val,
                         std::vector<T>& out,
                         std::vector<K>& out_val) {
  size_t left_size = left.size();
  size_t right_size = right.size();
  std::vector<std::pair<T,K>> left_pair(left_size);
  std::vector<std::pair<T,K>> right_pair(right_size);
  std::vector<std::pair<T,K>> out_pair;
  for(size_t i = 0; i < left_size; i++) {
    left_pair[i].first = left[i];
    left_pair[i].second = left_val[i];
  }
  for(size_t i = 0; i < right_size; i++) {
    right_pair[i].first = right[i];
    right_pair[i].second = right_val[i];
  }
  std::merge(left_pair.begin(), left_pair.end(),
             right_pair.begin(), right_pair.end(),
             std::back_inserter(out_pair), set_merge_pair_desc_helper<T,K>());
  out.resize(left_size + right_size);
  out_val.resize(left_size + right_size);
  for(size_t i = 0; i < left_size + right_size; i++) {
    out[i] = out_pair[i].first;
    out_val[i] = out_pair[i].second;
  }
}

template <class T>
std::vector<size_t> set_separate(const std::vector<T>& key) {
  size_t size = key.size();
  std::vector<size_t> ret;
  if(size == 0) {
    ret.push_back(0);
    return ret;
  } else {
    T current = key[0];
    ret.push_back(0);
    for(size_t i = 1; i < size; i++) {
      if(key[i] != current) {
        current = key[i];
        ret.push_back(i);
      }
    }
    ret.push_back(size);
    return ret;
  }
}

template <class T>
std::vector<T> set_unique(const std::vector<T>& key) {
  size_t size = key.size();
  std::vector<T> ret;
  if(size == 0) {
    return ret;
  } else {
    T current = key[0];
    ret.push_back(current);
    for(size_t i = 1; i < size; i++) {
      if(key[i] != current) {
        current = key[i];
        ret.push_back(current);
      }
    }
    return ret;
  }
}

template <class T>
int set_is_unique(const std::vector<T>& key) {
  size_t size = key.size();
  std::vector<size_t> ret;
  if(size == 0) {
    return true;
  } else {
    T current = key[0];
    for(size_t i = 1; i < size; i++) {
      if(key[i] != current) current = key[i];
      else return false;
    }
    return true;
  }
}

#else // SX

template <class T>
void set_intersection_vreg(const T* lp, const T* rp, T* op, int* valid,
                           size_t* left_idx, size_t* right_idx,
                           size_t* left_idx_stop, size_t* right_idx_stop,
                           size_t* out_idx) {
#include "set_operations.incl1" // decl/init vregs
  while(1) {
#include "set_operations.incl2" // decl/load left/rightelm
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_0[j]) {
        int eq = leftelm0[j] == rightelm0[j];
        int lt = leftelm0[j] < rightelm0[j];
        if(eq) {
          op[out_idx_0[j]++] = leftelm0[j];
        }
        if(eq || lt) {
          left_idx_0[j]++;
        }
        if(eq || !lt) {
          right_idx_0[j]++;
        }
        if(left_idx_0[j] == left_idx_stop_0[j] ||
           right_idx_0[j] == right_idx_stop_0[j]) {
          valid_0[j] = false;
        }
      }
    }
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_1[j]) {
        int eq = leftelm1[j] == rightelm1[j];
        int lt = leftelm1[j] < rightelm1[j];
        if(eq) {
          op[out_idx_1[j]++] = leftelm1[j];
        }
        if(eq || lt) {
          left_idx_1[j]++;
        }
        if(eq || !lt) {
          right_idx_1[j]++;
        }
        if(left_idx_1[j] == left_idx_stop_1[j] ||
           right_idx_1[j] == right_idx_stop_1[j]) {
          valid_1[j] = false;
        }
      }
    }
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_2[j]) {
        int eq = leftelm2[j] == rightelm2[j];
        int lt = leftelm2[j] < rightelm2[j];
        if(eq) {
          op[out_idx_2[j]++] = leftelm2[j];
        }
        if(eq || lt) {
          left_idx_2[j]++;
        }
        if(eq || !lt) {
          right_idx_2[j]++;
        }
        if(left_idx_2[j] == left_idx_stop_2[j] ||
           right_idx_2[j] == right_idx_stop_2[j]) {
          valid_2[j] = false;
        }
      }
    }
    int any_valid = false;
    for(int i = 0; i < SET_VLEN_EACH; i++) {
      if(valid_0[i] || valid_1[i] || valid_2[i])
        any_valid = true;
    }
    if(any_valid == false) break;
  }
  for(size_t i = 0; i < SET_VLEN_EACH; i++) {
    out_idx[i] = out_idx_0[i];
    out_idx[SET_VLEN_EACH * 1 + i] = out_idx_1[i];
    out_idx[SET_VLEN_EACH * 2 + i] = out_idx_2[i];
  }
}

template <class T>
std::vector<T> set_intersection(const std::vector<T>& left,
                                const std::vector<T>& right) {
  int valid[SET_VLEN];
  for(int i = 0; i < SET_VLEN; i++) valid[i] = true;

  size_t left_size = left.size();
  size_t right_size = right.size();
  if(left_size == 0 || right_size == 0) return std::vector<T>();

  size_t each = ceil_div(left_size, size_t(SET_VLEN));
  if(each % 2 == 0) each++;
  
  size_t left_idx[SET_VLEN];
  size_t right_idx[SET_VLEN];
  size_t left_idx_stop[SET_VLEN];
  size_t right_idx_stop[SET_VLEN];
  size_t out_idx[SET_VLEN];
  size_t out_idx_save[SET_VLEN];
  std::vector<T> out;
  out.resize(left_size);
  for(int i = 0; i < SET_VLEN; i++) {
    size_t pos = each * i;
    if(pos < left_size) {
      left_idx[i] = pos;
      out_idx[i] = pos;
      out_idx_save[i] = pos;
    } else {
      valid[i] = false;
      left_idx[i] = left_size;
      out_idx[i] = left_size;
      out_idx_save[i] = left_size;
    }
  }
  T left_start[SET_VLEN];
  for(size_t i = 0; i < SET_VLEN; i++) {
    if(valid[i]) left_start[i] = left[left_idx[i]];
    else left_start[i] = 0;
  }
  lower_bound(right.data(), right_size, left_start, SET_VLEN, right_idx);
  for(size_t i = 0; i < SET_VLEN; i++) {
    if(right_idx[i] == right_size) valid[i] = false;
    if(!valid[i]) right_idx[i] = right_size; // left is not valid
  }
  for(size_t i = 0; i < SET_VLEN-1; i++) {
    if(valid[i]) left_start[i] = left[left_idx[i+1]-1];
    else left_start[i] = 0;
  }
  left_start[SET_VLEN-1] = left[left_size-1];
  upper_bound(right.data(), right_size, left_start, SET_VLEN, right_idx_stop);
  for(size_t i = 0; i < SET_VLEN; i++) {
    if(!valid[i]) right_idx_stop[i] = right_size;
  }
  for(int i = 0; i < SET_VLEN - 1; i++) {
    left_idx_stop[i] = left_idx[i + 1];
  }
  left_idx_stop[SET_VLEN-1] = left_size;
  right_idx_stop[SET_VLEN-1] = right_size;
  for(int i = 0; i < SET_VLEN; i++) {
    if(right_idx[i] == right_idx_stop[i]) valid[i] = false;
  }
  
  auto lp = left.data();
  auto rp = right.data();
  auto op = out.data();
  set_intersection_vreg(lp, rp, op, valid,
                        left_idx, right_idx,
                        left_idx_stop, right_idx_stop,
                        out_idx);

  size_t total = 0;
  for(size_t i = 0; i < SET_VLEN; i++) {
    total += out_idx[i] - out_idx_save[i];
  }
  std::vector<T> ret(total);
  T* retp = &ret[0];
  size_t current = 0;
  for(size_t i = 0; i < SET_VLEN; i++) {
#pragma cdir nodep
#pragma _NEC ivdep
    for(size_t j = 0; j < out_idx[i] - out_idx_save[i]; j++) {
      retp[current + j] = out[out_idx_save[i] + j];
    }
    current += out_idx[i] - out_idx_save[i];
  }
  return ret;
}

template <class T>
void set_union_vreg(const T* lp, const T* rp, T* op, int* valid,
                    size_t* left_idx, size_t* right_idx,
                    size_t* left_idx_stop, size_t* right_idx_stop,
                    size_t* out_idx) {
#include "set_operations.incl1"
  while(1) {
#include "set_operations.incl2"
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_0[j]) {
        int eq = leftelm0[j] == rightelm0[j];
        int lt = leftelm0[j] < rightelm0[j];
        if(eq || lt) {
          op[out_idx_0[j]] = leftelm0[j];
          left_idx_0[j]++;
        } else {
          op[out_idx_0[j]] = rightelm0[j];
          right_idx_0[j]++;
        }
        out_idx_0[j]++;
        if(eq) {
          right_idx_0[j]++;
        }
        if(left_idx_0[j] == left_idx_stop_0[j] ||
           right_idx_0[j] == right_idx_stop_0[j]) {
          valid_0[j] = false;
        }
      }
    }
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_1[j]) {
        int eq = leftelm1[j] == rightelm1[j];
        int lt = leftelm1[j] < rightelm1[j];
        if(eq || lt) {
          op[out_idx_1[j]] = leftelm1[j];
          left_idx_1[j]++;
        } else {
          op[out_idx_1[j]] = rightelm1[j];
          right_idx_1[j]++;
        }
        out_idx_1[j]++;
        if(eq) {
          right_idx_1[j]++;
        }
        if(left_idx_1[j] == left_idx_stop_1[j] ||
           right_idx_1[j] == right_idx_stop_1[j]) {
          valid_1[j] = false;
        }
      }
    }
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_2[j]) {
        int eq = leftelm2[j] == rightelm2[j];
        int lt = leftelm2[j] < rightelm2[j];
        if(eq || lt) {
          op[out_idx_2[j]] = leftelm2[j];
          left_idx_2[j]++;
        } else {
          op[out_idx_2[j]] = rightelm2[j];
          right_idx_2[j]++;
        }
        out_idx_2[j]++;
        if(eq) {
          right_idx_2[j]++;
        }
        if(left_idx_2[j] == left_idx_stop_2[j] ||
           right_idx_2[j] == right_idx_stop_2[j]) {
          valid_2[j] = false;
        }
      }
    }
    int any_valid = false;
    for(int i = 0; i < SET_VLEN_EACH; i++) {
      if(valid_0[i] || valid_1[i] || valid_2[i])
        any_valid = true;
    }
    if(any_valid == false) break;
  }
  for(size_t i = 0; i < SET_VLEN_EACH; i++) {
    out_idx[i] = out_idx_0[i];
    out_idx[SET_VLEN_EACH * 1 + i] = out_idx_1[i];
    out_idx[SET_VLEN_EACH * 2 + i] = out_idx_2[i];
    left_idx[i] = left_idx_0[i];
    left_idx[SET_VLEN_EACH * 1 + i] = left_idx_1[i];
    left_idx[SET_VLEN_EACH * 2 + i] = left_idx_2[i];
    right_idx[i] = right_idx_0[i];
    right_idx[SET_VLEN_EACH * 1 + i] = right_idx_1[i];
    right_idx[SET_VLEN_EACH * 2 + i] = right_idx_2[i];
  }
}

template <class T>
std::vector<T> set_union(const std::vector<T>& left,
                         const std::vector<T>& right) {
  int valid[SET_VLEN];
  for(int i = 0; i < SET_VLEN; i++) valid[i] = true;

  size_t left_size = left.size();
  size_t right_size = right.size();
  if(left_size == 0) return right;
  if(right_size == 0) return left;

  size_t each = ceil_div(left_size, size_t(SET_VLEN));
  if(each % 2 == 0) each++;
  
  size_t left_idx[SET_VLEN];
  size_t right_idx[SET_VLEN];
  size_t left_idx_stop[SET_VLEN];
  size_t right_idx_stop[SET_VLEN];
  size_t out_idx[SET_VLEN];
  size_t out_idx_save[SET_VLEN];
  std::vector<T> out;
  out.resize(left_size + right_size);
  for(int i = 0; i < SET_VLEN; i++) {
    size_t pos = each * i;
    if(pos < left_size) {
      left_idx[i] = pos;
    } else {
      valid[i] = false;
      left_idx[i] = left_size;
    }
  }
  // need to split by different data for union
  for(int i = 0; i < SET_VLEN-1; i++) {
    if(valid[i]) {
      T last = left[left_idx[i+1]-1];
      while(true) {
        if(left_idx[i+1] < left_size - 1 && left[left_idx[i+1]] == last) {
          left_idx[i+1]++;
        } else break;
      }
    }
  }
  T left_start[SET_VLEN];
  for(size_t i = 0; i < SET_VLEN; i++) {
    if(valid[i]) left_start[i] = left[left_idx[i]];
    else left_start[i] = 0;
  }
  lower_bound(right.data(), right_size, left_start, SET_VLEN, right_idx);
  right_idx[0] = 0;
  for(size_t i = 0; i < SET_VLEN; i++) {
    if(right_idx[i] == right_size) valid[i] = false;
    if(!valid[i]) right_idx[i] = right_size; // left is not valid
  }
  out_idx[0] = 0;
  out_idx_save[0] = 0;
  for(int i = 1; i < SET_VLEN; i++) {
    out_idx[i] = (left_idx[i] - left_idx[i-1])
      + (right_idx[i] - right_idx[i-1])
      + out_idx[i-1];
    out_idx_save[i] = out_idx[i];
  }
  for(int i = 0; i < SET_VLEN - 1; i++) {
    left_idx_stop[i] = left_idx[i + 1];
    right_idx_stop[i] = right_idx[i + 1];
  }
  left_idx_stop[SET_VLEN-1] = left_size;
  right_idx_stop[SET_VLEN-1] = right_size;
  for(int i = 0; i < SET_VLEN; i++) {
    if(left_idx[i] == left_idx_stop[i] || right_idx[i] == right_idx_stop[i]) 
      valid[i] = false;
  }
  auto lp = left.data();
  auto rp = right.data();
  auto op = out.data();
  set_union_vreg(lp, rp, op, valid,
                 left_idx, right_idx,
                 left_idx_stop, right_idx_stop,
                 out_idx);

  size_t total = 0;
  for(size_t i = 0; i < SET_VLEN; i++) {
    total += (out_idx[i] - out_idx_save[i]) +
      (left_idx_stop[i] - left_idx[i]) +
      (right_idx_stop[i] - right_idx[i]);
  }
  std::vector<T> ret(total);
  T* retp = &ret[0];
  size_t current = 0;
  for(size_t i = 0; i < SET_VLEN; i++) {
    for(size_t j = 0; j < out_idx[i] - out_idx_save[i]; j++) {
      retp[current + j] = out[out_idx_save[i] + j];
    }
    current += out_idx[i] - out_idx_save[i];
    for(size_t j = 0; j < left_idx_stop[i] - left_idx[i]; j++) {
      retp[current + j] = lp[left_idx[i] + j];
    }
    current += left_idx_stop[i] - left_idx[i];
    for(size_t j = 0; j < right_idx_stop[i] - right_idx[i]; j++) {
      retp[current + j] = rp[right_idx[i] + j];
    }
    current += right_idx_stop[i] - right_idx[i];
  }
  return ret;
}

template <class T>
void set_difference_vreg(const T* lp, const T* rp, T* op, int* valid,
                         size_t* left_idx, size_t* right_idx,
                         size_t* left_idx_stop, size_t* right_idx_stop,
                         size_t* out_idx) {
#include "set_operations.incl1"
  while(1) {
#include "set_operations.incl2"
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_0[j]) {
        int eq = leftelm0[j] == rightelm0[j];
        int lt = leftelm0[j] < rightelm0[j];
        if(eq) {
          left_idx_0[j]++;
          right_idx_0[j]++;
        } else if(lt) {
          op[out_idx_0[j]++] = leftelm0[j];
          left_idx_0[j]++;
        } else {
          right_idx_0[j]++;
        }
        if(left_idx_0[j] == left_idx_stop_0[j] ||
           right_idx_0[j] == right_idx_stop_0[j]) {
          valid_0[j] = false;
        }
      }
    }
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_1[j]) {
        int eq = leftelm1[j] == rightelm1[j];
        int lt = leftelm1[j] < rightelm1[j];
        if(eq) {
          left_idx_1[j]++;
          right_idx_1[j]++;
        } else if(lt) {
          op[out_idx_1[j]++] = leftelm1[j];
          left_idx_1[j]++;
        } else {
          right_idx_1[j]++;
        }
        if(left_idx_1[j] == left_idx_stop_1[j] ||
           right_idx_1[j] == right_idx_stop_1[j]) {
          valid_1[j] = false;
        }
      }
    }
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_2[j]) {
        int eq = leftelm2[j] == rightelm2[j];
        int lt = leftelm2[j] < rightelm2[j];
        if(eq) {
          left_idx_2[j]++;
          right_idx_2[j]++;
        } else if(lt) {
          op[out_idx_2[j]++] = leftelm2[j];
          left_idx_2[j]++;
        } else {
          right_idx_2[j]++;
        }
        if(left_idx_2[j] == left_idx_stop_2[j] ||
           right_idx_2[j] == right_idx_stop_2[j]) {
          valid_2[j] = false;
        }
      }
    }
    int any_valid = false;
    for(int i = 0; i < SET_VLEN_EACH; i++) {
      if(valid_0[i] || valid_1[i] || valid_2[i])
        any_valid = true;
    }
    if(any_valid == false) break;
  }
  for(size_t i = 0; i < SET_VLEN_EACH; i++) {
    out_idx[i] = out_idx_0[i];
    out_idx[SET_VLEN_EACH * 1 + i] = out_idx_1[i];
    out_idx[SET_VLEN_EACH * 2 + i] = out_idx_2[i];
    left_idx[i] = left_idx_0[i];
    left_idx[SET_VLEN_EACH * 1 + i] = left_idx_1[i];
    left_idx[SET_VLEN_EACH * 2 + i] = left_idx_2[i];
  }
}

template <class T>
std::vector<T> set_difference(const std::vector<T>& left,
                              const std::vector<T>& right) {
  int valid[SET_VLEN];
  for(int i = 0; i < SET_VLEN; i++) valid[i] = true;

  size_t left_size = left.size();
  size_t right_size = right.size();
  if(left_size == 0) return std::vector<T>();
  if(right_size == 0) return left;

  size_t each = ceil_div(left_size, size_t(SET_VLEN));
  if(each % 2 == 0) each++;
  
  size_t left_idx[SET_VLEN];
  size_t right_idx[SET_VLEN];
  size_t left_idx_stop[SET_VLEN];
  size_t right_idx_stop[SET_VLEN];
  size_t out_idx[SET_VLEN];
  size_t out_idx_save[SET_VLEN];
  std::vector<T> out;
  out.resize(left_size);
  for(int i = 0; i < SET_VLEN; i++) {
    size_t pos = each * i;
    if(pos < left_size) {
      left_idx[i] = pos;
    } else {
      valid[i] = false;
      left_idx[i] = left_size;
    }
  }
  // need to split by different data for difference
  for(int i = 0; i < SET_VLEN-1; i++) {
    if(valid[i]) {
      T last = left[left_idx[i+1]-1];
      while(true) {
        if(left[left_idx[i+1]] == last && left_idx[i+1] < left_size - 1) {
          left_idx[i+1]++;
        } else break;
      }
    }
  }
  for(int i = 0; i < SET_VLEN; i++) {
    out_idx[i] = left_idx[i];
    out_idx_save[i] = left_idx[i];
  }
  T left_start[SET_VLEN];
  for(size_t i = 0; i < SET_VLEN; i++) {
    if(valid[i]) left_start[i] = left[left_idx[i]];
    else left_start[i] = 0;
  }
  lower_bound(right.data(), right_size, left_start, SET_VLEN, right_idx);
  right_idx[0] = 0;
  for(size_t i = 0; i < SET_VLEN; i++) {
    if(right_idx[i] == right_size) valid[i] = false;
    if(!valid[i]) right_idx[i] = right_size; // left is not valid
  }
  for(int i = 0; i < SET_VLEN - 1; i++) {
    left_idx_stop[i] = left_idx[i + 1];
    right_idx_stop[i] = right_idx[i + 1];
  }
  left_idx_stop[SET_VLEN-1] = left_size;
  right_idx_stop[SET_VLEN-1] = right_size;
  for(int i = 0; i < SET_VLEN; i++) {
    if(left_idx[i] == left_idx_stop[i] || right_idx[i] == right_idx_stop[i]) 
      valid[i] = false;
  }
  auto lp = left.data();
  auto rp = right.data();
  auto op = out.data();
  set_difference_vreg(lp, rp, op, valid,
                      left_idx, right_idx,
                      left_idx_stop, right_idx_stop,
                      out_idx);
  size_t total = 0;
  for(size_t i = 0; i < SET_VLEN; i++) {
    total += (out_idx[i] - out_idx_save[i]) +
      (left_idx_stop[i] - left_idx[i]);
  }
  std::vector<T> ret(total);
  T* retp = &ret[0];
  size_t current = 0;
  for(size_t i = 0; i < SET_VLEN; i++) {
    for(size_t j = 0; j < out_idx[i] - out_idx_save[i]; j++) {
      retp[current + j] = out[out_idx_save[i] + j];
    }
    current += out_idx[i] - out_idx_save[i];
    for(size_t j = 0; j < left_idx_stop[i] - left_idx[i]; j++) {
      retp[current + j] = lp[left_idx[i] + j];
    }
    current += left_idx_stop[i] - left_idx[i];
  }
  return ret;
}

template <class T>
void set_merge_vreg(const T* lp, const T* rp, T* op, int* valid,
                    size_t* left_idx, size_t* right_idx,
                    size_t* left_idx_stop, size_t* right_idx_stop,
                    size_t* out_idx) {
#include "set_operations.incl1"
  while(1) {
#include "set_operations.incl2"
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_0[j]) {
        if(leftelm0[j] <= rightelm0[j]) {
          op[out_idx_0[j]] = leftelm0[j];
          left_idx_0[j]++;
        } else {
          op[out_idx_0[j]] = rightelm0[j];
          right_idx_0[j]++;
        }
        out_idx_0[j]++;
        if(left_idx_0[j] == left_idx_stop_0[j] ||
           right_idx_0[j] == right_idx_stop_0[j]) {
          valid_0[j] = false;
        }
      }
    }
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_1[j]) {
        if(leftelm1[j] <= rightelm1[j]) {
          op[out_idx_1[j]] = leftelm1[j];
          left_idx_1[j]++;
        } else {
          op[out_idx_1[j]] = rightelm1[j];
          right_idx_1[j]++;
        }
        out_idx_1[j]++;
        if(left_idx_1[j] == left_idx_stop_1[j] ||
           right_idx_1[j] == right_idx_stop_1[j]) {
          valid_1[j] = false;
        }
      }
    }
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_2[j]) {
        if(leftelm2[j] <= rightelm2[j]) {
          op[out_idx_2[j]] = leftelm2[j];
          left_idx_2[j]++;
        } else {
          op[out_idx_2[j]] = rightelm2[j];
          right_idx_2[j]++;
        }
        out_idx_2[j]++;
        if(left_idx_2[j] == left_idx_stop_2[j] ||
           right_idx_2[j] == right_idx_stop_2[j]) {
          valid_2[j] = false;
        }
      }
    }
    int any_valid = false;
    for(int i = 0; i < SET_VLEN_EACH; i++) {
      if(valid_0[i] || valid_1[i] || valid_2[i])
        any_valid = true;
    }
    if(any_valid == false) break;
  }
  for(size_t i = 0; i < SET_VLEN_EACH; i++) {
    out_idx[i] = out_idx_0[i];
    out_idx[SET_VLEN_EACH * 1 + i] = out_idx_1[i];
    out_idx[SET_VLEN_EACH * 2 + i] = out_idx_2[i];
    left_idx[i] = left_idx_0[i];
    left_idx[SET_VLEN_EACH * 1 + i] = left_idx_1[i];
    left_idx[SET_VLEN_EACH * 2 + i] = left_idx_2[i];
    right_idx[i] = right_idx_0[i];
    right_idx[SET_VLEN_EACH * 1 + i] = right_idx_1[i];
    right_idx[SET_VLEN_EACH * 2 + i] = right_idx_2[i];
  }
}

template <class T>
std::vector<T> set_merge(const std::vector<T>& left,
                         const std::vector<T>& right) {
  int valid[SET_VLEN];
  for(int i = 0; i < SET_VLEN; i++) valid[i] = true;

  size_t left_size = left.size();
  size_t right_size = right.size();
  if(left_size == 0) return right;
  if(right_size == 0) return left;

  size_t each = ceil_div(left_size, size_t(SET_VLEN));
  if(each % 2 == 0) each++;
  
  size_t left_idx[SET_VLEN];
  size_t right_idx[SET_VLEN];
  size_t left_idx_stop[SET_VLEN];
  size_t right_idx_stop[SET_VLEN];
  size_t out_idx[SET_VLEN];
  std::vector<T> out;
  out.resize(left_size + right_size);
  for(int i = 0; i < SET_VLEN; i++) {
    size_t pos = each * i;
    if(pos < left_size) {
      left_idx[i] = pos;
    } else {
      valid[i] = false;
      left_idx[i] = left_size;
    }
  }
  T left_start[SET_VLEN];
  for(size_t i = 0; i < SET_VLEN; i++) {
    if(valid[i]) left_start[i] = left[left_idx[i]];
    else left_start[i] = 0;
  }
  lower_bound(right.data(), right_size, left_start, SET_VLEN, right_idx);
  right_idx[0] = 0;
  for(size_t i = 0; i < SET_VLEN; i++) {
    if(right_idx[i] == right_size) valid[i] = false;
    if(!valid[i]) right_idx[i] = right_size; // left is not valid
  }
  out_idx[0] = 0;
  for(int i = 1; i < SET_VLEN; i++) {
    out_idx[i] = (left_idx[i] - left_idx[i-1]) +
      (right_idx[i] - right_idx[i-1])
      + out_idx[i-1];
  }
  for(int i = 0; i < SET_VLEN - 1; i++) {
    left_idx_stop[i] = left_idx[i + 1];
    right_idx_stop[i] = right_idx[i + 1];
  }
  left_idx_stop[SET_VLEN-1] = left_size;
  right_idx_stop[SET_VLEN-1] = right_size;
  for(int i = 0; i < SET_VLEN; i++) {
    if(right_idx[i] == right_idx_stop[i]) 
      valid[i] = false;
  }
  auto lp = left.data();
  auto rp = right.data();
  auto op = out.data();
  set_merge_vreg(lp, rp, op, valid,
                 left_idx, right_idx,
                 left_idx_stop, right_idx_stop,
                 out_idx);

  for(size_t i = 0; i < SET_VLEN; i++) {
    for(size_t j = 0; j < left_idx_stop[i] - left_idx[i]; j++) {
      op[out_idx[i] + j] = lp[left_idx[i] + j];
    }
    for(size_t j = 0; j < right_idx_stop[i] - right_idx[i]; j++) {
      op[out_idx[i] + j] = rp[right_idx[i] + j];
    }
  }
  return out;
}

template <class T>
void set_merge_desc_vreg(const T* lp, const T* rp, T* op, int* valid,
                         size_t* left_idx, size_t* right_idx,
                         size_t* left_idx_stop, size_t* right_idx_stop,
                         size_t* out_idx) {
#include "set_operations.incl1"
  while(1) {
#include "set_operations.incl2"
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_0[j]) {
        if(leftelm0[j] >= rightelm0[j]) { // desc
          op[out_idx_0[j]] = leftelm0[j];
          left_idx_0[j]++;
        } else {
          op[out_idx_0[j]] = rightelm0[j];
          right_idx_0[j]++;
        }
        out_idx_0[j]++;
        if(left_idx_0[j] == left_idx_stop_0[j] ||
           right_idx_0[j] == right_idx_stop_0[j]) {
          valid_0[j] = false;
        }
      }
    }
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_1[j]) {
        if(leftelm1[j] >= rightelm1[j]) {
          op[out_idx_1[j]] = leftelm1[j];
          left_idx_1[j]++;
        } else {
          op[out_idx_1[j]] = rightelm1[j];
          right_idx_1[j]++;
        }
        out_idx_1[j]++;
        if(left_idx_1[j] == left_idx_stop_1[j] ||
           right_idx_1[j] == right_idx_stop_1[j]) {
          valid_1[j] = false;
        }
      }
    }
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_2[j]) {
        if(leftelm2[j] >= rightelm2[j]) {
          op[out_idx_2[j]] = leftelm2[j];
          left_idx_2[j]++;
        } else {
          op[out_idx_2[j]] = rightelm2[j];
          right_idx_2[j]++;
        }
        out_idx_2[j]++;
        if(left_idx_2[j] == left_idx_stop_2[j] ||
           right_idx_2[j] == right_idx_stop_2[j]) {
          valid_2[j] = false;
        }
      }
    }
    int any_valid = false;
    for(int i = 0; i < SET_VLEN_EACH; i++) {
      if(valid_0[i] || valid_1[i] || valid_2[i])
        any_valid = true;
    }
    if(any_valid == false) break;
  }
  for(size_t i = 0; i < SET_VLEN_EACH; i++) {
    out_idx[i] = out_idx_0[i];
    out_idx[SET_VLEN_EACH * 1 + i] = out_idx_1[i];
    out_idx[SET_VLEN_EACH * 2 + i] = out_idx_2[i];
    left_idx[i] = left_idx_0[i];
    left_idx[SET_VLEN_EACH * 1 + i] = left_idx_1[i];
    left_idx[SET_VLEN_EACH * 2 + i] = left_idx_2[i];
    right_idx[i] = right_idx_0[i];
    right_idx[SET_VLEN_EACH * 1 + i] = right_idx_1[i];
    right_idx[SET_VLEN_EACH * 2 + i] = right_idx_2[i];
  }
}

template <class T>
std::vector<T> set_merge_desc(const std::vector<T>& left,
                              const std::vector<T>& right) {
  int valid[SET_VLEN];
  for(int i = 0; i < SET_VLEN; i++) valid[i] = true;

  size_t left_size = left.size();
  size_t right_size = right.size();
  if(left_size == 0) return right;
  if(right_size == 0) return left;
  size_t each = ceil_div(left_size, size_t(SET_VLEN));
  if(each % 2 == 0) each++;
  
  size_t left_idx[SET_VLEN];
  size_t right_idx[SET_VLEN];
  size_t left_idx_stop[SET_VLEN];
  size_t right_idx_stop[SET_VLEN];
  size_t out_idx[SET_VLEN];
  std::vector<T> out;
  out.resize(left_size + right_size);
  for(int i = 0; i < SET_VLEN; i++) {
    size_t pos = each * i;
    if(pos < left_size) {
      left_idx[i] = pos;
    } else {
      valid[i] = false;
      left_idx[i] = left_size;
    }
  }
  T left_start[SET_VLEN];
  for(size_t i = 0; i < SET_VLEN; i++) {
    if(valid[i]) left_start[i] = left[left_idx[i]];
    else left_start[i] = 0;
  }
  lower_bound_desc(right.data(), right_size, left_start, SET_VLEN, right_idx);
  right_idx[0] = 0;
  for(size_t i = 0; i < SET_VLEN; i++) {
    if(right_idx[i] == right_size) valid[i] = false;
    if(!valid[i]) right_idx[i] = right_size; // left is not valid
  }
  out_idx[0] = 0;
  for(int i = 1; i < SET_VLEN; i++) {
    out_idx[i] = (left_idx[i] - left_idx[i-1]) +
      (right_idx[i] - right_idx[i-1])
      + out_idx[i-1];
  }
  for(int i = 0; i < SET_VLEN - 1; i++) {
    left_idx_stop[i] = left_idx[i + 1];
    right_idx_stop[i] = right_idx[i + 1];
  }
  left_idx_stop[SET_VLEN-1] = left_size;
  right_idx_stop[SET_VLEN-1] = right_size;
  for(int i = 0; i < SET_VLEN; i++) {
    if(right_idx[i] == right_idx_stop[i]) 
      valid[i] = false;
  }
  auto lp = left.data();
  auto rp = right.data();
  auto op = out.data();
  set_merge_desc_vreg(lp, rp, op, valid,
                      left_idx, right_idx,
                      left_idx_stop, right_idx_stop,
                      out_idx);
  for(size_t i = 0; i < SET_VLEN; i++) {
    for(size_t j = 0; j < left_idx_stop[i] - left_idx[i]; j++) {
      op[out_idx[i] + j] = lp[left_idx[i] + j];
    }
    for(size_t j = 0; j < right_idx_stop[i] - right_idx[i]; j++) {
      op[out_idx[i] + j] = rp[right_idx[i] + j];
    }
  }
  return out;
}

template <class T, class K>
void set_merge_pair_vreg(const T* lp, const K* lvp, const T* rp, const K* rvp,
                         T* op, K* ovp,
                         int* valid, size_t* left_idx, size_t* right_idx,
                         size_t* left_idx_stop, size_t* right_idx_stop,
                         size_t* out_idx) {
#include "set_operations.incl1"
  while(1) {
#include "set_operations.incl2"
  K leftval0[SET_VLEN_EACH];
  K leftval1[SET_VLEN_EACH];
  K leftval2[SET_VLEN_EACH];
  K rightval0[SET_VLEN_EACH];
  K rightval1[SET_VLEN_EACH];
  K rightval2[SET_VLEN_EACH];
#pragma _NEC vreg(leftval0)
#pragma _NEC vreg(leftval1)
#pragma _NEC vreg(leftval2)
#pragma _NEC vreg(rightval0)
#pragma _NEC vreg(rightval1)
#pragma _NEC vreg(rightval2)
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_0[j]) leftval0[j] = lvp[left_idx_0[j]];
      if(valid_0[j]) rightval0[j] = rvp[right_idx_0[j]];
      if(valid_1[j]) leftval1[j] = lvp[left_idx_1[j]];
      if(valid_1[j]) rightval1[j] = rvp[right_idx_1[j]];
      if(valid_2[j]) leftval2[j] = lvp[left_idx_2[j]];
      if(valid_2[j]) rightval2[j] = rvp[right_idx_2[j]];
    }
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_0[j]) {
        if(leftelm0[j] <= rightelm0[j]) {
          op[out_idx_0[j]] = leftelm0[j];
          ovp[out_idx_0[j]] = leftval0[j];
          left_idx_0[j]++;
        } else {
          op[out_idx_0[j]] = rightelm0[j];
          ovp[out_idx_0[j]] = rightval0[j];
          right_idx_0[j]++;
        }
        out_idx_0[j]++;
        if(left_idx_0[j] == left_idx_stop_0[j] ||
           right_idx_0[j] == right_idx_stop_0[j]) {
          valid_0[j] = false;
        }
      }
    }
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_1[j]) {
        if(leftelm1[j] <= rightelm1[j]) {
          op[out_idx_1[j]] = leftelm1[j];
          ovp[out_idx_1[j]] = leftval1[j];
          left_idx_1[j]++;
        } else {
          op[out_idx_1[j]] = rightelm1[j];
          ovp[out_idx_1[j]] = rightval1[j];
          right_idx_1[j]++;
        }
        out_idx_1[j]++;
        if(left_idx_1[j] == left_idx_stop_1[j] ||
           right_idx_1[j] == right_idx_stop_1[j]) {
          valid_1[j] = false;
        }
      }
    }
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_2[j]) {
        if(leftelm2[j] <= rightelm2[j]) {
          op[out_idx_2[j]] = leftelm2[j];
          ovp[out_idx_2[j]] = leftval2[j];
          left_idx_2[j]++;
        } else {
          op[out_idx_2[j]] = rightelm2[j];
          ovp[out_idx_2[j]] = rightval2[j];
          right_idx_2[j]++;
        }
        out_idx_2[j]++;
        if(left_idx_2[j] == left_idx_stop_2[j] ||
           right_idx_2[j] == right_idx_stop_2[j]) {
          valid_2[j] = false;
        }
      }
    }
    int any_valid = false;
    for(int i = 0; i < SET_VLEN_EACH; i++) {
      if(valid_0[i] || valid_1[i] || valid_2[i])
        any_valid = true;
    }
    if(any_valid == false) break;
  }
  for(size_t i = 0; i < SET_VLEN_EACH; i++) {
    out_idx[i] = out_idx_0[i];
    out_idx[SET_VLEN_EACH * 1 + i] = out_idx_1[i];
    out_idx[SET_VLEN_EACH * 2 + i] = out_idx_2[i];
    left_idx[i] = left_idx_0[i];
    left_idx[SET_VLEN_EACH * 1 + i] = left_idx_1[i];
    left_idx[SET_VLEN_EACH * 2 + i] = left_idx_2[i];
    right_idx[i] = right_idx_0[i];
    right_idx[SET_VLEN_EACH * 1 + i] = right_idx_1[i];
    right_idx[SET_VLEN_EACH * 2 + i] = right_idx_2[i];
  }
}

template <class T, class K>
void set_merge_pair(const std::vector<T>& left,
                    const std::vector<K>& left_val,
                    const std::vector<T>& right,
                    const std::vector<K>& right_val,
                    std::vector<T>& out,
                    std::vector<K>& out_val) {
  int valid[SET_VLEN];
  for(int i = 0; i < SET_VLEN; i++) valid[i] = true;
  size_t left_size = left.size();
  size_t right_size = right.size();
  if(left_size == 0) {
    out = right;
    out_val = right_val;
    return;
  }
  if(right_size == 0) {
    out = left;
    out_val = left_val;
    return;
  }
  if(left_size != left_val.size() || right_size != right_val.size())
    throw std::runtime_error("sizes of key and value are not the same");
  size_t each = ceil_div(left_size, size_t(SET_VLEN));
  if(each % 2 == 0) each++;
  
  size_t left_idx[SET_VLEN];
  size_t right_idx[SET_VLEN];
  size_t left_idx_stop[SET_VLEN];
  size_t right_idx_stop[SET_VLEN];
  size_t out_idx[SET_VLEN];
  out.resize(left_size + right_size);
  out_val.resize(left_size + right_size);
  for(int i = 0; i < SET_VLEN; i++) {
    size_t pos = each * i;
    if(pos < left_size) {
      left_idx[i] = pos;
    } else {
      valid[i] = false;
      left_idx[i] = left_size;
    }
  }
  T left_start[SET_VLEN];
  for(size_t i = 0; i < SET_VLEN; i++) {
    if(valid[i]) left_start[i] = left[left_idx[i]];
    else left_start[i] = 0;
  }
  lower_bound(right.data(), right_size, left_start, SET_VLEN, right_idx);
  right_idx[0] = 0;
  for(size_t i = 0; i < SET_VLEN; i++) {
    if(right_idx[i] == right_size) valid[i] = false;
    if(!valid[i]) right_idx[i] = right_size; // left is not valid
  }
  out_idx[0] = 0;
  for(int i = 1; i < SET_VLEN; i++) {
    out_idx[i] =
      (left_idx[i] - left_idx[i-1]) +
      (right_idx[i] - right_idx[i-1]) +
      out_idx[i-1];
  }
  for(int i = 0; i < SET_VLEN - 1; i++) {
    left_idx_stop[i] = left_idx[i + 1];
    right_idx_stop[i] = right_idx[i + 1];
  }
  left_idx_stop[SET_VLEN-1] = left_size;
  right_idx_stop[SET_VLEN-1] = right_size;
  for(int i = 0; i < SET_VLEN; i++) {
    if(right_idx[i] == right_idx_stop[i]) 
      valid[i] = false;
  }
  auto lp = left.data();
  auto lvp = left_val.data();
  auto rp = right.data();
  auto rvp = right_val.data();
  auto op = out.data();
  auto ovp = out_val.data();
  set_merge_pair_vreg(lp, lvp, rp, rvp, op, ovp,
                      valid, left_idx, right_idx,
                      left_idx_stop, right_idx_stop,
                      out_idx);

  for(size_t i = 0; i < SET_VLEN; i++) {
    for(size_t j = 0; j < left_idx_stop[i] - left_idx[i]; j++) {
      op[out_idx[i] + j] = lp[left_idx[i] + j];
      ovp[out_idx[i] + j] = lvp[left_idx[i] + j];
    }
    for(size_t j = 0; j < right_idx_stop[i] - right_idx[i]; j++) {
      op[out_idx[i] + j] = rp[right_idx[i] + j];
      ovp[out_idx[i] + j] = rvp[right_idx[i] + j];
    }
  }
}

template <class T, class K>
void set_merge_pair_desc_vreg(const T* lp, const K* lvp,
                              const T* rp, const K* rvp, T* op, K* ovp,
                              int* valid, size_t* left_idx, size_t* right_idx,
                              size_t* left_idx_stop, size_t* right_idx_stop,
                              size_t* out_idx) {
  
#include "set_operations.incl1"
  while(1) {
#include "set_operations.incl2"
  K leftval0[SET_VLEN_EACH];
  K leftval1[SET_VLEN_EACH];
  K leftval2[SET_VLEN_EACH];
  K rightval0[SET_VLEN_EACH];
  K rightval1[SET_VLEN_EACH];
  K rightval2[SET_VLEN_EACH];
#pragma _NEC vreg(leftval0)
#pragma _NEC vreg(leftval1)
#pragma _NEC vreg(leftval2)
#pragma _NEC vreg(rightval0)
#pragma _NEC vreg(rightval1)
#pragma _NEC vreg(rightval2)
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_0[j]) leftval0[j] = lvp[left_idx_0[j]];
      if(valid_0[j]) rightval0[j] = rvp[right_idx_0[j]];
      if(valid_1[j]) leftval1[j] = lvp[left_idx_1[j]];
      if(valid_1[j]) rightval1[j] = rvp[right_idx_1[j]];
      if(valid_2[j]) leftval2[j] = lvp[left_idx_2[j]];
      if(valid_2[j]) rightval2[j] = rvp[right_idx_2[j]];
    }
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_0[j]) {
        if(leftelm0[j] >= rightelm0[j]) { // desc
          op[out_idx_0[j]] = leftelm0[j];
          ovp[out_idx_0[j]] = leftval0[j];
          left_idx_0[j]++;
        } else {
          op[out_idx_0[j]] = rightelm0[j];
          ovp[out_idx_0[j]] = rightval0[j];
          right_idx_0[j]++;
        }
        out_idx_0[j]++;
        if(left_idx_0[j] == left_idx_stop_0[j] ||
           right_idx_0[j] == right_idx_stop_0[j]) {
          valid_0[j] = false;
        }
      }
    }
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_1[j]) {
        if(leftelm1[j] >= rightelm1[j]) {
          op[out_idx_1[j]] = leftelm1[j];
          ovp[out_idx_1[j]] = leftval1[j];
          left_idx_1[j]++;
        } else {
          op[out_idx_1[j]] = rightelm1[j];
          ovp[out_idx_1[j]] = rightval1[j];
          right_idx_1[j]++;
        }
        out_idx_1[j]++;
        if(left_idx_1[j] == left_idx_stop_1[j] ||
           right_idx_1[j] == right_idx_stop_1[j]) {
          valid_1[j] = false;
        }
      }
    }
#pragma cdir nodep
#pragma _NEC ivdep
    for(int j = 0; j < SET_VLEN_EACH; j++) {
      if(valid_2[j]) {
        if(leftelm2[j] >= rightelm2[j]) {
          op[out_idx_2[j]] = leftelm2[j];
          ovp[out_idx_2[j]] = leftval2[j];
          left_idx_2[j]++;
        } else {
          op[out_idx_2[j]] = rightelm2[j];
          ovp[out_idx_2[j]] = rightval2[j];
          right_idx_2[j]++;
        }
        out_idx_2[j]++;
        if(left_idx_2[j] == left_idx_stop_2[j] ||
           right_idx_2[j] == right_idx_stop_2[j]) {
          valid_2[j] = false;
        }
      }
    }
    int any_valid = false;
    for(int i = 0; i < SET_VLEN_EACH; i++) {
      if(valid_0[i] || valid_1[i] || valid_2[i])
        any_valid = true;
    }
    if(any_valid == false) break;
  }
  for(size_t i = 0; i < SET_VLEN_EACH; i++) {
    out_idx[i] = out_idx_0[i];
    out_idx[SET_VLEN_EACH * 1 + i] = out_idx_1[i];
    out_idx[SET_VLEN_EACH * 2 + i] = out_idx_2[i];
    left_idx[i] = left_idx_0[i];
    left_idx[SET_VLEN_EACH * 1 + i] = left_idx_1[i];
    left_idx[SET_VLEN_EACH * 2 + i] = left_idx_2[i];
    right_idx[i] = right_idx_0[i];
    right_idx[SET_VLEN_EACH * 1 + i] = right_idx_1[i];
    right_idx[SET_VLEN_EACH * 2 + i] = right_idx_2[i];
  }
}

template <class T, class K>
void set_merge_pair_desc(const std::vector<T>& left,
                         const std::vector<K>& left_val,
                         const std::vector<T>& right,
                         const std::vector<K>& right_val,
                         std::vector<T>& out, std::vector<K>& out_val) {
                    
  int valid[SET_VLEN];
  for(int i = 0; i < SET_VLEN; i++) valid[i] = true;
  size_t left_size = left.size();
  size_t right_size = right.size();
  if(left_size == 0) {
    out = right;
    out_val = right_val;
    return;
  }
  if(right_size == 0) {
    out = left;
    out_val = left_val;
    return;
  }
  if(left_size != left_val.size() || right_size != right_val.size())
    throw std::runtime_error("sizes of key and value are not the same");
  size_t each = ceil_div(left_size, size_t(SET_VLEN));
  if(each % 2 == 0) each++;
  
  size_t left_idx[SET_VLEN];
  size_t right_idx[SET_VLEN];
  size_t left_idx_stop[SET_VLEN];
  size_t right_idx_stop[SET_VLEN];
  size_t out_idx[SET_VLEN];
  out.resize(left_size + right_size);
  out_val.resize(left_size + right_size);
  for(int i = 0; i < SET_VLEN; i++) {
    size_t pos = each * i;
    if(pos < left_size) {
      left_idx[i] = pos;
    } else {
      valid[i] = false;
      left_idx[i] = left_size;
    }
  }
  T left_start[SET_VLEN];
  for(size_t i = 0; i < SET_VLEN; i++) {
    if(valid[i]) left_start[i] = left[left_idx[i]];
    else left_start[i] = 0;
  }
  lower_bound_desc(right.data(), right_size, left_start, SET_VLEN, right_idx);
  right_idx[0] = 0;
  for(size_t i = 0; i < SET_VLEN; i++) {
    if(right_idx[i] == right_size) valid[i] = false;
    if(!valid[i]) right_idx[i] = right_size; // left is not valid
  }
  out_idx[0] = 0;
  for(int i = 1; i < SET_VLEN; i++) {
    out_idx[i] =
      (left_idx[i] - left_idx[i-1]) +
      (right_idx[i] - right_idx[i-1]) +
      out_idx[i-1];
  }
  for(int i = 0; i < SET_VLEN - 1; i++) {
    left_idx_stop[i] = left_idx[i + 1];
    right_idx_stop[i] = right_idx[i + 1];
  }
  left_idx_stop[SET_VLEN-1] = left_size;
  right_idx_stop[SET_VLEN-1] = right_size;
  for(int i = 0; i < SET_VLEN; i++) {
    if(right_idx[i] == right_idx_stop[i]) 
      valid[i] = false;
  }
  auto lp = left.data();
  auto lvp = left_val.data();
  auto rp = right.data();
  auto rvp = right_val.data();
  auto op = out.data();
  auto ovp = out_val.data();
  set_merge_pair_desc_vreg(lp, lvp, rp, rvp, op, ovp,
                           valid, left_idx, right_idx,
                           left_idx_stop, right_idx_stop,
                           out_idx);
  for(size_t i = 0; i < SET_VLEN; i++) {
    for(size_t j = 0; j < left_idx_stop[i] - left_idx[i]; j++) {
      op[out_idx[i] + j] = lp[left_idx[i] + j];
      ovp[out_idx[i] + j] = lvp[left_idx[i] + j];
    }
    for(size_t j = 0; j < right_idx_stop[i] - right_idx[i]; j++) {
      op[out_idx[i] + j] = rp[right_idx[i] + j];
      ovp[out_idx[i] + j] = rvp[right_idx[i] + j];
    }
  }
}

// TODO: modify to use vreg
template <class T>
std::vector<size_t> set_separate(const std::vector<T>& key) {
  size_t size = key.size();
  if(size == 0) {return std::vector<size_t>(1);} 
  int valid[SET_VLEN];
  for(int i = 0; i < SET_VLEN; i++) valid[i] = true;
  size_t each = ceil_div(size, size_t(SET_VLEN));
  if(each % 2 == 0) each++;
  size_t key_idx[SET_VLEN];
  size_t key_idx_stop[SET_VLEN];
  size_t out_idx[SET_VLEN];
  size_t out_idx_save[SET_VLEN];
  T current_key[SET_VLEN];
  std::vector<size_t> out;
  out.resize(size);
  size_t* outp = &out[0];
  const T* keyp = &key[0];
  if(size > 0) {
    key_idx[0] = 1;
    outp[0] = 0;
    out_idx[0] = 1;
    out_idx_save[0] = 0;
    current_key[0] = keyp[0];
  } else {
    valid[0] = false;
    key_idx[0] = size;
    out_idx[0] = size;
    out_idx_save[0] = size;
  }
  for(int i = 1; i < SET_VLEN; i++) {
    size_t pos = each * i;
    if(pos < size) {
      key_idx[i] = pos;
      out_idx[i] = pos;
      out_idx_save[i] = pos;
      current_key[i] = keyp[pos-1];
    } else {
      valid[i] = false;
      key_idx[i] = size;
      out_idx[i] = size;
      out_idx_save[i] = size;
    }
  }
  for(int i = 0; i < SET_VLEN - 1; i++) {
    key_idx_stop[i] = key_idx[i + 1];
  }
  key_idx_stop[SET_VLEN-1] = size;
  // idx 0 is manually advanced; need to be checked
  if(key_idx[0] == key_idx_stop[0]) valid[0] = false;
  size_t max_size = 0;
  for(int i = 0; i < SET_VLEN; i++) {
    auto current = key_idx_stop[i] - key_idx[i];
    if(max_size < current) max_size = current;
  }
  for(size_t j = 0; j < max_size; j++) {
#pragma cdir nodep
#pragma _NEC ivdep
    for(int i = 0; i < SET_VLEN; i++) {
      if(valid[i]) {
        auto keyval = keyp[key_idx[i]];
        if(keyval != current_key[i]) {
          outp[out_idx[i]++] = key_idx[i];
          current_key[i] = keyval;
        }
        key_idx[i]++;
        if(key_idx[i] == key_idx_stop[i]) {valid[i] = false;}
      }
    }
  }
  size_t total = 0;
  for(size_t i = 0; i < SET_VLEN; i++) {
    total += out_idx[i] - out_idx_save[i];
  }
  std::vector<size_t> ret(total+1);
  size_t* retp = &ret[0];
  size_t current = 0;
  for(size_t i = 0; i < SET_VLEN; i++) {
    for(size_t j = 0; j < out_idx[i] - out_idx_save[i]; j++) {
      retp[current + j] = out[out_idx_save[i] + j];
    }
    current += out_idx[i] - out_idx_save[i];
  }
  retp[current] = size;
  return ret;
}

template <class T>
std::vector<T> set_unique(const std::vector<T>& key) {
  size_t size = key.size();
  if(size == 0) {return std::vector<T>();} 
  int valid[SET_VLEN];
  for(int i = 0; i < SET_VLEN; i++) valid[i] = true;
  size_t each = ceil_div(size, size_t(SET_VLEN));
  if(each % 2 == 0) each++;
  size_t key_idx[SET_VLEN];
  size_t key_idx_stop[SET_VLEN];
  size_t out_idx[SET_VLEN];
  size_t out_idx_save[SET_VLEN];
  T current_key[SET_VLEN];
  std::vector<T> out;
  out.resize(size);
  T* outp = &out[0];
  const T* keyp = &key[0];
  if(size > 0) {
    key_idx[0] = 1;
    outp[0] = keyp[0];
    out_idx[0] = 1;
    out_idx_save[0] = 0;
    current_key[0] = keyp[0];
  } else {
    valid[0] = false;
    key_idx[0] = size;
    out_idx[0] = size;
    out_idx_save[0] = size;
  }
  for(int i = 1; i < SET_VLEN; i++) {
    size_t pos = each * i;
    if(pos < size) {
      key_idx[i] = pos;
      out_idx[i] = pos;
      out_idx_save[i] = pos;
      current_key[i] = keyp[pos-1];
    } else {
      valid[i] = false;
      key_idx[i] = size;
      out_idx[i] = size;
      out_idx_save[i] = size;
    }
  }
  for(int i = 0; i < SET_VLEN - 1; i++) {
    key_idx_stop[i] = key_idx[i + 1];
  }
  key_idx_stop[SET_VLEN-1] = size;
  // idx 0 is manually advanced; need to be checked
  if(key_idx[0] == key_idx_stop[0]) valid[0] = false; 
  size_t max_size = 0;
  for(int i = 0; i < SET_VLEN; i++) {
    auto current = key_idx_stop[i] - key_idx[i];
    if(max_size < current) max_size = current;
  }
  for(size_t j = 0; j < max_size; j++) {
#pragma cdir nodep
#pragma _NEC ivdep
    for(int i = 0; i < SET_VLEN; i++) {
      if(valid[i]) {
        auto keyval = keyp[key_idx[i]];
        if(keyval != current_key[i]) {
          outp[out_idx[i]++] = keyval;
          current_key[i] = keyval;
        }
        key_idx[i]++;
        if(key_idx[i] == key_idx_stop[i]) {valid[i] = false;}
      }
    }
  }
  size_t total = 0;
  for(size_t i = 0; i < SET_VLEN; i++) {
    total += out_idx[i] - out_idx_save[i];
  }
  std::vector<T> ret(total);
  T* retp = &ret[0];
  size_t current = 0;
  for(size_t i = 0; i < SET_VLEN; i++) {
#pragma cdir nodep
#pragma _NEC ivdep
    for(size_t j = 0; j < out_idx[i] - out_idx_save[i]; j++) {
      retp[current + j] = out[out_idx_save[i] + j];
    }
    current += out_idx[i] - out_idx_save[i];
  }
  return ret;
}

// TODO: modify to use alloc_on_vreg
template <class T>
int set_is_unique(const std::vector<T>& key) {
  size_t size = key.size();
  if(size == 0) return true;
  int valid[SET_VLEN];
  int is_unique[SET_VLEN];
  for(int i = 0; i < SET_VLEN; i++) {
    valid[i] = true;
    is_unique[i] = true;
  }
  size_t each = ceil_div(size, size_t(SET_VLEN));
  if(each % 2 == 0) each++;
  size_t key_idx[SET_VLEN];
  size_t key_idx_stop[SET_VLEN];
  T current_key[SET_VLEN];
  const T* keyp = &key[0];
  if(size > 0) {
    key_idx[0] = 1;
    current_key[0] = keyp[0];
  } else {
    valid[0] = false;
    key_idx[0] = size;
  }
  for(int i = 1; i < SET_VLEN; i++) {
    size_t pos = each * i;
    if(pos < size) {
      key_idx[i] = pos;
      current_key[i] = keyp[pos-1];
    } else {
      valid[i] = false;
      key_idx[i] = size;
    }
  }
  for(int i = 0; i < SET_VLEN - 1; i++) {
    key_idx_stop[i] = key_idx[i + 1];
  }
  key_idx_stop[SET_VLEN-1] = size;
  // idx 0 is manually advanced; need to be checked
  if(key_idx[0] == key_idx_stop[0]) valid[0] = false; 
  size_t max_size = 0;
  for(int i = 0; i < SET_VLEN; i++) {
    auto current = key_idx_stop[i] - key_idx[i];
    if(max_size < current) max_size = current;
  }
  int any_unique = true;
  for(size_t j = 0; j < max_size; j++) {
#pragma cdir nodep
#pragma _NEC ivdep
    for(int i = 0; i < SET_VLEN; i++) {
      if(valid[i]) {
        auto keyval = keyp[key_idx[i]];
        if(keyval != current_key[i]) current_key[i] = keyval;
        else is_unique[i] = false;
        key_idx[i]++;
        if(key_idx[i] == key_idx_stop[i]) {valid[i] = false;}
      }
    }
    for(int i = 0; i < SET_VLEN; i++) if(!is_unique[i]) any_unique = false;
    if(any_unique == false) break;
  }
  return any_unique;
}


#endif // _SX

template <class T, class K>
void set_multimerge_pair(std::vector<std::vector<T>>& key,
                         std::vector<std::vector<K>>& val,
                         std::vector<T>& out_key,
                         std::vector<K>& out_val) {
  size_t part_size = key.size();
  if(part_size == 1) {
    out_key.swap(key[0]);
    out_val.swap(val[0]);
  } else if(part_size == 2) {
    set_merge_pair(key[0], val[0], key[1], val[1], out_key, out_val);
  } else {
    size_t next_part_size = ceil_div(part_size, size_t(2));
    std::vector<std::vector<T>> key_left(next_part_size);
    std::vector<std::vector<K>> val_left(next_part_size);
    for(size_t i = 0; i < next_part_size; i++) {
      key_left[i].swap(key[i]);
      val_left[i].swap(val[i]);
    }
    std::vector<T> out_key_left;
    std::vector<K> out_val_left;
    set_multimerge_pair(key_left, val_left, out_key_left, out_val_left);
    std::vector<std::vector<T>> key_right(part_size - next_part_size);
    std::vector<std::vector<K>> val_right(part_size - next_part_size);
    for(size_t i = 0; i < part_size - next_part_size; i++) {
      key_right[i].swap(key[i+next_part_size]);
      val_right[i].swap(val[i+next_part_size]);
    }
    std::vector<T> out_key_right;
    std::vector<K> out_val_right;
    set_multimerge_pair(key_right, val_right, out_key_right, out_val_right);
    set_merge_pair(out_key_left, out_val_left, out_key_right, out_val_right,
                   out_key, out_val);
                   
  }
}

template <class T, class K>
void set_multimerge_pair_desc(std::vector<std::vector<T>>& key,
                              std::vector<std::vector<K>>& val,
                              std::vector<T>& out_key,
                              std::vector<K>& out_val) {
  size_t part_size = key.size();
  if(part_size == 1) {
    out_key.swap(key[0]);
    out_val.swap(val[0]);
  } else if(part_size == 2) {
    set_merge_pair_desc(key[0], val[0], key[1], val[1], out_key, out_val);
  } else {
    size_t next_part_size = ceil_div(part_size, size_t(2));
    std::vector<std::vector<T>> key_left(next_part_size);
    std::vector<std::vector<K>> val_left(next_part_size);
    for(size_t i = 0; i < next_part_size; i++) {
      key_left[i].swap(key[i]);
      val_left[i].swap(val[i]);
    }
    std::vector<T> out_key_left;
    std::vector<K> out_val_left;
    set_multimerge_pair_desc(key_left, val_left, out_key_left, out_val_left);
    std::vector<std::vector<T>> key_right(part_size - next_part_size);
    std::vector<std::vector<K>> val_right(part_size - next_part_size);
    for(size_t i = 0; i < part_size - next_part_size; i++) {
      key_right[i].swap(key[i+next_part_size]);
      val_right[i].swap(val[i+next_part_size]);
    }
    std::vector<T> out_key_right;
    std::vector<K> out_val_right;
    set_multimerge_pair_desc(key_right, val_right, out_key_right, 
                             out_val_right);
    set_merge_pair_desc(out_key_left, out_val_left, out_key_right, 
                        out_val_right, out_key, out_val);
  }
}


template <class T>
void
set_multimerge(std::vector<std::vector<T>>& key,  // destructive
               std::vector<T>& out) {
  auto way = key.size();
  auto step = 1;
  while(step < way) {
    for(size_t i = 0; i < way; i+=step*2) {
      if(i + step < way)
        key[i] = set_merge(key[i], key[i+step]);
    }
    step *= 2;
  }
  out.swap(key[0]);
}

template <class T>
void
set_multimerge_desc(std::vector<std::vector<T>>& key, 
                    std::vector<T>& out) {
  auto way = key.size();
  auto step = 1;
  while(step < way) {
    for(size_t i = 0; i < way; i+=step*2) {
      if(i + step < way)
        key[i] = set_merge_desc(key[i], key[i+step]);
    }
    step *= 2;
  }
  out.swap(key[0]);
}

}  
}
#endif
