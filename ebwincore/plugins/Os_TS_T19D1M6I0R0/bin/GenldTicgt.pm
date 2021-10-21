package GenldTicgt;

=head1 The C<GenldTicgt> package

This package implements the L<GenldBackend> interface for the TI Code Generation Tools linker.
This package's documentation only describes the specifics for this toolchain. The common
documentation can be found in L<GenldBackend>.

=head2 Toolchain-specific output section attributes

The following attributes are expected by the C<GenldTicgt> back end in the output section type
specifications:

=over

=item C<_ticgt_extraattr>: Optional extra attributes for an output section definition

This optional attribute can be used to give extra attributes that are appended to the definition of
an output section.

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
	$self->{args_printOutputSection}->{extendedInputSpecs} = 1;

	# this is used to compute the locations data sections' ROM images
	$self->{'datastart'} =
	{
		lastInitSym => "__GLBL_RODATA_END",
		lastSec     => undef,
		nextSec     => undef,
	};

	$self->{'osecJustification'} = {
		'DATA' => "TOOLDIAG-1",
		'BSS' => "TOOLDIAG-2",
		'RODATA' => "TOOLDIAG-3",
	};

	$self->{blockCounter} = 0;

	return $self;
}

# Add a tooldiag justification which is automatically applied to all output sections of a certain type.
# Parameters:
#  self    : this backend.
#  osecType: the kind of output sections
#  justi   : the justification identifier
sub addOutputSectionJustification
{
	my $self = shift;
	my ($osecType, $justi) = @_;

	$self->{'osecJustification'} = {} unless defined $self->{'osecJustification'};
	$self->{'osecJustification'}->{$osecType} = $justi;
}

sub needsLoadFlash
{
	my $self = shift;
	my ($osecType) = @_;

	return ($osecType eq 'DATA') && (not $self->{noload});
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

	my $blockno = $self->{blockCounter}++;
	GenldCommon::printOut("GROUP (_LNKBLOCKGROUP_${blockno}_${block}):");
	GenldCommon::printOut("{");

	if (defined $self->{curBlock}) {
		die "No matching endMemoryBlockMappings for previous beginMemoryBlockMappings";
	}
	$self->{curBlock} = {
		name => $block,
		hasLoadFlash => 0
	};
}

sub endMemoryBlockMappings
{
	my $self = shift;
	my ($block) = @_;

	my $curBlockH = $self->{curBlock};
	die "Tried to end block \"$block\" that was never started using beginMemoryBlockMappings" unless defined $curBlockH;
	my $curBlockNm = $curBlockH->{name};
	die "Tried to end block \"$block\" that wasn't started using beginMemoryBlockMappings" unless defined $curBlockNm;
	die "Tried to end block \"$block\", although the last started block was \"$curBlock\"" unless $block eq $curBlockNm;

	GenldCommon::printOut("} run=$block");

	my $rSymbols = $self->{curBlock}->{symbols};
	if ($rSymbols) {
		GenldCommon::printOut("SECTIONS");
		GenldCommon::printOut("{");
		for my $symbol (keys %$rSymbols) {
			my $val = $rSymbols->{$symbol};
			GenldCommon::printOut("$symbol = $val;");
		}
		GenldCommon::printOut("}");
	}

	$self->{curBlock} = undef;
}

sub printSymbolDefinition
{
	my $self = shift;
	my ($symbol, $value) = @_;
	GenldCommon::printOut("$symbol = $value;");
}

sub beginSymbolDefinitions
{
	my $self = shift;
	my ($block) = @_;
	GenldCommon::printOut("Symbol_Definitions_$block:");
	GenldCommon::printOut("{");
}
sub endSymbolDefinitions
{
	my $self = shift;
	GenldCommon::printOut("}");
}

sub printBufferGap
{
	my $self = shift;
	my ($start, $end, $gap) = @_;
	$gap ||= 6;

	die "Gaps are currently not supported.";

	if(defined $start && defined $end)
	{
		GenldCommon::printOut(". += (($end > $start) ? $gap : 0); /* gap between regions */\n");
	}
	else
	{
		GenldCommon::printOut(". += $gap; /* gap between regions */\n");
	}
}

sub justifyOsec
{
	my $self = shift;
	my ($osecType, $range) = @_;
	my $justi = $self->{'osecJustification'}->{$osecType};
	if (defined $justi) {
		$self->printComment("Possible diagnostic $justi <$range>");
	}
}

sub _int
{
	my ($val) = @_;
	if ($val =~ m/^0[xX]([0-9a-fA-F]+)$/) {
		$val = hex $1;
	}
	return $val;
}

sub printOutputSection
{
	my $self = shift;
	my ($osecName, $osecType, $inFiles, $inSections, $startSymbol, $endSymbol, $initSymbol, $initEndSymbol,
	    $preciseFiles, $startAlign, $endAlign, $extraArgs) = @_;

	die "Called printOutputSection outside of a beginMemoryBlockMappings endMemoryBlockMappings pair"
		unless defined $self->{curBlock};
	die "ticgt backend doesn't support 'precise files': $preciseFiles"
		if defined $preciseFiles;
	die "osecType is a mandatory parameter" unless defined $osecType;

	my $curBlockNm = $self->{curBlock}->{name};
	if ($self->needsLoadFlash($osecType)) {
		if (($self->{curBlock}->{lastOsecType} || '') eq $osecType) {
			warn ("Added \"$osecName\" after another section of type $osecType. "
					."This might be an issue, if they are meant to share one contiguous load image. "
					."At least if the first one is empty.")
		}
		$self->{curBlock}->{hasLoadFlash} = 1
	}

	my $ot = $self->{osecTypes}{$osecType};
	my $disableRomInit = $GenldCommon::sysdesc{'-disableRomInit'};
	die "Unknown section type: $osecType" unless ref $ot eq 'HASH';

	$self->printComment("Output section $osecName (Type $osecType)");

	my $extraattr = $ot->{_ticgt_extraattr} || '';

	if ($self->{noload}) {
		$self->{curBlock}->{symbols} = {} unless $self->{curBlock}->{symbols};
		$self->{curBlock}->{symbols}->{$initSymbol} = "0" if defined $initSymbol;
		$self->{curBlock}->{symbols}->{$initEndSymbol} = "0" if defined $initEndSymbol;
		$initSymbol = undef;
		$initEndSymbol = undef;
	}

	my $align = $ot->{align} || 1;
	$align = $startAlign if defined $startAlign;
	$align = _int($align);

	my $endAlignVal = $endAlign || 1;
	$endAlignVal = _int($endAlignVal);

	# If there's alignment, we add LNKUNALIGNED_ prefixes just to make it easier to check the output.
	my $endAlignPrefix = ($endAlignVal > 1)? "_LNKUNALIGNED_" : "";

	my $begattrs = "";
	$begattrs.= "RUN_START(_LNKPAD_$startSymbol), " if $startSymbol;
	$begattrs.= "palign=$align";
	GenldCommon::printOut("LNKPADGRP_${osecName}: {} $begattrs");

	my $attrs = "";
	#$attrs.= "run=$curBlockNm, "; # This would cause a warning, because the group already specifies a run address.
	$attrs.= "load=FLASH, " if $self->needsLoadFlash($osecType);
	$attrs.= "RUN_START($startSymbol), " if $startSymbol;
	$attrs.= "RUN_END($endAlignPrefix$endSymbol), " if $endSymbol;
	$attrs.= "LOAD_START($initSymbol), " if $initSymbol;
	$attrs.= "LOAD_END($endAlignPrefix$initEndSymbol), " if $initEndSymbol;
	$attrs.= "fill = 0, " if $self->needsLoadFlash($osecType);
	$attrs.= $extraattr;
	$attrs.= "palign=1"; # We align this section using LNKPAD_ and LNKENDALIGN_.

	$self->justifyOsec($osecType, 'START');
	GenldCommon::printOut("${osecName}:");
	GenldCommon::printOut("{");

	my @allInputSpecs = ();
	push @allInputSpecs, { inFiles => $inFiles, inSections => $inSections };
	my $extendedInputSpecs = $extraArgs->{extendedInputSpecs};
	if ($extendedInputSpecs) {
		push @allInputSpecs, @$extendedInputSpecs;
	}

	for my $rInputSpec (@allInputSpecs) {
		my $inFiles = $rInputSpec->{inFiles};
		my $inSections = $rInputSpec->{inSections};
		my $files  = $inFiles || [ '*' ];
		my $isects = $inSections || $ot->{isectDefault};
		my $isectSelector = join(" ", map { "\"$_\" \"$_.*\"" } @$isects);
		for my $filespec (@$files)
		{
			my $lib = undef;
			my $file = undef;

			if (ref $filespec eq 'HASH') {
				$lib = $filespec->{lib};
				$file = $filespec->{file};
			} else {
				$file = $filespec;
			}

			# For some reason we use '.obj' instead of '.o' for ticgt.
			$file = "$1.o*" if ($file =~ m/^(.*)\.obj$/);
			$file = "$1.o*" if ($file =~ m/^(.*)\.o$/);

			# Print the select statement.
			if (defined $lib) {
				GenldCommon::printOut("\"$lib\"<\"$file\"> ($isectSelector)");
			} else {
				GenldCommon::printOut("\"$file\" ($isectSelector)");
				GenldCommon::printOut("\"*.lib\"<\"$file\"> ($isectSelector)");
			}
		}
	}
	GenldCommon::printOut("} $attrs");
	$self->justifyOsec($osecType, 'STOP');

	# To align the end, add an empty section having the according alignment.
	if ($endAlignVal > 1) {
		my $endattrs = "";
		$endattrs.= "RUN_END($endSymbol), " if $endSymbol;
		$endattrs.= "LOAD_END($initEndSymbol), " if $initEndSymbol;
		$endattrs.= "palign=$endAlignVal";
		GenldCommon::printOut("LNKENDALIGN_${osecName}: {} $endattrs");
	}

	# Epilogue...
	$self->{curBlock}->{lastOsecType} = $osecType;

	return;
}

# This function is invoked by GenldCommon::parseArgs() if unknown arguments are encountered
sub parseArgs
{
	my $self = shift;
	my ($arg, $sysdesc, $argv_position) = @_;

	if($arg eq '-noload')
	{
		$sysdesc->{$arg} = 1;
		$self->{noload} = 1;
		return 0;
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
