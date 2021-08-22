/**********************************************************************/
/*                                                                    */
/*  File name : appli.H                                               */
/*  Date      : 21.09.2015 21:46:02                                   */
/*  ----------------------------------------------------------------  */
/*  ISaGRAF project name : rou                                        */
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

#define _SCC_X0001	0X0001
#define _SCC_X0002	0X0002
#define _SCC_X0003	0X0003
#define _SCC_X0004	0X0004
#define _SCC_X0005	0X0005
#define _SCC_X0006	0X0006
#define _SCC_X0007	0X0007
#define _SCC_X0008	0X0008
#define _SCC_X0009	0X0009
#define _SCC_X0010	0X000A
#define _SCC_X0011	0X000B
#define _SCC_X0012	0X000C
#define _SCC_X0013	0X000D
#define _SCC_X0014	0X000E
#define _SCC_X0015	0X000F
#define _SCC_X0016	0X0010
#define _SCC_IO0002_00	0X0011
#define _SCC_IO0002_01	0X0012
#define _SCC_Y0003	0X0013
#define _SCC_Y0004	0X0014
#define _SCC_Y0005	0X0015
#define _SCC_Y0006	0X0016
#define _SCC_Y0007	0X0017
#define _SCC_Y0008	0X0018
#define _SCC_Y0009	0X0019
#define _SCC_Y0010	0X001A
#define _SCC_Y0011	0X001B
#define _SCC_Y0012	0X001C
#define _SCC_Y0013	0X001D
#define _SCC_Y0014	0X001E
#define _SCC_Y0015	0X001F
#define _SCC_Y0016	0X0020
#define _SCC_IO000B_00	0X0021
#define _SCC_MC0007	0X0022
#define _SCC_MC0008	0X0023
#define _SCC_RUN	0X0024
#define _SCC_REJ0001	0X0025
#define _SCC_REJ0002	0X0026
#define _SCC_REJ0003	0X0027
#define _SCC_REJ0004	0X0028
#define _SCC_REJ0005	0X0029
#define _SCC_Y0001	0X002A
#define _SCC_REJ0006	0X002B
#define _SCC_REJ0007	0X002C
#define _SCC_Y0002	0X002D
#define _SCC_SAVE	0X002E
#define _SCC_TIME_SET	0X002F
#define _SCC_AM0001	0X0031
#define _SCC_AM0002	0X0032
#define _SCC_AM0003	0X0033
#define _SCC_AM0004	0X0034
#define _SCC_AM0005	0X0035
#define _SCC_AM0006	0X0036
#define _SCC_AM0007	0X0037
#define _SCC_AM0008	0X0038
#define _SCC_MC0001	0X0039
#define _SCC_MC0002	0X003A
#define _SCC_MC0003	0X003B
#define _SCC_MC0004	0X003C
#define _SCC_MC0005	0X003D
#define _SCC_MC0006	0X003E
#define _SCC_TY0014	0X003F
#define _SCC_TY0013	0X0040
#define _SCC_D_01	0X0041
#define _SCC_CBOO00000001	0X0047
#define _SCC_CBOO00000000	0X0048
#define _SCC_DUMMYCOIL	0X0049
#define _SCC_TMPB4	0X004A
#define _SCC_TMPB3	0X004B
#define _SCC_TMPB2	0X004C
#define _SCC_TMPB1	0X004D
#define _SCC_TMPB9	0X004E
#define _SCC_TMPB8	0X004F
#define _SCC_TMPB7	0X0050
#define _SCC_TMPB6	0X0051
#define _SCC_TMPB5	0X0052
#define _SCC_TMPB13	0X0053
#define _SCC_TMPB12	0X0054
#define _SCC_TMPB11	0X0055
#define _SCC_TMPB10	0X0056

/* Analogs */

#define _SCC_IO0003_00	0X0001
#define _SCC_IO0003_01	0X0002
#define _SCC_IO0003_02	0X0003
#define _SCC_IO0003_03	0X0004
#define _SCC_IO0003_04	0X0005
#define _SCC_IO0003_05	0X0006
#define _SCC_IO0003_06	0X0007
#define _SCC_IO0003_07	0X0008
#define _SCC_IO0003_08	0X0009
#define _SCC_IO0003_09	0X000A
#define _SCC_IO0003_0A	0X000B
#define _SCC_IO0003_0B	0X000C
#define _SCC_IO0003_0C	0X000D
#define _SCC_IO0003_0D	0X000E
#define _SCC_IO0003_0E	0X000F
#define _SCC_IO0003_0F	0X0010
#define _SCC_IO0009_00	0X0011
#define _SCC_MM_S	0X0012
#define _SCC_V0001	0X0013
#define _SCC_MON_S	0X0014
#define _SCC_V0010	0X0015
#define _SCC_V0002	0X0016
#define _SCC_V0011	0X0017
#define _SCC_V0003	0X0018
#define _SCC_V0020	0X0019
#define _SCC_V0012	0X001A
#define _SCC_V0004	0X001B
#define _SCC_V0021	0X001C
#define _SCC_V0013	0X001D
#define _SCC_V0005	0X001E
#define _SCC_V0022	0X001F
#define _SCC_V0014	0X0020
#define _SCC_V0006	0X0021
#define _SCC_V0023	0X0022
#define _SCC_V0015	0X0023
#define _SCC_V0007	0X0024
#define _SCC_V0024	0X0025
#define _SCC_V0016	0X0026
#define _SCC_V0008	0X0027
#define _SCC_V0017	0X0028
#define _SCC_V0009	0X0029
#define _SCC_V0018	0X002A
#define _SCC_TD0001	0X002B
#define _SCC_V0019	0X002C
#define _SCC_TD0002	0X002D
#define _SCC_TD0003	0X002E
#define _SCC_TD0004	0X002F
#define _SCC_TD0005	0X0030
#define _SCC_TD0006	0X0031
#define _SCC_TD0007	0X0032
#define _SCC_TD0008	0X0033
#define _SCC_FV0001	0X0034
#define _SCC_FV0010	0X0035
#define _SCC_FV0002	0X0036
#define _SCC_FV0011	0X0037
#define _SCC_FV0020	0X0038
#define _SCC_FV0003	0X0039
#define _SCC_FV0012	0X003A
#define _SCC_FV0021	0X003B
#define _SCC_FV0004	0X003C
#define _SCC_FV0013	0X003D
#define _SCC_FV0022	0X003E
#define _SCC_FV0005	0X003F
#define _SCC_FV0014	0X0040
#define _SCC_FV0023	0X0041
#define _SCC_FV0006	0X0042
#define _SCC_DD_S	0X0043
#define _SCC_FV0015	0X0044
#define _SCC_FV0024	0X0045
#define _SCC_FV0007	0X0046
#define _SCC_FV0016	0X0047
#define _SCC_FV0008	0X0048
#define _SCC_C0001	0X0049
#define _SCC_FV0017	0X004A
#define _SCC_FV0009	0X004B
#define _SCC_FV0018	0X004C
#define _SCC_C0002	0X004D
#define _SCC_TI0001	0X004E
#define _SCC_FV0019	0X004F
#define _SCC_TI0002	0X0050
#define _SCC_C0003	0X0051
#define _SCC_TI0003	0X0052
#define _SCC_C0004	0X0053
#define _SCC_TI0004	0X0054
#define _SCC_C0005	0X0055
#define _SCC_TI0005	0X0056
#define _SCC_SS_S	0X0057
#define _SCC_TI0006	0X0058
#define _SCC_C0006	0X0059
#define _SCC_TI0007	0X005A
#define _SCC_XMIN0001	0X005B
#define _SCC_C0007	0X005C
#define _SCC_XMIN0002	0X005D
#define _SCC_XMIN0003	0X005E
#define _SCC_TI0008	0X005F
#define _SCC_XMIN0004	0X0060
#define _SCC_XMIN0005	0X0061
#define _SCC_XMIN0006	0X0062
#define _SCC_XMAX0001	0X0063
#define _SCC_XMIN0007	0X0064
#define _SCC_XMAX0002	0X0065
#define _SCC_XMIN0008	0X0066
#define _SCC_C0008	0X0067
#define _SCC_XMAX0003	0X0068
#define _SCC_XMAX0004	0X0069
#define _SCC_XMAX0005	0X006A
#define _SCC_XMAX0006	0X006B
#define _SCC_XMAX0007	0X006C
#define _SCC_XMAX0008	0X006D
#define _SCC_KPR0001	0X006E
#define _SCC_KPR0002	0X006F
#define _SCC_KPR0003	0X0070
#define _SCC_KPR0004	0X0071
#define _SCC_KPR0005	0X0072
#define _SCC_KPR0006	0X0073
#define _SCC_KPR0007	0X0074
#define _SCC_KPR0008	0X0075
#define _SCC_SP0001	0X0076
#define _SCC_SP0002	0X0077
#define _SCC_SP0003	0X0078
#define _SCC_SP0004	0X0079
#define _SCC_SP0005	0X007A
#define _SCC_HH_S	0X007B
#define _SCC_SP0006	0X007C
#define _SCC_SP0007	0X007D
#define _SCC_SP0008	0X007E
#define _SCC_SP2003	0X007F
#define _SCC_YY_S	0X0080
#define _SCC_CT0002	0X0081
#define _SCC_CT0001	0X0082
#define _SCC_XT0008	0X0083
#define _SCC_XT0007	0X0084
#define _SCC_XT0006	0X0085
#define _SCC_XT0005	0X0086
#define _SCC_XT0004	0X0087
#define _SCC_XT0003	0X0088
#define _SCC_XT0002	0X0089
#define _SCC_XT0001	0X008A
#define _SCC_CT0008	0X008B
#define _SCC_CT0007	0X008C
#define _SCC_CT0006	0X008D
#define _SCC_CT0005	0X008E
#define _SCC_CT0004	0X008F
#define _SCC_CT0003	0X0090
#define _SCC_TIME_S	0X0093
#define _SCC_CANA00000005	0X00A1
#define _SCC_CANA0000003C	0X00A2
#define _SCC_CANA0000000D	0X00A3
#define _SCC_CANA0000001F	0X00A4
#define _SCC_CANA00000030	0X00A5
#define _SCC_CANA00000047	0X00A6
#define _SCC_CANA0000015F	0X00A7
#define _SCC_CANA00000216	0X00A8
#define _SCC_CANA00000315	0X00A9
#define _SCC_CANA00000009	0X00AA
#define _SCC_CANA00000200	0X00AB
#define _SCC_CANA00000217	0X00AC
#define _SCC_CANA00000300	0X00AD
#define _SCC_CANA00000316	0X00AE
#define _SCC_CANA00000001	0X00AF
#define _SCC_CANA0000003A	0X00B0
#define _SCC_CANA00000048	0X00B1
#define _SCC_CANA00000002	0X00B2
#define _SCC_CANA00000014	0X00B3
#define _SCC_CANA00000100	0X00B4
#define _SCC_CANA00000160	0X00B5
#define _SCC_REAL_NULL	0X00B6
#define _SCC_FBL_RET	0X00BC
#define _SCC_TMPI5	0X00BD
#define _SCC_TMPI18	0X00BE
#define _SCC_TMPI15	0X00BF
#define _SCC_TMPI10	0X00C0
#define _SCC_TMPI3	0X00CB
#define _SCC_TMPR2	0X00CD
#define _SCC_TMPI4	0X00CE
#define _SCC_TMPR14	0X00CF
#define _SCC_TMPR13	0X00D0
#define _SCC_TMPR12	0X00D1
#define _SCC_TMPR11	0X00D2
#define _SCC_TMPR10	0X00D3
#define _SCC_TMPR9	0X00D4
#define _SCC_TMPR8	0X00D5
#define _SCC_TMPR7	0X00D6
#define _SCC_TMPR6	0X00D7
#define _SCC_TMPR5	0X00D8
#define _SCC_TMPR4	0X00D9
#define _SCC_TMPR3	0X00DA
#define _SCC_TMPI9	0X00DB
#define _SCC_TMPR17	0X00DC
#define _SCC_TMPR16	0X00DD
#define _SCC_TMPR15	0X00DE
#define _SCC_TMPI2	0X00F4
#define _SCC_TMPR19	0X00F5
#define _SCC_TMPR18	0X00F6
#define _SCC_TMPI6	0X00F7
#define _SCC_TMPR23	0X00F8
#define _SCC_TMPR22	0X00F9
#define _SCC_TMPI8	0X00FA
#define _SCC_TMPR21	0X00FB
#define _SCC_TMPR20	0X00FC
#define _SCC_TMPI7	0X00FD

/* Timers */

#define _SCC_CTMR000003E8	0X0001
#define _SCC_CTMR000001F4	0X0002
#define _SCC_CTMR000000FA	0X0003

/* Messages */


/* Functions */

#define _SCC_RETAIN_X	0X0001
#define _SCC_TO_A4_20	0X0002
#define _SCC_A4_20_TO	0X0003

/* Function blocks */

#define _SCC_STD_R_TRIG	0X0004

#define _SCC_STD_TON	0X0009

#define _SCC_STD_HYSTER	0X0010

#define _SCC_STD_AVERAGE	0X0011

#define _SCC_STD_BLINK	0X0013

#define _SCC_I_7024	0X0015

#define _SCC_SYSDAT_W	0X0016

#define _SCC_SYSTIM_R	0X0017

#define _SCC_SYSDAT_R	0X0018

#define _SCC_EBUS_F_R	0X0019

#define _SCC_SYSTIM_W	0X001A

#define _SCC_PID_AL	0X001B
