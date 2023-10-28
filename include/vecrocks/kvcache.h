////
//// Created by neole on 11/2/2023.
////
//
//#ifndef DISKANN_KVCACHE_H
//#define DISKANN_KVCACHE_H
//
//#include <unordered_map>
//#include <string>
//class Cache {
// private:
//  std::unordered_map<std::string, std::string> m_cache;
//  std::unordered_map<std::string, bool> m_deleteList;
//
// public:
//  Cache() {}
//
//  std::string get(const std::string& key) {
//    if (m_deleteList.count(key)) {
//      return "";
//    }
//
//    return m_cache[key];
//  }
//
//  void put(const std::string& key, const std::string& value) {
//    m_cache[key] = value;
//    m_deleteList[key] = true;
//  }
//};
//#endif  // DISKANN_KVCACHE_H
