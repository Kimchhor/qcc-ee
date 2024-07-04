; RUN: opt -passes=qasm-gen %s --disable-output &> %t
; RUN: diff --strip-trailing-cr %t %S/fredkin_n3.qasm

define dso_local i32 @main() {

;qreg q[3];
%q0 = alloca q1
%q1 = alloca q1
%q2 = alloca q1

;creg c[3];
%c0 = alloca i1
%c1 = alloca i1
%c2 = alloca i1

%q00 = x q1* %q0 ;x q[0];
%q10 = x q1* %q1 ;x q[1];

;cx q[2],q[1];
%q11 = cx0 q1* %q2, i32 1
%q12 = cx1 q1* %q10, i32 1

;cx q[0],q[1];
%q13 = cx0 q1* %q00, i32 2
%q14 = cx1 q1* %q12, i32 2

%q20 = h q1* %q2 ;h q[2];
%q01 = t q1* %q00 ;t q[0];
%q15 = tdg q1* %q14 ;tdg q[1];
%q21 = t q1* %q20 ;t q[2];

;cx q[2],q[1];
%q16 = cx0 q1* %q21, i32 3
%q17 = cx1 q1* %q15, i32 3

;cx q[0],q[2];
%q22 = cx0 q1* %q01, i32 4
%q23 = cx1 q1* %q21, i32 4

%q18 = t q1* %q17 ;t q[1];

;cx q[0],q[1];
%q19 = cx0 q1* %q01, i32 5
%q1A = cx1 q1* %q18, i32 5

%q24 = tdg q1* %q23 ;tdg q[2];
%q1B = tdg q1* %q1A ;tdg q[1];

;cx q[0],q[2];
%q25 = cx0 q1* %q01, i32 6
%q26 = cx1 q1* %q24, i32 6

;cx q[2],q[1];
%q1C = cx0 q1* %q26, i32 7
%q1D = cx1 q1* %q1B, i32 7

%q1E = t q1* %q1D ;t q[1];
%q27 = h q1* %q26 ;h q[2];

;cx q[2],q[1];
%q1F = cx0 q1* %q27, i32 8
%q1G = cx1 q1* %q1E, i32 8

%q02 = measure q1* %q01, i1* %c0 ;measure q[0] -> c[0];
%q1H = measure q1* %q1G, i1* %c1 ;measure q[1] -> c[1];
%q28 = measure q1* %q27, i1* %c2 ;measure q[2] -> c[2];
ret i32 0
}