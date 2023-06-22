//
// Created by lisirrx on 2023/6/15.
//
#include "vecrocks.h"

#include <rocksdb/iterator.h>

#include <string>

#include "codec.h"
#include "faiss/impl/IDSelector.h"
#include "fmt/format.h"
#include "rocksdb/db.h"

const std::string DEFAULT_COLLECTION = "default";
const std::string DEFAULT_INDEX = "default";
const std::string DEFAULT_PREFIX =
    fmt::format("t_c{}_idx{}_", DEFAULT_COLLECTION, DEFAULT_INDEX);

Vecrocks::DB::DB(int64_t dim) {
  this->d = dim;
  this->index = new faiss::IndexIDMap(new faiss::IndexFlatL2(d));
}

int Vecrocks::DB::Init(const std::string& store_path) {
  rocksdb::Options options;
  options.create_if_missing = true;
  rocksdb::Status status = rocksdb::DB::Open(options, store_path, &this->kv);

  if (!status.ok()) {
    this->kv->Close();
    delete this->kv;
    this->kv = nullptr;
    return 1;
  }

  // load all data to build index.
  auto iter = this->kv->NewIterator(rocksdb::ReadOptions());
  iter->SeekToFirst();
  for (iter->Seek(DEFAULT_PREFIX);
       iter->Valid() && iter->key().starts_with(DEFAULT_PREFIX); iter->Next()) {
    auto [vec, id] = codec.decode_idx(iter->key().ToString());

    if (id != -1) {
      int64_t ids[1] = {id};
      this->index->add_with_ids(1, vec, ids);
    }
  }
  delete iter;

  return 0;
}

int Vecrocks::DB::Put(int64_t id, const float* vec, const std::string& tag) {
  // save data by pk
  auto status = this->kv->Put(
      rocksdb::WriteOptions(), codec.encode_key(DEFAULT_COLLECTION, id),
      codec.encode_value(DEFAULT_COLLECTION, id, vec, this->d, tag));

  if (!status.ok()) {
    return 1;
  }

  // save vector index
  status = this->kv->Put(
      rocksdb::WriteOptions(),
      codec.encode_idx(DEFAULT_COLLECTION, DEFAULT_INDEX, id, vec, this->d),
      nullptr);
  if (!status.ok()) {
    // todo rollback pk-data
    return 1;
  }

  int64_t ids[1] = {id};
  this->index->add_with_ids(1, vec, ids);

  return 0;
}

Vecrocks::RowResult* Vecrocks::DB::Get(int64_t id) {
  std::string value;
  this->kv->Get(rocksdb::ReadOptions(),
                codec.encode_key(DEFAULT_COLLECTION, id), &value);
  auto tuple = codec.decode_value(value);

  return new RowResult{this->d, id, get<1>(tuple), get<2>(tuple)};
}
std::vector<Vecrocks::RowResult> Vecrocks::DB::MultiGet(
    const std::vector<int64_t>& ids) {
  std::vector<rocksdb::Slice> keys;
  keys.reserve(ids.size());

  std::vector<std::string> encoded_keys;
  encoded_keys.reserve(ids.size());

  std::transform(ids.begin(), ids.end(), std::back_inserter(encoded_keys),
                 [this](const int64_t& id) {
                   return codec.encode_key(DEFAULT_COLLECTION, id);
                 });
  std::transform(encoded_keys.begin(), encoded_keys.end(),
                 std::back_inserter(keys),
                 [this](const std::string& encoded_key) {
                   return rocksdb::Slice{encoded_key};
                 });

  std::vector<std::string> values;
  this->kv->MultiGet(rocksdb::ReadOptions(), keys, &values);

  std::vector<Vecrocks::RowResult> result;
  result.reserve(values.size());
  for (const auto& v : values) {
    auto tuple = codec.decode_value(v);
    result.push_back(
        RowResult{this->d, get<0>(tuple), get<1>(tuple), get<2>(tuple)});
  }
  return result;
}
std::vector<Vecrocks::SearchResult> Vecrocks::DB::Search(float* vec, int k) {
  auto distances = new float[k];
  auto labels = new faiss::idx_t[k];
  this->index->search(1, vec, k, distances, labels);

  std::vector<Vecrocks::SearchResult> result;
  result.reserve(k);

  std::vector<int64_t> ids;
  ids.reserve(k);
  for (int i = 0; i < k; ++i) {
    ids.push_back(static_cast<int64_t>(labels[i]));
  }

  std::vector<RowResult> row_results = this->MultiGet(ids);
  for (int i = 0; i < k; ++i) {
    result.push_back(Vecrocks::SearchResult{distances[i], row_results[i]});
  }

  return result;
}

int Vecrocks::DB::Del(int64_t id) {
  this->kv->Delete(rocksdb::WriteOptions(),
                   codec.encode_key(DEFAULT_COLLECTION, id));
  int64_t ids[1] = {id};

  this->index->remove_ids(faiss::IDSelectorArray{1, ids});

  return 0;
}

Vecrocks::DB::~DB() {
  this->kv->Close();
  delete this->kv;
  delete this->index;
}
