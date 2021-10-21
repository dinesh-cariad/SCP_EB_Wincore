#
# \file
#
# \brief AUTOSAR Compiler
#
# This file contains the implementation of the AUTOSAR
# module Compiler.
#
# \version 1.0.8
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2017 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#---------[ inclusions ]--------------------------------------------------------

use strict;
use File::Find;
use File::Basename;

#---------[ globals ]-----------------------------------------------------------
my @src_file_list;
#---------[ command line parameters ]-------------------------------------------
my $depend_dir = $ARGV[0];
my $obj_dir = $ARGV[1];

if (!-e $obj_dir)
{
    print "ERROR: parameter 2 obj_dir \"$obj_dir\" not found!\n";
    exit 1;
}

#---------[ start interpreter mainloop ]----------------------------------------
main();


#------------------------------------------------------------------------------
# main
#------------------------------------------------------------------------------
sub main
{
  if (-e $depend_dir)
  {
    find(\&find_callout, $depend_dir);
    foreach my $elem (@src_file_list)
    {
      
      tasking_to_make($elem);
    }
  }
  else
  {
    print "ERROR: parameter 1 depend_dir \"$depend_dir\" not found!\n";
    exit 1;
  }
}

#------------------------------------------------------------------------------
# find_callout
#------------------------------------------------------------------------------
sub find_callout
{
  my $file = $_;
  if (defined $_)
  {
    my @properties = stat($_);
    my $name = $File::Find::name;
    if ($name =~ /\.d$/)
    {
      $name=~s/\//\\/go;
      #$name=~s/\\/\\\\/go;
      push @src_file_list, $name;
    }
  }
}

#------------------------------------------------------------------------------
# tasking_to_make
#------------------------------------------------------------------------------
sub tasking_to_make
{
  my $source = $_[0];
  my $destination= $source;
  if (-e  $source)
  {
    open (INFILE, '<', $source ) or die "can not open $source\n";
    my $base = base_name($source);
    
    my @file = <INFILE>;
    my $line = @file[0];
    my $new_line="";
    # extract file stem from fist line in the file 
    my $file_stem="";
    
    if ($line=~/CONVERT_MARKER=0/)
    {
      # file already converted. nothing to do! 
      #print "scipping $base ";
      return;
    }
    else
    {
      print "converting $base\n";
    }
    
    if ($line=~/(^[a-zA-Z0-9_-]+)\.o\s+:/)
    {
        $file_stem=$1;
        $new_line="$obj_dir/$1.o : \\\n";
    }
    
    close(INFILE);
  
    # assemble dependency header files from the file 
    my $depend_string ="";
    foreach $line (@file)
    {
      chomp $line;
      if ($line=~/^$file_stem\.o\s+:\s+\"(.*)\"/)
      {
        $depend_string.=" $1 \\\n";
      }
      
      #print OUTFILE $line." \\\n";
    }
    $depend_string.="\n";
    open (OUTFILE, '>', $destination ) or die "can not open $destination\n";
    print OUTFILE "CONVERT_MARKER=0\n";
    print OUTFILE $new_line;
    print OUTFILE $depend_string;
    close (OUTFILE);
  }
}


#------------------------------------------------------------------------------
# base_name
#
# Description :
#------------------------------------------------------------------------------
sub base_name
{
    my $file=$_[0];
    my($name, $dir, $suffix) = fileparse($file);
    return $name;
}
# open (LOG, ">log_convert_dep.log" ) or die "can not open log.txt \n";
# print LOG "script was run\n";
# close(LOG);
