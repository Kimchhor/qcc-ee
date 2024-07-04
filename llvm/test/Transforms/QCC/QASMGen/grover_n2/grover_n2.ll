; RUN: opt -passes=qasm-gen %s --disable-output &> %t
; RUN: diff --strip-trailing-cr %t %S/grover_n2.qasm

define dso_local i32 @main() {

;qreg q[2];
%q0 = alloca q1
%q1 = alloca q1

;creg c[2];
%c0 = alloca i1
%c1 = alloca i1

%q00 = h q1* %q0 ;h q[0];
%q10 = h q1* %q1 ;h q[1];

%q11 = h q1* %q10 ;h q[1];

;cx q[0],q[1];
%q12 = cx0 q1* %q00, i32 1
%q13 = cx1 q1* %q11, i32 1

%q14 = h q1* %q13 ;h q[1];

%q01 = h q1* %q00 ;h q[0];
%q15 = h q1* %q14 ;h q[1];
%q02 = x q1* %q01 ;x q[0];
%q16 = x q1* %q15 ;x q[1];
%q17 = h q1* %q16 ;h q[1];

;cx q[0],q[1];
%q18 = cx0 q1* %q02, i32 2
%q19 = cx1 q1* %q17, i32 2

%q1A = h q1* %q19 ;h q[1];
%q03 = x q1* %q02 ;x q[0];
%q1B = x q1* %q1A ;x q[1];

%q04 = h q1* %q03;h q[0];
%q1C = h q1* %q1B ;h q[1];

%q05 = measure q1* %q04, i1* %c0 ;measure q[0] -> c[0];
%q1D = measure q1* %q1C, i1* %c1 ;measure q[1] -> c[1];
ret i32 0
}
