//
// Created by dongdong on 9/28/23.
//

#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "options.h"
#include "rocksdb/slice_transform.h"
namespace Vecrocks {
  class KvWrapper {
   public:
    virtual int        init(std::string& path) = 0;
    virtual std::string get(const std::string& key) = 0;

    virtual int put(const std::string& key, const std::string& value) = 0;

    virtual void prefix_scan(
        const std::string&                                prefix,
        std::vector<std::pair<std::string, std::string>>& res) = 0;

    virtual void printAll() = 0;

    virtual void build_cache() = 0;
  };

  class MemoryKv : public KvWrapper {
   public:
    MemoryKv() {
      // create a map
      _map = std::make_unique<std::map<std::string, std::string>>();
      _cache = std::make_unique<std::map<std::string, std::vector<std::pair<std::string, std::string>>>>();

    }

   public:
    int init(std::string& path) override {
    }
    std::string get(const std::string& key) override {
      return _map->at(key);
    }
    int put(const std::string& key, const std::string& value) override {
      _map->emplace(key, value);
      return 0;
    }
    void printAll() override {
      for (auto& kv : *_map) {
        diskann::cout << kv.first << " " << kv.second << std::endl;
      }
    }
    void build_cache() override {
      auto start = std::chrono::high_resolution_clock::now();

      for (auto& kv : *_map) {
        size_t pos = kv.first.find_last_of('_');
        if (pos != std::string::npos) {
          std::string prefix = kv.first.substr(0, pos + 1);

          if (_cache->count(prefix) != 0) {
            auto res = _cache->at(prefix);
            res.emplace_back(kv.first, kv.second);
          } else {
            std::vector<std::pair<std::string, std::string>> res;
            res.emplace_back(kv.first, kv.second);
            _cache->emplace(prefix, res);
          }
        }
      }

      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> elapsed = end - start;
      std::cout << "Build Cache time: " << elapsed.count() << " ms\n";
    }
    void prefix_scan(const std::string&                                prefix,
                     std::vector<std::pair<std::string, std::string>>& res) {
      auto start = std::chrono::high_resolution_clock::now();

      if (_cache->count(prefix) != 0) {
        res = _cache->at(prefix);
        return;
      } else {
        for (auto& kv : *_map) {
          if (kv.first.find(prefix) == 0) {
            res.emplace_back(kv.first, kv.second);
          }
        }
        _cache->emplace(prefix, res);
      }

      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> elapsed = end - start;
      std::cout << "Prefix Scan time: " << elapsed.count() << " ms\n";



    }

    std::unique_ptr<std::map<std::string, std::string>> _map;
    std::unique_ptr<std::map<std::string, std::vector<std::pair<std::string, std::string>>>> _cache;
  };

  class RocksDBKv : public KvWrapper {
   public:
    int init(std::string& path) override {
      rocksdb::Options options;
      options.create_if_missing = true;
      rocksdb::Status status = rocksdb::DB::Open(options, path, &this->kv);

      if (!status.ok()) {
        this->kv->Close();
        delete this->kv;
        this->kv = nullptr;
        return 1;
      }
      return 0;
    }
    std::string get(const std::string& key) override {
      std::string value;
      this->kv->Get(rocksdb::ReadOptions(), key,
                    &value);
      return value;
    }
    int put(const std::string& key, const std::string& value) override {
      auto status =
          this->kv->Put(rocksdb::WriteOptions(),key,value);

      if (!status.ok()) {
        return 1;
      }
    }
    void printAll() override {
    }
    void prefix_scan(
        const std::string&                                prefix,
        std::vector<std::pair<std::string, std::string>>& res) override {
      rocksdb::Options options;


      options.prefix_extractor.reset(rocksdb::NewCappedPrefixTransform(prefix.size()));

      // load all data to build index.
      auto iter = this->kv->NewIterator(rocksdb::ReadOptions());
      for (iter->Seek(prefix); iter->Valid(); iter->Next()) {
        std::string key = iter->key().ToString();
        if (key.find(prefix) != 0) {
          break;
        }
        res.emplace_back(std::make_pair(key, iter->value().ToString()));
      }
    }
    void build_cache() override {
    }

   private:
    rocksdb::DB* kv;

  };
}  // namespace Vecrocks
