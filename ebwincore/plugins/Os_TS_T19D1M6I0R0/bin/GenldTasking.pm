package GenldTasking;

=head1 The C<GenldTasking> package

This package implements the L<GenldBackend> interface for the Altium tasking linker.
This package's documentation only describes the specifics for this toolchain. The common
documentation can be found in L<GenldBackend>.

=head2 Toolchain-specific output section attributes

The following attributes are expected by the C<GenldTasking> back end in the output section type
specifications:

=over

=item C<_tasking_groupattrs>: Extra attributes for the group that surrounds an output section

For each output section that is defined, the Tasking back end creates an output section definition
nested inside a group. The group is needed to define properties of the output section such as the
alignment (giving the alignment to the output section itself would make it apply to all the input
sections inside the output section, which is not what we need). This attribute can be used to define
extra attributes for this group. It must B<not> be used for the alignment, which needs to be
specified using the standard C<align> attribut of the output section type specification.

Initially this feature was created to specify the C<nocopy> group attribute, which is the pendant to
C<NOLOAD> on GNU-like linkers, i.e. it indicates that the section has no contents that need to be
loaded during startup (or by another loader such as the debugger).

=item C<_tasking_secattrs>: Optional extra attributes for an output section definition

This optional attribute can be used to give extra attributes that are appended to the definition of
an output section. Examples for the usage include:

=over

=item *

the C<attributes> attribute which can indicate the type of access (e.g., read only, read write, read
execute)

=item *

the C<blocksize> attribute that ensures that the size of the section will be a multiple of the
C<blocksize> (but prefer the standard C<endalign> attribute)

=item *

the C<fill> attribute to define a fill value to be used for unused parts of loadable sections, for
example padding introduced for alignment

=back

=back

=head2 Implementation notes

=head3 Implementation of output section definitions

We define a group for each memory block that ensures that its contained groups/sections are located
in sequential order, and that sets the given memory block for allocating the contents.

A group can be used for three things (from tasking manual):

=over

=item 1.

Assign properties to the sections in a group like alignment and read/write attributes.

=item 2.

Define the mutual order of the sections in the group.

=item 3.

Restrict the possible addresses for the sections in a group.

=back

Sub groups inherit all properties from a parent group.

The memory block group we create in C<beginMemoryBlockMappings()> handles (2) + (3). (1) is done in
the nested groups created in C<printOutputSection()>.

The memory block group contains further groups for the actual output sections. We use the
outer group to enforce the following properties:

=over

=item C<ordered>

Locate the sections in the same order in the address space as they appear in the group (but not
necessarily adjacent)

=item C<contiguous>

Locate the sections in the group in a single address range, except for alignment gaps that may be
needed between sections

=item C<ordered> + C<contiguous> (sequential group):

Sections in the group are located in the same order as they appear in the linker script and occupy a
contiguous range of memory. This is the required semantics of the L<GenldBackend> interface.

=back

We need to create a nested group for each output section, and the output section within it. This is
needed because we want to define properties such as alignment for the output section we create.
B<The group defines properties for all the sections it contains>.

For example, C<group(align=64)> does I<NOT> mean that the group itself is aligned to 64 bytes, but
that all the output sections inside the group get this minimum alignment (of course, that implies
that the group itself will also have that alignment).

If we did not define an own output section inside the group, the linker would create output sections
inside the group from each input section, and apply all properties of the group to each input
section. This can be bad, for example with a large alignment such as 64 bytes, it would align each
input section to that, which normally means every object.

Consequently, both a nested group and the explicit definition of an output section are needed to
achieve the required semantics.

=head3 Selection of input sections from input files

Tasking does not directly support selection based on the object file, but the object file is encoded
in the section name which allows us to derive the section name based on the file.

The compiler generates sections of the form:
C<section_type_prefix[.core_association].module_name.symbol_name>
C<core_association> is not used by us.

Custom-named sections via the C<section> source code attribute get the name specified in the
attribute, except when C<--section-name-with-symbol> is used, then the symbol names are appended to
the section name.

We assume that section mapping either work by file (C<module_name>) I<OR> by custom section, not
mixed. If the C<$inFiles> parameter of C<printOutputSection()> is undefined, we assume a mapping
based on section names independent of the file, otherwise we assume that the named sections are to
be included from the given files.

=cut

use GenldCommon;

our @ISA;

BEGIN
{
	require GenldBackend;
	push @ISA, 'GenldBackend';
}

push @GenldCommon::usage_fns, \&tasking_usage;

sub _new
{
	my $pkg = shift;
	my $self = $pkg->SUPER::_new(@_);

	# each reserved section needs a name, we use this counter to assign
	# unique names with an ascending index
	$self->{gapIndex} = 0;

	# The ROM images for initialized data sections need to be placed in a separate
	# group, therefore we need to postpone emitting them. We record these postponed
	# sections in this array, and emit them on the next endMemoryBlockMappings()
	$self->{romInitDataSections} = [];

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
	GenldCommon::printOut("section_layout ::linear");
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

	GenldCommon::printOut("\ngroup (contiguous, ordered, run_addr = mem:$block)");
	GenldCommon::printOut("{");
}

sub endMemoryBlockMappings
{
	my $self = shift;
	my ($block) = @_;
	GenldCommon::printOut("}");

	# Create sections for the ROM images of initialized data sections
	if(@{$self->{romInitDataSections}} > 0)
	{
		GenldCommon::printOut("\ngroup (contiguous, ordered, load_addr = mem:$self->{memDefs}{initdata_memory})");
		GenldCommon::printOut("{");
		while(my $romImage = shift @{$self->{romInitDataSections}})
		{
			my ($osecName, $idat, $iend) = @{$romImage}{qw(osecName initSymbol initEndSymbol)};
			$self->_printOutputSectionHelper($osecName, @{$romImage}{qw(osecType input_sec_sels)});
			$self->printSymbolDefinition($idat, "\"_lc_ub_$osecName\"") if defined $idat;
			$self->printSymbolDefinition($iend, "\"_lc_ue_$osecName\"") if defined $iend;
			GenldCommon::printOut("\n");
		}
		GenldCommon::printOut("}");
	}
}

sub printSymbolDefinition
{
	my $self = shift;
	my ($symbol, $value) = @_;

	# Note on assignment operators:
	# := defines a symbol only if references exist in some object file
	#  = defines a symbol unconditionally
	GenldCommon::printOut("\"$symbol\" = $value;");
}

sub printBufferGap
{
	my $self = shift;
	my ($start, $end, $gap) = @_;
	$gap ||= 6;

	$self->printComment("gap between regions");
	GenldCommon::printOut("reserved \"regionBufferGap_$self->{gapIndex}\" (size=$gap);\n");

	$self->{gapIndex} += 1;
}

sub printOutputSectionWithGap
{
	# Tasking has its own section gap handling so we do not need to take care of this.
	# This is therefore only a dummy function for tasking.
	my $self = shift;
	my ($osecName, $osecType, $inFiles, $inSections, $startSymbol, $endSymbol, $initSymbol, $initEndSymbol) = @_;

	$self->printOutputSection($osecName, $osecType, $inFiles, $inSections, $startSymbol, $endSymbol, 
		$initSymbol, $initEndSymbol);
}

sub printOutputSection
{
	my $self = shift;
	my ($osecName, $osecType, $inFiles, $inSections, $startSymbol, $endSymbol, $initSymbol, $initEndSymbol) = @_;

	my $ot = $self->{osecTypes}{$osecType};
	die "Unknown section type: $osecType" unless ref $ot eq 'HASH';
	$inSections ||= $ot->{isectDefault};

	$self->printComment("Output section $osecName (Type $osecType)");

	# Create the input section selectors. We need these for both the input and the output
	# section, so we make sure that we use the same list for both of them
	my @input_sec_sels = ();

	if(defined $inFiles)
	{
		foreach my $file (@$inFiles)
		{
			if (ref $file eq 'HASH')
			{
				if ($file->{lib})
				{
					warn "Tasking does not support selection from archives: $file->{lib}, $file->{file}\n";
				}
				$file = $file->{file};
			}

			# strip the file suffix
			(my $modname = $file) =~ s/\.o$//;
			foreach my $sect (@$inSections)
			{
				push @input_sec_sels, "$sect.$modname.*";
			}
		}
	}
	else
	{
		foreach my $sect (@$inSections)
		{
			# custom-named section without --section-name-with-symbol
			# Also supports the -R" " switch, which puts objects in sections named only
			# after the section type prefix, e.g. .text
			push @input_sec_sels, $sect;

			# if the section ends with a wildcard, no need to add a separate .* since it
			# is already covered
			if($sect !~ /\*$/)
			{
				# custom-named section with --section-name-with-symbol,
				# or default prefixes
				push @input_sec_sels, "$sect.*";
			}
		}
	}

	# ROM IMAGE
	#
	# Record that we need an initialization image here for later
	if(defined $initSymbol)
	{
		if($GenldCommon::sysdesc{'-use-tasking-init'} || $GenldCommon::sysdesc{'-disableRomInit'})
		{
			# if tasking's own init code is used, we don't need to create explicit ROM
			# image sections and the initSymbol and initEndSymbol are not needed
			$self->printSymbolDefinition($initSymbol, 0);
			$self->printSymbolDefinition($initEndSymbol, 0) if defined $initEndSymbol;
		}
		else
		{
			my @input_sec_sels_rom = map { "\\\[$_\\\]" } @input_sec_sels;

			push @{$self->{romInitDataSections}},
			{
				osecName => "initdata.$osecName",
				osecType => '_TASKING_INITDATA',
				initSymbol => $initSymbol,
				initEndSymbol => $initEndSymbol,
				input_sec_sels => \@input_sec_sels_rom,
			};
		}
	}

	# RAM IMAGE
	$self->_printOutputSectionHelper($osecName, $osecType, \@input_sec_sels);

	$self->printSymbolDefinition($startSymbol, "\"_lc_ub_$osecName\"") if defined $startSymbol;
	$self->printSymbolDefinition($endSymbol, "\"_lc_ue_$osecName\"") if defined $endSymbol;
	GenldCommon::printOut("\n");
}

sub _printOutputSectionHelper
{
	my $self = shift;
	my ($osecName, $osecType, $input_sec_sels) = @_;

	my $ot = $self->{osecTypes}{$osecType};
	my $gattrs = $ot->{_tasking_groupattrs} || [];
	my $gattr = join(', ', ("align=$ot->{align}", @$gattrs));
	GenldCommon::printOut("group ($gattr)");
	GenldCommon::printOut("{");
	{
		my @secattrs = @{$ot->{_tasking_secattrs}};

		# Optionally align the end of the section to $end_align
		# We implement this by using the blocksize section attribute, which forces the allocation
		# of the section unit in blocks of the given size. In combination with the known start
		# alignment of the section, we can achieve the requested end alignment. This requires that
		# the start of the section is also aligned like its end.
		if (defined $ot->{endalign} && $ot->{endalign} > 1)
		{
			my $end_align = $ot->{endalign};

			if (grep /^blocksize=/, @secattrs)
			{
				die "endalign cannot be combined with a manual blocksize setting";
			}

			if ( ($ot->{align} % $end_align) == 0 )
			{
				push @secattrs, "blocksize=$end_align";
			}
			else
			{
				die "tasking: align ($ot->{align}) must be divisible without remainder by endalign ($end_align)";
			}
		}

		my $secattr = join(', ', @secattrs);
		GenldCommon::printOut("section \"$osecName\" ($secattr)");
		GenldCommon::printOut("{");
		GenldCommon::printOut(map { "select \"$_\";" } @$input_sec_sels);
		GenldCommon::printOut("}");
	}
	GenldCommon::printOut("}");
}

# This function is invoked by GenldCommon::parseArgs() if unknown arguments are encountered
sub parseArgs
{
	my $self = shift;
	my ($arg, $sysdesc, $argv_position) = @_;

	# this default implementation doesn't do anything
	if($arg eq '-use-tasking-init')
	{
		$sysdesc->{$arg} = 1;
	}
	else
	{
		return 0;
	}

	return 1;
}

sub tasking_usage
{
	print STDERR <<EOUSAGE;
Additional options specific to __PKG__:

  --use-tasking-init:
   The startup code will use Tasking's c_init() function for initializing the data. The linker script
   generator emits the ROM images of data sections needed for custom data initialization code, and
   instead tasking uses its own internal data structures.

EOUSAGE

}

1;

# Editor settings: DO NOT DELETE
# vi:set ts=4 sw=4 noet tw=100:
