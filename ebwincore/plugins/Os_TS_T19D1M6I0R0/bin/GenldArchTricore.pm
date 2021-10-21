package GenldArchTricore;

=head1 The C<GenldArchTricore> package

This package collects objects and functions that are needed for the TriCore architecture. This
package's documentation only describes the specifics for this architecture. The common documentation
can be found in L<GenldArch>.

=cut

our %memoryBlocks =
(
	'initdata_memory' => 'int_rom',
);

# TC3XX CODE regions and SMPU regions in general require 32 byte alignment.
# Therefore the minimum alignment for each section which could be the start of a
# memory-protection-region should be at least aligned to 32 byte.

our %osecTypes =
(
	'RESET'   =>
	{
		align           => 4,
		isectDefault    => [qw(.reset)],

		_diab_typedesc  => 'TEXT',
		_diab_extraattr => 'KEEP',

		_tasking_secattrs => [ 'fill=0', 'attributes=rx', 'blocksize=4' ],
	},
	'TEXT'    =>
	{
		align        => 32,
		endalign     => 32,
		isectDefault => [qw(.text)],

		_diab_typedesc => 'TEXT',

		_tasking_secattrs => [ 'fill=0', 'attributes=rx' ],

	},
	'SMALL_RODATA' =>
	{
		# Small read only data.
		# Place this before 'RODATA', if you want to separate "small rodata" from "rodata".
		# Otherwise 'RODATA' will collect all read only data.
		align        => 32,
		endalign     => 32,
		isectDefault => [qw(.ldata)],

		_diab_typedesc => 'CONST',

		_tasking_secattrs => [ 'fill=0', 'attributes=r' ],
	},
	'RODATA'  =>
	{
		align        => 32,
		endalign     => 32,
		isectDefault => [qw(.rodata .zrodata .ldata)],

		_diab_typedesc => 'CONST',

		_tasking_secattrs => [ 'fill=0', 'attributes=r' ],
	},
	'DATA'    =>
	{
		align        => 64,
		endalign     => 64, # must be at least BSS.align, so there are no gaps.
		isectDefault => [qw(.data .zdata .sdata)],

		_diab_typedesc => 'DATA',

		_tasking_secattrs => [ 'attributes=rw' ],
	},
	'SDATA'   =>
	{
		align        => 64,
		endalign     => 64, # must equal DATA.align, so there are no gaps.
		isectDefault => [qw(.sdata)],

		_diab_typedesc => '-',

		_tasking_secattrs => [ 'attributes=rw' ],
	},
	'BSS'            =>
	{
		# Note: BSS typically follows a DATA section. There must be no gap between them,
		#       otherwise uninitialized flash could be read during DATA initialization.
		#       Under certain circumstances linkers align sections to 64 byte,
		#       so the alignment must be at least 64.
		align        => 64,
		endalign     => 32,
		isectDefault => [qw(.bss .zbss .sbss .lbss)],
		# Note: ".lbss" is not described in diab's manual 5.9.6-E3 (related to -Xsmall-const).

		_diab_typedesc => 'BSS',

		_tasking_groupattrs => [ 'nocopy' ],
		_tasking_secattrs => [ 'attributes=rw' ],

		_gnu_extraattr    => 'NOLOAD',
	},
	'SBSS'           =>
	{
		align        => 64,
		endalign     => 32,
		isectDefault => [qw(.sbss)],

		_diab_typedesc => '-',

		_tasking_groupattrs => [ 'nocopy' ],
		_tasking_secattrs => [ 'attributes=rw' ],

		_gnu_extraattr    => 'NOLOAD',
	},
	'STACK'   =>
	{
		align        => 64,
		endalign     => 8,
		isectDefault => [qw(.bss .zbss .sbss)],

		_diab_typedesc => 'BSS',

		_tasking_groupattrs => [ 'nocopy' ],
		_tasking_secattrs   => [ 'attributes=rw' ],

		_gnu_extraattr    => 'NOLOAD',
	},
	'_TASKING_INITDATA' =>
	{
		isectDefault => [qw(.data .zdata .sdata)],
		align        => 64,
		endalign     => 64,

		_diab_typedesc => 'INITDATA',

		_tasking_secattrs   => [ 'fill=0', 'attributes=r' ],
	},
);

# This function is invoked by GenldCommon::parseArgs() if unknown arguments are encountered
sub parseArgs
{
	my ($arg, $sysdesc, $argv_position) = @_;

	if($arg eq '-corecsas')
	{
		GenldCommon::setArg($sysdesc, $arg, ++$$argv_position);
		$sysdesc->{$arg} = [ split(',', $sysdesc->{$arg}) ];
		if(grep { !/(^[1-9]\d*$)|(^0$)/ } @{$sysdesc->{$arg}})
		{
			GenldCommon::usage("$arg expects a comma-separated list of non-negative decimal integers");
		}
	}
	elsif ($arg eq '-map-all-user-ram-to-core')
	{
		GenldCommon::setArg($sysdesc, $arg, ++$$argv_position);
		my $coreId = $sysdesc->{$arg};
		GenldCommon::usage("$arg expects a non-negative decimal integer") if ($coreId !~ /^\d+$/);
		$sysdesc->{$arg} = int $coreId;
	}
	elsif ($arg eq '-mix-in-sda')
	{
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
Additional options specific to TriCore:

  -corecsas <CORE0_NCSA>[,<CORE1_NSA> ...]:
   Comma-separated list with number of CSAs for each core

  -map-all-user-ram-to-core <COREID>:
   Maps all user specific data to the RAM of the given core.
   That is the private data of tasks, ISRs and applications.
   This option is NOT recommended. It is meant for testing the OS in the presence of SDAs.
   __USER_DATA_CORE_OVERRIDE_BEGIN is set to the beginning of the remapped user specific data.

  -mix-in-sda:
   This option mixes in SBSS and SDATA variables whenever you match BSS or DATA
   without explicit input section specification.
   The SBSS and SDATA variables are put into separate .sbss* and .sdata* sections.
   This option should be used together with -map-all-user-ram-to-core.

EOUSAGE

}

1;

# Editor settings: DO NOT DELETE
# vi:set ts=4 sw=4 noet tw=100:
