package GenldArmkeil;

=head1 The C<GenldArmkeil> package

This package implements the L<GenldBackend> interface for the ARM KEIL linker.
This package's documentation only describes the specifics for this toolchain.
The common documentation can be found in L<GenldBackend>.

Because of the unusual structure of the armkeil linker the script actually
does not write the provided statements directly to the output file but rather
saves them in an internal array and generates the script by calling the
_genLinkerFile function. This is done when the endOutputSectionMappings function,
provided by the factory interface, is called.

=head2 Toolchain-specific output section attributes

The following attributes are expected by the C<GenldArmkeil> back end in the
output section type specifications:

=over

=item C<_armkeil_attr_fixed>: This attribute advises the linker to put the created
                              memory section on the same load and execution address.

If the command line option -disableRomInit is set, the linker script generator
uses this attribute to create memory section which are only located on the provided
RAM and are not copied from ROM to RAM during startup.

=item C<_armkeil_attr_uninit>: This attribute advises the linker no to initialize
                               the created memory section.

This is used for bss sections which are initialized by the kernel.

=item C<_armkeil_attr_empty>: This attribute advises the linker to create an empty
                              memory section.

The ARM/Keil linker is not able to create linker symbols inside of scatter files.
But for every created memory section the linker automatically generates section
specific symbols. This attribute is used to create symbols by specifying an empty
section to work around this limitation of the ARM/Keil toolchain.

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

	$self->{'content'} = [];
	$self->{'symbols'} = [];
	$self->{'currentBlock'} = undef;
	$self->{'gapCounter'} = 0;

	return $self;
}

# public functions ############################################################

sub beginOutputSectionMappings
{
	$self = shift;
	# nothing to do here
}

sub endOutputSectionMappings
{
	$self = shift;
	$self->_genLinkerFile()
}

sub beginMemoryBlockMappings
{
	$self = shift;
	$self->_setCurrentBlock(@_);
}

sub endMemoryBlockMappings
{
	$self = shift;
	$self->{'currentBlock'} = undef;
}

sub printComment
{
	$self = shift;
	my (@comments) = @_;
	$self->_setCurrentBlock(undef);
	$self->_addString("; $_") foreach(@comments);
}

sub printSymbolDefinition
{
	$self = shift;
	my ($symbol, $value, $global, $attrFIXED, $align) = @_;
	my $sym_align = (defined $align) ? $align : $self->{osecTypes}{'SYMBOL'}->{'align'};

	# is it a local or global symbol ?
	if(defined $global) {
		my $alignment = (defined $sym_align) ? "ALIGN $sym_align" : '';
		# check if input is already available but no mem section was defined,
		# can only happen if comments were previously added, if so then assume
		# that the comments were provided for the global symbol
		if(defined $self->{'currentBlock'}
			&& not defined $self->{'currentBlock'}{'name'}) {
			foreach (@{$self->{'currentBlock'}{'input'}}) {
				push(@{$self->{'symbols'}}, $_);
			}
			$self->{'currentBlock'} = undef;
			pop (@{$self->{'content'}});
		}

		# now add the global symbol to internal array
		my $fixed = (defined $attrFIXED) ? $attrFIXED : '';
		$value =~ s/"//g;
		push(@{$self->{'symbols'}}, "$symbol $value $alignment EMPTY $fixed 0 { }\n");
	} else {
		# local symbols must be defined inside of a mem section
		die "symbol: $symbol must be defined inside of a memory section!"
			if not defined $self->{'currentBlock'};
		if (($value eq '.') or ($value eq '0')) {
			$value = '+0'
		}
		$self->_addSymbol($symbol, $sym_align, $value, $attrFIXED);
	}
}

sub printOutputSection
{
	$self = shift;
	die "output sections can only be defined inside of a memory section!"
		if not defined $self->{'currentBlock'};
	$self->_addOutputSection(@_);
}

sub printBufferGap
{
	$self = shift;
	my ($start, $end, $gap) = @_;
	my $gapSize;
	my $gapName = "buffer_gap_$self->{gapCounter}";
	die "buffer gaps can only be defined inside of a memory section!"
		if not defined $self->{'currentBlock'};

	if (defined $start && defined $end) {
		$gapSize = $end - $start;
	} else {
		$gapSize = $gap;
	}

	$self->_addString("; gap between regions");
	$self->_addString("$gapName +0 EMPTY $gapSize { }");
	$self->{gapCounter} += 1;
}

# private functions ###########################################################

# _setCurrentBlock - depending on the current situation, this function selects
#                    the current hashmap or creates a new one to store provided
#                    data
# $name - name of the current memory section
#
# For each memory section a hashmap is used to store all provided data, this
# function selects an already created hashmap or creates a new one. This is
# needed to merge separated function calls to the same memory section. The
# function also checks if a comment was already added without previously calling
# the beginMemoryBlockMappings function where the hashmap is normally created
# and the comment should be stored. Is this the case, then it is assumed that
# the comment belongs to the newly created memory section and copy it appropriately.
# ####
sub _setCurrentBlock
{
	my $self = shift;
	my ($name) = @_;
	my @found = ();

	# currently no block is choosen
	if(not defined $self->{'currentBlock'}) {
		if(defined $name) {
			# check if a memory section with the same name already exists
			@found = grep{$_->{'name'} eq "$name"} @{$self->{'content'}};

			if(@found != 0) {
				$self->{'currentBlock'} = $found[0];
			}
		}

		if(@found == 0) {
			$self->{'currentBlock'} = ();
			$self->{'currentBlock'}{'name'} = (defined $name ) ? $name : undef;
			$self->{'currentBlock'}{'input'} = [];
			push(@{$self->{'content'}}, $self->{'currentBlock'});
		}
	} else {
		if(defined $name) {
			# check if a memory section with the same name already exists
			@found = grep{$_->{'name'} eq "$name"} @{$self->{'content'}};

			if(@found == 0) {
				$self->{'currentBlock'}{'name'} = $name;
			} else {
				push(@{$found[0]->{'input'}}, @{$self->{'currentBlock'}{'input'}});
				$self->{'currentBlock'} = @found;
				pop (@{$self->{'content'}});
			}
		}
	}
}

# _addString - add a new string to current selected data block (hashmap)
# $string - string to save
# ###
sub _addString
{
	my $self = shift;
	my ($string) = @_;
	push(@{$self->{'currentBlock'}{'input'}}, "$string");
}

# _addSymbol - add a new symbol to currently selected hashmap
# $symbol - name of the symbol
# $align  - align provided symbol to given address boundary
# ###
sub _addSymbol
{
	my $self = shift;
	my ($symbol, $align, $value, $attrFIXED) = @_;
	my $alignment = (defined $align) ? "ALIGN $align" : '';
	my $addr = (defined $value) ? $value : '+0';
	my $fixed = (defined $attrFIXED) ? $attrFIXED : '';
	$self->_addString("$symbol $addr $alignment EMPTY $fixed 0 { }");
}

# _val_to_dec - convert provided hex value to decimnal format
# $value - hex value
# ###
sub _val_to_dec
{
	my $self = shift;
	my ($value) = @_;

	if ($value =~ /^0x[0-9A-F]+$/i) {
		$value = hex($value);
	}

	return $value;
}

# _addOutputSection - stores all provided data of an output section to currently
#                     selected hashmap
# $osecName      - name output section
# $osecType      - type output section
# $inFiles       - array of input files
# $inSections    - provided input sections
# $startSymbol   - create start symbol
# $endSymbol     - create end symbol
# $initSymbol    - create init start symbol
# $initEndSymbol - create init end symbol
# $preciseFiles  - Use precise input file names
# $startAlign    - overrides the alignment of the output section
# $endAlign      - overrides the alignment of init end symbol
# ###
sub _addOutputSection
{
	my $self = shift;
	my ($osecName, $osecType, $inFiles, $inSections, $startSymbol, $endSymbol,
		$initSymbol, $initEndSymbol, $preciseFiles, $startAlign, $endAlign) = @_;

	# sanity checks
	die "section name not set!" if not defined $osecName;
	die "section type not set!" if not defined $osecType;
	die "Parameter preciseFiles not yet implemented for the ARM Keil Toolchain!"
		if defined $preciseFiles;

	# set internal variables
	my $ot = $self->{osecTypes}{$osecType};

	# assemble execution region
	$self->_addString("; Output section $osecName (Type $osecType)");
	my $attrUNINIT = (defined $ot->{_armkeil_attr_uninit}) ? $ot->{_armkeil_attr_uninit} : '';
	my $attrFIXED = (defined $ot->{_armkeil_attr_fixed}) ? $ot->{_armkeil_attr_fixed} : '';
	my $attrEMPTY = (defined $ot->{_armkeil_attr_empty}) ? $ot->{_armkeil_attr_empty} : '';

	# Override region alignment from architecture definition
	if (defined $startAlign) {
		$startAlign = $self->_val_to_dec($startAlign);
	}

	# Override end alignment from architecture defintion
	if (defined $endAlign) {
		$endAlign = $self->_val_to_dec($endAlign);
	}

	# Get the alignment from the architecture configuration file or
	# use the size argument to overwrite the alignment. (e.g. for MPU)
	# Trusted function stacks have a size of 0x0, therefore align it to
	# the minimum boundary.
	$startAlign = $ot->{align} if (!defined($startAlign) || $startAlign == 0);

	# add init and start symbols if provided
	$self->_addSymbol($initSymbol, $startAlign, undef, $attrFIXED) if defined $initSymbol;
	$self->_addSymbol($startSymbol, $startAlign, undef, $attrFIXED) if defined $startSymbol;

	# check if provided files or sections are empty or not set
	# if so than create just a symbol
	if(defined $ot->{_armkeil_attr_empty}) {
		$self->_addString("$osecName +0 ALIGN $startAlign $attrUNINIT $attrEMPTY $attrFIXED $osecType\_size");
	} else {
		$self->_addString("$osecName +0 ALIGN $startAlign $attrUNINIT $attrFIXED");
	}
	$self->_addString("{");

	my $files  = $inFiles || [ '*' ];
	my $isects = $inSections || $ot->{isectDefault};
	my @isectSelectors = map { $_ , "$_.*" } @$isects;

	foreach my $file (@$files) {
		my $modsel;

		if (ref $file eq 'HASH') {
			die "KEIL does not support the selection of archive members $file->{lib}($file->{file})\n";
			$modsel = $file->{file};
		} else {
			$modsel = $file;
		}

		# each selection statement must be enclosed in double
		# quotes if wildcards are used
		foreach my $selector (@isectSelectors) {
			$self->_addString("$modsel($selector)");
		}
	}

	$self->_addString("}");

	# add end and init_end symbols if provided
	$endAlign = $ot->{endalign} if (!defined($endAlign) || $endAlign == 0);

	$self->_addSymbol($endSymbol, undef, undef, $attrFIXED) if defined $endSymbol;
	$self->_addSymbol($initEndSymbol, $endAlign, undef, $attrFIXED) if defined $initEndSymbol;
	$self->_addString(" ");
}

# _genLinkerFile - this is where the linker script is actually generated
#
# iterate through the internal hashmap array and print all provided content to
# a file. all global symbols are store at an extra array and should be put at
# the end of the linker script, if some linker dependent functions are used,
# otherwise it doesn`t matter where they are printed.
# ###
sub _genLinkerFile
{
	my $self = shift;

	# print all provided memory regions
	foreach my $item (@{$self->{'content'}}) {
		if(defined $item) {
			$self->_writeLoadSection($item->{'name'}, \@{$item->{'input'}});
		}
	}

	# print global symbols
	if(@{$self->{'symbols'}}) {
		GenldCommon::printOut("GLOBAL_SYMBOLS 0");
		GenldCommon::printOut("{");
		foreach (@{$self->{'symbols'}}) {
			GenldCommon::printOut("$_");
		}
		GenldCommon::printOut("}\n");
	}
}

# _writeLoadSection - write a load section to file
# $name - name of the memory section
# $input - array with the provided content
# ###
sub _writeLoadSection
{
	my $self = shift;
	my ($name, $input) = @_;

	if(defined $name) {
		my $addr = "$name" . '_addr';
		my $size = "$name" . '_size';

		GenldCommon::printOut("$name $addr $size");
		GenldCommon::printOut("{");
		foreach (@$input) {
			GenldCommon::printOut("$_");
		}
		GenldCommon::printOut("}\n");
	} else {
		# no name provided, must be a comment
		foreach (@$input) {
			GenldCommon::printOut("$_");
		}
	}
}
