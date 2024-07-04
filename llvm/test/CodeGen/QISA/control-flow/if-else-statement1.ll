; Test If-Else statement1 instruction.
;
; RUN: llc < %s -march=qisa | FileCheck %s


define dso_local void @test_if_else_1(i32 %basis) #0 {
entry:
  ; CHECK: MOV	R0, 0x0
	; CHECK: CMP R2, R0
	; CHECK: BRNE .BB0_2
	; CHECK: BRI .BB0_1

  %cmp = icmp eq i32 %basis, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  ; CHECK: X	QR0

  %0 = x q1 undef
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %q.0 = phi q1 [ %0, %if.then ], [ undef, %entry ]
  %1 = h q1 %q.0
  ret void
}
