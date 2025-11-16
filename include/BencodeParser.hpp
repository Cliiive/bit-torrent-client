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

struct ParseResult {
    Value value;
    size_t consumed; // number of characters used from start
};

ParseResult parse(const std::string& data);

ParseResult parseInt(const std::string& data);
ParseResult parseString(const std::string& data);
ParseResult parseList(const std::string& data, size_t startPos = 0);
ParseResult parseDict(const std::string& data, size_t startPos = 0);

// Helper function to update position after parsing a value
// void _updatePosition(const Value& value, size_t& pos, const std::string& data);
bool _isValidBencodeInt(const std::string& s);
} // namespace bt::bencode