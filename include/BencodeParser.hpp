#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace bt::bencode {
struct List;
struct Dict;

using Value = std::variant<int64_t, std::string, List, Dict>;

struct List {
    std::vector<Value> values;
};

struct Dict {
    std::vector<std::pair<std::string, Value>> items;
};

Value parse(const std::string& data);

Value parseInt(const std::string& data);
Value parseString(const std::string& data);
Value parseList(const std::string& data);
Value parseDict(const std::string& data);

// Helper function to update position after parsing a value
void _updatePosition(const Value& value, size_t& pos, const std::string& data);
bool _isValidBencodeInt(const std::string& s);
} // namespace bt::bencode