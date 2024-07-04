; Test If-Else statement3 instruction.
;
; RUN: llc < %s -march=qisa | FileCheck %s


define dso_local void @test_if_else_3(i32 %basis) #0 {
entry:
  ; CHECK: MOV	R0, 0x1
	; CHECK: CMP R2, R0
	; CHECK: BRGE .BB0_2
	; CHECK: BRI .BB0_1

  %cmp = icmp sle i32 %basis, 0
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  ; CHECK: X	QR0
	; CHECK: BRI .BB0_5

  %0 = x q1 undef
  br label %if.end4

if.else:                                          ; preds = %entry
  ; CHECK: MOV	R0, 0x1
	; CHECK: CMP R2, R0
	; CHECK: BRLT .BB0_4
	; CHECK: BRI .BB0_3

  %cmp1 = icmp sge i32 %basis, 1
  br i1 %cmp1, label %if.then2, label %if.else3

if.then2:                                         ; preds = %if.else
  ; CHECK: Z	QR0
	; CHECK: BRI .BB0_5

  %1 = z q1 undef
  br label %if.end

if.else3:                                         ; preds = %if.else
  ; CHECK: Z	QR0

  %2 = z q1 undef
  br label %if.end

if.end:                                           ; preds = %if.else3, %if.then2
  %q.0 = phi q1 [ %1, %if.then2 ], [ %2, %if.else3 ]
  br label %if.end4

if.end4:                                          ; preds = %if.end, %if.then
  %q.1 = phi q1 [ %0, %if.then ], [ %q.0, %if.end ]
  %3 = h q1 %q.1
  ret void
}