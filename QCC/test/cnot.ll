; ModuleID = 'cnot.ll'
source_filename = "cnot.cpp"

define dso_local i32 @main() {
 
  ; qreg q[2];
  %qq0 = alloca q1
  %qq1 = alloca q1
  %qq2 = alloca q1
  %qq3 = alloca q1
  %qq4 = alloca q1
  %qq5 = alloca q1
  %qq6 = alloca q1
  %qq7 = alloca q1

  ; creg c[2];
  %c1 = alloca i1
  %c2 = alloca i1
  %c3 = alloca i1

  %1 = x q1* %qq0

  %2 = h q1* %1                ; 0

  %3 = cx0 q1* %2,    i32 1    ; 0
  %4 = cx1 q1* %qq5, i32 1     ; 5

  %5 = z q1* %3		; 0	 
  %6 = cx0 q1* %5, i32 2		; 0	
  %7 = cx1 q1* %qq3, i32 2		; 3

  %8 = x q1* %6		; 0
	%9 = cx0 q1* %qq2,  i32 3		; 2
  %10 = cx1 q1* %qq4, i32 3   ; 4

  %11 = h q1* %7		; 4	
  %12 = cx0 q1* %qq1, i32 4		; 1	
  %13 = cx1 q1* %qq6, i32 4		; 6	

  %14 = measure q1* %10, i1* %c3
  %15 = measure q1* %13, i1* %c2

  ret i32 0
}

