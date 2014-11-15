#include <cassert>
#include <iostream>
#include "iptools.h"
using namespace std;

int main()
{
    ipaddr host = 10 * 16777216 + 3;
    ipaddr gate = 10 * 16777216;
    ipaddr server = 9;
    assert(ip_subnet(host) == 10*16777216);
    assert(ip_subnet(gate) == 10*16777216);
    assert(ip_subnet(server) == 0);
    assert(ip_number(host) == 3);
    assert(ip_number(gate) == 0);
    assert(ip_number(server) == 9);
    assert(is_server(host) == false);
    assert(is_server(gate) == false);
    assert(is_server(server) == true);
    assert(is_host(host) == true);
    assert(is_host(gate) == false);
    assert(is_host(server) == false);
    assert(is_gate(host) == false);
    assert(is_gate(gate) == true);
    assert(is_gate(server) == false);
    cout << "All tests succeed!" << endl;
    return 0;
}
