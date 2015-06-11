########### 전자정보통신대학 컴퓨터공학부 ###################
######################## 20093284 나홍철 ####################

my $input_file = "input.txt";
my $output_file = "output.txt";

# Opening 'input.txt' for reading
open (my $input, "<", $input_file)  or die "cannot open < input.txt: $!";
# Opening 'output.txt' for writing
open (my $output, ">", $output_file)  or die "cannot open < output.txt: $!";

# Writing each lines in 'input.txt' to 'output.txt'
while($line=<$input>) {
	my @order = split(/ /,$line);	# Split input data
	foreach my $a (@order){			
		print $output "$a\n" ;
	}
}
close ($output);	# Closing output file
close ($input);		# Closing input file