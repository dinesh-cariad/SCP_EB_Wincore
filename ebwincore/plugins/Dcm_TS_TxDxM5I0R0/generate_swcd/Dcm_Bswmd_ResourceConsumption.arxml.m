
              <PROGRAMMING-LANGUAGE>C</PROGRAMMING-LANGUAGE>
              <RESOURCE-CONSUMPTION>
                <SHORT-NAME>ResourceConsumption</SHORT-NAME>
                <MEMORY-SECTIONS>
                  <MEMORY-SECTION>
                    <SHORT-NAME>CONST_UNSPECIFIED</SHORT-NAME>
                    <ALIGNMENT>UNSPECIFIED</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/CONST</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>CONST_32</SHORT-NAME>
                    <ALIGNMENT>32</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/CONST</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>CONST_16</SHORT-NAME>
                    <ALIGNMENT>16</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/CONST</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_CLEARED_8</SHORT-NAME>
                    <ALIGNMENT>8</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR_CLEARED</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>CONST_8</SHORT-NAME>
                    <ALIGNMENT>8</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/CONST</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>CODE</SHORT-NAME>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/CODE</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>CALLOUT_CODE</SHORT-NAME>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/CODE</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_INIT_UNSPECIFIED</SHORT-NAME>
                    <ALIGNMENT>UNSPECIFIED</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_CLEARED_UNSPECIFIED</SHORT-NAME>
                    <ALIGNMENT>UNSPECIFIED</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR_CLEARED</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_POWER_ON_CLEARED_UNSPECIFIED</SHORT-NAME>
                    <ALIGNMENT>UNSPECIFIED</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_INIT_8</SHORT-NAME>
                    <ALIGNMENT>8</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_INIT_16</SHORT-NAME>
                    <ALIGNMENT>16</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_CLEARED_16</SHORT-NAME>
                    <ALIGNMENT>16</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR_CLEARED</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_CLEARED_32</SHORT-NAME>
                    <ALIGNMENT>32</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR_CLEARED</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_INIT_32</SHORT-NAME>
                    <ALIGNMENT>32</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>CALIB_8</SHORT-NAME>
                    <ALIGNMENT>8</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/CALIB</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspPid/*)"!]
                  <MEMORY-SECTION>
                    <SHORT-NAME>CALIB_UNSPECIFIED</SHORT-NAME>
                    <ALIGNMENT>UNSPECIFIED</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/CALIB</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
[!ENDIF!]
                </MEMORY-SECTIONS>
              </RESOURCE-CONSUMPTION>
              <SW-VERSION>5.0.9</SW-VERSION>
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmRteUsage = 'true'"!]
              <SWC-BSW-MAPPING-REF DEST="SWC-BSW-MAPPING">/AUTOSAR_Dcm/SwcBswMappings/SwcBswMapping_0</SWC-BSW-MAPPING-REF>
[!ENDIF!]
              <VENDOR-ID>1</VENDOR-ID>
              <AR-RELEASE-VERSION>4.0.3</AR-RELEASE-VERSION>
              <BEHAVIOR-REF DEST="BSW-INTERNAL-BEHAVIOR">/AUTOSAR_Dcm/BswModuleDescriptions/Dcm/InternalBehavior_0</BEHAVIOR-REF>
              <VENDOR-SPECIFIC-MODULE-DEF-REFS>
                <VENDOR-SPECIFIC-MODULE-DEF-REF DEST="ECUC-MODULE-DEF">/TS_TxDxM5I0R0/Dcm</VENDOR-SPECIFIC-MODULE-DEF-REF>
              </VENDOR-SPECIFIC-MODULE-DEF-REFS>
