; Test Grover-State instructions.
;
; RUN: llc < %s -march=qisa | FileCheck %s


define dso_local void @kernel_grover() #0 {
entry:
  ; CHECK: QMOV	QR0, 0x0
	; CHECK: INIT_Z	QR0
	; CHECK: QMOV	QR1, 0x4
	; CHECK: INIT_Z	QR1
	; CHECK: QMOV	QR10, 0x1
	; CHECK: HADAMARD	QR0
	; CHECK: DEFORM	QR0
	; CHECK: FLIP_EXPAND	QR10, QR0
	; CHECK: FLIP_CONTRACT	QR10, QR0
	; CHECK: FLIP_SHIFT	QR10, QR0
	; CHECK: QMOV	QR2, 0x3
	; CHECK: QMOV	QR3, QR2
	; CHECK: INIT_Z	QR3
	; CHECK: MERGE_MXX	QR3, QR1
	; CHECK: SPLIT_MXX	QR3, QR1
	; CHECK: MERGE_MZZ	QR0, QR3
	; CHECK: SPLIT_MZZ	QR0, QR3
	; CHECK: CNOT_POST_MXX	QR0, QR1, QR3
	; CHECK: QMOV	QR10, 0x1
	; CHECK: HADAMARD	QR0
	; CHECK: DEFORM	QR0
	; CHECK: FLIP_EXPAND	QR10, QR0
	; CHECK: FLIP_CONTRACT	QR10, QR0
	; CHECK: FLIP_SHIFT	QR10, QR0
	; CHECK: X	QR0
	; CHECK: X	QR1
	; CHECK: QMOV	QR10, 0x5
	; CHECK: HADAMARD	QR1
	; CHECK: DEFORM	QR1
	; CHECK: FLIP_EXPAND	QR10, QR1
	; CHECK: FLIP_CONTRACT	QR10, QR1
	; CHECK: FLIP_SHIFT	QR10, QR1
	; CHECK: INIT_Z	QR2
	; CHECK: MERGE_MXX	QR2, QR1
	; CHECK: SPLIT_MXX	QR2, QR1
	; CHECK: MERGE_MZZ	QR0, QR2
	; CHECK: SPLIT_MZZ	QR0, QR2
	; CHECK: CNOT_POST_MXX	QR0, QR1, QR2
	; CHECK: QMOV	QR10, 0x5
	; CHECK: HADAMARD	QR1
	; CHECK: DEFORM	QR1
	; CHECK: FLIP_EXPAND	QR10, QR1
	; CHECK: FLIP_CONTRACT	QR10, QR1
	; CHECK: FLIP_SHIFT	QR10, QR1
	; CHECK: X	QR0
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
  %arrayidx4 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %4 = load q1, q1* %arrayidx4, align 4
  %arrayidx5 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %5 = h q1 %4
  store q1 %5, q1* %arrayidx5, align 4
  %arrayidx6 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %6 = load q1, q1* %arrayidx6, align 4
  %arrayidx7 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
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
  %arrayidx14 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %14 = load q1, q1* %arrayidx14, align 4
  %arrayidx15 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %15 = h q1 %14
  store q1 %15, q1* %arrayidx15, align 4
  %arrayidx16 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %16 = load q1, q1* %arrayidx16, align 4
  %arrayidx17 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %17 = h q1 %16
  store q1 %17, q1* %arrayidx17, align 4
  %arrayidx18 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %18 = load q1, q1* %arrayidx18, align 4
  %arrayidx19 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %19 = h q1 %18
  store q1 %19, q1* %arrayidx19, align 4
  %arrayidx20 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %20 = load q1, q1* %arrayidx20, align 4
  %arrayidx21 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %21 = x q1 %20
  store q1 %21, q1* %arrayidx21, align 4
  %arrayidx22 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %22 = load q1, q1* %arrayidx22, align 4
  %arrayidx23 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %23 = x q1 %22
  store q1 %23, q1* %arrayidx23, align 4
  %arrayidx24 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %24 = load q1, q1* %arrayidx24, align 4
  %arrayidx25 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %25 = h q1 %24
  store q1 %25, q1* %arrayidx25, align 4
  %arrayidx26 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %26 = load q1, q1* %arrayidx26, align 4
  %arrayidx27 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %27 = load q1, q1* %arrayidx27, align 4
  %arrayidx28 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %arrayidx29 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %28 = ctrl q1 %26
  store q1 %28, q1* %arrayidx28, align 4
  %29 = x q1 %27, %28
  store q1 %29, q1* %arrayidx29, align 4
  %arrayidx30 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %30 = load q1, q1* %arrayidx30, align 4
  %arrayidx31 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %31 = h q1 %30
  store q1 %31, q1* %arrayidx31, align 4
  %arrayidx32 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %32 = load q1, q1* %arrayidx32, align 4
  %arrayidx33 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %33 = x q1 %32
  store q1 %33, q1* %arrayidx33, align 4
  %arrayidx34 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %34 = load q1, q1* %arrayidx34, align 4
  %arrayidx35 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %35 = x q1 %34
  store q1 %35, q1* %arrayidx35, align 4
  %arrayidx36 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %36 = load q1, q1* %arrayidx36, align 4
  %arrayidx37 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %37 = h q1 %36
  store q1 %37, q1* %arrayidx37, align 4
  %arrayidx38 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %38 = load q1, q1* %arrayidx38, align 4
  %arrayidx39 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %39 = h q1 %38
  store q1 %39, q1* %arrayidx39, align 4
  %arrayidx40 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %40 = load q1, q1* %arrayidx40, align 4
  %41 = measure q1 %40
  %arrayidx41 = getelementptr inbounds [2 x i32], [2 x i32]* %c, i64 0, i64 0
  store i32 %41, i32* %arrayidx41, align 4
  %arrayidx42 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %42 = load q1, q1* %arrayidx42, align 4
  %43 = measure q1 %42
  %arrayidx43 = getelementptr inbounds [2 x i32], [2 x i32]* %c, i64 0, i64 1
  store i32 %43, i32* %arrayidx43, align 4
  ret void
}