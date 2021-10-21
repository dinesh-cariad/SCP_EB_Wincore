package GenldArchPA;

=head1 The C<GenldArchPA> package

This package collects objects and functions that are needed for the PA architecture. This
package's documentation only describes the specifics for this architecture. The common documentation
can be found in L<GenldArch>.

=cut

our %memoryBlocks =
(
	'initdata_memory' => 'rom',
);

our %osecTypes =
(
	'RESET'   =>
	{
		align           => 4,
		isectDefault    => [qw(.reset)],

		_diab_typedesc  => 'TEXT',
		_diab_extraattr => 'KEEP',
	},
	'TEXT'    =>
	{
		align        => 16,
		isectDefault => [qw(.text)],

		_diab_typedesc => 'TEXT',
	},
	'RODATA'  =>
	{
		align        => 16,
		isectDefault => [qw(.rodata .zrodata .ldata)],

		_diab_typedesc => 'CONST',
	},
	'DATA'    =>
	{
		align        => 32,
		isectDefault => [qw(.data .zdata .sdata)],

		_diab_typedesc => 'DATA',
	},
	'BSS'            =>
	{
		align        => 32,
		isectDefault => [qw(.bss .zbss .sbss)],

		_diab_typedesc => 'BSS',

		_gnu_extraattr    => 'NOLOAD',
	},
	'STACK'   =>
	{
		align        => 32,
		isectDefault => [qw(.bss .zbss .sbss)],

		_diab_typedesc => 'BSS',

		_gnu_extraattr    => 'NOLOAD',
	},
);

# This function is invoked by GenldCommon::parseArgs() if unknown arguments are encountered
sub parseArgs
{
	my ($arg, $sysdesc, $argv_position) = @_;

	return 0;
}

sub usage
{

}
1;

# Editor settings: DO NOT DELETE
# vi:set ts=4 sw=4 noet tw=100:
