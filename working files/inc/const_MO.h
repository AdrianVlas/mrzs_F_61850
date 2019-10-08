#ifndef __CONST_MO__
#define __CONST_MO__

#define BROADCAST_ADDRESS_MO      ((uint8_t)-1)

#define BUFFER_CANAL1_MO          500
#define BUFFER_CANAL2_MO          500

#define START_BYTE_MO             0x5A
#define STOP_BYTE_MO              0xA5

enum _start_label
{
  START_LABEL_NEW_TM = 10,
  START_LABEL_CONF_DIAGN,
  START_LABEL_NEW_BLOCK
};

#define SIZE_SENDING_DATA_TM            \
(                                       \
  sizeof(measurement) +                 \
  sizeof(frequency) +                   \
  sizeof(resistance) +                  \
  (sizeof(P) >> 1) +                    \
  (sizeof(Q) >> 1) +                    \
  (sizeof(S) >> 1) +                    \
  (sizeof(cos_phi_x1000) >> 1) +        \
  (sizeof(energy) >> 1) +               \
  (sizeof(phi_angle) >> 1) +            \
  sizeof(resurs_vymykacha) +            \
  sizeof(resurs_vidkljuchennja) +       \
  sizeof(state_inputs) +                \
  sizeof(state_outputs) +               \
  sizeof(state_leds) +                  \
  sizeof(active_functions) +            \
  sizeof(diagnostyka) +                 \
  sizeof(Output_Out_LAN_block)          \
)

#define SIZE_RECEIVING_DATA_TM          \
(                                       \
  sizeof(Input_In_GOOSE_block) +        \
  sizeof(Input_In_MMS_block  )          \
)

#define SIZE_RECEIVING_ACT_DATA_TM      \
(                                       \
  sizeof(Input_ctrl_In_GOOSE_block) +   \
  sizeof(Input_ctrl_In_MMS_block)       \
)

#define SIZE_DESCRIPTION_DATA_TM      \
(                                       \
  20    \
)

enum __type_head_info_mo
{
  SENDIND_TM_INFO = 1,
  SENDING_GOOSE_INFO,
  SENDING_BASIC_SETTINGS,
  ANY_CONFIRMATION,
  MAKING_NEW_BLOCKS,
  ANSWER_FOR_MAKING_NEW_BLOCKS,
  BREAK_LAST_ACTION,
  CONFIRM_BREAKING_LAST_ACTION,
  REQUEST_SETTINGS_NETWORK_LAYER,
  SENDING_SETTINGS_NETWORK_LAYER,
  CONFIRM_RECEIVING_SETTINGS_NETWORK_LAYER,
  REQUEST_MODBUS_TCP_REQ,
  SENDING_MODBUS_TCP_REQ,
  CONFIRM_RECEIVING_MODBUS_TCP_REQ
};

enum _IEC_state_queue_mo
{
  IEC_STATE_QUEUE_MO_ASK_BASIC_SETTINGS = 0,
  IEC_STATE_QUEUE_MO_ASK_MAKING_MEMORY_BLOCK,
  IEC_STATE_QUEUE_MO_ASK_SENDING_SETTING_NETWORK_LAYER,

  IEC_STATE_QUEUE_MO_NEW_MODBUS_TCP_REQ,
  
  IEC_STATE_QUEUE_MO_TRANSACTION_PROGRESS = 31
};

enum _state_queue_mo
{
  STATE_QUEUE_MO_ASK_BASIC_SETTINGS = 0,
  STATE_QUEUE_MO_SEND_BASIC_SETTINGS,
  STATE_QUEUE_MO_TRANSMITING_BASIC_SETTINGS,
  STATE_QUEUE_MO_ASK_MAKING_MEMORY_BLOCK,
  STATE_QUEUE_MO_TRANSMITING_MAKING_MEMORY_BLOCK,
  STATE_QUEUE_MO_ASK_SENDING_SETTING_NETWORK_LAYER,
  STATE_QUEUE_MO_RECEIVING_SETTING_NETWORK_LAYER,
  
  STATE_QUEUE_MO_ASK_NEW_MODBUS_TCP_REQ,
  STATE_QUEUE_MO_RECEIVING_NEW_MODBUS_TCP_REQ,
  
  STATE_QUEUE_MO_BREAK_LAST_ACTION_IN_IEC = 30,
  STATE_QUEUE_MO_TRANSACTION_PROGRESS_IN_IEC = 31
};
#define QUEUQ_MO_IRQ   (                                                                  \
                         MASKA_FOR_BIT(STATE_QUEUE_MO_ASK_BASIC_SETTINGS)         |       \
                         MASKA_FOR_BIT(STATE_QUEUE_MO_ASK_MAKING_MEMORY_BLOCK)    |       \
                         MASKA_FOR_BIT(STATE_QUEUE_MO_TRANSACTION_PROGRESS_IN_IEC)|       \
                         MASKA_FOR_BIT(STATE_QUEUE_MO_ASK_SENDING_SETTING_NETWORK_LAYER) |\
                         MASKA_FOR_BIT(STATE_QUEUE_MO_ASK_NEW_MODBUS_TCP_REQ)             \
                       )    

enum _state_acs
{
  ACS_MADE = 0,
  ACS_NO_FREE_MEMORY,
  
  ACS_ALL_BLOCKS_CREATED,
  ACS_BLOCKS_CREATED_OF_LAST_REQUEST,
  ACS_NOT_ALL_BLOCKS_CREATED_OF_LAST_REQUEST,
  ACS_NO_FREE_MEMORY_FOR_BLOCKS,
  ACS_ID_PRESENT_AND_SIZE_SAME,
  ACS_ID_PRESENT_AND_SIZE_NOT_SAME,
  ACS_UNCONTINUE_CHANGING_ID,
  ACS_SOME_ID_INCREASE_ACS,
  ACS_ERROR_PACKET
};


#endif
