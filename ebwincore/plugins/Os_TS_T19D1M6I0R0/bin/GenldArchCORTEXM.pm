package GenldArchCORTEXM;

=head1 The C<GenldArchCORTEXM> package

This package collects objects and functions that are needed for the CORTEXM
architecture. It allows sharing these objects between the generator scripts
of the AutoCore OS and the Microkernel for CORTEXM.

=head2 Objects provided by this package

=over

=item %osecTypes

This object defines the characteristics of the different types of output
sections on the CORTEXM architecture. The keys of this hash are the section
types as given to printOutputSection. The value is a hash that contains the
characteristics of the corresponding section type.

=over

=item align

Defines the minimum alignment of the output section in bytes.

=item endalign

Defines the minimum alignment of end symbol in bytes. The output section will be enlarged accordingly. If not specified, the end alignment is 1.

=item isectDefault

Defines the default set of input sections in case no explicit set of input
sections is given to printOutputSection.

=back

=back

=cut

our %memoryBlocks = (
	'initdata_memory' => 'rom',

);

our %osecTypes =
(
	'RESET'   =>
	{
		align           => 256,
		endalign        => 256,
		isectDefault    => [qw(.reset)],

		_diab_typedesc  => 'TEXT',
		_diab_extraattr => 'KEEP',

		_arm5_load_group => 'reset',
		_arm5_attr_fixed => 'FIXED',

		_armkeil_attr_fixed => 'FIXED',
	},
	'TEXT'    =>
	{
		align        => 32,
		endalign     => 32,
		isectDefault => [qw(.text)],

		_diab_typedesc => 'TEXT',

		_arm5_load_group => 'rom',
		_arm5_attr_fixed => 'FIXED',

		_armkeil_attr_fixed => 'FIXED',
	},
	'RODATA'  =>
	{
		align        => 32,
		endalign     => 32,
		isectDefault => [qw(.rodata .zrodata .ldata)],

		_diab_typedesc => 'CONST',

		_arm5_load_group => 'rom',
		_arm5_attr_fixed => 'FIXED',

		_armkeil_attr_fixed => 'FIXED',
	},
	'DATA'    =>
	{
		align        => 32,
		endalign     => 32,
		isectDefault => [qw(.data .zdata .sdata)],

		_diab_typedesc => 'DATA',

		_arm5_load_group => 'rom',
	},
	'DATA_BSS' =>
	{
		align        => 32,
		endalign     => 32,
		isectDefault => [qw(.bss .zbss .sbss)],

		_diab_typedesc => 'BSS',

		_gnu_extraattr    => 'NOLOAD',

		_arm5_load_group => 'rom',
		_arm5_attr_uninit => 'UNINIT',

		_armkeil_attr_uninit => 'UNINIT',
	},
	'BSS'            =>
	{
		align        => 32,
		endalign     => 32,
		isectDefault => [qw(.bss .zbss .sbss)],

		_diab_typedesc => 'BSS',

		_gnu_extraattr    => 'NOLOAD',

		_arm5_load_group => 'rom',
		_arm5_attr_uninit => 'UNINIT',
		_arm5_attr_fixed => 'FIXED',

		_armkeil_attr_uninit => 'UNINIT',
	},
	'STACK'   =>
	{
		align        => 32,
		endalign     => 32,
		isectDefault => [qw(.bss .zbss .sbss)],

		_diab_typedesc => 'BSS',

		_gnu_extraattr    => 'NOLOAD',

		_arm5_load_group => 'rom',
		_arm5_attr_fixed => 'FIXED',

		_armkeil_attr_uninit => 'UNINIT',
	},
	'SYMBOL'   =>
	{
		align        => 256,
	},
);

# This function is invoked by GenldCommon::parseArgs() if unknown arguments are encountered
sub parseArgs
{
	my ($arg, $sysdesc, $argv_position) = @_;

	if($arg eq '-emitResetSection')
	{
		GenldCommon::setArg($sysdesc, $arg, $$argv_position);
		$sysdesc->{$arg} = 1;
	}elsif ($arg eq '-mpuMinRegionSize')
	{
		GenldCommon::setArg($sysdesc, $arg, ++$$argv_position);
		my $mpuMinRegionSize = $sysdesc->{$arg};
		GenldCommon::usage("$arg expects a non-negative decimal integer") if ($mpuMinRegionSize !~ /^\d+$/);
		$sysdesc->{$arg} = int($mpuMinRegionSize);
	}
	else
	{
		return 0;
	}
	return 1;
}

sub usage
{
	print STDERR <<EOUSAGE;
Additional options specific to CORTEXM:

  -emitResetSection:
  Emit an output section that collects .reset sections into a dedicated reset memory block.

  -mpuMinRegionSize <REGION_SIZE>:
  Sets the smallest possible region size of the Cortex-M MPU. This is used to
  determine the minimum alignment of a memory region that may be programmed into
  the MPU.

EOUSAGE

}
1;

# Editor settings: DO NOT DELETE
# vi:set ts=4 sw=4 noet tw=100:
