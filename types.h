#ifndef _TYPES_H
#define _TYPES_H
#include <functional>
#include <cstddef>

typedef unsigned int ipaddr;
typedef std::size_t vIndex;
typedef std::size_t vSegment;
typedef int cValue;
typedef std::size_t vSequence;

typedef std::pair<vIndex, vSegment> VideoSegment;
namespace std {
        template <>
        struct hash < VideoSegment >
        {
                size_t operator()(const VideoSegment& k) const {
                        return hash<vIndex>()(k.first) ^
                                hash<vSegment>()(k.second);
                }
        };
};
#endif /* _TYPES_H */
