//
// Created by lisirrx on 2023/6/16.
//
#include <gtest/gtest.h>

#include "rocksdb/db.h"
// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  rocksdb::DB *db;
  rocksdb::Options options;
  options.create_if_missing = true;
  rocksdb::Status status = rocksdb::DB::Open(options, "./testdb", &db);
  std::cout << status.ToString() << std::endl;
  assert(status.ok());
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}