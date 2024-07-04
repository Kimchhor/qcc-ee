// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific swap gate1.


void test_swap_1() {
    // CHECK: %q = alloca q1, align 4
    qbit q;
    
    // CHECK: %q1 = alloca q1, align 4
    qbit q1;
    // CHECK: %0 = load q1, q1* %q, align 4
    // CHECK: %1 = load q1, q1* %q1, align 4
    // CHECK: %2 = ctrl q1 %0
    // CHECK: store q1 %2, q1* %q, align 4
    // CHECK: %3 = swap q1 %1, %2
    // CHECK: store q1 %3, q1* %q1, align 4
    swap -> q, q1;
}