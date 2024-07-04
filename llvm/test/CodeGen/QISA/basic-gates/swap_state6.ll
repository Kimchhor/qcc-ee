; Test Swap-State6 instructions.
;
; RUN: llc < %s -march=qisa | FileCheck %s


define dso_local void @kernel_swap_6() #0 {
entry:
  ; CHECK: QMOV	QR0, 0x0
	; CHECK: INIT_X	QR0
	; CHECK: QMOV	QR1, 0x4
	; CHECK: INIT_X	QR1
	; CHECK: Z	QR1
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
  %1 = init_x q1 %0
  store q1 %1, q1* %arrayidx1, align 4
  %arrayidx2 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %2 = load q1, q1* %arrayidx2, align 4
  %arrayidx3 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %3 = init_x q1 %2
  store q1 %3, q1* %arrayidx3, align 4
  %arrayidx4 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %4 = load q1, q1* %arrayidx4, align 4
  %arrayidx5 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %5 = z q1 %4
  store q1 %5, q1* %arrayidx5, align 4
  %arrayidx6 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %6 = load q1, q1* %arrayidx6, align 4
  %arrayidx7 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %7 = load q1, q1* %arrayidx7, align 4
  %arrayidx8 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %arrayidx9 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %8 = ctrl q1 %6
  store q1 %8, q1* %arrayidx8, align 4
  %9 = swap q1 %7, %8
  store q1 %9, q1* %arrayidx9, align 4
  %arrayidx10 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %10 = load q1, q1* %arrayidx10, align 4
  %11 = measure q1 %10
  %arrayidx11 = getelementptr inbounds [2 x i32], [2 x i32]* %c, i64 0, i64 0
  store i32 %11, i32* %arrayidx11, align 4
  %arrayidx12 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %12 = load q1, q1* %arrayidx12, align 4
  %13 = measure q1 %12
  %arrayidx13 = getelementptr inbounds [2 x i32], [2 x i32]* %c, i64 0, i64 1
  store i32 %13, i32* %arrayidx13, align 4
  ret void
}