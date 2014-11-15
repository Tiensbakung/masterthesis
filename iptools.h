#ifndef IPTOOLS_H
#define IPTOOLS_H
#include <string>
#include "types.h"

ipaddr ip_number(ipaddr ip);
ipaddr ip_subnet(ipaddr ip);
bool is_server(ipaddr ip);
bool is_gate(ipaddr ip);
bool is_host(ipaddr ip);
std::string IP(ipaddr i);

#endif /* IPTOOLS_H */
