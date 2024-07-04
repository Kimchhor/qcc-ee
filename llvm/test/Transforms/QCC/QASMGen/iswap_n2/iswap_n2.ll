; RUN: opt -passes=qasm-gen %s --disable-output &> %t
; RUN: diff --strip-trailing-cr %t %S/iswap_n2.qasm

define dso_local i32 @main() {

;qreg q[2];
%q0 = alloca q1
%q1 = alloca q1

;creg c[2];
%c0 = alloca i1
%c1 = alloca i1

%q00 = x q1* %q0 ;x q[0];
%q01 = s q1* %q00 ;s q[0];
%q10 = s q1* %q1 ;s q[1];
%q02 = h q1* %q01 ;h q[0];

;cx q[0],q[1];
%q11 = cx0 q1* %q02, i32 1 
%q12 = cx1 q1* %q10, i32 1 

%q03 = h q1* %q02 ;h q[0];
%q13 = h q1* %q12 ;h q[1];

;cx q[0],q[1];
%q14 = cx0 q1* %q03, i32 2 
%q15 = cx1 q1* %q13, i32 2

%q04 = h q1* %q03 ;h q[0];

%q05 = measure q1* %q04, i1* %c0  ;measure q[0] -> c[0];
%q16 = measure q1* %q15, i1* %c1  ;measure q[1] -> c[1];
ret i32 0
}