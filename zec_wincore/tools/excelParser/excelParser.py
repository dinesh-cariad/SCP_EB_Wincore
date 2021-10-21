# This tool is not complete. The purpose of this tool is to parse Excel Sheet into Arxml file
import pandas
from lxml import etree

class CSig:
    def __init__(self, sigName, startByte, startBit, sendArt, length, direction):
        self.sigName = sigName
        self.startByte = startByte
        self.startBit = startBit
        self.sendArt = sendArt
        self.length = length
        self.direction = direction

    def getSigName(self):
        return self.sigName

    def getSigStartByte(self):
        return self.startByte

    def getSigStartBit(self):
        return self.startBit

    def getSigSendArt(self):
        return self.sendArt

    def getSigLength(self):
        return self.length

    def getSigDirection(self):
        return self.direction
    
class CPdu:
    def __init__(self, name, length):
        self.name = name
        self.length = length
        self.signals = list()

    def addSig(self, name, startByte, startBit, sendArt, length, direction):
        self.signals.append(CSig(name, startByte, startBit, sendArt, length, direction))

    def getPduName(self):
        return self.name

    def getPduLength(self):
        return self.length

    def getSigsList(self):
        return self.signals

    def getRxSigs(self):
        lst = list()
        for sig in self.signals:
            if sig.getSigDirection() == "E":
                lst.append(sig)
        return lst

    def getTxSigs(self):
        lst = list()
        for sig in self.signals:
            if sig.getSigDirection() == "S":
                lst.append(sig)
        return lst

    def isPduRx(self):
        if self.getRxSigs():
            return True
        return False

    def isPduTx(self):
        if self.getTxSigs():
            return True
        return False
    
class CDataBase:
    def __init__(self):
        self.pdusList = list()

    def addPdu (self,pduObj):
        self.pdusList.append(pduObj)

    def getAllPdusList(self):
        return self.pdusList

    def isPduInDB(self, pduName):
        for pdu in self.pdusList:
            if pduName == pdu.getPduName():
                return True
        return False

    def getAllSigsList(self):
        lst = list()
        for pdu in self.pdusList:
            for sig in pdu.getSigsList():
                lst.append(sig)
        return lst

    def getPduNmFromSigNm (self, signalName):
        for pdu in self.getAllPdusList():
            for sig in pdu.getSigsList():
                if signalName == sig.getSigName():
                    return pdu.getPduName()

    def getAllRxPdus(self):
        lst = list()
        for pdu in self.getAllPdusList():
            if pdu.isPduRx():
                lst.append(pdu)
        return lst
    
    def getAllTxPdus(self):
        lst = list()
        for pdu in self.getAllPdusList():
            if pdu.isPduTx():
                lst.append(pdu)
        return lst
   
excelFile = pandas.read_excel('Modified2.xlsx')    
prevPdu = ""
locDataBase = CDataBase()
locPdu = None
for index, pduName in enumerate (excelFile.PDU):
    if(isinstance(pduName, str)):
        if  prevPdu != pduName:
            if locPdu is not None:
                locDataBase.addPdu(locPdu)
            prevPdu = pduName
            locPdu = CPdu(pduName, int(excelFile.PDULength[index]))
            sigDirection = excelFile.VCP_ClimateControl[index]
            if sigDirection == "S" or sigDirection == "E":
                locPdu.addSig(excelFile.Signal[index], excelFile.StartByte[index], excelFile.StartBit[index], excelFile.Signalsendeart[index], int(excelFile.SignalLength[index]), sigDirection)
        else:
            sigDirection = excelFile.VCP_ClimateControl[index]
            if sigDirection == "S" or sigDirection == "E":
                locPdu.addSig(excelFile.Signal[index], excelFile.StartByte[index], excelFile.StartBit[index], excelFile.Signalsendeart[index], int(excelFile.SignalLength[index]), sigDirection)

if not locDataBase.isPduInDB(locPdu.getPduName()):
    locDataBase.addPdu(locPdu)


for pdu in locDataBase.getAllPdusList():
    print ("###################### ")
    print("Pdu name is: ")
    print (pdu.getPduName())
    print (pdu.getPduLength())
    print ("Signals list is: ")
    for sig in pdu.getSigsList():
        print ("-------------")
        print ("sigName: ", sig.getSigName())
        print ("getSigStartByte: ", sig.getSigStartByte())
        print ("getSigStartBit: ", sig.getSigStartBit())
        print ("getSigSendArt: ", sig.getSigSendArt())
        print ("getSigLength: ", sig.getSigLength())
        

# ws.saveXML('DataTypes.arxml')

autosar = etree.Element("AUTOSAR")
packages = etree.SubElement(autosar, "AR-PACKAGES")

pduPackage = etree.SubElement(packages, "AR-PACKAGE")
pduShortName = etree.SubElement(pduPackage, "SHORT-NAME")
pduShortName.text = "PDU"
pduElements = etree.SubElement(pduPackage, "ELEMENTS")

for pdu in locDataBase.getAllPdusList():
    pduElement = etree.SubElement(pduElements, "I-SIGNAL-I-PDU")
    pduShortName = etree.SubElement(pduElement, "SHORT-NAME")
    pduShortName.text = pdu.getPduName()
    pduLength = etree.SubElement(pduElement, "LENGTH")
    pduLength.text = str(pdu.getPduLength())
    pduTimingSpecs = etree.SubElement(pduElement, "I-PDU-TIMING-SPECIFICATIONS")
    pduTiming = etree.SubElement(pduTimingSpecs, "I-PDU-TIMING")
    pduMinDelay = etree.SubElement(pduTiming, "MINIMUM-DELAY")
    transModeDecl = etree.SubElement(pduTiming, "TRANSMISSION-MODE-DECLARATION")
    transModeTiming = etree.SubElement(transModeDecl, "TRANSMISSION-MODE-TRUE-TIMING")
    cyclicTiming = etree.SubElement(transModeTiming, "CYCLIC-TIMING")
    eventTiming = etree.SubElement(transModeTiming, "EVENT-CONTROLLED-TIMING")
    

    sigsToPduMap = etree.SubElement(pduElement, "I-SIGNAL-TO-PDU-MAPPINGS")
    for sig in pdu.getSigsList():
        sigToPduMap  = etree.SubElement(sigsToPduMap, "I-SIGNAL-TO-PDU-MAPPING")
        sigToPduMapName = etree.SubElement(sigToPduMap, "SHORT-NAME")
        sigToPduMapName.text = sig.getSigName()


iSigPackage = etree.SubElement(packages, "AR-PACKAGE")
iSigShortName = etree.SubElement(iSigPackage, "SHORT-NAME")
iSigShortName.text = "ISignal"
iSigElements = etree.SubElement(iSigPackage, "ELEMENTS")
for sig in locDataBase.getAllSigsList():
    iSignal = etree.SubElement(iSigElements, "I-SIGNAL")
    sigShortName = etree.SubElement(iSignal, "SHORT-NAME")
    dtPolicy = etree.SubElement(iSignal, "DATA-TYPE-POLICY")
    dtPolicy.text = "OVERRIDE"
    sigName = sig.getSigName()
    sigShortName.text = (sigName + "_XIX_" + locDataBase.getPduNmFromSigNm(sigName))
    sigLength = etree.SubElement(iSignal, "LENGTH")
    sigLength.text = str(sig.getSigLength())
    sigRef = etree.SubElement(iSignal, "SYSTEM-SIGNAL-REF")
    sigRef.text = "/Signal/" + sig.getSigName()

sigPackage = etree.SubElement(packages, "AR-PACKAGE")
sigShortName = etree.SubElement(sigPackage, "SHORT-NAME")
sigShortName.text = "Signal"
sigElements = etree.SubElement(sigPackage, "ELEMENTS")
for sig in locDataBase.getAllSigsList():
    signal = etree.SubElement(sigElements, "SYSTEM-SIGNAL")
    sigShortName = etree.SubElement(signal, "SHORT-NAME")
    sigShortName.text = sig.getSigName()
    physProps = etree.SubElement(signal, "PHYSICAL-PROPS")

ipduGrpPackage = etree.SubElement(packages, "AR-PACKAGE")
ipduShortName = etree.SubElement(ipduGrpPackage, "SHORT-NAME")
ipduShortName.text = "IPDUGroup"
ipduElements = etree.SubElement(ipduGrpPackage, "ELEMENTS")
ipduGroup = etree.SubElement(ipduElements, "I-SIGNAL-I-PDU-GROUP")
ipduGroupShortName.text = "VLAN_Connect_Rx"
commDir = etree.SubElement(ipduElements, "COMMUNICATION-DIRECTION")
commDir.text = "IN"
iSigsIPdus = etree.SubElement(ipduElements, "I-SIGNAL-I-PDUS")
for pdu in locDataBase.getAllRxPdus():
    ipdu = etree.SubElement(iSigsIPdus, "I-SIGNAL-I-PDU-REF-CONDITIONAL")
    ipduShortName = etree.SubElement(ipdu, "I-SIGNAL-I-PDU-REF")
    ipduShortName.text = "/PDU/" + pdu.getPduName()

ipduGroup = etree.SubElement(ipduElements, "I-SIGNAL-I-PDU-GROUP")
ipduGroupShortName.text = "VLAN_Connect_Tx"
commDir = etree.SubElement(ipduElements, "COMMUNICATION-DIRECTION")
commDir.text = "IN"
iSigsIPdus = etree.SubElement(ipduElements, "I-SIGNAL-I-PDUS")
for pdu in locDataBase.getAllTxPdus():
    ipdu = etree.SubElement(iSigsIPdus, "I-SIGNAL-I-PDU-REF-CONDITIONAL")
    ipduShortName = etree.SubElement(ipdu, "I-SIGNAL-I-PDU-REF")
    ipduShortName.text = "/PDU/" + pdu.getPduName()

clusterPackage = etree.SubElement(packages, "AR-PACKAGE")
clusterElements = etree.SubElement(clusterPackage, "ELEMENTS")
ethCluster = etree.SubElement(clusterElements, "ETHERNET-CLUSTER")
ethClusterMeb = etree.SubElement(ethCluster, "ETHERNET-CLUSTER")
ethClusterVars = etree.SubElement(ethClusterMeb, "ETHERNET-CLUSTER-VARIANTS")
ethClusterCond = etree.SubElement(ethClusterVars, "ETHERNET-CLUSTER-CONDITIONAL")
ethClusterCond = etree.SubElement(ethClusterCond, "MAC-MULTICAST-GROUPS")



ipduShortName.text = "IPDUGroup"

ipduGroup = etree.SubElement(ipduElements, "I-SIGNAL-I-PDU-GROUP")


text_file = open("Output.txt", "w")
text_file.write(etree.tostring(autosar).decode('utf-8'))
text_file.close()
