########### 전자정보통신대학 컴퓨터공학부 ###################
######################## 20093284 나홍철 ####################

my ($text)= @ARGV;

# Infinite roof until command 'quit'
while (1){
	print "Input Command : ";	
	chomp(my $command = <stdin>);  # Deleting '\n' from input data
	
	# If 'count' command
	if( $command eq 'count') {
		my %word = ();		# Hash variable for word(key) & number of word(value) 
		
		open (my $input, "<",$text)  or die "cannot open input$!";
		
		# Reading line of text file
		while(my $line=<$input>) {
			chomp($line = $line);
			my @order = split(/ /,$line);		# Splitting line to word 	
			# Matching key & value
			foreach my $a (@order){
				# Matching complete, value of that key add 1 -> The number of words.
				if(exists $word{$a}){				
					$word{$a}++;
				}
				# Not Matching 
				else{
					$word{$a}= 1;
				}
			}
		}
		close ($input);
		
		print "\n";
		# Output word & the number of words
		foreach my $w (keys %word){
			print "$w : $word{$w} \n";
			}
		print "\n[Finished]\n\n";
		next;
		}
		
	# If 'replace' command
	elsif( $command eq 'replace'){
		my @temp;		# Store fixed word temporarily 
		my $a=0;
		
		open (my $input, "<",$text)  or die "cannot open input$!";
				
		while(my $line=<$input>) {
			chomp(@temp[$a] = $line);
			$a++;
		}		
		close ($input);
		
		# Infinite roof until command 'print'
		while(1){
			print "word-replace > ";
			chomp($com = <stdin>);  # Deleting '\n' from input data
									# Input command 'edit' or 'print'
			# Command 'edit'
			if($com eq 'edit'){					
				print "Input Word : ";
				chomp($in = <stdin>);  # word in text
				print "Input Replace Word : ";
				chomp($re = <stdin>);  # replace word
				
				#Replacing
				foreach $ll (@temp){
					$ll =~ s/$in/$re/i;					
				}
			}	
			
			# Command 'print'
			elsif($com eq 'print'){
				print "Input Newfile Name : ";
				chomp($name = <stdin>);		# Deleting '\n' from input data
											# Text name for output
				open(my $output,">",$name)  or die "cannot open output $!";
				print "\n";
				# Print contents of output text 
				foreach my $t (@temp){
					print $output "$t\n";
					print "$t\n";
				}
				close ($output);
				print "\n[Finished]\n\n";
				last;
			}			
			else{print "Wrong Command\n";}
		}
	}
	# Command 'quit'
	elsif( $command eq 'quit'){exit;}
	else{print "Wrong Command\n";}
}



