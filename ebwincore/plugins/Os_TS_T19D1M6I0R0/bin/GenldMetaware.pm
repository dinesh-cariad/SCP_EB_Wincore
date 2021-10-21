package GenldMetaware;

use GenldGnu;

our @ISA;

BEGIN
{
	require GenldGnu;
	push @ISA, 'GenldGnu';
}

sub _new
{
	my $pkg = shift;
	my $self = $pkg->SUPER::_new(@_);

	print $self;
	return $self;
}

sub mapFileGlobs
{
	my $self = shift;
	my $inFiles = shift;
	@files = map {
		ref $_ eq 'HASH'
		? "$_->{lib}($_->{file}"
		: ( "*/$_", "*\\\\$_" )
	} @$inFiles;
	return \@files;
}

sub outputSectionAlignment
{
	my $self = shift;
	my $secAlign = shift;

	return "ALIGN($secAlign)";
}

1;

# Editor settings: DO NOT DELETE
# vi:set ts=4 sw=4 noet tw=100:
