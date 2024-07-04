; Test While-Loop statement2 instruction.
;
; RUN: llc < %s -march=qisa | FileCheck %s


define dso_local void @test_while_loop_2() #0 {
entry:
  ; CHECK: X	QR0

  %0 = x q1 undef
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  ; CHECK: QMOV	QR1, QR0
	; CHECK: MEASURE_Z	QR1
	; CHECK: FMR	R0, QR1
	; CHECK: MOV	R1, 0x0
	; CHECK: CMP R0, R1
	; CHECK: BREQ .BB0_1
	; CHECK: BRI .BB0_2

  %time.0 = phi i32 [ 0, %entry ], [ %inc, %while.body ]
  %1 = measure q1 %0
  %cmp = icmp eq i32 %1, 0
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %inc = add nsw i32 %time.0, 1
  br label %while.cond 

while.end:                                        ; preds = %while.cond
  ret void
}