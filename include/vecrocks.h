//
// Created by lisirrx on 2023/6/16.
//

#ifndef VECROCKS_VECROCKS_H
#define VECROCKS_VECROCKS_H

#include <string>

#include "../src/codec.h"
#include "faiss/IndexFlat.h"
#include "faiss/IndexIDMap.h"
#include "rocksdb/db.h"

namespace Vecrocks {
struct RowResult {
  int64_t dim;
  int64_t id;
  const float* vec;
  std::string tag;
};

struct SearchResult {
  float distance;
  RowResult rowResult;
};

/**
 * Assume that the DB only have one collection, and the table schema is : [id
 * int64, vec float*, tag string]
 */
class DB {
 public:
  explicit DB(int64_t dim);

  int Init(const std::string& store_path);

  int Put(int64_t id, const float* vec, const std::string& tag);

  RowResult* Get(int64_t id);

  std::vector<RowResult> MultiGet(const std::vector<int64_t>& id);

  std::vector<SearchResult> Search(float* vec, int k);

  int Del(int64_t id);

  ~DB();

 private:
  int64_t d;
  rocksdb::DB* kv;
  faiss::IndexIDMap* index;
  codec codec;
};

}  // namespace Vecrocks

#endif  // VECROCKS_VECROCKS_H
