; ModuleID = "file_circuit-160"
target triple = "unknown-unknown-unknown"
target datalayout = ""

define void @"circuit-160"()
{
entry:
  %"qbits" = alloca [2 x q1]
  %"cbits" = alloca [2 x i32]
  %".2" = getelementptr [2 x q1], [2 x q1]* %"qbits", i32 0, i32 0
  %".3" = load volatile q1, q1* %".2"
  %".4" = h q1 %".3"
  store volatile q1 %".4", q1* %".2"
  %".6" = getelementptr [2 x q1], [2 x q1]* %"qbits", i32 0, i32 0
  %".7" = load volatile q1, q1* %".6"
  %".8" = getelementptr [2 x q1], [2 x q1]* %"qbits", i32 0, i32 1
  %".9" = load volatile q1, q1* %".8"
  %".10" = ctrl q1 %".7"
  %".11" = x q1 %".10", q1 %".9"
  store volatile q1 %".10", q1* %".6"
  store volatile q1 %".11", q1* %".8"
  %".14" = getelementptr [2 x q1], [2 x q1]* %"qbits", i32 0, i32 0
  %".15" = load volatile q1, q1* %".14"
  %".16" = measure q1 %".15"
  %".17" = getelementptr [2 x i32], [2 x i32]* %"cbits", i32 0, i32 0
  store volatile i32 %".16", i32* %".17"
  %".19" = getelementptr [2 x q1], [2 x q1]* %"qbits", i32 0, i32 1
  %".20" = load volatile q1, q1* %".19"
  %".21" = measure q1 %".20"
  %".22" = getelementptr [2 x i32], [2 x i32]* %"cbits", i32 0, i32 1
  store volatile i32 %".21", i32* %".22"
  ret void
}
