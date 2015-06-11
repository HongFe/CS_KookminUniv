########### 전자정보통신대학 컴퓨터공학부 ###################
######################## 20093284 나홍철 ####################

chomp($directory = <stdin>); # Deleteing '\n' from input data

opendir( DIR, $directory ) || die "Unable to open directory - $!\n";	# Opening directory
my @files = grep /\.txt/, readdir( DIR ); # Selecting '*.txt' files from directory
closedir( DIR );		# Closing directory

my $output_file="$directory/all_files.txt"; # Saving 'all_files.txt' in directory.
open (my $output, ">", $output_file)  or die "cannot open output text $!";

# Opening each files
foreach my $file (@files) { 
	open (my $input, "<", "$directory\\$file")  or die "cannot open $!";
			#Reading each lines in file
			while($line=<$input>) {				
				print $output $line;	# Writing line in 'all_files.txt'
			}
			print $output "\n";
    close( $file );		# Closing file
	}
close( $output );	# Closing output file