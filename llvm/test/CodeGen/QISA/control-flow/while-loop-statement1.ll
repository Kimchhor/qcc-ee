; Test While-Loop statement1 instruction.
;
; RUN: llc < %s -march=qisa | FileCheck %s


define dso_local void @test_while_loop_1() #0 {
entry:
  ; CHECK: MOV	R0, 0x0

  %q = alloca [3 x q1], align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  ; CHECK: MOV	R1, 0x3
	; CHECK: CMP R0, R1
	; CHECK: BRGE .BB0_3
	; CHECK: BRI .BB0_2

  %counter.0 = phi i32 [ 0, %entry ], [ %inc, %while.body ]
  %cmp = icmp slt i32 %counter.0, 3
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  ; CHECK: MOV	R1, 0xa000
	; CHECK: INC R1, R0
	; CHECK: INC R1, R0
	; CHECK: INC R1, R0
	; CHECK: INC R1, R0
	; CHECK: LLD	R2, R1
	; CHECK: QMOV	QR0, R2
	; CHECK: INIT_Z	QR0
	; CHECK: INC R0, 0x1
	; CHECK: BRI .BB0_1

  %idxprom = sext i32 %counter.0 to i64
  %arrayidx = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 %idxprom
  %0 = load q1, q1* %arrayidx, align 4
  %idxprom1 = sext i32 %counter.0 to i64
  %arrayidx2 = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 %idxprom1
  %1 = init_z q1 %0
  store q1 %1, q1* %arrayidx2, align 4
  %inc = add nsw i32 %counter.0, 1
  br label %while.cond 

while.end:                                        ; preds = %while.cond
  ret void
}