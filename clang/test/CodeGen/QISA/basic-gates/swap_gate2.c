// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific swap gate2.


void test_swap_2() {
    // CHECK: %q = alloca q1, align 4
    qbit q;
    
    // CHECK: %q1 = alloca q1, align 4
    qbit q1;
    
    // CHECK: %0 = load q1, q1* %q, align 4
    // CHECK: %1 = x q1 %0
    // CHECK: store q1 %1, q1* %q, align 4
    x -> q;

    // CHECK: %2 = load q1, q1* %q, align 4
    // CHECK: %3 = load q1, q1* %q1, align 4
    // CHECK: %4 = ctrl q1 %2
    // CHECK: store q1 %4, q1* %q, align 4
    // CHECK: %5 = swap q1 %3, %4
    // CHECK: store q1 %5, q1* %q1, align 4
    swap -> q, q1;
}