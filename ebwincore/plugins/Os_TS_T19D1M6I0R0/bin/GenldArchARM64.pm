package GenldArchARM64;

=head1 The C<GenldArchARM64> package

This package collects objects and functions that are needed for the ARM64 architecture. This
package's documentation only describes the specifics for this architecture. The common documentation
can be found in L<GenldArch>.

=cut

our %memoryBlocks = ();

our %osecTypes =
(
	'RESET'   =>
	{
		align           => 2048,
		endalign        => undef,
		isectDefault    => [qw(.reset)],

		_diab_typedesc  => 'TEXT',
		_diab_extraattr => 'KEEP',

		_armkeil_attr_fixed => 'FIXED'
	},
	'BOOTLOADER'	=>
	{
		align           => 2048,
		endalign        => undef,
		isectDefault    => [qw(.bootloader)],

		_diab_typedesc  => 'TEXT',
		_diab_extraattr => 'KEEP',

		_armkeil_attr_fixed => 'FIXED'
	},
	'TEXT'    =>
	{
		align        => 8,
		endalign     => 4096,
		isectDefault => [qw(.text)],

		_diab_typedesc => 'TEXT',

		_armkeil_attr_fixed => 'FIXED'
	},
	'RODATA'  =>
	{
		align        => 8,
		endalign     => 4096,
		isectDefault => [qw(.rodata .zrodata .ldata)],

		_diab_typedesc => 'CONST',

		_armkeil_attr_fixed => 'FIXED'
	},
	'DATA'    =>
	{
		align        => 4096,
		endalign     => 4096,
		isectDefault => [qw(.data .zdata .sdata)],

		_diab_typedesc => 'DATA'
	},
	'DATA_BSS'       =>
	{
		align        => 8,
		endalign     => 4096,
		isectDefault => [qw(.bss .zbss .sbss)],

		_diab_typedesc => 'BSS',

		_gnu_extraattr    => 'NOLOAD',

		_armkeil_attr_uninit => 'UNINIT'
	},
	'BSS'            =>
	{
		align        => 8,
		endalign     => 4096,
		isectDefault => [qw(.bss .zbss .sbss)],

		_diab_typedesc => 'BSS',

		_gnu_extraattr    => 'NOLOAD',

		_armkeil_attr_uninit => 'UNINIT',
		_armkeil_attr_fixed => 'FIXED'
	},
	'STACK'   =>
	{
		align        => 4096,
		endalign     => 4096,
		isectDefault => [qw(.bss .zbss .sbss)],

		_diab_typedesc => 'BSS',

		_gnu_extraattr    => 'NOLOAD',

		_armkeil_attr_fixed => 'FIXED'
	},
	'HEAP'   =>
	{
		align        => 4096,
		endalign     => 4096,
		isectDefault => [],

		_diab_typedesc => 'BSS',

		_armkeil_attr_empty => 'EMPTY',
		_armkeil_attr_fixed => 'FIXED'
	},
	'SYMBOL' =>
	{
		align        => 4096,
	},
);

# This function is invoked by GenldCommon::parseArgs() if unknown arguments are encountered
sub parseArgs
{
	my ($arg, $sysdesc, $argv_position) = @_;

	if($arg eq '-aliasExceptionTable')
	{
		GenldCommon::setArg($sysdesc, $arg, $$argv_position);
		$sysdesc->{$arg} = 1;
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
Additional options specific to ARM64:

  -aliasExceptionTable:
   Alias MK_RamExceptionTable to MK_ExceptionTable. For targets with a freely programmable vector base address.

EOUSAGE

}
1;

# Editor settings: DO NOT DELETE
# vi:set ts=4 sw=4 noet tw=100:
