# Create a new simulator object
set ns [new Simulator]

#Fisnish Procedure
proc finish {} {
    global ns nf
    $ns flush-trace
    close $nf
    exit 0
}


# Open the trace file
set tracefile [open out.tr w]
$ns trace-all $tracefile

# Define a 'nam' trace file
set nf [open out.nam w]
$ns namtrace-all $nf


# Set the routing protocol to Distance Vector
$ns rtproto DV

# Create 7 nodes
for {set i 0} {$i < 7} {incr i} {
    set n($i) [$ns node]
}

# Connect the nodes to form a ring topology
for {set i 0} {$i < 6} {incr i} {
    $ns duplex-link $n($i) $n([expr $i+1]) 0.512Mb 5ms DropTail
}
$ns duplex-link $n(6) $n(0) 0.512Mb 5ms DropTail

# Create a UDP agent and attach it to node 0
set udp0 [new Agent/UDP]
$ns attach-agent $n(0) $udp0



# Create a CBR traffic source and attach it to udp0
set cbr0 [new Application/Traffic/CBR]
$cbr0 set packetSize_ 1024
$cbr0 set interval_ 0.01
$cbr0 attach-agent $udp0

# Create a Null agent (a traffic sink) and attach it to node 3
set null3 [new Agent/Null]
$ns attach-agent $n(3) $null3

# Connect the traffic source and sink
$ns connect $udp0 $null3

# Schedule events for the CBR traffic source
$ns at 0.02 "$cbr0 start"
$ns at 1.5 "$cbr0 stop"

# Schedule events to bring down and up the link between node 2 and node 3
$ns rtmodel-at 0.4 down $n(2) $n(3)
$ns rtmodel-at 1.0 up $n(2) $n(3)




# Call the finish procedure after 2 seconds of simulation time
$ns at 2.0 "finish"

# Run the simulation
$ns run



#Q9 Answers
# a. Initially, the packets follow the shortest path from Node 0 to Node 3. In a ring topology with 7 nodes, the shortest path from Node 0 to Node 3 is through Node 1, Node 2, and Node 3. This is because the Distance Vector (DV) routing protocol finds the shortest path based on the number of hops.

#b. After the link between Node 2 and Node 3 fails, the packets will take a different path. They will go from Node 0 to Node 1, then to Node 2, then they will go back to Node 1, then to Node 0, then to Node 6, Node 5, Node 4, and finally reach Node 3. This is because the DV routing protocol will update the routing tables after the link failure, and find the new shortest path.

#c. If we remove the Distance Vector Algorithm, the packets will follow the same path as before the link failure, i.e., Node 0 -> Node 1 -> Node 2 -> Node 3. This is because without a dynamic routing protocol like DV, the nodes are not aware of the network topology changes, and they will continue to send packets along the original path. When the packets reach the failed link, they will be dropped.

