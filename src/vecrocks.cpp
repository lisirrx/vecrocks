//
// Created by lisirrx on 2023/6/15.
//
#include "vecrocks.h"

#include <rocksdb/iterator.h>

#include <cstdint>
#include <string>

#include "rocksdb/db.h"

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
  for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {
    int64_t id = std::stoll(iter->key().ToString());
    float* vec = (float*)iter->value().data();
    int64_t ids[1] = {id};
    this->index->add_with_ids(1, vec, ids);
  }
  delete iter;

  return 0;
}

int Vecrocks::DB::Put(int64_t id, const float* vec) {
  auto status =
      this->kv->Put(rocksdb::WriteOptions(), rocksdb::Slice(std::to_string(id)),
                    rocksdb::Slice((const char*)vec, sizeof(float) * this->d));

  if (!status.ok()) {
    return 1;
  }

  int64_t ids[1] = {id};
  this->index->add_with_ids(1, vec, ids);

  return 0;
}

void Vecrocks::DB::Get(int64_t id, float*& res) {
  std::string value;
  this->kv->Get(rocksdb::ReadOptions(), rocksdb::Slice(std::to_string(id)),
                &value);

  auto result = new float[this->d];
  memcpy(result, (float*)value.c_str(), this->d * sizeof(float));
  res = result;
}

std::vector<Vecrocks::SearchResult> Vecrocks::DB::Search(float* vec, int k) {
  auto distances = new float[k];
  auto labels = new faiss::idx_t[k];
  this->index->search(1, vec, k, distances, labels);

  std::vector<Vecrocks::SearchResult> result;
  result.reserve(k);

  for (int i = 0; i < k; ++i) {
    result.push_back(Vecrocks::SearchResult{labels[i], distances[i], nullptr});
  }

  return result;
}

Vecrocks::DB::~DB() {
  this->kv->Close();
  delete this->kv;
  delete this->index;
}
