// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific do-while statement3.


void test_do_while_3(){

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
    // CHECK: br label %do.body
    qbit q;
    h -> q;
    int counter = 5;
    int zero_counter = 0;
    int one_counter = 0;

    // CHECK: do.body: 
    // CHECK:   %2 = load q1, q1* %q, align 4
    // CHECK:   %3 = measure q1 %2
    // CHECK:   %cmp = icmp eq i32 %3, 0
    // CHECK:   br i1 %cmp, label %if.then, label %if.else
    do{
        // CHECK: if.then:
        // CHECK:   %4 = load i32, i32* %zero_counter, align 4
        // CHECK:   %inc = add nsw i32 %4, 1
        // CHECK:   store i32 %inc, i32* %zero_counter, align 4
        // CHECK:   br label %if.end
        if ((measure -> q) == 0)
            zero_counter++;
        // CHECK: if.else:
        // CHECK:   %5 = load i32, i32* %one_counter, align 4
        // CHECK:   %inc1 = add nsw i32 %5, 1
        // CHECK:   store i32 %inc1, i32* %one_counter, align 4
        // CHECK:   br label %if.end
        else
            one_counter++;
        // CHECK: if.end:
        // CHECK:   %6 = load i32, i32* %counter, align 4
        // CHECK:   %dec = add nsw i32 %6, -1
        // CHECK:   store i32 %dec, i32* %counter, align 4
        // CHECK:   br label %do.cond
        counter--;

    // CHECK: do.cond:
    // CHECK:   %7 = load i32, i32* %counter, align 4
    // CHECK:   %cmp2 = icmp sgt i32 %7, 1
    // CHECK:   br i1 %cmp2, label %do.body, label %do.end
    }while(counter > 1);

}