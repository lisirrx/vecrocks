//
// Created by lisirrx on 2023/6/16.
//

#ifndef VECROCKS_VECROCKS_H
#define VECROCKS_VECROCKS_H

#include <string>

#include "faiss/IndexFlat.h"
#include "faiss/IndexIDMap.h"
#include "rocksdb/db.h"

namespace Vecrocks {

struct SearchResult {
  int64_t id;
  float distance;
  float* vec;
};

class DB {
 public:
  explicit DB(int64_t dim);

  int Init(const std::string& store_path);

  int Put(int64_t id, const float* vec);

  void Get(int64_t id, float*& res);

  std::vector<SearchResult> Search(float* vec, int k);

  ~DB();

 private:
  int64_t d;
  rocksdb::DB* kv;
  faiss::IndexIDMap* index;
};

}  // namespace Vecrocks

#endif  // VECROCKS_VECROCKS_H
