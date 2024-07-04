; Test For-Loop statement2 instruction.
;
; RUN: llc < %s -march=qisa | FileCheck %s


define dso_local void @test_for_loop_2() #0 {
entry:
  ; CHECK: MOV	R0, 0x1
	; CHECK: STD	0xc000, R0

  %q = alloca [3 x q1], align 4
  %i = alloca i32, align 4
  store i32 1, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  ; CHECK: LDD	R0, 0xc000
	; CHECK: MOV	R1, 0x3
	; CHECK: CMP R0, R1
	; CHECK: BRGE .BB0_3
	; CHECK: BRI .BB0_2

  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  ; CHECK: LDD	R0, 0xc000
	; CHECK: LLD	R1, R0
	; CHECK: QMOV	QR0, R1
	; CHECK: MOV	R1, 0xa000
	; CHECK: INC R1, R0
	; CHECK: INC R1, R0
	; CHECK: INC R1, R0
	; CHECK: INC R1, R0
	; CHECK: LLD	R2, R1
	; CHECK: QMOV	QR1, R2
	; CHECK: QMOV	QR2, 0x3
	; CHECK: INIT_Z	QR2
	; CHECK: MERGE_MXX	QR2, QR1
	; CHECK: SPLIT_MXX	QR2, QR1
	; CHECK: MERGE_MZZ	QR0, QR2
	; CHECK: SPLIT_MZZ	QR0, QR2
	; CHECK: CNOT_POST_MXX	QR0, QR1, QR2
	; CHECK: INC R0, 0x1
	; CHECK: STD	0xc000, R0
	; CHECK: BRI .BB0_1

  %arrayidx = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 0
  %1 = load q1, q1* %arrayidx, align 4
  %2 = load i32, i32* %i, align 4
  %idxprom = sext i32 %2 to i64
  %arrayidx1 = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 %idxprom
  %3 = load q1, q1* %arrayidx1, align 4
  %arrayidx2 = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 0
  %4 = load i32, i32* %i, align 4
  %idxprom3 = sext i32 %4 to i64
  %arrayidx4 = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 %idxprom3
  %5 = ctrl q1 %1
  store q1 %5, q1* %arrayidx2, align 4
  %6 = x q1 %3, %5
  store q1 %6, q1* %arrayidx4, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %7 = load i32, i32* %i, align 4
  %inc = add nsw i32 %7, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond 

for.end:                                          ; preds = %for.cond
  ret void
}