module BlockProxyNode
{
        parameters:
                int address;
                int centralServerIP = default(-1);
                int numberOfServers;
        gates:
                inout ingress[];
                inout egress[];
        submodules:
                queue[sizeof(ingress)]: Queue;
                route: Route;
                server: BlockProxy;
        connections:
                route.localPort <--> server.port;
                for i=0..sizeof(ingress)-1 {
                    ingress[i] <--> route.in++;
                    queue[i].ingress <--> route.line++;
                    queue[i].egress <--> egress[i];
                }
}