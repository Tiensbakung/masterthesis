#include <fstream>
#include "common.h"
#include "filesystem.h"
using namespace std;

FileSystem::FileSystem(const char *fn)
{
        ServerSet s;
        size_t counter = 0;
        ipaddr addr;
        vIndex index = 0;
        ifstream infile(fn);
        while (infile >> index) {
                infile >> counter;
                s.clear();
                for (size_t i=0; i < counter; ++i) {
                        infile >> addr;
                        s.emplace(addr);
                }
                data[index] = s;
        }
}

bool FileSystem::exists(ipaddr addr, vIndex i) const
{
        return data.at(i).count(addr)>0;
}

const ServerSet& FileSystem::servers(vIndex i) const
{
        return data.at(i);
}
