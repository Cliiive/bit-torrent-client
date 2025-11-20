#include "TorrentMetadataLoader.hpp"
#include "spdlog/spdlog.h"

#include <string_view>

using namespace bt;
int main() {
    // Logger setup
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^%l%$] [thread %t] %v");

    spdlog::info("Main loop starts now");
    std::string_view path = "./build/debian-13.2.0-arm64-netinst.iso.torrent";

    try {
        auto torrentFile = core::parseTorrentData(path);
        spdlog::info("Torrent file constructed successfully.");
    } catch (const std::exception& e) {
        spdlog::critical("Error constructing torrent file: {}", e.what());
    }
    return 0;
}