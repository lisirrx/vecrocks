//
// Created by lisirrx on 2023/6/16.
//

#pragma once

#include <string>

#include "faiss/IndexFlat.h"
#include "faiss/IndexIDMap.h"
#include "metadata.h"
#include "rocksdb/db.h"

namespace Vecrocks {

struct SearchResult {
  int64_t id;
  float distance;
  Row* row;
};

class DB {
 public:
  explicit DB(int64_t dim);

  int Init(const std::string& store_path);
  int Put(int64_t id, const float* vec);
  //  int Put(Collection& collection, int64_t id, const Row& row);
  void Get(int64_t id, float*& res);
  //  int Get(Collection& collection, int64_t id, Row& result);
  std::vector<SearchResult> Search(float* vec, int k);
  //  std::vector<SearchResult> Search(Collection& collection, Index& index,
  //  float* vec, int k);

  ~DB();

 private:
  int64_t d;
  rocksdb::DB* kv;
  faiss::IndexIDMap* index;
};

}  // namespace Vecrocks

