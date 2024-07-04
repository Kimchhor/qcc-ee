// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific bell_state statement1.

void kernel_bell_1() {
    // CHECK: %q = alloca [2 x q1], align 4
    // CHECK: %c = alloca [2 x i32], align 4
    // CHECK: %arrayidx = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %0 = load q1, q1* %arrayidx, align 4
    // CHECK: %arrayidx1 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %1 = init_z q1 %0
    // CHECK: store q1 %1, q1* %arrayidx1, align 4
    // CHECK: %arrayidx2 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %2 = load q1, q1* %arrayidx2, align 4
    // CHECK: %arrayidx3 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %3 = init_z q1 %2
    // CHECK: store q1 %3, q1* %arrayidx3, align 4
    // CHECK: %arrayidx4 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %4 = load q1, q1* %arrayidx4, align 4
    // CHECK: %arrayidx5 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %5 = h q1 %4
    // CHECK: store q1 %5, q1* %arrayidx5, align 4
    // CHECK: %arrayidx6 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %6 = load q1, q1* %arrayidx6, align 4
    // CHECK: %arrayidx7 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %7 = load q1, q1* %arrayidx7, align 4
    // CHECK: %arrayidx8 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %arrayidx9 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %8 = ctrl q1 %6
    // CHECK: store q1 %8, q1* %arrayidx8, align 4
    // CHECK: %9 = x q1 %7, %8
    // CHECK: store q1 %9, q1* %arrayidx9, align 4
    // CHECK: %arrayidx10 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %10 = load q1, q1* %arrayidx10, align 4
    // CHECK: %11 = measure q1 %10
    // CHECK: %arrayidx11 = getelementptr inbounds [2 x i32], [2 x i32]* %c, i64 0, i64 0
    // CHECK: store i32 %11, i32* %arrayidx11, align 4
    // CHECK: %arrayidx12 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %12 = load q1, q1* %arrayidx12, align 4
    // CHECK: %13 = measure q1 %12
    // CHECK: %arrayidx13 = getelementptr inbounds [2 x i32], [2 x i32]* %c, i64 0, i64 1
    // CHECK: store i32 %13, i32* %arrayidx13, align 4

    qbit q[2];
    int c[2];

    prez -> q[0];
    prez -> q[1];

    h -> q[0];
    cnot -> q[0], q[1];

    c[0] = measure -> q[0];
    c[1] = measure -> q[1];
}