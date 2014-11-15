module CentralNode
{
        parameters:
                int address;
                int centralServerIP = default(0);
        gates:
                inout ingress[];
                inout egress[];
        submodules:
                queue[sizeof(ingress)]: Queue;
                route: CentralRoute;
                server: RegionServer;
                coordinator: Coordinator;
        connections:
                route.localPort <--> server.port;
                route.extraPort <--> coordinator.port;
                for i=0..sizeof(ingress)-1 {
                    ingress[i] <--> route.in++;
                    queue[i].ingress <--> route.line++;
                    queue[i].egress <--> egress[i];
                }
}