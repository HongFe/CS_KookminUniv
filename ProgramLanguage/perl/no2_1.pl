########### 전자정보통신대학 컴퓨터공학부 ###################
######################## 20093284 나홍철 ####################

chomp($directory = <stdin>);  # Deleting '\n' from input data


opendir( DIR, $directory ) || die "Unable to open directory - $!\n";	# Opening directory
my @files = grep /\.txt/, readdir( DIR ); 		# Selecting '*.txt' files from directory
closedir( DIR );		# Closing directory

my $lineNum=0;		# Line Number
my $wordNum=0;		# Word Number

print "< NAME , LINE , WORD > \n";

# Opening each files
foreach my $file (@files) {	
	open (my $input, "<", "$directory\\$file")  or die "cannot open $!";	# Opening File
			#Reading each lines in file
			while($line=<$input>) {				
				my @orderW = split(/ /,$line);	# Splitting word
				$wordNum+=@orderW;		# The number of Word
				$lineNum++;				# The number of Line		
				}			
	print "< $file , $lineNum , $wordNum > \n";
	$wordNum=0;
	$lineNum=0;	
	
    close( $file );		# Closing file
	}