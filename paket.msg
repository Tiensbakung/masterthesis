cplusplus {{
#include "types.h"
}}

class noncobject ipaddr;
class noncobject vIndex;
class noncobject vSegment;
class noncobject cValue;

packet Paket {
       ipaddr src = 0;
       ipaddr dst = 0;
       vIndex index = 0;
       vSegment seg = 0;
       cValue v = 0;
}