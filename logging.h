#ifndef _LOGGING_H
#define _LOGGING_H
#include "types.h"

enum LogCode {
        LOG_CLN,                // Cache length
        LOG_ERR,                // Error Message
        LOG_HHC,                // Precaching Hint Hits Cache
        LOG_HHD,                // Precaching Hint Hits Disk
        LOG_HNH,                // Precaching Hint No Hit
        LOG_NCS,                // Newly Cached Segment
        LOG_NFD,                // Queried Video Not Found
        LOG_NVI,                // New Video Info
        LOG_OHC,                // Video Operator Hits Cache
        LOG_OHD,                // Video Operator Hits Disk
        LOG_ONH,                // Video Operator No Hit
        LOG_OPE,                // Video Operation
        LOG_PCH,                // Precaching Hint
        LOG_PKT,                // Packet
        LOG_QLN,                // Queue length
        LOG_QRY,                // Query Video Segment Location
        LOG_RCM,                // Replacement Cache Miss
        LOG_REP,                // Request
        LOG_REQ,                // Reply
        LOG_RHC,                // Request Hits Cache
        LOG_RHD,                // Request Hits Disk
        LOG_RNH,                // Request No Hit
        LOG_RST,                // Reset existing connection
        LOG_SPH,                // Sequnce Packet Hit
        LOG_SPM,                // Sequnce Packet Miss
        LOG_SSH,                // Send Segment Hint
        LOG_STP,                // Setup Connection
        LOG_TDN,                // Tear Down Connection
        LOG_TEM                 // Terminate Connection
};

void logging(LogCode code, ipaddr src, ipaddr dst, vIndex i, vSegment s=0,
             vSequence v=0);

#endif /* _LOGGING_H */
