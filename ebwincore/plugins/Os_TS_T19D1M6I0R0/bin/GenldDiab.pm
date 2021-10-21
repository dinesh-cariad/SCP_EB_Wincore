package GenldDiab;

=head1 The C<GenldDiab> package

This package implements the L<GenldBackend> interface for the WindRiver Diab linker.
This package's documentation only describes the specifics for this toolchain. The common
documentation can be found in L<GenldBackend>.

=head2 Toolchain-specific output section attributes

The following attributes are expected by the C<GenldDiab> back end in the output section type
specifications:

=over

=item C<_diab_typedesc>: Defines the type of the section

Each output section definition in a linker script for Diab needs a type-spec, which is given by this
attribute. Valid values include C<BSS>, C<COMMENT>, C<CONST>, C<DATA>, C<TEXT> and C<BTEXT>. See the
WindRiver documentation for details.

=item C<_diab_extraattr>: Optional extra attributes for an output section definition

This optional attribute can be used to give extra attributes that are appended to the definition of
an output section. Currently, the C<KEEP> directive is used to avoid the removal of the output
section by the linker.

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

	# this is used to compute the locations data sections' ROM images
	$self->{'datastart'} =
	{
		lastInitSym => "__GLBL_RODATA_END",
		lastSec     => undef,
		nextSec     => undef,
	};

	return $self;
}

sub printComment
{
	my $self = shift;
	if (@_ == 1) {
		GenldCommon::printOut("/* $_ */") foreach(@_);
	} else {
		GenldCommon::printOut("/*");
		GenldCommon::printOut(" * $_") foreach(@_);
		GenldCommon::printOut("*/");
	}
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
	my ($block) = @_;
	GenldCommon::printOut("GROUP :");
	GenldCommon::printOut("{");
}

sub endMemoryBlockMappings
{
	my $self = shift;
	my ($block) = @_;
	GenldCommon::printOut("} > $block");
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

sub hexStringToInt
{
	my ($val) = @_;
	if ($val =~ m/^0[xX]([0-9a-fA-F]+)$/) {
		$val = hex $1;
	}
	return $val;
}

sub printOutputSectionSimple
{
	my $self = shift;
	my ($osecName, $osecType, $inFiles, $inSections, $startSymbol, $endSymbol, $initSymbol,
		$initEndSymbol, $preciseFiles, $startAlign, $endAlign, $extraArgs) = @_;

	my $ot = $self->{osecTypes}{$osecType};
	my $disableRomInit = $GenldCommon::sysdesc{'-disableRomInit'};
	die "Unknown section type: $osecType (for section '$osecName')" unless ref $ot eq 'HASH';

	$extraArgs ||= {}; # Set extraArgs to an empty hash if it isn't given.
	my $appendGap = $extraArgs->{gapSizeToAppend};

	$self->printComment("Output section $osecName (Type $osecType)");

	my $ds = $self->{datastart};
	$ds->{nextSec} = $osecName unless defined $ds->{nextSec};

	my $loadaddr = '';
	if(defined $initSymbol) {
		if(defined $disableRomInit)
		{
			$self->printSymbolDefinition($initSymbol, 0);
		} else {
			if(defined $ds->{lastSec})
			{
				$self->printSymbolDefinition($initSymbol,
						"$ds->{lastInitSym} + (ADDR($ds->{nextSec}) - ADDR($ds->{lastSec}))");
			}
			else
			{
				$self->printSymbolDefinition($initSymbol, $ds->{lastInitSym});
			}
			$ds->{lastInitSym} = $initSymbol;
			$ds->{lastSec} = $osecName;
			undef $ds->{nextSec};

			$loadaddr  = "LOAD($initSymbol) ";
		}
	}

	my $extraattr = $ot->{_diab_extraattr} || '';

	# Workaround for TCDIAB-13800 - Tricore: Diab Linker produces broken program headers
	my $align = $ot->{align} || 1;
	$align = $startAlign if defined $startAlign;
	$align = hexStringToInt($align);
	my $align_minus_1 = $align-1;
	GenldCommon::printOut(". = ((. + ($align_minus_1)) & (~($align_minus_1)));") if $align_minus_1 > 0;

	my $diabTypedescStr = ($ot->{_diab_typedesc} eq '-')? "" : "($ot->{_diab_typedesc})";
	GenldCommon::printOut("$osecName $diabTypedescStr ALIGN($align) $loadaddr $extraattr :");
	GenldCommon::printOut("{");
	$self->printSymbolDefinition($startSymbol, '.') if defined $startSymbol;

	my $files  = $inFiles || [ '*' ];
	my $isects = $inSections || $ot->{isectDefault};
	my $isectSelector = join(" ", map { "\"$_\" \"$_.*\"" } @$isects);

	foreach my $file (@$files)
	{
		my $selector;

		if (ref $file eq 'HASH')
		{
			$selector = "\"$file->{lib}\"[\"$file->{file}\"]";
		}
		else
		{
			$selector = "\"$file\"";
		}

		GenldCommon::printOut("$selector($isectSelector)");
	}

	# Optionally align the end of the section to $end_align bytes
	my $end_align = $ot->{endalign} || 1;
	$end_align = $endAlign if defined $endAlign;
	$end_align = hexStringToInt($end_align);
	GenldCommon::printOut(". = ALIGN($end_align);") if $end_align > 1;

	$endSymbol ||= "GENLD_SECEND_$osecName";
	$self->printSymbolDefinition($endSymbol, '.');

	if(defined $appendGap)
	{
		$self->printBufferGap(undef, undef, $appendGap);
	}

	GenldCommon::printOut("}\n");

	if(defined $initEndSymbol) {
		if(defined $disableRomInit) {
			$self->printSymbolDefinition($initEndSymbol, 0);
		} else {
			$self->printSymbolDefinition($initEndSymbol, "$initSymbol + ($endSymbol - ADDR($osecName))");
		}
	}
}
sub printOutputSection
{
	my $self = shift;
	my ($osecName, $osecType, $inFiles, $inSections, $startSymbol, $endSymbol, $initSymbol, $initEndSymbol,
	    $preciseFiles, $startAlign, $endAlign, $extraArgs) = @_;

	my $mixInSDA =
		$GenldCommon::sysdesc{'-mix-in-sda'}
		&& (not defined $inSections)
		&& (($osecType eq 'DATA') || ($osecType eq 'BSS'));

	if ($mixInSDA) {
		my $prefix = ".s".(lc $osecType).".";
		my $sOsecType = "S".$osecType;
		my $uscoreName = $osecName;
		my ($sdaEndSymbol, $sdaInitEndSymbol, $ndaStartSymbol, $ndaInitSymbol); # = undef
		$sdaEndSymbol = "LNK_SDA_END_".$osecType."_".$endSymbol if defined $endSymbol;
		$sdaInitEndSymbol = "LNK_SDA_INITEND_".$osecType."_".$initEndSymbol if defined $initEndSymbol;
		$ndaStartSymbol = "LNK_NONSDA_START_".$osecType."_".$startSymbol if defined $startSymbol;
		$ndaInitSymbol = "LNK_NONSDA_INIT_".$osecType."_".$initSymbol if defined $initSymbol;
		$self->printComment("Mixing in SDA variables:");
		$self->printOutputSectionSimple($prefix.$osecName, $sOsecType, $inFiles, undef,
			$startSymbol, $sdaEndSymbol, $initSymbol, $sdaInitEndSymbol, undef, undef,
			undef, $extraArgs);
		$self->printOutputSectionSimple($osecName, $osecType, $inFiles, undef,
			$ndaStartSymbol, $endSymbol, $ndaInitSymbol, $initEndSymbol, undef, undef,
			undef, $extraArgs);
	} else {
		$self->printOutputSectionSimple($osecName, $osecType, $inFiles, $inSections,
			$startSymbol, $endSymbol, $initSymbol, $initEndSymbol,
			$preciseFiles, $startAlign, $endAlign, $extraArgs);
	}
}

# Prints an output section which has a gap in the end (default gap length: 6 bytes)
#
# NOTE: $extraArgs is ignored when this subroutine is used. Otherwise we would
#       break compatibility with the MK TRICORE linker script generator. This
#       should not matter because $extraArgs seems not to be used for diab.
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

# If SDAs are used, Diab always demands for a section which is called ".sbss".
# Add this section after all other SDAs. It should be empty.
sub printDummySdaSections
{
	my $self = shift;
	if ($GenldCommon::sysdesc{'-mix-in-sda'}) {
		$self->printComment("Note: Range [LNK_SDA_DUMMY_BEGIN,LNK_SDA_DUMMY_END) should be empty.");
		GenldCommon::printOut("LNK_SDA_DUMMY_BEGIN = .;");
		GenldCommon::printOut(".sbss  ALIGN(32) :{}");
		GenldCommon::printOut("LNK_SDA_DUMMY_END = .;");
	}
}

1;

# Editor settings: DO NOT DELETE
# vi:set ts=4 sw=4 noet tw=100:
