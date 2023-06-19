//
// Created by neolee on 6/19/23.
//

#pragma once

#include "metadata.h"

namespace Vecrocks {

class codec {
 private:
  int64_t dim;

 public:
  // t_c${collection_name}_pk${id}
  std::string encode_key(const Vecrocks::Collection& collection, int64_t id);
  // t_c${collection_name}_idx${idx_name}_v${encoded_vec}_id
  std::string encode_idx(const Vecrocks::Collection& collection,
                         const Index& index, int64_t id, const Field& field);

  std::string encode_value(const Vecrocks::Collection& collection,
                           const Vecrocks::Row& row);

  std::tuple<Field*, int64_t> decode_idx(const std::string& str);
  int64_t decode_id(const std::string& str);
  Row* decode_value(const Vecrocks::Collection& collection,
                    const std::string& str);
};

}  // namespace Vecrocks
