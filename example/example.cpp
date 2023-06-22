#include <cassert>

#include "fmt/core.h"
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

  db->Put(1, a, "First");
  db->Put(2, b, "Second");
  db->Put(3, c, "Third");
  db->Put(4, d, "Forth");

  delete db;

  // open another db instance for reading
  auto db_for_read = new Vecrocks::DB(DIM);
  db_for_read->Init(PATH);

  auto res = db_for_read->Get(1);

  // Get the (0, 1) vec
  assert(res->vec[0] == 0.);
  assert(res->vec[1] == 1.0);

  // find 2 the most similar vector to (0, 0)
  int k = 2;
  float q[2] = {0, 0};

  auto search_result = db_for_read->Search(q, k);

  // Get (0,1) and (0,-1)
  assert(std::find_if(search_result.begin(), search_result.end(),
                      [](auto& res) { return res.rowResult.id == 1; }) !=
         search_result.end());
  assert(
      std::find_if(search_result.begin(), search_result.end(), [](auto& res) {
        return res.rowResult.id == 1;
      })->rowResult.tag == "First");
  assert(std::find_if(search_result.begin(), search_result.end(),
                      [](auto& res) { return res.rowResult.id == 4; }) !=
         search_result.end());
  assert(
      std::find_if(search_result.begin(), search_result.end(), [](auto& res) {
        return res.rowResult.id == 4;
      })->rowResult.tag == "Forth");

  res = db_for_read->Get(2);
  assert(res->vec[0] == 0.);
  assert(res->vec[1] == 2.);
  assert(res->tag == "Second");

  res = db_for_read->Get(3);
  assert(res->vec[0] == 0.);
  assert(res->vec[1] == -2.);
  assert(res->tag == "Third");

  db_for_read->Del(1);
  db_for_read->Del(4);

  search_result = db_for_read->Search(q, k);

  // Get (0,1) and (0,-1)
  assert(std::find_if(search_result.begin(), search_result.end(),
                      [](auto& res) { return res.rowResult.id == 2; }) !=
         search_result.end());
  assert(
      std::find_if(search_result.begin(), search_result.end(), [](auto& res) {
        return res.rowResult.id == 2;
      })->rowResult.tag == "Second");
  assert(std::find_if(search_result.begin(), search_result.end(),
                      [](auto& res) { return res.rowResult.id == 3; }) !=
         search_result.end());
  assert(
      std::find_if(search_result.begin(), search_result.end(), [](auto& res) {
        return res.rowResult.id == 3;
      })->rowResult.tag == "Third");

  delete db_for_read;
}

int main() {
  vecrocksTest();
}