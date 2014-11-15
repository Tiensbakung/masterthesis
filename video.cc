#include <fstream>
#include "video.h"
#include "common.h"
using namespace std;

VideoCollection::VideoCollection(const char *fn, const size_t c)
{
        data.reserve(c);
        ifstream infile(fn);
        string a;
        vSegment b;
        while (infile >> a >> b) {
                data.push_back(Video(a, b/SEGMENTDURATION));
        }
}

const Video& VideoCollection::operator[](size_t index) const
{
        return data[index];
}

const string& VideoCollection::getVideoName(size_t index) const
{
        return data[index].first;
}

const vSegment& VideoCollection::getVideoSegment(size_t index) const
{
        return data[index].second;
}

size_t VideoCollection::size() const
{
        return data.size();
}
