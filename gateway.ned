module GateWay
{
        parameters:
                int address;
        gates:
                inout ingress[];
                inout egress[];
                inout lanIn[];
                inout lanOut[];
        submodules:
                queue[sizeof(ingress)]: Queue;
                lanqueue[sizeof(lanIn)]: Queue;
                route: GateRoute;
                gate: Gate;
        connections:
                route.localPort <--> gate.port;
                for i=0..sizeof(ingress)-1 {
                    ingress[i] <--> route.in++;
                    queue[i].ingress <--> route.line++;
                    queue[i].egress <--> egress[i];
                }
                for i=0..sizeof(lanIn)-1 {
                    lanIn[i] <--> route.lanIn++;
                    route.lan++ <--> lanqueue[i].ingress;
                    lanqueue[i].egress <--> lanOut[i];
                }
}