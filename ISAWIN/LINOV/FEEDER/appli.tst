@ISA_SYMBOLS,27955712
#NAME,feeder,3.41
#DATE,03.10.2018
#SIZE,G=8,S=0,T=0,L=0,P=9,V=2
#COMMENT,wsma1tst

@PROGRAMS,8
#!5001,PARM_STO
#!5002,START
#!5003,FD_21
#!5004,FD_22
#!5005,FD_23
#!5006,TX_X
#!5007,MUX2
#!5008,KALMAN

@STEPS,0

@TRANSITIONS,0

@BOOLEANS,96
#!1003,RUN,+X,!0000,FALSE,TRUE
#!1004,SAVE,+X,!0000,FALSE,TRUE
#!1005,MQ_03,+X,!5005,FALSE,TRUE
#!1006,MQ_03,+X,!5004,FALSE,TRUE
#!1007,MQ_03,+X,!5003,FALSE,TRUE
#!1008,L0001,+X,!0000,FALSE,TRUE
#!1009,L0010,+X,!0000,FALSE,TRUE
#!100A,L0002,+X,!0000,FALSE,TRUE
#!100B,L0003,+X,!0000,FALSE,TRUE
#!100C,L0004,+X,!0000,FALSE,TRUE
#!100D,L0005,+X,!0000,FALSE,TRUE
#!100E,L0006,+X,!0000,FALSE,TRUE
#!100F,L0007,+X,!0000,FALSE,TRUE
#!1010,AM0001,+X,!0000,FALSE,TRUE
#!1011,L0008,+X,!0000,FALSE,TRUE
#!1012,AM0002,+X,!0000,FALSE,TRUE
#!1013,AM0003,+X,!0000,FALSE,TRUE
#!1014,L0009,+X,!0000,FALSE,TRUE
#!1015,RUN0001,+X,!0000,FALSE,TRUE
#!1016,AM0004,+X,!0000,FALSE,TRUE
#!1017,RUN0010,+X,!0000,FALSE,TRUE
#!1018,RUN0002,+X,!0000,FALSE,TRUE
#!1019,RUN0011,+X,!0000,FALSE,TRUE
#!101A,RUN0003,+X,!0000,FALSE,TRUE
#!101B,RUN0020,+X,!0000,FALSE,TRUE
#!101C,RUN0012,+X,!0000,FALSE,TRUE
#!101D,RUN0004,+X,!0000,FALSE,TRUE
#!101E,RUN0013,+X,!0000,FALSE,TRUE
#!101F,RUN0005,+X,!0000,FALSE,TRUE
#!1020,RUN0014,+X,!0000,FALSE,TRUE
#!1021,RUN0006,+X,!0000,FALSE,TRUE
#!1022,RUN0015,+X,!0000,FALSE,TRUE
#!1023,RUN0007,+X,!0000,FALSE,TRUE
#!1024,RUN0016,+X,!0000,FALSE,TRUE
#!1025,RUN0008,+X,!0000,FALSE,TRUE
#!1026,ALARM0009,+X,!0000,FALSE,TRUE
#!1027,ALARM0008,+X,!0000,FALSE,TRUE
#!1028,ALARM0007,+X,!0000,FALSE,TRUE
#!1029,ALARM0006,+X,!0000,FALSE,TRUE
#!102A,ALARM0005,+X,!0000,FALSE,TRUE
#!102B,ALARM0004,+X,!0000,FALSE,TRUE
#!102C,ALARM0003,+X,!0000,FALSE,TRUE
#!102D,ALARM0002,+X,!0000,FALSE,TRUE
#!102E,ALARM0001,+X,!0000,FALSE,TRUE
#!102F,RUN0017,+X,!0000,FALSE,TRUE
#!1030,ALARM0019,+X,!0000,FALSE,TRUE
#!1031,ALARM0018,+X,!0000,FALSE,TRUE
#!1032,ALARM0017,+X,!0000,FALSE,TRUE
#!1033,ALARM0016,+X,!0000,FALSE,TRUE
#!1034,ALARM0015,+X,!0000,FALSE,TRUE
#!1035,ALARM0014,+X,!0000,FALSE,TRUE
#!1036,ALARM0013,+X,!0000,FALSE,TRUE
#!1037,ALARM0012,+X,!0000,FALSE,TRUE
#!1038,ALARM0011,+X,!0000,FALSE,TRUE
#!1039,ALARM0010,+X,!0000,FALSE,TRUE
#!103A,RUN0009,+X,!0000,FALSE,TRUE
#!103B,ALARM0020,+X,!0000,FALSE,TRUE
#!103C,RUN0018,+X,!0000,FALSE,TRUE
#!103D,RUN0019,+X,!0000,FALSE,TRUE
#!103E,MC0001,+X,!0000,FALSE,TRUE
#!103F,LINK0001,+X,!0000,FALSE,TRUE
#!1040,MC0002,+X,!0000,FALSE,TRUE
#!1041,LINK0002,+X,!0000,FALSE,TRUE
#!1042,LINK0010,+X,!0000,FALSE,TRUE
#!1043,LINK0003,+X,!0000,FALSE,TRUE
#!1044,LINK0011,+X,!0000,FALSE,TRUE
#!1045,LINK0004,+X,!0000,FALSE,TRUE
#!1046,LINK0012,+X,!0000,FALSE,TRUE
#!1047,LINK0005,+X,!0000,FALSE,TRUE
#!1048,LINK0020,+X,!0000,FALSE,TRUE
#!1049,LINK0013,+X,!0000,FALSE,TRUE
#!104A,LINK0006,+X,!0000,FALSE,TRUE
#!104B,MC0003,+X,!0000,FALSE,TRUE
#!104C,LINK0014,+X,!0000,FALSE,TRUE
#!104D,LINK0007,+X,!0000,FALSE,TRUE
#!104E,LINK0015,+X,!0000,FALSE,TRUE
#!104F,LINK0008,+X,!0000,FALSE,TRUE
#!1050,LINK0016,+X,!0000,FALSE,TRUE
#!1051,LINK0009,+X,!0000,FALSE,TRUE
#!1052,LINK0017,+X,!0000,FALSE,TRUE
#!1053,LINK0018,+X,!0000,FALSE,TRUE
#!1054,MC0004,+X,!0000,FALSE,TRUE
#!1055,LINK0019,+X,!0000,FALSE,TRUE
#!1056,STARTWAIT,+X,!0000,FALSE,TRUE
#!1057,RES,+X,!5001,FALSE,TRUE
#!1058,READ,+X,!5001,FALSE,TRUE
#!1059,MQ_05,+X,!5003,FALSE,TRUE
#!105A,MQ_02,+X,!5003,FALSE,TRUE
#!105B,MQ_01,+X,!5003,FALSE,TRUE
#!105C,MQ_04,+X,!5004,FALSE,TRUE
#!105D,MQ_02,+X,!5004,FALSE,TRUE
#!105E,MQ_01,+X,!5004,FALSE,TRUE
#!105F,MQ_02,+X,!5005,FALSE,TRUE
#!1060,MQ_01,+X,!5005,FALSE,TRUE
#!1061,SL,+X,!5007,FALSE,TRUE
#!1062,RUN_,+X,!5008,FALSE,TRUE

@ANALOGS,150
#!2001,V0001,+X,!0000,F,
#!2002,V0010,+X,!0000,F,
#!2003,V0002,+X,!0000,F,
#!2004,V0011,+X,!0000,F,
#!2005,V0003,+X,!0000,F,
#!2006,V0020,+X,!0000,F,
#!2007,V0012,+X,!0000,F,
#!2008,V0004,+X,!0000,F,
#!2009,V0021,+X,!0000,F,
#!200A,V0013,+X,!0000,F,
#!200B,V0030,+X,!0000,F,
#!200C,V0005,+X,!0000,F,
#!200D,V0022,+X,!0000,F,
#!200E,V0014,+X,!0000,F,
#!200F,V0031,+X,!0000,F,
#!2010,V0006,+X,!0000,F,
#!2011,V0023,+X,!0000,F,
#!2012,V0040,+X,!0000,F,
#!2013,V0015,+X,!0000,F,
#!2014,V0032,+X,!0000,F,
#!2015,V0007,+X,!0000,F,
#!2016,V0024,+X,!0000,F,
#!2017,V0041,+X,!0000,F,
#!2018,V0016,+X,!0000,F,
#!2019,V0033,+X,!0000,F,
#!201A,V0050,+X,!0000,F,
#!201B,V0008,+X,!0000,F,
#!201C,V0025,+X,!0000,F,
#!201D,V0042,+X,!0000,F,
#!201E,V0017,+X,!0000,F,
#!201F,V0034,+X,!0000,F,
#!2020,V0051,+X,!0000,F,
#!2021,V0009,+X,!0000,F,
#!2022,V0026,+X,!0000,F,
#!2023,V0043,+X,!0000,F,
#!2024,V0060,+X,!0000,F,
#!2025,V0018,+X,!0000,F,
#!2026,V0035,+X,!0000,F,
#!2027,V0052,+X,!0000,F,
#!2028,TD0001,+X,!0000,F,
#!2029,V0027,+X,!0000,F,
#!202A,V0044,+X,!0000,F,
#!202B,V0019,+X,!0000,F,
#!202C,TD0002,+X,!0000,F,
#!202D,V0036,+X,!0000,F,
#!202E,V0053,+X,!0000,F,
#!202F,V0028,+X,!0000,F,
#!2030,V0045,+X,!0000,F,
#!2031,TD0003,+X,!0000,F,
#!2032,V0037,+X,!0000,F,
#!2033,V0054,+X,!0000,F,
#!2034,TD0004,+X,!0000,F,
#!2035,V0029,+X,!0000,F,
#!2036,V0046,+X,!0000,F,
#!2037,V0038,+X,!0000,F,
#!2038,V0055,+X,!0000,F,
#!2039,V0047,+X,!0000,F,
#!203A,V0039,+X,!0000,F,
#!203B,V0056,+X,!0000,F,
#!203C,V0048,+X,!0000,F,
#!203D,KP0001,+X,!0000,F,
#!203E,V0057,+X,!0000,F,
#!203F,V0049,+X,!0000,F,
#!2040,KP0002,+X,!0000,F,
#!2041,V0058,+X,!0000,F,
#!2042,KP0003,+X,!0000,F,
#!2043,KP0004,+X,!0000,F,
#!2044,V0059,+X,!0000,F,
#!2045,C0001,+X,!0000,F,
#!2046,C0010,+X,!0000,F,
#!2047,C0002,+X,!0000,F,
#!2048,TI0001,+X,!0000,F,
#!2049,TI0002,+X,!0000,F,
#!204A,C0003,+X,!0000,F,
#!204B,TI0003,+X,!0000,F,
#!204C,C0004,+X,!0000,F,
#!204D,TI0004,+X,!0000,F,
#!204E,C0005,+X,!0000,F,
#!204F,C0006,+X,!0000,F,
#!2050,XMIN0001,+X,!0000,F,
#!2051,C0007,+X,!0000,F,
#!2052,XMIN0002,+X,!0000,F,
#!2053,XMIN0003,+X,!0000,F,
#!2054,XMIN0004,+X,!0000,F,
#!2055,XMAX0001,+X,!0000,F,
#!2056,XMAX0002,+X,!0000,F,
#!2057,C0008,+X,!0000,F,
#!2058,XMAX0003,+X,!0000,F,
#!2059,XMAX0004,+X,!0000,F,
#!205A,C0009,+X,!0000,F,
#!205B,SP0001,+X,!0000,F,
#!205C,SP0002,+X,!0000,F,
#!205D,SP0003,+X,!0000,F,
#!205E,EC0001,+X,!0000,I,
#!205F,SP0004,+X,!0000,F,
#!2060,EC0010,+X,!0000,I,
#!2061,EC0002,+X,!0000,I,
#!2062,EC0011,+X,!0000,I,
#!2063,EC0020,+X,!0000,I,
#!2064,EC0003,+X,!0000,I,
#!2065,EC0012,+X,!0000,I,
#!2066,EC0004,+X,!0000,I,
#!2067,EC0013,+X,!0000,I,
#!2068,EC0005,+X,!0000,I,
#!2069,EC0014,+X,!0000,I,
#!206A,EC0006,+X,!0000,I,
#!206B,EC0015,+X,!0000,I,
#!206C,EC0007,+X,!0000,I,
#!206D,EC0016,+X,!0000,I,
#!206E,EC0008,+X,!0000,I,
#!206F,EC0017,+X,!0000,I,
#!2070,EC0009,+X,!0000,I,
#!2071,EC0018,+X,!0000,I,
#!2072,EC0019,+X,!0000,I,
#!2073,KAL1,+X,!0000,F,
#!2074,TC0002,+X,!0000,F,
#!2075,RC0001,+X,!0000,F,
#!2076,TC0001,+X,!0000,F,
#!2077,XT0001,+X,!0000,F,
#!2078,SAV_ST,+X,!5001,I,
#!2079,AI_4_4,+X,!5002,I,
#!207A,AI_4_3,+X,!5002,I,
#!207B,AI_4_2,+X,!5002,I,
#!207C,AI_4_1,+X,!5002,I,
#!207D,STATE_,+X,!5008,F,
#!207E,W_09,+X,!5003,I,
#!207F,W_05,+X,!5003,I,
#!2080,W_04,+X,!5003,I,
#!2081,W_03,+X,!5003,I,
#!2082,W_02,+X,!5003,I,
#!2083,W_01,+X,!5003,I,
#!2084,STATE,+X,!5008,F,
#!2085,W_09,+X,!5004,I,
#!2086,W_07,+X,!5004,I,
#!2087,W_06,+X,!5004,I,
#!2088,W_05,+X,!5004,I,
#!2089,W_04,+X,!5004,I,
#!208A,W_03,+X,!5004,I,
#!208B,W_02,+X,!5004,I,
#!208C,W_01,+X,!5004,I,
#!208D,W_09,+X,!5005,I,
#!208E,W_04,+X,!5005,I,
#!208F,W_03,+X,!5005,I,
#!2090,W_02,+X,!5005,I,
#!2091,W_01,+X,!5005,I,
#!2092,MUX2,+X,!5007,F,
#!2093,IN2,+X,!5007,F,
#!2094,IN1,+X,!5007,F,
#!2095,XIN_,+X,!5008,F,
#!2096,F_,+X,!5008,F,

@TIMERS,1
#!3001,SAV_TM,+X,!5001

@MESSAGES,0

@USP,9
#!B001,EEP_N_R
#!B002,EEP_N_W
#!B003,EEP_B_R
#!B004,EEP_B_W
#!B005,EEP_PR
#!B006,EEP_EN
#!B007,REAL_INT
#!B008,INT_REAL
#!B009,A4_20_TO

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@FBINSTANCES,0

@END_SYMBOLS
