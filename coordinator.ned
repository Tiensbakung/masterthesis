simple Coordinator
{
        parameters:
                xml clusters;
                bool analysisStats = default(false);
                int hotspotAnalysisInterval = default(0);
                int hotspotMinHits = default(0);
                int hotspotPercent = default(0);
                int precachingAnalysisInterval = default(0);
                int precachingPercent = default(0);
        gates:
                inout port;
}