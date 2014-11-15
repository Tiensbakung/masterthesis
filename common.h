#ifndef _COMMON_H_
#define _COMMON_H_
#define SEQNUMBER 340           // Segment length in (packets)
#define SEGMENTDURATION 10      // Length of a segment in (seconds)
#define PACKETLENGTH 1500       // in bytes
#define COPYNUMBERS 3           // Number of copies stored in servers
#define REPLICA_NUMBER 3        // Number of replica in a cluster for
                                // hotspot videos
enum VideoOperation {
        VIDEO_DROP,
        VIDEO_LIFT
};
#endif                          // _COMMON_H_
