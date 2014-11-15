#include <string>
#include "types.h"
using namespace std;
const ipaddr NETMASK = 0xffff0000;

ipaddr ip_number(ipaddr ip)
{
        return ip&(~NETMASK);
}

ipaddr ip_subnet(ipaddr ip)
{
        return ip&NETMASK;
}

bool is_server(ipaddr ip)
{
        return ip_subnet(ip) == 0;
}

bool is_gate(ipaddr ip)
{
        return ip_subnet(ip) && ip_number(ip)==0;
}

bool is_host(ipaddr ip)
{
        return  ip_subnet(ip) && ip_number(ip);
}


string IP(ipaddr i)
{
        unsigned char* p = (unsigned char*)&i;
        return to_string(*(p+3)) + "." + to_string(*(p+2)) + "." +
                to_string(*(p+1)) + "." + to_string(*(p+0));
}
