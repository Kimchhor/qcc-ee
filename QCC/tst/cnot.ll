; ModuleID = 'cnot.ll'
source_filename = "cnot.cpp"

define dso_local i1* @main() {
 
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

  %1 = h q1* %qq0

  %2 = x q1* %1                ; 0

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

  %14 = cx0 q1* %8, i32 5		; 0	
  %15 = cx1 q1* %qq7, i32 5		; 7
  
  %16 = swap q1* %13, i32 6		; 6	
  %17 = swap q1* %12, i32 6		; 1

  %18 = measure q1* %16, i1* %c3
  %19 = measure q1* %17, i1* %c2

  ret i1* %c3
}

