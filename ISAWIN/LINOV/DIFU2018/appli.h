/**********************************************************************/
/*                                                                    */
/*  File name : appli.H                                               */
/*  Date      : 16.09.2018 11:26:13                                   */
/*  ----------------------------------------------------------------  */
/*  ISaGRAF project name : difu2018                                   */
/*                                                                    */
/**********************************************************************/

/* Global variables of the various programs */

/* Programs */

#define _SCC_PARM_RAM	0X0001
#define _SCC_ANALOG	0X0002
#define _SCC_CONTROL	0X0003
#define _SCC_TX_X	0X0004
#define _SCC_MUX2	0X0005
#define _SCC_P_P0	0X0006

/* Booleans */

#define _SCC_X_01	0X0001
#define _SCC_X_02	0X0002
#define _SCC_X_03	0X0003
#define _SCC_X_04	0X0004
#define _SCC_X_05	0X0005
#define _SCC_X_06	0X0006
#define _SCC_X_07	0X0007
#define _SCC_X_08	0X0008
#define _SCC_X_09	0X0009
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
#define _SCC_Y_08	0X0018
#define _SCC_Y_09	0X0019
#define _SCC_Y_10	0X001A
#define _SCC_Y_11	0X001B
#define _SCC_Y_12	0X001C
#define _SCC_Y_13	0X001D
#define _SCC_Y_14	0X001E
#define _SCC_Y_15	0X001F
#define _SCC_Y_16	0X0020
#define _SCC_IO000A_00	0X0021
#define _SCC_IO000C_00	0X0022
#define _SCC_X_20	0X0023
#define _SCC_X_21	0X0024
#define _SCC_X_30	0X0025
#define _SCC_W0001	0X0026
#define _SCC_X_22	0X0027
#define _SCC_X_31	0X0028
#define _SCC_W0002	0X0029
#define _SCC_RUN	0X002A
#define _SCC_X_23	0X002B
#define _SCC_X_24	0X002C
#define _SCC_X_25	0X002D
#define _SCC_REV_01	0X002E
#define _SCC_REV_10	0X002F
#define _SCC_REV_02	0X0030
#define _SCC_REV_11	0X0031
#define _SCC_X_17	0X0032
#define _SCC_REV_03	0X0033
#define _SCC_X_26	0X0034
#define _SCC_REV_12	0X0035
#define _SCC_REV_04	0X0036
#define _SCC_X_18	0X0037
#define _SCC_REV_05	0X0038
#define _SCC_X_27	0X0039
#define _SCC_REV_06	0X003A
#define _SCC_REV_07	0X003B
#define _SCC_X_19	0X003C
#define _SCC_X_28	0X003D
#define _SCC_REV_08	0X003E
#define _SCC_SAVE	0X003F
#define _SCC_TIME_SET	0X0040
#define _SCC_REV_09	0X0041
#define _SCC_X_29	0X0042
#define _SCC_MODE_01	0X0043
#define _SCC_MODE_10	0X0044
#define _SCC_MODE_02	0X0045
#define _SCC_MODE_11	0X0046
#define _SCC_MODE_03	0X0047
#define _SCC_MODE_12	0X0048
#define _SCC_MODE_04	0X0049
#define _SCC_MODE_05	0X004A
#define _SCC_MODE_06	0X004B
#define _SCC_MODE_07	0X004C
#define _SCC_MODE_08	0X004D
#define _SCC_MODE_09	0X004E
#define _SCC_AM_01	0X004F
#define _SCC_AM_10	0X0050
#define _SCC_AM_02	0X0051
#define _SCC_AM_11	0X0052
#define _SCC_AM_03	0X0053
#define _SCC_AM_12	0X0054
#define _SCC_AM_04	0X0055
#define _SCC_AM_05	0X0056
#define _SCC_AM_06	0X0057
#define _SCC_AM_07	0X0058
#define _SCC_AM_08	0X0059
#define _SCC_AM_09	0X005A
#define _SCC_REJ_06	0X005C
#define _SCC_DIO_2	0X005E
#define _SCC_DIO_1	0X005F
#define _SCC_DIO_12_1	0X0060
#define _SCC_DIO_12_0	0X0061
#define _SCC_DIO_9	0X0062
#define _SCC_DIO_8	0X0063
#define _SCC_DIO_7	0X0064
#define _SCC_DIO_6	0X0065
#define _SCC_DIO_5	0X0066
#define _SCC_DIO_4	0X0067
#define _SCC_DIO_3	0X0068
#define _SCC_CBOO00000001	0X0070
#define _SCC_CBOO00000000	0X0071
#define _SCC_DUMMYCOIL	0X0072
#define _SCC_TMPB2	0X0073
#define _SCC_TMPB1	0X0074
#define _SCC_TMPB7	0X0075
#define _SCC_TMPB6	0X0076
#define _SCC_TMPB5	0X0077
#define _SCC_TMPB4	0X0078
#define _SCC_TMPB3	0X0079
#define _SCC_TMPB13	0X007A
#define _SCC_TMPB12	0X007B
#define _SCC_TMPB11	0X007C
#define _SCC_TMPB10	0X007D
#define _SCC_TMPB9	0X007E
#define _SCC_TMPB8	0X007F
#define _SCC_TMPB19	0X0080
#define _SCC_TMPB18	0X0081
#define _SCC_TMPB17	0X0082
#define _SCC_TMPB16	0X0083
#define _SCC_TMPB15	0X0084
#define _SCC_TMPB14	0X0085
#define _SCC_ED1_76132006_AUTO_O_VA	0X0095
#define _SCC_ED1_76132007_AUTO_O_VA	0X0096
#define _SCC_ED1_76132005_AUTO_O_VA	0X0097
#define _SCC_ED1_76132004_AUTO_O_VA	0X0098
#define _SCC_ED1_76132003_AUTO_O_VA	0X0099
#define _SCC_ED1_76132001_AUTO_O_VA	0X009A
#define _SCC_ED1_76132002_AUTO_O_VA	0X009B
#define _SCC_ED1_7613200B_AUTO_O_VA	0X009C
#define _SCC_ED1_7613200C_AUTO_O_VA	0X009D
#define _SCC_ED1_7613200A_AUTO_O_VA	0X009E
#define _SCC_ED1_76132009_AUTO_O_VA	0X009F
#define _SCC_ED1_76132008_AUTO_O_VA	0X00A0

/* Analogs */

#define _SCC_IO0009_00	0X0001
#define _SCC_XMIN_01	0X0002
#define _SCC_TD_05	0X0003
#define _SCC_XMIN_10	0X0004
#define _SCC_XMIN_02	0X0005
#define _SCC_V_01	0X0006
#define _SCC_XMIN_11	0X0007
#define _SCC_V_10	0X0008
#define _SCC_XMAX_01	0X0009
#define _SCC_XMIN_03	0X000A
#define _SCC_TD_06	0X000B
#define _SCC_XMAX_10	0X000C
#define _SCC_XMIN_12	0X000D
#define _SCC_XMAX_02	0X000E
#define _SCC_XMIN_04	0X000F
#define _SCC_MM_S	0X0010
#define _SCC_XMAX_11	0X0011
#define _SCC_XMAX_03	0X0012
#define _SCC_XMIN_05	0X0013
#define _SCC_K1_06	0X0014
#define _SCC_XMAX_12	0X0015
#define _SCC_TD_07	0X0016
#define _SCC_V_02	0X0017
#define _SCC_XMAX_04	0X0018
#define _SCC_XMIN_06	0X0019
#define _SCC_V_11	0X001A
#define _SCC_XMAX_05	0X001B
#define _SCC_XMIN_07	0X001C
#define _SCC_V_20	0X001D
#define _SCC_XMAX_06	0X001E
#define _SCC_XMIN_08	0X001F
#define _SCC_TD_08	0X0020
#define _SCC_XMAX_07	0X0021
#define _SCC_XMIN_09	0X0022
#define _SCC_V_03	0X0023
#define _SCC_XMAX_08	0X0024
#define _SCC_V_12	0X0025
#define _SCC_TD_09	0X0026
#define _SCC_V_21	0X0027
#define _SCC_XMAX_09	0X0028
#define _SCC_V_04	0X0029
#define _SCC_V_13	0X002A
#define _SCC_V_22	0X002B
#define _SCC_V_05	0X002C
#define _SCC_V_14	0X002D
#define _SCC_K2_09	0X002E
#define _SCC_V_23	0X002F
#define _SCC_P0_01	0X0030
#define _SCC_TI_01	0X0031
#define _SCC_V_06	0X0032
#define _SCC_V0001	0X0033
#define _SCC_MON_S	0X0034
#define _SCC_P0_10	0X0035
#define _SCC_V_15	0X0036
#define _SCC_TI_10	0X0037
#define _SCC_V_24	0X0038
#define _SCC_V0010	0X0039
#define _SCC_P0_02	0X003A
#define _SCC_TI_02	0X003B
#define _SCC_V0002	0X003C
#define _SCC_P0_11	0X003D
#define _SCC_K3_09	0X003E
#define _SCC_TI_11	0X003F
#define _SCC_V_07	0X0040
#define _SCC_V0011	0X0041
#define _SCC_P0_03	0X0042
#define _SCC_TI_03	0X0043
#define _SCC_V_16	0X0044
#define _SCC_V0003	0X0045
#define _SCC_V_25	0X0046
#define _SCC_V0020	0X0047
#define _SCC_P0_12	0X0048
#define _SCC_TI_12	0X0049
#define _SCC_V0012	0X004A
#define _SCC_P0_04	0X004B
#define _SCC_TI_04	0X004C
#define _SCC_V0004	0X004D
#define _SCC_V_08	0X004E
#define _SCC_V0021	0X004F
#define _SCC_V_17	0X0050
#define _SCC_V0013	0X0051
#define _SCC_P0_05	0X0052
#define _SCC_V0030	0X0053
#define _SCC_TI_05	0X0054
#define _SCC_V0005	0X0055
#define _SCC_V0022	0X0056
#define _SCC_V0014	0X0057
#define _SCC_P0_06	0X0058
#define _SCC_V_09	0X0059
#define _SCC_V0031	0X005A
#define _SCC_TI_06	0X005B
#define _SCC_V_18	0X005C
#define _SCC_V0006	0X005D
#define _SCC_V0023	0X005E
#define _SCC_V0040	0X005F
#define _SCC_V0015	0X0060
#define _SCC_P0_07	0X0061
#define _SCC_V0032	0X0062
#define _SCC_TI_07	0X0063
#define _SCC_V0007	0X0064
#define _SCC_V0024	0X0065
#define _SCC_V0041	0X0066
#define _SCC_V_19	0X0067
#define _SCC_V0016	0X0068
#define _SCC_P0_08	0X0069
#define _SCC_V0033	0X006A
#define _SCC_TI_08	0X006B
#define _SCC_V0050	0X006C
#define _SCC_V0008	0X006D
#define _SCC_V0025	0X006E
#define _SCC_V0042	0X006F
#define _SCC_V0017	0X0070
#define _SCC_P0_09	0X0071
#define _SCC_V0034	0X0072
#define _SCC_TI_09	0X0073
#define _SCC_KPR_01	0X0074
#define _SCC_V0009	0X0075
#define _SCC_V0026	0X0076
#define _SCC_KPR_10	0X0077
#define _SCC_V0043	0X0078
#define _SCC_V0018	0X0079
#define _SCC_KPR_02	0X007A
#define _SCC_V0035	0X007B
#define _SCC_KPR_11	0X007C
#define _SCC_V0027	0X007D
#define _SCC_V0044	0X007E
#define _SCC_KPR_03	0X007F
#define _SCC_V0019	0X0080
#define _SCC_KPR_12	0X0081
#define _SCC_V0036	0X0082
#define _SCC_KPR_04	0X0083
#define _SCC_V0028	0X0084
#define _SCC_V0045	0X0085
#define _SCC_V0037	0X0086
#define _SCC_KPR_05	0X0087
#define _SCC_V0029	0X0088
#define _SCC_V0046	0X0089
#define _SCC_KPR_06	0X008A
#define _SCC_V0038	0X008B
#define _SCC_C_01	0X008C
#define _SCC_KPR_07	0X008D
#define _SCC_C_10	0X008E
#define _SCC_V0047	0X008F
#define _SCC_V0039	0X0090
#define _SCC_KPR_08	0X0091
#define _SCC_V0048	0X0092
#define _SCC_C_02	0X0093
#define _SCC_KPR_09	0X0094
#define _SCC_C_11	0X0095
#define _SCC_SP_01	0X0096
#define _SCC_V0049	0X0097
#define _SCC_SP_10	0X0098
#define _SCC_SP_02	0X0099
#define _SCC_C_03	0X009A
#define _SCC_C_12	0X009B
#define _SCC_SP_11	0X009C
#define _SCC_SP_03	0X009D
#define _SCC_SP_12	0X009E
#define _SCC_C_04	0X009F
#define _SCC_SP_04	0X00A0
#define _SCC_SP_05	0X00A1
#define _SCC_C_05	0X00A2
#define _SCC_DD_S	0X00A3
#define _SCC_SP_06	0X00A4
#define _SCC_C_06	0X00A5
#define _SCC_SP_07	0X00A6
#define _SCC_SP_08	0X00A7
#define _SCC_C_07	0X00A8
#define _SCC_SP_09	0X00A9
#define _SCC_C_08	0X00AA
#define _SCC_SS_S	0X00AB
#define _SCC_C_09	0X00AC
#define _SCC_SPR_06	0X00AD
#define _SCC_F_V_01	0X00AE
#define _SCC_F_V_10	0X00AF
#define _SCC_HH_S	0X00B0
#define _SCC_F_V_02	0X00B1
#define _SCC_F_V_11	0X00B2
#define _SCC_F_V_20	0X00B3
#define _SCC_F_V_03	0X00B4
#define _SCC_F_V_12	0X00B5
#define _SCC_F_V_21	0X00B6
#define _SCC_F_V_04	0X00B7
#define _SCC_F_V_13	0X00B8
#define _SCC_F_V_22	0X00B9
#define _SCC_F_V_05	0X00BA
#define _SCC_F_V_14	0X00BB
#define _SCC_F_V_23	0X00BC
#define _SCC_TSP_09	0X00BD
#define _SCC_F_V_06	0X00BE
#define _SCC_F_V_15	0X00BF
#define _SCC_F_V_24	0X00C0
#define _SCC_KKOR_06	0X00C1
#define _SCC_F_V_07	0X00C2
#define _SCC_F_V_16	0X00C3
#define _SCC_F_V_25	0X00C4
#define _SCC_F_V_08	0X00C5
#define _SCC_F_V_17	0X00C6
#define _SCC_F_V_09	0X00C7
#define _SCC_F_V_18	0X00C8
#define _SCC_F_V_19	0X00C9
#define _SCC_TPV_09	0X00CA
#define _SCC_SP2_09	0X00CB
#define _SCC_YY_S	0X00CD
#define _SCC_TD_01	0X00CF
#define _SCC_TD_10	0X00D0
#define _SCC_TD_02	0X00D1
#define _SCC_TD_11	0X00D2
#define _SCC_TD_03	0X00D3
#define _SCC_TD_12	0X00D4
#define _SCC_TD_04	0X00D5
#define _SCC_XT_06	0X00D8
#define _SCC_XT_05	0X00D9
#define _SCC_XT_04	0X00DA
#define _SCC_XT_03	0X00DB
#define _SCC_XT_02	0X00DC
#define _SCC_XT_01	0X00DD
#define _SCC_AO_9_2	0X00DE
#define _SCC_AO_9_1	0X00DF
#define _SCC_AO_8_4	0X00E0
#define _SCC_AO_8_3	0X00E1
#define _SCC_AO_8_2	0X00E2
#define _SCC_AO_8_1	0X00E3
#define _SCC_AO_7_4	0X00E4
#define _SCC_AO_7_3	0X00E5
#define _SCC_AO_7_2	0X00E6
#define _SCC_AO_7_1	0X00E7
#define _SCC_XT_12	0X00E8
#define _SCC_XT_11	0X00E9
#define _SCC_XT_10	0X00EA
#define _SCC_XT_09	0X00EB
#define _SCC_XT_08	0X00EC
#define _SCC_XT_07	0X00ED
#define _SCC_TIME_S	0X00EE
#define _SCC_AO_9_4	0X00EF
#define _SCC_AO_9_3	0X00F0
#define _SCC_TARA2	0X00F1
#define _SCC_TARA1	0X00F2
#define _SCC_WEIDTH2	0X00F3
#define _SCC_WEIDTH1	0X00F4
#define _SCC_CANA00000016	0X011E
#define _SCC_CANA00000318	0X011F
#define _SCC_CANA00000348	0X0120
#define _SCC_CANA00000015	0X0121
#define _SCC_CANA0000000B	0X0122
#define _SCC_CANA00000330	0X0123
#define _SCC_CANA00000014	0X0124
#define _SCC_CANA00000018	0X0125
#define _SCC_CANA00000030	0X0126
#define _SCC_CANA00000031	0X0127
#define _SCC_CANA0000000D	0X0128
#define _SCC_CANA00006665	0X0129
#define _SCC_CANA0000000C	0X012A
#define _SCC_CANA00000003	0X012B
#define _SCC_CANA00000006	0X012C
#define _SCC_CANA00000005	0X012D
#define _SCC_CANA00000004	0X012E
#define _SCC_CANA00000007	0X012F
#define _SCC_CANA00000008	0X0130
#define _SCC_CANA00000000	0X0131
#define _SCC_CANA00000051	0X0132
#define _SCC_CANA000001DB	0X0133
#define _SCC_CANA00000117	0X0134
#define _SCC_CANA0000035D	0X0135
#define _SCC_CANA00000361	0X0136
#define _SCC_CANA00000223	0X0137
#define _SCC_CANA00000300	0X0138
#define _SCC_CANA0000035E	0X0139
#define _SCC_CANA00000360	0X013A
#define _SCC_CANA00000362	0X013B
#define _SCC_CANA00000200	0X013C
#define _SCC_CANA00000224	0X013D
#define _SCC_CANA0000012C	0X013E
#define _SCC_CANA000001DC	0X013F
#define _SCC_CANA0000000A	0X0140
#define _SCC_CANA00000100	0X0141
#define _SCC_CANA00000118	0X0142
#define _SCC_CANA00000001	0X0143
#define _SCC_CANA0000003C	0X0144
#define _SCC_CANA00000052	0X0145
#define _SCC_CANA00000002	0X0146
#define _SCC_REAL_NULL	0X0147
#define _SCC_CANA7FFFFFFF	0X014A
#define _SCC_FBL_RET	0X014F
#define _SCC_TMPI5	0X0150
#define _SCC_TMPI14	0X0151
#define _SCC_TMPI9	0X0152
#define _SCC_TMPI23	0X0153
#define _SCC_TMPI20	0X0154
#define _SCC_TMPI17	0X0155
#define _SCC_TMPI2	0X0168
#define _SCC_TMPI16	0X0173
#define _SCC_TMPI15	0X0174
#define _SCC_TMPI13	0X0175
#define _SCC_TMPI12	0X0176
#define _SCC_TMPR14	0X017E
#define _SCC_TMPR13	0X017F
#define _SCC_TMPR12	0X0180
#define _SCC_TMPR10	0X0181
#define _SCC_TMPR9	0X0182
#define _SCC_TMPR8	0X0183
#define _SCC_TMPR7	0X0184
#define _SCC_TMPR6	0X0185
#define _SCC_TMPR5	0X0186
#define _SCC_TMPR4	0X0187
#define _SCC_TMPR3	0X0188
#define _SCC_TMPR2	0X0189
#define _SCC_TMPR26	0X0192
#define _SCC_TMPR25	0X0193
#define _SCC_TMPR24	0X0194
#define _SCC_TMPR23	0X0195
#define _SCC_TMPR22	0X0196
#define _SCC_TMPR21	0X0197
#define _SCC_TMPR20	0X0198
#define _SCC_TMPR19	0X0199
#define _SCC_TMPR18	0X019A
#define _SCC_TMPR17	0X019B
#define _SCC_TMPR16	0X019C
#define _SCC_TMPR28	0X019D
#define _SCC_TMPR27	0X019E
#define _SCC_ED1_76132006_PV_O_VA	0X01B8
#define _SCC_ED1_76132007_KP_O_VA	0X01B9
#define _SCC_ED1_76132007_X0_O_VA	0X01BA
#define _SCC_ED1_76132007_SP_O_VA	0X01BB
#define _SCC_ED1_76132007_PV_O_VA	0X01BC
#define _SCC_ED1_76132005_KP_O_VA	0X01BD
#define _SCC_ED1_76132005_X0_O_VA	0X01BE
#define _SCC_ED1_76132005_SP_O_VA	0X01BF
#define _SCC_ED1_76132005_PV_O_VA	0X01C0
#define _SCC_ED1_76132006_KP_O_VA	0X01C1
#define _SCC_ED1_76132006_X0_O_VA	0X01C2
#define _SCC_ED1_76132006_SP_O_VA	0X01C3
#define _SCC_ED1_76132004_KP_O_VA	0X01C4
#define _SCC_ED1_76132004_X0_O_VA	0X01C5
#define _SCC_ED1_76132004_SP_O_VA	0X01C6
#define _SCC_ED1_76132004_PV_O_VA	0X01C7
#define _SCC_ED1_76132003_KP_O_VA	0X01C8
#define _SCC_ED1_76132003_X0_O_VA	0X01C9
#define _SCC_ED1_76132003_SP_O_VA	0X01CA
#define _SCC_ED1_76132003_PV_O_VA	0X01CB
#define _SCC_ED1_76132002_KP_O_VA	0X01CC
#define _SCC_ED1_76132002_X0_O_VA	0X01CD
#define _SCC_ED1_76132002_SP_O_VA	0X01CE
#define _SCC_ED1_76132002_PV_O_VA	0X01CF
#define _SCC_ED1_76132001_KP_O_VA	0X01DD
#define _SCC_ED1_76132001_X0_O_VA	0X01DE
#define _SCC_ED1_76132001_SP_O_VA	0X01DF
#define _SCC_ED1_76132001_PV_O_VA	0X01E0
#define _SCC_TMPI3	0X01E2
#define _SCC_TMPI6	0X01E5
#define _SCC_TMPR11	0X01E6
#define _SCC_TMPI4	0X01E7
#define _SCC_TMPI10	0X01EA
#define _SCC_TMPI8	0X01EB
#define _SCC_TMPR15	0X01EC
#define _SCC_TMPI11	0X01EE
#define _SCC_TMPR33	0X01F0
#define _SCC_TMPR32	0X01F1
#define _SCC_TMPR31	0X01F2
#define _SCC_TMPR30	0X01F3
#define _SCC_TMPR29	0X01F4
#define _SCC_ED1_7613200B_SP_O_VA	0X01F9
#define _SCC_ED1_7613200B_PV_O_VA	0X01FA
#define _SCC_ED1_7613200C_KP_O_VA	0X01FB
#define _SCC_ED1_7613200C_X0_O_VA	0X01FC
#define _SCC_ED1_7613200C_SP_O_VA	0X01FD
#define _SCC_ED1_7613200C_PV_O_VA	0X01FE
#define _SCC_ED1_7613200A_KP_O_VA	0X01FF
#define _SCC_ED1_7613200A_X0_O_VA	0X0200
#define _SCC_ED1_7613200A_SP_O_VA	0X0201
#define _SCC_ED1_7613200A_PV_O_VA	0X0202
#define _SCC_ED1_7613200B_KP_O_VA	0X0203
#define _SCC_ED1_7613200B_X0_O_VA	0X0204
#define _SCC_ED1_76132009_KP_O_VA	0X0205
#define _SCC_ED1_76132009_X0_O_VA	0X0206
#define _SCC_ED1_76132009_SP_O_VA	0X0207
#define _SCC_ED1_76132009_PV_O_VA	0X0208
#define _SCC_ED1_76132008_KP_O_VA	0X0209
#define _SCC_ED1_76132008_X0_O_VA	0X020A
#define _SCC_ED1_76132008_SP_O_VA	0X020B
#define _SCC_ED1_76132008_PV_O_VA	0X020C
#define _SCC_TMPR1	0X020D
#define _SCC_TMPI22	0X020E
#define _SCC_TMPI21	0X020F
#define _SCC_TMPI19	0X0210

/* Timers */

#define _SCC_CTMR000003E8	0X0001
#define _SCC_CTMR000000FA	0X0002

/* Messages */


/* Functions */

#define _SCC_RETAIN_X	0X0001
#define _SCC_V0_10_TO	0X0002
#define _SCC_A4_20_TO	0X0003
#define _SCC_WD_LONG	0X0004
#define _SCC_TO_A4_20	0X0005
#define _SCC_W_MB_REL	0X0006
#define _SCC_R_MB_REL	0X0007

/* Function blocks */

#define _SCC_STD_R_TRIG	0X0004

#define _SCC_STD_AVERAGE	0X0011

#define _SCC_STD_BLINK	0X0013

#define _SCC_EBUS_F_W	0X0015

#define _SCC_SYSDAT_R	0X0016

#define _SCC_MBUS_N_R	0X0017

#define _SCC_SYSTIM_W	0X0018

#define _SCC_SYSDAT_W	0X0019

#define _SCC_SYSTIM_R	0X001A

#define _SCC_I_7022	0X001B

#define _SCC_I_7017	0X001C

#define _SCC_I_7033	0X001D

#define _SCC_I_7024	0X001E

#define _SCC_PID_AL	0X001F
