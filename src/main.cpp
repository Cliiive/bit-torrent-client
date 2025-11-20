#include "TorrentMetadataLoader.hpp"
#include "spdlog/spdlog.h"

#include <string_view>

using namespace bt;
int main() {
    // Logger setup
    spdlog::set_level( spdlog::level::debug );
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^-%L-%$] [thread %t] %v");

    spdlog::info( "Main loop starts now" );
    std::string_view path = "./build/torrentFile";

    try {
    auto torrentFile = core::constructTorrentFile( path );
    spdlog::info( "Torrent file constructed successfully." );
    } catch ( const std::exception& e ) {
        spdlog::error( "Error constructing torrent file: {}", e.what() );
    }
    return 0;
}