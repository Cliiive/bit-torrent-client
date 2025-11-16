#include <regex>
#include <stdexcept>

#include "BencodeParser.hpp"

namespace bt::bencode {
constexpr char INT_START = 'i';
constexpr char LIST_START = 'l';
constexpr char DICT_START = 'd';
constexpr char END = 'e';
constexpr char COLON = ':';

ParseResult parse(const std::string& data) {
    if (data.empty()) {
        throw std::invalid_argument("Empty data");
    }

    char firstChar = data[0];
    if (firstChar == INT_START) {
        return parseInt(data);
    } else if (std::isdigit(firstChar)) {
        return parseString(data);
    } else if (firstChar == LIST_START) {
        return parseList(data);
    } else if (firstChar == DICT_START) {
        return parseDict(data);
    } else {
        throw std::invalid_argument("Invalid bencode data");
    }
}

ParseResult parseInt(const std::string& data) {
    // get the number between 'i' and 'e'
    const size_t endPos = data.find(END, 1);
    if (endPos == std::string::npos) {
        throw std::invalid_argument("Invalid integer bencode");
    }
    const std::string intStr = std::string(data.substr(1, endPos - 1));

    if (!_isValidBencodeInt(intStr)) {
        throw std::invalid_argument("Invalid integer bencode");
    }
    return {std::stoll(intStr), endPos + 1};
}

ParseResult parseString(const std::string& data) {
    // get the length of the string
    const size_t colonPos = data.find(COLON);
    if (colonPos == std::string::npos) {
        throw std::invalid_argument("Invalid string bencode");
    }
    const std::string lenStr = std::string(data.substr(0, colonPos));
    size_t strLen = std::stoul(lenStr);

    size_t startPos = colonPos + 1;
    if (startPos + strLen > data.size()) {
        throw std::invalid_argument("String length exceeds data size");
    }

    return {std::string(data.substr(startPos, strLen)), startPos + strLen};
}

ParseResult parseDict(const std::string& data, size_t startPos) {
    if (data[startPos] != 'd') {
        throw std::invalid_argument("Expected dict");
    }

    size_t pos = startPos + 1;
    Dict dict;

    while (pos < data.size() && data[pos] != END) {
        ParseResult keyRes = parseString(data.substr(pos));
        if (!std::holds_alternative<std::string>(keyRes.value)) {
            throw std::invalid_argument("Dictionary keys must be strings");
        }
        std::string key = std::get<std::string>(keyRes.value);
        pos += keyRes.consumed;

        ParseResult valRes = parse(data.substr(pos));
        dict.items.emplace_back(key, valRes.value);
        pos += valRes.consumed;
    }

    if (pos >= data.size() || data[pos] != END) {
        throw std::invalid_argument("Unterminated dict");
    }

    return {dict, pos - startPos + 1};
}

ParseResult parseList(const std::string& data, size_t startPos) {
    if (data[startPos] != LIST_START) {
        throw std::invalid_argument("Expected list");
    }

    size_t pos = startPos + 1;
    List list;

    while (pos < data.size() && data[pos] != END) {
        ParseResult res = parse(data.substr(pos));
        list.values.push_back(res.value);
        pos += res.consumed; // advance exactly the number of chars parsed
    }

    if (pos >= data.size() || data[pos] != END) {
        throw std::invalid_argument("Unterminated list");
    }

    return {list, pos - startPos + 1}; // +1 for the ending 'e'
}

bool _isValidBencodeInt(const std::string& s) {
    static const std::regex re("^(0|-[1-9][0-9]*|[1-9][0-9]*)$");
    return std::regex_match(s, re);
}
} // namespace bt::bencode