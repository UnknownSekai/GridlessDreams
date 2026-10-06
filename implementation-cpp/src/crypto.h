#pragma once
#include "json.hpp"
#include <string>
#include <vector>

namespace crypto {

std::vector<uint8_t> encrypt(const nlohmann::json& data);
nlohmann::json decrypt(const std::string& ciphertext);

}
