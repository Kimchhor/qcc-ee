// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific do-while statement1.


void test_do_while_1() {

    // CHECK: %q = alloca [3 x q1], align 4
    // CHECK: %counter = alloca i32, align 4
    // CHECK: store i32 0, i32* %counter, align 4
    // CHECK: br label %do.body
    qbit q[3];
    int counter = 0;

    // CHECK: do.body:
    do{
        // CHECK:   %0 = load i32, i32* %counter, align 4
        // CHECK:   %inc = add nsw i32 %0, 1
        // CHECK:   store i32 %inc, i32* %counter, align 4
        // CHECK:   %arrayidx = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 3
        // CHECK:   %1 = load q1, q1* %arrayidx, align 4
        counter++;

        // CHECK:   %arrayidx1 = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 3
        // CHECK:   %2 = init_z q1 %1
        // CHECK:   store q1 %2, q1* %arrayidx1, align 4
        prez -> q[3];
    
    // CHECK:   br label %do.cond
    // CHECK: do.cond:
    // CHECK:   %3 = load i32, i32* %counter, align 4
    // CHECK:   %cmp = icmp slt i32 %3, 3
    // CHECK:   br i1 %cmp, label %do.body, label %do.end
    }while(counter < 3);
}