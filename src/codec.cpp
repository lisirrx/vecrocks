//
// Created by neolee on 6/19/23.
//

#include "codec.h"

#include <regex>
#include <string_view>

#include "fmt/core.h"
#include "fmt/format.h"

// t_c${collection_name}_pk${id}

std::regex PK_REG(R"(t_c(\S+)_pk(\d+))");
std::regex IDX_REG(R"(t_c(\S+)_idx(\S+)_([v|f|i|s])(\S+)_(\d+))");
const std::string VALUE_SEPARATOR = "\u0001";

std::string Vecrocks::codec::encode_key(const Vecrocks::Collection& collection,
                                        int64_t id) {
  return fmt::format("t_c{}_pk{}", collection.name(), id);
}

// t_c${collection_name}_idx${idx_name}_${encoded_vec}_id
std::string Vecrocks::codec::encode_idx(const Vecrocks::Collection& collection,
                                        const Index& index, int64_t id,
                                        const Field& field) {
  return fmt::format("t_c{}_idx{}{}_{}_{}", collection.name(), index.name(),
                     field_type_to_string(field.field_type()), field.value(),
                     id);
}

std::string Vecrocks::codec::encode_value(
    const Vecrocks::Collection& collection, const Vecrocks::Row& row) {
  std::vector<std::string_view> s;
  for (const auto& field : row.fields()) {
    s.push_back(field.value());
  }

  return fmt::format("{}", fmt::join(s, VALUE_SEPARATOR));
}
std::tuple<Vecrocks::Field*, int64_t> Vecrocks::codec::decode_idx(
    const std::string& str) {
  std::smatch match;
  if (std::regex_search(str, match, IDX_REG)) {
    auto collection_name = match[1];
    auto idx_name = match[2];
    auto idx_type = match[3];
    auto idx_value = match[4];
    auto ref_id = match[5];
    return std::make_tuple(
        new Field{FieldMeta{idx_name, string_to_field_type(idx_type.str()[0])},
                  idx_value},
        std::stoll(ref_id));
  }
}
int64_t Vecrocks::codec::decode_id(const std::string& str) {
  std::smatch match;
  if (std::regex_search(str, match, IDX_REG)) {
    auto collection_name = match[1];
    auto id = match[2];
    return std::stoll(id);
  }
}

Vecrocks::Row* Vecrocks::codec::decode_value(
    const Vecrocks::Collection& collection, const std::string& str) {
  auto field_metas = collection.fields();
  std::vector<Field> fields;
  auto split_res = split(str, VALUE_SEPARATOR);
  for (int i = 0; i < split_res.size(); ++i) {
    fields.push_back(Field{field_metas[i], split_res[i]});
  }

  return new Row{fields};
}
