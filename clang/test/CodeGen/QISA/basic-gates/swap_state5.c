// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific swap_state statement5.

void kernel_swap_5() {
    // CHECK: %q = alloca [2 x q1], align 4
    // CHECK: %c = alloca [2 x i32], align 4
    // CHECK: %arrayidx = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %0 = load q1, q1* %arrayidx, align 4
    // CHECK: %arrayidx1 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %1 = init_x q1 %0
    // CHECK: store q1 %1, q1* %arrayidx1, align 4
    // CHECK: %arrayidx2 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %2 = load q1, q1* %arrayidx2, align 4
    // CHECK: %arrayidx3 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %3 = init_x q1 %2
    // CHECK: store q1 %3, q1* %arrayidx3, align 4
    // CHECK: %arrayidx4 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %4 = load q1, q1* %arrayidx4, align 4
    // CHECK: %arrayidx5 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %5 = load q1, q1* %arrayidx5, align 4
    // CHECK: %arrayidx6 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %arrayidx7 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %6 = ctrl q1 %4
    // CHECK: store q1 %6, q1* %arrayidx6, align 4
    // CHECK: %7 = swap q1 %5, %6
    // CHECK: store q1 %7, q1* %arrayidx7, align 4
    // CHECK: %arrayidx8 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %8 = load q1, q1* %arrayidx8, align 4
    // CHECK: %9 = measure q1 %8
    // CHECK:%arrayidx9 = getelementptr inbounds [2 x i32], [2 x i32]* %c, i64 0, i64 0
    // CHECK:store i32 %9, i32* %arrayidx9, align 4
    // CHECK: %arrayidx10 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %10 = load q1, q1* %arrayidx10, align 4
    // CHECK: %11 = measure q1 %10
    // CHECK: %arrayidx11 = getelementptr inbounds [2 x i32], [2 x i32]* %c, i64 0, i64 1
    // CHECK: store i32 %11, i32* %arrayidx11, align 4

    qbit q[2];
    int c[2];

    prex -> q[0];
    prex -> q[1];

    swap -> q[0], q[1];

    c[0] = measure -> q[0];
    c[1] = measure -> q[1];
}