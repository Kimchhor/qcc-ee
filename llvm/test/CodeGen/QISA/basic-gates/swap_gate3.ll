; Test SWAP gate3 instructions.
;
; RUN: llc < %s -march=qisa | FileCheck %s


define dso_local void @test_swap_3() #0 {
entry:
  ; CHECK; QMOV	QR0, 0x4
	; CHECK: Z	QR0
	; CHECK: QMOV	QR1, 0x0
	; CHECK: QMOV	QR11, 0x1
	; CHECK: QMOV	QR12, 0x3
	; CHECK: INIT_Z	QR11
	; CHECK: MERGE_MXX	QR11, QR0
	; CHECK: SPLIT_MXX	QR11, QR0
	; CHECK: MOVE_POST_MXX	QR11, QR0
	; CHECK: INIT_Z	QR12
	; CHECK: MERGE_MXX	QR12, QR1
	; CHECK: SPLIT_MXX	QR12, QR1
	; CHECK: MOVE_POST_MXX	QR12, QR1
	; CHECK: INIT_X	QR0
	; CHECK: MERGE_MZZ	QR0, QR12
	; CHECK: SPLIT_MZZ	QR0, QR12
	; CHECK: MOVE_POST_MZZ	QR0, QR12
	; CHECK: INIT_X	QR1
	; CHECK: MERGE_MZZ	QR1, QR11
	; CHECK: SPLIT_MZZ	QR1, QR11
	; CHECK: MOVE_POST_MZZ	QR1, QR11
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
  %5 = swap q1 %3, %4
  store q1 %5, q1* %q1, align 4
  %6 = load q1, q1* %q, align 4
  %7 = measure_x q1 %6
  store i32 %7, i32* %c, align 4
  %8 = load q1, q1* %q1, align 4
  %9 = measure q1 %8
  store i32 %9, i32* %c, align 4
  ret void
}