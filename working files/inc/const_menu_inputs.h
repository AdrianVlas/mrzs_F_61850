#ifndef __CONSTANTS_INPUTS__
#define __CONSTANTS_INPUTS__

#define EKRAN_LIST_INPUTS_FOR_RANGUVANNJA        (EKRAN_CONTROL_UP + 1)

#define EKRAN_RANGUVANNJA_INPUT_1                (EKRAN_TRANSFORMATOR_INFO_CONTROL + 1)
#define EKRAN_RANGUVANNJA_INPUT_2                (EKRAN_RANGUVANNJA_INPUT_1 + 1)
#define EKRAN_RANGUVANNJA_INPUT_3                (EKRAN_RANGUVANNJA_INPUT_2 + 1)
#define EKRAN_RANGUVANNJA_INPUT_4                (EKRAN_RANGUVANNJA_INPUT_3 + 1)
#define EKRAN_RANGUVANNJA_INPUT_5                (EKRAN_RANGUVANNJA_INPUT_4 + 1)
#define EKRAN_RANGUVANNJA_INPUT_6                (EKRAN_RANGUVANNJA_INPUT_5 + 1)
#define EKRAN_RANGUVANNJA_INPUT_7                (EKRAN_RANGUVANNJA_INPUT_6 + 1)
#define EKRAN_RANGUVANNJA_INPUT_8                (EKRAN_RANGUVANNJA_INPUT_7 + 1)

#if (                                \
     (MODYFIKACIA_VERSII_PZ == 0) || \
     (MODYFIKACIA_VERSII_PZ == 1) || \
     (MODYFIKACIA_VERSII_PZ == 3)    \
    )

#define EKRAN_RANGUVANNJA_INPUT_9                (EKRAN_RANGUVANNJA_INPUT_8 + 1)
#define EKRAN_RANGUVANNJA_INPUT_10               (EKRAN_RANGUVANNJA_INPUT_9 + 1)
#define EKRAN_RANGUVANNJA_INPUT_11               (EKRAN_RANGUVANNJA_INPUT_10 + 1)
#define EKRAN_RANGUVANNJA_INPUT_12               (EKRAN_RANGUVANNJA_INPUT_11 + 1)
#define EKRAN_RANGUVANNJA_INPUT_13               (EKRAN_RANGUVANNJA_INPUT_12 + 1)
#define EKRAN_RANGUVANNJA_INPUT_14               (EKRAN_RANGUVANNJA_INPUT_13 + 1)
#define EKRAN_RANGUVANNJA_INPUT_15               (EKRAN_RANGUVANNJA_INPUT_14 + 1)
#define EKRAN_RANGUVANNJA_INPUT_16               (EKRAN_RANGUVANNJA_INPUT_15 + 1)

#if (MODYFIKACIA_VERSII_PZ == 0)

#define EKRAN_RANGUVANNJA_INPUT_17               (EKRAN_RANGUVANNJA_INPUT_16 + 1)
#define EKRAN_RANGUVANNJA_INPUT_18               (EKRAN_RANGUVANNJA_INPUT_17 + 1)
#define EKRAN_RANGUVANNJA_INPUT_19               (EKRAN_RANGUVANNJA_INPUT_18 + 1)
#define EKRAN_RANGUVANNJA_INPUT_20               (EKRAN_RANGUVANNJA_INPUT_19 + 1)

#endif

#endif

#if (MODYFIKACIA_VERSII_PZ == 0)

#define EKRAN_RANGUVANNJA_INPUT_LAST             EKRAN_RANGUVANNJA_INPUT_20

#elif (                                 \
       (MODYFIKACIA_VERSII_PZ == 1) ||  \
       (MODYFIKACIA_VERSII_PZ == 3)     \
      )   

#define EKRAN_RANGUVANNJA_INPUT_LAST             EKRAN_RANGUVANNJA_INPUT_16

#else

#define EKRAN_RANGUVANNJA_INPUT_LAST             EKRAN_RANGUVANNJA_INPUT_8

#endif

#define MAX_ROW_LIST_INPUTS_FOR_RANGUVANNJA       NUMBER_INPUTS
#define MAX_ROW_RANGUVANNJA_INPUT                 NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL

#define NAME_RANG_SMALL_RU         \
  "  ����.���.��   ",                   \
  " ����� ���������",                   \
  "   ����� ����   ",                   \
  " �����./�������.",                   \
  "  ��������� ��  ",                   \
  "����.�� ��.�����",                   \
  "     ���.��     ",                   \
  " �������� ���.  ",                   \
  "    ����.��     ",                   \
  " �������� ����. ",                   \
  " 1-� ��.������� ",                   \
  " 2-� ��.������� ",                   \
  " 3-� ��.������� ",                   \
  " 4-� ��.������� ",                   \
  " �.���.���.� �� ",                   \
  "   ����.���1    ",                   \
  "   ����.���2    ",                   \
  " ����.���.���2  ",                   \
  "   ����.���3    ",                   \
  "   ����.���4    ",                   \
  "����.��� 0.4�� 1",                   \
  "����.��� 0.4�� 2",                   \
  "��.�.��� 0.4�� 2",                   \
  "    ����.���    ",                   \
  " ���� ��� �� �� ",                   \
  "    ����.���    ",                   \
  "   ����.����1   ",                   \
  "   ����.����2   ",                   \
  "   ����.����3   ",                   \
  " ����.����.���  ",                   \
  " ���/���� �� �� ",                   \
  "   ����.���1    ",                   \
  "   ����.���2    ",                   \
  " ���� ���� �� ��",                   \
  " ����.���(���)  ",                   \
  "  ����.�����1   ",                   \
  "  ���� �����1   ",                   \
  "  ����.�����2   ",                   \
  "  ���� �����2   ",                   \
  "  ����.������1  ",                   \
  "  ����.������2  ",                   \
  "    ����.��x    ",                   \
  " ��.�-�������1  ",                   \
  " ��.�-�������2  ",                   \
  " ��.�-�������3  ",                   \
  " ��.�-�������4  ",                   \
  " ��.�-�������5  ",                   \
  " ��.�-�������6  ",                   \
  " ��.�-�������7  ",                   \
  " ��.�-�������8  ",                   \
  " ���.�-��������1",                   \
  " ���.�-��������1",                   \
  " ���.�-��������2",                   \
  " ���.�-��������2",                   \
  " ���.�-��������3",                   \
  " ���.�-��������3",                   \
  " ���.�-��������4",                   \
  " ���.�-��������4"

#define NAME_RANG_SMALL_UA         \
  " ����.�����.�� ",                   \
  " ����.��������� ",                   \
  "   ����.����    ",                   \
  " ̳��./�������. ",                   \
  "    ���� ��     ",                   \
  "����.�� ����.�.",                   \
  "    ����.��    ",                   \
  " �������� ����.",                   \
  "    ����.��     ",                   \
  " �������� ����. ",                   \
  " 1-� ��.������� ",                   \
  " 2-� ��.������� ",                   \
  " 3-� ��.������� ",                   \
  " 4-� ��.������� ",                   \
  " �.���.���.�� ��",                   \
  "   ����.���1    ",                   \
  "   ����.���2    ",                   \
  " ����.�����.���2",                   \
  "   ����.���3    ",                   \
  "   ����.���4    ",                   \
  "����.��� 0.4�� 1",                   \
  "����.��� 0.4�� 2",                   \
  "��.�.��� 0.4�� 2",                   \
  "    ����.���    ",                   \
  " ���� ��� �� ��",                   \
  "    ����.���    ",                   \
  "   ����.����1   ",                   \
  "   ����.����2   ",                   \
  "   ����.����3   ",                   \
  " ����.����.���  ",                   \
  " ���/���� �� ��",                   \
  "   ����.���1    ",                   \
  "   ����.���2    ",                   \
  "���� ���� �� ��",                   \
  " ����.���(���)  ",                   \
  "  ����.�����1   ",                   \
  "  ���� �����1   ",                   \
  "  ����.�����2   ",                   \
  "  ���� �����2   ",                   \
  "  ����.������1  ",                   \
  "  ����.������2  ",                   \
  "    ����.��x    ",                   \
  " ��.�-�������1  ",                   \
  " ��.�-�������2  ",                   \
  " ��.�-�������3  ",                   \
  " ��.�-�������4  ",                   \
  " ��.�-�������5  ",                   \
  " ��.�-�������6  ",                   \
  " ��.�-�������7  ",                   \
  " ��.�-�������8  ",                   \
  " ���.�-������1 ",                   \
  " ��.�-������1  ",                   \
  " ���.�-������2 ",                   \
  " ��.�-������2  ",                   \
  " ���.�-������3 ",                   \
  " ��.�-������3  ",                   \
  " ���.�-������4 ",                   \
  " ��.�-������4  "

#define NAME_RANG_SMALL_EN         \
  " ON CB Blocking ",                   \
  "Indication Reset",                   \
  "  Relay Reset   ",                   \
  "  Local/Remote  ",                   \
  "  State of CB   ",                   \
  "����.�� ��.�����",                   \
  "     On CB      ",                   \
  " On CB Control  ",                   \
  "     Off CB     ",                   \
  " Off CB Control ",                   \
  " Pick-up Set 1  ",                   \
  " Pick-up Set 2  ",                   \
  " Pick-up Set 3  ",                   \
  " Pick-up Set 4  ",                   \
  " �.���.���.� �� ",                   \
  "  Blc.of OCP1   ",                   \
  "  Blc.of OCP2   ",                   \
  "  OCP2 Acc.Blc. ",                   \
  "  Blc.of OCP3   ",                   \
  "  Blc.of OCP4   ",                   \
  "OCP 0.4kV 1 Blc.",                   \
  "OCP 0.4kV 2 Blc.",                   \
  "OCP 0.4kV 2 Ac.B",                   \
  "    ����.���    ",                   \
  " ���� ��� �� �� ",                   \
  "    ����.���    ",                   \
  "   ����.����1   ",                   \
  "   ����.����2   ",                   \
  "   ����.����3   ",                   \
  " ����.����.���  ",                   \
  " ���/���� �� �� ",                   \
  "   ����.���1    ",                   \
  "   ����.���2    ",                   \
  " CBFP Start f.DI",                   \
  "  Blc.of NPSP   ",                   \
  "   ����.Umin1   ",                   \
  "   ���� Umin1   ",                   \
  "   ����.Umin2   ",                   \
  "   ���� Umin2   ",                   \
  "   ����.Umax1   ",                   \
  "   ����.Umax2   ",                   \
  "   Blc.of UPx   ",                   \
  "    UDF1 In     ",                   \
  "    UDF2 In     ",                   \
  "    UDF3 In     ",                   \
  "    UDF4 In     ",                   \
  "    UDF5 In     ",                   \
  "    UDF6 In     ",                   \
  "    UDF7 In     ",                   \
  "    UDF8 In     ",                   \
  "UD Flip-Flop1 S.",                   \
  "UD Flip-Flop1 R.",                   \
  "UD Flip-Flop2 S.",                   \
  "UD Flip-Flop2 R.",                   \
  "UD Flip-Flop3 S.",                   \
  "UD Flip-Flop3 R.",                   \
  "UD Flip-Flop4 S.",                   \
  "UD Flip-Flop4 R."
        
#define NAME_RANG_SMALL_KZ         \
  "  ����.���.��   ",                   \
  " ����� ���������",                   \
  "   ����� ����   ",                   \
  " �����./�������.",                   \
  "  ��������� ��  ",                   \
  "����.�� ��.�����",                   \
  "     ���.��     ",                   \
  " �������� ���.  ",                   \
  "    ����.��     ",                   \
  " �������� ����. ",                   \
  " 1-� ��.������� ",                   \
  " 2-� ��.������� ",                   \
  " 3-� ��.������� ",                   \
  " 4-� ��.������� ",                   \
  " �.���.���.� �� ",                   \
  "   ����.���1    ",                   \
  "   ����.���2    ",                   \
  " ����.���.���2  ",                   \
  "   ����.���3    ",                   \
  "   ����.���4    ",                   \
  "����.��� 0.4�� 1",                   \
  "����.��� 0.4�� 2",                   \
  "��.�.��� 0.4�� 2",                   \
  "    ����.���    ",                   \
  " ���� ��� �� �� ",                   \
  "    ����.���    ",                   \
  "   ����.����1   ",                   \
  "   ����.����2   ",                   \
  "   ����.����3   ",                   \
  " ����.����.���  ",                   \
  " ���/���� �� �� ",                   \
  "   ����.���1    ",                   \
  "   ����.���2    ",                   \
  " ���� ���� �� ��",                   \
  " ����.���(���)  ",                   \
  "  ����.�����1   ",                   \
  "  ���� �����1   ",                   \
  "  ����.�����2   ",                   \
  "  ���� �����2   ",                   \
  "  ����.������1  ",                   \
  "  ����.������2  ",                   \
  "    ����.��x    ",                   \
  " ��.�-�������1  ",                   \
  " ��.�-�������2  ",                   \
  " ��.�-�������3  ",                   \
  " ��.�-�������4  ",                   \
  " ��.�-�������5  ",                   \
  " ��.�-�������6  ",                   \
  " ��.�-�������7  ",                   \
  " ��.�-�������8  ",                   \
  " ���.�-��������1",                   \
  " ���.�-��������1",                   \
  " ���.�-��������2",                   \
  " ���.�-��������2",                   \
  " ���.�-��������3",                   \
  " ���.�-��������3",                   \
  " ���.�-��������4",                   \
  " ���.�-��������4"
        
#endif 
