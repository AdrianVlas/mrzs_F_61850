#ifndef __CONST_MODBUS_MEMORY_MAP__
#define __CONST_MODBUS_MEMORY_MAP__

#define SET_DATA_INTO_EDIT_TABLE          0
#define SET_DATA_IMMEDITATE               1

/*****************************************************/
/* Адресний простір тільки для читання (Блок 1) */
/*****************************************************/
#define M_ADDRESS_FIRST_RO_BLOCK_1        0

#define MA_SERIAL_NUMBER                  0
#define MA_NAME_CHAR_1_2                  1
#define MA_NAME_CHAR_3_4                  2
#define MA_NAME_CHAR_5_6                  3
#define MA_NAME_CHAR_7_8                  4
#define MA_NAME_CHAR_9_10                 5
#define MA_NAME_CHAR_11_12                6
#define MA_NAME_CHAR_13_14                7
#define MA_NAME_CHAR_15_16                8
#define MA_NAME_CHAR_17_18                9
#define MA_VERSION_SW                     10
#define MA_DATA_COMP_1                    11
#define MA_DATA_COMP_2                    12
#define MA_TIME_COMP                      13
#define MA_VERSION_GMM                    14
#define MA_ZBIRKA_SW                      15

#define M_ADDRESS_LAST_RO_BLOCK_1         15
/*****************************************************/

/*****************************************************/
/* Регістри користувача */
/*****************************************************/
#define M_ADDRESS_FIRST_USER_REGISTER_DATA      16
#define M_ADDRESS_LAST_USER_REGISTER_DATA       99
/*****************************************************/

/*****************************************************/
/* Адресний простір виходів  */
/*****************************************************/
#define MA_OUTPUTS                        100
#define BIT_MA_OUTPUTS_BASE                   100
#define BIT_MA_OUTPUTS_FIRST                  100
#define BIT_MA_OUTPUTS_LAST                   (BIT_MA_OUTPUTS_FIRST + NUMBER_OUTPUTS - 1)
/*****************************************************/

/*****************************************************/
/* Адресний простір входів  */
/*****************************************************/
#define MA_INPUTS                         200
#define BIT_MA_INPUTS_BASE                    200
#define BIT_MA_INPUTS_FIRST                   200
#define BIT_MA_INPUTS_LAST                    (BIT_MA_INPUTS_FIRST + NUMBER_INPUTS - 1)
/*****************************************************/

/*****************************************************/
/* Адресний простір світлоіндикаторів  */
/*****************************************************/
#define MA_LEDS                           232
#define BIT_MA_LEDS_BASE                      232
#define BIT_MA_LEDS_FIRST                     232
#define BIT_MA_LEDS_LAST                      (BIT_MA_LEDS_FIRST + NUMBER_LEDS - 1)
/*****************************************************/

/*****************************************************/
/* Адресний простір встановлення уставок і витримок */
/*****************************************************/
#define M_ADDRESS_FIRST_SETPOINTS_PART1 300

#define MA_GROUP_USTAVOK                  300

#define MA_TYPE_MTZ1                      301
#define MA_TYPE_MTZ2                      302
#define MA_TYPE_MTZ3                      303
#define MA_TYPE_MTZ4                      304

#define MA_TYPE_MTZ04_2                   306

#define M_ADDRESS_LAST_SETPOINTS_PART1    306

#define M_ADDRESS_FIRST_SETPOINTS_ZACHYSTIV 307
#define SHIFT_G1        0
#define SHIFT_G2        (2307 - M_ADDRESS_FIRST_SETPOINTS_ZACHYSTIV)
#define SHIFT_G3        (2454 - M_ADDRESS_FIRST_SETPOINTS_ZACHYSTIV)
#define SHIFT_G4        (2601 - M_ADDRESS_FIRST_SETPOINTS_ZACHYSTIV)

#define MA_STP_MTZ1                       307
#define MA_STP_MTZ1_N_VPERED              308
#define MA_STP_MTZ1_N_NAZAD               309
#define MA_STP_MTZ1_PO_NAPRUZI            310
#define MA_STP_MTZ1_U                     311
#define MA_STP_MTZ1_ANGLE                 312
#define MA_TO_MTZ1                        313
#define MA_TO_MTZ1_N_VPERED               314
#define MA_TO_MTZ1_N_NAZAD                315
#define MA_TO_MTZ1_PO_NAPRUZI             316
#define MA_STP_MTZ2                       317
#define MA_STP_MTZ2_N_VPERED              318
#define MA_STP_MTZ2_N_NAZAD               319
#define MA_STP_MTZ2_PO_NAPRUZI            320
#define MA_STP_MTZ2_U                     321
#define MA_STP_MTZ2_ANGLE                 322
#define MA_TO_MTZ2                        323
#define MA_TO_MTZ2_N_VPERED               324
#define MA_TO_MTZ2_N_NAZAD                325
#define MA_TO_MTZ2_PO_NAPRUZI             326
#define MA_TO_MTZ2_VVID_PR                327
#define MA_TO_MTZ2_PR                     328
#define MA_TO_MTZ2_N_VPERED_PR            329
#define MA_TO_MTZ2_N_NAZAD_PR             330
#define MA_TO_MTZ2_PO_NAPRUZI_PR          331
#define MA_STP_MTZ3                       332
#define MA_STP_MTZ3_N_VPERED              333
#define MA_STP_MTZ3_N_NAZAD               334
#define MA_STP_MTZ3_PO_NAPRUZI            335
#define MA_STP_MTZ3_U                     336
#define MA_STP_MTZ3_ANGLE                 337
#define MA_TO_MTZ3                        338
#define MA_TO_MTZ3_N_VPERED               339
#define MA_TO_MTZ3_N_NAZAD                340
#define MA_TO_MTZ3_PO_NAPRUZI             341
#define MA_STP_MTZ4                       342
#define MA_STP_MTZ4_N_VPERED              343
#define MA_STP_MTZ4_N_NAZAD               344
#define MA_STP_MTZ4_PO_NAPRUZI            345
#define MA_STP_MTZ4_U                     346
#define MA_STP_MTZ4_ANGLE                 347
#define MA_TO_MTZ4                        348
#define MA_TO_MTZ4_N_VPERED               349
#define MA_TO_MTZ4_N_NAZAD                350
#define MA_TO_MTZ4_PO_NAPRUZI             351

#define MA_STP_MTZ04_1                    352
#define MA_TO_MTZ04_1                     353
#define MA_STP_MTZ04_2                    354
#define MA_TO_MTZ04_2                     355
#define MA_TO_MTZ04_2_VVID_PR             356
#define MA_TO_MTZ04_2_PR                  357

#define MA_STP_NZZ1_3I0                   358
#define MA_TO_NZZ1_3I0                    359

#define MA_STP_NZZ1_3U0                   362
#define MA_TO_NZZ1_3U0                    363

#define MA_TO_NZZ1                        365

#define MA_STP_TZNP1_3I0_VPERED           366
#define MA_STP_TZNP1_3U0_VPERED           367
#define MA_TO_TZNP1_VPERED                368
#define MA_STP_TZNP1_3I0_NAZAD            369
#define MA_STP_TZNP1_3U0_NAZAD            370
#define MA_TO_TZNP1_NAZAD                 371
#define MA_STP_TZNP1_ANGLE                372
#define MA_STP_TZNP2_3I0_VPERED           373
#define MA_STP_TZNP2_3U0_VPERED           374
#define MA_TO_TZNP2_VPERED                375
#define MA_STP_TZNP2_3I0_NAZAD            376
#define MA_STP_TZNP2_3U0_NAZAD            377
#define MA_TO_TZNP2_NAZAD                 378
#define MA_STP_TZNP2_ANGLE                379
#define MA_STP_TZNP3_3I0_VPERED           380
#define MA_STP_TZNP3_3U0_VPERED           381
#define MA_TO_TZNP3_VPERED                382
#define MA_STP_TZNP3_3I0_NAZAD            383
#define MA_STP_TZNP3_3U0_NAZAD            384
#define MA_TO_TZNP3_NAZAD                 385
#define MA_STP_TZNP3_ANGLE                386

#define MA_STP_ZOP1                       387
#define MA_TO_ZOP1                        388

#define MA_STP_UMIN1                      393
#define MA_TO_UMIN1                       394
#define MA_STP_UMIN2                      395
#define MA_TO_UMIN2                       396
#define MA_STP_BLK_UMIN1_BY_I             397
#define MA_STP_BLK_UMIN2_BY_I             398

#define MA_STP_UMAX1                      399
#define MA_TO_UMAX1                       400
#define MA_STP_UMAX2                      401
#define MA_TO_UMAX2                       402

//#define MA_STP_U_OL                       406
//#define MA_STP_U_RL                       407
//#define MA_TO_U_OL                        408
//#define MA_TO_U_RL                        409

#define MA_STP_UROV                       418
#define MA_TO_UROV1                       419
#define MA_TO_UROV2                       420

//#define MA_STP_AVR_MIN1                   421
//#define MA_STP_AVR_MAX1                   422
//#define MA_STP_AVR_MIN2                   423
//#define MA_STP_AVR_MAX2                   424
//#define MA_TO_AVR_RL                      425
//#define MA_TO_AVR_OL                      426
//#define MA_STP_KRATN_AVR                  427
//#define MA_TO_AVR_ZAVERSHENNJA            428
//#define MA_TO_AVR_RESET_BLK               429

#define MA_TO_APV_BLOCK_VID_VV            430
#define MA_TO_APV_CYCLE_1                 431
#define MA_TO_APV_CYCLE_2                 432
#define MA_TO_APV_CYCLE_3                 433
#define MA_TO_APV_CYCLE_4                 434
#define MA_TO_APV_BLOCK_VID_APV1          435
#define MA_TO_APV_BLOCK_VID_APV2          436
#define MA_TO_APV_BLOCK_VID_APV3          437
#define MA_TO_APV_BLOCK_VID_APV4          438

//#define MA_STP_APV_ZMN1                   439
//#define MA_STP_APV_ZMN2                   440
//#define MA_TO_APV_ZMN1                    441
//#define MA_TO_APV_ZMN2                    442
//#define MA_STP_KRATN_APV_ZMN              443
//#define MA_TO_APV_ZMN_ZAVERSHENNJA        444

#define MA_STP_ACHR1_F_RAB                445
#define MA_STP_ACHR2_F_RAB                446
#define MA_STP_CHAPV1_F_RAB               447
#define MA_STP_CHAPV2_F_RAB               448
#define MA_STP_ACHR_CHAPV_UF              449
#define MA_TO_ACHR_1                      450
#define MA_TO_ACHR_2                      451
#define MA_TO_CHAPV_1                     452
#define MA_TO_CHAPV_2                     453

#define M_ADDRESS_LAST_SETPOINTS_ZACHYSTIV 453

#define M_ADDRESS_FIRST_SETPOINTS_CONTINUE 454

#define MA_LINE_1                         455
#define MA_DOVGYNA_1_1                    456
#define MA_OPIR_1_1                       457
#define MA_DOVGYNA_1_2                    458
#define MA_OPIR_1_2                       459
#define MA_DOVGYNA_1_3                    460
#define MA_OPIR_1_3                       461
#define MA_DOVGYNA_1_4                    462
#define MA_OPIR_1_4                       463
#define MA_DOVGYNA_1_5                    464
#define MA_OPIR_1_5                       465
#define MA_DOVGYNA_1_6                    466
#define MA_OPIR_1_6                       467
#define MA_DOVGYNA_1_7                    468
#define MA_OPIR_1_7                       469
#define MA_DOVGYNA_1_8                    470
#define MA_OPIR_1_8                       471
#define MA_LINE_2                         472
#define MA_DOVGYNA_2_1                    473
#define MA_OPIR_2_1                       474
#define MA_DOVGYNA_2_2                    475
#define MA_OPIR_2_2                       476
#define MA_DOVGYNA_2_3                    477
#define MA_OPIR_2_3                       478
#define MA_DOVGYNA_2_4                    479
#define MA_OPIR_2_4                       480
#define MA_DOVGYNA_2_5                    481
#define MA_OPIR_2_5                       482
#define MA_DOVGYNA_2_6                    483
#define MA_OPIR_2_6                       484
#define MA_DOVGYNA_2_7                    485
#define MA_OPIR_2_7                       486
#define MA_DOVGYNA_2_8                    487
#define MA_OPIR_2_8                       488

#define MA_TN1                            489

#define MA_TT                             491
#define MA_T0                             492
#define MA_TT04                           493

#define MA_TO_SWCH_UDL_BLK_ON             494
#define MA_TO_SWCH_ON                     495
#define MA_TO_SWCH_OFF                    496
#define MA_TO_FAULT_CONTROL_CIRCUIT       497

#define MA_STP_Inom                       499
#define MA_STP_RKS_Inom                   500
#define MA_STP_Inom_vymk                  501
#define MA_STP_RKS_Inom_vymk              502
#define MA_STP_POCHATKOVYJ_RESURS_LSW     503
#define MA_STP_POCHATKOVYJ_RESURS_MSW     504
#define MA_STP_KRYTYCHNYJ_RESURS          505
#define MA_POCHATKOVA_K_VYMK_LSW          506
#define MA_POCHATKOVA_K_VYMK_MSW          507

#define MA_UVV_TYPE_SIGNAL_INPUT          512
#define MA_UVV_TYPE_INPUT                 513
#define MA_UVV_TYPE_OUTPUT                514
#define MA_UVV_TYPE_OUTPUT_MODIF          515
#define MA_TYPE_DF                        516
#define MA_UVV_TYPE_LED                   517

#define MA_DOPUSK_DV_1                    518
#define MA_DOPUSK_DV_2                    519
#define MA_DOPUSK_DV_3                    520
#define MA_DOPUSK_DV_4                    521
#define MA_DOPUSK_DV_5                    522
#define MA_DOPUSK_DV_6                    523
#define MA_DOPUSK_DV_7                    524
#define MA_DOPUSK_DV_8                    525
#define MA_DOPUSK_DV_9                    526
#define MA_DOPUSK_DV_10                   527

#define MA_DF_PAUSE_1                     534
#define MA_DF_PAUSE_2                     535
#define MA_DF_PAUSE_3                     536
#define MA_DF_PAUSE_4                     537
#define MA_DF_PAUSE_5                     538
#define MA_DF_PAUSE_6                     539
#define MA_DF_PAUSE_7                     540
#define MA_DF_PAUSE_8                     541
#define MA_DF_WORK_1                      542
#define MA_DF_WORK_2                      543
#define MA_DF_WORK_3                      544
#define MA_DF_WORK_4                      545
#define MA_DF_WORK_5                      546
#define MA_DF_WORK_6                      547
#define MA_DF_WORK_7                      548
#define MA_DF_WORK_8                      549

#define MA_CONTROL_BASE                   550
#define BIT_MA_CONTROL_BASE                   55000

#define MA_CONTROL_MTZ                    550
#define BIT_MA_CONTROL_MTZ_BASE               55000
#define BIT_MA_CONTROL_MTZ1                   55000
#define BIT_MA_CONTROL_MTZ2                   55001
#define BIT_MA_CONTROL_MTZ3                   55002
#define BIT_MA_CONTROL_MTZ4                   55003
#define BIT_MA_CONTROL_MTZ2_PRYSKORENNJA      55004
#define BIT_MA_CONTROL_MTZ2_PRYSKORENA        55005
#define BIT_MA_CONTROL_MTZ1_N_VPERED          55006
#define BIT_MA_CONTROL_MTZ1_N_NAZAD           55007
#define BIT_MA_CONTROL_MTZ2_N_VPERED          55008
#define BIT_MA_CONTROL_MTZ2_N_NAZAD           55009
#define BIT_MA_CONTROL_MTZ3_N_VPERED          55010
#define BIT_MA_CONTROL_MTZ3_N_NAZAD           55011
#define BIT_MA_CONTROL_MTZ4_N_VPERED          55012
#define BIT_MA_CONTROL_MTZ4_N_NAZAD           55013
#define BIT_MA_CONTROL_MTZ_NESPR_KIL_NAPR     55014     

#define MA_CONTROL_MTZ04                  551
#define BIT_MA_CONTROL_MTZ04_BASE             55016
#define BIT_MA_CONTROL_MTZ04_1                55016
#define BIT_MA_CONTROL_MTZ04_2                55017
#define BIT_MA_CONTROL_MTZ04_2_PRYSKORENNJA   55018
#define BIT_MA_CONTROL_MTZ04_2_PRYSKORENA     55019

#define MA_CONTROL_ZDZ                    552
#define BIT_MA_CONTROL_ZDZ_BASE               55032
#define BIT_MA_CONTROL_ZDZ                    55032
#define BIT_MA_CONTROL_ZDZ_STARTED_FROM_MTZ1  55033
#define BIT_MA_CONTROL_ZDZ_STARTED_FROM_MTZ2  55034
#define BIT_MA_CONTROL_ZDZ_STARTED_FROM_MTZ3  55035
#define BIT_MA_CONTROL_ZDZ_STARTED_FROM_MTZ4  55036

#define MA_CONTROL_NZZ                    553
#define BIT_MA_CONTROL_NZZ_BASE               55048
#define BIT_MA_CONTROL_NZZ_3I0                55048

#define BIT_MA_CONTROL_NZZ_SECTOR             55050
#define BIT_MA_CONTROL_NZZ                    55051
#define BIT_MA_CONTROL_NZZ_3U0                55052
#define BIT_MA_CONTROL_NZZ_TYPE               55053

#define MA_CONTROL_TZNP                   554
#define BIT_MA_CONTROL_TZNP_BASE              55064
#define BIT_MA_CONTROL_TZNP1                  55064
#define BIT_MA_CONTROL_TZNP1_VPERED           55065
#define BIT_MA_CONTROL_TZNP1_NAZAD            55066
#define BIT_MA_CONTROL_TZNP2                  55067
#define BIT_MA_CONTROL_TZNP2_VPERED           55068
#define BIT_MA_CONTROL_TZNP2_NAZAD            55069
#define BIT_MA_CONTROL_TZNP3                  55070
#define BIT_MA_CONTROL_TZNP3_VPERED           55071
#define BIT_MA_CONTROL_TZNP3_NAZAD            55072

#define MA_CONTROL_ZOP                    555
#define BIT_MA_CONTROL_ZOP_BASE               55080
#define BIT_MA_CONTROL_ZOP1                   55080

#define MA_CONTROL_ACHR_CHAPV             556
#define BIT_MA_CONTROL_ACHR_CHAPV_BASE        55096
#define BIT_MA_CONTROL_ACHR1_STATE            55096
#define BIT_MA_CONTROL_ACHR2_STATE            55097
#define BIT_MA_CONTROL_CHAPV1_STATE           55098
#define BIT_MA_CONTROL_CHAPV2_STATE           55099
#define BIT_MA_CONTROL_CHAPV_VID_DV_STATE     55100

#define MA_CONTROL_UMIN                   557
#define BIT_MA_CONTROL_UMIN_BASE              55112
#define BIT_MA_CONTROL_UMIN1                  55112
#define BIT_MA_CONTROL_UMIN2                  55113
#define BIT_MA_CONTROL_PO_UMIN1_AND_OR        55114
#define BIT_MA_CONTROL_PO_UMIN2_AND_OR        55115
#define BIT_MA_CONTROL_BLK_UMIN1_BY_U         55116
#define BIT_MA_CONTROL_BLK_UMIN2_BY_U         55117
#define BIT_MA_CONTROL_BLK_UMIN1_BY_I         55118
#define BIT_MA_CONTROL_BLK_UMIN2_BY_I         55119

#define MA_CONTROL_UMAX                   558
#define BIT_MA_CONTROL_UMAX_BASE              55128
#define BIT_MA_CONTROL_UMAX1                  55128
#define BIT_MA_CONTROL_UMAX2                  55129
#define BIT_MA_CONTROL_PO_UMAX1_AND_OR        55130
#define BIT_MA_CONTROL_PO_UMAX2_AND_OR        55131

#define MA_CONTROL_558                    559
#define BIT_MA_CONTROL_558_BASE               55144

#define BIT_MA_CONTROL_PHASE_LINE             55145
//#define BIT_MA_CONTROL_3U0_Ubc_TN2            55146
#define BIT_MA_CONTROL_IB_I04                 55147

//#define BIT_MA_CONTROL_APV_ZMN                55148
//#define BIT_MA_CONTROL_U_OL                   55149
//#define BIT_MA_CONTROL_U_RL                   55150

#define MA_CONTROL_APV                    561
#define BIT_MA_CONTROL_APV_BASE               55176
#define BIT_MA_CONTROL_APV_CYCLE1             55176
#define BIT_MA_CONTROL_APV_CYCLE2             55177
#define BIT_MA_CONTROL_APV_CYCLE3             55178
#define BIT_MA_CONTROL_APV_CYCLE4             55179
#define BIT_MA_CONTROL_APV_STARTED_FROM_MTZ1  55180
#define BIT_MA_CONTROL_APV_STARTED_FROM_MTZ2  55181
#define BIT_MA_CONTROL_APV_STARTED_FROM_MTZ3  55182
#define BIT_MA_CONTROL_APV_STARTED_FROM_MTZ4  55183

#define MA_CONTROL_UROV_PART1             562
#define BIT_MA_CONTROL_UROV_BASE_PART1           55192
#define BIT_MA_CONTROL_UROV_STATE                55192
#define BIT_MA_CONTROL_UROV_STARTED_FROM_MTZ1    55193
#define BIT_MA_CONTROL_UROV_STARTED_FROM_MTZ2    55194
#define BIT_MA_CONTROL_UROV_STARTED_FROM_MTZ3    55195
#define BIT_MA_CONTROL_UROV_STARTED_FROM_MTZ4    55196
#define BIT_MA_CONTROL_UROV_STARTED_FROM_MTZ04_1 55197
#define BIT_MA_CONTROL_UROV_STARTED_FROM_MTZ04_2 55198
#define BIT_MA_CONTROL_UROV_STARTED_FROM_UMAX1   55199
#define BIT_MA_CONTROL_UROV_STARTED_FROM_UMAX2   55200
#define BIT_MA_CONTROL_UROV_STARTED_FROM_UMIN1   55201
#define BIT_MA_CONTROL_UROV_STARTED_FROM_UMIN2   55202
#define BIT_MA_CONTROL_UROV_STARTED_FROM_ZOP1    55203

#define BIT_MA_CONTROL_UROV_STARTED_FROM_NZZ     55205
#define BIT_MA_CONTROL_UROV_STARTED_FROM_3I0     55206


#define MA_CONTROL_UROV_PART2             563
#define BIT_MA_CONTROL_UROV_BASE_PART2         55208
#define BIT_MA_CONTROL_UROV_STARTED_FROM_3U0   55208
#define BIT_MA_CONTROL_UROV_STARTED_FROM_TZNP1 55209
#define BIT_MA_CONTROL_UROV_STARTED_FROM_TZNP2 55210
#define BIT_MA_CONTROL_UROV_STARTED_FROM_TZNP3 55211
#define BIT_MA_CONTROL_UROV_STARTED_FROM_ZDZ   55212
#define BIT_MA_CONTROL_UROV_STARTED_FROM_ACHR1 55213
#define BIT_MA_CONTROL_UROV_STARTED_FROM_ACHR2 55214

//#define MA_CONTROL_AVR                    564
//#define BIT_MA_CONTROL_AVR_BASE               55224
//#define BIT_MA_CONTROL_AVR                    55224
//#define BIT_MA_CONTROL_AVR_OTKL_BLK_VID_ZAHYSTIV 55225

#define MA_CONTROL_CTRL_VV                565
#define BIT_MA_CONTROL_CTRL_VV_BASE            55240
#define BIT_MA_CONTROL_CTRL_VV_STATE           55240

#define BIT_MA_CONTROL_CTRL_BLK_ON_CB_MISCEVE  55243
#define BIT_MA_CONTROL_CTRL_BLK_OFF_CB_MISCEVE 55244
#define BIT_MA_CONTROL_CTRL_FB_ACTIVATION      55245
#define BIT_MA_CONTROL_CTRL_RESURS_VV          55246
#define BIT_MA_CONTROL_CTRL_READY_TU           55247
#define BIT_MA_CONTROL_CTRL_WINDOW_OFF_CB      55248

#define MA_CONTROL_VMP                    566
#define BIT_MA_CONTROL_VMP_BASE                55256
#define BIT_MA_CONTROL_VMP_STATE               55256

#define MA_CONFIGURATION_PART1            567
#define BIT_MA_CONFIGURATION_BASE_PART1        55272
#define BIT_MA_CONFIGURATION_MTZ               55272
#define BIT_MA_CONFIGURATION_MTZ04             55273
#define BIT_MA_CONFIGURATION_ZDZ               55274
#define BIT_MA_CONFIGURATION_ZZ                55275
#define BIT_MA_CONFIGURATION_TZNP              55276
#define BIT_MA_CONFIGURATION_ZOP               55277
#define BIT_MA_CONFIGURATION_ACHR_CHAPV        55278
#define BIT_MA_CONFIGURATION_UMIN              55279
#define BIT_MA_CONFIGURATION_UMAX              55280

//#define BIT_MA_CONFIGURATION_APV_ZMN           55283
//#define BIT_MA_CONFIGURATION_KROL              55284

#define BIT_MA_CONFIGURATION_APV               55286
#define BIT_MA_CONFIGURATION_UROV              55287

#define MA_CONFIGURATION_PART2            568
#define BIT_MA_CONFIGURATION_BASE_PART2        55288
//#define BIT_MA_CONFIGURATION_AVR               55288
#define BIT_MA_CONFIGURATION_VMP               55289
#define BIT_MA_CONFIGURATION_EL                55290

#define MA_CONTROL_LAST                   568
#define BIT_MA_CONTROL_LAST                   (BIT_MA_CONTROL_BASE + ((MA_CONTROL_LAST - MA_CONTROL_BASE + 1) << 4) - 1)

/***/
#define MA_TO_DEACTIVATION_PASSWORD_INTERFACE 569
#define MA_PASSWORD_INTERFACE                 570
/***/

#define MA_LANGUAGE_MENU                      571


#define MA_SPEED_RS485                        573
#define MA_STOP_BITS_RS485                    574
#define MA_PARE_RS485                         575
#define MA_TIMEOUT_RS485                      576
#define MA_LOGICAL_ADDRESS                    577

#define MA_NAME_OF_CELL_CHARS_01_02           585
#define MA_NAME_OF_CELL_CHARS_03_04           586
#define MA_NAME_OF_CELL_CHARS_05_06           587
#define MA_NAME_OF_CELL_CHARS_07_08           588
#define MA_NAME_OF_CELL_CHARS_09_10           589
#define MA_NAME_OF_CELL_CHARS_11_12           590
#define MA_NAME_OF_CELL_CHARS_13_14           591
#define MA_NAME_OF_CELL_CHARS_15_16           592

#define M_ADDRESS_LAST_SETPOINTS_CONTINUE     592
/*****************************************************/

/*****************************************************/
/* Адресний простір дати і часу */
/*****************************************************/
#define M_ADDRESS_FIRST_TIME_AND_DATA         593

#define MA_YEAR                               593
#define MA_MONTH                              594
#define MA_DAY                                595
#define MA_HOUR                               596
#define MA_MINUTE                             597
#define MA_SECOND                             598
#define MA_MILISECOND                         599

#define M_ADDRESS_LAST_TIME_AND_DATA          599
/*****************************************************/

/*****************************************************/
/* Текучі активні функції */
/*****************************************************/
#define M_ADDRESS_FIRST_CURRENT_AF            600
#define BIT_MA_CURRENT_AF_BASE                  60000

#define BIT_MA_1_GRUPA_USTAVOK                  60000
#define BIT_MA_2_GRUPA_USTAVOK                  60001
#define BIT_MA_3_GRUPA_USTAVOK                  60002
#define BIT_MA_4_GRUPA_USTAVOK                  60003
#define BIT_MA_BLOCK_MTZ1                       60004
#define BIT_MA_MTZ1                             60005
#define BIT_MA_PO_MTZ1                          60006
#define BIT_MA_PO_MTZPN1                        60007
#define BIT_MA_PO_MTZN1_VPERED                  60008
#define BIT_MA_PO_MTZN1_NAZAD                   60009
#define BIT_MA_SECTOR_VPERED_MTZN1              60010
#define BIT_MA_SECTOR_NAZAD_MTZN1               60011
#define BIT_MA_PO_U_MTZPN1                      60012

#define BIT_MA_BLOCK_MTZ2                       60016
#define BIT_MA_MTZ2                             60017
#define BIT_MA_PO_MTZ2                          60018
#define BIT_MA_PO_MTZPN2                        60019
#define BIT_MA_PO_MTZN2_VPERED                  60020
#define BIT_MA_PO_MTZN2_NAZAD                   60021
#define BIT_MA_SECTOR_VPERED_MTZN2              60022
#define BIT_MA_SECTOR_NAZAD_MTZN2               60023
#define BIT_MA_PO_U_MTZPN2                      60024
#define BIT_MA_BLOCK_USK_MTZ2                   60025

#define BIT_MA_BLOCK_MTZ3                       60032
#define BIT_MA_MTZ3                             60033
#define BIT_MA_PO_MTZ3                          60034
#define BIT_MA_PO_MTZPN3                        60035
#define BIT_MA_PO_MTZN3_VPERED                  60036
#define BIT_MA_PO_MTZN3_NAZAD                   60037
#define BIT_MA_SECTOR_VPERED_MTZN3              60038
#define BIT_MA_SECTOR_NAZAD_MTZN3               60039
#define BIT_MA_PO_U_MTZPN3                      60040

#define BIT_MA_BLOCK_MTZ4                       60048
#define BIT_MA_MTZ4                             60049
#define BIT_MA_PO_MTZ4                          60050
#define BIT_MA_PO_MTZPN4                        60051
#define BIT_MA_PO_MTZN4_VPERED                  60052
#define BIT_MA_PO_MTZN4_NAZAD                   60053
#define BIT_MA_SECTOR_VPERED_MTZN4              60054
#define BIT_MA_SECTOR_NAZAD_MTZN4               60055
#define BIT_MA_PO_U_MTZPN4                      60056

#define BIT_MA_BLOCK_MTZ04_1                    60057
#define BIT_MA_MTZ04_1                          60058
#define BIT_MA_PO_MTZ04_1                       60059
#define BIT_MA_BLOCK_MTZ04_2                    60060
#define BIT_MA_MTZ04_2                          60061
#define BIT_MA_PO_MTZ04_2                       60062
#define BIT_MA_BLOCK_USK_MTZ04_2                60063

#define BIT_MA_NCN_MTZ                          60064
#define BIT_MA_PO_BLOCK_U_MTZN                  60065

#define BIT_MA_PUSK_ZDZ_VID_DV                  60067
#define BIT_MA_ZDZ                              60068

#define BIT_MA_BLOCK_NZZ                        60080
#define BIT_MA_PO_NZZ                           60081
#define BIT_MA_NZZ                              60082
#define BIT_MA_PO_ZZ1_3I0                       60083
#define BIT_MA_ZZ1_3I0                          60084

#define BIT_MA_PO_ZZ_3U0                        60087
#define BIT_MA_ZZ_3U0                           60088
#define BIT_MA_SECTOR_NZZ                       60089

#define BIT_MA_BLOCK_TZNP1                      60096
#define BIT_MA_PO_3I0_TZNP1_VPERED              60097
#define BIT_MA_PO_3I0_TZNP1_NAZAD               60098
#define BIT_MA_PO_3U0_TZNP1_VPERED              60099
#define BIT_MA_PO_3U0_TZNP1_NAZAD               60100
#define BIT_MA_PO_TZNP1_VPERED                  60101
#define BIT_MA_PO_TZNP1_NAZAD                   60102
#define BIT_MA_SECTOR_TZNP1_VPERED              60103
#define BIT_MA_SECTOR_TZNP1_NAZAD               60104
#define BIT_MA_TZNP1                            60105
#define BIT_MA_BLOCK_TZNP2                      60106
#define BIT_MA_PO_3I0_TZNP2_VPERED              60107
#define BIT_MA_PO_3I0_TZNP2_NAZAD               60108
#define BIT_MA_PO_3U0_TZNP2_VPERED              60109
#define BIT_MA_PO_3U0_TZNP2_NAZAD               60110
#define BIT_MA_PO_TZNP2_VPERED                  60111
#define BIT_MA_PO_TZNP2_NAZAD                   60112
#define BIT_MA_SECTOR_TZNP2_VPERED              60113
#define BIT_MA_SECTOR_TZNP2_NAZAD               60114
#define BIT_MA_TZNP2                            60115
#define BIT_MA_BLOCK_TZNP3                      60116
#define BIT_MA_PO_3I0_TZNP3_VPERED              60117
#define BIT_MA_PO_3I0_TZNP3_NAZAD               60118
#define BIT_MA_PO_3U0_TZNP3_VPERED              60119
#define BIT_MA_PO_3U0_TZNP3_NAZAD               60120
#define BIT_MA_PO_TZNP3_VPERED                  60121
#define BIT_MA_PO_TZNP3_NAZAD                   60122
#define BIT_MA_SECTOR_TZNP3_VPERED              60123
#define BIT_MA_SECTOR_TZNP3_NAZAD               60124
#define BIT_MA_TZNP3                            60125

#define BIT_MA_BLOCK_ZOP                        60128
#define BIT_MA_ZOP                              60129
#define BIT_MA_PO_ZOP                           60130

#define BIT_MA_BLOCK_UMIN1                      60144
#define BIT_MA_PO_UBLK_UMIN1                    60145
#define BIT_MA_PO_IBLK_UMIN1                    60146
#define BIT_MA_UMIN1                            60147
#define BIT_MA_PO_UMIN1                         60148
#define BIT_MA_BLOCK_UMIN2                      60149
#define BIT_MA_PO_UBLK_UMIN2                    60150
#define BIT_MA_PO_IBLK_UMIN2                    60151
#define BIT_MA_UMIN2                            60152
#define BIT_MA_PO_UMIN2                         60153
#define BIT_MA_START_UMIN1                      60154
#define BIT_MA_START_UMIN2                      60155

#define BIT_MA_BLOCK_UMAX1                      60160
#define BIT_MA_UMAX1                            60161
#define BIT_MA_PO_UMAX1                         60162
#define BIT_MA_BLOCK_UMAX2                      60163
#define BIT_MA_UMAX2                            60164
#define BIT_MA_PO_UMAX2                         60165

//#define M_ADDRESS_APV_ZMN                     611
//#define BIT_MA_APV_ZMN_BASE                     60176
//#define BIT_MA_RESET_BLOCK_APV_ZMN              60176
//#define BIT_MA_BLOCK_APV_ZMN                    60177
//#define BIT_MA_PO_UAPV_ZMN1                     60178
//#define BIT_MA_PO_UAPV_ZMN2                     60179
//#define BIT_MA_PO_APV_ZMN                       60180
//#define BIT_MA_APV_ZMN                          60181
//#define BIT_MA_BLOCK_KRATN_APV_ZMN              60182
//#define BIT_MA_BLOCK_APV_ZMN_VID_ZAKHYSTIV      60183

//#define BIT_MA_PO_U_OL                          60192
//#define BIT_MA_U_OL                             60193
//#define BIT_MA_PO_U_RL                          60194
//#define BIT_MA_U_RL                             60195

#define BIT_MA_STAT_BLK_APV                     60224
#define BIT_MA_APV                              60225
#define BIT_MA_APV2                             60226
#define BIT_MA_APV3                             60227
#define BIT_MA_APV4                             60228
#define BIT_MA_APV_WORK                         60229

#define BIT_MA_PUSK_UROV_VID_DV                 60240
#define BIT_MA_UROV1                            60241
#define BIT_MA_UROV2                            60242
#define BIT_MA_PO_UROV                          60243

//#define M_ADDRESS_AVR                         616
//#define BIT_MA_AVR_BASE                         60256
//#define BIT_MA_OTKL_AVR                         60256
//#define BIT_MA_SBROS_BLOCK_AVR                  60257
//#define BIT_MA_STAT_BLOCK_AVR                   60258
//#define BIT_MA_PO_UAVR_MIN1                     60259
//#define BIT_MA_PO_UAVR_MAX1                     60260
//#define BIT_MA_PO_UAVR_MIN2                     60261
//#define BIT_MA_PO_UAVR_MAX2                     60262
//#define BIT_MA_BLOCK_AVR_VID_ZAKHYSTIV          60263
//#define BIT_MA_BLOCK_KRATN_AVR                  60264
//#define BIT_MA_PUSK_AVR                         60265

#define BIT_MA_ACHR_CHAPV_VID_DV                60272
#define BIT_MA_BLOCK_ACHR1                      60273
#define BIT_MA_BLOCK_ACHR2                      60274
#define BIT_MA_VN_BLOCK_CHAPV                   60275
#define BIT_MA_PO_ACHR1                         60276
#define BIT_MA_PO_ACHR2                         60277
#define BIT_MA_ACHR_CHAPV1                      60278
#define BIT_MA_ACHR_CHAPV2                      60279
#define BIT_MA_RAZR_CHAPV                       60280
#define BIT_MA_PO_CHAPV1                        60281
#define BIT_MA_PO_CHAPV2                        60282

#define M_ADDRESS_DF                          618
#define BIT_MA_DF_BASE                          60288
#define BIT_MA_INPUT_DF1                        60288
#define BIT_MA_INPUT_DF2                        60289
#define BIT_MA_INPUT_DF3                        60290
#define BIT_MA_INPUT_DF4                        60291
#define BIT_MA_INPUT_DF5                        60292
#define BIT_MA_INPUT_DF6                        60293
#define BIT_MA_INPUT_DF7                        60294
#define BIT_MA_INPUT_DF8                        60295
#define BIT_MA_OUTPUT_DF1                       60296
#define BIT_MA_OUTPUT_DF2                       60297
#define BIT_MA_OUTPUT_DF3                       60298
#define BIT_MA_OUTPUT_DF4                       60299
#define BIT_MA_OUTPUT_DF5                       60300
#define BIT_MA_OUTPUT_DF6                       60301
#define BIT_MA_OUTPUT_DF7                       60302
#define BIT_MA_OUTPUT_DF8                       60303

#define M_ADDRESS_DT                          619
#define BIT_MA_DT_BASE                          60304
#define BIT_MA_DT1_SET                          60304
#define BIT_MA_DT1_RESET                        60305
#define BIT_MA_DT2_SET                          60306
#define BIT_MA_DT2_RESET                        60307
#define BIT_MA_DT3_SET                          60308
#define BIT_MA_DT3_RESET                        60309
#define BIT_MA_DT4_SET                          60310
#define BIT_MA_DT4_RESET                        60311
#define BIT_MA_DT1_OUT                          60312
#define BIT_MA_DT2_OUT                          60313
#define BIT_MA_DT3_OUT                          60314
#define BIT_MA_DT4_OUT                          60315

#define BIT_MA_D_AND1                           60320
#define BIT_MA_D_AND2                           60321
#define BIT_MA_D_AND3                           60322
#define BIT_MA_D_AND4                           60323
#define BIT_MA_D_AND5                           60324
#define BIT_MA_D_AND6                           60325
#define BIT_MA_D_AND7                           60326
#define BIT_MA_D_AND8                           60327
#define BIT_MA_D_OR1                            60328
#define BIT_MA_D_OR2                            60329
#define BIT_MA_D_OR3                            60330
#define BIT_MA_D_OR4                            60331
#define BIT_MA_D_OR5                            60332
#define BIT_MA_D_OR6                            60333
#define BIT_MA_D_OR7                            60334
#define BIT_MA_D_OR8                            60335
#define BIT_MA_D_NOT1                           60336
#define BIT_MA_D_NOT2                           60337
#define BIT_MA_D_NOT3                           60338
#define BIT_MA_D_NOT4                           60339
#define BIT_MA_D_NOT5                           60340
#define BIT_MA_D_NOT6                           60341
#define BIT_MA_D_NOT7                           60342
#define BIT_MA_D_NOT8                           60343
#define BIT_MA_D_NOT9                           60344
#define BIT_MA_D_NOT10                          60345
#define BIT_MA_D_NOT11                          60346
#define BIT_MA_D_NOT12                          60347
#define BIT_MA_D_NOT13                          60348
#define BIT_MA_D_NOT14                          60349
#define BIT_MA_D_NOT15                          60350
#define BIT_MA_D_NOT16                          60351
#define BIT_MA_D_XOR1                           60352
#define BIT_MA_D_XOR2                           60353
#define BIT_MA_D_XOR3                           60354
#define BIT_MA_D_XOR4                           60355
#define BIT_MA_D_XOR5                           60356
#define BIT_MA_D_XOR6                           60357
#define BIT_MA_D_XOR7                           60358
#define BIT_MA_D_XOR8                           60359

#define M_ADDRESS_COMMAND_BASE                623
#define BIT_MA_COMMAND_BASE                     60368
#define BIT_MA_VKL_VV                           60368
#define BIT_MA_OTKL_VV                          60369
#define BIT_MA_BLOCK_VKL_VV                     60370

#define BIT_MA_WORK_BO                          60372
#define BIT_MA_WORK_BV                          60373
#define BIT_MA_STATE_VV                         60374
#define BIT_MA_GEAR_VV                          60375
#define BIT_MA_CONTROL_VKL                      60376
#define BIT_MA_CONTROL_VIDKL                    60377
#define BIT_MA_PEREVYSHCHENNJA_Inom_VYMK        60378
#define BIT_MA_KRYTYCHNYJ_RESURS_VYMYKACHA      60379
#define BIT_MA_VYCHERPANYJ_RESURS_VYMYKACHA     60380
#define BIT_MA_DEFECT                           60381
#define BIT_MA_AVAR_DEFECT                      60382
#define BIT_MA_RESET_LEDS                       60383
#define BIT_MA_RESET_RELES                      60384
#define BIT_MA_RESET_GENERAL_AF                 60385
#define BIT_MA_RESET_RESURS_VYMYKACHA           60386
#define BIT_MA_SETTINGS_CHANGED                 60387
#define BIT_MA_PASSWORD_SET                     60388
#define BIT_MA_MISCEVE_DYSTANCIJNE              60389
#define BIT_MA_WORK_A_REJESTRATOR               60390
#define BIT_MA_WORK_D_REJESTRATOR               60391

#define BIT_MA_ERROR_CONF_EL                    60397

#define BIT_MA_OTKL_VID_ZOVN_ZAHYSTIV           60396                         

//#define BIT_MA_DVERI_SHAFY_UPR_VIDKR            60400
//#define BIT_MA_ACUMUL_BATAREJA_ROZRJADGENA      60401
//#define BIT_MA_REZERVTE_GYVLENNJA               60402
#define BIT_MA_VIDKL_VID_ZAKHYSTIV              60403
#define BIT_MA_BLK_GRUP_USTAVOK_VID_ZACHYSTIV   60404
#define BIT_MA_INVERS_DV_GRUPA_USTAVOK          60405
#define BIT_MA_READY_TU                         60406
#define BIT_MA_RESET_BLOCK_READY_TU_VID_ZAHYSTIV 60407

#define M_ADDRESS_LAST_CURRENT_AF             625
#define BIT_MA_CURRENT_AF_LAST                  (BIT_MA_CURRENT_AF_BASE + ((M_ADDRESS_LAST_CURRENT_AF - M_ADDRESS_FIRST_CURRENT_AF + 1) << 4) - 1)
/*****************************************************/

/*****************************************************/
/* Загальні активні функції */
/*****************************************************/
#define M_ADDRESS_FIRST_GENERAL_AF            626
#define BIT_MA_GENERAL_AF_BASE                  60416

#define M_ADDRESS_LAST_GENERAL_AF             651
#define BIT_MA_GENERAL_AF_LAST                  (BIT_MA_GENERAL_AF_BASE + ((M_ADDRESS_LAST_GENERAL_AF - M_ADDRESS_FIRST_GENERAL_AF + 1) << 4) - 1)
/*****************************************************/

/*****************************************************/
/* Митєві вимірювання розраховані фетодом перетворення Фур'є */
/*****************************************************/
#define M_ADDRESS_FIRST_MEASUREMENTS_1        652

#define OFFSET_MEASUREMENT_UA_1                 0
#define OFFSET_MEASUREMENT_UB_1                 1
#define OFFSET_MEASUREMENT_UC_1                 2
#define OFFSET_MEASUREMENT_IA_1                 3
#define OFFSET_MEASUREMENT_IB_1                 4
#define OFFSET_MEASUREMENT_IC_1                 5
#define OFFSET_MEASUREMENT_I04_1                6
#define OFFSET_ACTIVE_POWER                     7
#define OFFSET_REACTIVE_POWER                   8
#define OFFSET_FULL_POWER                       9
#define OFFSET_COS_PHI                          10
#define OFFSET_MEASUREMENT_3I0                  11
#define OFFSET_FREQUENCY_1                      12
#define OFFSET_EA_PLUS_LWB                      13
#define OFFSET_EA_PLUS_MWB                      14
#define OFFSET_EA_MINUS_LWB                     15
#define OFFSET_EA_MINUS_MWB                     16
#define OFFSET_ER_1_LWB                         17
#define OFFSET_ER_1_MWB                         18
#define OFFSET_ER_2_LWB                         19
#define OFFSET_ER_2_MWB                         20
#define OFFSET_ER_3_LWB                         21
#define OFFSET_ER_3_MWB                         22
#define OFFSET_ER_4_LWB                         23
#define OFFSET_ER_4_MWB                         24
#define OFFSET_MEASUREMENT_UAB_1                25
#define OFFSET_MEASUREMENT_UBC_1                26
#define OFFSET_MEASUREMENT_UCA_1                27
#define OFFSET_MEASUREMENT_3U0_1                28
#define OFFSET_MEASUREMENT_3I0_r                29
#define OFFSET_MEASUREMENT_3I0_HIGH_GARM        30
#define OFFSET_MEASUREMENT_I1                   31
#define OFFSET_MEASUREMENT_I2                   32


//#define OFFSET_MEASUREMENT_UAB_2                35
//#define OFFSET_MEASUREMENT_UBC_2                36

//#define OFFSET_FREQUENCY_2                      39

#define OFFSET_BASE_CANAL_FOR_ANGLE             41
#define OFFSET_ANGLE_UA_1                       42
#define OFFSET_ANGLE_UB_1                       43
#define OFFSET_ANGLE_UC_1                       44
#define OFFSET_ANGLE_UAB_1                      45
#define OFFSET_ANGLE_UBC_1                      46
#define OFFSET_ANGLE_UCA_1                      47
#define OFFSET_ANGLE_3U0_1                      48
//#define OFFSET_ANGLE_UAB_2                      49
//#define OFFSET_ANGLE_UBC_2                      50
#define OFFSET_ANGLE_IA_1                       51
#define OFFSET_ANGLE_IB_1                       52
#define OFFSET_ANGLE_IC_1                       53
#define OFFSET_ANGLE_I04_1                      54
#define OFFSET_ANGLE_3I0                        55
#define OFFSET_ANGLE_3I0_r                      56

//#define OFFSET_ANGLE_3I0_3U0                    60




#define M_ADDRESS_LAST_MEASUREMENTS_1         713
/*****************************************************/

/**************Лічильник ресурсів********************/
#define M_COUNTER_FIRST                       714

#define MA_COUNTER_VIDKLJUCHENNJA_LSW         714
#define MA_COUNTER_VIDKLJUCHENNJA_MSW         715
#define MA_COUNTER_VYMYKACHA_LSW              716
#define MA_COUNTER_VYMYKACHA_MSW              717

#define M_COUNTER_LAST                        717

/*****************************************************/

/*****************************************************/
/* Вимірювання в момент роботи дискретного реєстратора*/
/*****************************************************/
#define MEASUREMENTS_DR_WIDTH                 29 
#define M_ADDRESS_FIRST_MEASUREMENTS_DR       718

#define DR_OFFSET_MEASUREMENT_UA_1            0
#define DR_OFFSET_MEASUREMENT_UB_1            1
#define DR_OFFSET_MEASUREMENT_UC_1            2
#define DR_OFFSET_MEASUREMENT_IA_1            3
#define DR_OFFSET_MEASUREMENT_IB_1            4
#define DR_OFFSET_MEASUREMENT_IC_1            5
#define DR_OFFSET_MEASUREMENT_I04_1           6
#define DR_OFFSET_MEASUREMENT_UAB_1           7
#define DR_OFFSET_MEASUREMENT_UBC_1           8
#define DR_OFFSET_MEASUREMENT_UCA_1           9
#define DR_OFFSET_MEASUREMENT_3U0_1           10
#define DR_OFFSET_MEASUREMENT_3I0             11
#define DR_OFFSET_MEASUREMENT_3I0_r           12
#define DR_OFFSET_MEASUREMENT_3I0_HIGH_GARM   13
#define DR_OFFSET_MEASUREMENT_I1              14
#define DR_OFFSET_MEASUREMENT_I2              15
#define DR_OFFSET_FREQUENCY_1                 16

//#define DR_OFFSET_MEASUREMENT_UAB_2           19
//#define DR_OFFSET_MEASUREMENT_UBC_2           20

//#define DR_OFFSET_FREQUENCY_2                 23

#define DR_OFFSET_VMP                         25

#define DR_OFFSET_LABEL_TIME_LSW              26
#define DR_OFFSET_LABEL_TIME_MSW              27
#define DR_OFFSET_LABEL_PROTECT               28

#define BLOCK_PROTECTION_MTZ                  1
#define BLOCK_PROTECTION_3I0                  2
#define BLOCK_PROTECTION_3U0                  3
#define BLOCK_PROTECTION_UMIN                 4
#define BLOCK_PROTECTION_UMAX                 5
#define BLOCK_PROTECTION_ZOP                  6

#define BLOCK_PROTECTION_FREQUENCY_1_ACHR     9
#define BLOCK_PROTECTION_FREQUENCY_1_CAPV     10
#define BLOCK_PROTECTION_FREQUENCY_2_ACHR     11
#define BLOCK_PROTECTION_FREQUENCY_2_CAPV     12
#define BLOCK_PROTECTION_MTZ04                13

#define M_ADDRESS_LAST_MEASUREMENTS_DR        1007
/*****************************************************/

/* Адресний простір вcтановлення ранжування (крів реєстраторів) */
/*****************************************************/
#define M_ADDRESS_FIRST_SETPOINTS_RANG        1016

#define M_ADDRESS_FIRST_USER_REGISTER         1016
#define M_ADDRESS_LAST_USER_REGISTER          1099

#define M_ADDRESS_FIRST_DO_RANG               1100
#define M_ADDRESS_LAST_DO_RANG                1323

#define M_ADDRESS_FIRST_DI_RANG               1324
#define M_ADDRESS_LAST_DI_RANG                1451

#define M_ADDRESS_FIRST_LED_RANG              1452
#define M_ADDRESS_LAST_LED_RANG               1515

#define M_ADDRESS_FIRST_DF_RANG               1516
#define M_ADDRESS_LAST_DF_RANG                1899

#define M_ADDRESS_FIRST_DB_RANG               1900
#define M_ADDRESS_LAST_DB_RANG                1963

#define M_ADDRESS_FIRST_DT_RANG               1964
#define M_ADDRESS_LAST_DT_RANG                2059

#define M_ADDRESS_FIRST_D_AND_RANG            2060
#define M_ADDRESS_LAST_D_AND_RANG             2123

#define M_ADDRESS_FIRST_D_OR_RANG             2124
#define M_ADDRESS_LAST_D_OR_RANG              2187

#define M_ADDRESS_FIRST_D_XOR_RANG            2188
#define M_ADDRESS_LAST_D_XOR_RANG             2203

#define M_ADDRESS_FIRST_D_NOT_RANG            2204
#define M_ADDRESS_LAST_D_NOT_RANG             2219

#define M_ADDRESS_LAST_SETPOINTS_RANG         2219
/*****************************************************/

/*****************************************************/
/* Адресний простір настроювання аналогового реєстратора */
/*****************************************************/
#define M_ADDRESS_FIRST_SETPOINTS_RANG_AR     3000
#define M_ADDRESS_LAST_SETPOINTS_RANG_AR      (M_ADDRESS_FIRST_SETPOINTS_RANG_AR + MAX_FUNCTIONS_IN_AREG - 1)

#define MA_PREFAULT_INTERVAL_AR               3032
#define MA_POSTFAULT_INTERVAL_AR              3033
#define MA_TOTAL_NUMBER_RECORDS_AR            3034
#define MA_CURRENT_NUMBER_RECORD_AR           3035
/*****************************************************/

/*****************************************************/
/* Адресний простір настроювання дискретного реєстратора */
/*****************************************************/
#define M_ADDRESS_FIRST_SETPOINTS_RANG_DR     3036
#define M_ADDRESS_LAST_SETPOINTS_RANG_DR      (M_ADDRESS_FIRST_SETPOINTS_RANG_DR + MAX_FUNCTIONS_IN_DREG - 1)

#define MA_TOTAL_NUMBER_RECORDS_DR            3070
#define MA_CURRENT_NUMBER_RECORD_DR           3071
/*****************************************************/

/*****************************************************/
/* Адресний простір очистки аналогового і дискретного реєстраторів */
/*****************************************************/
#define MA_CLEAR_NUMBER_RECORD_AR     3074
#define CMD_WORD_CLEAR_AR                     0x1312
#define MA_CLEAR_NUMBER_RECORD_DR     3075
#define CMD_WORD_CLEAR_DR                     0x1012
/*****************************************************/

/*****************************************************/
/* Адресний простір реєстратора програмних подій     */
/*****************************************************/
#define MA_CLEAR_NUMBER_RECORD_PR_ERR         61800
#define MA_CURRENT_NUMBER_RECORD_PR_ERR       61801
#define MA_TOTAL_NUMBER_RECORDS_PR_ERR        61802
#define MA_FIRST_ADR_PR_ERR_WINDOW            61803
#define MA_LASR_ADR_PR_ERR_WINDOW             61818
/*****************************************************/

/*****************************************************/
/* Митєві вимірювання розраховані фетодом квадратного кореня суми квадратів миттєвих значень за період */
/*****************************************************/
#define M_ADDRESS_FIRST_MEASUREMENTS_2        61904

#define OFFSET_MEASUREMENT_3I0_I              3

#define M_ADDRESS_LAST_MEASUREMENTS_2         61921
/*****************************************************/

/*****************************************************/
/* Адрексний простір вводу серійного номеру          */
/*****************************************************/
#define MA_SET_SERIAL_NUMBER                  61948
/*****************************************************/

/*****************************************************/
/* Адрексний простір внутрішнього користування       */
/*****************************************************/
#define MA_POSSIBILITY_USTUVANNJA             61967
#define MA_ADDRESS_FIRST_USTUVANNJA           61968
#define MA_ADDRESS_LAST_USTUVANNJA            (MA_ADDRESS_FIRST_USTUVANNJA  + NUMBER_ANALOG_CANALES - 1)
#define MA_ADDRESS_FIRST_PHI_USTUVANNJA       (MA_ADDRESS_LAST_USTUVANNJA + 1)
#define MA_ADDRESS_LAST_PHI_USTUVANNJA        (MA_ADDRESS_FIRST_PHI_USTUVANNJA  + NUMBER_ANALOG_CANALES - 1)
/*****************************************************/

/*****************************************************/
/* Адреса установки максимальної кількості ітерацій для розширеної логіки */
/*****************************************************/
#define MA_NUMBER_ITERATION_EL                62000
/*****************************************************/

/*****************************************************/
/* Адреса подачі команди скинути настройки у мінімальні параметри */
/*****************************************************/
#define MA_DEFAULT_SETTINGS                   62100
#define CMD_WORD_SET_DEFAULT_SETTINGS         0x1111
/*****************************************************/

/*****************************************************/
/* Адреса подачі команди скинути покази технічного обліку електроенергії */
/*****************************************************/
#define MA_CLEAR_ENERGY                       63001
#define CMD_WORD_CLEAR_ENERGY                 0x1234
/*****************************************************/

#define MA_CMD_RESET_RESURS_MIN_MAX           63002
#define MA_LSW_GLOBAL_RESURS_MIN              63003
#define MA_MSW_GLOBAL_RESURS_MIN              63004
#define MA_LSW_GLOBAL_RESURS_MAX              63005
#define MA_MSW_GLOBAL_RESURS_MAX              63006
#define MA_LSW_GLOBAL_RESURS                  63007
#define MA_MSW_GLOBAL_RESURS                  63008
#define MA_LSW_ADR_MEMORY_TO_WRITE            63009
#define MA_MSW_ADR_MEMORY_TO_WRITE            63010
#define MA_LSW_DATA_MEMORY_TO_WRITE           63011
#define MA_MSW_DATA_MEMORY_TO_WRITE           63012
#define MA_NB_REG_FROM_MEM_READ               63013
#define MA_LSW_ADR_MEMORY_TO_READ             63014
#define MA_MSW_ADR_MEMORY_TO_READ             63015

#define M_ADDRESS_FIRST_READ_DAMP_MEM         63016
#define NUMBER_REGISTER_IN_DUMP_MEMORY        128
#define M_ADDRESS_LAST_READ_DAMP_MEM          (M_ADDRESS_FIRST_READ_DAMP_MEM + NUMBER_REGISTER_IN_DUMP_MEMORY)

#define M_ADDRESS_FIRST_TMP_MEASURMENTS       63256
#define NUMBER_REGISTER_TMP_MEASURMENTS       ((NUMBER_ANALOG_CANALES + 3)<<1)
#define M_ADDRESS_LAST_TMP_MEASURMENTS        (M_ADDRESS_FIRST_TMP_MEASURMENTS + NUMBER_REGISTER_TMP_MEASURMENTS)

#define MA_PART_RECEIVE_DIG_OSCILOGRAPH       63303
#define M_ADDRESS_FIRST_DIG_OSCILOGRAPH       63304
#define NUMBER_REGISTER_DIG_OSCILOGRAPH       16
#define M_ADDRESS_LAST_DIG_OSCILOGRAPH        (M_ADDRESS_FIRST_DIG_OSCILOGRAPH + NUMBER_REGISTER_DIG_OSCILOGRAPH)

#define MA_TEST_WATCHDOGS                     63472
#define CMD_TEST_EXTERNAL_WATCHDOG            0xCDEF
/*****************************************************/

#define SOURCE_OUTPUTS_RANG                   0
#define SOURCE_LEDS_RANG                      1
#define SOURCE_DF_PLUS_RANG                   2
#define SOURCE_DF_MINUS_RANG                  3
#define SOURCE_DF_BLK_RANG                    4
#define SOURCE_AR_RANG                        5
#define SOURCE_DR_RANG                        6
#define SOURCE_SET_DT_PLUS_RANG               7
#define SOURCE_SET_DT_MINUS_RANG              8
#define SOURCE_RESET_DT_PLUS_RANG             9
#define SOURCE_RESET_DT_MINUS_RANG            10
#define SOURCE_D_AND_RANG                     11
#define SOURCE_D_OR_RANG                      12
#define SOURCE_D_XOR_RANG                     13
#define SOURCE_D_NOT_RANG                     14

#endif
