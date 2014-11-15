#include <iostream>
#include "filesystem.h"
using namespace std;

int main()
{
        for (size_t i=0; i < 3; ++i) {
                for (size_t j=0; j < 5; ++j) {
                        if (FileSystem::instance().exists(i, j)) cout << "Y ";
                        else cout << "N ";
                }
                cout << endl;
        }
        return 0;
}
