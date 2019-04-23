#ifndef __CONST_MENU_CHOSE_COMMUNICATION_PARAMETERS__
#define __CONST_MENU_CHOSE_COMMUNICATION_PARAMETERS__

#define EKRAN_CHOSE_COMMUNICATION_PARAMETERS      (EKRAN_TYPE_BUTTON_UVV + 1)
#define EKRAN_VIEW_NAME_OF_CELL                   (EKRAN_CHOSE_COMMUNICATION_PARAMETERS + 1)
#define EKRAN_CHOSE_SETTING_RS485                 (EKRAN_VIEW_NAME_OF_CELL + 1)
#define EKRAN_PHY_LAYER_RS485                     (EKRAN_CHOSE_SETTING_RS485 + 1)
#define EKRAN_PROTOCOL_RS485                      (EKRAN_PHY_LAYER_RS485 + 1)
#define EKRAN_VIEW_SPEED_RS485                    (EKRAN_PROTOCOL_RS485 + 1)
#define EKRAN_VIEW_PARE_RS485                     (EKRAN_VIEW_SPEED_RS485 + 1)
#define EKRAN_VIEW_STOP_BITS_RS485                (EKRAN_VIEW_PARE_RS485 + 1)
#define EKRAN_VIEW_TIMEOUT_RS485                  (EKRAN_VIEW_STOP_BITS_RS485 + 1)
#define EKRAN_PROTOCOLS_RS485                     (EKRAN_VIEW_TIMEOUT_RS485 + 1)
#define EKRAN_ADDRESS_RS485                       (EKRAN_PROTOCOLS_RS485 + 1)
#define EKRAN_CHOSE_SETTING_ETHERNET              (EKRAN_ADDRESS_RS485 + 1)
#define EKRAN_SETTING_NETWORK_LAYER_ETHERNET      (EKRAN_CHOSE_SETTING_ETHERNET + 1)

enum _index_ml_chcp
{
  INDEX_ML_CHCP_NAME = 0,
  INDEX_ML_CHCP_SETTING_RS485,

#if (MODYFIKACIA_VERSII_PZ == 4)  
  INDEX_ML_CHCP_SETTING_ETHERNET,
#endif  
  
  MAX_ROW_FOR_CHOSE_COMMUNICATION_PARAMETERS
};

#define MAX_ROW_FOR_CHOSE_SETTING_RS485              2
#define MAX_ROW_FOR_CHOSE_SETTING_ETHERNET           1
#define MAX_ROW_FOR_PHY_LAYER_RS485                  4
#define MAX_ROW_FOR_PROTOCOLS_RS485                  1
#define MAX_ROW_FOR_VIEW_NAME_OF_CELL                (MAX_CHAR_IN_NAME_OF_CELL >> POWER_MAX_COL_LCD)
#define MAX_ROW_FOR_ADDRESS                          1
#define MAX_ROW_FOR_VIEW_SPEED_INTERFACE             1
#define MAX_ROW_FOR_VIEW_PARE_INTERFACE              1
#define MAX_ROW_FOR_VIEW_STOP_BITS_INTERFACE         1
#define MAX_ROW_FOR_VIEW_TIMEOUT_INTERFACE           1
#define MAX_ROW_FOR_SETTING_NETWORK_LAYER_ETHERNET   3

#define INDEX_ML_PHY_LAYER_RS485         0
#define INDEX_ML_PROTOCOL_RS485          1

#define INDEX_ML_ADDRESS                 0

#define INDEX_ML_NETWORK_LAYER_ETHERNET  0

#define INDEX_ML_NL_IPV4                 0
#define INDEX_ML_NL_MASK                 1
#define INDEX_ML_NL_GATEWAY              2

#define COL_ADDRESS_BEGIN                6
#define COL_ADDRESS_END                  8

#define INDEX_ML_CHSRS485_SPEED              0
#define INDEX_ML_CHSRS485_PARE_BIT           1
#define INDEX_ML_CHSRS485_NUMBER_STOP_BIT    2
#define INDEX_ML_CHSRS485_TIMEOUT            3

#define COL_TIMEOUT_INTERFACE_BEGIN    3
#define COL_TIMEOUT_INTERFACE_COMMA    5
#define COL_TIMEOUT_INTERFACE_END      6

#define COL_IP4_GATEWAY_BEGIN           1
#define COL_IP4_GATEWAY_END             MAX_COL_LCD

#define COL_MASK_BEGIN                 7
#define COL_MASK_END                   8

#endif
