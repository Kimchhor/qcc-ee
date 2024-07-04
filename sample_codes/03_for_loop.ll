; ModuleID = '/home/sengthai/Documents/Lab/QCC/backend/03_for_loop.qcc'
source_filename = "/home/sengthai/Documents/Lab/QCC/backend/03_for_loop.qcc"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @kernel(i32 %size) #0 {
entry:
  %size.addr = alloca i32, align 4
  %q0 = alloca q1, align 1
  %s = alloca i32, align 4
  store i32 %size, i32* %size.addr, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, i32* %s, align 4
  %1 = load i32, i32* %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load q1, q1* %q0, align 1
  %3 = h q1 %2
  store q1 %3, q1* %q0, align 1
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %4 = load i32, i32* %s, align 4
  %inc = add nsw i32 %4, 1
  store i32 %inc, i32* %s, align 4
  br label %for.cond, !llvm.loop !4

for.end:                                          ; preds = %for.cond
  %5 = load q1, q1* %q0, align 1
  %6 = measure q1 %5
  ret i32 %6
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 13.0.0 (git@bitbucket.org:youngsun/qcc-backend.git 21eb47a32363cd9e90c24cacfae98dbd3aa32750)"}
!4 = distinct !{!4, !5}
!5 = !{!"llvm.loop.mustprogress"}
