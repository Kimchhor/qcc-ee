; Test H gate instructions.
;
; RUN: llc < %s -march=qisa | FileCheck %s


define dso_local void @test_h_gate_1() #0 {
entry:
  ; CHECK: QMOV	QR0, 0x0
	; CHECK: QMOV	QR10, 0x1
	; CHECK: HADAMARD	QR0
	; CHECK: DEFORM	QR0
	; CHECK: FLIP_EXPAND	QR10, QR0
	; CHECK: FLIP_CONTRACT	QR10, QR0
	; CHECK: FLIP_SHIFT	QR10, QR0

  %q = alloca q1, align 4
  %0 = load q1, q1* %q, align 4
  %1 = h q1 %0
  store q1 %1, q1* %q, align 4
  ret void
}


define dso_local void @test_h_gate_2() #0 {
entry:
  ; CHECK: QMOV	QR0, 0x4
	; CHECK: QMOV	QR10, 0x5
	; CHECK: HADAMARD	QR0
	; CHECK: DEFORM	QR0
	; CHECK: FLIP_EXPAND	QR10, QR0
	; CHECK: FLIP_CONTRACT	QR10, QR0
	; CHECK: FLIP_SHIFT	QR10, QR0
	; CHECK: QMOV	QR0, 0x0
	; CHECK: QMOV	QR10, 0x1
	; CHECK: HADAMARD	QR0
	; CHECK: DEFORM	QR0
	; CHECK: FLIP_EXPAND	QR10, QR0
	; CHECK: FLIP_CONTRACT	QR10, QR0
	; CHECK: FLIP_SHIFT	QR10, QR0

  %q = alloca q1, align 4
  %q1 = alloca q1, align 4
  %0 = load q1, q1* %q, align 4
  %1 = h q1 %0
  store q1 %1, q1* %q, align 4
  %2 = load q1, q1* %q1, align 4
  %3 = h q1 %2
  store q1 %3, q1* %q1, align 4
  ret void
}