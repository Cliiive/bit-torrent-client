cmake_minimum_required(VERSION 3.16)
project(bit_torrent_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/doctest)

# Main executable
add_executable(bit-torrent-client
    src/main.cpp
    src/TorrentFile.cpp
    src/Handshake.cpp
    src/Peer.cpp
    src/BencodeParser.cpp
)

# Bencode parser tests
add_executable(bt-bencode-tests
    tests/BencodeParserTests.cpp
    src/BencodeParser.cpp
)
