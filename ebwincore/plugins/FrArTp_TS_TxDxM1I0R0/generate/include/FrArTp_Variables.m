[!// Mapping from receive PDU paths and PDU IDs to receive PDU pool indices
[!// <path(ref(FrArTpPduRef))>:<FrArTpPduId>:<RxPoolIdx>
[!VAR "RxPdu2RxPoolMap"="''"!][!//
[!// Mapping from receive PDU pool indices to channel paths
[!// <RxPoolIdx>:<path(FrArTpChannel)>
[!VAR "RxPool2ChannelMap"="''"!][!//
[!// Mapping from channel paths to receive pool indices
[!// <path(FrArTpChannel)>:<RxPoolIdx>
[!VAR "Channel2RxPoolMap"="''"!][!//
[!// Mapping from channel paths to channel indices
[!// <path(FrArTpChannel)>:<ChannelIdx>
[!VAR "Channel2IdxMap"="''"!][!//
[!// Mapping from connection paths to connection indices
[!// <path(rArTpConnection)>:<ConnectionIdx>
[!VAR "Connection2IdxMap"="''"!][!//
[!// Number of used receive PDU pools
[!VAR "RxPoolCounter"="'-1'"!][!//
[!//
[!SELECT "as:modconf('FrArTp')[1]/FrArTpMultipleConfig"!][!//
[!// Iterate all channels
[!LOOP "./FrArTpChannel/*"!][!// Iterate all channels
  [!VAR "NewRxPool"="'TRUE'"!][!//
  [!VAR "ExistingRxPool"="''"!][!//
  [!// Iterate all receive PDUs
  [!LOOP "node:order(FrArTpPdu/*[FrArTpPduDirection='FRARTP_RX'],'FrArTpPduId')"!][!//
    [!IF "contains($RxPdu2RxPoolMap,concat(as:path(node:ref(FrArTpPduRef)),':'))"!][!//
      [!VAR "NewRxPool"="'FALSE'"!][!//
      [!VAR "ExistingRxPool"="text:split(text:grep(text:split($RxPdu2RxPoolMap),concat(as:path(node:ref(FrArTpPduRef)),'.*')),':')[3]"!][!//
    [!ENDIF!][!//
  [!ENDLOOP!][!//
  [!IF "$NewRxPool='TRUE'"!][!//
    [!VAR "RxPoolCounter" = "$RxPoolCounter + 1"!][!//
    [!VAR "UsedRxPool" = "$RxPoolCounter"!][!//
    [!LOOP "node:order(FrArTpPdu/*[FrArTpPduDirection='FRARTP_RX'],'FrArTpPduId')"!][!//
      [!VAR "RxPdu2RxPoolMap" = "concat($RxPdu2RxPoolMap,as:path(node:ref(FrArTpPduRef)),':',node:value(FrArTpPduId),':',num:i($UsedRxPool),' ')"!][!//
    [!ENDLOOP!][!//
  [!ELSE!][!//
    [!VAR "UsedRxPool" = "$ExistingRxPool"!][!//
  [!ENDIF!][!//
  [!VAR "Channel2RxPoolMap" = "concat($Channel2RxPoolMap,as:path(.),':',num:i($UsedRxPool),' ')"!][!//
  [!VAR "RxPool2ChannelMap" = "concat($RxPool2ChannelMap,num:i($UsedRxPool),':',as:path(.),' ')"!][!//
[!ENDLOOP!][!//
[!//
[!// Mapping from transmit PDU paths and PDU IDs to transmit PDU pool indices
[!// <path(ref(FrArTpPduRef))>:<FrArTpPduId>:<TxPoolIdx>
[!VAR "TxPdu2TxPoolMap"="''"!][!//
[!// Mapping from transmit PDU pool indices to channel paths
[!// <TxPoolIdx>:<path(FrArTpChannel)>
[!VAR "TxPool2ChannelMap"="''"!][!//
[!// Mapping from channel paths to transmit PDU pool indices
[!// <path(FrArTpChannel)>:<TxPoolIdx>
[!VAR "Channel2TxPoolMap"="''"!][!//
[!// Number of used transmit PDU pools
[!VAR "TxPoolCounter"="'-1'"!][!//
[!// List of transmit PDU IDs
[!VAR "PduIds"="''"!][!//
[!//
[!LOOP "./FrArTpChannel/*/FrArTpPdu/*[FrArTpPduDirection = 'FRARTP_TX'][1]"!][!//
[!VAR "PduIds"="concat($PduIds,' ',FrArTpPduId)"!][!//
[!ENDLOOP!][!//
[!//
[!VAR "PduIdListSorted"="num:order(text:split($PduIds))"!][!//
[!//
[!// Iterate all transmit PDU IDs
[!FOR "i"="1" TO "count(text:split($PduIdListSorted))"!][!//
[!VAR "PduId"="num:i(text:replace(text:replace(text:split($PduIdListSorted,',')[num:i($i)],'\[',''),'\]',''))"!][!//
[!// Iterate all transmit PDUs with the current IDs
[!LOOP "./FrArTpChannel/*/FrArTpPdu/*[(FrArTpPduDirection = 'FRARTP_TX') and (FrArTpPduId = $PduId)]/../.."!][!//
  [!VAR "NewTxPool"="'TRUE'"!][!//
  [!VAR "ExistingTxPool"="''"!][!//
  [!LOOP "node:order(FrArTpPdu/*[FrArTpPduDirection='FRARTP_TX'],'FrArTpPduId')"!][!//
    [!IF "contains($TxPdu2TxPoolMap,concat(as:path(node:ref(FrArTpPduRef)),':'))"!][!//
      [!VAR "NewTxPool"="'FALSE'"!][!//
      [!VAR "ExistingTxPool"="text:split(text:grep(text:split($TxPdu2TxPoolMap),concat(as:path(node:ref(FrArTpPduRef)),'.*')),':')[3]"!][!//
    [!ENDIF!][!//
  [!ENDLOOP!][!//
  [!IF "$NewTxPool='TRUE'"!][!//
    [!VAR "TxPoolCounter" = "$TxPoolCounter + 1"!][!//
    [!VAR "UsedTxPool" = "$TxPoolCounter"!][!//
    [!LOOP "node:order(FrArTpPdu/*[FrArTpPduDirection='FRARTP_TX'],'FrArTpPduId')"!][!//
      [!VAR "TxPdu2TxPoolMap" = "concat($TxPdu2TxPoolMap,as:path(node:ref(FrArTpPduRef)),':',node:value(FrArTpPduId),':',num:i($UsedTxPool),' ')"!][!//
    [!ENDLOOP!][!//
  [!ELSE!][!//
    [!VAR "UsedTxPool" = "$ExistingTxPool"!][!//
  [!ENDIF!][!//
  [!VAR "Channel2TxPoolMap" = "concat($Channel2TxPoolMap,as:path(.),':',num:i($UsedTxPool),' ')"!][!//
  [!VAR "TxPool2ChannelMap" = "concat($TxPool2ChannelMap,num:i($UsedTxPool),':',as:path(.),' ')"!][!//
[!ENDLOOP!][!//
[!ENDFOR!][!//
[!//
[!VAR "channelIdx" = "'0'"!][!//
[!LOOP "text:order(text:split($RxPool2ChannelMap))"!][!//
[!VAR "Channel2IdxMap" = "concat($Channel2IdxMap,text:split(.,':')[2],':',num:i($channelIdx),' ')"!][!//
[!VAR "channelIdx" = "$channelIdx + 1"!][!//
[!ENDLOOP!][!//
[!ENDSELECT!][!//
[!//
[!VAR "connectionIdx" = "'0'"!][!//
[!FOR "i" = "1" TO "count(text:split($Channel2IdxMap))"!][!//
  [!VAR "channel"="text:split($Channel2IdxMap)[position() = $i]"!][!//
  [!VAR "channelPath" = "text:split($channel,':')[1]"!][!//
  [!FOR "iConn" = "1" TO "count(as:ref($channelPath)/FrArTpConnection/*)"!][!//
    [!VAR "Connection2IdxMap" = "concat($Connection2IdxMap,num:i($connectionIdx),':',as:path(as:ref($channelPath)/FrArTpConnection/*[position() = $iConn]),' ')"!][!//
    [!VAR "connectionIdx" = "$connectionIdx + 1"!][!//
  [!ENDFOR!][!//
[!ENDFOR!][!//
[!VAR "numTxSdus"="count(as:modconf('FrArTp')[1]/FrArTpMultipleConfig/FrArTpChannel/*/FrArTpConnection/*[node:exists(FrArTpTxSdu)])"!][!//
[!VAR "numRxSdus"="count(as:modconf('FrArTp')[1]/FrArTpMultipleConfig/FrArTpChannel/*/FrArTpConnection/*[node:exists(FrArTpRxSdu)])"!][!//
