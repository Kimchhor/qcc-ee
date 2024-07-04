; Test While-Loop statement3 instruction.
;
; RUN: llc < %s -march=qisa | FileCheck %s


define dso_local void @test_while_loop_3() #0 {
entry:
  ; CHECK: QMOV	QR10, 0x1
	; CHECK: HADAMARD	QR0
	; CHECK: DEFORM	QR0
	; CHECK: FLIP_EXPAND	QR10, QR0
	; CHECK: FLIP_CONTRACT	QR10, QR0
	; CHECK: FLIP_SHIFT	QR10, QR0
	; CHECK: MOV	R0, 0x5
	; CHECK: MOV	R1, 0x0
	; CHECK: MOV	R2, R1

  %0 = h q1 undef
  br label %while.cond

while.cond:                                       ; preds = %if.end, %entry
  ; CHECK: MOV	R4, 0x2
	; CHECK: CMP R0, R4
	; CHECK: BRLT .BB0_6
	; CHECK: BRI .BB0_2

  %one_counter.0 = phi i32 [ 0, %entry ], [ %one_counter.1, %if.end ]
  %zero_counter.0 = phi i32 [ 0, %entry ], [ %zero_counter.1, %if.end ]
  %counter.0 = phi i32 [ 5, %entry ], [ %dec, %if.end ]
  %cmp = icmp sgt i32 %counter.0, 1
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  ; CHECK: QMOV	QR1, QR0
	; CHECK: MEASURE_Z	QR1
	; CHECK: FMR	R4, QR1
	; CHECK: MOV	R5, 0x0
	; CHECK: CMP R4, R5
	; CHECK: BRNE .BB0_4
	; CHECK: BRI .BB0_3

  %1 = measure q1 %0
  %cmp1 = icmp eq i32 %1, 0
  br i1 %cmp1, label %if.then, label %if.else

if.then:                                          ; preds = %while.body
  ; CHECK: INC R1, 0x1

  %inc = add nsw i32 %zero_counter.0, 1
  br label %if.end

if.else:                                          ; preds = %while.body
  ; CHECK: DEC R0, 0x1
	; CHECK: BRI .BB0_1

  %inc2 = add nsw i32 %one_counter.0, 1
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  ; CHECK: INC R2, 0x1
	; CHECK: BRI .BB0_5

  %one_counter.1 = phi i32 [ %one_counter.0, %if.then ], [ %inc2, %if.else ]
  %zero_counter.1 = phi i32 [ %inc, %if.then ], [ %zero_counter.0, %if.else ]
  %dec = add nsw i32 %counter.0, -1
  br label %while.cond 

while.end:                                        ; preds = %while.cond
  ret void
}