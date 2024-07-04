; RUN: opt -passes=qasm-gen %s --disable-output &> %t
; RUN: diff --strip-trailing-cr %t %S/qrng_n4.qasm

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

%q01 = measure q1* %q00, i1* %c0 ;measure q[0] -> c[0];
%q11 = measure q1* %q10, i1* %c1 ;measure q[1] -> c[1];
%q21 = measure q1* %q20, i1* %c2 ;measure q[2] -> c[2];
%q31 = measure q1* %q30, i1* %c3 ;measure q[3] -> c[3];
ret i32 0
}