########### ����������Ŵ��� ��ǻ�Ͱ��к� ###################
######################## 20093284 ��ȫö ####################

use Encode;

chomp($file = <stdin>);  # Deleting '\n' from input data

# Decoding korean character.
$year = decode("cp949", "[0-9][0-9][0-9][0-9]��");
# Two types of character. ex) 11Month & 1Month
$day1 = decode("cp949", "[0-9][0-9]��");
$day2 = decode("cp949", "[0-9]��");

$hour1 = decode("cp949", "[0-9][0-9]��");
$hour2 = decode("cp949", "[0-9]��");

$min1 = decode("cp949", "[0-9][0-9]��");
$min2 = decode("cp949", "[0-9]��");

$qua1 = decode("cp949", "[0-9][0-9]��");
$qua2 = decode("cp949", "[0-9][0-9]��");
$qua3 = decode("cp949", "[0-9]��");
$qua4 = decode("cp949", "[0-9]��");

open (my $input, "<", $file)  or die "cannot open $!";	

#Reading each lines in file
while(my $line=<$input>) {
	my @order = split(/ /,$line);		
	foreach my $a (@order){
		$a = decode("cp949", $a);
		
		# Matching & Adding [year]
		if( $a =~ /$year/){
			$a =~ s/$year/<DATE>$a/;
			$a = encode("cp949", $a); 
			print "$a ";
		}
		# Matching & Adding [day]		
		elsif( $a =~ /$day1/){
			my $d = decode("cp949", "��");
			$a =~ s/$d/$d<\/DATE>/;
			$a = encode("cp949", $a); 
			print "$a ";
		}
		elsif( $a =~ /$day2/){
			my $d = decode("cp949", "��");
			$a =~ s/$d/$d<\/DATE>/;
			$a = encode("cp949", $a); 
			print "$a ";
		}
		# Matching & Adding [hour]
		elsif( $a =~ /$hour1/){
			$a =~ s/$hour/<TIME>$a/;
			$a = encode("cp949", $a); 
			print "$a ";
		}
		elsif( $a =~ /$hour2/){
			$a =~ s/$hour/<TIME>$a/;
			$a = encode("cp949", $a); 
			print "$a ";
		}
		# Matching & Adding [minute]
		elsif( $a =~ /$min1/){
			my $m = decode("cp949", "��");
			$a =~ s/$m/$m<\/TIME>/;
			$a = encode("cp949", $a); 
			print "$a ";
		}
		elsif( $a =~ /$min2/){
			my $m = decode("cp949", "��");
			$a =~ s/$m/$m<\/TIME>/;
			$a = encode("cp949", $a); 
			print "$a ";
		}
		# Matching & Adding [quantity]
		elsif( $a =~ /$qua1/){
			my $m = decode("cp949", "��");
			$a =~ s/$m/$m<\/QUANTITY>/;
			$a = encode("cp949", $a); 
			print "<QUANTITY>$a ";
		}
		elsif( $a =~ /$qua2/){
			my $m = decode("cp949", "��");
			$a =~ s/$m/$m<\/QUANTITY>/;
			$a = encode("cp949", $a); 
			print "<QUANTITY>$a ";
		}
		elsif( $a =~ /$qua3/){
			my $m = decode("cp949", "��");
			$a =~ s/$m/$m<\/QUANTITY>/;
			$a = encode("cp949", $a); 
			print "<QUANTITY>$a ";
		}
		elsif( $a =~ /$qua4/){
			my $m = decode("cp949", "��");
			$a =~ s/$m/$m<\/QUANTITY>/;
			$a = encode("cp949", $a); 
			print "<QUANTITY>$a ";
		}
		# Not Matching
		else{$a = encode("cp949", $a);
			print "$a "; 
		}
	}
}			
	close( $input );	
