
// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific while-loop statement2.


void test_while_loop_2() {
    // CHECK: %q = alloca q1, align 4
    // CHECK: %time = alloca i32, align 4
    // CHECK: %0 = load q1, q1* %q, align 4
    // CHECK: %1 = h q1 %0
    // CHECK: store q1 %1, q1* %q, align 4
    // CHECK: store i32 0, i32* %time, align 4
    // CHECK: br label %while.cond
    
    qbit q;
    h -> q;
    int time = 0; 

    // CHECK: while.cond:
    // CHECK:   %2 = load q1, q1* %q, align 4
    // CHECK:   %3 = measure q1 %2
    // CHECK:   %cmp = icmp eq i32 %3, 0
    // CHECK:   br i1 %cmp, label %while.body, label %while.end

    // CHECK: while.body:
    // CHECK:   %4 = load i32, i32* %time, align 4
    // CHECK:   %inc = add nsw i32 %4, 1
    // CHECK:   store i32 %inc, i32* %time, align 4
    // CHECK:   br label %while.cond

    while((measure -> q)==0){
        time++;
    }
}