package GenldArch;

=head1 The C<GenldArch> package

This package serves documentary purposes only. It describes the common interface implemented by all
C<< GenldArch<TARGET> >> packages so we have the common documentation in one place.

=head2 Objects provided by this package

This package most prominently provides two global objects, although these are only intended to be
directly used by the linker script generator script. When an instance of the back end is created,
references to these two objects are typically passed along to the back end. This allows the
architecture-specific generator script to do modifications to these objects, or provide different
versions altogether, for example if the are different architecture variants with differing
characteristics.

The main reason for the existence of the C<< GenldArch<TARGET> >> packages (as opposed to defining
these properties in the also architecture-specific generator script directly) is that these
properties can be shared between the generator scripts of the AutoCore OS and the microkernel.

=over

=item C<%osecTypes>

This object defines the characteristics of the different types of output sections on the respective
architecture. Its keys are the section types as given to C<printOutputSection()>. The value
references a hash that contains the characteristics of the corresponding section type.

The following standard attributes should be supported by all back ends:

=over

=item C<align>: Defines the minimum alignment of the output section in bytes.

=item C<endalign>: Defines the minimum alignment of end symbol in bytes.

The output section will be enlarged accordingly. The default end alignment is 1.

=item C<isectDefault>: Default set of input sections for this output-section type

This list is used by the back end in C<printOutputSection()> in case the parameter C<$inSections> is
not defined.

=back

In addition to these standard attributes, toolchain-specific attributes may be defined. Those should
be documented with the backend of the corresponding toolchain.

=item C<%memoryBlocks>

Contains definition of the memory blocks defined in the linker scripts for specific purposes, which
are directly needed in the backend. Currently, the only such memory is the memory used to store
initialization images for initialized data sections.

=over

=item C<initdata_memory>

Maps to a string giving the name of the memory block that images for initialized data
sections shall be mapped to.

=back

=back

=cut

1;

# Editor settings: DO NOT DELETE
# vi:set ts=4 sw=4 noet tw=100:
