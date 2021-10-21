package GenldCommon;

# Read the documentation e.g. using
# pod2man GenldCommon.pm | man -l -
# Or if you fancy HTML:
# pod2html GenldCommon.pm >GenldCommon.html

=head1 NAME

GenLd Linker Script Generator Framework - C<GenldCommon> Module

=head1 SYNOPSIS

Please refer to the usage information provided by the script on information on the parameters.
Because of the restricted Perl environment this generator is designed to be run in (i.e. even the
core modules of the Perl distribution are not available), the usage message cannot be extracted from
POD and is therefore omitted here to avoid duplication.

Generally, there is one linker script generator C<< genld-<TARGET>.pl >> per architecture which is
invoked from the board file. The invocation will normally involve the usage of the parameters:

=over

=item C<--backend> to select the toolchain-specific back end

It is important to select the back end on the command line I<before> any back-end-specific options
are specified. It's a good idea to have it as the very first parameter in the script invocation.

=item any number of C<--include> to include static parts of the linker script

Parts of the linker script that do not depend on any configuration can be stored in static files to
simplify the linker script generator and in particular to reduce the feature set that the back ends
need to implement. The most prominent example of a typically static part of the linker script is the
definitions of the different memory blocks. L<GenldBackend> does not include functionality to create
such memory definitions in a toolchain-agnostic way.

=back

=head1 DESCRIPTION

The linker script generator framework is intended to produce simple linker scripts for an OS
configuration, attempting to separate the specifics of a particular toolchain used from the
specifics of the target platform.

The framework is structured into different classes of modules/scripts:

=over

=item C<GenldCommon>

This module holds common utility functions that are shared by many target/toolchain combinations,
for example parsing the command line arguments of the script.

=item Architecture-specific property definitions (C<< GenldArch<TARGET> >>)

For each target architecture C<TARGET>, the module C<< GenldArch<TARGET> >> defines target-specific
properties such as the alignment required for the different types of output sections.

These packages may add target-specific command-line options if needed.

=item Toolchain-specific backends (L<GenldBackend> and descendents C<< Genld<TOOL> >>)

For each supported toolchain C<TOOL>, the module C<< Genld<TOOL> >> implements the linker back-end
interface defined by L<GenldBackend> for the specific toolchain. This package abstracts the syntax
of the toolchain-specific linker-script language.

These packages may add toolchain-specific command-line options if needed.

=item Architecture-specific generator script (C<< genld-<TARGET>.pl >>)

For each target architecture C<TARGET>, there is a linker script generator script that goes by the
name C<< genld-<TARGET>.pl >>. This script is meant to describe the memory layout needed for a
particular target in a toolchain-independent way, relying on the toolchain-specific back-end module
to actually create the linker script.

The generator script may add target-specific command-line options if needed.

=back

=head2 Utility functions provided by this package

=over

=item C<usage(@msg)>

Outputs the usage message which describes the invocation of the linker script generator and the
known parameters. An optional message may be given in the arguments which is prepended to the usage
text, commonly used if the usage information is printed as reaction to a wrong-usage error detected
by the script.

The parameters can be extended by architecture-specific and toolchain-specific arguments, for which
the corresponding modules should provide separate functions that provide the usage information for
those parameters. These functions can be registered by adding references to them to the
C<@usage_fns> variable of this package. The functions do not get any parameters. They are invoked in
the order of the array after printing the generic usage information.

=item C<parseArgs($arch_parser_fn)>

This is the main parser function for the command line arguments. It parses the common arguments and
delegates parsing of unknown arguments to the C<parseArgs($arg, \%sysdesc, \$i)> function of
L<GenldBackend> and the optional architecture-specific argument parser function given by the
C<$arch_parser_fn> parameter. These functions are supposed to return a non-zero value in case the
argument was recognized, add the needed information to C<%sysdesc> and increment <$i> to skip any
further parameters that may have been part of the parsed argument.

The C<%sysdesc> hash produced by the argument parser by convention should contain a hash key for
each simple argument with the corresponding value set as the hash value, or a non-zero value in
case of simple flag options. For more complex options (those describing the OS-Application and their
contained OS objects), a nested container structure is created. In this case, the argument name
(e.g. C<-app>) is again used as the hash key, but the corresponding value now references an array of
all the corresponding container objects. Each of these objects is a hash. The hash has an entry with
the key C<contains>, which references an array of further hash keys of the hash pointing to the
contained nested objects (in case of an OS-Application, these might be C<-task>, C<-isr>, etc.).
The values of each of these hash entries will again be references to arrays of container objects,
exactly like C<-app> in the main C<%sysdesc> hash.

The following helper functions are provided to be used by C<parseArgs()>, including
architecture-specific or toolchain-specific C<parseArgs()> functions:

=over

=item C<setArg($container, $arg, $argidx)>

This function is used to add a simple flag argument or an argument getting a simple value as
parameter to the referenced C<$container> (most often, the container will be C<\%sysdesc>). The
parameter C<$container> either references a hash or an array.

The parameter C<$arg> gives the name of the argument (e.g. C<-o>), C<$argidx> gives the index of the
entry following the argument in C<@ARGV> (which would be the parameter of the argument if
applicable). For simple flag options, C<$argidx> is undefined and the value 1 will be used.
If C<$argidx> is given, C<setArg()> verifies the presence of the additional parameter in C<@ARGV>
and dies with a usage message in case it is not present.

If C<$container> references a hash, the value will be set using the argument as key for the hash.
The function also checks whether a value is already present for the argument, and dies in case a
different value has previously been set.

If C<$container> references an array, the value will be appended to this array.

=item C<setHashEntry(\%hash, $type, $argidx)>

This function adds a new entry to the referenced hash for an object that gets or represents a memory
region. The created object's attributes are its C<$type> (e.g., 'Memory Region'), its name as
acquired from C<$ARGV[$argidx]>, and a list of object files which is initialized to the be empty.
The created object is inserted into the referenced hash with the object's name being used as the
key, therefore only one object may exist by that name.

=item C<addUniqueArrayEntry(\%sysdesc, \@array, $type, $argidx)>

This function adds a new entry to the referenced array for an object that gets or represents a
memory region. The created object's attributes are its C<$type> (e.g., 'OS-Application'), its name
as acquired from C<$ARGV[$argidx]>, and a list of object files which is initialized to the be empty.
The created object is appended to the referenced array after checking that no object with the same
name and type currently exists in the referenced system description.
If the given system description C<\%sysdesc> already contains an object of the given type
having the same name, this function invokes C<die>.

=back

=item C<printOut(@lines)>

Prints the lines given as parameters to this function to the linker script. Each parameter is
printed to a line of its own. This function attempts to produce a nicely formatted linker script by
scanning the output for the different types of brackets, computing an indentation level and
automatically indenting the lines to that indentation level. Indentation is done using tabs. Any
manual indentation in the given output lines is replaced with the computed indentation.

This function should exclusively be used to create output for the linker script, otherwise the
mechanism that determines the current indentation level might break because it misses opening or
closing brackets.

=back

=cut

use GenldBackend;

our $outh;
our $indentLevel = 0;
our %sysdesc;

# Additional usage messages by main script and backend
our @usage_fns = ();

# Helpers for parsing the arguments
#
sub usage
{
	if(@_)
	{
		print STDERR <<EOUSAGE;
------------------------------------------------------------------------------
Error: @_

Command line was:
$0 @ARGV
------------------------------------------------------------------------------

EOUSAGE
	}
	print STDERR <<EOUSAGE;
Usage: $0 [Options] [<app-spec> ...] [<region-spec> ...] [<system-object> ...]

Options:
  -backend <\$OS_TOOLCHAIN>: selects the linker backend to use for the generator
  -prepend <line>: prepend <line> to the file (e.g. to add a preprocessor directive)
                   Can be specified multiple times, <lines> are added in the given order.
  -include <infile>: prepend <infile> as static part to the generated linker script.
                     Can be specified multiple times, files are included in the given order.
  -numcores: Number of cores the hardware provides
  -o <outfile>: Name of the output file
  -num-thread-slots <nslots-core0>[,<nslots-core1> ...]:
  -thread-stack-all <stack-thread-0>[,<stack-thread-1> ...]:
   Comma-separated list with number of stack slots for each core
  -thread-stack-all <stack-thread-0>[,<stack-thread-1> ...]:
   Comma-separated list with the sizes of the stack slots
  -text-region-size <size>:
   Set size of the provided text section.
  -const-region-size <size>:
   Set size of the provided rodata section.
  -data-region-size <size>:
   Set size of the provided data section.
  -qmos-data-size <size>:
   Size of the QMOS data/bss region
  -kern-data-size <size>:
   Size of the Microkernel data/bss region
  -mpu-cache-size <size>:
   Size of the MPU cache
  -disableRomInit: Disable generation of data sections in ROM for
    initialization. This is useful for targets without ROM, where some bootloader
    or the debugger loads the binary directly into RAM.
  -size <value>: Size of a region. This parameter is an option to an app-spec, isr-spec, memregsym,
    task-spec or stack-speck to define the region size of the specific element.
  -internal-stack-all <kernel>,<aux1>,<aux2>,<idle shutdown>,<errorHook>,<protectionHook>:
   Comma-separated list with sizes of the internal stacks
   -padBytes :For adding pad byte to make the empty sections non-empty for QEMU (Provided 
   only for QEMU as a workaround)

Application specification:
  <app-spec>: -app <Appname> [-size <value>] [<object.o> ...] <core-mapping> [<task-spec> ...] [<isr-spec> ...] [<memregsym> ...]
  OS-Application. Subsequent task and ISR specifications define tasks and ISRs belonging
  to the application.

Core mapping:
  <core-mapping>: -core <COREID>

  Defines the CPU core the OS-Application is mapped to.

Task specification:
  <task-spec>: -task <Taskname> [-size <value>] [<object.o> ...]

  Defines the tasks belonging to the enclosing application specification. The list of object files
  are those objects that are mapped to the private sections of the task.

ISR specification:
  <isr-spec>: -isr <ISR name> [-size <value>] [<object.o> ...]

  Defines the ISRs belonging to the enclosing application specification. The list of object files
  are those objects that are mapped to the private sections of the ISR.

Specification of memory region with mapped objects:
  <memregsym>: -memregsym <Region Name> [-size <value>] [<object.o> ...]

Stack specification:
  <stack-spec>: -stack <Stack name> [-size <value>]

  Defines a stack region and optionally the size of the stack region.

Specification of memory region with fixed addresses:
  <region-spec>:
  -mr <StartSymbol>=<Address> <EndSymbol>=<Address> [<BssStartSymbol>=<Address> [<InitDataSymbol>=<Address>]]

Specification of system objects (tasks/ISRs not assigned to a regular OS-Application):
  <system-object>: -sysobj <name>

EOUSAGE

	foreach my $fn (@usage_fns)
	{
		$fn->();
	}

	exit 1;
}

sub setArg
{
	my ($container, $arg, $argidx) = @_;
	my $value = 1; # init with 1 for simple flag options ($argidx undefined)

	if ( (defined $argidx) && ($argidx >= @ARGV) )
	{
		usage("$arg requires a value!");
	}
	else
	{
		$value = $ARGV[$argidx];
	}

	if(ref $container eq 'HASH')
	{
		if(exists $container->{$arg} && $container->{$arg} ne $value) {
			usage("Conflicting definitions for $arg:\n",
				"Value 1: $container->{$arg}\n",
				"Value 2: $value");
		}

		$container->{$arg} = $value;

	}
	elsif(ref $container eq 'ARRAY')
	{
		push @$container, $value;
	}
	else
	{
		die "Unexpected value for \$container parameter";
	}
}

sub setHashEntry
{
	my ($hash, $type, $argidx) = @_;
	usage("$type requires a name!") unless $argidx<@ARGV;

	my $value = $ARGV[$argidx];
	if(exists $hash->{$value}) {
		usage("Multiple definitions of $type $value\n");
	}

	$hash->{$value} = {
		'name'  => $value,
		'type'  => $type,
		'files' => [],
	};
	return $hash->{$value};
}

sub addUniqueArrayEntry
{
	my ($rsysdesc, $array, $type, $argidx) = @_;
	# This script must run on a Perl v5.8 without any packages, so better don't use signatures.
	die "[FATAL] Internal signature error: addUniqueArrayEntry expects a reference to a hashmap as first argument"
		unless ref $rsysdesc eq ref {};
	usage("$type requires a name!") unless $argidx<@ARGV;

	my $value = $ARGV[$argidx];

	# Check whether this type-value-pair is unique in the given system description.
	$rsysdesc->{'_unique_ids_'} ||= {};
	my $rUniqueIds = $rsysdesc->{'_unique_ids_'};
	$rUniqueIds->{$type} ||= {};
	my $rSetOfType = $rUniqueIds->{$type};
	if (defined $rSetOfType->{$value}) {
		usage("Multiple definitions of ${type} '${value}'\n");
	}
	$rSetOfType->{$value} = 1;

	push @$array, {
		'name'  => $value,
		'type'  => $type,
		'files' => [],
	};
	return $array->[-1];
}

sub parseArgs
{
	my ($arch_parser_fn) = @_;

	%sysdesc = ( );
	my $curapp; # Hash ref to the active application specification, undefined before the first app spec
	my $curcontainer; # Hash ref to active container of private object files

	# Args from command line
	for(my $i=0; $i < @ARGV; $i++)
	{
		my $a = $ARGV[$i];
		if($a eq '-o')
		{
			setArg(\%sysdesc, $a, ++$i);
		}
		elsif($a eq '-numcores')
		{
			setArg(\%sysdesc, $a, ++$i);
			usage("Value for $a must be a positive integer") if $sysdesc{$a} !~ /^\d+$/ || $sysdesc{$a}==0;
		}
		elsif($a eq '-app')
		{
			$sysdesc{$a} ||= [];
			$curapp = addUniqueArrayEntry(\%sysdesc, $sysdesc{$a}, 'OS-Application', ++$i);

			$curcontainer = $curapp;

			$curapp->{'contains'} = ['-task', '-isr', '-memregsym'];
			$curapp->{'-task'} = [];
			$curapp->{'-isr'} = [];
			$curapp->{'-memregsym'} = [];
		}
		elsif($a eq '-core')
		{
			usage("-core must be part of an app-spec") unless ref $curapp;
			setArg($curapp, $a, ++$i);
			usage("Value for $a must be numeric") if $curapp->{$a} !~ /^\d+$/;
		}

		elsif($a eq '-size')
		{
			usage("must be part of an app, task, isr or stack") unless ref $curcontainer;
			setArg($curcontainer, $a, ++$i);
		}

		elsif($a eq '-task' || $a eq '-isr' || $a eq '-memregsym')
		{
			usage("$a must be part of an app-spec") unless ref $curapp;
			$curcontainer = addUniqueArrayEntry(\%sysdesc, $curapp->{$a}, ucfirst(substr($a, 1)), ++$i);
		}
		elsif($a eq '-num-thread-slots')
		{
			setArg(\%sysdesc, $a, ++$i);
			$sysdesc{$a} = [ split(',', $sysdesc{$a}) ];
			if(grep { !/(^[1-9]\d*$)|(^0$)/ } @{$sysdesc{$a}})
			{
				usage("$a expects a comma-separated list of non-negative decimal integers");
			}
		}

		elsif($a eq '-thread-stack-all')
		{
			setArg(\%sysdesc, $a, ++$i);
			$sysdesc{$a} = [ split(',', $sysdesc{$a}) ];
			if(grep { !/(^[1-9]\d*$)|(^0$)/ } @{$sysdesc{$a}})
			{
				usage("$a expects a comma-separated list of non-negative decimal integers");
			}
		}

		elsif($a eq '-internal-stack-all')
		{
			setArg(\%sysdesc, $a, ++$i);
			$sysdesc{$a} = [ split(',', $sysdesc{$a}) ];
			if(grep { !/(^[1-9]\d*$)|(^0$)/ } @{$sysdesc{$a}})
			{
				usage("$a expects a comma-separated list of non-negative decimal integers");
			}
		}

		elsif($a eq '-prepend')
		{
			$sysdesc{$a} = [ ] unless defined $sysdesc{$a};
			setArg($sysdesc{$a}, $a, ++$i);
		}

		elsif($a eq '-text-region-size')
		{
			setArg(\%sysdesc, $a, ++$i);
		}

		elsif($a eq '-const-region-size')
		{
			setArg(\%sysdesc, $a, ++$i);
		}

		elsif($a eq '-data-region-size')
		{
			setArg(\%sysdesc, $a, ++$i);
		}

		elsif($a eq '-qmos-data-size')
		{
			setArg(\%sysdesc, $a, ++$i);
		}

		elsif($a eq '-kern-data-size')
		{
			setArg(\%sysdesc, $a, ++$i);
		}

		elsif($a eq '-mpu-cache-size')
		{
			setArg(\%sysdesc, $a, ++$i);
		}

		elsif($a eq '-stack')
		{
			$sysdesc{$a} = [ ] unless defined $sysdesc{$a};
			$curstack = addUniqueArrayEntry(\%sysdesc, $sysdesc{$a}, 'OS-Stack', ++$i);

			$curcontainer = $curstack;
		}

		elsif($a eq '-include')
		{
			$sysdesc{$a} = [ ] unless defined $sysdesc{$a};
			setArg($sysdesc{$a}, $a, ++$i);
		}
		elsif($a eq '-backend')
		{
			setArg(\%sysdesc, $a, ++$i);
			$sysdesc{'backend'} = GenldBackend->createBackend($sysdesc{'-backend'}, $sysdesc);
			usage($sysdesc{'backend'}) unless ref $sysdesc{'backend'};
		}
		elsif($a eq '-mr')
		{
			$sysdesc{$a} ||= {};
			my $mrcontainer = setHashEntry($sysdesc{$a},'Memory Region', ++$i);

			setArg($mrcontainer, 'RSA', ++$i);
			setArg($mrcontainer, 'RLA', ++$i);
			setArg($mrcontainer, 'BSA', ++$i) if ($i<$#ARGV && $ARGV[$i+1] !~ /^-/);
			setArg($mrcontainer, 'IDAT', ++$i) if ($i<$#ARGV && $ARGV[$i+1] !~ /^-/);
		}
		elsif($a eq '-sysobj')
		{
			$sysdesc{'-sysobj'} ||= [];
			setArg($sysdesc{'-sysobj'}, 'Object-Name', ++$i);
		}
		elsif(defined $sysdesc{'backend'} && $sysdesc{'backend'}->parseArgs($a, \%sysdesc, \$i) != 0)
		{
			# handled by GenldBackend
		}
		elsif((ref $arch_parser_fn eq 'CODE') && ($arch_parser_fn->($a, \%sysdesc, \$i) != 0))
		{
			# handled by main script
		}
		elsif(ref $curcontainer)
		{
			push @{$curcontainer->{files}}, $a;
		}
		# optional special bootloader object file containing a .bootloader section
		elsif($a eq '-bootloader')
		{
			setArg(\%sysdesc, $a, ++$i);
		}
		# add start symbol for bootloader
		elsif($a eq '-bootloaderEntry')
		{
			setArg(\%sysdesc, $a, ++$i);
		}
		# add glue code from additional .bootloader sections to the end of bootloader
		elsif($a eq '-addBootloaderGlue')
		{
			setArg(\%sysdesc, $a, $i);
		}
		# disable generation of data sections in ROM for initialization
		elsif($a eq '-disableRomInit')
		{
			setArg(\%sysdesc, $a, $i);
		}
		#Adds the pad byte to make the empty sections non-empty for QEMU 
		elsif($a eq '-padBytes')
		{
			setArg(\%sysdesc, $a, $i);
		}
		else
		{
			usage("Don't know what to do with option $a");
		}
	}

	# Check that we got everything needed
	usage("-backend not specified") unless $sysdesc{'-backend'};
	usage("-numcores not specified") unless $sysdesc{'-numcores'};
	usage("No output file specified (-o)") unless $sysdesc{'-o'} ne '';

	foreach my $app (@{$sysdesc{'-app'}})
	{
		if($app->{'-core'} >= $sysdesc{'-numcores'})
		{
			usage("Invalid core Id $app->{'-core'} specified for OS-Application $app->{name}");
		}
	}

	open($outh, ">$sysdesc{'-o'}") or die "Unable to open $sysdesc{'-o'} for writing: $!\n";
	prependHeader();
	printHeader();

	return \%sysdesc;
}

# Output routines
sub now
{
	my ($mm, $hh, $DD, $MM, $YY) = (localtime(time))[1,2,3,4,5];
	my $genyear = sprintf("%04d", ($YY+1900));
	my $now = sprintf("%04d-%02d-%02d %02d:%02d", ($YY+1900), ($MM+1), $DD, $hh, $mm);
	return $now;
}

sub prependHeader
{
	if($sysdesc{'-prepend'})
	{
		foreach my $prepend (@{$sysdesc{'-prepend'}})
		{
			print $outh $prepend,"\n";
		}
	}
}

sub printHeader
{
	my $now = now();
	print $outh <<EOHEADER;
/*
 * Linker script
 *
 * GENERATED FILE: DO NOT EDIT
 * Generated by $0 on $now
 */

EOHEADER
}

sub processIncludes
{
	my ($filterfn) = @_;

	if($sysdesc{'-include'})
	{
		foreach my $include (@{$sysdesc{'-include'}})
		{
			open(my $INCLUDEH, $include) or die "Cannot open $include: $!";
			foreach my $line (<$INCLUDEH>) {
				$line = $filterfn->($line) if $filterfn;
				print $outh $line;
			}
			close($INCLUDEH);
		}
	}
}

sub printOut
{
	die unless defined $outh;

	foreach my $text (@_)
	{
		my $newlines_after = ($text =~ /\n+$/) ? $& : '';

		my @lines = split("\n", $text);

		foreach my $line (@lines)
		{
			my $openingBrackets = ($line =~ tr/\{\(\[//);
			my $closingBrackets = ($line =~ tr/\}\)\]//);
			my $indentChange = ($openingBrackets - $closingBrackets);

			my $indentation;
			if($indentChange >= 0) {
				$indentation = "\t" x $indentLevel;
			} else {
				# When indentation reduces, we must indent the line with reduced indentation
				$indentation = "\t" x ($indentLevel+$indentChange);
			}

			$indentLevel += $indentChange;
			$line =~ s/^\t+//; # remove manual indentation
			print $outh $indentation,$line,"\n";
		}

		print $outh $newlines_after;
	}
}

sub printOutNoIndent
{
	die unless defined $outh;

	foreach my $text (@_)
	{
		# use LIMIT -1 to prevent removing empty trailing lines
		my @lines = split("\n", $text, -1);

		foreach my $line (@lines)
		{
			print $outh $line."\n";
		}
	}
}

sub finalize
{
	close $outh;
}

1;

# Editor settings: DO NOT DELETE
# vi:set ts=4 sw=4 noet tw=100:
