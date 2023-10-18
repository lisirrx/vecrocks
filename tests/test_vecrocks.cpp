//
// Created by dongdong on 9/15/23.
//
#include "vecrocks/vecrocks.h"

int main(){

  auto prefix = "edge_1";
  std::map<std::string, std::string> map{{"edge_1_1", "1"}, {"edge_2_1", "2"}};
  for (auto& kv : map) {
    if (kv.first.find(prefix) == 0) {
      std::cout << kv.first << std::endl;
    }
  }

//
//  Vecrocks::Options options;
//
//  options.dim = 2;
//
//  // mem index
//  options.max_record = 10;
//
//  options.skip_disk_search = true;
//  Vecrocks::DB<float, uint32_t> *db;
//
//  Vecrocks::DB<float, uint32_t>::Open(options, "./db", &db);
//
//  float a[2] {1.0, 0.0};
//  float b[2] {1.0, 1.0};
//
//  db->insert(a);
//  db->insert(b);
//
//  float q[2] {0.0, 0.0};
//  uint32_t tag[10];
//  float distance[10];
//  db->search(q, 1, tag, distance);
//
//  printf("%f\n", distance[0]);
//  printf("%ld\n", tag[0]);

}