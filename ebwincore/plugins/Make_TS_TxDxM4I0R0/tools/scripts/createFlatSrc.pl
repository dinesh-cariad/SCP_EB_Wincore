#
# \file
#
# \brief AUTOSAR Make
#
# This file contains the implementation of the AUTOSAR
# module Make.
#
# \version 4.0.27
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.

#================================================================
# copies all sources to a flattened directory
# copies only header files which are detected in the depend files
#================================================================

use strict;
use File::Basename;
use File::Copy;
use File::Next;
use File::Path qw(make_path);

my ($src_file_list, $hdr_file_list, $depend_dir, $dest_dir) = parse_arguments(\@ARGV);

die "ERROR: Could not find \"$src_file_list\"\n" if (!(-e $src_file_list));
die "ERROR: Could not find \"$hdr_file_list\"\n" if (!(-e $hdr_file_list));
die "ERROR: Could not find \"$depend_dir\"\n" if (!(-e $depend_dir));
die "ERROR: Could not find \"$dest_dir\"\n" if (!(-e $dest_dir));

my $src_dir = $dest_dir . "/src";
my $depend_file = $dest_dir . "/depend.inc";

die "ERROR: Could not find \"$src_dir\"\n" if (!(-e $src_dir));

main();

#------------------------------------------------------------------------------
# MAIN Function
#------------------------------------------------------------------------------
sub main
{
  open(INFILE, '<', $src_file_list ) or die "ERROR: cannot open $src_file_list\n";
  my @file = <INFILE>;
  close(INFILE);

  my $depend_string = "";
  # iterate over each line of SRC_DIRS.txt
  foreach my $line (@file)
  {
    # change backward slashes into forward slashes
    $line =~ s/\\/\//g;
    # remove line feed
    chomp $line;
    $line = trim($line);
    # check if file exists
    if (-e $line)
    {
      # strip filename from full path name
      my ($name, $path, $suffix) = fileparse($line);
      # generate destination full path name from delivery dir variable
      my $dest = $src_dir . "/" . $name;

      # now copy the file
      # all dependencies to header files are assembled and returned as a
      # makefile formatted string
      $depend_string .= copy_c_func ($line, $dest);
      $depend_string .= "\n";
    } #end if (-e $line)
  } # end foreach
  # write the dependency makefile
  open(OUTFILE, '>', $depend_file ) or die "ERROR: cannot open $depend_file\n";
  print OUTFILE $depend_string;
  close(OUTFILE);
}

#===  copy_c_func  ===============================================================
#       METHOD:  copy_c_func
#   PARAMETERS:  source file full path, destination file full path
#      RETURNS:  A string with the makefile dependencies
#  DESCRIPTION:  copies a source file if it is not present or older than the
#                original. invokes parsing of the corresponding dependency
#                file for copying the header files.
#===============================================================================
sub copy_c_func
{
  my $src = $_[0];
  my $dst = $_[1];

  # copy the file
  copy_file($src, $dst);

  # strip path from file name
  my ($name, $path, $suffix) = fileparse($src);
  # strip file extension from file name
  my @base = $name =~ /(.*!?)\..*/;
  # turn the hyphon
  my $new_base = $base[0];
  my $new_name = $name;
  $new_base =~ s/-/_/g;
  $new_name =~ s/-/_/g;
  # construct depend file name
  my $depend_string = "";
  my $depend_file = $depend_dir . "/" . $base[0] . ".d";
  if (-e $depend_file)
  {
    $depend_string = "./obj/$new_base.o : ./src/$new_name \\\n";
    # copy all headers assosiated with this file and
    # additionally assemble a make dependency into a
    # makefile formatted string
    $depend_string .= depend_func($depend_file);
  }
  else
  {
    # the depend file has to exist at this moment!
    print "WARNING: \"$depend_file\" does not exist!\n"
  }
  return $depend_string;
}

#===  depend_func===============================================================
#       METHOD:  depend_func
#   PARAMETERS:  ARG0 - the full path to the dependency .d file
#      RETURNS:  a string of all header files dependencies in the .d file
#  DESCRIPTION:  copies all header files defined in the .d file to the delivery
#                directory
#===============================================================================
sub depend_func
{
  my $dep = $_[0];
  # open *.d file
  open(INFILE, '<', $dep ) or die "ERROR: cannot open $dep\n";
  my @file = <INFILE>;
  close(INFILE);
  # iterate over each line of *.d file
  my $depend_string = "";
  foreach my $line (@file)
  {
      # check if line contains an included header file otherwise ignore it
      if ($line =~ /\s*(.*\.[hc])\s.*/)
      {
        my $h_file = $1;
        $h_file =~ s/\\/\//g;

        # call create dest to determine wether this file is an OS file which
        # needs to be stored in a subfolder and creates subfolders if needed
        my $dest = create_h_dest($h_file);
        die "ERROR INVALID RETURN VALUE\n" if (!defined $dest);
        $depend_string .= "  ./src/" . $dest . "\\\n";
        my $full_dest_path = "";

        $full_dest_path = $src_dir . "/". $dest;

        # copy the header file
        copy_file($h_file, $full_dest_path);
      }
  }
  return $depend_string;
}

#===  create_h_dest  ==========================================================
#       METHOD:  create_h_dest
#   PARAMETERS:  ARG0 : full path to header file
#      RETURNS:  path to the destination for the header file
#  DESCRIPTION:  call create dest to determine wether this file is an OS file
#                which needs to be stored in a subfolder and creates subfolders
#                if needed. Returns the corresponding path.
#===============================================================================
sub create_h_dest
{
  my $h_file = $_[0];
  # extract all OS related sub paths
  # for example Os_TS_T2D21M4I2R0/include/PA/XPC56XXK/Os_XPC56XXK.h
  # will extract in $1: /PA/XPC56XXK
  my $dest_path;
  my ($basename, $path, $suffix) = fileparse($h_file);
  if ($h_file =~ /_TS_.*include\/(.*)\/.*/)
  {
    $dest_path = $1;
    my $dest_full_path = $src_dir . "/" . $dest_path;
    if (!-e $dest_full_path)
    {
      print "creating $dest_full_path\n";
      make_path($dest_full_path);
    }
    $dest_path = $dest_path . "/" . $basename;
  }
  else
  {
    $dest_path = $basename;
  }
  # turn the slash
  $dest_path =~ s/\\/\//g;
  return $dest_path;
}

#===  copy_file  ===============================================================
#       METHOD:  copy_file
#   PARAMETERS:  source file full path, destination file full path
#      RETURNS:  --
#  DESCRIPTION:  copies a source file if it is not present or older than the
#                original.
#===============================================================================
sub copy_file
{
  my $src = $_[0];
  my $dst = $_[1];

  if (!(defined $dst))
  {
    die "ERROR: destination not defined!";
  }
  else
  {
    my ($dest_name, $dest_path, $dest_suffix) = fileparse($dst);
    if (!-e $dest_path)
    {
      die "ERROR: destination path $dst does not exist!\n";
    }
    my $tmp_name = $dest_name;
    $tmp_name =~ s/-/_/g;
    if ($tmp_name ne $dest_name)
    {
      my $newdir = $dest_path . $tmp_name;
      $dst = $dest_path . $tmp_name;
    }
  }

  my ($name, $path, $suffix) = fileparse($src);
  # check if source file exists
  if (-e $src)
  {
    if (!-e $dst)
    {
      # copy file if it does not exist
      copy($src, $dst);
    }
    else
    {
      my @dest_stats = stat($dst);
      my @src_stats = stat($src);
      if ($dest_stats[9] < $src_stats[9])
      {
        print "overwriting newer file $name\n";
        # copy file if it is older than the source
        copy($src, $dst);
      }
    }
  }
  else
  {
    print "WARNING: $src does not exist!\n";
  }
}

#------------------------------------------------------------------------------
# parse_arguments
#------------------------------------------------------------------------------
sub parse_arguments
{
  my $args = $_[0];
  my $last_index = $#{$args};
  my $src = "";
  my $hdr = "";
  my $depDir = "";
  my $destDir= "";

  foreach my $elem (@{$args})
  {
    if ($elem =~ /--src=([!-~]+)/)
    {
      $src = $1;
    }
    elsif ($elem =~ /--hdr=([!-~]+)/)
    {
      $hdr = $1;
    }
    elsif ($elem =~ /--depDir=([!-~]+)/)
    {
      $depDir = $1;
    }
    elsif ($elem =~ /--destDir=([!-~]+)/)
    {
      $destDir = $1;
    }
  }
  return ($src, $hdr,$depDir,$destDir);
}

#------------------------------------------------------------------------------
# get_cur_dir_base
#------------------------------------------------------------------------------
sub get_cur_dir_base
{
  my $normalized_file = $_[0];
  $normalized_file =~ s/\\\\/\\/go;
  $normalized_file =~ s/\\/\//go;
  die "ERROR: file does not exist $normalized_file\n" if (!(-e $normalized_file));
  my $finding;
  my $root;
  if ($normalized_file =~ /(.*)\/(.*)$/)
  {
    $finding = $2;
    $root = $1;
  }
  return $root, $finding;
}

#===  trim  ====================================================================
#       METHOD:  trim
#   PARAMETERS:  ARG0 : string to trim whitespace from
#      RETURNS:  string without leading and trailing whitespaces
#  DESCRIPTION:  strips leading and trailing whitespaces from string
#===============================================================================
sub trim
{
  my $string = shift;
  $string =~ s/^\s+//;
  $string =~ s/\s+$//;
  return $string;
}
