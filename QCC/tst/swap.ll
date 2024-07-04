; ModuleID = 'swap.cpp'

@last_cnot_id = global i32 2

define dso_local i32 @main() {
  %1 = alloca q1
  %2 = alloca q1
  %3 = swap q1* %1, i32 1
  %4 = swap q1* %2, i32 1
  %5 = h q1* %4
  %6 = swap q1* %3, i32 2
  %7 = swap q1* %5, i32 2
  %8 = h q1* %7
  ret i32 0
}




