
// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific while-loop statement3.


void test_while_loop_3(){
    // CHECK: %q = alloca q1, align 4
    // CHECK: %counter = alloca i32, align 4
    // CHECK: %zero_counter = alloca i32, align 4
    // CHECK: %one_counter = alloca i32, align 4
    // CHECK: %0 = load q1, q1* %q, align 4
    // CHECK: %1 = h q1 %0
    // CHECK: store q1 %1, q1* %q, align 4
    // CHECK: store i32 5, i32* %counter, align 4
    // CHECK: store i32 0, i32* %zero_counter, align 4
    // CHECK: store i32 0, i32* %one_counter, align 4
    // CHECK: br label %while.cond
    
    qbit q;
    h -> q;

    int counter = 5;
    int zero_counter = 0;
    int one_counter = 0;

    // CHECK: while.cond:
    // CHECK:   %2 = load i32, i32* %counter, align 4
    // CHECK:   %cmp = icmp sgt i32 %2, 1
    // CHECK:   br i1 %cmp, label %while.body, label %while.end

    // CHECK: while.body:
    // CHECK:   %3 = load q1, q1* %q, align 4
    // CHECK:   %4 = measure q1 %3
    // CHECK:   %cmp1 = icmp eq i32 %4, 0
    // CHECK:   br i1 %cmp1, label %if.then, label %if.else

    while(counter > 1){
        
        // CHECK: if.then:
        // CHECK:   %5 = load i32, i32* %zero_counter, align 4
        // CHECK:   %inc = add nsw i32 %5, 1
        // CHECK:   store i32 %inc, i32* %zero_counter, align 4
        // CHECK:   br label %if.end
        if ((measure -> q) == 0)
            zero_counter++;
 
        // CHECK: if.else:
        // CHECK:   %6 = load i32, i32* %one_counter, align 4
        // CHECK:   %inc2 = add nsw i32 %6, 1
        // CHECK:   store i32 %inc2, i32* %one_counter, align 4
        // CHECK:   br label %if.end
        else
            one_counter++;

        // CHECK:   %dec = add nsw i32 %7, -1
        // CHECK:   store i32 %dec, i32* %counter, align 4
        // CHECK:   br label %while.cond
        counter--;
    }

}