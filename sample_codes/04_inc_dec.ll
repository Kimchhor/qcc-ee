; ModuleID = '/home/sengthai/Documents/Lab/QCC/backend/04_inc_dec.qcc'
source_filename = "/home/sengthai/Documents/Lab/QCC/backend/04_inc_dec.qcc"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @kernel() #0 {
entry:
  %q0 = alloca q1, align 1
  %result = alloca i32, align 4
  %0 = load q1, q1* %q0, align 1
  %1 = h q1 %0
  store q1 %1, q1* %q0, align 1
  %2 = load q1, q1* %q0, align 1
  %3 = measure q1 %2
  store i32 %3, i32* %result, align 4
  %4 = load i32, i32* %result, align 4
  %cmp = icmp slt i32 %4, 1
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %5 = load i32, i32* %result, align 4
  %inc = add nsw i32 %5, 1
  store i32 %inc, i32* %result, align 4
  br label %if.end

if.else:                                          ; preds = %entry
  %6 = load i32, i32* %result, align 4
  %dec = add nsw i32 %6, -1
  store i32 %dec, i32* %result, align 4
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %7 = load i32, i32* %result, align 4
  ret i32 %7
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 13.0.0 (git@bitbucket.org:youngsun/qcc-backend.git 21eb47a32363cd9e90c24cacfae98dbd3aa32750)"}
