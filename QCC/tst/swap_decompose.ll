; ModuleID = 'cnot.ll'
source_filename = "cnot.ll"

define dso_local i32 @main() {
  %1 = alloca q1, align 1
  %2 = alloca q1, align 1
  %3 = cx0 q1* %1, i32 3
  %4 = cx1 q1* %2, i32 3
  %5 = h q1* %4
  %6 = cx1 q1* %3, i32 2
  %7 = cx0 q1* %5, i32 2
  %8 = h q1* %7
  %9 = cx0 q1* %8, i32 4
  %10 = cx1 q1* %7, i32 4
  %11 = cx1 q1* %9, i32 4
  %12 = cx0 q1* %9, i32 6
  %13 = cx1 q1* %10, i32 6
  %14 = cx1 q1* %12, i32 7
  %15 = cx0 q1* %13, i32 7
  %16 = cx0 q1* %14, i32 8
  %17 = cx1 q1* %15, i32 8
  %18 = cx1 q1* %11, i32 5
  %19 = cx0 q1* %16, i32 5
  %20 = cx1 q1* %9, i32 5
  ret i32 0
}
