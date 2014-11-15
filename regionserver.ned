simple RegionServer extends Server
{
        parameters:
                int reportInterval=default(0);
                bool reportStats = default(false);
                @class(RegionServer);
}