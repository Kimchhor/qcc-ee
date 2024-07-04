; Test Do-While statement2 instruction.
;
; RUN: llc < %s -march=qisa | FileCheck %s


define dso_local void @test_do_while_2() #0 {
entry:
  ; CHECK: X	QR0
	; CHECK: MEASURE_Z	QR0
	; CHECK: FMR	R0, QR0

  %0 = x q1 undef
  br label %do.body

do.body:                                          ; preds = %do.cond, %entry
  ; CHECK: MOV	R1, 0x0
	; CHECK: CMP R0, R1
	; CHECK: BREQ .BB0_1
	; CHECK: BRI .BB0_2

  %time.0 = phi i32 [ 0, %entry ], [ %inc, %do.cond ]
  %inc = add nsw i32 %time.0, 1
  br label %do.cond

do.cond:                                          ; preds = %do.body
  %1 = measure q1 %0
  %cmp = icmp eq i32 %1, 0
  br i1 %cmp, label %do.body, label %do.end 

do.end:                                           ; preds = %do.cond
  ret void
}