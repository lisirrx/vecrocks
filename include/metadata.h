//
// Created by neolee on 6/19/23.
//

#pragma once

#include <string>
#include <vector>

#include "utils.h"

namespace Vecrocks {
enum class FieldType {
  VECTOR,
  INT64,
  FLOAT,
  STRING

};

std::string field_type_to_string(const FieldType& fieldType);

FieldType string_to_field_type(const char& fieldType);

enum class IndexType { FLAT, IVF_FLAT };

enum class IndexMetric { L2 };

class Field;

class FieldMeta {
 private:
  std::string _name;
  FieldType _type;
  int64_t vec_dim = 0;

 public:
  FieldMeta(const std::string& name, FieldType type, int64_t vecDim)
      : _name(name), _type(type), vec_dim(vecDim) {}
  FieldMeta(const std::string& name, FieldType type)
      : _name(name), _type(type) {}
  const std::string& name() const { return _name; }

  friend Field;
};

class Field {
 private:
  FieldMeta meta;
  std::string _value;

 public:
  Field(const FieldMeta& meta, const std::string& value)
      : meta(meta), _value(value) {}

  const std::string& name() const { return meta._name; }
  const std::string& value() const { return _value; }
  const FieldType& field_type() const { return meta._type; }
};

class Row {
 private:
  std::vector<Field> _fields;

 public:
  explicit Row(const std::vector<Field>& fields) : _fields(fields) {}
  const std::vector<Field> fields() const { return _fields; }
};
class Index {
 private:
  FieldMeta* index_field;
  IndexType index_type;
  IndexMetric index_metric;

 public:
  const std::string& name() const { return index_field->name(); }
};

class Collection {
 private:
  std::string _name;
  std::vector<FieldMeta> _fields;
  std::vector<Index> _indexes;

 public:
  const std::string& name() const { return _name; }
  const std::vector<FieldMeta>& fields() const { return _fields; }
};

}  // namespace Vecrocks
