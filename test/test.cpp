//
// Created by lisirrx on 2023/6/16.
//
#include <gtest/gtest.h>

#include "rocksdb/db.h"
#include "vecrocks.h"
using codec = Vecrocks::codec;

TEST(CodecEncodeKeyTest, BasicAssertions) {
  codec codec;
  auto key1 = codec.encode_key("Test", 1);
  assert(key1 == "t_cTest_pk1");

  key1 = codec.encode_key("", 1);
  assert(key1 == "t_c_pk1");
}

TEST(CodecIdxTest, BasicAssertions) {
  codec codec;
  float float_vec[] = {1.1, 2.2};
  auto idx1 = codec.encode_idx("Test", "Test", 1, float_vec, 2);

  auto res = codec.decode_idx(idx1);
  assert((get<0>(res))[0] == 1.1f);
  assert((get<0>(res))[1] == 2.2f);
  assert(get<1>(res) == 1);
}
