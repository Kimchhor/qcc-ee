; Test MeasureZ instructions.
;
; RUN: llc < %s -march=qisa | FileCheck %s


define dso_local void @test_measureZ_1() #0 {
entry:
  ; CHECK: QMOV	QR0, 0x0
	; CHECK: MEASURE_Z	QR0
	; CHECK: FMR	R0, QR0
	; CHECK: STD	0xc000, R0

  %q = alloca q1, align 4
  %c = alloca i32, align 4
  %0 = load q1, q1* %q, align 4
  %1 = measure q1 %0
  store i32 %1, i32* %c, align 4
  ret void
}


define dso_local void @test_measureZ_2() #0 {
entry:
  ; CHECK: QMOV	QR0, 0x4
	; CHECK: MEASURE_Z	QR0
	; CHECK: FMR	R0, QR0
	; CHECK: STD	0xc004, R0
	; CHECK: QMOV	QR0, 0x0
	; CHECK: MEASURE_Z	QR0
	; CHECK: FMR	R0, QR0
	; CHECK: STD	0xc000, R0

  %q = alloca q1, align 4
  %q1 = alloca q1, align 4
  %c = alloca i32, align 4
  %c1 = alloca i32, align 4
  %0 = load q1, q1* %q, align 4
  %1 = measure q1 %0
  store i32 %1, i32* %c, align 4
  %2 = load q1, q1* %q1, align 4
  %3 = measure q1 %2
  store i32 %3, i32* %c1, align 4
  ret void
}