#ifndef CIF_SWIG_DEFS_H
#define CIF_SWIG_DEFS_H

#define CIF_HWTYPE_NONE                           0
#define CIF_HWTYPE_VIRTUAL                        1
#define CIF_HWTYPE_CANCARDX                       2
#define CIF_HWTYPE_CANAC2PCI                      6
#define CIF_HWTYPE_CANCARDY                      12
#define CIF_HWTYPE_CANCARDXL                     15
#define CIF_HWTYPE_CANCASEXL                     21
#define CIF_HWTYPE_CANCASEXL_LOG_OBSOLETE        23
#define CIF_HWTYPE_CANBOARDXL                    25
#define CIF_HWTYPE_CANBOARDXL_PXI                27
#define CIF_HWTYPE_VN2600                        29
#define CIF_HWTYPE_VN2610                        29
#define CIF_HWTYPE_VN3300                        37
#define CIF_HWTYPE_VN3600                        39
#define CIF_HWTYPE_VN7600                        41
#define CIF_HWTYPE_CANCARDXLE                    43
#define CIF_HWTYPE_VN8900                        45
#define CIF_HWTYPE_VN2640                        53
#define CIF_HWTYPE_VN1610                        55
#define CIF_HWTYPE_VN1630                        57
#define CIF_HWTYPE_VN1640                        59
#define CIF_HWTYPE_VN1611                        63
    
#define CIF_BITRATE_5K                         5000
#define CIF_BITRATE_10K                        10000
#define CIF_BITRATE_15K                        15000
#define CIF_BITRATE_20K                        20000
#define CIF_BITRATE_50K                        50000
#define CIF_BITRATE_100K                       100000
#define CIF_BITRATE_125K                       125000
#define CIF_BITRATE_250K                       250000
#define CIF_BITRATE_500K                       500000
#define CIF_BITRATE_800K                       800000
#define CIF_BITRATE_1M                         1000000
#define CIF_INVALID_refID  0x0BFFFFFF

#define CIF_ADDRESSING_NORMAL                   0
#define CIF_ADDRESSING_NORMAL_FIXED_PHYSICAL    1
#define CIF_ADDRESSING_NORMAL_FIXED_FUNCTIONAL  2
#define CIF_ADDRESSING_EXTENDED                 3
#define CIF_ADDRESSING_MIXED                    4
#define CIF_ADDRESSING_EXTENDED_ZF              5
#define CIF_TATYPE_PHYSICAL                     0
#define CIF_TATYPE_FUNCTIONAL                   1

#define CIF_CanMessageIdentifier_STANDARD  0
#define CIF_CanMessageIdentifier_EXTENDED  1

#define CIF_MSGSTATUS_UNINIT  0
#define CIF_MSGSTATUS_TX_ERROR  1
#define CIF_MSGSTATUS_PENDING  2
#define CIF_MSGSTATUS_SUPPRESSPOSRESPONSE  3
#define CIF_MSGSTATUS_TIMEOUT_P2  4
#define CIF_MSGSTATUS_NRC  5
#define CIF_MSGSTATUS_OK  6
#define CIF_MSGSTATUS_PARAMETER_ERROR  7

#define CIF_PCAN_CT_NONE    0
#define CIF_PCAN_CT_ISA     1
#define CIF_PCAN_CT_DNG     2
#define CIF_PCAN_CT_PCI     3
#define CIF_PCAN_CT_USB     4
#define CIF_PCAN_CT_PCCARD  5

#define CIF_HW_INDEX_CAN0  0
#define CIF_HW_INDEX_CAN1  1
#define CIF_HW_INDEX_CAN2  2
#define CIF_HW_INDEX_CAN3  3


#endif /* CIF_SWIG_DEFS_H */
