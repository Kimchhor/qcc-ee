; ModuleID = '/home/sengthai/Documents/Lab/QCC/backend/02_if_else.qcc'
source_filename = "/home/sengthai/Documents/Lab/QCC/backend/02_if_else.qcc"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @kernel(i32 %basis) #0 {
entry:
  %basis.addr = alloca i32, align 4
  %q0 = alloca q1, align 1
  %c = alloca i32, align 4
  store i32 %basis, i32* %basis.addr, align 4
  %0 = load q1, q1* %q0, align 1
  %1 = h q1 %0
  store q1 %1, q1* %q0, align 1
  %2 = load i32, i32* %basis.addr, align 4
  %cmp = icmp eq i32 %2, 0
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %3 = load q1, q1* %q0, align 1
  %4 = x q1 %3
  store q1 %4, q1* %q0, align 1
  br label %if.end4

if.else:                                          ; preds = %entry
  %5 = load i32, i32* %basis.addr, align 4
  %cmp1 = icmp eq i32 %5, 1
  br i1 %cmp1, label %if.then2, label %if.else3

if.then2:                                         ; preds = %if.else
  %6 = load q1, q1* %q0, align 1
  %7 = z q1 %6
  store q1 %7, q1* %q0, align 1
  br label %if.end

if.else3:                                         ; preds = %if.else
  %8 = load q1, q1* %q0, align 1
  %9 = h q1 %8
  store q1 %9, q1* %q0, align 1
  br label %if.end

if.end:                                           ; preds = %if.else3, %if.then2
  br label %if.end4

if.end4:                                          ; preds = %if.end, %if.then
  %10 = load q1, q1* %q0, align 1
  %11 = measure q1 %10
  store i32 %11, i32* %c, align 4
  %12 = load i32, i32* %c, align 4
  ret i32 %12
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 13.0.0 (git@bitbucket.org:youngsun/qcc-backend.git 21eb47a32363cd9e90c24cacfae98dbd3aa32750)"}
