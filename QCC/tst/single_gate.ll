; ModuleID = 'single_gate.ll'
source_filename = "single_gate.cpp"

define dso_local i1* @main() {
 
  %1 = alloca q1    ; q0    
  %q1 = alloca q1   ; q1      
  %c1 = alloca i1		; c0	 

  %2 = h q1* %1		  ; q0
  %3 = x q1* %q1		; q1	
  %4 = z q1* %3		  ; q1	
  %5 = measure q1* %4, i1* %c1		; q1	

  ret i1* %c1
}

