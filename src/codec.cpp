//
// Created by neolee on 6/20/23.
//
#include "codec.h"

#include "fmt/core.h"
#include "fmt/format.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

std::regex PK_REG(R"(t_c(\S+)_pk(\d+))");
std::regex IDX_REG(R"(t_c(\S+)_idx(\S+)_(\S+)_(\d+))");

std::string Vecrocks::encode_vec(const float* vec, const int64_t& dim) {
  std::vector<float> float_vec{vec, vec + dim};
  json vec_json = json(float_vec);
  return vec_json.dump();
}

const float* Vecrocks::decode_vec(const std::string& vec) {
  auto vec_json = json::parse(vec).get<std::vector<float>>();
  auto float_array = new float[vec_json.size()];
  std::memcpy(float_array, vec_json.data(), vec_json.size() * sizeof(float));
  return float_array;
}

std::string Vecrocks::codec::encode_key(const std::string& collection,
                                        int64_t id) {
  std::string result = fmt::format("t_c{}_pk{}", collection, id);

  return result;
}
std::string Vecrocks::codec::encode_idx(const std::string& collection,
                                        const std::string& index, int64_t id,
                                        const float* vec, int64_t dim) {
  std::string result = fmt::format("t_c{}_idx{}_{}_{}", collection, index,
                                   encode_vec(vec, dim), id);
  return result;
}
std::string Vecrocks::codec::encode_value(const std::string& collection,
                                          int64_t id, const float* vec,
                                          int64_t dim, const std::string& tag) {
  std::vector<std::string> s;
  s.emplace_back(std::to_string(id));
  s.emplace_back(encode_vec(vec, dim));
  s.emplace_back(tag);
  json json;
  json["id"] = id;
  json["vec"] = encode_vec(vec, dim);
  json["tag"] = tag;

  std::string result = json.dump();
  return result;
}

std::tuple<const float*, int64_t> Vecrocks::codec::decode_idx(
    const std::string& str) {
  std::smatch match;
  if (std::regex_search(str, match, IDX_REG)) {
    auto collection_name = match[1];
    auto idx_name = match[2];
    auto idx_value = match[3];
    auto ref_id = match[4];
    return std::make_tuple(decode_vec(idx_value), std::stoll(ref_id));
  } else {
    return std::make_tuple(nullptr, -1);
  }
}

int64_t Vecrocks::codec::decode_id(const std::string& str) {
  std::smatch match;
  if (std::regex_search(str, match, PK_REG)) {
    auto collection_name = match[1];
    auto id = match[2];
    return std::stoll(id);
  }
}

std::tuple<const int64_t, const float*, const std::string>
Vecrocks::codec::decode_value(const std::string& str) {
  auto json = json::parse(str);
  return std::make_tuple(json["id"].get<int64_t>(), decode_vec(json["vec"]),
                         json["tag"]);
}
