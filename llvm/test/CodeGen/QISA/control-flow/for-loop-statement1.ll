; Test For-Loop statement1 instruction.
;
; RUN: llc < %s -march=qisa | FileCheck %s


define dso_local void @test_for_loop_1() #0 {
entry:
  ; CHECK: MOV	R0, 0x0

  %q = alloca [3 x q1], align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  ; CHECK: MOV	R1, 0x3
	; CHECK: CMP R0, R1
	; CHECK: BRGE .BB0_4
	; CHECK: BRI .BB0_2

  %i.0 = phi i32 [ 0, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  ; CHECK: MOV	R1, 0xa000
	; CHECK: INC R1, R0
	; CHECK: INC R1, R0
	; CHECK: INC R1, R0
	; CHECK: INC R1, R0
	; CHECK: LLD	R2, R1
	; CHECK: QMOV	QR0, R2
	; CHECK: INIT_Z	QR0

  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 %idxprom
  %0 = load q1, q1* %arrayidx, align 4
  %idxprom1 = sext i32 %i.0 to i64
  %arrayidx2 = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 %idxprom1
  %1 = init_z q1 %0
  store q1 %1, q1* %arrayidx2, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  ; CHECK: INC R0, 0x1
	; CHECK: BRI .BB0_1

  %inc = add nsw i32 %i.0, 1
  br label %for.cond 

for.end:                                          ; preds = %for.cond
  ret void
}
