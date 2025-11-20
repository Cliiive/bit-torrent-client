#include <spdlog/spdlog.h>
#include <string>

#include "TorrentMetadataLoader.hpp"

namespace bt::core {
const TorrentFile constructTorrentFile( std::string_view& path ) {
    std::string torrentData{};
    try {
        torrentData = detail::loadTorrentFile( path );
    }
    catch ( const std::exception& e ) {
        spdlog::error( "Error loading torrent file: {}", e.what() );
        throw;
    }
    
    spdlog::debug( "Torrent file data: {}", torrentData );
}

namespace detail {
std::string loadTorrentFile(std::string_view& path) {
    spdlog::info( "Loading torrent file from path: {}", path );
    
    // get a file descriptor
    FILE* file = fopen( std::string( path ).c_str(), "rb" );
    if ( !file ) {
        throw std::runtime_error( "Failed to open torrent file" );
    }

    // seek to end to get file size
    fseek( file, 0, SEEK_END );
    long fileSize = ftell( file );
    fseek( file, 0, SEEK_SET );

    // read file contents
    std::vector<uint8_t> fileData( fileSize );
    size_t bytesRead = fread( fileData.data(), 1, fileSize, file );
    if ( bytesRead != static_cast<size_t>( fileSize ) ) {
        spdlog::error( "Failed to read entire torrent file: {}", path );
        fclose( file );
        throw std::runtime_error( "Failed to read entire torrent file" );
    }
    fclose( file );

    return std::string( reinterpret_cast<const char*>( fileData.data() ), fileData.size() );
}
}
}