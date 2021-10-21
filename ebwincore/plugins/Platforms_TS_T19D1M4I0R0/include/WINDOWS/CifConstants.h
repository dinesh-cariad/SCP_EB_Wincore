#ifndef CIFCONSTANTS_H
#define CIFCONSTANTS_H


/*==================[inclusions]============================================*/

/*==================[type definitions]======================================*/


typedef enum _cif_message_status
{
   CIF_MSGSTATUSUNINIT = 0,
   CIF_MSGSTATUSTX_ERROR = 1,
   CIF_MSGSTATUSPENDING = 2,
   CIF_MSGSTATUSSUPPRESSPOSRESPONSE = 3,
   CIF_MSGSTATUSTIMEOUT_P2 = 4,
   CIF_MSGSTATUSNRC = 5,
   CIF_MSGSTATUSOK = 6,
   CIF_MSGSTATUSPARAMETERERROR = 7
} cif_message_status_t;


typedef enum _cif_canframe_identifier
{
   CIF_CANIDENTIFIER_STANDARD = 0,
   CIF_CANIDENTIFIER_EXTENDED = 1
} cif_canframe_identifier_t;


typedef enum _cif_can_bitrates
{
   CIF_CAN_BITRATE_5K   = 5000,
   CIF_CAN_BITRATE_10K  = 10000,
   CIF_CAN_BITRATE_15K  = 15000,
   CIF_CAN_BITRATE_20K  = 20000,
   CIF_CAN_BITRATE_50K  = 50000,
   CIF_CAN_BITRATE_100K = 100000,
   CIF_CAN_BITRATE_125K = 125000,
   CIF_CAN_BITRATE_250K = 250000,
   CIF_CAN_BITRATE_500K = 500000,
   CIF_CAN_BITRATE_800K = 800000,
   CIF_CAN_BITRATE_1M   = 1000000
} cif_can_bitrates_t;


typedef enum _cif_vector_hw_types
{
   CIF_VECTOR_HW_NONE = 0,
   CIF_VECTOR_HW_VIRTUAL = 1,
   CIF_VECTOR_HW_CANCARDX = 2,
   CIF_VECTOR_HW_CANAC2PCI = 6,
   CIF_VECTOR_HW_CANCARDY = 12,
   CIF_VECTOR_HW_CANCARDXL = 15,
   CIF_VECTOR_HW_CANCASEXL = 21,
   CIF_VECTOR_HW_CANBOARDXL = 25,
   CIF_VECTOR_HW_CANBOARDXL_PXI = 27,
   CIF_VECTOR_HW_VN2600 = 29,
   CIF_VECTOR_HW_VN2610 = 29,
   CIF_VECTOR_HW_VN3300 = 37,
   CIF_VECTOR_HW_VN3600 = 39,
   CIF_VECTOR_HW_VN7600 = 41,
   CIF_VECTOR_HW_CANCARDXLE = 43,
   CIF_VECTOR_HW_VN8900 = 45,
   CIF_VECTOR_HW_VN2640 = 53,
   CIF_VECTOR_HW_VN1610 = 55,
   CIF_VECTOR_HW_VN1630 = 57,
   CIF_VECTOR_HW_VN1640 = 59,
   CIF_VECTOR_HW_VN1611 = 63
} cif_vector_hw_types_t;


typedef enum _cif_peak_hw_types
{
   CIF_PCAN_HW_NONE   = 0,
   CIF_PCAN_HW_ISA    = 1,
   CIF_PCAN_HW_DNG    = 2,
   CIF_PCAN_HW_PCI    = 3,
   CIF_PCAN_HW_USB    = 4,
   CIF_PCAN_HW_PCCARD = 5
} cif_peak_hw_types_t;


typedef enum _cif_eb_hw_types
{
   CIF_EB_HW_UNKNOWN = 0,
   CIF_EB_HW_EB2100 =  1,
   CIF_EB_HW_EB5100 =  2,
   CIF_EB_HW_EB6100 =  3,
   CIF_EB_HW_EBX200 =  4,
   CIF_EB_HW_EB2200 =  5,
   CIF_EB_HW_EB5200 =  6,
   CIF_EB_HW_EB6200 =  7
} cif_eb_hw_types_t;


typedef enum _cif_eb_hw_connection_types
{
   CIF_EB_HW_CONNECTION_USB = 0,
   CIF_EB_HW_CONNECTION_PCI = 3,
   CIF_EB_HW_CONNECTION_TCP = 4,
   CIF_EB_HW_CONNECTION_UNKNOWN = 6
} cif_eb_hw_connection_types_t;


typedef enum _cif_tp_ta_types
{
   CIF_TP_TATYPE_TATYPE_PHYSICAL = 0,
   CIF_TP_TATYPE_TATYPE_FUNCTIONAL = 1
} cif_tp_ta_types_t;


typedef enum _cif_tp_addressing_formats
{
   CIF_TP_ADDRESSING_FORMAT_NORMAL = 0,
   CIF_TP_ADDRESSING_FORMAT_NORMAL_FIXED = 1,
   CIF_TP_ADDRESSING_FORMAT_EXTENDED = 2,
   CIF_TP_ADDRESSING_FORMAT_MIXED = 3
} cif_tp_addressing_formats;


typedef enum _cif_flexray_channels
{
   CIF_FR_CHANNEL_A = 1,
   CIF_FR_CHANNEL_B = 2,
   CIF_FR_CHANNEL_AB = 3
} cif_flexray_channels_t;


typedef enum _cif_flexray_tx_modes
{
   CIF_FR_TX_MODE_SINGLESHOT = 0,
   CIF_FR_TX_MODE_CYCLIC = 1
} cif_flexray_tx_modes_t;


typedef enum _cif_flexray_startup_modes
{
   CIF_FR_STARTUP_MODE_NO_COLDSTART = 0,
   CIF_FR_STARTUP_MODE_COLDSTART_LEADING = 1,
   CIF_FR_STARTUP_MODE_COLDSTART_FOLLOWING = 2
} cif_flexray_startup_modes_t;

#define INVALID_REFID   0x0BFFFFFFU
typedef enum _cif_common_constants
{
   CIF_INVALID_REFID = INVALID_REFID
} cif_common_constants_t;

typedef enum 
{
  PCAN_FD_20_MHZ = 20,
  PCAN_FD_24_MHZ = 24,
  PCAN_FD_30_MHZ = 30,
  PCAN_FD_40_MHZ = 40,
  PCAN_FD_60_MHZ = 60,
  PCAN_FD_80_MHZ = 80
} cif_pcanFd_Clocks_t;


typedef enum CardType
{
    PCAN_CT_NONE   = 0,
    PCAN_CT_ISA    = 1,
    PCAN_CT_DNG    = 2,
    PCAN_CT_PCI    = 3,
    PCAN_CT_USB    = 4,
    PCAN_CT_PCCARD = 5
} cif_peak_CardType_t;

/*==================[class declarations]====================================*/


#endif /* CIFCONSTANTS_H */
