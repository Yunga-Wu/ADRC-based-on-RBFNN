#ifndef RTW_HEADER_ADRConSiaAUV_ex01_acc_h_
#define RTW_HEADER_ADRConSiaAUV_ex01_acc_h_
#include <stddef.h>
#include <float.h>
#include <string.h>
#ifndef ADRConSiaAUV_ex01_acc_COMMON_INCLUDES_
#define ADRConSiaAUV_ex01_acc_COMMON_INCLUDES_
#include <stdlib.h>
#define S_FUNCTION_NAME simulink_only_sfcn 
#define S_FUNCTION_LEVEL 2
#define RTW_GENERATED_S_FUNCTION
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#endif
#include "ADRConSiaAUV_ex01_acc_types.h"
#include "multiword_types.h"
#include "rtGetInf.h"
#include "rt_nonfinite.h"
#include "mwmathutil.h"
#include "rt_defines.h"
typedef struct { real_T B_5_0_0 ; real_T B_5_1_0 [ 12 ] ; real_T B_5_5_0 ;
real_T B_5_6_0 [ 12 ] ; real_T B_5_10_0 ; real_T B_5_11_0 [ 12 ] ; real_T
B_5_15_0 ; real_T B_5_26_0 ; real_T B_5_27_0 ; real_T B_5_28_0 ; real_T
B_5_29_0 [ 2 ] ; real_T B_5_30_0 [ 2 ] ; real_T B_5_40_0 ; real_T B_5_45_0 [
2 ] ; real_T B_5_50_0 ; real_T B_5_60_0 ; real_T B_5_64_0 [ 2 ] ; real_T
B_5_65_0 ; real_T B_5_66_0 ; real_T B_5_71_0 ; real_T B_5_75_0 ; real_T
B_5_90_0 ; real_T B_5_0_0_m ; real_T B_5_1_0_c ; real_T B_5_3_0 ; real_T
B_5_4_0 ; real_T B_5_6_0_k ; real_T B_5_7_0 ; real_T B_5_8_0 ; real_T B_5_9_0
[ 12 ] ; real_T B_5_10_0_c ; real_T B_5_12_0 ; real_T B_5_13_0 ; real_T
B_5_14_0 ; real_T B_5_15_0_b ; real_T B_5_16_0 ; real_T B_5_17_0 [ 12 ] ;
real_T B_5_18_0 ; real_T B_5_20_0 ; real_T B_5_21_0 ; real_T B_5_22_0 ;
real_T B_5_23_0 ; real_T B_5_24_0 ; real_T B_5_25_0 [ 12 ] ; real_T
B_5_26_0_p ; real_T B_5_28_0_c ; real_T B_5_29_0_f ; real_T B_5_32_0 [ 2 ] ;
real_T B_5_35_0 [ 2 ] ; real_T B_4_0_0 [ 7 ] ; real_T B_4_1_1 [ 12 ] ; real_T
B_4_1_2 ; real_T B_3_0_0 [ 7 ] ; real_T B_3_1_1 [ 12 ] ; real_T B_3_1_2 ;
real_T B_2_0_0 [ 7 ] ; real_T B_2_1_1 [ 12 ] ; real_T B_2_1_2 ; real_T
B_1_0_1 ; real_T B_0_0_1 ; uint8_T B_5_86_0 ; uint8_T B_5_89_0 ; uint8_T
B_5_30_0_g ; uint8_T B_5_31_0 ; uint8_T B_5_33_0 ; uint8_T B_5_34_0 ; char_T
pad_B_5_34_0 [ 2 ] ; } B_ADRConSiaAUV_ex01_T ; typedef struct { real_T
TimeStampA ; real_T LastUAtTimeA ; real_T TimeStampB ; real_T LastUAtTimeB ;
struct { real_T modelTStart ; } TransportDelay_RWORK ; struct { void *
TUbufferPtrs [ 2 ] ; } TransportDelay_PWORK ; void * Scope1_PWORK [ 4 ] ;
void * ToWorkspace_PWORK ; void * ToWorkspace1_PWORK ; void *
ToWorkspace2_PWORK ; void * ToWorkspace3_PWORK ; void * ToWorkspace4_PWORK ;
void * ToWorkspace5_PWORK ; void * Scope_PWORK ; void * Scope1_PWORK_e ; void
* ToWorkspace_PWORK_f ; void * ToWorkspace1_PWORK_k ; void * Scope_PWORK_j ;
void * Scope_PWORK_e ; void * Scope_PWORK_b ; void * Scope_PWORK_l ; int32_T
MATLABFunction_sysIdxToRun ; int32_T MATLABFunction_sysIdxToRun_o ; int32_T
MATLABFunction_sysIdxToRun_c ; int32_T fhan_sysIdxToRun ; int32_T
fhan_sysIdxToRun_e ; struct { int_T Tail ; int_T Head ; int_T Last ; int_T
CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay_IWORK ; int_T
Integrator_IWORK ; int_T Integrator_IWORK_k ; int_T Integrator_IWORK_e ;
uint8_T Output_DSTATE ; uint8_T Output_DSTATE_a ; char_T pad_Output_DSTATE_a
[ 2 ] ; } DW_ADRConSiaAUV_ex01_T ; typedef struct { real_T Integrator_CSTATE
[ 12 ] ; real_T Integrator_CSTATE_i [ 12 ] ; real_T Integrator_CSTATE_p [ 12
] ; real_T StateSpace_CSTATE [ 3 ] ; real_T Integrator1_CSTATE ; real_T
Integrator_CSTATE_c ; real_T SFunction_CSTATE [ 5 ] ; real_T
StateSpace_CSTATE_n [ 3 ] ; real_T Integrator1_CSTATE_p ; real_T
Integrator_CSTATE_j ; real_T Integrator_CSTATE_a ; } X_ADRConSiaAUV_ex01_T ;
typedef struct { real_T Integrator_CSTATE [ 12 ] ; real_T Integrator_CSTATE_i
[ 12 ] ; real_T Integrator_CSTATE_p [ 12 ] ; real_T StateSpace_CSTATE [ 3 ] ;
real_T Integrator1_CSTATE ; real_T Integrator_CSTATE_c ; real_T
SFunction_CSTATE [ 5 ] ; real_T StateSpace_CSTATE_n [ 3 ] ; real_T
Integrator1_CSTATE_p ; real_T Integrator_CSTATE_j ; real_T
Integrator_CSTATE_a ; } XDot_ADRConSiaAUV_ex01_T ; typedef struct { boolean_T
Integrator_CSTATE [ 12 ] ; boolean_T Integrator_CSTATE_i [ 12 ] ; boolean_T
Integrator_CSTATE_p [ 12 ] ; boolean_T StateSpace_CSTATE [ 3 ] ; boolean_T
Integrator1_CSTATE ; boolean_T Integrator_CSTATE_c ; boolean_T
SFunction_CSTATE [ 5 ] ; boolean_T StateSpace_CSTATE_n [ 3 ] ; boolean_T
Integrator1_CSTATE_p ; boolean_T Integrator_CSTATE_j ; boolean_T
Integrator_CSTATE_a ; } XDis_ADRConSiaAUV_ex01_T ; struct
P_ADRConSiaAUV_ex01_T_ { real_T P_0 ; real_T P_1 ; real_T P_2 ; real_T P_3 ;
real_T P_4 ; real_T P_5 [ 5 ] ; real_T P_6 [ 4 ] ; real_T P_7 [ 3 ] ; real_T
P_8 ; real_T P_9 ; real_T P_10 ; real_T P_11 ; real_T P_12 ; real_T P_13 ;
real_T P_14 ; real_T P_15 ; real_T P_16 ; real_T P_17 ; real_T P_18 ; real_T
P_19 ; real_T P_20 ; real_T P_21 ; real_T P_22 ; real_T P_23 [ 5 ] ; real_T
P_24 [ 4 ] ; real_T P_25 [ 3 ] ; real_T P_26 ; real_T P_27 ; real_T P_28 ;
real_T P_29 ; real_T P_30 ; real_T P_31 ; real_T P_32 ; real_T P_33 ; real_T
P_34 ; real_T P_35 ; real_T P_36 ; real_T P_37 ; real_T P_38 ; real_T P_39 ;
real_T P_40 ; real_T P_41 ; real_T P_42 ; real_T P_43 ; real_T P_44 ; real_T
P_45 ; real_T P_46 ; real_T P_47 ; real_T P_48 [ 12 ] ; real_T P_49 ; real_T
P_50 ; real_T P_51 ; real_T P_52 ; real_T P_53 ; real_T P_54 ; real_T P_55 [
12 ] ; real_T P_56 ; real_T P_57 ; real_T P_58 ; real_T P_59 ; real_T P_60 ;
real_T P_61 ; real_T P_62 [ 12 ] ; real_T P_63 ; real_T P_64 ; real_T P_65 ;
real_T P_66 [ 2 ] ; real_T P_67 [ 2 ] ; uint32_T P_68 [ 5 ] ; uint32_T P_69 [
4 ] ; uint32_T P_70 [ 4 ] ; uint32_T P_71 [ 3 ] ; uint32_T P_72 [ 3 ] ;
uint32_T P_73 [ 4 ] ; uint32_T P_74 [ 5 ] ; uint32_T P_75 [ 4 ] ; uint32_T
P_76 [ 4 ] ; uint32_T P_77 [ 3 ] ; uint32_T P_78 [ 3 ] ; uint32_T P_79 [ 4 ]
; uint8_T P_80 ; uint8_T P_81 ; uint8_T P_82 ; uint8_T P_83 ; uint8_T P_84 ;
uint8_T P_85 ; uint8_T P_86 ; uint8_T P_87 ; } ; extern P_ADRConSiaAUV_ex01_T
ADRConSiaAUV_ex01_rtDefaultP ;
#endif
