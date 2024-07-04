; RUN: opt -passes=qasm-gen %s --disable-output &> %t
; RUN: diff --strip-trailing-cr %t %S/adder.qasm

define dso_local i32 @main() {
  
  %1 = alloca q1
  %2 = alloca q1
  %3 = alloca q1
  %4 = alloca q1
  
  %5 = alloca i1
  %6 = alloca i1
  %7 = alloca i1
  %8 = alloca i1

  %9 = x q1* %1     
  %10 = x q1* %2 
  %11 = h q1* %4  
  %12 = cx0 q1* %3, i32 1  
  %13 = cx1 q1* %11, i32 1 
  %14 = t q1* %9
  %15 = t q1* %10 
  %16 = t q1* %12
  %17 = tdg q1* %13
  %18 = cx0 q1* %14, i32 2	
  %19 = cx1 q1* %15, i32 2		
  %20 = cx0 q1* %16, i32 3		
  %21 = cx1 q1* %17, i32 3	
  %22 = cx0 q1* %21, i32 4		
  %23 = cx1 q1* %18, i32 4
  %24 = cx0 q1* %19, i32 5
  %25 = cx1 q1* %20, i32 5	
  %26 = cx0 q1* %23, i32 6 
  %27 = cx1 q1* %24, i32 6	
  %28 = cx0 q1* %25, i32 7	
  %29 = cx1 q1* %22, i32 7 
  %30 = tdg q1* %26	
  %31 = tdg q1* %27	
  %32 = tdg q1* %28	
  %33 = t q1* %29	  
  %34 = cx0 q1* %30, i32 8  
  %35 = cx1 q1* %31, i32 8	
  %36 = cx0 q1* %32, i32 9	
  %37 = cx1 q1* %33, i32 9	
  %38 = s q1* %37	
  %39 = cx0 q1* %38, i32 10	
  %40 = cx1 q1* %34, i32 10	
  %41 = h q1* %39	
  %42 = measure q1* %40, i1* %5		; 1	
  %43 = measure q1* %35, i1* %6		; 2
  %44 = measure q1* %36, i1* %7		; 3		
  %45 = measure q1* %41, i1* %8		; 4	


  ret i32 0
}
