; RUN: opt -passes=qasm-gen %s --disable-output &> %t
; RUN: diff --strip-trailing-cr %t %S/lpn_n5.qasm

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

%q00 = h q1* %q0 ;h q[0];
%q10 = h q1* %q1 ;h q[1];
%q30 = h q1* %q3 ;h q[3];
%q40 = h q1* %q4;h q[4];

;cx q[3], q[2];
%q20 = cx0 q1* %q30, i32 1
%q21 = cx1 q1* %q2, i32 1

;cx q[0], q[2];
%q22 = cx0 q1* %q00, i32 2 ; cx q[2],q[3];
%q23 = cx1 q1* %q21, i32 2

%q01 = h q1* %q00 ;h q[0];
%q11 = h q1* %q10 ;h q[1];
%q24 = h q1* %q23 ;h q[2];
%q31 = h q1* %q30 ;h q[3];
%q41 = h q1* %q40 ;h q[4];

%q02 = measure q1* %q01, i1* %c0 ;measure q[0] -> c[0];
%q12 = measure q1* %q11, i1* %c1 ;measure q[1] -> c[1];
%q25 = measure q1* %q24, i1* %c2 ;measure q[2] -> c[2];
%q32 = measure q1* %q31, i1* %c3 ;measure q[3] -> c[3];
%q42 = measure q1* %q41, i1* %c4 ;measure q[4] -> c[4];
ret i32 0
}