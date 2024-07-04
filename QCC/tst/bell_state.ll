; ModuleID = 'bell_state.ll'
source_filename = "bell_state.cpp"

define dso_local i1* @main() {
 
  %1 = alloca q1    ; q0       
  %2 = alloca q1    ; q1    
  %3 = alloca q1    ; q2
  %4 = alloca i1

  %5 = x q1* %1		          ; x	q0
  %6 = z q1* %2		          ; z q1
  %7 = cx0 q1* %5, i32 1		; cx0 q1		
  %8 = cx1 q1* %3, i32 1    ; cx1 q2

  %9 = measure q1* %8, i1* %4		; measure q2	

  ret i1* %4
}

