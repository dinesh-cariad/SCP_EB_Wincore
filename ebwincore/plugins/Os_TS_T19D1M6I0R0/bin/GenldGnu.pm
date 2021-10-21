package GenldGnu;

=head1 The C<GenldGnu> package

This package implements the L<GenldBackend> interface for the GNU ld linker that is typically used
with GCC-based toolchains.
This package's documentation only describes the specifics for this toolchain. The common
documentation can be found in L<GenldBackend>.

=head2 Toolchain-specific output section attributes

The following attributes are expected by the C<GenldGnu> back end in the output section type
specifications:

=over

=item C<_gnu_extraattr>: Optional extra attributes for an output section definition

This optional attribute can be used to give extra attributes that are appended to the definition of
an output section. Currently, the C<NOLOAD> attribute is used to instruct the linker that the
section has no contents that need to be loaded during startup.

=back

=cut

use GenldCommon;

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

	# We currently have an extra argument
	$self->{args_printOutputSection}->{gapSizeToAppend} = 1;

	# remembers the memory block we are currently mapping to,
	# undef if none
	$self->{'currentMemoryBlock'} = undef;

	return $self;
}

sub printComment
{
	my $self = shift;
	GenldCommon::printOut("/*");
	GenldCommon::printOut(" * $_") foreach(@_);
	GenldCommon::printOut("*/");
}

sub beginOutputSectionMappings
{
	my $self = shift;
	GenldCommon::printOut("SECTIONS");
	GenldCommon::printOut("{");
}

sub endOutputSectionMappings
{
	my $self = shift;
	GenldCommon::printOut("}");
}

sub beginMemoryBlockMappings
{
	my $self = shift;
	my ($currentMemoryBlock, $phdr) = @_;

	$self->{currentMemoryBlock} = $currentMemoryBlock;
	$self->{'phdr'} = $phdr;
	$self->{'osecCount'} = 0;
}

sub endMemoryBlockMappings
{
	my $self = shift;
	my ($block) = @_;

	$self->{currentMemoryBlock} = undef;
	$self->{'phdr'} = undef;
	$self->{'osecCount'} = undef;
}

sub printSymbolDefinition
{
	my $self = shift;
	my ($symbol, $value) = @_;
	GenldCommon::printOut("$symbol = $value;");
}

sub printBufferGap
{
	my $self = shift;
	my ($start, $end, $gap) = @_;
	$gap ||= 6;

	GenldCommon::printOut(". += $gap; /* gap between regions */");
}

sub printSectionDebugLabel
{
	my $self = shift;
	my ($symbol, $value) = @_;
	return if $self->{noSectionDebug};
	$value ||= ".";
	$self->printSymbolDefinition($symbol, $value);
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

# prints an output section
# $osecName:   name of the output section
# $osecType:   type of the output section (key to %osecTypes)
# $inFiles:    ref to array with input files/file patterns
#                each entry is either: a scalar, representing an input file name/pattern
#                a hash with entries:
#                	file => input file name/pattern
#                	lib  => library file name/pattern
# $inSections: ref to array with input sections/input-section patterns, undef for section-type-dependent defaults
# $extraArgs->{gapSizeToAppend}:  appends a gap to the end of the section
#
sub printOutputSection
{
	my $self = shift;
	my ($osecName, $osecType, $inFiles, $inSections, $startSymbol, $endSymbol, $initSymbol,
		$initEndSymbol, $preciseFiles, $startAlign, $endAlign, $extraArgs) = @_;

	my $ot = $self->{osecTypes}{$osecType};
	my $disableRomInit = $GenldCommon::sysdesc{'-disableRomInit'};
	# Adds the pad byte to make the empty sections non-empty for QEMU 
	my $padBytes = $GenldCommon::sysdesc{'-padBytes'};
	die "Unknown section type: $osecType (for section '$osecName')" unless ref $ot eq 'HASH';

	$extraArgs ||= {}; # Set extraArgs to an empty hash if it isn't given.
	my $appendGap = $extraArgs->{gapSizeToAppend};
	
	$self->{'osecCount'}++;
	$self->printComment("Output section $osecName (Type $osecType)");

	# Override region alignment from architecture definition
	if (defined $startAlign)
	{
		$startAlign = $self->_val_to_dec($startAlign);
	}

	# Override end alignment from architecture defintion
	if (defined $endAlign)
	{
		$endAlign = $self->_val_to_dec($endAlign);
	}

	# Define default symbol names. This is meant to make debugging easier.
	$initEndSymbol ||= "__EBDBGLD_ENDOF_LOAD_$osecName" if (defined $initSymbol) and not $self->{noSectionDebug};

	# Get the alignment from the architecture configuration file or
	# use the size argument to overwrite the alignment. (e.g. for MPU)
	# Trusted function stacks have a size of 0x0, therefore align it to
	# the minimum boundary.
	$startAlign = $ot->{align} if !defined($startAlign);
	$startAlign = 8 if $startAlign == 0;

	my $secAlign = $self->outputSectionAlignment($startAlign);
	my $typestr = defined $ot->{_gnu_extraattr} ? "($ot->{_gnu_extraattr}) " : '';
	GenldCommon::printOut("$osecName $secAlign $typestr:");
	GenldCommon::printOut("{");

	$self->printSectionDebugLabel("__EBDBGLD_BEFOREBEGINOF_RUN_$osecName");
	GenldCommon::printOut(". = ALIGN($startAlign);");
	$self->printSymbolDefinition($startSymbol, '.') if defined $startSymbol;
	
	## Added just for padding to make empty sections non-empty 
	if(($padBytes) && ($osecName =~ m/^(data_C|bss_C)/))
	{
		GenldCommon::printOut("BYTE(0xeb);");
	}

	# input section mappings
	my @files = ( '*' );
	if($inFiles)
	{
		if ($preciseFiles)
		{
			# Use precise input file names
			@files = @$inFiles;
		}
		else
		{
			# GNU wants the full path name, which we do not support. We prepend a wildcard
			# to make the path of the module irrelevant
			# Also match backslashes which might appear in our paths on windows.
			@files =@{$self->mapFileGlobs($inFiles)};
		}
	}

	my $isects = $inSections || $ot->{isectDefault};
	my $isectSelector = join(" ", map { "$_ $_.*" } @$isects);

	foreach my $file (@files)
	{
		# If file contains an archive selector, indicated by opening (,
		# add a matching closing parenthesis
		my $post = ($file =~ /\(/) ? ')' : '';
		GenldCommon::printOut("\"$file\"($isectSelector)$post");
	}

	# Align the end of the section to $end_align bytes
	# Could be from the architecture definitions or from a given parameter
	# or no alignment.
	# This must be done even if there is no end symbol, because the end of the region could be
	# defined by the begin of the following region. In this case the RUN region might have a different size than the
	# LOAD region, which might result in reading uninitialized flash.
	$endAlign = $ot->{endalign} || 1 if !defined($endAlign);

	GenldCommon::printOut(". = ALIGN($endAlign);") if $endAlign > 1;

	$self->printSectionDebugLabel("__EBDBGLD_ENDOF_RUN_$osecName");
	if (defined $endSymbol)
	{
		$self->printSymbolDefinition($endSymbol, '.');
	}

	if(defined $appendGap)
	{
		$self->printBufferGap(undef, undef, $appendGap);
	}

	my $loadaddr = '';
	if(defined $initSymbol && !$disableRomInit) {
		$loadaddr = "AT>$self->{memDefs}{initdata_memory}";
	}

	#This is currently GNU only: Explicitly specify segments in the ELF header.
	#Print out for the first mapping of a new block. All following sections use this implicitly
	my $phdr = '';
	if(defined $self->{'phdr'} && $self->{'osecCount'} > 0) {
		$phdr = $self->{'phdr'};
	}
	GenldCommon::printOut("} >$self->{currentMemoryBlock} $phdr $loadaddr");


	if(defined $initSymbol)
	{
		my $align_minus_1 = $startAlign - 1;
		my $padding = sprintf "(((ADDR($osecName) + %d) & (~ %d)) - ADDR($osecName))", $align_minus_1, $align_minus_1;
		if ($disableRomInit) {
			$self->printSymbolDefinition($initSymbol, 0);
		} else {
			$self->printSymbolDefinition($initSymbol, sprintf("LOADADDR($osecName) + $padding"));
		}

		if(defined $initEndSymbol)
		{
			if ($disableRomInit) {
				$self->printSymbolDefinition($initEndSymbol, 0);
			} else {
				# SIZEOF(osecName) also contains the padding, so subtract the padding (like it was added to LOADADDR).
				$self->printSymbolDefinition($initEndSymbol, "$initSymbol + SIZEOF($osecName) - $padding");
			}
		}
	}
}

sub mapFileGlobs
{
	my $self = shift;
	my $inFiles = shift;
	@files = map {
		ref $_ eq 'HASH'
		? "$_->{lib}($_->{file}"
		: ( $_.'*', "*/$_", "*\\\\$_" )
	} @$inFiles;
	return \@files;
}

# For normal GNU LDs, aligning the section using ALIGN inside the section specification suffices.
# Make this behavior overrideable for derived backends based on the GNU syntax.
sub outputSectionAlignment
{
	return '';
}

# Prints an output section which has a gap in the end (default gap length: 6 bytes)
sub printOutputSectionWithGap
{
	my $self = shift;
	my ($osecName, $osecType, $inFiles, $inSections, $startSymbol, $endSymbol, $initSymbol,
		$initEndSymbol, $preciseFiles, $startAlign, $endAlign, $gapSize) = @_;

	$self->printOutputSectionH({
		osecName => $osecName,
		osecType => $osecType,
		inFiles => $inFiles,
		inSections => $inSections,
		startSymbol => $startSymbol,
		endSymbol => $endSymbol,
		initSymbol => $initSymbol,
		initEndSymbol => $initEndSymbol,
		preciseFiles => $preciseFiles,
		startAlign => $startAlign,
		endAlign => $endAlign,
		gapSizeToAppend => $gapSize || 6,
	});
}

# This function is invoked by GenldCommon::parseArgs() if unknown arguments are encountered
sub parseArgs
{
	my $self = shift;
	my ($arg, $sysdesc, $argv_position) = @_;

	if($arg eq '-no-section-debug')
	{
		$sysdesc->{$arg} = 1;
		$self->{noSectionDebug} = 1;
	}
	else
	{
		return 0;
	}

	return 1;
}

1;

# Editor settings: DO NOT DELETE
# vi:set ts=4 sw=4 noet tw=100:
