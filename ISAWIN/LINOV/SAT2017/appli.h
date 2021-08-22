/**********************************************************************/
/*                                                                    */
/*  File name : appli.H                                               */
/*  Date      : 03.09.2019 11:31:40                                   */
/*  ----------------------------------------------------------------  */
/*  ISaGRAF project name : sat2017                                    */
/*                                                                    */
/**********************************************************************/

/* Global variables of the various programs */

/* Programs */

#define _SCC_PARM_RAM	0X0001
#define _SCC_ANALOG	0X0002
#define _SCC_GASPUMP	0X0003
#define _SCC_CONTROL	0X0004
#define _SCC_TX_X	0X0005
#define _SCC_BLEDING	0X0006
#define _SCC_DEXPO	0X0007
#define _SCC_MUX2	0X0008
#define _SCC_FLIMIT	0X0009
#define _SCC_SLOW_UP	0X000A
#define _SCC_KALMAN	0X000B
#define _SCC_P_P0	0X000C
#define _SCC_PIDMY	0X000D

/* Booleans */

#define _SCC_X_01	0X0001
#define _SCC_X_02	0X0002
#define _SCC_X_03	0X0003
#define _SCC_X_04	0X0004
#define _SCC_X_05	0X0005
#define _SCC_X_06	0X0006
#define _SCC_X_07	0X0007
#define _SCC_X_08	0X0008
#define _SCC_IO0003_08	0X0009
#define _SCC_X_10	0X000A
#define _SCC_X_11	0X000B
#define _SCC_X_12	0X000C
#define _SCC_X_13	0X000D
#define _SCC_X_14	0X000E
#define _SCC_X_15	0X000F
#define _SCC_X_16	0X0010
#define _SCC_Y_01	0X0011
#define _SCC_Y_02	0X0012
#define _SCC_Y_03	0X0013
#define _SCC_Y_04	0X0014
#define _SCC_Y_05	0X0015
#define _SCC_Y_06	0X0016
#define _SCC_Y_07	0X0017
#define _SCC_IO0006_07	0X0018
#define _SCC_Y_09	0X0019
#define _SCC_Y_10	0X001A
#define _SCC_Y_11	0X001B
#define _SCC_Y_12	0X001C
#define _SCC_Y_13	0X001D
#define _SCC_Y_14	0X001E
#define _SCC_Y_15	0X001F
#define _SCC_Y_16	0X0020
#define _SCC_IO0009_00	0X0021
#define _SCC_IO000A_00	0X0022
#define _SCC_IO000A_01	0X0023
#define _SCC_IO000A_02	0X0024
#define _SCC_IO000A_03	0X0025
#define _SCC_IO000D_00	0X0026
#define _SCC_RUN	0X0027
#define _SCC_X_09	0X0028
#define _SCC_Y_08	0X0029
#define _SCC_SAVE	0X002A
#define _SCC_TIME_SET	0X002B
#define _SCC_MODE_01	0X002C
#define _SCC_MODE_10	0X002D
#define _SCC_MODE_02	0X002E
#define _SCC_MODE_11	0X002F
#define _SCC_MODE_03	0X0030
#define _SCC_MODE_20	0X0031
#define _SCC_MODE_12	0X0032
#define _SCC_MODE_04	0X0033
#define _SCC_MODE_21	0X0034
#define _SCC_MODE_13	0X0035
#define _SCC_MODE_05	0X0036
#define _SCC_MODE_14	0X0037
#define _SCC_MODE_06	0X0038
#define _SCC_MODE_15	0X0039
#define _SCC_MODE_07	0X003A
#define _SCC_MODE_16	0X003B
#define _SCC_MODE_08	0X003C
#define _SCC_MODE_17	0X003D
#define _SCC_MODE_09	0X003E
#define _SCC_MODE_18	0X003F
#define _SCC_MODE_19	0X0040
#define _SCC_AM_PUMP_01	0X0041
#define _SCC_M_C_01	0X0042
#define _SCC_M_C_10	0X0043
#define _SCC_M_C_02	0X0044
#define _SCC_M_C_11	0X0045
#define _SCC_M_C_20	0X0046
#define _SCC_M_C_03	0X0047
#define _SCC_M_C_12	0X0048
#define _SCC_M_C_21	0X0049
#define _SCC_M_C_04	0X004A
#define _SCC_M_C_13	0X004B
#define _SCC_M_C_05	0X004C
#define _SCC_M_C_14	0X004D
#define _SCC_M_C_06	0X004E
#define _SCC_M_C_15	0X004F
#define _SCC_M_C_07	0X0050
#define _SCC_M_C_16	0X0051
#define _SCC_M_C_08	0X0052
#define _SCC_M_C_17	0X0053
#define _SCC_M_C_09	0X0054
#define _SCC_M_C_18	0X0055
#define _SCC_M_C_19	0X0056
#define _SCC_AM_01	0X0057
#define _SCC_AM_10	0X0058
#define _SCC_AM_02	0X0059
#define _SCC_AM_11	0X005A
#define _SCC_AM_03	0X005B
#define _SCC_AM_20	0X005C
#define _SCC_AM_12	0X005D
#define _SCC_AM_04	0X005E
#define _SCC_AM_21	0X005F
#define _SCC_AM_13	0X0060
#define _SCC_AM_05	0X0061
#define _SCC_AM_14	0X0062
#define _SCC_AM_06	0X0063
#define _SCC_AM_15	0X0064
#define _SCC_AM_07	0X0065
#define _SCC_AM_16	0X0066
#define _SCC_AM_08	0X0067
#define _SCC_AM_17	0X0068
#define _SCC_AM_09	0X0069
#define _SCC_AM_18	0X006A
#define _SCC_AM_19	0X006B
#define _SCC_REJ_01	0X006D
#define _SCC_REJ_02	0X006E
#define _SCC_EN_P_01	0X006F
#define _SCC_REJ_03	0X0070
#define _SCC_EN_P_02	0X0071
#define _SCC_REJ_12	0X0072
#define _SCC_EN_P_03	0X0073
#define _SCC_EN_P_04	0X0074
#define _SCC_EN_P_05	0X0075
#define _SCC_EN_P_06	0X0076
#define _SCC_REJ_15	0X0077
#define _SCC_REJ_07	0X0078
#define _SCC_REJ_08	0X0079
#define _SCC_REJ_18	0X007A
#define _SCC_AO_12	0X007C
#define _SCC_TMP	0X007D
#define _SCC_CBOO00000001	0X0093
#define _SCC_CBOO00000000	0X0094
#define _SCC_TMPB2	0X0095
#define _SCC_TMPB1	0X0096
#define _SCC_TMPB7	0X0097
#define _SCC_TMPB6	0X0098
#define _SCC_TMPB5	0X0099
#define _SCC_TMPB4	0X009A
#define _SCC_TMPB3	0X009B
#define _SCC_TMPB13	0X009C
#define _SCC_TMPB12	0X009D
#define _SCC_TMPB11	0X009E
#define _SCC_TMPB10	0X009F
#define _SCC_TMPB9	0X00A0
#define _SCC_TMPB8	0X00A1
#define _SCC_TMPB20	0X00A2
#define _SCC_TMPB19	0X00A3
#define _SCC_TMPB18	0X00A4
#define _SCC_TMPB17	0X00A5
#define _SCC_TMPB16	0X00A6
#define _SCC_TMPB15	0X00A7
#define _SCC_TMPB14	0X00A8
#define _SCC_TMPB25	0X00A9
#define _SCC_TMPB24	0X00AA
#define _SCC_TMPB23	0X00AB
#define _SCC_TMPB22	0X00AC
#define _SCC_TMPB21	0X00AD
#define _SCC_DUMMYCOIL	0X00AE
#define _SCC_ED1_6EB18C07_AUTO_O_VA	0X00B7
#define _SCC_ED1_6EB18C01_FIRST_VA	0X00BB
#define _SCC_ED1_6EB18C24_AUTO_O_VA	0X00BD
#define _SCC_ED1_6EB18C22_AUTO_O_VA	0X00BE
#define _SCC_ED1_6EB18C1E_AUTO_O_VA	0X00C1
#define _SCC_ED1_6EB18C1C_AUTO_O_VA	0X00C2
#define _SCC_ED1_6EB18C0A_AUTO_O_VA	0X00D3

/* Analogs */

#define _SCC_IO0000_00	0X0001
#define _SCC_IO0000_01	0X0002
#define _SCC_IO0000_02	0X0003
#define _SCC_IO0000_03	0X0004
#define _SCC_IO0000_04	0X0005
#define _SCC_IO0000_05	0X0006
#define _SCC_IO0000_06	0X0007
#define _SCC_IO0000_07	0X0008
#define _SCC_IO0000_08	0X0009
#define _SCC_IO0000_09	0X000A
#define _SCC_IO0000_0A	0X000B
#define _SCC_IO0000_0B	0X000C
#define _SCC_IO0000_0C	0X000D
#define _SCC_IO0000_0D	0X000E
#define _SCC_IO0000_0E	0X000F
#define _SCC_IO0000_0F	0X0010
#define _SCC_IO0001_00	0X0011
#define _SCC_IO0001_01	0X0012
#define _SCC_IO0001_02	0X0013
#define _SCC_IO0001_03	0X0014
#define _SCC_IO0001_04	0X0015
#define _SCC_IO0001_05	0X0016
#define _SCC_IO0001_06	0X0017
#define _SCC_IO0001_07	0X0018
#define _SCC_IO0001_08	0X0019
#define _SCC_IO0001_09	0X001A
#define _SCC_IO0001_0A	0X001B
#define _SCC_IO0001_0B	0X001C
#define _SCC_IO0001_0C	0X001D
#define _SCC_IO0001_0D	0X001E
#define _SCC_IO0001_0E	0X001F
#define _SCC_IO0001_0F	0X0020
#define _SCC_IO0002_00	0X0021
#define _SCC_IO0002_01	0X0022
#define _SCC_IO0002_02	0X0023
#define _SCC_IO0002_03	0X0024
#define _SCC_IO0002_04	0X0025
#define _SCC_IO0002_05	0X0026
#define _SCC_IO0002_06	0X0027
#define _SCC_IO0004_00	0X0028
#define _SCC_IO0004_01	0X0029
#define _SCC_IO0004_02	0X002A
#define _SCC_IO0004_03	0X002B
#define _SCC_IO0004_04	0X002C
#define _SCC_IO0004_05	0X002D
#define _SCC_IO0004_06	0X002E
#define _SCC_IO0004_07	0X002F
#define _SCC_IO0005_00	0X0030
#define _SCC_IO0005_01	0X0031
#define _SCC_IO0005_02	0X0032
#define _SCC_IO0005_03	0X0033
#define _SCC_IO0005_04	0X0034
#define _SCC_IO0005_05	0X0035
#define _SCC_IO0005_06	0X0036
#define _SCC_IO0005_07	0X0037
#define _SCC_IO0007_00	0X0038
#define _SCC_IO0007_01	0X0039
#define _SCC_IO0007_02	0X003A
#define _SCC_IO0007_03	0X003B
#define _SCC_IO0007_04	0X003C
#define _SCC_IO0007_05	0X003D
#define _SCC_IO0007_06	0X003E
#define _SCC_IO0007_07	0X003F
#define _SCC_XMIN_01	0X0040
#define _SCC_TD_05	0X0041
#define _SCC_XMIN_10	0X0042
#define _SCC_XMIN_02	0X0043
#define _SCC_V_01	0X0044
#define _SCC_TD_14	0X0045
#define _SCC_XMIN_11	0X0046
#define _SCC_V_10	0X0047
#define _SCC_XMAX_01	0X0048
#define _SCC_XMIN_03	0X0049
#define _SCC_XMIN_20	0X004A
#define _SCC_TD_06	0X004B
#define _SCC_TP_ZD_01	0X004C
#define _SCC_XMAX_10	0X004D
#define _SCC_XMIN_12	0X004E
#define _SCC_TP_ZD_02	0X004F
#define _SCC_XMAX_02	0X0050
#define _SCC_XMIN_04	0X0051
#define _SCC_XMIN_21	0X0052
#define _SCC_TP_ZD_03	0X0053
#define _SCC_MM_S	0X0054
#define _SCC_TP_ZD_04	0X0055
#define _SCC_XMAX_11	0X0056
#define _SCC_XMIN_13	0X0057
#define _SCC_TD_15	0X0058
#define _SCC_TP_ZD_05	0X0059
#define _SCC_XMAX_03	0X005A
#define _SCC_XMIN_05	0X005B
#define _SCC_XMAX_20	0X005C
#define _SCC_TP_ZD_06	0X005D
#define _SCC_K2_12	0X005E
#define _SCC_XMAX_12	0X005F
#define _SCC_XMIN_14	0X0060
#define _SCC_TD_07	0X0061
#define _SCC_V_02	0X0062
#define _SCC_XMAX_04	0X0063
#define _SCC_XMIN_06	0X0064
#define _SCC_XMAX_21	0X0065
#define _SCC_V_11	0X0066
#define _SCC_K2_21	0X0067
#define _SCC_XMAX_13	0X0068
#define _SCC_XMIN_15	0X0069
#define _SCC_XMAX_05	0X006A
#define _SCC_XMIN_07	0X006B
#define _SCC_TD_16	0X006C
#define _SCC_V_20	0X006D
#define _SCC_XMAX_14	0X006E
#define _SCC_XMIN_16	0X006F
#define _SCC_XMAX_06	0X0070
#define _SCC_XMIN_08	0X0071
#define _SCC_TD_08	0X0072
#define _SCC_XMIN_17	0X0073
#define _SCC_XMAX_15	0X0074
#define _SCC_XMAX_07	0X0075
#define _SCC_XMIN_09	0X0076
#define _SCC_TD_17	0X0077
#define _SCC_V_03	0X0078
#define _SCC_XMIN_18	0X0079
#define _SCC_XMAX_16	0X007A
#define _SCC_XMAX_08	0X007B
#define _SCC_V_12	0X007C
#define _SCC_TD_09	0X007D
#define _SCC_XMIN_19	0X007E
#define _SCC_XMAX_17	0X007F
#define _SCC_V_21	0X0080
#define _SCC_K3_12	0X0081
#define _SCC_XMAX_09	0X0082
#define _SCC_V_30	0X0083
#define _SCC_XMAX_18	0X0084
#define _SCC_TD_18	0X0085
#define _SCC_K3_21	0X0086
#define _SCC_XMAX_19	0X0087
#define _SCC_V_04	0X0088
#define _SCC_V_13	0X0089
#define _SCC_TD_19	0X008A
#define _SCC_V_22	0X008B
#define _SCC_V_31	0X008C
#define _SCC_K2_08	0X008D
#define _SCC_V_40	0X008E
#define _SCC_K4_12	0X008F
#define _SCC_V_05	0X0090
#define _SCC_V_14	0X0091
#define _SCC_V_23	0X0092
#define _SCC_V_32	0X0093
#define _SCC_CBT	0X0094
#define _SCC_V_41	0X0095
#define _SCC_P0_01	0X0096
#define _SCC_TI_01	0X0097
#define _SCC_V_50	0X0098
#define _SCC_V_06	0X0099
#define _SCC_MON_S	0X009A
#define _SCC_P0_10	0X009B
#define _SCC_V_15	0X009C
#define _SCC_K3_08	0X009D
#define _SCC_TI_10	0X009E
#define _SCC_V_24	0X009F
#define _SCC_P0_02	0X00A0
#define _SCC_TI_02	0X00A1
#define _SCC_V_33	0X00A2
#define _SCC_V_42	0X00A3
#define _SCC_P0_11	0X00A4
#define _SCC_TI_11	0X00A5
#define _SCC_V_07	0X00A6
#define _SCC_P0_03	0X00A7
#define _SCC_TI_03	0X00A8
#define _SCC_V_16	0X00A9
#define _SCC_TI_20	0X00AA
#define _SCC_V_25	0X00AB
#define _SCC_P0_12	0X00AC
#define _SCC_TI_12	0X00AD
#define _SCC_V_34	0X00AE
#define _SCC_P0_04	0X00AF
#define _SCC_V_43	0X00B0
#define _SCC_TI_04	0X00B1
#define _SCC_TI_21	0X00B2
#define _SCC_V_08	0X00B3
#define _SCC_P0_13	0X00B4
#define _SCC_TI_13	0X00B5
#define _SCC_V_17	0X00B6
#define _SCC_P0_05	0X00B7
#define _SCC_V_26	0X00B8
#define _SCC_TI_05	0X00B9
#define _SCC_V_35	0X00BA
#define _SCC_P0_14	0X00BB
#define _SCC_V_44	0X00BC
#define _SCC_TI_14	0X00BD
#define _SCC_P0_06	0X00BE
#define _SCC_V_09	0X00BF
#define _SCC_TI_06	0X00C0
#define _SCC_V_18	0X00C1
#define _SCC_P0_15	0X00C2
#define _SCC_V_27	0X00C3
#define _SCC_TI_15	0X00C4
#define _SCC_V_36	0X00C5
#define _SCC_P0_07	0X00C6
#define _SCC_TI_07	0X00C7
#define _SCC_V_45	0X00C8
#define _SCC_P0_16	0X00C9
#define _SCC_TI_16	0X00CA
#define _SCC_V_19	0X00CB
#define _SCC_P0_08	0X00CC
#define _SCC_TI_08	0X00CD
#define _SCC_V_28	0X00CE
#define _SCC_V_37	0X00CF
#define _SCC_P0_17	0X00D0
#define _SCC_TI_17	0X00D1
#define _SCC_V_46	0X00D2
#define _SCC_P0_09	0X00D3
#define _SCC_TI_09	0X00D4
#define _SCC_KPR_01	0X00D5
#define _SCC_CB_01	0X00D6
#define _SCC_P0_18	0X00D7
#define _SCC_KPR_10	0X00D8
#define _SCC_TI_18	0X00D9
#define _SCC_V_29	0X00DA
#define _SCC_KPR_02	0X00DB
#define _SCC_V_38	0X00DC
#define _SCC_KPR_11	0X00DD
#define _SCC_V_47	0X00DE
#define _SCC_CB_02	0X00DF
#define _SCC_KPR_20	0X00E0
#define _SCC_P0_19	0X00E1
#define _SCC_TI_19	0X00E2
#define _SCC_KPR_03	0X00E3
#define _SCC_KPR_12	0X00E4
#define _SCC_KPR_21	0X00E5
#define _SCC_CB_03	0X00E6
#define _SCC_KPR_04	0X00E7
#define _SCC_V_39	0X00E8
#define _SCC_KPR_13	0X00E9
#define _SCC_V_48	0X00EA
#define _SCC_KPR_05	0X00EB
#define _SCC_CB_04	0X00EC
#define _SCC_KPR_14	0X00ED
#define _SCC_KPR_06	0X00EE
#define _SCC_KPR_15	0X00EF
#define _SCC_C_01	0X00F0
#define _SCC_CB_05	0X00F1
#define _SCC_V_49	0X00F2
#define _SCC_KPR_07	0X00F3
#define _SCC_C_10	0X00F4
#define _SCC_KR_P_01	0X00F5
#define _SCC_KPR_16	0X00F6
#define _SCC_KR_P_02	0X00F7
#define _SCC_KPR_08	0X00F8
#define _SCC_CB_06	0X00F9
#define _SCC_KPR_17	0X00FA
#define _SCC_KR_P_03	0X00FB
#define _SCC_C_02	0X00FC
#define _SCC_KPR_09	0X00FD
#define _SCC_KR_P_04	0X00FE
#define _SCC_C_11	0X00FF
#define _SCC_KPR_18	0X0100
#define _SCC_SP_01	0X0101
#define _SCC_C_20	0X0102
#define _SCC_KR_P_05	0X0103
#define _SCC_KPR_19	0X0104
#define _SCC_KR_P_06	0X0105
#define _SCC_SP_10	0X0106
#define _SCC_SP_02	0X0107
#define _SCC_C_03	0X0108
#define _SCC_C_12	0X0109
#define _SCC_SP_11	0X010A
#define _SCC_C_21	0X010B
#define _SCC_SP_03	0X010C
#define _SCC_SP_12	0X010D
#define _SCC_C_04	0X010E
#define _SCC_C_13	0X010F
#define _SCC_SP_04	0X0110
#define _SCC_SP_21	0X0111
#define _SCC_C_22	0X0112
#define _SCC_SP_13	0X0113
#define _SCC_SP_05	0X0114
#define _SCC_C_05	0X0115
#define _SCC_DD_S	0X0116
#define _SCC_C_14	0X0117
#define _SCC_SP_14	0X0118
#define _SCC_SP_06	0X0119
#define _SCC_SP_15	0X011A
#define _SCC_C_06	0X011B
#define _SCC_SP_07	0X011C
#define _SCC_C_15	0X011D
#define _SCC_SP_16	0X011E
#define _SCC_SP_08	0X011F
#define _SCC_C_07	0X0120
#define _SCC_SP_17	0X0121
#define _SCC_C_16	0X0122
#define _SCC_SP_09	0X0123
#define _SCC_SP_18	0X0124
#define _SCC_MAX_V_17	0X0125
#define _SCC_C_08	0X0126
#define _SCC_C_17	0X0127
#define _SCC_SP_19	0X0128
#define _SCC_MIN_V_17	0X0129
#define _SCC_SS_S	0X012A
#define _SCC_C_09	0X012B
#define _SCC_C_18	0X012C
#define _SCC_C_19	0X012D
#define _SCC_SPR_01	0X012E
#define _SCC_SPR_02	0X012F
#define _SCC_SPR_03	0X0130
#define _SCC_SPR_12	0X0131
#define _SCC_SPR_15	0X0132
#define _SCC_K_01	0X0133
#define _SCC_SPR_07	0X0134
#define _SCC_SPR_08	0X0135
#define _SCC_K_02	0X0136
#define _SCC_K_03	0X0137
#define _SCC_K_12	0X0138
#define _SCC_F_V_01	0X0139
#define _SCC_F_V_10	0X013A
#define _SCC_HH_S	0X013B
#define _SCC_F_V_02	0X013C
#define _SCC_F_V_11	0X013D
#define _SCC_F_V_20	0X013E
#define _SCC_F_V_03	0X013F
#define _SCC_F_V_12	0X0140
#define _SCC_F_V_21	0X0141
#define _SCC_F_V_04	0X0142
#define _SCC_KKOR_01	0X0143
#define _SCC_F_V_30	0X0144
#define _SCC_F_V_13	0X0145
#define _SCC_KKOR_02	0X0146
#define _SCC_F_V_22	0X0147
#define _SCC_F_V_05	0X0148
#define _SCC_F_V_31	0X0149
#define _SCC_KKOR_03	0X014A
#define _SCC_F_V_14	0X014B
#define _SCC_KKOR_12	0X014C
#define _SCC_F_V_23	0X014D
#define _SCC_F_V_06	0X014E
#define _SCC_K_15	0X014F
#define _SCC_F_V_32	0X0150
#define _SCC_F_V_15	0X0151
#define _SCC_F_V_24	0X0152
#define _SCC_F_V_07	0X0153
#define _SCC_F_V_33	0X0154
#define _SCC_F_V_16	0X0155
#define _SCC_KKOR_15	0X0156
#define _SCC_KKOR_07	0X0157
#define _SCC_F_V_25	0X0158
#define _SCC_K_07	0X0159
#define _SCC_F_V_08	0X015A
#define _SCC_F_V_34	0X015B
#define _SCC_KKOR_08	0X015C
#define _SCC_SP2_12	0X015D
#define _SCC_F_V_17	0X015E
#define _SCC_F_V_26	0X015F
#define _SCC_F_V_09	0X0160
#define _SCC_F_V_35	0X0161
#define _SCC_F_V_18	0X0162
#define _SCC_F_V_27	0X0163
#define _SCC_K_08	0X0164
#define _SCC_F_V_36	0X0165
#define _SCC_F_V_19	0X0166
#define _SCC_F_V_28	0X0167
#define _SCC_F_V_37	0X0168
#define _SCC_SP2_15	0X0169
#define _SCC_F_V_29	0X016A
#define _SCC_F_V_38	0X016B
#define _SCC_F_V_39	0X016C
#define _SCC_SP2_18	0X016D
#define _SCC_YY_S	0X016E
#define _SCC_TM_ZD_01	0X016F
#define _SCC_TM_ZD_02	0X0170
#define _SCC_TM_ZD_03	0X0171
#define _SCC_TM_ZD_04	0X0172
#define _SCC_TM_ZD_05	0X0173
#define _SCC_TM_ZD_06	0X0174
#define _SCC_TD_01	0X0175
#define _SCC_TD_10	0X0176
#define _SCC_TD_02	0X0177
#define _SCC_TD_11	0X0178
#define _SCC_TD_03	0X0179
#define _SCC_TD_20	0X017A
#define _SCC_TD_12	0X017B
#define _SCC_TD_04	0X017C
#define _SCC_TD_21	0X017D
#define _SCC_TD_13	0X017E
#define _SCC_XT_06	0X0181
#define _SCC_XT_05	0X0182
#define _SCC_XT_04	0X0183
#define _SCC_XT_03	0X0184
#define _SCC_XT_02	0X0185
#define _SCC_XT_01	0X0186
#define _SCC_TC_21	0X0187
#define _SCC_TC_20	0X0188
#define _SCC_XT_20	0X0189
#define _SCC_XT_19	0X018A
#define _SCC_XT_18	0X018B
#define _SCC_XT_17	0X018C
#define _SCC_XT_16	0X018D
#define _SCC_XT_15	0X018E
#define _SCC_XT_14	0X018F
#define _SCC_XT_13	0X0190
#define _SCC_XT_12	0X0191
#define _SCC_XT_11	0X0192
#define _SCC_XT_10	0X0193
#define _SCC_XT_09	0X0194
#define _SCC_XT_08	0X0195
#define _SCC_XT_07	0X0196
#define _SCC_TSP_12	0X0197
#define _SCC_TPV_12	0X0198
#define _SCC_XT_21	0X0199
#define _SCC_TIME_S	0X019A
#define _SCC_V_00	0X019B
#define _SCC_CANA000003E8	0X01D8
#define _SCC_CANA00000031	0X01D9
#define _SCC_CANA00000030	0X01DA
#define _SCC_CANA0000110C	0X01DB
#define _SCC_CANA00000003	0X01DC
#define _SCC_CANA00001101	0X01DD
#define _SCC_CANA00000004	0X01DE
#define _SCC_CANA0000000A	0X01DF
#define _SCC_CANA00002100	0X01E0
#define _SCC_CANA00002001	0X01E1
#define _SCC_CANA0000003C	0X01E2
#define _SCC_CANA0000000D	0X01E3
#define _SCC_CANA00000006	0X01E4
#define _SCC_CANA00000005	0X01E5
#define _SCC_CANA00000007	0X01E6
#define _SCC_CANA00000008	0X01E7
#define _SCC_CANA000002E0	0X01E8
#define _SCC_CANA000002E9	0X01E9
#define _SCC_CANA000002FA	0X01EA
#define _SCC_CANA000002FF	0X01EB
#define _SCC_CANA000000BA	0X01EC
#define _SCC_CANA000000BE	0X01ED
#define _SCC_CANA00000300	0X01EE
#define _SCC_CANA0000030E	0X01EF
#define _SCC_CANA0000026F	0X01F0
#define _SCC_CANA000002AC	0X01F1
#define _SCC_CANA000000B8	0X01F2
#define _SCC_CANA000002B6	0X01F3
#define _SCC_CANA000002DE	0X01F4
#define _SCC_CANA00000320	0X01F5
#define _SCC_CANA0000032C	0X01F6
#define _SCC_CANA000002EA	0X01F7
#define _SCC_CANA000002F8	0X01F8
#define _SCC_CANA0000012C	0X01F9
#define _SCC_CANA00000234	0X01FA
#define _SCC_CANA00000236	0X01FB
#define _SCC_CANA0000026E	0X01FC
#define _SCC_CANA000000B4	0X01FD
#define _SCC_CANA00000000	0X01FE
#define _SCC_CANA00000001	0X01FF
#define _SCC_CANA00000078	0X0200
#define _SCC_CANA00000096	0X0201
#define _SCC_CANA00000002	0X0202
#define _SCC_REAL_NULL	0X0203
#define _SCC_FBL_RET	0X0216
#define _SCC_TMPI12	0X0217
#define _SCC_TMPI9	0X0218
#define _SCC_TMPI6	0X0219
#define _SCC_TMPI5	0X021A
#define _SCC_TMPI21	0X021B
#define _SCC_TMPI18	0X021C
#define _SCC_TMPI15	0X021D
#define _SCC_TMPI30	0X021E
#define _SCC_TMPI27	0X021F
#define _SCC_TMPI24	0X0220
#define _SCC_TMPI39	0X0221
#define _SCC_TMPI36	0X0222
#define _SCC_TMPI33	0X0223
#define _SCC_TMPR1	0X0224
#define _SCC_ED1_6EB18A72_STATE_VA	0X0225
#define _SCC_ED1_6EB18A72_STATE__VA	0X0226
#define _SCC_TMPR4	0X0228
#define _SCC_TMPI7	0X0264
#define _SCC_ED1_6EB18C07_KP_O_VA	0X0265
#define _SCC_ED1_6EB18C07_X0_O_VA	0X0266
#define _SCC_ED1_6EB18C07_SP_O_VA	0X0267
#define _SCC_ED1_6EB18C07_PV_O_VA	0X0268
#define _SCC_ED1_6EB18C2B_KE__VA	0X026A
#define _SCC_ED1_6EB18C2B_SUM__VA	0X026B
#define _SCC_ED1_6EB18C2B_XT__VA	0X026C
#define _SCC_ED1_6EB18C2B_SUM0__VA	0X026D
#define _SCC_ED1_6EB18C29_KE__VA	0X0270
#define _SCC_ED1_6EB18C29_SUM__VA	0X0271
#define _SCC_ED1_6EB18C29_XT__VA	0X0272
#define _SCC_ED1_6EB18C29_SUM0__VA	0X0273
#define _SCC_ED1_6EB18C29_XOUT__VA	0X0274
#define _SCC_ED1_6EB18C40_KE__VA	0X0278
#define _SCC_ED1_6EB18C40_SUM__VA	0X0279
#define _SCC_ED1_6EB18C40_XT__VA	0X027A
#define _SCC_ED1_6EB18C40_SUM0__VA	0X027B
#define _SCC_ED1_6EB18C40_XOUT__VA	0X027C
#define _SCC_ED1_6EB18C01_LOCOUT_VA	0X0280
#define _SCC_ED1_6EB18C01_DELTA_VA	0X0281
#define _SCC_ED1_6EB18C01_OUT__VA	0X0282
#define _SCC_ED1_6EB18C24_KP_O_VA	0X0283
#define _SCC_ED1_6EB18C24_X0_O_VA	0X0284
#define _SCC_ED1_6EB18C24_SP_O_VA	0X0285
#define _SCC_ED1_6EB18C24_PV_O_VA	0X0286
#define _SCC_ED1_6EB18C3E_KE__VA	0X0287
#define _SCC_ED1_6EB18C3E_SUM__VA	0X0288
#define _SCC_ED1_6EB18C3E_XT__VA	0X0289
#define _SCC_ED1_6EB18C3E_SUM0__VA	0X028A
#define _SCC_ED1_6EB18C3E_XOUT__VA	0X028B
#define _SCC_ED1_6EB18C22_X0_O_VA	0X029C
#define _SCC_ED1_6EB18C22_SP_O_VA	0X029D
#define _SCC_ED1_6EB18C22_PV_O_VA	0X029E
#define _SCC_ED1_6EB18C3C_SUM0__VA	0X02A0
#define _SCC_ED1_6EB18C3C_XOUT__VA	0X02A1
#define _SCC_ED1_6EB18C22_KP_O_VA	0X02B2
#define _SCC_ED1_6EB18C3C_KE__VA	0X02B4
#define _SCC_ED1_6EB18C3C_SUM__VA	0X02B5
#define _SCC_ED1_6EB18C3C_XT__VA	0X02B6
#define _SCC_TMPR6	0X02B7
#define _SCC_TMPI2	0X02B8
#define _SCC_TMPR2	0X02B9
#define _SCC_ED1_6EB18C3A_KE__VA	0X02BC
#define _SCC_ED1_6EB18C3A_SUM__VA	0X02BD
#define _SCC_ED1_6EB18C3A_XT__VA	0X02BE
#define _SCC_ED1_6EB18C3A_SUM0__VA	0X02BF
#define _SCC_ED1_6EB18C3A_XOUT__VA	0X02C0
#define _SCC_ED1_6EB18C1E_KP_O_VA	0X02C1
#define _SCC_ED1_6EB18C1E_X0_O_VA	0X02C2
#define _SCC_ED1_6EB18C1E_SP_O_VA	0X02C3
#define _SCC_ED1_6EB18C1E_PV_O_VA	0X02C4
#define _SCC_ED1_6EB18C38_KE__VA	0X02C5
#define _SCC_ED1_6EB18C38_SUM__VA	0X02C6
#define _SCC_ED1_6EB18C38_XT__VA	0X02C7
#define _SCC_ED1_6EB18C38_SUM0__VA	0X02C8
#define _SCC_ED1_6EB18C38_XOUT__VA	0X02C9
#define _SCC_ED1_6EB18C1C_KP_O_VA	0X02CC
#define _SCC_ED1_6EB18C1C_X0_O_VA	0X02CD
#define _SCC_ED1_6EB18C1C_SP_O_VA	0X02CE
#define _SCC_ED1_6EB18C1C_PV_O_VA	0X02CF
#define _SCC_ED1_6EB18C36_KE__VA	0X02D1
#define _SCC_ED1_6EB18C36_SUM__VA	0X02D2
#define _SCC_ED1_6EB18C36_XT__VA	0X02D3
#define _SCC_ED1_6EB18C36_SUM0__VA	0X02D4
#define _SCC_ED1_6EB18C36_XOUT__VA	0X02D5
#define _SCC_ED1_6EB18C34_KE__VA	0X02D7
#define _SCC_ED1_6EB18C34_SUM__VA	0X02D8
#define _SCC_ED1_6EB18C34_XT__VA	0X02D9
#define _SCC_ED1_6EB18C34_SUM0__VA	0X02DA
#define _SCC_ED1_6EB18C34_XOUT__VA	0X02DB
#define _SCC_ED1_6EB18C32_SUM__VA	0X02DC
#define _SCC_ED1_6EB18C32_XT__VA	0X02DD
#define _SCC_ED1_6EB18C32_SUM0__VA	0X02DE
#define _SCC_ED1_6EB18C32_XOUT__VA	0X02DF
#define _SCC_ED1_6EB18C32_KE__VA	0X02E1
#define _SCC_ED1_6EB18C30_KE__VA	0X02E3
#define _SCC_ED1_6EB18C30_SUM__VA	0X02E4
#define _SCC_ED1_6EB18C30_XT__VA	0X02E5
#define _SCC_ED1_6EB18C30_SUM0__VA	0X02E6
#define _SCC_ED1_6EB18C30_XOUT__VA	0X02E7
#define _SCC_ED1_6EB18C2E_KE__VA	0X02E9
#define _SCC_ED1_6EB18C2E_SUM__VA	0X02EA
#define _SCC_ED1_6EB18C2E_XT__VA	0X02EB
#define _SCC_ED1_6EB18C2E_SUM0__VA	0X02EC
#define _SCC_ED1_6EB18C2E_XOUT__VA	0X02ED
#define _SCC_ED1_6EB18C0A_KP_O_VA	0X02EE
#define _SCC_ED1_6EB18C0A_X0_O_VA	0X02EF
#define _SCC_ED1_6EB18C0A_SP_O_VA	0X02F0
#define _SCC_ED1_6EB18C0A_PV_O_VA	0X02F1
#define _SCC_ED1_6EB18C2C_SUM0__VA	0X02F2
#define _SCC_ED1_6EB18C2C_XOUT__VA	0X02F3
#define _SCC_ED1_6EB18C2B_XOUT__VA	0X02F6
#define _SCC_ED1_6EB18C2C_KE__VA	0X02F7
#define _SCC_ED1_6EB18C2C_SUM__VA	0X02F8
#define _SCC_ED1_6EB18C2C_XT__VA	0X02F9
#define _SCC_TMPI3	0X02FA
#define _SCC_TMPR3	0X02FD

/* Timers */

#define _SCC_CTMR00000000	0X0008
#define _SCC_CTMR00001388	0X0009
#define _SCC_CTMR000001F4	0X000A
#define _SCC_CTMR000003E8	0X000B
#define _SCC_CTMR000000FA	0X000C
#define _SCC_TMPT5	0X000D
#define _SCC_TMPT1	0X000E

/* Messages */


/* Functions */

#define _SCC_RETAIN_X	0X0001
#define _SCC_V0_10_TO	0X0002
#define _SCC_A4_20_TO	0X0003
#define _SCC_INT_REAL	0X0004
#define _SCC_TO_A4_20	0X0005

/* Function blocks */

#define _SCC_STD_R_TRIG	0X0004

#define _SCC_STD_TON	0X0009

#define _SCC_STD_DERIVATE	0X000F

#define _SCC_STD_AVERAGE	0X0011

#define _SCC_STD_BLINK	0X0013

#define _SCC_EBUS_F_R	0X0015

#define _SCC_SYSTIM_R	0X0016

#define _SCC_SYSDAT_R	0X0017

#define _SCC_SYSTIM_W	0X0018

#define _SCC_SYSDAT_W	0X0019

#define _SCC_EBUS_N_R	0X001A

#define _SCC_MBUS_N_W	0X001B

#define _SCC_MBUS_R	0X001C

#define _SCC_WD_BIT	0X001D

#define _SCC_PID_AL	0X001E