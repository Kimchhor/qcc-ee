; Test For-Loop statement3 instruction.
;
; RUN: llc < %s -march=qisa | FileCheck %s


define dso_local void @test_for_loop_3() #0 {
entry:
  ; CHECK: MOV	R0, 0x0
	; CHECK: STD	0xc004, R0

  %q = alloca [3 x q1], align 4
  %c = alloca [3 x i32], align 4
  %i = alloca i32, align 4
  %i3 = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  ; CHECK: LDD	R0, 0xc004
	; CHECK: MOV	R1, 0x3
	; CHECK: CMP R0, R1
	; CHECK: BRGE .BB0_4
	; CHECK: BRI .BB0_2

  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  ; CHECK: LDD	R0, 0xc004
	; CHECK: MOV	R1, 0xa000
	; CHECK: INC R1, R0
	; CHECK: INC R1, R0
	; CHECK: INC R1, R0
	; CHECK: INC R1, R0
	; CHECK: LLD	R0, R1
	; CHECK: QMOV	QR0, R0
	; CHECK: QMOV	QR10, 0x3
	; CHECK: HADAMARD	QR0
	; CHECK: DEFORM	QR0
	; CHECK: FLIP_EXPAND	QR10, QR0
	; CHECK: FLIP_CONTRACT	QR10, QR0
	; CHECK: FLIP_SHIFT	QR10, QR0

  %1 = load i32, i32* %i, align 4
  %idxprom = sext i32 %1 to i64
  %arrayidx = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 %idxprom
  %2 = load q1, q1* %arrayidx, align 4
  %3 = load i32, i32* %i, align 4
  %idxprom1 = sext i32 %3 to i64
  %arrayidx2 = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 %idxprom1
  %4 = h q1 %2
  store q1 %4, q1* %arrayidx2, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  ; CHECK: LDD	R0, 0xc004
	; CHECK: INC R0, 0x1
	; CHECK: STD	0xc004, R0
	; CHECK: BRI .BB0_1

  %5 = load i32, i32* %i, align 4
  %inc = add nsw i32 %5, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond 

for.end:                                          ; preds = %for.cond
  ; CHECK: MOV	R0, 0x0
	; CHECK: STD	0xc000, R0

  store i32 0, i32* %i3, align 4
  br label %for.cond4

for.cond4:                                        ; preds = %for.inc11, %for.end
  ; CHECK: LDD	R0, 0xc000
	; CHECK: MOV	R1, 0x3
	; CHECK: CMP R0, R1
	; CHECK: BRGE .BB0_8
	; CHECK: BRI .BB0_6

  %6 = load i32, i32* %i3, align 4
  %cmp5 = icmp slt i32 %6, 3
  br i1 %cmp5, label %for.body6, label %for.end13

for.body6:                                        ; preds = %for.cond4
  ; CHECK: LDD	R0, 0xc000
	; CHECK: MOV	R1, 0xa000
	; CHECK: MOV	R2, R1
	; CHECK: INC R2, R0
	; CHECK: INC R2, R0
	; CHECK: INC R2, R0
	; CHECK: INC R2, R0
	; CHECK: LLD	R0, R2
	; CHECK: QMOV	QR0, R0
	; CHECK: MEASURE_Z	QR0
	; CHECK: FMR	R0, QR0
	; CHECK: MOV	R2, 0x8
	; CHECK: INC R2, R1
	; CHECK: STI	R2, 0x0, R0

  %7 = load i32, i32* %i3, align 4
  %idxprom7 = sext i32 %7 to i64
  %arrayidx8 = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 %idxprom7
  %8 = load q1, q1* %arrayidx8, align 4
  %9 = measure q1 %8
  %10 = load i32, i32* %i3, align 4
  %idxprom9 = sext i32 %10 to i64
  %arrayidx10 = getelementptr inbounds [3 x i32], [3 x i32]* %c, i64 0, i64 %idxprom9
  store i32 %9, i32* %arrayidx10, align 4
  br label %for.inc11

for.inc11:                                        ; preds = %for.body6
  ; CHECK: LDD	R0, 0xc000
	; CHECK: INC R0, 0x1
	; CHECK: STD	0xc000, R0
	; CHECK: BRI .BB0_5

  %11 = load i32, i32* %i3, align 4
  %inc12 = add nsw i32 %11, 1
  store i32 %inc12, i32* %i3, align 4
  br label %for.cond4 

for.end13:                                        ; preds = %for.cond4
  ret void
}