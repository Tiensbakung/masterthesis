#ifndef _VIDEO_H_
#define _VIDEO_H_
#include <vector>
#include <string>
#include "types.h"
#define TOTAL_VIDEO_NUMBER 44491
// typedef std::size_t vSegment;
typedef std::pair<std::string, vSegment> Video;

class VideoCollection
{
private:
        std::vector<Video> data;
        VideoCollection(const char *fn="data/videos",
                        const size_t c=TOTAL_VIDEO_NUMBER);
        ~VideoCollection(){}
public:
        static const VideoCollection& instance() {
                static VideoCollection ref;
                return ref;
        }
        const Video& operator[](std::size_t index) const;
        const std::string& getVideoName(std::size_t index) const;
        const vSegment& getVideoSegment(std::size_t index) const;
        std::size_t size() const;
};

#endif /* _VIDEO_H_ */
