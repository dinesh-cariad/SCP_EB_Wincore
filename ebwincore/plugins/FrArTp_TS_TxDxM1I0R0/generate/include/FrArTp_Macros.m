[!// transform stMin float to stMin byte coding
[!MACRO "stMinConversion", "time"!][!//
[!IF "$time < 0.0001"!][!//
[!VAR "stMin"="0.0"!][!//
[!ELSE!][!//
[!IF "$time <= 0.0009"!][!//
[!VAR "stMin"="240 + ceiling($time div 0.0001)"!][!//
[!ELSE!][!//
[!VAR "stMin"="ceiling($time div 0.001)"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!"num:i($stMin)"!][!//
[!ENDMACRO!][!//
[!//
[!// transform time to ticks
[!MACRO "timeToTicks", "time"!][!//
[!"num:i(ceiling($time div node:value(as:modconf('FrArTp')[1]/FrArTpGeneral/FrArTpMainFuncCycle)))"!][!//
[!ENDMACRO!][!//
[!//
[!// get protocol type
[!MACRO "getProtocolType", "protocolType"!][!//
[!IF "$protocolType = 'FRARTP_ISO'"!][!//
[!"num:i('1')"!][!//
[!ELSE!][!//
[!"num:i('2')"!][!//
[!ENDIF!][!//
[!ENDMACRO!][!//
[!//
[!// get address size
[!MACRO "getAddrSize", "addrType"!][!//
[!IF "$addrType = 'FRARTP_TB'"!][!//
[!"num:i('2')"!][!//
[!ELSE!][!//
[!"num:i('1')"!][!//
[!ENDIF!][!//
[!ENDMACRO!][!//
[!//
[!// get maximum number of concurrent connections per channel
[!MACRO "getNumActConnections", "path"!][!//
[!VAR "numConnections"="num:i(count(as:ref($path)/FrArTpConnection/*))"!][!//
[!IF "node:exists(as:ref($path)/FrArTpConcurrentConnections)"!][!//
[!"num:i(node:value(as:ref($path)/FrArTpConcurrentConnections))"!][!//
[!ELSE!][!//
[!"num:i($numConnections)"!][!//
[!ENDIF!][!//
[!ENDMACRO!][!//
[!//
[!// Write config data of type FrArTp_ChannelCfgType
[!VAR "ActiveConnectionsIdx"="'0'"!][!//
[!MACRO "ChannelCfg", "path"!][!//
[!VAR "numMaxActConn"!][!//
[!CALL "getNumActConnections", "path"="$path"!][!//
[!ENDVAR!][!//
[!VAR "txPoolIdx"="text:split(text:grep(text:split($Channel2TxPoolMap),concat($path,':.*')),':')[2]"!][!//
        <field comment="[!"node:name(as:ref($path))"!]">
          <field>
            <int>[!CALL "timeToTicks", "time"="node:value(as:ref($path)/FrArTpTimeoutAr)"!]</int> <!-- timeoutAr -->
          </field>
          <field>
            <int>[!CALL "timeToTicks", "time"="node:value(as:ref($path)/FrArTpTimeBr)"!]</int> <!-- timeBr -->
          </field>
          <field>
            <int>[!CALL "timeToTicks", "time"="node:value(as:ref($path)/FrArTpTimeoutCr)"!]</int> <!-- timeoutCr -->
          </field>
          <field>
            <int>[!CALL "timeToTicks", "time"="node:value(as:ref($path)/FrArTpTimeoutAs)"!]</int> <!-- timeoutAs -->
          </field>
          <field>
            <int>[!CALL "timeToTicks", "time"="node:value(as:ref($path)/FrArTpTimeoutBs)"!]</int> <!-- timeoutBs -->
          </field>
          <field>
            <int>[!CALL "timeToTicks", "time"="node:value(as:ref($path)/FrArTpTimeCs)"!]</int> <!-- timeCs -->
          </field>
          <field>
            <int>[!"num:i($txPoolIdx)"!]</int> <!-- txPoolIdx -->
          </field>
          <field>
            <int>[!"num:i($ActiveConnectionsIdx)"!]</int> <!-- actConnIdx -->
          </field>
          <field>
            <int>[!"$numMaxActConn"!]</int> <!-- numMaxActConn -->
          </field>
          <field>
            <int>[!CALL "getAddrSize", "addrType"="node:value(as:ref($path)/FrArTpAdrType)"!]</int> <!-- addrSize -->
          </field>
          <field>
            <int>[!"num:i(node:value(as:ref(as:ref($path)/FrArTpPdu/*[1]/FrArTpPduRef)/PduLength))"!]</int> <!-- payloadLength -->
          </field>
          <field>
            <int>[!CALL "getProtocolType", "protocolType"="node:value(as:ref($path)/FrArTpLm)"!]</int> <!-- format -->
          </field>
          <field>
            <int>[!"num:i(node:value(as:ref($path)/FrArTpMaxBs))"!]</int> <!-- maxBs -->
          </field>
          <field>
            <int>[!"num:i(node:value(as:ref($path)/FrArTpMaxWft))"!]</int><!-- MaxWft -->
          </field>
          <field>
            <int>[!CALL "stMinConversion", "time"="node:value(as:ref($path)/FrArTpStMin)"!]</int> <!-- stMin -->
          </field>
        </field>
[!VAR "ActiveConnectionsIdx"="$ActiveConnectionsIdx + $numMaxActConn"!][!//
[!ENDMACRO!][!//
[!//
[!// Write config data of type FrArTp_RxPduCfgType
[!MACRO "RxPduCfg", "rxPduId"!][!//
[!VAR "rxPoolIdx"="$RxPdu2RxPoolMap"!]
[!VAR "rxPoolIdx"="text:split(text:grep(text:split($RxPdu2RxPoolMap),concat('.*:',num:i($rxPduId),':.*')),':')[3]"!][!//
[!VAR "firstChannelName"="text:split(text:grep(text:split($RxPool2ChannelMap),concat($rxPoolIdx,':.*'))[1],':')[2]"!][!//
[!VAR "firstChannelId"="text:split(text:grep(text:split($Channel2IdxMap),concat($firstChannelName,':.*')),':')[2]"!][!//
[!VAR "numConnections"="'0'"!][!//
[!VAR "baseConnectionIdx"="'65535'"!][!//
[!FOR "i"="1" TO "count(text:grep(text:split($RxPool2ChannelMap),concat($rxPoolIdx,':.*')))"!][!//
[!VAR "channel"="text:grep(text:split($RxPool2ChannelMap),concat($rxPoolIdx,':.*'))[position()=$i]"!][!//
[!VAR "channelPath"="text:split($channel,':')[2]"!][!//
[!VAR "channelConnections"="count(as:ref($channelPath)/FrArTpConnection/*)"!][!//
[!VAR "numConnections"="num:i($numConnections + $channelConnections)"!][!//
[!FOR "iConn"="1" TO "$channelConnections"!][!//
[!VAR "connectionPath"="as:path(as:ref($channelPath)/FrArTpConnection/*[position() = $iConn])"!][!//
[!VAR "connectionIdx"="text:split(text:grep(text:split($Connection2IdxMap),concat('.*:',$connectionPath)),':')[1]"!][!//
[!IF "$connectionIdx < $baseConnectionIdx"!][!//
[!VAR "baseConnectionIdx" = "$connectionIdx"!][!//
[!ENDIF!][!//
[!ENDFOR!][!//
[!ENDFOR!][!//
        <field>
          <field>
            <int>[!"num:i($baseConnectionIdx)"!]</int> <!-- startConnIdx -->
          </field>
          <field>
            <int>[!"num:i($numConnections)"!]</int> <!-- numConn -->
          </field>
          <field>
            <int>[!"num:i($firstChannelId)"!]</int> <!-- channelIdx -->
          </field>
        </field>
[!ENDMACRO!][!//
[!//
[!// get handle Id of RxSduHandleId (i.e. the PduRSourcePduHandleId corresponding to FrArTpRxSduRef)
[!MACRO "getRxSduHandleId", "path"!][!//
[!IF "node:exists(as:ref($path)/FrArTpRxSdu)"!][!//
[!"asc:getPduId('PduR', 'ForLowerLayer', 'Tp', 'Rx', asc:getPdus('PduR', 'ForLowerLayer', 'Tp', 'Rx', 1, as:ref($path)/FrArTpRxSdu/FrArTpRxSduRef)[1])"!][!//
[!ELSE!][!//
[!"num:i(65535)"!][!//
[!ENDIF!][!//
[!ENDMACRO!][!//
[!//
[!// get handle Id of TxSduHandleId (i.e. the PduRDestPduHandleId corresponding to FrArTpTxSduRef)
[!MACRO "getTxSduHandleId", "path"!][!//
[!IF "node:exists(as:ref($path)/FrArTpTxSdu)"!][!//
[!"asc:getPduId('PduR', 'ForLowerLayer', 'Tp', 'Tx', asc:getPdus('PduR', 'ForLowerLayer', 'Tp', 'Tx', 1, as:ref($path)/FrArTpTxSdu/FrArTpTxSduRef)[1])"!][!//
[!ELSE!][!//
[!"num:i(65535)"!][!//
[!ENDIF!][!//
[!ENDMACRO!][!//
[!//
[!//
[!// get handle Id of TxPdu (i.e. the FrIfTxPduId corresponding to FrArTpPduRef)
[!MACRO "getTxPduId", "path"!][!//
[!"asc:getPduId('FrIf', 'ForUpperLayer', 'If', 'Tx', asc:getPdus('FrIf', 'ForUpperLayer', 'If', 'Tx', 1, as:ref($path)/FrArTpPduRef)[1])"!][!//
[!ENDMACRO!][!//
[!//
[!//
[!// get channel index of connection path
[!MACRO "getChannelIdxOfConnection", "path"!][!//
[!"num:i(text:split(text:grep(text:split($Channel2IdxMap),concat(as:path(as:ref($path)/../..),':.*')),':')[2])"!][!//
[!ENDMACRO!][!//
[!// Write config data of type FrArTp_ConnectionCfgType
[!MACRO "ConnectionCfg", "path"!][!//
        <field comment="[!"node:name(as:ref($path))"!]">
          <field>
            <int>[!"num:i(node:value(as:ref($path)/FrArTpLa))"!]</int> <!-- localAddress -->
          </field>
          <field>
            <int>[!"num:i(node:value(as:ref($path)/FrArTpRa))"!]</int> <!-- remoteAddress -->
          </field>
          <field>
            <int>[!CALL "getRxSduHandleId", "path"="$path"!]</int> <!-- pduRRxPduId -->
          </field>
          <field>
            <int>[!CALL "getTxSduHandleId", "path"="$path"!]</int> <!-- pduRTxPduId -->
          </field>
          <field>
            <int>[!CALL "getChannelIdxOfConnection", "path"="$path"!]</int> <!-- channelIdx -->
          </field>
          <field>
          [!IF "node:value(as:ref($path)/FrArTpMultRec) = 'true'"!]
            <int>1</int> <!-- isMultRec -->
          [!ELSE!]
            <int>0</int> <!-- isMultRec -->
          [!ENDIF!]
          </field>
        </field>
[!ENDMACRO!][!//
[!//
[!// Write config data of type FrArTp_TxPoolCfgType
[!MACRO "TxPoolCfg", "txPoolIdx"!][!//
[!VAR "numTxPdus"="count(text:grep(text:split($TxPdu2TxPoolMap),concat('.*:',num:i($txPoolIdx))))"!][!//
[!VAR "LowestTxPduId"="65535"!][!//
[!FOR "i"="1" TO "count(text:grep(text:split($TxPdu2TxPoolMap),concat('.*:.*:',num:i($txPoolIdx))))"!][!//
[!VAR "txPool"="text:grep(text:split($TxPdu2TxPoolMap),concat('.*:.*:',num:i($txPoolIdx)))[position()=$i]"!][!//
[!VAR "TxPduId"="num:i(text:split($txPool,':')[2])"!][!//
[!IF "$LowestTxPduId > $TxPduId"!][!//
[!VAR "LowestTxPduId"="num:i($TxPduId)"!][!//
[!ENDIF!][!//
[!ENDFOR!][!//
        <field>
          <field>
            <int>[!"$LowestTxPduId"!]</int> <!-- startTxPduIdx -->
          </field>
          <field>
            <int>[!"num:i($numTxPdus)"!]</int> <!-- numTxPdus -->
          </field>
        </field>
[!ENDMACRO!][!//
[!//
[!// Write config data of type FrArTp_TxPduCfgType
[!MACRO "TxPduCfg", "txPduIdx"!][!//
[!VAR "txPoolIdx"="text:split(text:grep(text:split($TxPdu2TxPoolMap),concat('.*:',num:i($txPduIdx),':.*')),':')[3]"!][!//
[!VAR "PduRef"="text:split(text:grep(text:split($TxPdu2TxPoolMap),concat('.*:',num:i($txPduIdx),':.*')),':')[1]"!][!//
[!VAR "PduRefPath"="as:path(FrArTpMultipleConfig/FrArTpChannel/*/FrArTpPdu/*[(FrArTpPduDirection='FRARTP_TX') and (FrArTpPduRef = concat('ASPath:',$PduRef))][1])"!]
        <field>
          <field>
            <int>[!CALL "getTxPduId", "path"="$PduRefPath"!]</int> <!-- frIfTxPduId -->
          </field>
          <field>
            <int>[!"num:i($txPoolIdx)"!]</int> <!-- txPoolIdx -->
          </field>
        </field>
[!ENDMACRO!][!//
[!//
[!// Write config data of type TxSdu2Conn
[!MACRO "TxSdu2ConnCfg", "path"!][!//
[!VAR "connectionIdx"="text:split(text:grep(text:split($Connection2IdxMap),concat('.*:',$path)),':')[1]"!][!//
        <field>
          <int>[!"num:i($connectionIdx)"!]</int> <!-- connIdx -->
        </field>
[!ENDMACRO!][!//
[!//
[!// Write config data of type RxSdu2Conn
[!MACRO "RxSdu2ConnCfg", "path"!][!//
[!VAR "connectionIdx"="text:split(text:grep(text:split($Connection2IdxMap),concat('.*:',$path)),':')[1]"!][!//
        <field>
          <int>[!"num:i($connectionIdx)"!]</int> <!-- connIdx -->
        </field>
[!ENDMACRO!][!//
[!//
