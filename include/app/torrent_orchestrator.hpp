#include "core/torrent_metadata_loader.hpp"
#include <string>

using namespace bt;
class TorrentOrchestrator {
private:
    core::TorrentMetadata _metadata;
    core::TorrentMetadata _loadMetadata(const std::string_view path) const;

    // PeerManager
    // PiecesManager
    //...

public:
    TorrentOrchestrator(std::string path);
    void start();
};