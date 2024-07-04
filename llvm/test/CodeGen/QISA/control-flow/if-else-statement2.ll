; Test If-Else statement2 instruction.
;
; RUN: llc < %s -march=qisa | FileCheck %s


define dso_local void @test_if_else_2(i32 %basis) #0 {
entry:
  ; CHECK: MOV	R0, 0x0
	; CHECK: CMP R2, R0
	; CHECK: BREQ .BB0_2
	; CHECK: BRI .BB0_1

  %cmp = icmp ne i32 %basis, 0
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  ; CHECK: X	QR0
	; CHECK: BRI .BB0_3

  %0 = x q1 undef
  br label %if.end

if.else:                                          ; preds = %entry
  ; CHECK: Z	QR0

  %1 = z q1 undef
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %q.0 = phi q1 [ %0, %if.then ], [ %1, %if.else ]
  %2 = h q1 %q.0
  ret void
}