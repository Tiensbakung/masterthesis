module Host
{
        parameters:
                int gateway;
        gates:
                inout lanIn;
                inout lanOut;
        submodules:
                app: Browser;
                queue: Queue;
        connections:
                app.port <--> queue.ingress;
                queue.egress <--> lanOut;
                app.portIn <--> lanIn;
}