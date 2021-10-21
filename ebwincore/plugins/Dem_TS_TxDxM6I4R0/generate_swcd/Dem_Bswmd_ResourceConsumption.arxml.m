
[!CALL "CALCULATE_MULTICORE_SETTINGS"!][!//
              <PROGRAMMING-LANGUAGE>C</PROGRAMMING-LANGUAGE>
              <RESOURCE-CONSUMPTION>
                <SHORT-NAME>ResourceConsumption</SHORT-NAME>
                <MEMORY-SECTIONS>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_CLEARED_UNSPECIFIED</SHORT-NAME>
                    <ALIGNMENT>UNSPECIFIED</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR_CLEARED</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_CLEARED_32</SHORT-NAME>
                    <ALIGNMENT>32</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR_CLEARED</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_CLEARED_16</SHORT-NAME>
                    <ALIGNMENT>16</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR_CLEARED</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_CLEARED_8</SHORT-NAME>
                    <ALIGNMENT>8</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR_CLEARED</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_INIT_UNSPECIFIED</SHORT-NAME>
                    <ALIGNMENT>UNSPECIFIED</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_INIT_32</SHORT-NAME>
                    <ALIGNMENT>32</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_INIT_16</SHORT-NAME>
                    <ALIGNMENT>16</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_INIT_8</SHORT-NAME>
                    <ALIGNMENT>8</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_SAVED_ZONE_UNSPECIFIED</SHORT-NAME>
                    <ALIGNMENT>UNSPECIFIED</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR_CLEARED</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_SAVED_ZONE_16</SHORT-NAME>
                    <ALIGNMENT>16</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR_CLEARED</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>VAR_SAVED_ZONE_8</SHORT-NAME>
                    <ALIGNMENT>8</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/VAR_CLEARED</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
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
                    <SHORT-NAME>CONST_8</SHORT-NAME>
                    <ALIGNMENT>8</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/CONST</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>CALIB_UNSPECIFIED</SHORT-NAME>
                    <ALIGNMENT>UNSPECIFIED</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/CALIB</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>CALIB_32</SHORT-NAME>
                    <ALIGNMENT>32</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/CALIB</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>CALIB_16</SHORT-NAME>
                    <ALIGNMENT>16</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/CALIB</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>CALIB_8</SHORT-NAME>
                    <ALIGNMENT>8</ALIGNMENT>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/CALIB</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
                  <MEMORY-SECTION>
                    <SHORT-NAME>CODE</SHORT-NAME>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/CODE</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
[!IF "(count(DemConfigSet/*/DemEventParameter/*[node:refexists(DemDTCClassRef) and as:ref(DemDTCClassRef)/DemImmediateNvStorage = 'true']) > 0) or
      (DemGeneral/DemImmediateStorageInternalEvents = 'true') or (DemGeneral/DemClearDTCBehavior != 'DEM_CLRRESP_VOLATILE')"!][!//
                  <MEMORY-SECTION>
                    <SHORT-NAME>[!"text:toupper(as:modconf('Dem')[1]/DemGeneral/DemCallbackMemStackMainFuncTrigger)"!]_CODE</SHORT-NAME>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/CALLOUT_CODE</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
[!ENDIF!][!//
[!IF "node:exists(DemGeneral/DemCalloutDynamicDTCFnc)"!][!//
                  <MEMORY-SECTION>
                    <SHORT-NAME>[!"text:toupper(as:modconf('Dem')[1]/DemGeneral/DemCalloutDynamicDTCFnc)"!]_CODE</SHORT-NAME>
                    <SW-ADDRMETHOD-REF DEST="SW-ADDR-METHOD">/AUTOSAR_MemMap/SwAddrMethods/CALLOUT_CODE</SW-ADDRMETHOD-REF>
                  </MEMORY-SECTION>
[!ENDIF!][!//
                </MEMORY-SECTIONS>
              </RESOURCE-CONSUMPTION>
              <SW-VERSION>6.4.1</SW-VERSION>
[!IF "DemGeneral/DemRteUsage = 'true'"!][!//
              <SWC-BSW-MAPPING-REF DEST="SWC-BSW-MAPPING">/AUTOSAR_Dem/SwcBswMappings/SwcBswMapping_[!"$MasterCoreId"!]</SWC-BSW-MAPPING-REF>
[!ENDIF!][!//
              <VENDOR-ID>1</VENDOR-ID>
              <AR-RELEASE-VERSION>4.0.3</AR-RELEASE-VERSION>
              <BEHAVIOR-REF DEST="BSW-INTERNAL-BEHAVIOR">/AUTOSAR_Dem/BswModuleDescriptions/Dem/InternalBehavior_[!"$MasterCoreId"!]</BEHAVIOR-REF>
