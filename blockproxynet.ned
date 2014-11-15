network BlockProxyNet
{
	types:
		channel Fiber0 extends ned.DatarateChannel {
			datarate = 1Gbps;
		}
		channel Ether extends ned.DatarateChannel {
			datarate = 100Mbps;
		}

	submodules:
		node11: BlockProxyNode;
		node10: BlockProxyNode;
		node13: BlockProxyNode;
		node1: BlockProxyNode;
		node0: BlockProxyNode;
		node3: BlockProxyNode;
		node2: BlockProxyNode;
		node5: BlockProxyNode;
		node4: BlockProxyNode;
		node7: BlockProxyNode;
		node6: BlockProxyNode;
		node9: BlockProxyNode;
		node8: BlockProxyNode;
		node12: BlockProxyNode;
		node14: BlockProxyNode;
		gate4: GateWay;
		h4[36]: Host;
		gate3: GateWay;
		h3[135]: Host;
		gate2: GateWay;
		h2[12]: Host;
		gate1: GateWay;
		h1[185]: Host;
		gate0: GateWay;
		h0[128]: Host;

	connections:
		node0.ingress++ <--> Fiber0 <--> node5.egress++;
		node0.egress++ <--> Fiber0 <--> node5.ingress++;

		node0.ingress++ <--> Fiber0 <--> node6.egress++;
		node0.egress++ <--> Fiber0 <--> node6.ingress++;

		node0.ingress++ <--> Fiber0 <--> node7.egress++;
		node0.egress++ <--> Fiber0 <--> node7.ingress++;

		node0.ingress++ <--> Fiber0 <--> gate3.egress++;
		node0.egress++ <--> Fiber0 <--> gate3.ingress++;

		node1.ingress++ <--> Fiber0 <--> node2.egress++;
		node1.egress++ <--> Fiber0 <--> node2.ingress++;

		node1.ingress++ <--> Fiber0 <--> node4.egress++;
		node1.egress++ <--> Fiber0 <--> node4.ingress++;

		node1.ingress++ <--> Fiber0 <--> node5.egress++;
		node1.egress++ <--> Fiber0 <--> node5.ingress++;

		node10.ingress++ <--> Fiber0 <--> node12.egress++;
		node10.egress++ <--> Fiber0 <--> node12.ingress++;

		node10.ingress++ <--> Fiber0 <--> node13.egress++;
		node10.egress++ <--> Fiber0 <--> node13.ingress++;

		node10.ingress++ <--> Fiber0 <--> gate1.egress++;
		node10.egress++ <--> Fiber0 <--> gate1.ingress++;

		node11.ingress++ <--> Fiber0 <--> node8.egress++;
		node11.egress++ <--> Fiber0 <--> node8.ingress++;

		node11.ingress++ <--> Fiber0 <--> node9.egress++;
		node11.egress++ <--> Fiber0 <--> node9.ingress++;

		node12.ingress++ <--> Fiber0 <--> node13.egress++;
		node12.egress++ <--> Fiber0 <--> node13.ingress++;

		node12.ingress++ <--> Fiber0 <--> node5.egress++;
		node12.egress++ <--> Fiber0 <--> node5.ingress++;

		node12.ingress++ <--> Fiber0 <--> gate0.egress++;
		node12.egress++ <--> Fiber0 <--> gate0.ingress++;

		node13.ingress++ <--> Fiber0 <--> node6.egress++;
		node13.egress++ <--> Fiber0 <--> node6.ingress++;

		node14.ingress++ <--> Fiber0 <--> node7.egress++;
		node14.egress++ <--> Fiber0 <--> node7.ingress++;

		node14.ingress++ <--> Fiber0 <--> node8.egress++;
		node14.egress++ <--> Fiber0 <--> node8.ingress++;

		node14.ingress++ <--> Fiber0 <--> gate2.egress++;
		node14.egress++ <--> Fiber0 <--> gate2.ingress++;

		node2.ingress++ <--> Fiber0 <--> node4.egress++;
		node2.egress++ <--> Fiber0 <--> node4.ingress++;

		node2.ingress++ <--> Fiber0 <--> node7.egress++;
		node2.egress++ <--> Fiber0 <--> node7.ingress++;

		node3.ingress++ <--> Fiber0 <--> node4.egress++;
		node3.egress++ <--> Fiber0 <--> node4.ingress++;

		node3.ingress++ <--> Fiber0 <--> node5.egress++;
		node3.egress++ <--> Fiber0 <--> node5.ingress++;

		node3.ingress++ <--> Fiber0 <--> gate0.egress++;
		node3.egress++ <--> Fiber0 <--> gate0.ingress++;

		node4.ingress++ <--> Fiber0 <--> gate4.egress++;
		node4.egress++ <--> Fiber0 <--> gate4.ingress++;

		node5.ingress++ <--> Fiber0 <--> gate4.egress++;
		node5.egress++ <--> Fiber0 <--> gate4.ingress++;

		node6.ingress++ <--> Fiber0 <--> node9.egress++;
		node6.egress++ <--> Fiber0 <--> node9.ingress++;

		node6.ingress++ <--> Fiber0 <--> gate1.egress++;
		node6.egress++ <--> Fiber0 <--> gate1.ingress++;

		node6.ingress++ <--> Fiber0 <--> gate3.egress++;
		node6.egress++ <--> Fiber0 <--> gate3.ingress++;

		node7.ingress++ <--> Fiber0 <--> node8.egress++;
		node7.egress++ <--> Fiber0 <--> node8.ingress++;

		node8.ingress++ <--> Fiber0 <--> node9.egress++;
		node8.egress++ <--> Fiber0 <--> node9.ingress++;

		node8.ingress++ <--> Fiber0 <--> gate3.egress++;
		node8.egress++ <--> Fiber0 <--> gate3.ingress++;

		node9.ingress++ <--> Fiber0 <--> gate1.egress++;
		node9.egress++ <--> Fiber0 <--> gate1.ingress++;

		for i=0..35 {
			gate4.lanIn++ <--> Ether <--> h4[i].lanOut;
			gate4.lanOut++ <--> Ether <--> h4[i].lanIn;
		}
		for i=0..134 {
			gate3.lanIn++ <--> Ether <--> h3[i].lanOut;
			gate3.lanOut++ <--> Ether <--> h3[i].lanIn;
		}
		for i=0..11 {
			gate2.lanIn++ <--> Ether <--> h2[i].lanOut;
			gate2.lanOut++ <--> Ether <--> h2[i].lanIn;
		}
		for i=0..184 {
			gate1.lanIn++ <--> Ether <--> h1[i].lanOut;
			gate1.lanOut++ <--> Ether <--> h1[i].lanIn;
		}
		for i=0..127 {
			gate0.lanIn++ <--> Ether <--> h0[i].lanOut;
			gate0.lanOut++ <--> Ether <--> h0[i].lanIn;
		}
}
