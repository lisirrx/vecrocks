//
// Created by dongdong on 9/28/23.
//
#include "v2/index_merger.h"
#include "vecrocks/id_generator.h"

int main(){
 Vecrocks::IdGenerator<uint64_t> idg("./db", 50);

  const int   n = 4;
  std::thread threads[n];
  for (int i = 0; i < n; ++i) {
    threads[i] = std::thread([&idg, i]() {
      for (int j = 0; j < 500; j++) {
        auto next = idg.next();
        std::cout << "Thread " << i + 1 << "- Next ID : " << next << std::endl;
      }
    });
  }

  for (int i = 0; i < n; ++i) {
    threads[i].join();
  }
}