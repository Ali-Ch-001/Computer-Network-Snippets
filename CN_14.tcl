# Prompt the user for input
puts "Please enter a number greater than 4: "
set user_input [gets stdin]

# Check if the input is a valid number
if {![string is integer -strict $user_input] || $user_input <= 4} {
    puts "Invalid input. Please enter a number greater than 4."
    exit
}

# Convert the input to an integer
set number [expr {$user_input}]

# Check if the number is even or odd and print accordingly
if {$number % 2 == 0} {
    puts "Even integers from 4 to $number:"
    for {set i 4} {$i <= $number} {set i [expr {$i + 2}]} {
        puts $i
    }
} else {
    puts "Odd integers from 3 to $number:"
    for {set i 3} {$i <= $number} {set i [expr {$i + 2}]} {
        puts $i
    }
}

