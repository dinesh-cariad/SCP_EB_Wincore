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
# detects header files of the same name in different directories
#================================================================

use strict;
use Env qw(PROJECT_ROOT);

my $file_hash;
my ($src, $hdr, $out_file) = parse_arguments(\@ARGV);
my $global_hash;
my $dir_hash;

my $out_FH;

if (-e $hdr)
{
  parse_hdr_list($hdr);
}

if (-e $src)
{
  parse_src_list($src);
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
  my $out = "";

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
    elsif ($elem =~ /--out=([!-~]+)/)
    {
      $out = $1;
    }
  }
  return ($src, $hdr, $out);
}

#------------------------------------------------------------------------------
# parse_hdr_list
#------------------------------------------------------------------------------
sub parse_hdr_list
{
  my $hdr_file = $_[0];
  open(INFILE, '<', $hdr_file) or die "ERROR: cannot open $hdr_file for reading!\n";
  my @hdr_list = <INFILE>;
  close(INFILE);

  foreach my $dir (@hdr_list)
  {
    if ($dir =~ /\s*([!-~]+)\s*/)
    {
      my $stripped_dir = $1;
      my $pattern = '\.[h]$';
      list_dir($stripped_dir, $pattern);
    }
  }
}

#------------------------------------------------------------------------------
# parse_src_list
#------------------------------------------------------------------------------
sub parse_src_list
{
  my $src_file = $_[0];
  open(INFILE, '<', $src_file) or die "ERROR: cannot open $src_file for reading!\n";
  my @src_list = <INFILE>;
  close(INFILE);
  foreach my $file (@src_list)
  {
    if ($file =~ /\s*([!-~]+)\s*/)
    {
      my $stripped_dir =$1;
      if (-e $stripped_dir)
      {
        my ($rootdir, $filename) = get_cur_dir_base($stripped_dir);
        if (defined $rootdir)
        {
          if (!(defined $dir_hash->{$rootdir}))
          {
            my $pattern = '\.[c]$';
            $dir_hash->{$rootdir} = 1;
            list_dir($rootdir, $pattern);
          }
        }
      }
    }
  }
}

#===  list_dir   ==========================================================
# description: list directory content
#===============================================================================
sub list_dir
{
  my $dir_path = $_[0];
  my $pattern = $_[1];
  if (-e $dir_path)
  {
    opendir(IMD, $dir_path) || die("ERROR: cannot open directory $dir_path");
    my @thefiles= readdir(IMD);
    closedir(IMD);
    foreach my $file (@thefiles)
    {
      if ($file =~ /^[^.].*/)
      {
        if ($file =~ /$pattern/)
        {
          my $fullpath = $dir_path . "/" .  $file;
          if (defined $global_hash->{$file})
          {
           print STDERR "WARNING: duplicate $file: $fullpath duplicates $global_hash->{$file}\n";
          }
          else
          {
            $global_hash->{$file}= $dir_path . "/" .  $file;
          }
        }
      }
    }
  }
  return undef;
}

#------------------------------------------------------------------------------
# get_cur_dir_base
#------------------------------------------------------------------------------
sub get_cur_dir_base
{
  my $normalized_file = $_[0];
  $normalized_file =~ s/\\\\/\\/go;
  $normalized_file =~ s/\\/\//go;
  my $finding;
  my $root;
  if ($normalized_file =~ /(.*)\/(.*)$/)
  {
     $finding = $2;
     $root = $1;
  }

  return $root, $finding;
}
