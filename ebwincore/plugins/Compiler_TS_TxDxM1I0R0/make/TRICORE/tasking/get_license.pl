use strict;
my @input_parameters= @ARGV;
my $commandline = shift @input_parameters;
my $make_goal = shift @input_parameters;

die "could not find compiler in $ARGV[0]" if (!-e $commandline);

$| = 1; # autoflush

my $wait_loop=1;
my $counter=0;

while ( ($wait_loop==1) &&
		(($make_goal eq "all")||($make_goal eq "single_file")||($make_goal eq "single_lib"))
	)
{
	my $retval = qx($commandline -v 2>&1);
	
	my @lines = split /\n/, $retval;
	
	my $tasking_v6_msg = "all in use";
	my $tasking_v5_msg = "ctc F104: protection error: No valid license available";
	my $tasking_v4_msg = "ctc F104: protection error: Licensed number of users already reached";
	my $tasking_v3_msg = "Licensed number of users already reached";
	my $delay_found=0;
	foreach my $line (@lines)
	{
		if (($line=~/$tasking_v6_msg/) ||
			($line=~/$tasking_v5_msg/) ||
			($line=~/$tasking_v4_msg/) ||
			($line=~/$tasking_v3_msg/))
		{
			$delay_found=1;
			last;
		}
	}
	if ($delay_found==1)
	{
		print STDERR "COMPILER all in use $counter\r";
		Win32::Sleep(1000);
	}
	else
	{
		$wait_loop=0;
	}
	$counter++;
}