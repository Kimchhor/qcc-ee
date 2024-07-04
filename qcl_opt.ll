; ModuleID = './qcl.ll'
source_filename = "./qcl.qcc"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @kernel() #0 {
entry:
  %0 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i32 0, i32 0
  %1 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i32 0, i32 1
  %q = alloca [2 x q1], align 4
  %2 = getelementptr inbounds [2 x i32], [2 x i32]* %c, i32 0, i32 0
  %3 = getelementptr inbounds [2 x i32], [2 x i32]* %c, i32 0, i32 1
  %c = alloca [2 x i32], align 4
  %arrayidx = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %4 = load q1, q1* %arrayidx, align 4
  %arrayidx1 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %5 = x q1 %4
  store q1 %5, q1* %arrayidx1, align 4
  %arrayidx2 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %6 = load q1, q1* %arrayidx2, align 4
  %arrayidx3 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %7 = h q1 %6
  store q1 %7, q1* %arrayidx3, align 4
  %arrayidx4 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %8 = load q1, q1* %arrayidx4, align 4
  %arrayidx5 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %9 = load q1, q1* %arrayidx5, align 4
  %arrayidx6 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %10 = ctrl q1 %8
  store q1 %10, q1* %arrayidx6, align 4
  %arrayidx7 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %11 = x q1 %10, %9
  store q1 %11, q1* %arrayidx7, align 4
  %arrayidx8 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %12 = load q1, q1* %arrayidx8, align 4
  %arrayidx9 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %13 = load q1, q1* %arrayidx9, align 4
  %arrayidx10 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %14 = ctrl q1 %12
  store q1 %14, q1* %arrayidx10, align 4
  %arrayidx11 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %15 = swap q1 %14, %13
  store q1 %15, q1* %arrayidx11, align 4
  %arrayidx12 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
  %16 = load q1, q1* %arrayidx12, align 4
  %17 = measure q1 %16
  %arrayidx13 = getelementptr inbounds [2 x i32], [2 x i32]* %c, i64 0, i64 0
  store i32 %17, i32* %arrayidx13, align 4
  %arrayidx14 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
  %18 = load q1, q1* %arrayidx14, align 4
  %19 = measure q1 %18
  %arrayidx15 = getelementptr inbounds [2 x i32], [2 x i32]* %c, i64 0, i64 1
  store i32 %19, i32* %arrayidx15, align 4
  ret void
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 13.0.0 (https://github.com/QCL-PKNU/Quantum-Classical-Compiler.git 2ab7ae5211319148931c8c74def7c426c9e7a6d8)"}
