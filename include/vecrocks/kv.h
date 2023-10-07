//
// Created by dongdong on 9/28/23.
//

#pragma once

#include <string>
#include <unordered_map>
#include <memory>
namespace Vecrocks {
  class KvWrapper {
   public:
    virtual void init(std::string& path)= 0;
    virtual std::string get(std::string& key)= 0;

    virtual void put(const std::string& key, const std::string& value)= 0;

    virtual void printAll()= 0;

  };

  class MemoryKv : public  KvWrapper {
   public:
    MemoryKv(){
      // create a map
      _map = std::make_unique<std::map<std::string, std::string>>();
    }

   public:
    void init(std::string& path) override {
    }
    std::string get(std::string& key) override {
      return _map->at(key);
    }
    void put(const std::string& key, const std::string& value) override {
      _map->emplace(key, value);
    }
    void printAll() override {
      for (auto& kv : *_map) {
        diskann::cout << kv.first << " " << kv.second << std::endl;
      }
    }

    std::unique_ptr<std::map<std::string, std::string>> _map;
  };

  class RocksDBKv : public  KvWrapper {
    public:
    void init(std::string& path) override {
    }
    std::string get(std::string& key) override {
    }
    void put(const std::string& key, const  std::string& value) override {
    }
    void printAll() override {
    }

  };
}
