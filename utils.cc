#include <cstdio>
#include <iostream>
#include "common.h"
#include "paket_m.h"
#include "report_m.h"
using namespace std;

Paket* _generatePaket(const char *name=NULL, ipaddr src=0, ipaddr dst=0,
                              vIndex i=0, vSegment s=0, cValue v=0,
                              int64 l=PACKETLENGTH)
{
        Paket *p = new Paket(name);
        p->setSrc(src);
        p->setDst(dst);
        p->setIndex(i);
        p->setSeg(s);
        p->setV(v);
        p->setByteLength(l);
        return p;
}

Paket* generateNetworkPacket(ipaddr src, ipaddr dst, vIndex i, vSegment s, cValue v)
{
        return _generatePaket("packet", src, dst, i, s, v);
}

Paket* generateNewVideoRequest(ipaddr src, ipaddr dst, vIndex i)
{
        return _generatePaket("new_video", src, dst, i);
}

Paket* generateNotFound(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        return _generatePaket("not_found", src, dst, i, s);
}

Paket* generatePrecachingHint(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        return _generatePaket("precaching_hint", src, dst, i, s);
}

Paket* generateQuery(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        return _generatePaket("query", src, dst, i, s);
}

Paket* generateReply(ipaddr src, ipaddr dst, vIndex i, vSegment s, cValue v)
{
        return _generatePaket("reply", src, dst, i, s, v);
}

Report* generateReport(ipaddr src, ipaddr dst, Stats& stats)
{
        Report *report = new Report("report");
        report->setSrc(src);
        report->setDst(dst);
        report->getStats().swap(stats);
        return report;
}

Paket* generateSegmentHint(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        return _generatePaket("segment_hint", src, dst, i, s);
}

Paket* generateSegmentRequest(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        return _generatePaket("request", src, dst, i, s);
}

Paket* generateSessionCloseNotify(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        return _generatePaket("close", src, dst, i, s);
}

Paket* generateTearDownNotify(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        return _generatePaket("teardown", src, dst, i, s);
}

Paket* generateTermination(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        return _generatePaket("termination", src, dst, i, s);
}

Paket* generateVideoInfo(ipaddr src, ipaddr dst, vIndex i, vSegment s, cValue v)
{
        return _generatePaket("video_info", src, dst, i, s, v);
}

Paket* generateVideoOperation(ipaddr src, ipaddr dst, vIndex i, vSegment s, cValue v)
{
        return _generatePaket("operation", src, dst, i, s, v);
}
