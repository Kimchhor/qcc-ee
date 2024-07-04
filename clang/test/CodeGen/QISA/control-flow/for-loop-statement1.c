
// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific for-loop statement1.

void test_for_loop_1() {

    // CHECK: %q = alloca [3 x q1], align 4
    // CHECK: %i = alloca i32, align 4
    // CHECK: store i32 0, i32* %i, align 4
    // CHECK: br label %for.cond
    qbit q[3];

    // CHECK: for.cond:                        
    // CHECK: %0 = load i32, i32* %i, align 4
    // CHECK: %cmp = icmp slt i32 %0, 3
    // CHECK: br i1 %cmp, label %for.body, label %for.end

    // CHECK: for.body:      
    // CHECK:   %1 = load i32, i32* %i, align 4
    // CHECK:   %idxprom = sext i32 %1 to i64
    // CHECK:   %arrayidx = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 %idxprom
    // CHECK:   %2 = load q1, q1* %arrayidx, align 4
    // CHECK:   %3 = load i32, i32* %i, align 4
    // CHECK:   %idxprom1 = sext i32 %3 to i64
    // CHECK:   %arrayidx2 = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 %idxprom1
    // CHECK:   %4 = init_z q1 %2
    // CHECK:   store q1 %4, q1* %arrayidx2, align 4
    // CHECK:   br label %for.inc
    for (int i = 0; i < 3; i++)
        prez -> q[i];

}