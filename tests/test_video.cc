#include "video.h"
#include <iostream>
using namespace std;

int main()
{
        const size_t N = 10;
        cout << "video collection size: "
             << VideoCollection::instance().size() << endl ;
        for (size_t i = 0; i < N; ++i) {
                std::cout << i << ": name: "
                          << VideoCollection::instance().getVideoName(i)
                          << " size: "
                          << VideoCollection::instance().getVideoSegment(i)
                          << std::endl;
        }
        return 0;
}
