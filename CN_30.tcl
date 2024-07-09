# Create a new simulator object
set ns [new Simulator]

# Define the finish procedure
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

# Create 6 nodes
for {set i 0} {$i < 6} {incr i} {
    set n($i) [$ns node]
}

# Connect the nodes to form a star topology
for {set i 1} {$i < 6} {incr i} {
    $ns duplex-link $n(0) $n($i) 0.512Mb 10ms DropTail
}

# Create a TCP agent and attach it to node 1
set tcp1 [new Agent/TCP]
$ns attach-agent $n(1) $tcp1

# Create a TCP sink and attach it to node 4
set sink4 [new Agent/TCPSink]
$ns attach-agent $n(4) $sink4

# Connect the TCP source and sink
$ns connect $tcp1 $sink4

# Create a UDP agent and attach it to node 2
set udp2 [new Agent/UDP]
$ns attach-agent $n(2) $udp2

# Create a CBR traffic source and attach it to udp2
set cbr2 [new Application/Traffic/CBR]
$cbr2 set packetSize_ 1000
$cbr2 set rate_ 256kb
$cbr2 attach-agent $udp2

# Create a Null agent (a traffic sink) and attach it to node 5
set null5 [new Agent/Null]
$ns attach-agent $n(5) $null5

# Connect the UDP source and sink
$ns connect $udp2 $null5

# Schedule events for the TCP and UDP traffic sources
$ns at 0.1 "$tcp1 start"
$ns at 1.5 "$tcp1 stop"
$ns at 0.2 "$cbr2 start"
$ns at 1.3 "$cbr2 stop"

# Schedule events to bring down and up the links
$ns rtmodel-at 0.5 down $n(0) $n(5)
$ns rtmodel-at 0.9 up $n(0) $n(5)
$ns rtmodel-at 0.7 down $n(0) $n(4)
$ns rtmodel-at 1.2 up $n(0) $n(4)

# Call the finish procedure after 2 seconds of simulation time
$ns at 2.0 "finish"

# Run the simulation
$ns run

