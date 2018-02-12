#include "header.h"

/***********************************************************************************/
//������ ������� ��� �������� ����� ����� ��������� RS-485
/***********************************************************************************/
void start_transmint_data_via_RS_485(unsigned int count)
{
  //����������, �� ����� ���������� ���
  unsigned int current_time_tim4 = TIM4->CNT;
  unsigned int delta_time;

  //���������� ���, �������� ����� ��������������� �������
  if (
      (current_time_tim4 > time_last_receive_byte) ||
      (
       (current_time_tim4 == time_last_receive_byte) &&
       (mark_current_tick_RS_485 != 0)  
      ) 
     )   
    delta_time = current_time_tim4 - time_last_receive_byte;
  else 
    delta_time = current_time_tim4 + 0xffff - time_last_receive_byte;

  if (max_reaction_time_rs_485 < delta_time) max_reaction_time_rs_485 = delta_time;
  
  if (delta_time < MAX_TIMEOUT_PACKET)
  {
    //����� ��������� �� �����

    //���������� ��������� �� ��������
    GPIO_485DE->BSRRL = GPIO_PIN_485DE;

    //������� ��, �� ��������, �� �������� ���� ���������
    USART_ClearFlag(USART_RS485, USART_FLAG_TC);

    //���������� ��������� �����������, ����  ���� �������� �� ����� �� DMA1 ����� ����� DMA1_ChannelRS485_TX
    DMA_StreamRS485_Tx->CR |= DMA_IT_TC;

    //��������� ���� DMA ���� �� ���������
    if ((DMA_StreamRS485_Tx->CR & (uint32_t)DMA_SxCR_EN) !=0) DMA_StreamRS485_Tx->CR &= ~(uint32_t)DMA_SxCR_EN;
    DMA_StreamRS485_Tx->NDTR = count;
    //���������� �������� ����� DMA
    if ((USART_RS485->CR3 & USART_DMAReq_Tx) == 0) USART_RS485->CR3 |= USART_DMAReq_Tx;

    //������� ���������, �� �������� ��� ���������� �������� ����� ��� DMA1 �� ������ RS-485_TX
    DMA_ClearFlag(DMA_StreamRS485_Tx, DMA_FLAG_TCRS485_Tx | DMA_FLAG_HTRS485_Tx | DMA_FLAG_TEIRS485_Tx | DMA_FLAG_DMEIRS485_Tx | DMA_FLAG_FEIRS485_Tx);
    //��������� ��������
    DMA_StreamRS485_Tx->CR |= (uint32_t)DMA_SxCR_EN;
  }
  else
  {
    //�� ����� ��������� �� �����, �� ������� ��������� ������ ��� �� ��������� ��� ���������� ������ - ����� ���������� �� ������ ����������� �� RS-485
    restart_monitoring_RS485();
  }
}
/***********************************************************************************/
