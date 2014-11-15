cplusplus {{
#include <unordered_map>
#include "types.h"
typedef std::unordered_map<VideoSegment, std::size_t> Stats;
}}

class noncobject ipaddr;
class noncobject Stats;

packet Report {
       ipaddr src = 0;
       ipaddr dst = 0;
       Stats stats;
}