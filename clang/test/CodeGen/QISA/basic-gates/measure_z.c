// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific measure Z.

void test_measureZ_1() {
    // CHECK-LABEL: define dso_local void @test_measureZ_1() #0 {
    // CHECK: entry:

    // CHECK: %q = alloca q1, align 4
    qbit q;
    // CHECK: %c = alloca i32, align 4
    int c;

    // CHECK: %0 = load q1, q1* %q, align 4
    // CHECK: %1 = measure q1 %0
    // CHECK: store i32 %1, i32* %c, align 4
    c = measure -> q;
}

void test_measureZ_2() {
    // CHECK-LABEL: define dso_local void @test_measureZ_2() #0 {
    // CHECK: entry:

    // CHECK: %q = alloca q1, align 4
    qbit q;

    // CHECK: %q1 = alloca q1, align 4
    qbit q1;

    // CHECK: %c = alloca i32, align 4
    int c;

    // CHECK: %c1 = alloca i32, align 4
    int c1;

    // CHECK: %0 = load q1, q1* %q, align 4
    // CHECK: %1 = measure q1 %0
    // CHECK: store i32 %1, i32* %c, align 4
    c = measure -> q;

    // CHECK: %2 = load q1, q1* %q1, align 4
    // CHECK: %3 = measure q1 %2
    // CHECK: store i32 %3, i32* %c1, align 4    
    c1 = measure -> q1;
}