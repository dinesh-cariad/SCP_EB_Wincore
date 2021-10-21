package GenldBackend;

=head1 The C<GenldBackend> package

This package serves as an interface specification for the linker-specific back-end packages. It
provides a factory to create instances of back-end objects.

=head2 Factory Interface

=over

=item $pkg->createBackend($toolname, $sysdesc)

This function creates a back-end object for the given linker, specified by its name in C<$toolname>.
C<$toolname> is the string as found in the C<OS_TOOLCHAIN> make variable in the build environment.
C<$sysdesc> is a reference to the system description created by the command line parser in
L<GenldCommon>.

The back end must be implemented in a package with the name C<GenldTool>, where the first character
of C<Tool> is uppercase. The method returns a reference to the created back-end object, or a string
with an error message in case of an error.

=back

=head2 Backend Interface

This is the interface that needs to be implemented by B<every> toolchain-specific back end. To keep
this maintainable, it is important to carefully think about possible extensions / changes to this
interface, since they need to be done across all the back-ends and using generator scripts
consistently, or otherwise the main goal of the linker script generator framework will be missed.

It is assumed that each generator script contains a certain nesting structure:

=over

=item *

At the outermost level, section mappings are initiated using
C<< $be->beginOutputSectionMappings() >> and terminated by  C<< $be->endOutputSectionMappings() >>.

This is done only once for a linker script.

=item *

Nested into the section mappings, it is assumed that output section mappings are grouped according
to the target memory block. Therefore at the next level inside a section mapping it is expected to
have several memory block mappings initiated using C<< $be->beginMemoryBlockMappings($block) >> and
ended by C<< $be->endMemoryBlockMappings($block) >>.

=item *

Finally, inside the mappings of a memory block, individual output sections are created by mapping
input sections to them. This is done by calling $be->printOutputSection().

=back

This logical structure should be followed by any generator script. It is to be seen as an abstract
description of the memory layout required on the target platform. It does not necessarily have to
match the structure of the resulting linker script. It is the task of the back end to map this
abstract representation to the toolchain-specific notation. The back end may keep as state the
information of previously executed functions that do not lead to the immediate generation of linker
script output. For example, there may not be statements that need to be generated for a memory
block, but the information is needed for each individual output section that is produced. In this
case, the back end can internally store the current memory block from the last
C<< $be->beginMemoryBlockMappings($block) >> and use this information when creating the output
section. To generalize this, in the worst case the back end can absorb the entire abstract
definition of the memory layout and create the actual output on the final
C<< $be->endOutputSectionMappings() >> call, when it is known that the mappings are complete.

Here comes the descriptions of the individual functions of the interface:

=over

=item C<< $be->beginOutputSectionMappings() >>, C<< $be->endOutputSectionMappings() >>

These functions are called to start / end the output section mappings. These must be used only once
for the generation of the linker script, i.e. it is a valid assumption for the back end that after
the call to C<< $be->endOutputSectionMappings() >> all output sections have been defined.

=item C<< $be->beginMemoryBlockMappings($block) >>, C<< $be->endMemoryBlockMappings($block) >>

Normally, each target has several types of memory (e.g. flash, core-local RAMs, global RAMS). This
pair of functions is used to wrap the output section definitions that go to each of these memories.
The parameter C<$block> is a string that gives the name of the corresponding memory block.
Typically, this name is defined in the memory block definitions in a static linker script snippet
that is added to the generated linker script using the I<-include> option.

These functions must only be used during the output section mappings, i.e. after the call of
C<< $be->beginOutputSectionMappings() >> and before the call of
C<< $be->endOutputSectionMappings() >>.

=item C<< $be->printOutputSection($osecName, $osecType, $inFiles, $inSections, $startSymbol, $endSymbol, $initSymbol, $initEndSymbol, $preciseFiles, $startAlign, $endAlign, $extraArgs) >>

This function creates an output section and maps input sections to the created section. It gets the
following parameters:

=over

=item C<$osecName>: The name of the output section

=item C<$osecType>: The type of the output section as a string, e.g. C<'TEXT'>.

It is expected that the attributes of the given section type can be found in the
architecture-specific description of the section types, found in C<< GenldArch<TARGET> >> module.

It is possible (and sometimes necessary) to extend the section-type descriptions in the
C<< GenldArch<TARGET> >> module by toolchain-specific attributes. This should be avoided if possible
because these attributes then need to be added for all architectures using this toolchain. In case
such attributes are needed, their name shall be prefixed with C<< _<TOOL>_ >>, as for example in
C<_tasking_groupattrs> or C<_diab_typedesc>.

=item C<$inFiles>: Reference to an array with input file specifications

This parameter gives a list of input files from that the input sections should be taken. If this
parameter is undefined, the selection of input sections is solely based on the input section names.

Each entry of the array can be either a string, giving the name of an object file (wild-card pattern
allowed), or a hash reference to select objects contained within a library. The referenced hash has
the keys C<lib> and C<file> giving the name of a library and an object file contained in that
library. Again, wild-card patterns are permissible for both. Unfortunately, linker support varies
concerning the selection from library files and the allowed syntax, and therefore the usage of this
feature should be limited as far as possible.

Also, the support for wild-card patterns varies across toolchains, so the usage should be limited to
a single C<*> if possible, which is widely supported.

=item C<$inSections>: Reference to an array with input sections

This parameter gives a list of input sections that should be mapped to the output section. If this
parameter is undefined, a default value is taken from the output section attributes of the
C<< GenldArch<TARGET> >> module.

Each entry of the array is a string containing the name of an input section, where the use of a
wild-card pattern is permissible.

The selection of input section is the cross product of the specified input files and input sections,
i.e. all input sections are looked for in all the given files.

=item C<$startSymbol>, C<$endSymbol>

These parameters optionally give the names of symbols that shall be defined at the start and end of
the run-location (VMA) of the output section. Proper alignment of these symbols is done as required
by the C<$startAlign> and C<$endAlign> parameters.

=item C<$initSymbol>, C<$initEndSymbol>

These parameters optionally give the names of symbols that shall be defined at the start and end of
the load-location (LMA) of the output section. Presence of a value for C<$initSymbol> indicates that
this is a section that needs to be loaded during startup and therefore triggers the creation of a
load region if required for the target. Giving a value for C<$initSymbol> is therefore mandatory for
all loadable output sections.

=item C<$preciseFiles>

This parameter is necessary for toolchains that require a full path name, e.g. GNU.

=item C<$startAlign>, C<$endAlign>
These parameters specifies the alignment of the beginning and the ending of the output section.

=item C<$extraArgs>
This parameter gives a hash that contains additional parameters that are for example only relevant for
a specific toolchain. A parameter shall only be added to the signature of printOutputSection if all
currently used toolchain backends support this feature. Otherwise, this C<$extraArgs> parameter is to
be used. 

=back

These functions must only be used during a memory block's section mappings, i.e. after the call of
C<< $be->beginMemoryBlockMappings($block) >> and before the call of
C<< $be->endMemoryBlockMappings($block) >>.

=item C<< $be->printComment(@lines) >>

This function can be used to print a comment at the current location in the linker file. It is
possible that the comment may not be located together with the surrounding logical operations in the
output linker file, if these operations to not directly lead to the generation of linker script
output.

The comment to be printed is given as a parameter list to the function. Each entry of the list shall
preferably be printed on a line of its own in the generated linker script.

=item C<< $be->printSymbolDefinition($symbol, $value) >>

This function can be used to define the symbol whose identifier is given in C<$symbol> to the value
given in C<$value>. Generally, no interpretation of C<$value> is performed by the back end, so
C<$value> can in its simplest form be an absolute address, or a more sophisticated expression that
also refers to other symbol identifiers. Use should be kept as simple as possible, and specialized
support for creating symbols for section start and end symbols provided by
C<< $be->printOutputSection() >> shall be preferred over using this function.

=item C<< $be->printBufferGap($start, $end, $gap) >>

This function inserts a gap of unused address space after the last output section, whose VMA start
and end addresses are given by the parameters C<$start> and C<$end>. The size of the gap in bytes is
given by the parameter C<$gap>. A gap needs only be output if the previous output section was not
empty, which for some linkers can be checked by testing C<< $end > $start >>. This is not required
though and always outputting the gap is permissible.

Use cases for this gap are MPUs on architectures such as TriCore 1.6 where only the start address of
a memory access is checked to be inside the region bounds, but the last address affected by the
access might be outside the allowed region and would not trigger a memory protection exception.
Having the gap ensures that such accesses do not corrupt the memory of an adjoining region.

=back

=cut

sub createBackend
{
	my $pkg = shift;
	my ($toolname, $sysdesc, $osecTypes) = @_;

	my $bepkg = 'Genld' . ucfirst($toolname);
	eval { require "$bepkg.pm"; };
	return "Could not load the back-end package $bepkg for the selected linker back end: $@" if $@;
	return $bepkg->_new($sysdesc, $osecTypes);
}

sub _new
{
	my $pkg = shift;
	my ($sysdesc) = @_;

	my $self =
	{
		'sysdesc'                   => $sysdesc,

		# This set contains the names of printOutputSection's arguments.
		# Only add arguments here, if all backends know how to deal with them
		# (one way of dealing with an argument is reporting an error).
		'args_printOutputSection'   =>  {
			osecName => 1, osecType => 1,
			inFiles => 1, inSections => 1,
			startSymbol => 1, endSymbol => 1,
			initSymbol => 1, initEndSymbol => 1,
			preciseFiles => 1,
			startAlign => 1, endAlign => 1
		},
	};

	bless $self, $pkg;
	return $self;
}

# This function is invoked by GenldCommon::parseArgs() if unknown arguments are encountered
sub parseArgs
{
	my $self = shift;
	my ($arg, $sysdesc, $argv_position) = @_;

	# this default implementation doesn't do anything
	return 0;
}

sub setArchCharacteristics
{
	my $self = shift;
	my ($osecTypes, $memDefs) = @_;

	$self->{osecTypes} = $osecTypes;
	$self->{memDefs} = $memDefs;
}

# This function takes a reference to a hash containing printOutputSection's arguments
# and calls printOutputSection.
sub printOutputSectionH
{
	my $self = shift;
	my ($args) = @_;

	my $rSignature = $self->{args_printOutputSection};
	for my $key (keys %$args) {
		die "printOutputSectionH: unsupported argument \"$key\"" unless $rSignature->{$key};
	}

	return $self->printOutputSection(
		$args->{osecName},
		$args->{osecType},
		$args->{inFiles},
		$args->{inSections},
		$args->{startSymbol},
		$args->{endSymbol},
		$args->{initSymbol},
		$args->{initEndSymbol},
		$args->{preciseFiles},
		$args->{startAlign},
		$args->{endAlign},
		# For additional parameters, use this hash.
		$args,
	);
}

sub beginSymbolDefinitions { }

sub endSymbolDefinitions { }

1;

# Editor settings: DO NOT DELETE
# vi:set ts=4 sw=4 noet tw=100:
