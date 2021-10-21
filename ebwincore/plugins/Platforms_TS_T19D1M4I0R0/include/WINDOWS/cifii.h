#ifndef CIFII_H
#define CIFII_H

void setModuleLogLevel(char* moduleName, unsigned int logLevel);
int getEBHwSessionHandle(int ebTalRefID);
int initFrVector(int hwType, char* fibexFile, int hwIdx, unsigned int startupMode);
int initFrEB(int hwRefId, char* fibexFile, unsigned int startupMode);
int initCanPeak(int hwType, int chID, int baudrate);
int initCanVector(int hwType, int chID, int baudrate, int hwIdx);
int initCanEB(int hwRefId, int chID, int baudrate);
int initCanPeak(int hwType, int chID, int baudrate);
int initCanVector(int hwType, int chID, int baudrate, int hwIdx);
int initCanEB(int hwRefId, int chID, int baudrate);
int initCanVectorFD( int hwType, 
                     int chId, 
                     int hwIndex,
                     int arbitrationBitRate,
                     int tseg1Abr,
                     int tseg2Abr,
                     int sjwAbr,
                     int dataBitRate,
                     int tseg1Dbr,
                     int tseg2Dbr,
                     int sjwDbr);
int initCanPeakFD( int arbitrationBitRate,
                   int tseg1Abr,
                   int tseg2Abr,
                   int sjwAbr,
                   int dataBitRate,
                   int tseg1Dbr,
                   int tseg2Dbr,
                   int sjwDbr,
                   int f_clock_mhz);
int initCanTp(int hwRefId,
      unsigned int rxCanId,
      unsigned int txCanIdPhys,
      unsigned int txCanIdFunc,
      unsigned int msgIdType,
      unsigned int taType,
      unsigned int ourAddr,
      unsigned int peerAddrPhys,
      unsigned int peerAddrFunc,
      unsigned int netAddrExt,
      unsigned int addressing,
      bool padding,
      unsigned int blocksize,
      unsigned int stmin);
int initFrAudiTp(
      int hwRefId,
      int targetAddressPhys,
      int targetAddressFunc,
      int sourceAddress,
      int txSlot,
      int rxSlot,
      int taType);
int initFrIsoTp(
      int hwRefId,
      int targetAddressPhys,
      int targetAddressFunc,
      int sourceAddress,
      int txSlot,
      int rxSlot,
      int taType);
int initFrBmwTp(int hwRefId, int targetAddress, int sourceAddress, int txSlot, int rxSlot);
std::vector<unsigned char>* udsTransmit(int tpRefID, std::vector<unsigned char>* inputVector);
void enableTp(int tpRefID);
void disableTp(int tpRefID);
int sendFrameTp(int tpRefID, std::vector<unsigned char>* inputVector);
std::vector<unsigned char>* receiveFrameTp(int tpRefID, unsigned int IN_timeout);
int addCanLoggerPort(int hwRefId);
int addCanPort(int hwRefId, int startId, int endId, int idtype);
int addFrPort(int IN_hwRefID, int IN_frChannel, int IN_slot, int IN_frCyclRep, int IN_frCyclOff);
bool startPort(int portRefId);
bool stopPort(int portRefId);
int removePort(int portRefId);
bool startFrMeasurement(int hwRefId);
bool stopFrMeasurement(int hwRefId);
unsigned int sendFrameCAN(unsigned int IN_hwRefID,
                           unsigned int IN_canId, 
                           std::vector<unsigned char> *IN_data,
                           bool IN_ext, 
                           bool IN_sync);
unsigned int sendFrameCANFD(unsigned int IN_hwRefID,
                           unsigned int IN_canId, 
                           std::vector<unsigned char> *IN_data,
                           bool IN_ext, 
                           bool IN_sync,
                           bool IN_FD,
                           bool IN_BRS);
unsigned int sendFrameFR( unsigned int IN_hwRefID,
                          unsigned int IN_frChannel,
                          unsigned int IN_slot,
                          unsigned int IN_txMode,
                          unsigned int IN_frCyclRep,
                          unsigned int IN_frCyclOff,
                          std::vector<unsigned char> *IN_data,
                          unsigned int IN_sync);
std::vector<unsigned char> * receiveFrameCAN(unsigned int portRefId,
                                                         unsigned int timeout,
                                                         unsigned int* OUT_CanId,
                                                         unsigned int* OUT_timeStampHi,
                                                         unsigned int* OUT_timeStampLo,
                                                         unsigned int* OUT_is_ext,
                                                         unsigned int* OUT_status,
                                                         unsigned int* OUT_is_tx);
std::vector<unsigned char> * receiveFrameFR( unsigned int IN_portRefId,
                                                   unsigned int IN_timeout,
                                                   unsigned int* OUT_SlotId,
                                                   unsigned int* OUT_timeStampHi,
                                                   unsigned int* OUT_timeStampLo,
                                                   unsigned int* OUT_frChannel,
                                                   unsigned int* OUT_status,
                                                   unsigned int* OUT_Flags);
bool deinitHw(int hwRefId);
int deinitTp(int tpRefId);
void setP2Timeout(int tpRefId, int t);
void setP2StarTimeout(int tpRefId, int t);
int getP2Timeout(int tpRefId);
int getP2StarTimeout(int tpRefId);

#endif