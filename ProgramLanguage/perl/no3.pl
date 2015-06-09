########### 전자정보통신대학 컴퓨터공학부 ###################
######################## 20093284 나홍철 ####################

my ($string, $type)= @ARGV;

# If directory data.
if (-d $type ){
	# Opening directory
	opendir( DIR, $type ) || die "Unable to open directory - $!\n";
	my @files = grep /\.txt/, readdir( DIR ); # Selecting '*.txt' files from directory
	closedir( DIR );
		# Opening each files in directory
	foreach my $file (@files) {	
		open (my $input, "<", "$type\\$file")  or die "cannot open $!";
			#Reading each lines in file
			while(my $line=<$input>) {				
				if($line =~ /$string/){		# Finding string line
					print "$line\n";
				}
			}			
	close( $file );		# Closing file
	}
}

else {
	open (my $input, "<", $type)  or die "cannot open $!";	
		#Reading each lines in file
		while(my $line=<$input>) {				
			if($line =~ /$string/){		# If string of input data include
				print "$line\n";		#
			}
		}			
	close( $file );	
}
