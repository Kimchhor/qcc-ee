; RUN: opt -passes=qasm-gen %s --disable-output &> %t
; RUN: diff --strip-trailing-cr %t %S/qaoa_n3.qasm

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
%q10 = h q1* %q1 ;h q[1];
%q20 = h q1* %q2 ;h q[2];

;cx q[0],q[2];
%q21 = cx0 q1* %q00, i32 1
%q22 = cx1 q1* %q20, i32 1

%q23 = rz q1* %q22, double 5.654426 ;rz(5.654426) q[2];

;cx q[0],q[2];
%q24 = cx0 q1* %q00, i32 2
%q25 = cx1 q1* %q23, i32 2

;cx q[0],q[1];
%q11 = cx0 q1* %q00, i32 3
%q12 = cx1 q1* %q10, i32 3

;cx q[1],q[2];
%q26 = cx0 q1* %q12, i32 4
%q27 = cx1 q1* %q25, i32 4

%q28 = rz q1* %q27, double -11.308885 ;rz(-11.308885) q[2];

;cx q[1],q[2];
%q29 = cx0 q1* %q12, i32 5
%q2A = cx1 q1* %q28, i32 5

;cx q[0],q[1];
%q13 = cx0 q1* %q00, i32 6
%q14 = cx1 q1* %q12, i32 6

%q2B = rx q1* %q2A, double 1.713248 ;rx(1.713248) q[2];
%q15 = rz q1* %q14, double -16.963310 ;rz(-16.96331) q[1];
%q01 = rx q1* %q00, double 1.713248 ;rx(1.713248) q[0];
%q16 = rx q1* %q15, double 1.713248 ;rx(1.713248) q[1];

%q2C = measure q1* %q2B, i1* %c2 ;measure q[2] -> c[2];
%q02 = measure q1* %q01, i1* %c0 ;measure q[0] -> c[0];
%q17 = measure q1* %q16, i1* %c1 ;measure q[1] -> c[1];
ret i32 0
}