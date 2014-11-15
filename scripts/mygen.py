#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""A simple python script for generating necessary dynamic data (e.g.
routing table, etc.) for the /sim/ simulation.
Network topologies are defined by graphviz dot language.
"""
import ConfigParser
import itertools
import numpy as np
import random
from collections import defaultdict
import xml.etree.cElementTree as ET
from xml.dom import minidom

import networkx as nx
import pygraphviz as pgv

MAX = 10000.0                   # Constant used for inverting bandwidth


def topo_parser(filename):
    """Parse dot lanague defined topologies and return a parsed graph.

    Keyword Arguments:
    ------------------
    filename -- .dot file for a defined topology.

    Returns:
    --------
    graph -- networkx graph, see networkx.
    """
    agraph = pgv.AGraph(filename)
    G = nx.from_agraph(agraph)
    G.name = agraph.graph_attr['label']
    return G


def clustering(G, dist, min_size=3, hint=None):
    """Build clusters for graph G.

    Keyword Arguments:
    ------------------
    G: graph
    min_size: minimum cluster size.
    """
    edges = filter(lambda x: is_server(x[0]) and is_server(x[1]),
                   G.edges(data=True))
    radius = hint or MAX/int(min(edges, key=lambda x: x[2])[2]['weight'])
    diameter = max([x for y in dist.values() for x in y.values()])
    flag = True
    while flag:
        clusters = {}
        servers_to_clusters = {}
        flag = False
        if radius >= diameter:
            print 'Impossible clustering.'
            break
        servers = filter(lambda x: is_server(x), G)
        while servers:
            center = random.choice(servers)
            clusters[center] = set()
            for node in servers:
                if dist[node][center] <= radius:
                    clusters[center].add(node)
                    servers_to_clusters[node] = center
            for node in clusters[center]:
                servers.remove(node)
        lens = [len(x) for x in clusters.values()]
        if min(lens) < min_size:
            flag = True
            radius += 1
    return clusters, servers_to_clusters


def assign_gates_to_proxies(G):
    gates_to_proxies = {}
    for gate in filter(is_gate, G):
        proxy = random.choice(filter(is_server, G[gate]))
        gates_to_proxies[gate] = proxy
    return gates_to_proxies


def assign_gates_to_clusters(G, clusters, servers):
    gates_to_clusters = {}
    for gate in filter(is_gate, G):
        server = random.choice(filter(is_server, G[gate]))
        center = servers[server]
        gates_to_clusters[gate] = center
    return gates_to_clusters


def merge_gates(clusters, gates_to_clusters):
    for gate, center in gates_to_clusters.iteritems():
        clusters[center].add(gate)


def is_server(node):
    return node.isdigit()


def is_gate(node):
    return node.startswith('g')


def IP(node):
    if is_server(node):
        addr = node
    elif is_gate(node):
        addr = str(int(node[1:]) * 256**2 + 10 * 256**3)
    return addr


def xmldoc(xmlfile, entry=None, ID=None):
    line = 'xmldoc("data/{0}.xml", "'.format(xmlfile)
    if entry:
        if ID:
            line += "/root/{0}[@id='{1}']\")".format(entry, ID)
        else:
            line += "/root/{0}\")".format(entry)
    else:
        line += "/root\")"
    return line


def node_to_port(neighbors, route):
    """Rewrites forwarding node in route to forwarding port.

    Keyword Arguments:
    neighbors -- A sorted iterable of adjacent neighbor nodes.
    route     -- Routing table.
    """
    for k, v in route.iteritems():
        route[k] = neighbors.index(v)


def route_single_node(source, prev):
    """Return a routing table for node source.

    Keyword Arguments:
    ------------------
    source -- the node which the routing table is for.
    prev -- dictionary of <destination: previous node>s.

    Returns:
    --------
    routing table: dictionary of <destination: forwarding node>.
    """
    route = {}
    if source in prev:
        del prev[source]
    for target in prev:
        k = target
        while prev[k] != source:
            k = prev[k]
        route[target] = k
    return route


def route_all_nodes(G):
    """Returns routing tables for all nodes in graph G.

    Keyword Arguments:
    ------------------
    G -- Graph.

    Returns:
    --------
    routing tables: dictionary <node: routing table>, where routint table
                    is itself a dictionary <destination: forwarding node>.
    """
    dist = {}
    routing_tables = {}
    for node in G:
        di, prev = random_dijkstra(G, node)
        route = route_single_node(node, prev)
        node_to_port(sorted(G[node]), route)
        routing_tables[node] = route
        dist[node] = di
    return dist, routing_tables


def random_dijkstra(G, source):
    """Return routing tables for all nodes in non-negative weighted graph.

    Keyword Arguments:
    ------------------
    G  -- The graph data structure. Edge weight must be defined by edge
              attribute 'weight'.
    source -- Designated source node for find the routing table.

    Returns:
    --------
    dist vector -- Distance from source to all other nodes.
    routing tables -- A list of [node, routing table], where routing table
        is a list of (destination, node)) tuples.
    """
    numbers = range(len(G))
    np.random.shuffle(numbers)
    tickets = {x: y for x, y in itertools.izip(G.nodes(), numbers)}
    tickets['__unknown__'] = len(G)
    prev = defaultdict(lambda: '__unknown__')
    di = defaultdict(lambda: float('inf'))
    h = set(G.nodes())
    prev[source] = source
    di[source] = 0
    while len(h):
        node = min(h, key=lambda x: di[x])
        h.remove(node)
        for n in G[node]:
            if n not in h:
                continue
            c = di[node] + MAX/int(G[node][n][0]['weight'])
            if c < di[n]:
                di[n] = c
                prev[n] = node
            elif c == di[n] and tickets[node] < tickets[prev[n]]:
                prev[n] = node
    return di, prev


def dump_routes(routing_tables, filename):
    """Dumps routint tables to a xml file.

    Keyword Arguments:
    ------------------
    routing_tables -- routing tables for all nodes.
    filename       -- file name for the output xml file.
    """
    root = ET.Element('root')
    for source in routing_tables:
        table = ET.SubElement(root, 'table', id=source)
        for target, port in routing_tables[source].iteritems():
            ET.SubElement(table, 'entry', dest=str(IP(target)),
                          port=str(port))
    s = ET.tostring(root, 'utf-8')
    s = minidom.parseString(s)
    with open(filename, 'wb') as f:
        f.write(s.toprettyxml())


def gen_videostorage(servernumber, copynumber=3, videonumber=44491):
    """Generates a random videostorage sample.

    Keyword Arguments:
    ------------------
    servernumber -- Number of servers.
    copynumber   -- Numebr of copies, data redundancy, a server stores
                    mostly one copy.
    videonumber  -- (default 44491) Number of videos.
    """
    d = {}
    for i in xrange(videonumber):
        d[i] = random.sample(xrange(servernumber), copynumber)
    return d


def dump_proxies(G, filename):
    root = ET.Element('root')
    for server in filter(is_server, G):
        proxy = ET.SubElement(root, 'proxy', id=server)
        entry = ET.SubElement(proxy, 'entry')
        entry.text = server
    s = ET.tostring(root, 'utf-8')
    s = minidom.parseString(s)
    with open(filename, 'wb') as f:
        f.write(s.toprettyxml())


def dump_clusters(clusters, filename):
    root = ET.Element('root')
    for center, cluster in clusters.iteritems():
        c = ET.SubElement(root, 'cluster', id=center)
        for s in cluster:
            e = ET.SubElement(c, 'entry')
            e.text = IP(s)
    s = ET.tostring(root, 'utf-8')
    s = minidom.parseString(s)
    with open(filename, 'wb') as f:
        f.write(s.toprettyxml())


def dump_ini(G, gates_to_clusters, gates_to_proxies, clusters,
             servers_to_clusters, filename):
    config = ConfigParser.SafeConfigParser()
    config.optionxform = str
    # Config Section
    config.add_section('General')
    config.set('General', 'cmdenv-status-frequency', '1800s')
    config.set('General', 'sim-time-limit', '2hours')
    config.set('General', 'num-rngs', '4')
    config.set('General', '**.blockproxy.rng-0', '0')
    config.set('General', '**.proxy.rng-0', '0')
    config.set('General', '**.gate.rng-0', '1')
    config.set('General', '**.coordinator.rng-0', '2')
    config.set('General', '**.browser.rng-0', '3')

    # Setup routing tables
    for node in G:
        if is_server(node):
            k = '**.node{0}.route.table'.format(node)
        elif is_gate(node):
            k = '**.gate{0}.route.table'.format(node[1:])
        v = xmldoc('routes', 'table', node)
        config.set('General', k, v)

    # Setup IP addresses
    for node in G:
        if is_server(node):
            k = '**.node{0}.address'.format(node)
        elif is_gate(node):
            k = '**.gate{0}.address'.format(node[1:])
        config.set('General', k, str(IP(node)))
        if is_gate(node):
            k = '**.h{0}[*].gateway'.format(node[1:])
            config.set('General', k, str(IP(node)))
            k = '**.gate*.gate.numberOfVideos'
            config.set('General', k, '5000')

    # ProxyNet Section
    section = 'Config ProxyNet'
    config.add_section(section)
    config.set(section, 'network', 'ProxyNet')
    n = len(filter(is_server, G))
    config.set(section, '**.node*.numberOfServers', str(n))
    for gate in filter(is_gate, G):
        k = '**.gate{0}.gate.cluster'.format(gate[1:])
        v = xmldoc('proxies', 'proxy', gates_to_proxies[gate])
        config.set(section, k, v)

    # BlockProxy Net
    section = 'Config BlockProxyNet'
    config.add_section(section)
    config.set(section, 'network', 'BlockProxyNet')
    config.set(section, '**.node*.numberOfServers', str(n))
    for gate in filter(is_gate, G):
        k = '**.gate{0}.gate.cluster'.format(gate[1:])
        v = xmldoc('proxies', 'proxy', gates_to_proxies[gate])
        config.set(section, k, v)

    # Cluster Net
    section = 'Config ClusterNet'
    config.add_section(section)
    config.set(section, 'network', 'ClusterNet')
    for s, v in servers_to_clusters.iteritems():
        k = '**.node{0}.centralServerIP'.format(s)
        config.set(section, k, v)

    for g, c in gates_to_clusters.iteritems():
        k = '**.gate{0}.gate.cluster'.format(g[1:])
        v = xmldoc('clusters', 'cluster', c)
        config.set(section, k, v)

    for c in clusters:
        k = '**.node{0}.coordinator.clusters'.format(c)
        v = xmldoc('clusters')
        config.set(section, k, v)

    # Stats Net
    section = 'Config StatsNet'
    config.add_section(section)
    config.set(section, 'network', 'ClusterNet')
    config.set(section, '**.server.reportStats', 'true')
    config.set(section, '**.server.reportInterval', '240')
    config.set(section, '**.coordinator.analysisStats', 'true')
    config.set(section, '**.coordinator.hotspotAnalysisInterval', '900')
    config.set(section, '**.coordinator.hotspotPercent', '10')
    config.set(section, '**.coordinator.hotspotMinHits', '30')
    config.set(section, '**.coordinator.precachingAnalysisInterval', '600')
    config.set(section, '**.coordinator.precachingPercent', '60')

    for s, v in servers_to_clusters.iteritems():
        k = '**.node{0}.centralServerIP'.format(s)
        config.set(section, k, v)

    for g, c in gates_to_clusters.iteritems():
        k = '**.gate{0}.gate.cluster'.format(g[1:])
        v = xmldoc('clusters', 'cluster', c)
        config.set(section, k, v)

    for c in clusters:
        k = '**.node{0}.coordinator.clusters'.format(c)
        v = xmldoc('clusters')
        config.set(section, k, v)

    with open(filename, 'wb') as f:
        config.write(f)


def dump_videostorage(vs, filename='videostorage'):
    """Dumps a videostorage data to a file.
    Keyword Arguments:
    vs       -- Videostorage data, dict of <server index: set(video index)>.
    filename -- (default 'videostorage') File name to dump to.
    """
    with open(filename, 'wb') as f:
        for video, servers in vs.iteritems():
            f.write('{0} {1} '.format(video, len(servers)))
            f.write(' '.join(itertools.imap(str, servers)))
            f.write('\n')


def dump_net(G, filename, net_name, clusters=None):
    """Dumps a OMNeT++ ned file.
    Keyword Arguments:
    ------------------
    G        -- A networkx graph.
    filename -- file name for the ned file.
    """
    def nodename(node):
        if node.isdigit():
            return 'node' + node
        elif node.startswith('g'):
            return 'gate' + node[1:]

    channels = {}               # <datarate (Mbps): channel name>
    counter = 0
    for e in G.edges_iter(data=True):
        weight = int(e[2]['weight'])
        if weight not in channels:
            channels[weight] = 'Fiber{0}'.format(counter)
            counter += 1
    conn1 = '\t\t{0}.ingress++ <--> {1} <--> {2}.egress++;\n'
    conn2 = '\t\t{0}.egress++ <--> {1} <--> {2}.ingress++;\n'

    with open(filename, 'wb') as f:
        if net_name in ('ClusterNet', 'StatsNet'):
            f.write('network ClusterNet\n{\n')
        else:
            f.write('network {0}\n{{\n'.format(net_name))
        f.write('\ttypes:\n')

        # channels
        for rate, name in channels.iteritems():
            f.write('\t\tchannel {0} extends ned.DatarateChannel {{\n'.format(name))
            f.write('\t\t\tdatarate = {0}Gbps;\n\t\t}}\n'.format(rate/1000))
        f.write('\t\tchannel Ether extends ned.DatarateChannel {\n')
        f.write('\t\t\tdatarate = 100Mbps;\n\t\t}\n')

        # submodules
        f.write('\n\tsubmodules:\n')
        if net_name in ('StatsNet', 'ClusterNet'):
            for node in filter(is_server, G):
                if node in clusters:
                    f.write('\t\tnode{0}: CentralNode;\n'.format(node))
                else:
                    f.write('\t\tnode{0}: RegionNode;\n'.format(node))
        elif net_name in ('ProxyNet', 'BlockProxyNet'):
            for node in filter(is_server, G):
                f.write('\t\tnode{0}: {1}Node;\n'.format(node, net_name[:-3]))
        for gate in filter(is_gate, G):
            f.write('\t\tgate{0}: GateWay;\n'.format(gate[1:]))
            r = int(G.node[gate]['hosts'])
            f.write('\t\th{0}[{1}]: Host;\n'.format(gate[1:], r))

        # connections
        f.write('\n\tconnections:\n')
        l = sorted(G)
        for i in xrange(len(G)):
            for j in xrange(i+1, len(G)):
                n1, n2 = l[i], l[j]
                if not G.has_edge(n1, n2):
                    continue
                weight = int(G[n1][n2][0]['weight'])
                f.write(conn1.format(nodename(n1), channels[weight],
                                     nodename(n2)))
                f.write(conn2.format(nodename(n1), channels[weight],
                                     nodename(n2)))
                f.write('\n')
        for gate in filter(is_gate, G):
            r = int(G.node[gate]['hosts'])
            f.write('\t\tfor i=0..{0} {{\n'.format(r-1))
            f.write('\t\t\t'+nodename(gate)+'.lanIn++ <--> ')
            f.write('Ether <--> h'+gate[1:]+'[i].lanOut;\n')
            f.write('\t\t\t'+nodename(gate)+'.lanOut++ <--> ')
            f.write('Ether <--> h'+gate[1:]+'[i].lanIn;\n\t\t}\n')
        f.write('}\n')


def main():
    G = topo_parser('france-D-B-L-N-C-A-N-N.dot')
    # dist, prev = random_dijkstra(G, '0')
    # print 'Dist:', dist
    # print 'Prev:', prev
    # route = route_single_node('0', prev)
    # node_to_port(sorted(G['0']), route)
    # print 'Route:', route
    dist, routing_tables = route_all_nodes(G)
    clusters, servers = clustering(G, dist, 4)
    gates_to_proxies = assign_gates_to_proxies(G)
    gates_to_clusters = assign_gates_to_clusters(G, clusters, servers)
    merge_gates(clusters, gates_to_clusters)
    dump_routes(routing_tables, 'france/routes.xml')
    dump_proxies(G, 'france/proxies.xml')
    dump_clusters(clusters, 'france/clusters.xml')
    dump_net(G, 'france/proxynet.ned', 'ProxyNet')
    dump_net(G, 'france/blockproxynet.ned', 'BlockProxyNet')
    dump_net(G, 'france/clusternet.ned', 'ClusterNet', clusters)
    dump_ini(G, gates_to_clusters, gates_to_proxies, clusters,
             servers, 'france/omnetpp.ini')
    # n = len(filter(is_server, G))
    # vs = gen_videostorage(n, copynumber=3, videonumber=44491)
    # dump_videostorage(vs, '../data/videostorage25-3')


if __name__ == '__main__':
    main()
