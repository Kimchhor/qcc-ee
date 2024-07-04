// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific do-while statement2.


void test_do_while_2() {


    // CHECK: %q = alloca q1, align 4
    // CHECK: %time = alloca i32, align 4
    // CHECK: %0 = load q1, q1* %q, align 4
    // CHECK: %1 = h q1 %0
    // CHECK: store q1 %1, q1* %q, align 4
    // CHECK: store i32 0, i32* %time, align 4
    // CHECK: br label %do.body
    qbit q;
    h -> q;
    int time = 0;

    // CHECK: do.body:
    // CHECK:   %2 = load i32, i32* %time, align 4
    // CHECK:   %inc = add nsw i32 %2, 1
    // CHECK:   store i32 %inc, i32* %time, align 4
    // CHECK:   br label %do.cond
    do{
        time++;
        // CHECK: do.cond:
        // CHECK:   %3 = load q1, q1* %q, align 4
        // CHECK:   %4 = measure q1 %3
        // CHECK:   %cmp = icmp eq i32 %4, 0
        // CHECK:   br i1 %cmp, label %do.body, label %do.end
    }while((measure -> q)==0);

}