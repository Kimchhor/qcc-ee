; ModuleID = 'main.cpp'
source_filename = "main.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define dso_local i32 @main() {
  %1 = alloca q1
  %2 = alloca q1
  %a = alloca q1 
  %c1 = alloca i1
  %c2 = alloca i1
  %3  = x q1* %1
  %4  = y q1* %3
  %5  = z q1* %2
  %6  = h q1* %5
  %7  = i q1* %6
  %8  = s q1* %7
  %9  = t q1* %8
  %10 = sdg q1* %9
  %11 = tdg q1* %10
  %12 = rx q1* %11, double 3.140000
  %13 = ry q1* %12, double 3.140000
  %14 = rz q1* %13, double 3.140000
  %15 = cx0 q1* %14,  i32 1
  %16 = cx1 q1* %4,   i32 1
  %17 = swap q1* %15, i32 2
  %18 = swap q1* %16, i32 2
  %19 = cx0 q1* %17,  i32 3
  %20 = cx1 q1* %18,  i32 3
  %21 = cx0 q1* %a,   i32 3
  %22 = measure q1* %21, i1* %c2
  ret i32 0
}
