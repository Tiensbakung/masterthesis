#include <vector>
#include <string>
#include <iterator>
#include <sstream>
#include <iostream>
using namespace std;

size_t fzipf()
{
        static vector<size_t> v;
        if (v.empty()) {
                static size_t seed = 1;
                string fn = "./scripts/fzipf.R " + to_string(seed++) + " " + to_string(4096);
                FILE *in;
                string str;
                char buffer[4096];
                if (!(in = popen(fn.c_str(), "r"))) {
                        cerr << "Execution error!" << endl ;
                        exit(0);
                }
                while (fgets(buffer, sizeof(buffer), in)) {
                        str.append(buffer);
                }
                pclose(in);
                stringstream stream(str);
                copy(istream_iterator<size_t>(stream),
                     istream_iterator<size_t>(), back_inserter(v));
        }
        size_t t = v.back();
        v.pop_back();
        return t;
}


size_t kzipf()
{
        static vector<size_t> v;
        if (v.empty()) {
                static size_t seed = 1;
                string fn = "./scripts/kzipf.R " + to_string(seed++) + " " + to_string(4096);
                FILE *in;
                string str;
                char buffer[4096];
                if (!(in = popen(fn.c_str(), "r"))) {
                        cerr << "Execution error!" << endl ;
                        exit(0);
                }
                while (fgets(buffer, sizeof(buffer), in)) {
                        str.append(buffer);
                }
                pclose(in);
                stringstream stream(str);
                copy(istream_iterator<size_t>(stream),
                     istream_iterator<size_t>(), back_inserter(v));
        }
        size_t t = v.back();
        v.pop_back();
        return t;
}
