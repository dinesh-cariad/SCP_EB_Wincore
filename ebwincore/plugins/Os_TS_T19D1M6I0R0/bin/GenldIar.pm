package GenldIar;

=head1 The C<GenldIar> package

This package implements the L<GenldBackend> interface for the IAR linker.
This package's documentation only describes the specifics for this toolchain. The common
documentation can be found in L<GenldBackend>.

=head2 Limitations

The IAR linker cannot assign the current address (commonly referred to
by other linkers using .) to a symbol. Thus, the function
C<printSymbolDefinition($symbol,$value)> is not implemented. Instead,
symbols have to be defined in the board-specific LD-script snippet
that commonly is used to define the output memory regions.

=over

=back

=cut

use GenldCommon;

# use strict;
# use warnings;

our @ISA;

BEGIN
{
	require GenldBackend;
	push @ISA, 'GenldBackend';
}

sub _new
{
	my $pkg = shift;
	my $self = $pkg->SUPER::_new(@_);

	# remembers the memory block we are currently mapping to, undef if none
	$self->{'currentMemoryBlock'} = undef;
	return $self;
}

sub printComment
{
	my $self = shift;
	GenldCommon::printOut("\n/* $_ */") foreach(@_);
}

sub beginOutputSectionMappings
{
	my $self = shift;
}

sub endOutputSectionMappings
{
	my $self = shift;
}

sub beginMemoryBlockMappings
{
	my $self = shift;
	my ($currentMemoryBlock) = @_;

	$self->{'currentMemoryBlock'} = $currentMemoryBlock;
}

sub endMemoryBlockMappings
{
	my $self = shift;
	my ($block) = @_;

	$self->{'currentMemoryBlock'} = undef;
}

sub printSymbolDefinition
{
	my $self = shift;
	my ($symbol, $value) = @_;
	GenldCommon::printOut("/* symbol definitions are not supported for IAR linker-script generator yet */");
	GenldCommon::printOut("/* define necessary symbols in board-specific ldscript header instead */");
}

sub printEmptyObjectRegionDefinition
{
	my $self = shift;
	my ($symbol, $value) = @_;
	GenldCommon::printOut("define block ${symbol} with size=0 { };");
	GenldCommon::printOut("place in empty { block ${symbol} };");
}

sub printBufferGap
{
	my $self = shift;
	my ($start, $end, $gap) = @_;
	$gap ||= 6;
	GenldCommon::printOut("/* gap between regions is not supported for IAR linker-script generator yet */");
}

sub printSectionDebugLabel
{
	my $self = shift;
	my ($symbol, $value) = @_;
	# currently no debug labels are necessary
	return;
}

sub _val_to_dec
{
	my $self = shift;
	my ($value) = @_;
	if ($value =~ /^0x[0-9A-F]+$/i)
	{
		$value = hex($value);
	}
	return $value;
}

sub printOutputSection
{
	my $self = shift;
	my ($osecName, $osecType, $inFiles, $inSections, $startSymbol, $endSymbol, $initSymbol,
		$initEndSymbol, $preciseFiles, $startAlign, $endAlign) = @_;

	my $ot = $self->{osecTypes}{$osecType};
	my $disableRomInit = $GenldCommon::sysdesc{'-disableRomInit'};
	die "Unknown section type: $osecType" unless ref $ot eq 'HASH';

	$self->printComment("******************_printOutputSection_begin_****************************");

	$self->printComment("Output section \'$osecName\' (Type \'$osecType\')");

	# Override region alignment from architecture definition
	if (defined $startAlign)
	{
		$startAlign = $self->_val_to_dec($startAlign);
	}
	else
	{
		# Get the alignment from the architecture configuration file or
		# use the size argument to overwrite the alignment. (e.g. for MPU)
		# Trusted function stacks have a size of 0x0, therefore align it to
		# the minimum boundary.
		$startAlign = $ot->{align};
	}

	# Override end alignment from architecture defintion
	if (defined $endAlign)
	{
		$endAlign = $self->_val_to_dec($endAlign);
	}


	# input section mappings
	my @files = ( '' );
	if($inFiles)
	{
		# Remove duplicate members in inFiles
		my @unique = keys %{{ map { $_ => 1 } @$inFiles }};
		$inFiles = \@unique;

		if ($preciseFiles)
		{
			# Use precise input file names
			@files = @$inFiles;
		}
		else
		{
			@files = map {
				ref $_ eq 'HASH'
				? "$_->{lib}($_->{file}"
				: $_
			} @$inFiles;
		}
	}

	my $isects = $inSections || $ot->{isectDefault};

	# Section selection patters
	my @isecSelectors = ();
	# Section selection patters for initialization data
	my @isecInitSelectors = ();

	# Generate patters for input section selection
	foreach my $file (@files)
	{
		$file = " object \"${file}\"" unless $file eq '';

		@sec = map { $_, $_.".*" } @$isects;
		# Filter out duplicate section patterns, sort to make output more readable.
		@sec= sort(keys %{{ map { $_ => 1 } @sec}});
		# Construct initalization sections from initial section patterns.
		@secInit = map { $_."_init" } @sec;

		@sec = map { "\tsection $_${file}" } @sec;
		@secInit = map { "\tsection $_${file}" } @secInit;

		push @isecSelectors, @sec;
		push @isecInitSelectors, @secInit;
	}

	# define the memory region (memory block)
	my $alignStr = '';
	if ($startAlign ne '0')
	{
		$alignStr = ", alignment = ${startAlign}";
	}
	GenldCommon::printOut("define block ${osecName} with fixed order${alignStr}");
	GenldCommon::printOut("{");

	# include into the memory block sections from given files
	GenldCommon::printOut(join(",\n", @isecSelectors));

	if ($osecType eq "DATA")
	{
		# All *_init sections will go to a different memory block
		GenldCommon::printOut("}");
		GenldCommon::printOut("except");
		GenldCommon::printOut("{");
		GenldCommon::printOut("\tsection *_init");
	}

	GenldCommon::printOut("};");

	# if the sections have to be initialized, create init block and place the sections in it
	if (defined $initSymbol && !$disableRomInit)
	{
		$self->printComment("Manual initialization");
		GenldCommon::printOut("initialize manually with packing = none");
		GenldCommon::printOut("{");
		GenldCommon::printOut(join(",\n", @isecSelectors));
		GenldCommon::printOut("};");

		# create init block
		$self->printComment("Output section \'${osecName}_init\' (data initialization)");
		GenldCommon::printOut("define block ${osecName}_init with fixed order, alignment = 32");
		GenldCommon::printOut("{");


		GenldCommon::printOut(join(",\n", @isecInitSelectors));
		GenldCommon::printOut("};");

		$self->printComment("Initial data placement (to ROM)");
		GenldCommon::printOut("place in $self->{memDefs}{initdata_memory} { block ${osecName}_init };");
	}

	# place the memory region (memory block) in specified memory type
	if ($osecType ne "DATA" && $osecType ne "BSS" && $osecType ne "STACK")
	{
		GenldCommon::printOut("place in $self->{'currentMemoryBlock'} { block ${osecName} };");
	}

	$self->printComment("*******************_printOutputSection_end_*****************************");
	GenldCommon::printOut("\n");
}

# Wraps previously defined blocks, and places them in order into the respective memory
sub placeInOrder()
{
	my $self = shift;
	my ($osecName, $r_arrayblocks, $minSize) = @_;
	my $blocks = join(', ', @$r_arrayblocks);
	$self->printComment("Place ${blocks} to $self->{'currentMemoryBlock'}");
	my $sizeStr = '';
	if (defined $minSize)
	{
		$sizeStr = ", minimum size = ${minSize}";
	}
	GenldCommon::printOut("define block group_${osecName} with fixed order${sizeStr}");
	GenldCommon::printOut("{");
	GenldCommon::printOut("\tblock ".(join ",\n\tblock ", @$r_arrayblocks));
	GenldCommon::printOut("};");
	my $placement = 'in';
	if ($osecName eq "STACKS")
	{
		$placement = 'at start of';
	}
	GenldCommon::printOut("place ${placement} $self->{'currentMemoryBlock'} { block group_${osecName} };");
}

# This function is invoked by GenldCommon::parseArgs() if unknown arguments are encountered
sub parseArgs
{
	my $self = shift;
	my ($arg, $sysdesc, $argv_position) = @_;

	return 0;
}

1;

# Editor settings: DO NOT DELETE
# vi:set ts=4 sw=4 noet tw=100:
