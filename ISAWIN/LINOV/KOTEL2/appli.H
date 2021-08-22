/**********************************************************************/
/*                                                                    */
/*  File name : appli.H                                               */
/*  Date      : 22.02.2020 21:41:57                                   */
/*  ----------------------------------------------------------------  */
/*  ISaGRAF project name : kotel2                                     */
/*                                                                    */
/**********************************************************************/

/* Global variables of the various programs */

/* Programs */

#define _SCC_CONTROL	0X0001
#define _SCC_PROTECT	0X0002

/* Booleans */

#define _SCC_X0001	0X0001
#define _SCC_X0010	0X0002
#define _SCC_X0002	0X0003
#define _SCC_X0011	0X0004
#define _SCC_X0003	0X0005
#define _SCC_X0020	0X0006
#define _SCC_X0012	0X0007
#define _SCC_Y0001	0X0008
#define _SCC_X0004	0X0009
#define _SCC_X0021	0X000A
#define _SCC_Y0010	0X000B
#define _SCC_X0013	0X000C
#define _SCC_Y0002	0X000D
#define _SCC_X0005	0X000E
#define _SCC_X0022	0X000F
#define _SCC_Y0011	0X0010
#define _SCC_X0014	0X0011
#define _SCC_Y0003	0X0012
#define _SCC_X0006	0X0013
#define _SCC_X0023	0X0014
#define _SCC_Y0012	0X0015
#define _SCC_X0015	0X0016
#define _SCC_Y0004	0X0017
#define _SCC_X0007	0X0018
#define _SCC_X0024	0X0019
#define _SCC_Y0013	0X001A
#define _SCC_X0016	0X001B
#define _SCC_Y0005	0X001C
#define _SCC_X0008	0X001D
#define _SCC_Y0014	0X001E
#define _SCC_X0017	0X001F
#define _SCC_Y0006	0X0020
#define _SCC_X0009	0X0021
#define _SCC_Y0015	0X0022
#define _SCC_X0018	0X0023
#define _SCC_Y0007	0X0024
#define _SCC_Y0016	0X0025
#define _SCC_X0019	0X0026
#define _SCC_Y0008	0X0027
#define _SCC_Y0009	0X0028
#define _SCC_CVIT	0X0029
#define _SCC_PRLOCK	0X002A
#define _SCC_AM0001	0X002B
#define _SCC_AM0002	0X002C
#define _SCC_AM0003	0X002D
#define _SCC_AM0004	0X002E
#define _SCC_AM0005	0X002F
#define _SCC_ALM0001	0X0030
#define _SCC_ALM0010	0X0031
#define _SCC_ALM0002	0X0032
#define _SCC_ALM0011	0X0033
#define _SCC_ALM0003	0X0034
#define _SCC_ALM0012	0X0035
#define _SCC_ALM0004	0X0036
#define _SCC_ALM0013	0X0037
#define _SCC_ALM0005	0X0038
#define _SCC_ALM0014	0X0039
#define _SCC_ALM0006	0X003A
#define _SCC_ALM0015	0X003B
#define _SCC_ALM0007	0X003C
#define _SCC_ALM0016	0X003D
#define _SCC_ALM0008	0X003E
#define _SCC_ALM0009	0X003F
#define _SCC_HALT	0X0040
#define _SCC_DUMMYCOIL	0X0041
#define _SCC_TMPB2	0X005F
#define _SCC_TMPB1	0X0060

/* Analogs */

#define _SCC_PG15A	0X0001
#define _SCC_PV	0X0002
#define _SCC_TV3A	0X0003
#define _SCC_TV4A	0X0004
#define _SCC_FG	0X0005
#define _SCC_GG	0X0006
#define _SCC_TD0001	0X0007
#define _SCC_TD0002	0X0008
#define _SCC_TD0003	0X0009
#define _SCC_TD0004	0X000A
#define _SCC_TD0005	0X000B
#define _SCC_GPC	0X000C
#define _SCC_KP0001	0X000D
#define _SCC_KP0002	0X000E
#define _SCC_KP0003	0X000F
#define _SCC_KP0004	0X0010
#define _SCC_PA	0X0011
#define _SCC_KP0005	0X0012
#define _SCC_VX	0X0013
#define _SCC_CP	0X0014
#define _SCC_QA	0X0015
#define _SCC_TA5A	0X0016
#define _SCC_TI0001	0X0017
#define _SCC_TA6A	0X0018
#define _SCC_TI0002	0X0019
#define _SCC_GPH	0X001A
#define _SCC_TI0003	0X001B
#define _SCC_TI0004	0X001C
#define _SCC_TI0005	0X001D
#define _SCC_FP	0X001E
#define _SCC_PV12A	0X001F
#define _SCC_XMIN0001	0X0020
#define _SCC_XMIN0002	0X0021
#define _SCC_XMIN0003	0X0022
#define _SCC_XMIN0004	0X0023
#define _SCC_XMIN0005	0X0024
#define _SCC_XMAX0001	0X0025
#define _SCC_XMAX0002	0X0026
#define _SCC_XMAX0003	0X0027
#define _SCC_XMAX0004	0X0028
#define _SCC_XMAX0005	0X0029
#define _SCC_TP17A	0X002A
#define _SCC_TP18A	0X002B
#define _SCC_CV	0X002C
#define _SCC_TPAR	0X002D
#define _SCC_SP0001	0X002E
#define _SCC_KK001	0X002F
#define _SCC_SP0002	0X0030
#define _SCC_CX	0X0031
#define _SCC_SP0003	0X0032
#define _SCC_SP0004	0X0033
#define _SCC_SP0005	0X0034
#define _SCC_SP0006	0X0035
#define _SCC_SP0007	0X0036
#define _SCC_FV	0X0037
#define _SCC_SP0008	0X0038
#define _SCC_K0001	0X0039
#define _SCC_K0002	0X003A
#define _SCC_K0003	0X003B
#define _SCC_K0004	0X003C
#define _SCC_PP7A	0X003D
#define _SCC_TK	0X003E
#define _SCC_CA	0X003F
#define _SCC_PP	0X0040
#define _SCC_LV	0X0041
#define _SCC_FA	0X0042
#define _SCC_XR0005	0X0043
#define _SCC_XR0004	0X0044
#define _SCC_XR0003	0X0045
#define _SCC_XR0002	0X0046
#define _SCC_XR0001	0X0047
#define _SCC_XT0005	0X0048
#define _SCC_XT0004	0X0049
#define _SCC_XT0003	0X004A
#define _SCC_XT0002	0X004B
#define _SCC_XT0001	0X004C
#define _SCC_FBL_RET	0X004D

/* Timers */

#define _SCC_TPR0001	0X0001
#define _SCC_TPR0010	0X0002
#define _SCC_TPR0002	0X0003
#define _SCC_TPR0011	0X0004
#define _SCC_TPR0003	0X0005
#define _SCC_TPR0012	0X0006
#define _SCC_TPR0004	0X0007
#define _SCC_TPR0013	0X0008
#define _SCC_TPR0005	0X0009
#define _SCC_TPR0014	0X000A
#define _SCC_TPR0006	0X000B
#define _SCC_TPR0015	0X000C
#define _SCC_TPR0007	0X000D
#define _SCC_TPR0016	0X000E
#define _SCC_TPR0008	0X000F
#define _SCC_TPR0009	0X0010
#define _SCC_CTMR000000C8	0X0011

/* Function blocks */

#define _SCC_STD_SR	0X0003

#define _SCC_STD_TON	0X0009

#define _SCC_PID_AL	0X0015
