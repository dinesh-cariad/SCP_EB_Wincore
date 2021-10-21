package GenldArchARM;

=head1 The C<GenldArchARM> package

This package collects objects and functions that are needed for the ARM architecture. This package's
documentation only describes the specifics for this architecture. The common documentation can be
found in L<GenldArch>.

=cut

our %memoryBlocks =
(
	'initdata_memory' => 'FLASH',
);


our %osecTypes =
(
	'MMU' =>
	{
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
		'DATA_BSS' =>
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
		'SYMBOL'   =>
		{
			align        => 4096,
		},
	},
	'MPU' =>
	{
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
			endalign     => 8,
			isectDefault => [qw(.text)],

			_diab_typedesc => 'TEXT',

			_armkeil_attr_fixed => 'FIXED'
		},
		'RODATA'  =>
		{
			align        => 8,
			endalign     => 8,
			isectDefault => [qw(.rodata .zrodata .ldata)],

			_diab_typedesc => 'CONST',

			_armkeil_attr_fixed => 'FIXED'
		},
		'DATA'    =>
		{
			align        => 8,
			endalign     => 8,
			isectDefault => [qw(.data .zdata .sdata)],

			_diab_typedesc => 'DATA'
		},
		'DATA_BSS' =>
		{
			align        => 8,
			endalign     => 8,
			isectDefault => [qw(.bss .zbss .sbss)],

			_diab_typedesc => 'BSS',

			_gnu_extraattr    => 'NOLOAD',

			_armkeil_attr_uninit => 'UNINIT'
		},
		'BSS'            =>
		{
			align        => 8,
			endalign     => 8,
			isectDefault => [qw(.bss .zbss .sbss)],

			_diab_typedesc => 'BSS',

			_gnu_extraattr    => 'NOLOAD',

			_armkeil_attr_uninit => 'UNINIT',
			_armkeil_attr_fixed => 'FIXED'
		},
		'STACK'   =>
		{
			align        => 8,
			endalign     => 8,
			isectDefault => [qw(.bss .zbss .sbss)],

			_diab_typedesc => 'BSS',

			_gnu_extraattr    => 'NOLOAD',

			_armkeil_attr_fixed => 'FIXED'
		},
		'SYMBOL'   =>
		{
			align        => 8,
		},
	},
	'MPU_V8R' =>
	{
		'RESET'   =>
		{
			align           => 64,
			endalign        => undef,
			isectDefault    => [qw(.reset)],

			_diab_typedesc  => 'TEXT',
			_diab_extraattr => 'KEEP',

			_armkeil_attr_fixed => 'FIXED'
		},
		'BOOTLOADER'	=>
		{
			align           => 64,
			endalign        => undef,
			isectDefault    => [qw(.bootloader)],

			_diab_typedesc  => 'TEXT',
			_diab_extraattr => 'KEEP',

			_armkeil_attr_fixed => 'FIXED'
		},
		'TEXT'    =>
		{
			align        => 64,
			endalign     => 64,
			isectDefault => [qw(.text)],

			_diab_typedesc => 'TEXT',

			_armkeil_attr_fixed => 'FIXED'
		},
		'RODATA'  =>
		{
			align        => 64,
			endalign     => 64,
			isectDefault => [qw(.rodata .zrodata .ldata)],

			_diab_typedesc => 'CONST',

			_armkeil_attr_fixed => 'FIXED'
		},
		'DATA'    =>
		{
			align        => 64,
			endalign     => 64,
			isectDefault => [qw(.data .zdata .sdata)],

			_diab_typedesc => 'DATA'
		},
		'DATA_BSS' =>
		{
			align        => 64,
			endalign     => 64,
			isectDefault => [qw(.bss .zbss .sbss)],

			_diab_typedesc => 'BSS',

			_gnu_extraattr    => 'NOLOAD',

			_armkeil_attr_uninit => 'UNINIT'
		},
		'BSS'            =>
		{
			align        => 64,
			endalign     => 64,
			isectDefault => [qw(.bss .zbss .sbss)],

			_diab_typedesc => 'BSS',

			_gnu_extraattr    => 'NOLOAD',

			_armkeil_attr_uninit => 'UNINIT',
			_armkeil_attr_fixed => 'FIXED'
		},
		'STACK'   =>
		{
			align        => 64,
			endalign     => 64,
			isectDefault => [qw(.bss .zbss .sbss)],

			_diab_typedesc => 'BSS',

			_gnu_extraattr    => 'NOLOAD',

			_armkeil_attr_fixed => 'FIXED'
		},
		'SYMBOL'   =>
		{
			align        => 64,
		},
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
	elsif($arg eq '-memprottype')
	{
		GenldCommon::setArg($sysdesc, $arg, ++$$argv_position);
		GenldCommon::usage("$arg expects one of MMU, MPU, or MPU_V8R") unless defined $osecTypes{$sysdesc->{'-memprottype'}};
	}
	elsif($arg eq '-emptyRegionAddress')
	{
		GenldCommon::setArg($sysdesc, $arg, ++$$argv_position);
		unless ($sysdesc->{'-emptyRegionAddress'} =~ m/^0x(\d+)$/)
		{
			GenldCommon::usage("$arg must be set to an address in the form of \"0xdeadbeef\"");
		}
		GenldCommon::usage("$arg must not be 0") if int($1) == 0;
	}
	# disable generation of an additional ExceptionTable_EL2
	elsif($arg eq '-no-exctable-el2')
	{
		GenldCommon::setArg($sysdesc, $arg, $$argv_position);
		$sysdesc->{$arg} = 1;
	}
	# exception table in ram (when ram is at 0x00, Eg:S6J3300)
	elsif($arg eq '-ram_at_lower_addr')
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
Additional options specific to ARM:

  -aliasExceptionTable:
   Alias MK_RamExceptionTable to MK_ExceptionTable. For targets with a freely programmable vector base address.

  -memprottype:
   Used memory protection variant. May be one of MMU, MPU, or MPU_V8R. Defaults to MMU.

  -emptyRegionAddress:
   Address where emtpy regions shall be mapped to. Preferably, this should be mapped to an otherwise unused address.
   The address must not be 0.

  -no-exctable-el2: Disable generation of an additional ExceptionTable_EL2.

  -ram_at_lower_addr: Fixed exception table at address 0x0 is in RAM

EOUSAGE

}
1;

# Editor settings: DO NOT DELETE
# vi:set ts=4 sw=4 noet tw=100:
