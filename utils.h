#ifndef _UTIL_H_
#define _UTIL_H_
#include <string.h>
#include "paket_m.h"
#include "report_m.h"

Paket* generateNetworkPacket(ipaddr src, ipaddr dst, vIndex i, vSegment s, cValue v);
Paket* generateNewVideoRequest(ipaddr src, ipaddr dst, vIndex i);
Paket* generateNotFound(ipaddr src, ipaddr dst, vIndex i, vSegment s);
Paket* generatePrecachingHint(ipaddr src, ipaddr dst, vIndex i, vSegment s);
Paket* generateQuery(ipaddr src, ipaddr dst, vIndex i, vSegment s);
Paket* generateReply(ipaddr src, ipaddr dst, vIndex i, vSegment s, cValue v);
Report* generateReport(ipaddr src, ipaddr dst, Stats& stats);
Paket* generateSegmentHint(ipaddr src, ipaddr dst, vIndex i, vSegment s);
Paket* generateSegmentRequest(ipaddr src, ipaddr dst, vIndex i, vSegment s);
Paket* generateSessionCloseNotify(ipaddr src, ipaddr dst, vIndex i, vSegment s);
Paket* generateTearDownNotify(ipaddr src, ipaddr dst, vIndex i, vSegment s);
Paket* generateTermination(ipaddr src, ipaddr dst, vIndex i, vSegment s);
Paket* generateVideoInfo(ipaddr src, ipaddr dst, vIndex i, vSegment s, cValue v);
Paket* generateVideoOperation(ipaddr src, ipaddr dst, vIndex i, vSegment s, cValue v);
size_t fzipf();
size_t kzipf();

#endif  /* _UTIL_H_ */
