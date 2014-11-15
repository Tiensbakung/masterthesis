#!/bin/sh
if [ -z $1 ]
then
    >&2 echo "Error: Topology must be specified!"
else
    echo "Extracting topology $1..."
    tar xvzf scripts/$1.tar.gz -C /tmp
    echo "Installing topology $1..."
    mv /tmp/$1/proxies.xml data/
    mv /tmp/$1/clusters.xml data/
    mv /tmp/$1/routes.xml data/
    mv /tmp/$1/proxynet.ned .
    mv /tmp/$1/blockproxynet.ned .
    mv /tmp/$1/clusternet.ned .
    mv /tmp/$1/omnetpp.ini .
    rmdir /tmp/$1
    echo "Done!"
fi
