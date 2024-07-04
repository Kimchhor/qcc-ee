; RUN: opt -passes=qasm-gen %s --disable-output &> %t
; RUN: diff --strip-trailing-cr %t %S/error_correctiond3_n5.qasm

define dso_local i32 @main() {

;qreg q[5];
%q0 = alloca q1
%q1 = alloca q1
%q20 = alloca q1
%q3 = alloca q1
%q4 = alloca q1

;creg c[5];
%c0 = alloca i1
%c1 = alloca i1
%c2 = alloca i1
%c3 = alloca i1
%c4 = alloca i1

%q00 = h q1* %q0 ;h q[0];
%q10 = h q1* %q1 ;h q[1];
%q30 = h q1* %q3 ;h q[3];
%q40 = h q1* %q4 ;h q[4];

;cx q[1],q[2];
%q21 = cx0 q1* %q10, i32 1 
%q22 = cx1 q1* %q20, i32 1

%q11 = h q1* %q10 ;h q[1];
%q23 = h q1* %q22 ;h q[2];

;cx q[1],q[2];
%q24 = cx0 q1* %q11, i32 2
%q25 = cx1 q1* %q23, i32 2

%q12 = h q1* %q11 ;h q[1];
%q26 = h q1* %q25 ;h q[2];

;cx q[1],q[2];
%q27 = cx0 q1* %q12, i32 3
%q28 = cx1 q1* %q26, i32 3

;cx q[4],q[2];
%q29 = cx0 q1* %q40, i32 4
%q2A = cx1 q1* %q28, i32 4

;cx q[1],q[2];
%q2B = cx0 q1* %q12, i32 5
%q2C = cx1 q1* %q2A, i32 5

%q13 = h q1* %q12 ;h q[1];
%q2D = h q1* %q2C ;h q[2];

;cx q[1],q[2];
%q2E = cx0 q1* %q13, i32 6
%q2F = cx1 q1* %q2D, i32 6

%q14 = h q1* %q13 ;h q[1];
%q2G = h q1* %q2F ;h q[2];

;cx q[1],q[2];
%q2H = cx0 q1* %q14, i32 7
%q2I = cx1 q1* %q2G, i32 7

%q41 = sdg q1* %q40 ;sdg q[4];

;cx q[4],q[2];
%q2J = cx0 q1* %q41, i32 8
%q2K = cx1 q1* %q2I, i32 8

%q2L = h q1* %q2K ;h q[2];

;cx q[4],q[2];
%q2M = cx0 q1* %q41, i32 9
%q2N = cx1 q1* %q2L, i32 9

%q2O = h q1* %q2N ;h q[2];

;cx q[0],q[2];
%q2P = cx0 q1* %q00, i32 10
%q2Q = cx1 q1* %q2O, i32 10

%q01 = h q1* %q00 ;h q[0];
%q2R = h q1* %q2Q ;h q[2];

;cx q[0],q[2];
%q2S = cx0 q1* %q01, i32 11
%q2T = cx1 q1* %q2R, i32 11

%q02 = h q1* %q01 ;h q[0];
%q2U = h q1* %q2T ;h q[2];

;cx q[0],q[2];
%q2V = cx0 q1* %q02, i32 12
%q2W = cx1 q1* %q2U, i32 12

;cx q[3],q[2];
%q2X = cx0 q1* %q30, i32 13
%q2Y = cx1 q1* %q2W, i32 13

;cx q[0],q[2];
%q2Z = cx0 q1* %q02, i32 14
%q200 = cx1 q1* %q2Y, i32 14

%q03 = h q1* %q02 ;h q[0];
%q201 = h q1* %q200 ;h q[2];

;cx q[0],q[2];
%q202 = cx0 q1* %q03, i32 15
%q203 = cx1 q1* %q201, i32 15

%q04 = h q1* %q03 ;h q[0];
%q204 = h q1* %q203 ;h q[2];

;cx q[0],q[2];
%q205 = cx0 q1* %q04, i32 16
%q206 = cx1 q1* %q204, i32 16

;cx q[1],q[2];
%q207 = cx0 q1* %q14, i32 17
%q208 = cx1 q1* %q206, i32 17

%q15 = h q1* %q14 ;h q[1];
%q209 = h q1* %q208 ;h q[2];

;cx q[1],q[2];
%q20A = cx0 q1* %q15, i32 18
%q20B = cx1 q1* %q209, i32 18

%q16 = h q1* %q15 ;h q[1];
%q20C = h q1* %q20B ;h q[2];

;cx q[1],q[2];
%q20D = cx0 q1* %q16, i32 19 
%q20E = cx1 q1* %q20C, i32 19

;cx q[3],q[2];
%q20F = cx0 q1* %q30, i32 20
%q20G = cx1 q1* %q20E, i32 20

;cx q[1],q[2];
%q20H = cx0 q1* %q16, i32 21
%q20I = cx1 q1* %q20G, i32 21

%q17 = h q1* %q16 ;h q[1];
%q20J = h q1* %q20I ;h q[2];

;cx q[1],q[2];
%q20K = cx0 q1* %q17, i32 22
%q20L = cx1 q1* %q20J, i32 22

%q18 = h q1* %q17 ;h q[1];
%q20M = h q1* %q20L ;h q[2];

;cx q[1],q[2];
%q20N = cx0 q1* %q18, i32 23
%q20O = cx1 q1* %q20M, i32 23

;cx q[3],q[2];
%q20P = cx0 q1* %q30, i32 24
%q20Q = cx1 q1* %q20O, i32 24

;cx q[0],q[2];
%q20R = cx0 q1* %q04, i32 25
%q20S = cx1 q1* %q20Q, i32 25

%q31 = h q1* %q30 ;h q[3];
%q42 = h q1* %q41 ;h q[4];

;cx q[3],q[2];
%q20T = cx0 q1* %q31, i32 26
%q20V = cx1 q1* %q20S, i32 26

%q20W = h q1* %q20V ;h q[2];
%q32 = h q1* %q31 ;h q[3];

;cx q[3],q[2];
%q20X = cx0 q1* %q32, i32 27
%q20Y = cx1 q1* %q20W, i32 27

%q20Z = h q1* %q20Y ;h q[2];
%q33 = h q1* %q32 ;h q[3];

;cx q[3],q[2];
%q2000 = cx0 q1* %q33, i32 28
%q2001 = cx1 q1* %q20Z, i32 28

;cx q[0],q[2];
%q2002 = cx0 q1* %q04, i32 29
%q2003 = cx1 q1* %q2001, i32 29

;cx q[3],q[2];
%q2004 = cx0 q1* %q33, i32 30
%q2005 = cx1 q1* %q2003, i32 30

%q2006 = h q1* %q2005 ;h q[2];
%q34 = h q1* %q33 ;h q[3];

;cx q[3],q[2];
%q2007 = cx0 q1* %q34, i32 31
%q2008 = cx1 q1* %q2006, i32 31

%q2009 = h q1* %q2008 ;h q[2];
%q35 = h q1* %q34 ;h q[3];

;cx q[3],q[2];
%q200A = cx0 q1* %q35, i32 32
%q200B = cx1 q1* %q2009, i32 32

;cx q[4],q[2];
%q200C = cx0 q1* %q42, i32 33
%q200D = cx1 q1* %q200B, i32 33

%q200E = h q1* %q200D ;h q[2];
%q43 = h q1* %q42 ;h q[4];

;cx q[4],q[2];
%q200F = cx0 q1* %q43, i32 34
%q200G = cx1 q1* %q200E, i32 34

%q200H = h q1* %q200G ;h q[2];
%q44 = h q1* %q43 ;h q[4];

;cx q[4],q[2];
%q200I = cx0 q1* %q44, i32 35
%q200J = cx1 q1* %q200H, i32 35

;cx q[0],q[2];
%q200K = cx0 q1* %q04, i32 36
%q200L = cx1 q1* %q200J, i32 36

;cx q[4],q[2];
%q200M = cx0 q1* %q44, i32 37
%q200N = cx1 q1* %q200L, i32 37

%q200O = h q1* %q200N ;h q[2];
%q45 = h q1* %q44 ;h q[4];

;cx q[4],q[2];
%q200P = cx0 q1* %q45, i32 38
%q200Q = cx1 q1* %q200O, i32 38

%q200R = h q1* %q200Q ;h q[2];
%q46 = h q1* %q45 ;h q[4];

;cx q[4],q[2];
%q200S = cx0 q1* %q46, i32 39
%q200T = cx1 q1* %q200R, i32 39

;cx q[1],q[2] ;
%q200U = cx0 q1* %q18, i32 40
%q200V = cx1 q1* %q200T, i32 40

%q200W = h q1* %q200V ;h q[2];

;cx q[1],q[2];
%q200X = cx0 q1* %q18, i32 41
%q200Y = cx1 q1* %q200W, i32 41

%q19 = h q1* %q18 ;h q[1];

;cx q[4],q[2];
%q200Z = cx0 q1* %q46, i32 42
%q20000 = cx1 q1* %q200Y, i32 42

;cx q[0],q[1];
%q1A = cx0 q1* %q04, i32 43
%q1B = cx1 q1* %q19, i32 43

%q20001 = h q1* %q20000 ;h q[2];
%q47 = h q1* %q46 ;h q[4];
%q1C = h q1* %q1B ;h q[1];

;cx q[4],q[2];
%q20002 = cx0 q1* %q47, i32 44
%q20003 = cx1 q1* %q20001, i32 44

%q20004 = h q1* %q20003 ;h q[2];
%q48 = h q1* %q47 ;h q[4];

;cx q[4],q[2];
%q20005 = cx0 q1* %q48, i32 45
%q20006 = cx1 q1* %q20004, i32 45

;cx q[1],q[2];
%q20007 = cx0 q1* %q1C, i32 46
%q20008 = cx1 q1* %q20006, i32 46

;cx q[4],q[2];
%q20009 = cx0 q1* %q48, i32 47
%q2000A = cx1 q1* %q20008, i32 47

%q2000B = h q1* %q2000A ;h q[2];
%q49 = h q1* %q48 ;h q[4];

;cx q[4],q[2];
%q2000C = cx0 q1* %q49, i32 48
%q2000D = cx1 q1* %q2000B, i32 48

%q2000E = h q1* %q2000D  ;h q[2];
%q4A = h q1* %q49 ;h q[4];

;cx q[4],q[2];
%q2000F = cx0 q1* %q4A, i32 49
%q2000G = cx1 q1* %q2000E, i32 49

%q1D = sdg q1* %q1C ;sdg q[1];
%q2000H = h q1* %q2000G ;h q[2];
%q36 = h q1* %q35 ;h q[3];
%q4B = h q1* %q4A ;h q[4];

%q05 = measure q1* %q04, i1* %c0 ;measure q[0] -> c[0];
%q1E = measure q1* %q1D, i1* %c1 ;measure q[1] -> c[1];
%q2000I = measure q1* %q2000H, i1* %c2 ;measure q[2] -> c[2];
%q37 = measure q1* %q36, i1* %c3 ;measure q[3] -> c[3];
%q4C = measure q1* %q4B, i1* %c4 ;measure q[4] -> c[4];
ret i32 0
}