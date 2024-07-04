; RUN: opt -passes=qasm-gen %s --disable-output &> %t
; RUN: diff --strip-trailing-cr %t %S/cat_state_n4.qasm

define dso_local i32 @main() {

;qreg bits[4];
%q0 = alloca q1
%q1 = alloca q1
%q2 = alloca q1
%q3 = alloca q1

;creg c[4];
%c0 = alloca i1
%c1 = alloca i1
%c2 = alloca i1
%c3 = alloca i1

%q01 = h q1* %q0 ;h bits[0];

;cx bits[0],bits[1];
%q10 = cx0 q1* %q01, i32 1
%q11 = cx1 q1* %q1, i32 1

;cx bits[1],bits[2];
%q20 = cx0 q1* %q11, i32 2
%q21 = cx1 q1* %q2, i32 2

;cx bits[2],bits[3];
%q30 = cx0 q1* %q21, i32 3
%q31 = cx1 q1* %q3, i32 3

%q02 = measure q1* %q01, i1* %c0 ;measure bits[0] -> c[0];
%q12 = measure q1* %q11, i1* %c1 ;measure bits[1] -> c[1];
%q22 = measure q1* %q21, i1* %c2 ;measure bits[2] -> c[2];
%q32 = measure q1* %q31, i1* %c3 ;measure bits[3] -> c[3];

ret i32 0
}