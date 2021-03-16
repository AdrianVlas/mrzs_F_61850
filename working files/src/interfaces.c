#include "header.h"

/***********************************************************************************/
//�������� ����������� ���������� USB � ����������
/***********************************************************************************/
void Usb_routines_irq(void)
{
  static int32_t time_local;
  ++time_local;
  static int32_t time_receive;
  
  /***
  ������ � USB
  ***/
  int ptr_out = from_USB_ptr_out_irq;
  if ((ptr_out != from_USB_ptr_in_irq) && (count_out_previous >= 0))
  {
    int diff = ptr_out - from_USB_ptr_in_irq;
    if (diff < 0) diff += BUFFER_USB_IN;
    count_out += diff;
    from_USB_ptr_in_irq = ptr_out;
    if (count_out > BUFFER_USB)
    {
      count_out = 2*BUFFER_USB;//�� ������, �� �� ��� ����� �� ����� ���������
      from_USB_ptr_in = ptr_out;
    }
    
    time_receive = time_local;
  }
  /***/

  /***
  ��������� � USB
  ***/
  if ((USB_Tx_State != 1) && (data_usb_transmiting_irq == true))
  { 
    uint32_t usb_transmiting_count_tmp = 0;
    static u8 buffer_USB_tmp[BUFFER_USB];  
  
    while (to_USB_ptr_in_irq != to_USB_ptr_out)
    {
      buffer_USB_tmp[usb_transmiting_count_tmp++] = buffer_USB_out[to_USB_ptr_in_irq++];
    
      if (to_USB_ptr_in_irq >= BUFFER_USB_OUT)
      {
        if (to_USB_ptr_in_irq == BUFFER_USB_OUT) to_USB_ptr_in_irq = 0;
        else total_error_sw_fixed(105);
      }
    
      if (usb_transmiting_count_tmp >= BUFFER_USB)
      {
        if (usb_transmiting_count_tmp == BUFFER_USB) break;
        else total_error_sw_fixed(106);
      }
    }
    if (to_USB_ptr_in_irq == to_USB_ptr_out)  data_usb_transmiting_irq = false;
  
    if (usb_transmiting_count_tmp != 0)
    {
      if (from_USB_ptr_in_irq == from_USB_ptr_out_irq)
      {
        int32_t delta = time_local - time_receive;
        if (delta < 0) delta += (1u << 31);
        if (delta < MAX_TIMEOUT_PACKET_USB)
        {
          APP_FOPS.pIf_DataTx(buffer_USB_tmp, usb_transmiting_count_tmp);
        }
      }
    }
  }
  /***/
}  
/***********************************************************************************/

/***********************************************************************************/
//�������� ����������� ���������� USB � ���������� ����� ������
/***********************************************************************************/
void Usb_routines(void)
{
  /***
  ������ � USB
  ***/
  if (count_out != 0)
  {
    //Գ����� �������� ������� TIM4 � ���� ����� ���������� 10 ���
    uint16_t current_count_tim4_USB = ((uint16_t)TIM4->CNT);
    uint16_t delta_USB = 0;//�� ������������ 0, ��� ���� ������ ���� ������ ����� �� ������������ ������, ���� � ��� ��������, �� ���� ���� 0
    
    //���������� �� ���������� ������� ��� ��� - � ���� ���, �� ������� �������� ������� TIM4
    if (count_out_previous != count_out)
    {
      previous_count_tim4_USB = current_count_tim4_USB;
      count_out_previous = count_out;
    }
    else
    {
      //ϳ��� ��������� ���� ���� �� ���������� �� ������� ���� - ���� ������ ���������� �� ���� ��������
      
      //��������� ������ ���� �� ����������� ��� ��� �� USB
      if (current_count_tim4_USB >= previous_count_tim4_USB) delta_USB = current_count_tim4_USB - previous_count_tim4_USB;
      else delta_USB = (0x10000 - previous_count_tim4_USB) + current_count_tim4_USB; //0x10000 - �� ������ ����� �������, �� �� �������� ���� ���������� �������� [0; 65535]
    }
    
    if (
        (delta_USB > 188) /*1 - ������� 10 ���, �� TIM4 ��������� � ����������� 10 ���. 188 �������� ��� 1880 ���. 1880 �� �� ��� 1,5 ������� �� �������� 9600 � ������ 1-start + 8-data + pare + 2-stop*/
        &&
        (data_usb_transmiting_irq == false)  
       )   
    {
      //������� ������� �������� ����������
      usb_received_count = count_out;
      count_out_previous = -1; //�� ��'���� ����� ���� �����������, �� ����� count_out ���� ���������
      count_out = 0;
      count_out_previous = 0; 
      
      //������� ������� ��� � ������� �����
      if (usb_received_count <= BUFFER_USB)
      {
        for (int i = 0; i < usb_received_count; i++) 
        {
          usb_received[i] = buffer_USB_in[from_USB_ptr_in++];
          if (from_USB_ptr_in >= BUFFER_USB_IN)
          {
            if (from_USB_ptr_in == BUFFER_USB_IN) from_USB_ptr_in = 0;
            else total_error_sw_fixed(101);
          }
        }
        //���������� ����� �� ��������� MODBUS-RTU ��������� � USB
        inputPacketParserUSB();
      }
    }
  }
  /***/
  
  /***
  �������� � USB
  ***/
  if (data_usb_transmiting == true)
  {
    for (int i = 0; i < usb_transmiting_count; ++i)
    {
      buffer_USB_out[to_USB_ptr_out++] = usb_transmiting[i];
      if (to_USB_ptr_out >= BUFFER_USB_OUT)
      {
        if (to_USB_ptr_out == BUFFER_USB_OUT) to_USB_ptr_out = 0;
        else total_error_sw_fixed(104);
      }
    }

    usb_transmiting_count = 0;
    data_usb_transmiting = false;
    data_usb_transmiting_irq = true;
  }
  /***/
}  
/***********************************************************************************/

/***********************************************************************************/
//���������/������������� USART ��� RS-485
/***********************************************************************************/
void USART_RS485_Configure(void)
{
  USART_InitTypeDef USART_InitStructure;

  unsigned int speed_frame_RS485;
  switch (current_settings.speed_RS485)
  {
  case 0:
    {
      speed_frame_RS485 = 9600;
      break;
    }
  case 1:
    {
      speed_frame_RS485 = 14400;
      break;
    }
  case 2:
    {
      speed_frame_RS485 = 19200;
      break;
    }
  case 3:
    {
      speed_frame_RS485 = 28800;
      break;
    }
  case 4:
    {
      speed_frame_RS485 = 38400;
      break;
    }
  case 5:
    {
      speed_frame_RS485 = 57600;
      break;
    }
  default:
    {
      speed_frame_RS485 = 115200;
      break;
    }
  }
  
  unsigned int pare_RS485;
  switch (current_settings.pare_bit_RS485)
  {
  case 0:
    {
      pare_RS485 = USART_Parity_No;
      break;
    }
  case 1:
    {
      pare_RS485 = USART_Parity_Odd;
      break;
    }
  default:
    {
      pare_RS485 = USART_Parity_Even;
      break;
    }
  }

  //���������� ��������� �� ������
  GPIO_485DE->BSRRH = GPIO_PIN_485DE;

  //����������� ��������� ����������
  USART_ITConfig(USART_RS485, USART_IT_TC, DISABLE);
  USART_ITConfig(USART_RS485, USART_IT_IDLE, DISABLE);
  USART_ITConfig(USART_RS485, USART_IT_LBD, DISABLE);
  USART_ITConfig(USART_RS485, USART_IT_PE, DISABLE);
  USART_ITConfig(USART_RS485, USART_IT_ERR, DISABLE);

  //��������� ���� DMA_Tx ���� �� ���������
  if ((DMA_StreamRS485_Tx->CR & (uint32_t)DMA_SxCR_EN) !=0) DMA_StreamRS485_Tx->CR &= ~(uint32_t)DMA_SxCR_EN;
  DMA_StreamRS485_Tx->NDTR = 0;
  //����������� �������� ����� DMA, ���� �� ����������
  if ((USART_RS485->CR3 & USART_DMAReq_Tx) != 0) USART_RS485->CR3 &= (uint16_t)((~(uint32_t)USART_DMAReq_Tx) & 0xffff);
  // ������� ���������, �� �������� ��� ���������� �������� ����� ��� DMA1 �� ������ TX 
  DMA_ClearFlag(DMA_StreamRS485_Tx, DMA_FLAG_TCRS485_Tx | DMA_FLAG_HTRS485_Tx | DMA_FLAG_TEIRS485_Tx | DMA_FLAG_DMEIRS485_Tx | DMA_FLAG_FEIRS485_Tx);

  //��������� ���� DMA_Rx ���� �� ���������
  if ((DMA_StreamRS485_Rx->CR & (uint32_t)DMA_SxCR_EN) !=0) DMA_StreamRS485_Rx->CR &= ~(uint32_t)DMA_SxCR_EN;
  DMA_StreamRS485_Rx->NDTR = BUFFER_RS485;
  //����������� �������� ����� DMA, ���� �� ����������
  if ((USART_RS485->CR3 & USART_DMAReq_Rx) != 0) USART_RS485->CR3 &= (uint16_t)((~(uint32_t)USART_DMAReq_Rx) & 0xffff);
  // ������� ���������, �� �������� ��� ���������� �������� ����� ��� DMA1 �� ������ RX 
  DMA_ClearFlag(DMA_StreamRS485_Rx, DMA_FLAG_TCRS485_Rx | DMA_FLAG_HTRS485_Rx | DMA_FLAG_TEIRS485_Rx | DMA_FLAG_DMEIRS485_Rx | DMA_FLAG_FEIRS485_Rx);
  
  //������� ������� ��������� ����
  RxBuffer_RS485_count = 0;

  //���������� USART
  USART_RS485->CR1 &= (uint16_t)(~USART_CR1_UE);

  //���������� ���������
  USART_InitStructure.USART_BaudRate = speed_frame_RS485;
  if (pare_RS485 == USART_Parity_No) USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  else USART_InitStructure.USART_WordLength = USART_WordLength_9b;
  if (current_settings.number_stop_bit_RS485 == 0) USART_InitStructure.USART_StopBits = USART_StopBits_1;
  else USART_InitStructure.USART_StopBits = USART_StopBits_2; 
  USART_InitStructure.USART_Parity = pare_RS485;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  //�������� ������������ USART_RS485
  USART_Init(USART_RS485, &USART_InitStructure);

  //���������� �������� ����� DMA
  USART_RS485->CR3 |= USART_DMAReq_Rx;
  USART_RS485->CR3 |= USART_DMAReq_Tx;

  //���������� ����������� �� USART ��� RS-485 ��� �������� �������
  USART_ITConfig(USART_RS485, USART_IT_IDLE, ENABLE);
  USART_ITConfig(USART_RS485, USART_IT_LBD, ENABLE);
  USART_ITConfig(USART_RS485, USART_IT_PE, ENABLE);
  USART_ITConfig(USART_RS485, USART_IT_ERR, ENABLE);

  //���������� USART_RS485
  USART_RS485->CR1 |= USART_CR1_UE;
}
/***********************************************************************************/

/***********************************************************************************/
//³��������� ���������� ������ RS-485
/***********************************************************************************/
void restart_monitoring_RS485(void)
{
  //������� �� ������ �������
  USART_RS485->SR;
  (unsigned short int)(USART_RS485->DR & (uint16_t)0x01FF);
      
  if ((DMA_StreamRS485_Rx->CR & (uint32_t)DMA_SxCR_EN) !=0) DMA_StreamRS485_Rx->CR &= ~(uint32_t)DMA_SxCR_EN;

  // ������� ���������, �� �������� ��� ���������� �������� ����� ��� DMA1 �� ������ RS-485_RX 
  DMA_ClearFlag(DMA_StreamRS485_Rx, DMA_FLAG_TCRS485_Rx | DMA_FLAG_HTRS485_Rx | DMA_FLAG_TEIRS485_Rx | DMA_FLAG_DMEIRS485_Rx | DMA_FLAG_FEIRS485_Rx);

  DMA_StreamRS485_Rx->NDTR = BUFFER_RS485;
  RxBuffer_RS485_count_previous = 0;
  RxBuffer_RS485_count = 0;
  //��������� ���������� �������
  DMA_StreamRS485_Rx->CR |= (uint32_t)DMA_SxCR_EN;
}
/***********************************************************************************/

/***********************************************************************************/
//
/***********************************************************************************/
/***********************************************************************************/
