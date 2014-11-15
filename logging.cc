#include <iostream>
#include "logging.h"
#include "common.h"
#include "paket_m.h"
#include "iptools.h"
using namespace std;
#define VS(i,s) std::to_string(i) + ":" + std::to_string(s)
#define VSS(i,s,v) VS(i,s) + ":" + std::to_string(v)

void _log(LogCode code, ipaddr src, ipaddr dst, const char* msg)
{
        switch (code) {
        case LOG_CLN: cout << "[CLN]"; break;
        case LOG_ERR: cout << "[ERR]"; break;
        case LOG_HHC: cout << "[HHC]"; break;
        case LOG_HHD: cout << "[HHD]"; break;
        case LOG_HNH: cout << "[HNH]"; break;
        case LOG_NCS: cout << "[NCS]"; break;
        case LOG_NVI: cout << "[NVI]"; break;
        case LOG_NFD: cout << "[NFD]"; break;
        case LOG_OHC: cout << "[OHC]"; break;
        case LOG_OHD: cout << "[OHD]"; break;
        case LOG_ONH: cout << "[ONH]"; break;
        case LOG_OPE: cout << "[OPE]"; break;
        case LOG_PCH: cout << "[PCH]"; break;
        case LOG_PKT: cout << "[PKT]"; break;
        case LOG_QLN: cout << "[QLN]"; break;
        case LOG_QRY: cout << "[QRY]"; break;
        case LOG_RCM: cout << "[RCM]"; break;
        case LOG_REP: cout << "[REP]"; break;
        case LOG_REQ: cout << "[REQ]"; break;
        case LOG_RHC: cout << "[RHC]"; break;
        case LOG_RHD: cout << "[RHD]"; break;
        case LOG_RNH: cout << "[RNH]"; break;
        case LOG_RST: cout << "[RST]"; break;
        case LOG_SPH: cout << "[SPH]"; break;
        case LOG_SPM: cout << "[SPM]"; break;
        case LOG_SSH: cout << "[SSH]"; break;
        case LOG_STP: cout << "[STP]"; break;
        case LOG_TDN: cout << "[TDN]"; break;
        case LOG_TEM: cout << "[TEM]"; break;
        }
        cout << " <" << IP(src) << ", " << IP(dst) << "> @"<< simTime()
           << "s: " << msg << endl;
}

void _log(LogCode code, ipaddr src, ipaddr dst, const string& msg)
{
        _log(code, src, dst, msg.c_str());
}

// void _log_buffer(ipaddr src, ipaddr dst, vIndex i, vSegment s)
// {
//         ostringstream oss;
//         oss << "Buffer in advance video segment: " << VS(i, s);
//         _log(LOG_BUF, src, dst, oss.str());
// }

void _log_cln(ipaddr src, ipaddr dst, size_t l)
{
        ostringstream oss;
        oss << "Cache size: " << l;
        _log(LOG_CLN, src, dst, oss.str());
}

void _log_hhc(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << VS(i, s) << " hits cache.";
        _log(LOG_HHC, src, dst, oss.str());
}

void _log_hhd(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << VS(i, s) << " hits disk.";
        _log(LOG_HHD, src, dst, oss.str());
}

void _log_hnh(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << VS(i, s) << " doesn't hit.";
        _log(LOG_HNH, src, dst, oss.str());
}

void _log_ncs(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << "Cached new segment " << VS(i, s);
        _log(LOG_NCS, src, dst, oss.str());
}

void _log_newvideo(ipaddr src, ipaddr dst, vIndex i, vSegment s, ipaddr p)
{
        ostringstream oss;
        oss << "New video " << VS(i,s) << "@" << IP(p);
        _log(LOG_NVI, src, dst, oss.str());
}

void _log_notfound(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << "Queried segment " << VS(i,s) << " not found";
        _log(LOG_NFD, src, dst, oss.str());
}

void _log_ohc(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << VS(i, s) << " hits cache.";
        _log(LOG_OHC, src, dst, oss.str());
}

void _log_ohd(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << VS(i, s) << " hits disk.";
        _log(LOG_OHD, src, dst, oss.str());
}

void _log_onh(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << VS(i, s) << " doesn't hit.";
        _log(LOG_ONH, src, dst, oss.str());
}

void _log_operation(ipaddr src, ipaddr dst, vIndex i, vSequence v)
{
        ostringstream oss;
        oss << "Received operation ";
        if (v == VIDEO_DROP)
                oss << "DROP for " << i;
        else if (v == VIDEO_LIFT)
                oss << "LIFT for " << i;
        _log(LOG_OPE, src, dst, oss.str());
}

void _log_precachinghint(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << "Received hint for " << VS(i, s);
        _log(LOG_PCH, src, dst, oss.str());
}

void _log_packet(ipaddr src, ipaddr dst, vIndex i, vSegment s, vSequence v)
{
        ostringstream oss;
        oss << "Receive packet " << VSS(i, s, v);
        _log(LOG_PKT, src, dst, oss.str());
}

void _log_queuelength(ipaddr addr, int index, size_t l)
{
        ostringstream oss;
        oss << "Queue length: " << l;
        _log(LOG_QLN, addr, index, oss.str());
}

void _log_query(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << "Query segment " << VS(i, s);
        _log(LOG_QRY, src, dst, oss.str());
}

void _log_rcm(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << VS(i, s) << " replacement cache miss.";
        _log(LOG_RCM, src, dst, oss.str());
}

void _log_reply(ipaddr src, ipaddr dst, vIndex i, vSegment s, ipaddr p)
{
        ostringstream oss;
        oss << "Segment " << VS(i,s) << " found in " + IP(p);
        _log(LOG_REP, src, dst, oss.str());
}

void _log_request(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << "Request for segment " << VS(i,s);
        _log(LOG_REQ, src, dst, oss.str());
}

void _log_reset(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << "Connection " << VS(i, s) << " resets.";
        _log(LOG_RST, src, dst, oss.str());
}

void _log_rhc(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << "Request " << VS(i, s) << " hits cache.";
        _log(LOG_RHC, src, dst, oss.str());
}

void _log_rhd(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << "Request " << VS(i, s) << " hits disk.";
        _log(LOG_RHD, src, dst, oss.str());
}

void _log_rnh(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << "Request " << VS(i, s) << " doesn't hit.";
        _log(LOG_RNH, src, dst, oss.str());
}

void _log_setup(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << "Setup connection for video " << VS(i, s);
        _log(LOG_STP, src, dst, oss.str());
}

void _log_sph(ipaddr src, ipaddr dst, vIndex i, vSegment s, cValue v)
{
        ostringstream oss;
        oss << VSS(i, s, v) << " paket hit.";
        _log(LOG_SPH, src, dst, oss.str());
}

void _log_spm(ipaddr src, ipaddr dst, vIndex i, vSegment s, cValue v)
{
        ostringstream oss;
        oss << VSS(i, s, v) << " paket miss.";
        _log(LOG_SPM, src, dst, oss.str());
}

void _log_ssh(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << "Send segment hint: " << VS(i, s) ;
        _log(LOG_SSH, src, dst, oss.str());
}

void _log_teardown(ipaddr src, ipaddr dst, vIndex i, vSegment s)
{
        ostringstream oss;
        oss << "Tear down connection for video " << VS(i,s);
        _log(LOG_TDN, src, dst, oss.str());
}

void _log_termination(ipaddr src, ipaddr dst, vIndex i, vSegment s, vSequence v)
{
        ostringstream oss;
        oss << "Terminate video " << VS(i,s) << " at " << v;
        _log(LOG_TEM, src, dst, oss.str());
}

void logging(LogCode code, ipaddr src, ipaddr dst, vIndex i, vSegment s, vSequence v)
{
        switch (code) {
        case LOG_CLN: _log_cln(src, dst, i); break;
        case LOG_HHC: _log_hhc(src, dst, i, s); break;
        case LOG_HHD: _log_hhd(src, dst, i, s); break;
        case LOG_HNH: _log_hnh(src, dst, i, s); break;
        case LOG_NCS: _log_ncs(src, dst, i, s); break;
        case LOG_NFD: _log_notfound(src, dst, i, s); break;
        case LOG_NVI: _log_newvideo(src, dst, i, s, v); break;
        case LOG_OHC: _log_ohc(src, dst, i, s); break;
        case LOG_OHD: _log_ohd(src, dst, i, s); break;
        case LOG_ONH: _log_onh(src, dst, i, s); break;
        case LOG_OPE: _log_operation(src, dst, i, v); break;
        case LOG_PCH: _log_precachinghint(src, dst, i, s); break;
        case LOG_PKT: _log_packet(src, dst, i, s, v); break;
        case LOG_QLN: _log_queuelength(src, dst, i); break;
        case LOG_QRY: _log_query(src, dst, i, s); break;
        case LOG_RCM: _log_rcm(src, dst, i, s); break;
        case LOG_REP: _log_reply(src, dst, i, s, v); break;
        case LOG_REQ: _log_request(src, dst, i, s); break;
        case LOG_RHC: _log_rhc(src, dst, i, s); break;
        case LOG_RHD: _log_rhd(src, dst, i, s); break;
        case LOG_RNH: _log_rnh(src, dst, i, s); break;
        case LOG_RST: _log_reset(src, dst, i, s); break;
        case LOG_SPH: _log_sph(src, dst, i, s, v); break;
        case LOG_SPM: _log_spm(src, dst, i, s, v); break;
        case LOG_SSH: _log_ssh(src, dst, i, s); break;
        case LOG_STP: _log_setup(src, dst, i, s); break;
        case LOG_TDN: _log_teardown(src, dst, i, s); break;
        case LOG_TEM: _log_termination(src, dst, i, s, v); break;
        default: cout << "Unknown logging code: " << hex << showbase
                    << code << dec << endl;
        }
}
