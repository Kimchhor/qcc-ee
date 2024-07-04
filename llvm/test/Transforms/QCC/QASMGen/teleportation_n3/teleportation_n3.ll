; RUN: opt -passes=qasm-gen %s --disable-output &> %t
; RUN: diff --strip-trailing-cr %t %S/teleportation_n3.qasm

define dso_local i32 @main() {

;qreg q[3];
%q0 = alloca q1
%q1 = alloca q1
%q2 = alloca q1

;creg c[3];
%c0 = alloca i1
%c1 = alloca i1
%c2 = alloca i1

%q00 = h q1* %q0 ;h q[0];
%q01 = t q1* %q00 ;t q[0];
%q02 = h q1* %q01 ;h q[0];
%q20 = h q1* %q2 ;h q[2];
%q03 = s q1* %q02;s q[0];

;cx q[2],q[1];
%q10 = cx0 q1* %q20, i32 1
%q11 = cx1 q1* %q1, i32 1 

;cx q[0],q[1];
%q12 = cx0 q1* %q03, i32 2
%q13 = cx1 q1* %q11, i32 2

%q04 = h q1* %q03 ;h q[0];

%q05 = measure q1* %q04, i1* %c0 ;measure q[0] -> c[0];
%q14 = measure q1* %q13, i1* %c1 ;measure q[1] -> c[1];
%q21 = measure q1* %q20, i1* %c2 ;measure q[2] -> c[2];
ret i32 0
}