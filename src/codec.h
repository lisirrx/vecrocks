//
// Created by neolee on 6/20/23.
//

#ifndef VECROCKS_CODEC_H
#define VECROCKS_CODEC_H

#include <cstdint>
#include <regex>
#include <string>
#include <vector>

namespace Vecrocks {

std::string encode_vec(const float* vec, const int64_t& dim);
const float* decode_vec(const std::string& vec);

class codec {
 public:
  // t_c${collection_name}_pk${id}
  std::string encode_key(const std::string& collection, int64_t id);
  // t_c${collection_name}_idx${idx_name}_v${encoded_vec}_id
  std::string encode_idx(const std::string& collection,
                         const std::string& index, int64_t id, const float* vec,
                         int64_t dim);

  std::string encode_value(const std::string& collection, int64_t id,
                           const float* vec, int64_t dim,
                           const std::string& tag);

  std::tuple<const float*, int64_t> decode_idx(const std::string& str);
  int64_t decode_id(const std::string& str);
  std::tuple<const int64_t, const float*, const std::string> decode_value(
      const std::string& str);
};

}  // namespace Vecrocks

#endif  // VECROCKS_CODEC_H
