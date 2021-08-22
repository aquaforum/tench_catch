/**********************************************************************/
/*                                                                    */
/*  File name : appli.H                                               */
/*  Date      : 24.11.2016 13:19:56                                   */
/*  ----------------------------------------------------------------  */
/*  ISaGRAF project name : kf1002                                     */
/*                                                                    */
/**********************************************************************/

/* Global variables of the various programs */

/* Programs */

#define _SCC_PARM_STO	0X0001
#define _SCC_IOS	0X0002
#define _SCC_CONTROL	0X0003
#define _SCC_DISPATCH	0X0004

/* Booleans */

#define _SCC_IO0001_00	0X0001
#define _SCC_IO0001_01	0X0002
#define _SCC_IO0001_02	0X0003
#define _SCC_IO0001_03	0X0004
#define _SCC_IO0001_04	0X0005
#define _SCC_IO0001_05	0X0006
#define _SCC_IO0001_06	0X0007
#define _SCC_IO0001_07	0X0008
#define _SCC_IO0001_08	0X0009
#define _SCC_IO0001_09	0X000A
#define _SCC_IO0001_0A	0X000B
#define _SCC_IO0001_0B	0X000C
#define _SCC_IO0001_0C	0X000D
#define _SCC_IO0001_0D	0X000E
#define _SCC_IO0001_0E	0X000F
#define _SCC_IO0001_0F	0X0010
#define _SCC_Y_B	0X0011
#define _SCC_Y_F	0X0012
#define _SCC_Y_E	0X0013
#define _SCC_Y_D	0X0014
#define _SCC_Y_C	0X0015
#define _SCC_Y_H	0X0016
#define _SCC_Y_G	0X0017
#define _SCC_Y_IJ	0X0018
#define _SCC_Y_ZOL	0X0019
#define _SCC_IO0003_01	0X001A
#define _SCC_IO0003_02	0X001B
#define _SCC_IO0003_03	0X001C
#define _SCC_IO0003_04	0X001D
#define _SCC_IO0003_05	0X001E
#define _SCC_Y_ALARM	0X001F
#define _SCC_IO0003_07	0X0020
#define _SCC_IO000A_00	0X0021
#define _SCC_X_PRESH	0X0022
#define _SCC_X_TAUTO	0X0023
#define _SCC_P_OFF	0X0024
#define _SCC_X_POWER	0X0025
#define _SCC_LOCK	0X0026
#define _SCC_RUN	0X0027
#define _SCC_X_DL	0X0028
#define _SCC_A_POWER	0X0029
#define _SCC_X_PA	0X002A
#define _SCC_X_KH	0X002B
#define _SCC_SAVE	0X002C
#define _SCC_UL_RUN	0X002D
#define _SCC_CALARM	0X002E
#define _SCC_Y_TSTOP	0X002F
#define _SCC_X_A	0X0030
#define _SCC_Y_BW	0X0031
#define _SCC_X_B	0X0032
#define _SCC_X_C	0X0033
#define _SCC_Y_TSTART	0X0034
#define _SCC_A_TRAUTO	0X0035
#define _SCC_X_D	0X0036
#define _SCC_AY_PUMP	0X0037
#define _SCC_X_E	0X0038
#define _SCC_A_A	0X0039
#define _SCC_Y_FW	0X003A
#define _SCC_X_F	0X003B
#define _SCC_A_B	0X003C
#define _SCC_X_KT	0X003D
#define _SCC_X_G	0X003E
#define _SCC_A_C	0X003F
#define _SCC_X_H	0X0040
#define _SCC_A_D	0X0041
#define _SCC_X_I	0X0042
#define _SCC_NET	0X0043
#define _SCC_A_E	0X0044
#define _SCC_X_J	0X0045
#define _SCC_A_F	0X0046
#define _SCC_X_PS	0X0047
#define _SCC_A_G	0X0048
#define _SCC_A_H	0X0049
#define _SCC_Y_PUMP	0X004A
#define _SCC_TEST	0X004B
#define _SCC_A_I	0X004C
#define _SCC_A_J	0X004D
#define _SCC_X_TON	0X004E
#define _SCC_X_PAUSE	0X004F
#define _SCC_ALARM	0X0050
#define _SCC_A_TR	0X0051
#define _SCC_START	0X0052
#define _SCC_TR_M	0X0053
#define _SCC_FREG_RUN	0X0054
#define _SCC_PTS_P	0X0055
#define _SCC_START_P	0X0056
#define _SCC_PTS_Q	0X0057
#define _SCC_START_Q	0X0058
#define _SCC_Q_TNP	0X0059
#define _SCC_Q_TRP	0X005A
#define _SCC_Q_T_BW	0X005B
#define _SCC_Q_S_OUT	0X005C
#define _SCC_Q_T_B	0X005D
#define _SCC_Q_T_F	0X005E
#define _SCC_Q_T_P	0X005F
#define _SCC_Q_T_VW	0X0060
#define _SCC_Q_T_VS	0X0061
#define _SCC_BPAUSE	0X0062
#define _SCC_BUNCLAMP	0X0063
#define _SCC_TONY_IJ	0X0064
#define _SCC_TSTY_IJ	0X0065
#define _SCC_GS_LATCH	0X0066
#define _SCC_C_T_CAPT	0X0067
#define _SCC_Q_TTOFF	0X0068
#define _SCC_DIO_2	0X0069
#define _SCC_Q_WAIT	0X006A
#define _SCC_S_WAIT	0X006B
#define _SCC_RE_X_PAUSE	0X006C
#define _SCC_TR_A	0X006D
#define _SCC_CBOO00000001	0X0073
#define _SCC_CBOO00000000	0X0074
#define _SCC_DUMMYCOIL	0X0075
#define _SCC_TMPB1	0X0076
#define _SCC_TMPB2	0X0077
#define _SCC_TMPB5	0X0078
#define _SCC_TMPB4	0X0079
#define _SCC_TMPB3	0X007A

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
#define _SCC_IO0009_00	0X0011
#define _SCC_NC	0X0012
#define _SCC_PSUSP_ZD	0X0013
#define _SCC_NV_MIN_ZD	0X0014
#define _SCC_NPV	0X0015
#define _SCC_KF_POIL	0X0016
#define _SCC_LSUSP	0X0017
#define _SCC_KP_V	0X0018
#define _SCC_PAIR	0X0019
#define _SCC_PSUSP	0X001A
#define _SCC_NS_ST_ZD	0X001B
#define _SCC_K_QV	0X001C
#define _SCC_QSUSP	0X001D
#define _SCC_QVS	0X001E
#define _SCC_GVODA	0X001F
#define _SCC_QVW	0X0020
#define _SCC_NS_ZD	0X0021
#define _SCC_NF	0X0022
#define _SCC_LCS	0X0023
#define _SCC_QVS_ZD	0X0024
#define _SCC_POIL	0X0025
#define _SCC_RC	0X0026
#define _SCC_KF_PSUSP	0X0027
#define _SCC_LVODA	0X0028
#define _SCC_NV_ZD	0X0029
#define _SCC_QVW_ZD	0X002A
#define _SCC_KP	0X002B
#define _SCC_GSUSP_ZD	0X002C
#define _SCC_LP	0X002D
#define _SCC_STATE	0X002E
#define _SCC_LS_START	0X002F
#define _SCC_N_KF	0X0030
#define _SCC_GSUSP	0X0031
#define _SCC_NS_MIN_ZD	0X0032
#define _SCC_STD	0X0033
#define _SCC_LS_MAX	0X0034
#define _SCC_LS_MIN	0X0035
#define _SCC_NPS	0X0036
#define _SCC_QWV_T	0X0037
#define _SCC_QVS_T	0X0038
#define _SCC_QSUSP_T	0X0039
#define _SCC_Y_STATE	0X003A
#define _SCC_T_QVW	0X003B
#define _SCC_T_QVS	0X003C
#define _SCC_T_QSUSP	0X003D
#define _SCC_CANA00000034	0X003F
#define _SCC_CANA00000020	0X0040
#define _SCC_CANA00000040	0X0041
#define _SCC_CANA00000013	0X0042
#define _SCC_CANA00000014	0X0043
#define _SCC_CANA00000015	0X0044
#define _SCC_CANA00000016	0X0045
#define _SCC_CANA00000019	0X0046
#define _SCC_CANA0000001A	0X0047
#define _SCC_CANA0000001B	0X0048
#define _SCC_CANA00000064	0X0049
#define _SCC_CANA00000065	0X004A
#define _SCC_CANA00000066	0X004B
#define _SCC_CANA00000067	0X004C
#define _SCC_CANA00000004	0X004D
#define _SCC_CANA00000005	0X004E
#define _SCC_CANA00000006	0X004F
#define _SCC_CANA00000007	0X0050
#define _SCC_CANA00000008	0X0051
#define _SCC_CANA00000009	0X0052
#define _SCC_CANA0000000A	0X0053
#define _SCC_CANA0000000B	0X0054
#define _SCC_CANA0000000C	0X0055
#define _SCC_CANA0000000D	0X0056
#define _SCC_CANA0000000E	0X0057
#define _SCC_CANA0000000F	0X0058
#define _SCC_CANA00000010	0X0059
#define _SCC_CANA00000011	0X005A
#define _SCC_CANA00000012	0X005B
#define _SCC_CANA00000001	0X005C
#define _SCC_CANA00000002	0X005D
#define _SCC_CANA00000003	0X005E
#define _SCC_CANA00000000	0X005F
#define _SCC_REAL_NULL	0X0060
#define _SCC_FBL_RET	0X0067
#define _SCC_TMPR1	0X0068
#define _SCC_TMPI2	0X0069
#define _SCC_TMPI4	0X0079
#define _SCC_TMPI3	0X007A
#define _SCC_TMPR2	0X007B
#define _SCC_TMPI5	0X0081
#define _SCC_TMPI10	0X0082

/* Timers */

#define _SCC_TALL	0X0001
#define _SCC_TFILT	0X0002
#define _SCC_TVS	0X0003
#define _SCC_TVW	0X0004
#define _SCC_TF	0X0005
#define _SCC_TNP	0X0006
#define _SCC_TNP_ZD	0X0007
#define _SCC_TF_ZD	0X0008
#define _SCC_TP_ZD	0X0009
#define _SCC_TB_ZD	0X000A
#define _SCC_TRP_ZD	0X000B
#define _SCC_TA_ZD	0X000C
#define _SCC_TBW_ZD	0X000D
#define _SCC_TPO_ZD	0X000E
#define _SCC_TTOFF_ZD	0X000F
#define _SCC_T_TP	0X0010
#define _SCC_T_TNP	0X0011
#define _SCC_T_TVW	0X0012
#define _SCC_T_TVS	0X0013
#define _SCC_T_TFILT	0X0014
#define _SCC_T_TALL	0X0015
#define _SCC_T_BW	0X0016
#define _SCC_T_BW_C	0X0017
#define _SCC_TPAUSE_ZD	0X0018
#define _SCC_TUNCLAMP_ZD	0X0019
#define _SCC_ZDTY_IJ	0X001A
#define _SCC_T_TF	0X001B
#define _SCC_T_WAIT	0X001C
#define _SCC_T_TB	0X001D
#define _SCC_CTMR00001770	0X001F
#define _SCC_CTMR00007530	0X0020
#define _SCC_CTMR00000320	0X0021
#define _SCC_CTMR00001388	0X0022
#define _SCC_CTMR000003E8	0X0023
#define _SCC_CTMR000007D0	0X0024
#define _SCC_CTMR000000C8	0X0025
#define _SCC_CTMR000001F4	0X0026
#define _SCC_CTMR00000000	0X0027
#define _SCC_CTMR001B7740	0X0028
#define _SCC_CTMR05265BFF	0X0029
#define _SCC_TMPT1	0X002A
#define _SCC_TMPT6	0X0033

/* Functions */

#define _SCC_EEP_N_R	0X0001
#define _SCC_EEP_N_W	0X0002
#define _SCC_EEP_PR	0X0003
#define _SCC_EEP_EN	0X0004
#define _SCC_REAL_INT	0X0005
#define _SCC_INT_REAL	0X0006
#define _SCC_A4_20_TO	0X0007

/* Function blocks */

#define _SCC_STD_SR	0X0003

#define _SCC_STD_R_TRIG	0X0004

#define _SCC_STD_TON	0X0009

#define _SCC_STD_TOF	0X000A

#define _SCC_STD_TP	0X000B

#define _SCC_STD_AVERAGE	0X0011

#define _SCC_STD_BLINK	0X0013

#define _SCC_STD_INTEGRAL	0X0014

#define _SCC_EBUS_N_R	0X0015

#define _SCC_I_7060D	0X0016

#define _SCC_EBUS_B_W	0X0017

#define _SCC_EBUS_B_R	0X0018

#define _SCC_EBUS_N_W	0X0019
