#!/usr/bin/perl
#
# Copyright (C) Elektrobit Automotive GmbH
# All rights reserved
#
# NAME: create_epds.pl
# CREATED BY: Bernd Haberstumpf
# DATE: $Id: create_epds_20.pl,v 1.2 2010/01/08 16:44:35 mawe2550 Exp $
#
# create_epds.pl < <EcucParamDef.xml>
#
# Extracts epd-files for all modules defined  in a Parameter Definition file 
# provided by Autosar
#
# This version of create_epds is only suiteable for AUTOSAR 2.0

use File::Copy;
use IO::Handle;

local(*TEMP);

local($TMP) = "Temp_$$.xml";
open(TEMP,">$TMP") or die "Failed to open $TMP";
my($fn) = undef;
while(<>) {
  /<MODULE-DEF.*>/ and &print_hdr and $fn = undef;
  /<MODULE-DEF.*>/../<\/MODULE-DEF>/ and chomp and print TEMP "$_\n";
  /<SHORT-NAME>(.*)<\/SHORT-NAME>/ and !defined($fn) and $fn = "$1.epd";
  /<\/MODULE-DEF>/ and do { &print_ftr;create_file($fn);$fn = undef; }
}
close(TEMP);
unlink($TMP);
exit(0);

sub create_file($) {
  my($fn) = shift;
  print "Creating file $fn...\n";
  TEMP->flush;
  copy( "$TMP", "$fn" );
  seek( TEMP, 0, 0 );
  truncate( TEMP, 0 );
}

sub print_hdr {
  print TEMP <<EOF1
<?xml version="1.0" encoding="UTF-8"?>
<AUTOSAR xmlns="http://autosar.org" 
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
         xsi:schemaLocation="http://autosar.org AUTOSAR_ECUC.xsd">
  <TOP-LEVEL-PACKAGES>
    <AR-PACKAGE UUID="ECUC:AUTOSAR">
      <SHORT-NAME>AUTOSAR</SHORT-NAME>
      <ELEMENTS>
EOF1
}

sub print_ftr {
  print TEMP <<EOF2
      </ELEMENTS>
    </AR-PACKAGE>
  </TOP-LEVEL-PACKAGES>
</AUTOSAR>
EOF2
}
