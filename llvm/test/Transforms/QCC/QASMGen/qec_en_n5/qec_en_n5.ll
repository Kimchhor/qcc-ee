; RUN: opt -passes=qasm-gen %s --disable-output &> %t
; RUN: diff --strip-trailing-cr %t %S/qec_en_n5.qasm

define dso_local i32 @main() {

;qreg q[5];
%q0 = alloca q1
%q1 = alloca q1
%q2 = alloca q1
%q3 = alloca q1
%q4 = alloca q1

;creg c[5];
%c0 = alloca i1
%c1 = alloca i1
%c2 = alloca i1
%c3 = alloca i1
%c4 = alloca i1

%q20 = h q1* %q2 ;h q[2];
%q21 = t q1* %q20 ;t q[2];
%q22 = h q1* %q21 ;h q[2];

%q00 = h q1* %q0 ;h q[0];
%q10 = h q1* %q1 ;h q[1];
%q23 = h q1* %q22 ;h q[2];

;cx q[1], q[2];
%q24 = cx0 q1* %q10, i32 1
%q25 = cx1 q1* %q23, i32 1

;cx q[0], q[2];
%q26 = cx0 q1* %q00, i32 2
%q27 = cx1 q1* %q25, i32 2

%q01 = h q1* %q00 ;h q[0];
%q11= h q1* %q10 ;h q[1];
%q30 = h q1* %q3 ;h q[3];

;cx q[3], q[2];
%q28 = cx0 q1* %q30, i32 3
%q29 = cx1 q1* %q27, i32 3

%q2A = h q1* %q29 ;h q[2];
%q31 = h q1* %q30  ;h q[3];

;cx q[3], q[2];
%q2B = cx0 q1* %q31, i32 4
%q2C = cx1 q1* %q2A, i32 4

;cx q[0], q[2];
%q2D = cx0 q1* %q01, i32 5
%q2E = cx1 q1* %q2C, i32 5

;cx q[1], q[2];
%q2F = cx0 q1* %q11, i32 6
%q2G = cx1 q1* %q2E, i32 6

%q2H = h q1* %q2G ;h q[2];
%q40 = h q1* %q4 ;h q[4];

;cx q[4], q[2];
%q2I = cx0 q1* %q40, i32 7
%q2J = cx1 q1* %q2H, i32 7

%q2K = h q1* %q2J ;h q[2];
%q41 = h q1* %q40 ;h q[4];

;cx q[4], q[2];
%q2L = cx0 q1* %q41, i32 8
%q2M = cx1 q1* %q2K, i32 8

;cx q[1], q[2];
%q2N = cx0 q1* %q11, i32 9
%q2O = cx1 q1* %q2M, i32 9

;cx q[3], q[2];
%q2P = cx0 q1* %q31, i32 10
%q2Q = cx1 q1* %q2O, i32 10

%q2R = measure q1* %q2Q, i1* %c2 ;measure q[2] -> c[2];
%q42 = measure q1* %q41, i1* %c4 ;measure q[4] -> c[4];
%q02 = measure q1* %q01, i1* %c0 ;measure q[0] -> c[0];
%q12 = measure q1* %q11, i1* %c1 ;measure q[1] -> c[1];
%q32 = measure q1* %q31, i1* %c3 ;measure q[3] -> c[3];
ret i32 0
}