// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific grover_state statement.

void kernel_grover() {

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
    // CHECK: %arrayidx6 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %6 = load q1, q1* %arrayidx6, align 4
    // CHECK: %arrayidx7 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %7 = h q1 %6
    // CHECK: store q1 %7, q1* %arrayidx7, align 4
    // CHECK: %arrayidx8 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %8 = load q1, q1* %arrayidx8, align 4
    // CHECK: %arrayidx9 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %9 = h q1 %8
    // CHECK: store q1 %9, q1* %arrayidx9, align 4
    // CHECK: %arrayidx10 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %10 = load q1, q1* %arrayidx10, align 4
    // CHECK: %arrayidx11 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %11 = load q1, q1* %arrayidx11, align 4
    // CHECK: %arrayidx12 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %arrayidx13 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %12 = ctrl q1 %10
    // CHECK: store q1 %12, q1* %arrayidx12, align 4
    // CHECK: %13 = x q1 %11, %12
    // CHECK: store q1 %13, q1* %arrayidx13, align 4
    // CHECK: %arrayidx14 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %14 = load q1, q1* %arrayidx14, align 4
    // CHECK: %arrayidx15 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %15 = h q1 %14
    // CHECK: store q1 %15, q1* %arrayidx15, align 4
    // CHECK: %arrayidx16 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %16 = load q1, q1* %arrayidx16, align 4
    // CHECK: %arrayidx17 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %17 = h q1 %16
    // CHECK: store q1 %17, q1* %arrayidx17, align 4
    // CHECK: %arrayidx18 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %18 = load q1, q1* %arrayidx18, align 4
    // CHECK: %arrayidx19 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %19 = h q1 %18
    // CHECK: store q1 %19, q1* %arrayidx19, align 4
    // CHECK: %arrayidx20 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %20 = load q1, q1* %arrayidx20, align 4
    // CHECK: %arrayidx21 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %21 = x q1 %20
    // CHECK: store q1 %21, q1* %arrayidx21, align 4
    // CHECK: %arrayidx22 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %22 = load q1, q1* %arrayidx22, align 4
    // CHECK: %arrayidx23 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %23 = x q1 %22
    // CHECK: store q1 %23, q1* %arrayidx23, align 4
    // CHECK: %arrayidx24 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %24 = load q1, q1* %arrayidx24, align 4
    // CHECK: %arrayidx25 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %25 = h q1 %24
    // CHECK: store q1 %25, q1* %arrayidx25, align 4
    // CHECK: %arrayidx26 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %26 = load q1, q1* %arrayidx26, align 4
    // CHECK: %arrayidx27 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %27 = load q1, q1* %arrayidx27, align 4
    // CHECK: %arrayidx28 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %arrayidx29 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %28 = ctrl q1 %26
    // CHECK: store q1 %28, q1* %arrayidx28, align 4
    // CHECK: %29 = x q1 %27, %28
    // CHECK: store q1 %29, q1* %arrayidx29, align 4
    // CHECK: %arrayidx30 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %30 = load q1, q1* %arrayidx30, align 4
    // CHECK: %arrayidx31 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %31 = h q1 %30
    // CHECK: store q1 %31, q1* %arrayidx31, align 4
    // CHECK: %arrayidx32 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %32 = load q1, q1* %arrayidx32, align 4
    // CHECK: %arrayidx33 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %33 = x q1 %32
    // CHECK: store q1 %33, q1* %arrayidx33, align 4
    // CHECK: %arrayidx34 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %34 = load q1, q1* %arrayidx34, align 4
    // CHECK: %arrayidx35 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %35 = x q1 %34
    // CHECK: store q1 %35, q1* %arrayidx35, align 4
    // CHECK: %arrayidx36 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %36 = load q1, q1* %arrayidx36, align 4
    // CHECK: %arrayidx37 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %37 = h q1 %36
    // CHECK: store q1 %37, q1* %arrayidx37, align 4
    // CHECK: %arrayidx38 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %38 = load q1, q1* %arrayidx38, align 4
    // CHECK: %arrayidx39 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %39 = h q1 %38
    // CHECK: store q1 %39, q1* %arrayidx39, align 4
    // CHECK: %arrayidx40 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 0
    // CHECK: %40 = load q1, q1* %arrayidx40, align 4
    // CHECK: %41 = measure q1 %40
    // CHECK: %arrayidx41 = getelementptr inbounds [2 x i32], [2 x i32]* %c, i64 0, i64 0
    // CHECK: store i32 %41, i32* %arrayidx41, align 4
    // CHECK: %arrayidx42 = getelementptr inbounds [2 x q1], [2 x q1]* %q, i64 0, i64 1
    // CHECK: %42 = load q1, q1* %arrayidx42, align 4
    // CHECK: %43 = measure q1 %42
    // CHECK: %arrayidx43 = getelementptr inbounds [2 x i32], [2 x i32]* %c, i64 0, i64 1
    // CHECK: store i32 %43, i32* %arrayidx43, align 4

    qbit q[2];
    int c[2];

    prez -> q[0];
    prez -> q[1];

    h -> q[0];
    h -> q[1];
    h -> q[1];
    cnot -> q[0], q[1];

    h -> q[1];
    h -> q[0];
    h -> q[1];
    x -> q[0];
    x -> q[1];
    h -> q[1];
    cnot -> q[0], q[1];

    h -> q[1];
    x -> q[0];
    x -> q[1];
    h -> q[0];
    h -> q[1];

    c[0] = measure -> q[0];
    c[1] = measure -> q[1];
}