@ISA_SYMBOLS,32668875
#NAME,ap2,3.41
#DATE,30.12.2013
#SIZE,G=5,S=0,T=0,L=0,P=8,V=38
#COMMENT,wsma1tst

@PROGRAMS,5
#!5001,PARM_STO
#!5002,IOS
#!5003,CONTROL
#!5004,T2MSG
#!5005,TOTIME

@STEPS,0

@TRANSITIONS,0

@BOOLEANS,54
#!1001,X_VO,+I,!0000,FALSE,TRUE
#!1002,X_VC,+I,!0000,FALSE,TRUE
#!1003,X_VK,+I,!0000,FALSE,TRUE
#!1004,X_SIR,+I,!0000,FALSE,TRUE
#!1005,X_VODA,+I,!0000,FALSE,TRUE
#!1006,X_AMSIR,+I,!0000,FALSE,TRUE
#!1007,X_AMVODA,+I,!0000,FALSE,TRUE
#!1008,X_AMVK,+I,!0000,FALSE,TRUE
#!1009,X_9,+I,!0000,FALSE,TRUE
#!100A,X_10,+I,!0000,FALSE,TRUE
#!100B,X_11,+I,!0000,FALSE,TRUE
#!100C,X_12,+I,!0000,FALSE,TRUE
#!100D,X_13,+I,!0000,FALSE,TRUE
#!100E,X_14,+I,!0000,FALSE,TRUE
#!100F,X_15,+I,!0000,FALSE,TRUE
#!1010,X_PUSK,+I,!0000,FALSE,TRUE
#!1011,Y_SIR,+O,!0000,FALSE,TRUE
#!1012,Y_VODA,+O,!0000,FALSE,TRUE
#!1013,Y_VK,+O,!0000,FALSE,TRUE
#!1014,Y_4,+O,!0000,FALSE,TRUE
#!1015,Y_ALARM,+O,!0000,FALSE,TRUE
#!1016,Y_5,+O,!0000,FALSE,TRUE
#!1017,Y_6,+O,!0000,FALSE,TRUE
#!1018,Y_7,+O,!0000,FALSE,TRUE
#!101E,A_COMMIT,+X,!0000,FALSE,TRUE
#!101F,A_T,+X,!0000,FALSE,TRUE
#!1020,A_SVK,+X,!0000,FALSE,TRUE
#!1021,RUN,+X,!0000,FALSE,TRUE
#!1022,SAVE,+X,!0000,FALSE,TRUE
#!1023,A_NOVODA,+X,!0000,FALSE,TRUE
#!1024,REJ_VODA,+X,!0000,FALSE,TRUE
#!1025,A_KOND,+X,!0000,FALSE,TRUE
#!1026,REJ_ROST,+X,!0000,FALSE,TRUE
#!1027,C_SHOWRZ,+X,!0000,FALSE,TRUE
#!1028,A_VODA,+X,!0000,FALSE,TRUE
#!1029,C_SHOWZV,+X,!0000,FALSE,TRUE
#!102A,A_BVK,+X,!0000,FALSE,TRUE
#!102B,A_ALERT,+X,!0000,FALSE,TRUE
#!102C,A_VISK,+X,!0000,FALSE,TRUE
#!102D,REJ_P,+X,!0000,FALSE,TRUE
#!102E,A_M,+X,!0000,FALSE,TRUE
#!102F,A_SIR,+X,!0000,FALSE,TRUE
#!1030,A_VLOW,+X,!0000,FALSE,TRUE
#!1031,A_R,+X,!0000,FALSE,TRUE
#!1032,Y_ALERT,+X,!0000,FALSE,TRUE
#!1033,Y_DR,+X,!0000,FALSE,TRUE
#!1034,Y_PID,+X,!0000,FALSE,TRUE
#!1035,Y_HYST,+X,!0000,FALSE,TRUE
#!1036,TO_VODA,+X,!0000,FALSE,TRUE
#!1037,READ,+X,!5001,FALSE,TRUE
#!1038,RES,+X,!5001,FALSE,TRUE
#!1039,RE_PUSK,+X,!5003,FALSE,TRUE
#!103A,RE_X_AMSIR,+X,!5003,FALSE,TRUE
#!103B,A_PUSK,+X,!5003,FALSE,TRUE

@ANALOGS,74
#!200A,COUT,+X,!0000,F,
#!200B,KOR,+X,!0000,F,
#!200C,V_01,+X,!0000,F,
#!200D,SP_H_1,+X,!0000,F,
#!200E,V_02,+X,!0000,F,
#!200F,SP_H_2,+X,!0000,F,
#!2010,V_6,+X,!0000,F,
#!2011,SP_H_3,+X,!0000,F,
#!2012,V_03,+X,!0000,F,
#!2013,V_7,+X,!0000,F,
#!2014,F_V_TEMP,+X,!0000,I,
#!2015,V_04,+X,!0000,F,
#!2016,V_8,+X,!0000,F,
#!2017,SPR_DM,+X,!0000,F,
#!2018,V_05,+X,!0000,F,
#!2019,V_06,+X,!0000,F,
#!201A,SP_M_1,+X,!0000,F,
#!201B,V_07,+X,!0000,F,
#!201C,SP_M_2,+X,!0000,F,
#!201D,SP_M_3,+X,!0000,F,
#!201E,V_08,+X,!0000,F,
#!201F,SP_R_MAX,+X,!0000,F,
#!2020,SPR_R_MAX,+X,!0000,F,
#!2021,F_V_6,+X,!0000,I,
#!2022,SP_R_MIN,+X,!0000,F,
#!2023,SP_R_ZV,+X,!0000,F,
#!2024,F_V_7,+X,!0000,I,
#!2025,F_V_8,+X,!0000,I,
#!2026,SP_H_F,+X,!0000,F,
#!2027,DH_MAX,+X,!0000,F,
#!2028,HH,+X,!0000,I,
#!2029,SP_H_N,+X,!0000,F,
#!202A,SP_M_F,+X,!0000,F,
#!202B,V_TEMP,+X,!0000,F,
#!202C,SPR_M,+X,!0000,F,
#!202D,SP_R_D,+X,!0000,F,
#!202E,V_VAK,+X,!0000,F,
#!202F,SPR_R,+X,!0000,F,
#!2030,F_V_L,+X,!0000,I,
#!2031,F_V_M,+X,!0000,I,
#!2032,RP_1,+X,!0000,F,
#!2033,RP_2,+X,!0000,F,
#!2034,F_V_R,+X,!0000,I,
#!2035,TD,+X,!0000,F,
#!2036,DR_MAX,+X,!0000,F,
#!2037,RP_3,+X,!0000,F,
#!2038,V_L,+X,!0000,F,
#!2039,MM,+X,!0000,I,
#!203A,V_M,+X,!0000,F,
#!203B,SP_M_D1,+X,!0000,F,
#!203C,SP_M_D2,+X,!0000,F,
#!203D,SP_M_D3,+X,!0000,F,
#!203E,STATE,+X,!0000,I,
#!203F,TI,+X,!0000,F,
#!2040,V_R,+X,!0000,F,
#!2041,KPR,+X,!0000,F,
#!2042,F_V_VAK,+X,!0000,I,
#!2043,SPR_M_L,+X,!0000,F,
#!2044,RZ_2,+X,!0000,F,
#!2045,STEPR,+X,!0000,F,
#!2046,RZ_3,+X,!0000,F,
#!2047,RZ_4,+X,!0000,F,
#!2048,T_COUT,+X,!0000,F,
#!2049,AP_NOM,+X,!0000,I,
#!204A,SAV_ST,+X,!5001,I,
#!204B,DR,+X,!5002,F,
#!204C,H_P_START,+X,!5003,F,
#!204D,PV,+X,!5003,F,
#!204E,A,+X,!5003,F,
#!204F,H_,+X,!5004,I,
#!2050,M_,+X,!5004,I,
#!2051,S_,+X,!5005,I,
#!2052,M_,+X,!5005,I,
#!2053,TM,+X,!5005,I,

@TIMERS,5
#!3001,TM_ZD,+X,!0000
#!3002,TALL,+X,!0000
#!3003,SAV_TM,+X,!5001
#!3004,T_PWM,+X,!5003
#!3005,T_,+X,!5004

@MESSAGES,4
#!4001,T2MSG,+X,!5004,255
#!4002,TM_,+X,!5004,10
#!4003,TOTIME,+X,!5005,255
#!4004,RES,+X,!5005,4

@USP,8
#!B001,EEP_N_R
#!B002,EEP_N_W
#!B003,EEP_PR
#!B004,EEP_EN
#!B005,REAL_INT
#!B006,INT_REAL
#!B007,A4_20_TO
#!B008,V0_10_TO

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
