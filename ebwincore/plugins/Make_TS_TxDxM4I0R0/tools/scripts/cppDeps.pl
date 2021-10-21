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
# emulates the header dependencies mechanism of the gcc compiler
# this is because of a known bug in gcc which crashes the build
# in case that the amount of date passed via option file is 
# larger than 32k
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=71850
#================================================================

use strict;
use File::Basename;
use File::Find;
use Cwd 'abs_path';
use Time::HiRes qw ( time );
my $index=0;


################################################################################
#  Constants
################################################################################
use constant{
  IDLE_STATE     => 0,
  PARSE_MF       => 1,
  PARSE_MT       => 2,
  MM_NOT_FOUND   => 3,
  MM_FOUND       => 4,
  TIME_NOT_FOUND => 5,
  TIME_FOUND     => 6,
};

my $start_time = time;

################################################################################
#  Globals
################################################################################
my $UNKNOWN = "UNKNOWN";
my $MF = $UNKNOWN; # file to write dependencies
my $OUTPUT_FILE = $UNKNOWN;
my @MT = (); # targets in the make rule
my $MM = MM_NOT_FOUND;
my $TIME = TIME_NOT_FOUND;
my $INPUT_FILE = $UNKNOWN;
my $OPT_FILE = $UNKNOWN;
my @INC_DIRS = ();
my $DEPENDENCIES = undef;
$DEPENDENCIES->{file_list} = [];

my $headersHash = {};

################################################################################
#  Input Parameter Checks
################################################################################
parse_arguments(\@ARGV);

if ($MM == MM_NOT_FOUND)
{
  die "ERROR: -MM Parameter not found\n";
}

if (!(-e $INPUT_FILE))
{
  die "ERROR: input file \"$INPUT_FILE\" does not exist\n";
}

if (!(-e $OPT_FILE))
{
  die "ERROR: options file \"$OPT_FILE\" does not exist\n";
}

if ($#MT == -1)
{
  my $target = not_dir(base_name($INPUT_FILE)) . ".o";
  push @MT, $target;
}

parse_options($OPT_FILE);

if ($#INC_DIRS == -1)
{
  my $outstr = sprintf( "WARNING: no include dirs found in \"%s\"\n", not_dir($OPT_FILE));
  print STDERR $outstr;
}

################################################################################
#  Main
################################################################################
parse_input_file($INPUT_FILE);

my $output = generate_output();

if ($MF eq $UNKNOWN)
{
  print $output;
}
else
{
  open(OUTFILE, '>', $OUTPUT_FILE ) or die "ERROR: cannot open $OUTPUT_FILE\n";
  print OUTFILE $output;
  close(OUTFILE);
}

my $end_time = time;

if ($TIME == TIME_FOUND)
{
  print STDERR sprintf ("time: %2.3f\n", $end_time - $start_time);
}

################################################################################
#  Subroutines
################################################################################

#------------------------------------------------------------------------------
# generate_output
#------------------------------------------------------------------------------
sub generate_output
{
  my $out_string = "";
  my $index = 0;
  my $last_index = $#MT;
  foreach my $elem (@MT)
  {
    $out_string .= " " if ($index != 0);
    $out_string .= "$elem";
    $out_string .= ":" if ($index == $last_index);
    $out_string .= " \\\n";
    $index++;
  }
  $out_string .= " " . $INPUT_FILE . "\\\n";
  $last_index = $#{$DEPENDENCIES->{file_list}};
  $index = 0;
  foreach my $elem (@{$DEPENDENCIES->{file_list}})
  {
    $out_string .= " $elem";
    $out_string .= " \\" if ($index != $last_index);
    $out_string .= "\n";
    $index++;
  }
  return $out_string;
}

#------------------------------------------------------------------------------
# parse_input_file
#------------------------------------------------------------------------------
sub parse_input_file
{
  my $input_file = $_[0];

  if (defined $DEPENDENCIES->{$input_file})
  {
    return;
  }
  $DEPENDENCIES->{$input_file} = 1;

  open(INFILE, '<', $input_file) or die "ERROR: cannot open $input_file!\n";
  my @lines = <INFILE>;
  close(INFILE);
  my $line_number = 1;
  foreach my $line (@lines)
  {
    if ($line =~ /^\s*#include\s+[<"](.*)[">]/)
    {
      my $file = $1;
      my $header_full_path;
      $file =~ s/[<">]//go;

      $header_full_path = find_header_dir($file);
      if (defined $header_full_path)
      {
        if (!(defined $DEPENDENCIES->{$header_full_path}))
        {
          push @{$DEPENDENCIES->{file_list}}, $header_full_path;
          parse_input_file($header_full_path);
          $DEPENDENCIES->{$header_full_path}=1;
        }
      }
    }
    $line_number++;
  }
}

#------------------------------------------------------------------------------
# find_header_dir
#------------------------------------------------------------------------------
sub find_header_dir
{
  my $input_file = $_[0];
  if (defined $headersHash->{$input_file})
  {
    return $headersHash->{$input_file};
  }
  return undef;
}

#------------------------------------------------------------------------------
# parse_options
#------------------------------------------------------------------------------
sub parse_options
{
  my $input_file = $_[0];
  open(INFILE, '<', $input_file) or die "ERROR: cannot open $input_file!\n";
  my @lines = <INFILE>;
  close(INFILE);
  foreach my $line (@lines)
  {
    if ($line =~ /^-I(.*)$/)
    {
      if (-e $1)
      {
        my $dir = trim($1);
        $dir = abs_path($dir);
        push @INC_DIRS, normalized_path($dir);
        getHeadersFromDir(normalized_path($dir));
      }
    }
  }
}

#------------------------------------------------------------------------------
# getHeadersFromDir
#------------------------------------------------------------------------------
sub getHeadersFromDir
{
  my $dir = $_[0];
  if (opendir(IMD, $dir))
  {
    my @thefiles = readdir(IMD);
    closedir(IMD);
    foreach my $file (@thefiles)
    {
      if ($file =~ /^.*[h]$/)
      {
        my $full_path = $dir . "/" . $file;
        $headersHash->{$file} = $full_path;
      }
    }
  }
}

#------------------------------------------------------------------------------
# parse_arguments
#------------------------------------------------------------------------------
sub parse_arguments
{
  my $args = $_[0];
  my $parser_state = IDLE_STATE;
  my $last_index = $#{$args};

  foreach my $elem (@{$args})
  {
    $index++;
    if ($elem eq "-MF")
    {
      $parser_state = PARSE_MF;
      next;
    }
    if ($parser_state == PARSE_MF)
    {
      $MF = $elem;
      $OUTPUT_FILE = normalized_path($elem);
      $parser_state = IDLE_STATE;
      next;
    }
    if ($elem eq "-MT")
    {
      $parser_state = PARSE_MT;
      next;
    }
    if ($parser_state == PARSE_MT)
    {
      push @MT, $elem;
      $parser_state = IDLE_STATE;
      next;
    }
    if ($elem eq "-MM")
    {
      $MM = MM_FOUND;
      next;
    }
    if ($elem eq "-time")
    {
      $TIME = TIME_FOUND;
      next;
    }
    if ($elem =~ /^@(.+)/)
    {
      $OPT_FILE = normalized_path($1);
      next
    }
    if ($parser_state == IDLE_STATE)
    {
      $INPUT_FILE = normalized_path($elem);
      next;
    }
  }
}

#------------------------------------------------------------------------------
# normalized_path
#------------------------------------------------------------------------------
sub normalized_path
{
  my $input_file = $_[0];
  $input_file =~ s/\\\\/\\/go;
  $input_file =~ s/\\/\//go;
  return $input_file;
}

#------------------------------------------------------------------------------
# not_dir
#------------------------------------------------------------------------------
sub not_dir
{
  my $file = $_[0];
  my($name, $dir, $suffix) = fileparse($file);
  return $name;
}

#------------------------------------------------------------------------------
# base_name
#------------------------------------------------------------------------------
sub base_name
{
  my $file = $_[0];
  my $name = $UNKNOWN;
  if ($file =~ /^(.*)\.(.*)$/)
  {
    $name = $1;
  }
  return $name;
}

#------------------------------------------------------------------------------
# trim
#------------------------------------------------------------------------------
sub trim
{
  my $string = shift;
  $string =~ s/^\s+//;
  $string =~ s/\s+$//;
  return $string;
}
