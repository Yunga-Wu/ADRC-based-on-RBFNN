#include <math.h>
#include "ADRConSiaAUV_ex01_acc.h"
#include "ADRConSiaAUV_ex01_acc_private.h"
#include <stdio.h>
#include "slexec_vm_simstruct_bridge.h"
#include "slexec_vm_zc_functions.h"
#include "slexec_vm_lookup_functions.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simtarget/slSimTgtMdlrefSfcnBridge.h"
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#include "simtarget/slAccSfcnBridge.h"
#ifndef __RTW_UTFREE__  
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T ADRConSiaAUV_ex01_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T *
bufSzPtr , int_T * tailPtr , int_T * headPtr , int_T * lastPtr , real_T
tMinusDelay , real_T * * uBufPtr , boolean_T isfixedbuf , boolean_T
istransportdelay , int_T * maxNewBufSzPtr ) { int_T testIdx ; int_T tail = *
tailPtr ; int_T bufSz = * bufSzPtr ; real_T * tBuf = * uBufPtr + bufSz ;
real_T * xBuf = ( NULL ) ; int_T numBuffer = 2 ; if ( istransportdelay ) {
numBuffer = 3 ; xBuf = * uBufPtr + 2 * bufSz ; } testIdx = ( tail < ( bufSz -
1 ) ) ? ( tail + 1 ) : 0 ; if ( ( tMinusDelay <= tBuf [ testIdx ] ) && !
isfixedbuf ) { int_T j ; real_T * tempT ; real_T * tempU ; real_T * tempX = (
NULL ) ; real_T * uBuf = * uBufPtr ; int_T newBufSz = bufSz + 1024 ; if (
newBufSz > * maxNewBufSzPtr ) { * maxNewBufSzPtr = newBufSz ; } tempU = (
real_T * ) utMalloc ( numBuffer * newBufSz * sizeof ( real_T ) ) ; if ( tempU
== ( NULL ) ) { return ( false ) ; } tempT = tempU + newBufSz ; if (
istransportdelay ) tempX = tempT + newBufSz ; for ( j = tail ; j < bufSz ; j
++ ) { tempT [ j - tail ] = tBuf [ j ] ; tempU [ j - tail ] = uBuf [ j ] ; if
( istransportdelay ) tempX [ j - tail ] = xBuf [ j ] ; } for ( j = 0 ; j <
tail ; j ++ ) { tempT [ j + bufSz - tail ] = tBuf [ j ] ; tempU [ j + bufSz -
tail ] = uBuf [ j ] ; if ( istransportdelay ) tempX [ j + bufSz - tail ] =
xBuf [ j ] ; } if ( * lastPtr > tail ) { * lastPtr -= tail ; } else { *
lastPtr += ( bufSz - tail ) ; } * tailPtr = 0 ; * headPtr = bufSz ; utFree (
uBuf ) ; * bufSzPtr = newBufSz ; * uBufPtr = tempU ; } else { * tailPtr =
testIdx ; } return ( true ) ; } real_T
ADRConSiaAUV_ex01_acc_rt_TDelayInterpolate ( real_T tMinusDelay , real_T
tStart , real_T * uBuf , int_T bufSz , int_T * lastIdx , int_T oldestIdx ,
int_T newIdx , real_T initOutput , boolean_T discrete , boolean_T
minorStepAndTAtLastMajorOutput ) { int_T i ; real_T yout , t1 , t2 , u1 , u2
; real_T * tBuf = uBuf + bufSz ; if ( ( newIdx == 0 ) && ( oldestIdx == 0 )
&& ( tMinusDelay > tStart ) ) return initOutput ; if ( tMinusDelay <= tStart
) return initOutput ; if ( ( tMinusDelay <= tBuf [ oldestIdx ] ) ) { if (
discrete ) { return ( uBuf [ oldestIdx ] ) ; } else { int_T tempIdx =
oldestIdx + 1 ; if ( oldestIdx == bufSz - 1 ) tempIdx = 0 ; t1 = tBuf [
oldestIdx ] ; t2 = tBuf [ tempIdx ] ; u1 = uBuf [ oldestIdx ] ; u2 = uBuf [
tempIdx ] ; if ( t2 == t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else {
yout = u1 ; } } else { real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ;
real_T f2 = 1.0 - f1 ; yout = f1 * u1 + f2 * u2 ; } return yout ; } } if (
minorStepAndTAtLastMajorOutput ) { if ( newIdx != 0 ) { if ( * lastIdx ==
newIdx ) { ( * lastIdx ) -- ; } newIdx -- ; } else { if ( * lastIdx == newIdx
) { * lastIdx = bufSz - 1 ; } newIdx = bufSz - 1 ; } } i = * lastIdx ; if (
tBuf [ i ] < tMinusDelay ) { while ( tBuf [ i ] < tMinusDelay ) { if ( i ==
newIdx ) break ; i = ( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } } else { while
( tBuf [ i ] >= tMinusDelay ) { i = ( i > 0 ) ? i - 1 : ( bufSz - 1 ) ; } i =
( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } * lastIdx = i ; if ( discrete ) {
double tempEps = ( DBL_EPSILON ) * 128.0 ; double localEps = tempEps *
muDoubleScalarAbs ( tBuf [ i ] ) ; if ( tempEps > localEps ) { localEps =
tempEps ; } localEps = localEps / 2.0 ; if ( tMinusDelay >= ( tBuf [ i ] -
localEps ) ) { yout = uBuf [ i ] ; } else { if ( i == 0 ) { yout = uBuf [
bufSz - 1 ] ; } else { yout = uBuf [ i - 1 ] ; } } } else { if ( i == 0 ) {
t1 = tBuf [ bufSz - 1 ] ; u1 = uBuf [ bufSz - 1 ] ; } else { t1 = tBuf [ i -
1 ] ; u1 = uBuf [ i - 1 ] ; } t2 = tBuf [ i ] ; u2 = uBuf [ i ] ; if ( t2 ==
t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else { yout = u1 ; } } else {
real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ; real_T f2 = 1.0 - f1 ; yout
= f1 * u1 + f2 * u2 ; } } return ( yout ) ; } void rt_ssGetBlockPath (
SimStruct * S , int_T sysIdx , int_T blkIdx , char_T * * path ) {
_ssGetBlockPath ( S , sysIdx , blkIdx , path ) ; } void rt_ssSet_slErrMsg (
void * S , void * diag ) { SimStruct * castedS = ( SimStruct * ) S ; if ( !
_ssIsErrorStatusAslErrMsg ( castedS ) ) { _ssSet_slErrMsg ( castedS , diag )
; } else { _ssDiscardDiagnostic ( castedS , diag ) ; } } void
rt_ssReportDiagnosticAsWarning ( void * S , void * diag ) {
_ssReportDiagnosticAsWarning ( ( SimStruct * ) S , diag ) ; } void
rt_ssReportDiagnosticAsInfo ( void * S , void * diag ) {
_ssReportDiagnosticAsInfo ( ( SimStruct * ) S , diag ) ; } static void
mdlOutputs ( SimStruct * S , int_T tid ) { B_ADRConSiaAUV_ex01_T * _rtB ;
DW_ADRConSiaAUV_ex01_T * _rtDW ; P_ADRConSiaAUV_ex01_T * _rtP ;
X_ADRConSiaAUV_ex01_T * _rtX ; real_T rtb_B_5_23_0 [ 3 ] ; real_T
rtb_B_5_24_0 ; real_T rtb_B_5_25_0 ; real_T rtb_B_5_35_0 ; real_T
rtb_B_5_53_0 ; real_T rtb_B_5_72_0 ; real_T * lastU ; int32_T i ; uint32_T ri
; uint8_T rtb_B_5_38_0 ; boolean_T tmp ; _rtDW = ( ( DW_ADRConSiaAUV_ex01_T *
) ssGetRootDWork ( S ) ) ; _rtX = ( ( X_ADRConSiaAUV_ex01_T * )
ssGetContStates ( S ) ) ; _rtP = ( ( P_ADRConSiaAUV_ex01_T * ) ssGetModelRtp
( S ) ) ; _rtB = ( ( B_ADRConSiaAUV_ex01_T * ) _ssGetModelBlockIO ( S ) ) ; {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay =
simTime - _rtP -> P_0 ; _rtB -> B_5_0_0 =
ADRConSiaAUV_ex01_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer ,
_rtDW -> TransportDelay_IWORK . CircularBufSize , & _rtDW ->
TransportDelay_IWORK . Last , _rtDW -> TransportDelay_IWORK . Tail , _rtDW ->
TransportDelay_IWORK . Head , _rtP -> P_1 , 1 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } tmp = ssGetIsOkayToUpdateMode ( S ) ; for ( i = 0 ; i < 12 ; i ++ ) {
if ( tmp ) { if ( _rtDW -> Integrator_IWORK != 0 ) { _rtX ->
Integrator_CSTATE [ i ] = _rtB -> B_5_9_0 [ i ] ; } _rtB -> B_5_1_0 [ i ] =
_rtX -> Integrator_CSTATE [ i ] ; } else { _rtB -> B_5_1_0 [ i ] = _rtX ->
Integrator_CSTATE [ i ] ; } } _rtB -> B_5_5_0 = ( muDoubleScalarMod ( _rtB ->
B_5_13_0 + _rtB -> B_5_1_0 [ 11 ] , _rtB -> B_5_12_0 ) - _rtB -> B_5_13_0 ) *
_rtP -> P_2 ; tmp = ssGetIsOkayToUpdateMode ( S ) ; for ( i = 0 ; i < 12 ; i
++ ) { if ( tmp ) { if ( _rtDW -> Integrator_IWORK_k != 0 ) { _rtX ->
Integrator_CSTATE_i [ i ] = _rtB -> B_5_17_0 [ i ] ; } _rtB -> B_5_6_0 [ i ]
= _rtX -> Integrator_CSTATE_i [ i ] ; } else { _rtB -> B_5_6_0 [ i ] = _rtX
-> Integrator_CSTATE_i [ i ] ; } } _rtB -> B_5_10_0 = ( muDoubleScalarMod (
_rtB -> B_5_21_0 + _rtB -> B_5_6_0 [ 11 ] , _rtB -> B_5_20_0 ) - _rtB ->
B_5_21_0 ) * _rtP -> P_3 ; tmp = ssGetIsOkayToUpdateMode ( S ) ; for ( i = 0
; i < 12 ; i ++ ) { if ( tmp ) { if ( _rtDW -> Integrator_IWORK_e != 0 ) {
_rtX -> Integrator_CSTATE_p [ i ] = _rtB -> B_5_25_0 [ i ] ; } _rtB ->
B_5_11_0 [ i ] = _rtX -> Integrator_CSTATE_p [ i ] ; } else { _rtB ->
B_5_11_0 [ i ] = _rtX -> Integrator_CSTATE_p [ i ] ; } } _rtB -> B_5_15_0 = (
muDoubleScalarMod ( _rtB -> B_5_29_0_f + _rtB -> B_5_11_0 [ 11 ] , _rtB ->
B_5_28_0_c ) - _rtB -> B_5_29_0_f ) * _rtP -> P_4 ; i = ssIsSampleHit ( S , 1
, 0 ) ; if ( i != 0 ) { ssCallAccelRunBlock ( S , 5 , 16 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 5 , 17 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 5 , 18 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 5 , 19 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 5 , 20 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 5 , 21 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 5 , 22 ,
SS_CALL_MDL_OUTPUTS ) ; } rtb_B_5_23_0 [ 0 ] = 0.0 ; rtb_B_5_23_0 [ 1 ] = 0.0
; rtb_B_5_23_0 [ 2 ] = 0.0 ; for ( ri = _rtP -> P_73 [ 0U ] ; ri < _rtP ->
P_73 [ 1U ] ; ri ++ ) { rtb_B_5_23_0 [ _rtP -> P_72 [ ri ] ] += _rtP -> P_7 [
ri ] * _rtX -> StateSpace_CSTATE [ 0U ] ; } for ( ri = _rtP -> P_73 [ 1U ] ;
ri < _rtP -> P_73 [ 2U ] ; ri ++ ) { rtb_B_5_23_0 [ _rtP -> P_72 [ ri ] ] +=
_rtP -> P_7 [ ri ] * _rtX -> StateSpace_CSTATE [ 1U ] ; } for ( ri = _rtP ->
P_73 [ 2U ] ; ri < _rtP -> P_73 [ 3U ] ; ri ++ ) { rtb_B_5_23_0 [ _rtP ->
P_72 [ ri ] ] += _rtP -> P_7 [ ri ] * _rtX -> StateSpace_CSTATE [ 2U ] ; }
rtb_B_5_24_0 = _rtP -> P_9 * rtb_B_5_23_0 [ 1 ] ; rtb_B_5_25_0 = _rtX ->
Integrator1_CSTATE ; _rtB -> B_5_26_0 = _rtX -> Integrator1_CSTATE -
rtb_B_5_23_0 [ 0 ] ; _rtB -> B_5_27_0 = _rtX -> Integrator_CSTATE_c ; _rtB ->
B_5_28_0 = _rtB -> B_5_27_0 - rtb_B_5_23_0 [ 1 ] ; _rtB -> B_5_29_0 [ 0 ] =
_rtB -> B_5_26_0 ; _rtB -> B_5_29_0 [ 1 ] = _rtB -> B_5_28_0 ;
ssCallAccelRunBlock ( S , 5 , 30 , SS_CALL_MDL_OUTPUTS ) ; rtb_B_5_35_0 = ( (
_rtP -> P_12 * _rtB -> B_5_26_0 + _rtP -> P_13 * rtb_B_5_24_0 ) - _rtB ->
B_5_30_0 [ 1 ] ) * _rtP -> P_14 ; rtb_B_5_24_0 = ( muDoubleScalarSin ( _rtP
-> P_17 * ssGetTaskTime ( S , 0 ) + _rtP -> P_18 ) * _rtP -> P_15 + _rtP ->
P_16 ) * _rtP -> P_19 ; i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) {
rtb_B_5_38_0 = _rtDW -> Output_DSTATE ; _rtB -> B_5_40_0 = _rtP -> P_20 *
_rtB -> B_5_32_0 [ _rtDW -> Output_DSTATE ] ; } rtb_B_5_24_0 = ( rtb_B_5_24_0
+ _rtB -> B_5_40_0 ) * _rtP -> P_21 ; rtb_B_5_35_0 += rtb_B_5_24_0 ; _rtB ->
B_5_45_0 [ 0 ] = _rtP -> P_22 * rtb_B_5_35_0 ; _rtB -> B_5_45_0 [ 1 ] = _rtB
-> B_5_15_0 ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) {
ssCallAccelRunBlock ( S , 5 , 46 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 5 , 47 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 5 , 48 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 5 , 49 , SS_CALL_MDL_OUTPUTS ) ; } _rtB -> B_5_50_0
= rtb_B_5_25_0 - _rtB -> B_5_0_0 ; ssCallAccelRunBlock ( S , 0 , 0 ,
SS_CALL_MDL_OUTPUTS ) ; rtb_B_5_23_0 [ 0 ] = 0.0 ; rtb_B_5_23_0 [ 1 ] = 0.0 ;
rtb_B_5_23_0 [ 2 ] = 0.0 ; for ( ri = _rtP -> P_79 [ 0U ] ; ri < _rtP -> P_79
[ 1U ] ; ri ++ ) { rtb_B_5_23_0 [ _rtP -> P_78 [ ri ] ] += _rtP -> P_25 [ ri
] * _rtX -> StateSpace_CSTATE_n [ 0U ] ; } for ( ri = _rtP -> P_79 [ 1U ] ;
ri < _rtP -> P_79 [ 2U ] ; ri ++ ) { rtb_B_5_23_0 [ _rtP -> P_78 [ ri ] ] +=
_rtP -> P_25 [ ri ] * _rtX -> StateSpace_CSTATE_n [ 1U ] ; } for ( ri = _rtP
-> P_79 [ 2U ] ; ri < _rtP -> P_79 [ 3U ] ; ri ++ ) { rtb_B_5_23_0 [ _rtP ->
P_78 [ ri ] ] += _rtP -> P_25 [ ri ] * _rtX -> StateSpace_CSTATE_n [ 2U ] ; }
rtb_B_5_53_0 = _rtX -> Integrator1_CSTATE_p ; _rtB -> B_5_60_0 = ( ( ( _rtX
-> Integrator1_CSTATE_p - rtb_B_5_23_0 [ 0 ] ) * _rtP -> P_28 + _rtP -> P_29
* rtb_B_5_23_0 [ 1 ] * _rtP -> P_30 ) - rtb_B_5_23_0 [ 2 ] ) * _rtP -> P_31 ;
i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { ssCallAccelRunBlock ( S , 5
, 61 , SS_CALL_MDL_OUTPUTS ) ; } rtb_B_5_25_0 = _rtB -> B_5_60_0 +
rtb_B_5_24_0 ; _rtB -> B_5_64_0 [ 0 ] = _rtP -> P_32 * rtb_B_5_25_0 ; _rtB ->
B_5_64_0 [ 1 ] = _rtB -> B_5_10_0 ; _rtB -> B_5_65_0 = rtb_B_5_53_0 - _rtB ->
B_5_0_0 ; _rtB -> B_5_66_0 = _rtX -> Integrator_CSTATE_j ;
ssCallAccelRunBlock ( S , 1 , 0 , SS_CALL_MDL_OUTPUTS ) ; rtb_B_5_53_0 = _rtB
-> B_5_0_0 - _rtB -> B_5_5_0 ; _rtB -> B_5_71_0 = _rtP -> P_36 * rtb_B_5_53_0
; if ( ( _rtDW -> TimeStampA >= ssGetT ( S ) ) && ( _rtDW -> TimeStampB >=
ssGetT ( S ) ) ) { rtb_B_5_72_0 = 0.0 ; } else { rtb_B_5_72_0 = _rtDW ->
TimeStampA ; lastU = & _rtDW -> LastUAtTimeA ; if ( _rtDW -> TimeStampA <
_rtDW -> TimeStampB ) { if ( _rtDW -> TimeStampB < ssGetT ( S ) ) {
rtb_B_5_72_0 = _rtDW -> TimeStampB ; lastU = & _rtDW -> LastUAtTimeB ; } }
else if ( _rtDW -> TimeStampA >= ssGetT ( S ) ) { rtb_B_5_72_0 = _rtDW ->
TimeStampB ; lastU = & _rtDW -> LastUAtTimeB ; } rtb_B_5_72_0 = ( _rtB ->
B_5_71_0 - * lastU ) / ( ssGetT ( S ) - rtb_B_5_72_0 ) ; } rtb_B_5_24_0 += (
_rtP -> P_34 * rtb_B_5_53_0 + _rtX -> Integrator_CSTATE_a ) + rtb_B_5_72_0 ;
_rtB -> B_5_75_0 = _rtP -> P_37 * rtb_B_5_53_0 ; i = ssIsSampleHit ( S , 1 ,
0 ) ; if ( i != 0 ) { ssCallAccelRunBlock ( S , 5 , 76 , SS_CALL_MDL_OUTPUTS
) ; } _rtB -> B_2_0_0 [ 0 ] = _rtB -> B_5_10_0_c ; _rtB -> B_2_0_0 [ 1 ] =
_rtB -> B_5_10_0_c ; _rtB -> B_2_0_0 [ 2 ] = rtb_B_5_24_0 ; _rtB -> B_2_0_0 [
3 ] = _rtP -> P_38 * rtb_B_5_24_0 ; _rtB -> B_2_0_0 [ 4 ] = _rtB -> B_5_6_0_k
; _rtB -> B_2_0_0 [ 5 ] = _rtB -> B_5_7_0 ; _rtB -> B_2_0_0 [ 6 ] = _rtB ->
B_5_8_0 ; ssCallAccelRunBlock ( S , 2 , 1 , SS_CALL_MDL_OUTPUTS ) ; i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { ssCallAccelRunBlock ( S , 5 ,
79 , SS_CALL_MDL_OUTPUTS ) ; } _rtB -> B_3_0_0 [ 0 ] = _rtB -> B_5_18_0 ;
_rtB -> B_3_0_0 [ 1 ] = _rtB -> B_5_18_0 ; _rtB -> B_3_0_0 [ 2 ] =
rtb_B_5_25_0 ; _rtB -> B_3_0_0 [ 3 ] = _rtP -> P_39 * rtb_B_5_25_0 ; _rtB ->
B_3_0_0 [ 4 ] = _rtB -> B_5_14_0 ; _rtB -> B_3_0_0 [ 5 ] = _rtB -> B_5_15_0_b
; _rtB -> B_3_0_0 [ 6 ] = _rtB -> B_5_16_0 ; ssCallAccelRunBlock ( S , 3 , 1
, SS_CALL_MDL_OUTPUTS ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) {
ssCallAccelRunBlock ( S , 5 , 82 , SS_CALL_MDL_OUTPUTS ) ; } _rtB -> B_4_0_0
[ 0 ] = _rtB -> B_5_26_0_p ; _rtB -> B_4_0_0 [ 1 ] = _rtB -> B_5_26_0_p ;
_rtB -> B_4_0_0 [ 2 ] = rtb_B_5_35_0 ; _rtB -> B_4_0_0 [ 3 ] = _rtP -> P_40 *
rtb_B_5_35_0 ; _rtB -> B_4_0_0 [ 4 ] = _rtB -> B_5_22_0 ; _rtB -> B_4_0_0 [ 5
] = _rtB -> B_5_23_0 ; _rtB -> B_4_0_0 [ 6 ] = _rtB -> B_5_24_0 ;
ssCallAccelRunBlock ( S , 4 , 1 , SS_CALL_MDL_OUTPUTS ) ; i = ssIsSampleHit (
S , 2 , 0 ) ; if ( i != 0 ) { rtb_B_5_38_0 = ( uint8_T ) ( ( uint32_T )
rtb_B_5_38_0 + _rtB -> B_5_30_0_g ) ; if ( rtb_B_5_38_0 > _rtP -> P_81 ) {
_rtB -> B_5_86_0 = _rtB -> B_5_31_0 ; } else { _rtB -> B_5_86_0 =
rtb_B_5_38_0 ; } } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) {
rtb_B_5_38_0 = ( uint8_T ) ( ( uint32_T ) _rtDW -> Output_DSTATE_a + _rtB ->
B_5_33_0 ) ; if ( rtb_B_5_38_0 > _rtP -> P_83 ) { _rtB -> B_5_89_0 = _rtB ->
B_5_34_0 ; } else { _rtB -> B_5_89_0 = rtb_B_5_38_0 ; } _rtB -> B_5_90_0 =
_rtB -> B_5_35_0 [ _rtDW -> Output_DSTATE_a ] ; } UNUSED_PARAMETER ( tid ) ;
} static void mdlOutputsTID4 ( SimStruct * S , int_T tid ) {
B_ADRConSiaAUV_ex01_T * _rtB ; P_ADRConSiaAUV_ex01_T * _rtP ; _rtP = ( (
P_ADRConSiaAUV_ex01_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_ADRConSiaAUV_ex01_T * ) _ssGetModelBlockIO ( S ) ) ; _rtB -> B_5_0_0_m =
_rtP -> P_41 ; _rtB -> B_5_1_0_c = _rtP -> P_42 ; _rtB -> B_5_3_0 = _rtP ->
P_43 ; _rtB -> B_5_4_0 = _rtP -> P_44 ; _rtB -> B_5_6_0_k = _rtP -> P_45 ;
_rtB -> B_5_7_0 = _rtP -> P_46 ; _rtB -> B_5_8_0 = _rtP -> P_47 ; _rtB ->
B_5_10_0_c = _rtP -> P_49 ; _rtB -> B_5_12_0 = _rtP -> P_50 ; _rtB ->
B_5_13_0 = _rtP -> P_51 ; _rtB -> B_5_14_0 = _rtP -> P_52 ; _rtB ->
B_5_15_0_b = _rtP -> P_53 ; _rtB -> B_5_16_0 = _rtP -> P_54 ; _rtB ->
B_5_18_0 = _rtP -> P_56 ; _rtB -> B_5_20_0 = _rtP -> P_57 ; _rtB -> B_5_21_0
= _rtP -> P_58 ; _rtB -> B_5_22_0 = _rtP -> P_59 ; _rtB -> B_5_23_0 = _rtP ->
P_60 ; _rtB -> B_5_24_0 = _rtP -> P_61 ; memcpy ( & _rtB -> B_5_9_0 [ 0 ] , &
_rtP -> P_48 [ 0 ] , 12U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_5_17_0
[ 0 ] , & _rtP -> P_55 [ 0 ] , 12U * sizeof ( real_T ) ) ; memcpy ( & _rtB ->
B_5_25_0 [ 0 ] , & _rtP -> P_62 [ 0 ] , 12U * sizeof ( real_T ) ) ; _rtB ->
B_5_26_0_p = _rtP -> P_63 ; _rtB -> B_5_28_0_c = _rtP -> P_64 ; _rtB ->
B_5_29_0_f = _rtP -> P_65 ; _rtB -> B_5_30_0_g = _rtP -> P_84 ; _rtB ->
B_5_31_0 = _rtP -> P_85 ; _rtB -> B_5_33_0 = _rtP -> P_86 ; _rtB -> B_5_34_0
= _rtP -> P_87 ; _rtB -> B_5_32_0 [ 0 ] = _rtP -> P_66 [ 0 ] ; _rtB ->
B_5_35_0 [ 0 ] = _rtP -> P_67 [ 0 ] ; _rtB -> B_5_32_0 [ 1 ] = _rtP -> P_66 [
1 ] ; _rtB -> B_5_35_0 [ 1 ] = _rtP -> P_67 [ 1 ] ; UNUSED_PARAMETER ( tid )
; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { B_ADRConSiaAUV_ex01_T *
_rtB ; DW_ADRConSiaAUV_ex01_T * _rtDW ; P_ADRConSiaAUV_ex01_T * _rtP ; real_T
* lastU ; int32_T isHit ; _rtDW = ( ( DW_ADRConSiaAUV_ex01_T * )
ssGetRootDWork ( S ) ) ; _rtP = ( ( P_ADRConSiaAUV_ex01_T * ) ssGetModelRtp (
S ) ) ; _rtB = ( ( B_ADRConSiaAUV_ex01_T * ) _ssGetModelBlockIO ( S ) ) ; {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK . Head = ( ( _rtDW -> TransportDelay_IWORK . Head < (
_rtDW -> TransportDelay_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK
. Head == _rtDW -> TransportDelay_IWORK . Tail ) { if ( !
ADRConSiaAUV_ex01_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK . CircularBufSize , & _rtDW -> TransportDelay_IWORK .
Tail , & _rtDW -> TransportDelay_IWORK . Head , & _rtDW ->
TransportDelay_IWORK . Last , simTime - _rtP -> P_0 , uBuffer , ( boolean_T )
0 , false , & _rtDW -> TransportDelay_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
uBuffer + _rtDW -> TransportDelay_IWORK . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK . Head ] = _rtB -> B_5_90_0 ; } _rtDW ->
Integrator_IWORK = 0 ; _rtDW -> Integrator_IWORK_k = 0 ; _rtDW ->
Integrator_IWORK_e = 0 ; ssCallAccelRunBlock ( S , 5 , 30 ,
SS_CALL_MDL_UPDATE ) ; isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0
) { _rtDW -> Output_DSTATE = _rtB -> B_5_86_0 ; } if ( _rtDW -> TimeStampA ==
( rtInf ) ) { _rtDW -> TimeStampA = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeA ; } else if ( _rtDW -> TimeStampB == ( rtInf ) ) { _rtDW ->
TimeStampB = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB ; } else if (
_rtDW -> TimeStampA < _rtDW -> TimeStampB ) { _rtDW -> TimeStampA = ssGetT (
S ) ; lastU = & _rtDW -> LastUAtTimeA ; } else { _rtDW -> TimeStampB = ssGetT
( S ) ; lastU = & _rtDW -> LastUAtTimeB ; } * lastU = _rtB -> B_5_71_0 ;
isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Output_DSTATE_a = _rtB -> B_5_89_0 ; } UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdateTID4 ( SimStruct * S , int_T tid ) { UNUSED_PARAMETER (
tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { B_ADRConSiaAUV_ex01_T * _rtB ;
P_ADRConSiaAUV_ex01_T * _rtP ; XDot_ADRConSiaAUV_ex01_T * _rtXdot ;
X_ADRConSiaAUV_ex01_T * _rtX ; uint32_T ri ; _rtXdot = ( (
XDot_ADRConSiaAUV_ex01_T * ) ssGetdX ( S ) ) ; _rtX = ( (
X_ADRConSiaAUV_ex01_T * ) ssGetContStates ( S ) ) ; _rtP = ( (
P_ADRConSiaAUV_ex01_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_ADRConSiaAUV_ex01_T * ) _ssGetModelBlockIO ( S ) ) ; memcpy ( & _rtXdot ->
Integrator_CSTATE [ 0 ] , & _rtB -> B_2_1_1 [ 0 ] , 12U * sizeof ( real_T ) )
; memcpy ( & _rtXdot -> Integrator_CSTATE_i [ 0 ] , & _rtB -> B_3_1_1 [ 0 ] ,
12U * sizeof ( real_T ) ) ; memcpy ( & _rtXdot -> Integrator_CSTATE_p [ 0 ] ,
& _rtB -> B_4_1_1 [ 0 ] , 12U * sizeof ( real_T ) ) ; _rtXdot ->
StateSpace_CSTATE [ 0 ] = 0.0 ; _rtXdot -> StateSpace_CSTATE [ 1 ] = 0.0 ;
_rtXdot -> StateSpace_CSTATE [ 2 ] = 0.0 ; for ( ri = _rtP -> P_69 [ 0U ] ;
ri < _rtP -> P_69 [ 1U ] ; ri ++ ) { _rtXdot -> StateSpace_CSTATE [ _rtP ->
P_68 [ ri ] ] += _rtP -> P_5 [ ri ] * _rtX -> StateSpace_CSTATE [ 0U ] ; }
for ( ri = _rtP -> P_69 [ 1U ] ; ri < _rtP -> P_69 [ 2U ] ; ri ++ ) { _rtXdot
-> StateSpace_CSTATE [ _rtP -> P_68 [ ri ] ] += _rtP -> P_5 [ ri ] * _rtX ->
StateSpace_CSTATE [ 1U ] ; } for ( ri = _rtP -> P_69 [ 2U ] ; ri < _rtP ->
P_69 [ 3U ] ; ri ++ ) { _rtXdot -> StateSpace_CSTATE [ _rtP -> P_68 [ ri ] ]
+= _rtP -> P_5 [ ri ] * _rtX -> StateSpace_CSTATE [ 2U ] ; } for ( ri = _rtP
-> P_71 [ 0U ] ; ri < _rtP -> P_71 [ 1U ] ; ri ++ ) { _rtXdot ->
StateSpace_CSTATE [ _rtP -> P_70 [ ri ] ] += _rtP -> P_6 [ ri ] * _rtB ->
B_5_45_0 [ 0U ] ; } for ( ri = _rtP -> P_71 [ 1U ] ; ri < _rtP -> P_71 [ 2U ]
; ri ++ ) { _rtXdot -> StateSpace_CSTATE [ _rtP -> P_70 [ ri ] ] += _rtP ->
P_6 [ ri ] * _rtB -> B_5_45_0 [ 1U ] ; } _rtXdot -> Integrator1_CSTATE = _rtB
-> B_5_27_0 ; _rtXdot -> Integrator_CSTATE_c = _rtB -> B_0_0_1 ;
ssCallAccelRunBlock ( S , 5 , 30 , SS_CALL_MDL_DERIVATIVES ) ; _rtXdot ->
StateSpace_CSTATE_n [ 0 ] = 0.0 ; _rtXdot -> StateSpace_CSTATE_n [ 1 ] = 0.0
; _rtXdot -> StateSpace_CSTATE_n [ 2 ] = 0.0 ; for ( ri = _rtP -> P_75 [ 0U ]
; ri < _rtP -> P_75 [ 1U ] ; ri ++ ) { _rtXdot -> StateSpace_CSTATE_n [ _rtP
-> P_74 [ ri ] ] += _rtP -> P_23 [ ri ] * _rtX -> StateSpace_CSTATE_n [ 0U ]
; } for ( ri = _rtP -> P_75 [ 1U ] ; ri < _rtP -> P_75 [ 2U ] ; ri ++ ) {
_rtXdot -> StateSpace_CSTATE_n [ _rtP -> P_74 [ ri ] ] += _rtP -> P_23 [ ri ]
* _rtX -> StateSpace_CSTATE_n [ 1U ] ; } for ( ri = _rtP -> P_75 [ 2U ] ; ri
< _rtP -> P_75 [ 3U ] ; ri ++ ) { _rtXdot -> StateSpace_CSTATE_n [ _rtP ->
P_74 [ ri ] ] += _rtP -> P_23 [ ri ] * _rtX -> StateSpace_CSTATE_n [ 2U ] ; }
for ( ri = _rtP -> P_77 [ 0U ] ; ri < _rtP -> P_77 [ 1U ] ; ri ++ ) { _rtXdot
-> StateSpace_CSTATE_n [ _rtP -> P_76 [ ri ] ] += _rtP -> P_24 [ ri ] * _rtB
-> B_5_64_0 [ 0U ] ; } for ( ri = _rtP -> P_77 [ 1U ] ; ri < _rtP -> P_77 [
2U ] ; ri ++ ) { _rtXdot -> StateSpace_CSTATE_n [ _rtP -> P_76 [ ri ] ] +=
_rtP -> P_24 [ ri ] * _rtB -> B_5_64_0 [ 1U ] ; } _rtXdot ->
Integrator1_CSTATE_p = _rtB -> B_5_66_0 ; _rtXdot -> Integrator_CSTATE_j =
_rtB -> B_1_0_1 ; _rtXdot -> Integrator_CSTATE_a = _rtB -> B_5_75_0 ; }
static void mdlInitializeSizes ( SimStruct * S ) { ssSetChecksumVal ( S , 0 ,
4068830705U ) ; ssSetChecksumVal ( S , 1 , 1161573688U ) ; ssSetChecksumVal (
S , 2 , 141597552U ) ; ssSetChecksumVal ( S , 3 , 1666190284U ) ; { mxArray *
slVerStructMat = NULL ; mxArray * slStrMat = mxCreateString ( "simulink" ) ;
char slVerChar [ 10 ] ; int status = mexCallMATLAB ( 1 , & slVerStructMat , 1
, & slStrMat , "ver" ) ; if ( status == 0 ) { mxArray * slVerMat = mxGetField
( slVerStructMat , 0 , "Version" ) ; if ( slVerMat == NULL ) { status = 1 ; }
else { status = mxGetString ( slVerMat , slVerChar , 10 ) ; } }
mxDestroyArray ( slStrMat ) ; mxDestroyArray ( slVerStructMat ) ; if ( (
status == 1 ) || ( strcmp ( slVerChar , "10.3" ) != 0 ) ) { return ; } }
ssSetOptions ( S , SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork (
S ) != sizeof ( DW_ADRConSiaAUV_ex01_T ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file." ) ; } if ( ssGetSizeofGlobalBlockIO ( S
) != sizeof ( B_ADRConSiaAUV_ex01_T ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file." ) ; } { int ssSizeofParams ;
ssGetSizeofParams ( S , & ssSizeofParams ) ; if ( ssSizeofParams != sizeof (
P_ADRConSiaAUV_ex01_T ) ) { static char msg [ 256 ] ; sprintf ( msg ,
"Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file." ) ; } } _ssSetModelRtp ( S , ( real_T *
) & ADRConSiaAUV_ex01_rtDefaultP ) ; rt_InitInfAndNaN ( sizeof ( real_T ) ) ;
} static void mdlInitializeSampleTimes ( SimStruct * S ) { { SimStruct *
childS ; SysOutputFcn * callSysFcns ; childS = ssGetSFunction ( S , 0 ) ;
callSysFcns = ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ]
= ( SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 1 ) ; callSysFcns
= ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 2 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 3 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 4 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; } slAccRegPrmChangeFcn ( S , mdlOutputsTID4 ) ; }
static void mdlTerminate ( SimStruct * S ) { }
#include "simulink.c"
