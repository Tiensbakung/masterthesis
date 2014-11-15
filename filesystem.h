#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_
#include <unordered_set>
#include <unordered_map>
#include "types.h"

// typedef int ipaddr;
// typedef std::size_t vIndex;

typedef std::unordered_set<ipaddr> ServerSet;

class FileSystem
{
private:
        std::unordered_map<vIndex, ServerSet> data;
        FileSystem(const char *fn = "data/videostorage");
        virtual ~FileSystem() {}
public:
        static const FileSystem& instance() {
                static FileSystem ref;
                return ref;
        }
        bool exists(ipaddr addr, vIndex i) const;
        const ServerSet& servers(vIndex i) const;
};

#endif  /* _FILESYSTEM_H_ */
