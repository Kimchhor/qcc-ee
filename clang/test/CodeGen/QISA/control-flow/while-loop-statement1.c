
// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific while-loop statement1.


void test_while_loop_1() {
    // CHECK: %q = alloca [3 x q1], align 4
    // CHECK: %counter = alloca i32, align 4
    // CHECK: store i32 0, i32* %counter, align 4
    // CHECK: br label %while.cond
    
    qbit q[3];
    int counter = 0;

    // CHECK: while.cond:                                 
    // CHECK:   %0 = load i32, i32* %counter, align 4
    // CHECK:   %cmp = icmp slt i32 %0, 3
    // CHECK:   br i1 %cmp, label %while.body, label %while.end

    // CHECK: while.body:
    while(counter < 3){
        // CHECK:   %arrayidx = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 3
        // CHECK:   %1 = load q1, q1* %arrayidx, align 4
        // CHECK:   %arrayidx1 = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 3
        // CHECK:   %2 = init_z q1 %1
        // CHECK:   store q1 %2, q1* %arrayidx1, align 4
        prez -> q[3];

        // CHECK:   %3 = load i32, i32* %counter, align 4
        // CHECK:   %inc = add nsw i32 %3, 1
        // CHECK:   store i32 %inc, i32* %counter, align 4
        counter++;

        // CHECK:   br label %while.cond

    }
}

