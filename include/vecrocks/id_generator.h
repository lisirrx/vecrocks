//
// Created by dongdong on 9/28/23.
//

#pragma once

#include <string>
#include <mutex>
#include <atomic>
#include <iostream>
#include <fstream>

namespace Vecrocks {

  template<typename TagT>
  class IdGenerator {
   public:
    IdGenerator(const std::string& working_path, TagT step) {
      max_id_file = working_path + "/max_id";
      _step = step;
      std::ifstream file(max_id_file);
      if (file.is_open()) {
        file >> current_max_range;
        std::cout << "Read from max_id file " << max_id_file << " "
                  << current_max_range << std::endl;
        current_id = current_max_range + 1;
      } else {
        current_max_range = step;
        current_id.store(1);

        std::ofstream out(max_id_file);
        out << current_max_range;

      }
    }

    TagT next() {
      TagT id = current_id.fetch_add(1);

      if (id > current_max_range) {
        std::lock_guard<std::mutex> lock(mutex);
        if (id > current_max_range) {
          current_max_range += _step;
          std::ofstream file(max_id_file);
          // todo @lh keep out in memory
          if (file.is_open()) {
            file << current_max_range;
            file.close();
          }
        }
      }
      return id;
    }

   private:
    std::atomic<TagT> current_id;
    TagT              current_max_range;
    TagT              _step;
    std::string       max_id_file;
    std::mutex        mutex;
  };

}