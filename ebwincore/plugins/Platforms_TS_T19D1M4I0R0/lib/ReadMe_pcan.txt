===============================================================================
ReadMe.txt

PCAN-USB
Copyright (c) 2011 PEAK-System Technik GmbH, Darmstadt, Germany
All rights reserved.
===============================================================================

Maximize the Notepad Window to read this file more easily.


Contents:
---------
  * Introduction
  * System Requirements
  * Contents of this directory
  * Installation
  * How to contact PEAK-System Technik GmbH
  * LIFE SUPPORT APPLIANCES


Introduction
------------
The PCAN-USB adapter allows the connection of a CAN bus to an USB interface of
an IBM compatible PC. It is especially suitable for use with notebook computers
since these usually do not have an ISA or PCI slot. With the help of this
adapter, any PC can be linked to a High-speed CAN (HS-CAN).

With the so-called ISO version of the PCAN-USB adapter an isolation of up to
500 V between the PC and the CAN parts of the adapter is achieved by use of a
DC/DC converter and an optocoupler.

The provided drivers, the PCAN-Light API, and the PCAN-View CAN bus Monitor
software are the feature-reduced versions of the larger software packages
PCAN-Evaluation, PCAN-Developer, or PCAN-Explorer. These can be aquired
separately.


System Requirements
-------------------
- A free USB port (USB 1.1 or USB 2.0) at the PC or at an USB hub connected
  to the PC
- Operating systems: Windows (XP, Vista, Windows 7) and Linux


Contents of this directory
--------------------------
ReadMe.txt
    This text file.

LiesMich.txt
    The german translation of this file.

PCAN-USB_UserMan_eng.pdf
    The english User Manual in Adobe Reader PDF format.

PCAN-USB_UserMan_deu.pdf
    The german User Manual in Adobe Reader PDF format.

PeakOemDrv.exe
    The driver setup program.

\Drivers
    The 32Bit and 64Bit device drivers for Windows.

\PCAN-Basic API
    Contains API definitions for several development environments,
    interface-DLL, and sample projects that demonstrate how to use the
    PCAN-Basic API.	
	
\PCAN-View
    Contains a simple CAN Bus monitor for Windows.

Installation under Windows
--------------------------
We recommend that you setup the driver before connecting the PCAN-USB adapter
to the computer for the first time.


> Do the following to setup the driver:

1.  Please make sure that you are logged in as user with administrator
    privileges (not required for normal use of the PCAN-USB adapter later on).

2.  Start the driver setup program PeakOemDrv.exe.

3.  Follow the instructions of the setup program and install the driver for
    the PCAN-USB adapter.

Tip:
    During this procedure you can additionally select to install the CAN
    monitor PCAN-View for Windows.

> Do the following to connect the PCAN-USB adapter and complete the
  initialization:

1.  Connect the PCAN-USB adapter to an USB port of your PC. The PC can remain
    powered on.

2.  Windows reports that new hardware has been detected and possibly starts an
    installation wizard. This depends on the used Windows version. If
    applicable, confirm the steps for driver initialization.

After the installation process is completed successfully, the red LED on
the PCAN-USB adapter is illuminated.


How to contact PEAK-System Technik GmbH
---------------------------------------
If you have any questions concerning the installation of the PCAN-USB adapter,
or require information about other PEAK CAN products, then please contact us:

PEAK-System Technik GmbH
Otto-Roehm-Str. 69
D-64293 Darmstadt
Germany

Tel. +49 6151 / 8173-20
FAX  +49 6151 / 8173-29

support@peak-system.com
http://www.peak-system.com


LIFE SUPPORT APPLIANCES
-----------------------
These products are not designed for use in life support appliances, devices,
or systems where malfunction of these products can reasonably be expected to
result in personal injury. PEAK-System customers using or selling these
products for use in such applications do so at their own risk and agree to
fully indemnify PEAK-System for any damages resulting from such improper use
or sale.
