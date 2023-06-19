//
// Created by neolee on 6/19/23.
//
#pragma once

#include <regex>
#include <string>

namespace Vecrocks {
std::string encodeVec(const float* vec, const int64_t& dim);
const float* decodeVec(const std::string& vec);

std::vector<std::string> split(const std::string& str,
                               const std::string& delimiter);

}  // namespace Vecrocks
