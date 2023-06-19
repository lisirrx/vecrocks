#include <fmt/core.h>

#include <cassert>

#include "fmt/format.h"
#include "rocksdb/db.h"
#include "vecrocks.h"

void vecrocksTest() {
  const std::string PATH = "./testdb";
  const int DIM = 2;

  auto db = new Vecrocks::DB(DIM);
  db->Init(PATH);
  // Add 4 vectors to db
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

  // Get the (0, 1) vec
  assert(res[0] == 0.);
  assert(res[1] == 1.0);

  // find 2 the most similar vector to (0, 0)
  int k = 2;
  float q[2] = {0, 0};

  auto search_result = db_for_read->Search(q, k);

  // Get (0,1) and (0,-1)
  assert(search_result[0].id == 1);
  assert(search_result[1].id == 4);

  db_for_read->Get(1, res);
  assert(res[0] == 0.);
  assert(res[1] == 1.);

  db_for_read->Get(4, res);
  assert(res[0] == 0.);
  assert(res[1] == -1.);

  delete db_for_read;
}

int main() {
  vecrocksTest();

  return 0;
}