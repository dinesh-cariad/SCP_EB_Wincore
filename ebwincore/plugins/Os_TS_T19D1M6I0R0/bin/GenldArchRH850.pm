package GenldArchRH850;

=head1 The C<GenldArchRH850> package

This package collects objects and functions that are needed for the RH850 architecture. This
package's documentation only describes the specifics for this architecture. The common documentation
can be found in L<GenldArch>.

=cut

our %memoryBlocks =
(
	'initdata_memory' => 'rom_a',
);

our %osecTypes =
(
	'RESET'   =>
	{
		align        => 512,
		isectDefault => [qw(.reset)],
	},
	'TEXT'    =>
	{
		align        => 16,
		isectDefault => [qw(.text)],
	},
	'RODATA'  =>
	{
		align        => 16,
		isectDefault => [qw(.rodata .zrodata)],
	},
	'DATA'    =>
	{
		align        => 4,
		isectDefault => [qw(.data .zdata .sdata)],
	},
	'BSS'            =>
	{
		align        => 4,
		isectDefault => [qw(.bss .zbss .sbss)],

		_ghs_type    => 'CLEAR'
	},
	'STACK'   =>
	{
		align        => 16,
		isectDefault => [qw(.bss .zbss .sbss)],

		_ghs_type    => 'NOLOAD'
	},
	'PLAIN'   =>
	{
		align        => 0,
		isectDefault => [qw(.plain)],
	},
);

# This function is invoked by GenldCommon::parseArgs() if unknown arguments are encountered
sub parseArgs
{
	my ($arg, $sysdesc, $argv_position) = @_;

	if($arg eq '-has_globalram')
	{
		GenldCommon::setArg($sysdesc, $arg);
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
Additional options specific to RH850:

  -has_globalram:
   Create a region mapping data to the global RAM memory. Do not use this option on derivatives that
   do not have a global RAM.

EOUSAGE

}

1;

# Editor settings: DO NOT DELETE
# vi:set ts=4 sw=4 noet tw=100:
