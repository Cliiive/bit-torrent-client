#include "TorrentFile.hpp"
#include <string_view>

using namespace bt;
int main() {
    std::string_view path = "/path/to/torrent";
    auto torrentFile = constructTorrentFile( path );
    return 0;
}