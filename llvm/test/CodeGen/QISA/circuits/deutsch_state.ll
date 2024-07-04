; Test Deutsch-State instructions.
;
; RUN: llc < %s -march=qisa | FileCheck %s


define dso_local void @kernel_deutsch() #0 {
entry:
  ; CHECK: QMOV	QR0, 0x0
	; CHECK: INIT_Z	QR0
	; CHECK: QMOV	QR1, 0x4
	; CHECK: INIT_Z	QR1
	; CHECK: X	QR1
	; CHECK: QMOV	QR10, 0x1
	; CHECK: HADAMARD	QR0
	; CHECK: DEFORM	QR0
	; CHECK: FLIP_EXPAND	QR10, QR0
	; CHECK: FLIP_CONTRACT	QR10, QR0
	; CHECK: FLIP_SHIFT	QR10, QR0
	; CHECK: QMOV	QR10, 0x5
	; CHECK: HADAMARD	QR1
	; CHECK: DEFORM	QR1
	; CHECK: FLIP_EXPAND	QR10, QR1
	; CHECK: FLIP_CONTRACT	QR10, QR1
	; CHECK: FLIP_SHIFT	QR10, QR1
	; CHECK: QMOV	QR2, 0x3
	; CHECK: INIT_Z	QR2
	; CHECK: MERGE_MXX	QR2, QR1
	; CHECK: SPLIT_MXX	QR2, QR1
	; CHECK: MERGE_MZZ	QR0, QR2
	; CHECK: SPLIT_MZZ	QR0, QR2
	; CHECK: CNOT_POST_MXX	QR0, QR1, QR2
	; CHECK: QMOV	QR10, 0x1
	; CHECK: HADAMARD	QR0
	; CHECK: DEFORM	QR0
	; CHECK: FLIP_EXPAND	QR10, QR0
	; CHECK: FLIP_CONTRACT	QR10, QR0
	; CHECK: FLIP_SHIFT	QR10, QR0
	; CHECK: MEASURE_Z	QR0
	; CHECK: FMR	R0, QR0
	; CHECK: STD	0xc000, R0
	; CHECK: MEASURE_Z	QR1
	; CHECK: FMR	R0, QR1
	; CHECK: STD	0xc004, R0

  %q = alloca [2 x q1], align 4
  %c = alloca [2 x i32], align 4
  %arrayidx = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %0 = load q1, q1* %arrayidx, align 4
  %arrayidx1 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %1 = init_z q1 %0
  store q1 %1, q1* %arrayidx1, align 4
  %arrayidx2 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %2 = load q1, q1* %arrayidx2, align 4
  %arrayidx3 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %3 = init_z q1 %2
  store q1 %3, q1* %arrayidx3, align 4
  %arrayidx4 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %4 = load q1, q1* %arrayidx4, align 4
  %arrayidx5 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %5 = x q1 %4
  store q1 %5, q1* %arrayidx5, align 4
  %arrayidx6 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %6 = load q1, q1* %arrayidx6, align 4
  %arrayidx7 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %7 = h q1 %6
  store q1 %7, q1* %arrayidx7, align 4
  %arrayidx8 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %8 = load q1, q1* %arrayidx8, align 4
  %arrayidx9 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %9 = h q1 %8
  store q1 %9, q1* %arrayidx9, align 4
  %arrayidx10 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %10 = load q1, q1* %arrayidx10, align 4
  %arrayidx11 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %11 = load q1, q1* %arrayidx11, align 4
  %arrayidx12 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %arrayidx13 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %12 = ctrl q1 %10
  store q1 %12, q1* %arrayidx12, align 4
  %13 = x q1 %11, %12
  store q1 %13, q1* %arrayidx13, align 4
  %arrayidx14 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %14 = load q1, q1* %arrayidx14, align 4
  %arrayidx15 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %15 = h q1 %14
  store q1 %15, q1* %arrayidx15, align 4
  %arrayidx16 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %16 = load q1, q1* %arrayidx16, align 4
  %17 = measure q1 %16
  %arrayidx17 = getelementptr inbounds [2 x i32], [2 x i32]* %c, i64 0, i64 0
  store i32 %17, i32* %arrayidx17, align 4
  %arrayidx18 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %18 = load q1, q1* %arrayidx18, align 4
  %19 = measure q1 %18
  %arrayidx19 = getelementptr inbounds [2 x i32], [2 x i32]* %c, i64 0, i64 1
  store i32 %19, i32* %arrayidx19, align 4
  ret void
}