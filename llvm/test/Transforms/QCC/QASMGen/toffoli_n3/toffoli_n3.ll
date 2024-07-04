; RUN: opt -passes=qasm-gen %s --disable-output &> %t
; RUN: diff --strip-trailing-cr %t %S/toffoli_n3.qasm

define dso_local i32 @main() {
    
;qreg a[3];
%q0 = alloca q1
%q1 = alloca q1
%q2 = alloca q1

;creg c[3];
%c0 = alloca i1
%c1 = alloca i1
%c2 = alloca i1

%q00 = x q1* %q0 ;x a[0];
%q10 = x q1* %q1 ;x a[1];
%q20 = h q1* %q2 ;h a[2];

;cx a[1],a[2];
%q21 = cx0 q1* %q10, i32 1
%q22 = cx1 q1* %q20, i32 1

%q23 = tdg q1* %q22  ;tdg a[2];

;cx a[0],a[2];
%q24 = cx0 q1* %q00, i32 2
%q25 = cx1 q1* %q23, i32 2

%q26 = t q1* %q25  ;t a[2];

;cx a[1],a[2];
%q27 = cx0 q1* %q10, i32 3
%q28 = cx1 q1* %q26, i32 3

%q29 = tdg q1* %q28 ;tdg a[2];

;cx a[0],a[2];
%q2A = cx0 q1* %q00, i32 4
%q2B = cx1 q1* %q29, i32 4

%q11 = tdg q1* %q10 ;tdg a[1];
%q2C = t q1* %q2B ;t a[2];

;cx a[0],a[1];
%q12 = cx0 q1* %q00, i32 5
%q13 = cx1 q1* %q11, i32 5

%q2D = h q1* %q2C ;h a[2];
%q14 = tdg q1* %q13 ;tdg a[1];

;cx a[0],a[1];
%q15 = cx0 q1* %q00, i32 6
%q16 = cx1 q1* %q14, i32 6

%q01 = t q1* %q00 ;t a[0];
%q17 = s q1* %q16 ;s a[1];

%q02 = measure q1* %q01, i1* %c0 ;measure a[0] -> c[0];
%q18 = measure q1* %q17, i1* %c1 ;measure a[1] -> c[1];
%q2E = measure q1* %q2D, i1* %c2 ;measure a[2] -> c[2];
ret i32 0
}