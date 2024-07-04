; RUN: opt -passes=qasm-gen %s --disable-output &> %t
; RUN: diff --strip-trailing-cr %t %S/hs4_n4.qasm

define dso_local i32 @main() {
;qreg q[4];
%q0 = alloca q1
%q1 = alloca q1
%q2 = alloca q1
%q3 = alloca q1

;creg c[4];
%c0 = alloca i1
%c1 = alloca i1
%c2 = alloca i1
%c3 = alloca i1

%q00 = h q1* %q0 ;h q[0];
%q10 = h q1* %q1 ;h q[1];
%q20 = h q1* %q2 ;h q[2];
%q30 = h q1* %q3 ;h q[3];

%q01 = x q1* %q00 ;x q[0];
%q21 = x q1* %q20 ;x q[2];
%q11 = h q1* %q10 ;h q[1];
%q31 = h q1* %q30 ;h q[3];

;cx q[0],q[1];
%q12 = cx0 q1* %q01, i32 1 
%q13 = cx1 q1* %q11, i32 1

;cx q[2],q[3];
%q32 = cx0 q1* %q21, i32 2 
%q33 = cx1 q1* %q31, i32 2 

%q14 = h q1* %q13 ;h q[1];
%q34 = h q1* %q33 ;h q[3];
%q02 = x q1* %q01 ;x q[0];
%q22 = x q1* %q21 ;x q[2];
%q03 = h q1* %q02 ;h q[0];
%q15 = h q1* %q14 ;h q[1];
%q23 = h q1* %q22 ;h q[2];
%q35 = h q1* %q34 ;h q[3];
%q16 = h q1* %q15 ;h q[1];
%q36 = h q1* %q35 ;h q[3];

;cx q[0],q[1];
%q17 = cx0 q1* %q03, i32 3
%q18 = cx1 q1* %q16, i32 3

;cx q[2],q[3];
%q37 = cx0 q1* %q23, i32 4
%q38 = cx1 q1* %q36, i32 4

%q19 = h q1* %q18 ;h q[1];
%q39 = h q1* %q38 ;h q[3];
%q04 = h q1* %q03 ;h q[0];
%q1A = h q1* %q19 ;h q[1];
%q24 = h q1* %q23 ;h q[2];
%q3A = h q1* %q39 ;h q[3];

%q05 = measure q1* %q04, i1* %c0 ;measure q[0] -> c[0];
%q1B = measure q1* %q1A, i1* %c1 ;measure q[1] -> c[1];
%q25 = measure q1* %q24, i1* %c2 ;measure q[2] -> c[2];
%q3B = measure q1* %q3A, i1* %c3 ;measure q[3] -> c[3];
ret i32 0
}