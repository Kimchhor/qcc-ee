; ModuleID = './01_single_gates.qcc'
source_filename = "./01_single_gates.qcc"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @kernel() #0 {
entry:
  %q0 = alloca q1, align 4
  %q1 = alloca q1, align 4
  %c = alloca i32, align 4
  %c1 = alloca i32, align 4
  %0 = load q1, q1* %q0, align 4
  %1 = x q1 %0
  store q1 %1, q1* %q0, align 4
  %2 = load q1, q1* %q0, align 4
  %3 = z q1 %2
  store q1 %3, q1* %q0, align 4
  %4 = load q1, q1* %q0, align 4
  %5 = h q1 %4
  store q1 %5, q1* %q0, align 4
  %6 = load q1, q1* %q1, align 4
  %7 = z q1 %6
  store q1 %7, q1* %q1, align 4
  %8 = load q1, q1* %q1, align 4
  %9 = h q1 %8
  store q1 %9, q1* %q1, align 4
  %10 = load q1, q1* %q1, align 4
  %11 = x q1 %10
  store q1 %11, q1* %q1, align 4
  %12 = load q1, q1* %q1, align 4
  %13 = s q1 %12
  store q1 %13, q1* %q1, align 4
  %14 = load q1, q1* %q0, align 4
  %15 = measure q1 %14
  store i32 %15, i32* %c, align 4
  %16 = load i32, i32* %c, align 4
  ret i32 %16
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 13.0.0 (https://github.com/QCL-PKNU/Quantum-Classical-Compiler.git 38b79329abafc99fa2517df7aaedb7ac612e29c1)"}
