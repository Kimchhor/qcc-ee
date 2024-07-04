; Test CNOT gate instructions.
;
; RUN: llc < %s -march=qisa | FileCheck %s


define dso_local void @test_cnot_1() #0 {
entry:
  ; CHECK: QMOV	QR0, 0x4
	; CHECK: QMOV	QR1, 0x0
	; CHECK: QMOV	QR2, 0x3
	; CHECK: INIT_Z	QR2
	; CHECK: MERGE_MXX	QR2, QR1
	; CHECK: SPLIT_MXX	QR2, QR1
	; CHECK: MERGE_MZZ	QR0, QR2
	; CHECK: SPLIT_MZZ	QR0, QR2
	; CHECK: CNOT_POST_MXX	QR0, QR1, QR2

  %q = alloca q1, align 4
  %q1 = alloca q1, align 4
  %0 = load q1, q1* %q, align 4
  %1 = load q1, q1* %q1, align 4
  %2 = ctrl q1 %0
  store q1 %2, q1* %q, align 4
  %3 = x q1 %1, %2
  store q1 %3, q1* %q1, align 4
  ret void
}


define dso_local void @test_cnot_2() #0 {
entry:
  ; CHECK: QMOV	QR0, 0x4
	; CHECK: X	QR0
	; CHECK: QMOV	QR1, 0x0
	; CHECK: QMOV	QR2, 0x3
	; CHECK: INIT_Z	QR2
	; CHECK: MERGE_MXX	QR2, QR1
	; CHECK: SPLIT_MXX	QR2, QR1
	; CHECK: MERGE_MZZ	QR0, QR2
	; CHECK: SPLIT_MZZ	QR0, QR2
	; CHECK: CNOT_POST_MXX	QR0, QR1, QR2

  %q = alloca q1, align 4
  %q1 = alloca q1, align 4
  %0 = load q1, q1* %q, align 4
  %1 = x q1 %0
  store q1 %1, q1* %q, align 4
  %2 = load q1, q1* %q, align 4
  %3 = load q1, q1* %q1, align 4
  %4 = ctrl q1 %2
  store q1 %4, q1* %q, align 4
  %5 = x q1 %3, %4
  store q1 %5, q1* %q1, align 4
  ret void
}


define dso_local void @test_cnot_3() #0 {
entry:
  ; CHECK: QMOV	QR0, 0x4
	; CHECK: Z	QR0
	; CHECK: QMOV	QR1, 0x0
	; CHECK: QMOV	QR2, 0x3
	; CHECK: INIT_Z	QR2
	; CHECK: MERGE_MXX	QR2, QR1
	; CHECK: SPLIT_MXX	QR2, QR1
	; CHECK: MERGE_MZZ	QR0, QR2
	; CHECK: SPLIT_MZZ	QR0, QR2
	; CHECK: CNOT_POST_MXX	QR0, QR1, QR2
	; CHECK: MEASURE_Z	QR1
	; CHECK: FMR	R0, QR1
	; CHECK: STD	0xc004, R0

  %q = alloca q1, align 4
  %q1 = alloca q1, align 4
  %c = alloca i32, align 4
  %c1 = alloca i32, align 4
  %0 = load q1, q1* %q, align 4
  %1 = z q1 %0
  store q1 %1, q1* %q, align 4
  %2 = load q1, q1* %q, align 4
  %3 = load q1, q1* %q1, align 4
  %4 = ctrl q1 %2
  store q1 %4, q1* %q, align 4
  %5 = x q1 %3, %4
  store q1 %5, q1* %q1, align 4
  %6 = load q1, q1* %q, align 4
  %7 = measure_x q1 %6
  store i32 %7, i32* %c, align 4
  %8 = load q1, q1* %q1, align 4
  %9 = measure q1 %8
  store i32 %9, i32* %c, align 4
  ret void
}