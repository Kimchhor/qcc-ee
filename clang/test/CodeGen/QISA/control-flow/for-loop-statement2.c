
// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific for-loop statement2.


void test_for_loop_2() {
    // CHECK: %q = alloca [3 x q1], align 4
    // CHECK: %i = alloca i32, align 4
    // CHECK: store i32 1, i32* %i, align 4
    // CHECK: br label %for.cond
    qbit q[3];

    // CHECK: for.cond:
    // CHECK:   %0 = load i32, i32* %i, align 4
    // CHECK:   %cmp = icmp slt i32 %0, 3
    // CHECK:   br i1 %cmp, label %for.body, label %for.end

    // CHECK: for.body:
    // CHECK:   %arrayidx = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 0
    // CHECK:   %1 = load q1, q1* %arrayidx, align 4
    // CHECK:   %2 = load i32, i32* %i, align 4
    // CHECK:   %idxprom = sext i32 %2 to i64
    // CHECK:   %arrayidx1 = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 %idxprom
    // CHECK:   %3 = load q1, q1* %arrayidx1, align 4
    // CHECK:   %arrayidx2 = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 0
    // CHECK:   %4 = load i32, i32* %i, align 4
    // CHECK:   %idxprom3 = sext i32 %4 to i64
    for (int i = 1; i < 3; i++)
        // CHECK:   %arrayidx4 = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 %idxprom3
        // CHECK:   %5 = ctrl q1 %1
        // CHECK:   store q1 %5, q1* %arrayidx2, align 4
        // CHECK:   %6 = x q1 %3, %5
        cnot -> q[0], q[i];
    // CHECK:   store q1 %6, q1* %arrayidx4, align 4
    // CHECK:   br label %for.inc

}