#include <faiss/IndexFlat.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <random>

#include "rocksdb/db.h"
#include "vecrocks.h"

// 64-bit int
using idx_t = faiss::idx_t;

std::string printArray(float* vec, int d) {
  std::string s{"[ "};
  for (int i = 0; i < d; ++i) {
    s += std::to_string(vec[i]);
    s += " ";
  }
  s += "]\n";
  return s;
}

void vecrocksTest() {
  const std::string PATH = "./testdb";
  const int DIM = 2;

  auto db = new Vecrocks::DB(DIM);
  db->Init(PATH);
  float a[2] = {0, 1.0};
  float b[2] = {0, 2.0};
  float c[2] = {0, -2.0};
  float d[2] = {0, -1.0};

  db->Put(1, a);
  db->Put(2, b);
  db->Put(3, c);
  db->Put(4, d);

  delete db;

  // open another db instance for reading
  auto db_for_read = new Vecrocks::DB(DIM);
  db_for_read->Init(PATH);

  float* res;
  db_for_read->Get(1, res);
  printArray(res, 2);

  int k = 2;
  float q[2] = {0, 0};

  auto search_result = db_for_read->Search(q, k);
  for (Vecrocks::SearchResult result : search_result) {
    db_for_read->Get(result.id, res);
    std::cout << result.id << " : " << printArray(res, DIM);
  }
}

int main() {
  // create the Collection
  vecrocksTest();
}