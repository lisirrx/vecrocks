//
// Created by neolee on 6/19/23.
//
#include "utils.h"

#include "metadata.h"
std::string encodeVec(const float* vec, const int64_t& dim) {
  const char* c = reinterpret_cast<const char*>(vec);
  return std::string{c, c + sizeof(float) * dim};
}

const float* decodeVec(const std::string& vec) {
  const auto* c = reinterpret_cast<const float*>(vec.c_str());
  return c;
}

std::vector<std::string> split(const std::string& str,
                               const std::string& delimiter) {
  std::regex ws_re(delimiter);  // whitespace

  std::vector<std::string> v(
      std::sregex_token_iterator(str.begin(), str.end(), ws_re, -1),
      std::sregex_token_iterator());
  return v;
}

std::string field_type_to_string(const Vecrocks::FieldType& fieldType) {
  switch (fieldType) {
    case Vecrocks::FieldType::VECTOR:
      return "v";
    case Vecrocks::FieldType::INT64:
      return "i";
    case Vecrocks::FieldType::FLOAT:
      return "f";
    case Vecrocks::FieldType::STRING:
      return "s";
  }
}

Vecrocks::FieldType string_to_field_type(const char& fieldType) {
  switch (fieldType) {
    case 'v':
      return Vecrocks::FieldType::VECTOR;
    case 'i':
      return Vecrocks::FieldType::INT64;
    case 'f':
      return Vecrocks::FieldType::FLOAT;
    case 's':
      return Vecrocks::FieldType::STRING;
    default:
      return Vecrocks::FieldType::STRING;
  }
}