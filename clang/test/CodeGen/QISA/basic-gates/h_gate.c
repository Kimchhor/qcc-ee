// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific Hadamard gate.

void test_h_gate_1() {

    // CHECK: %q = alloca q1, align 4
    qbit q;

    // CHECK: %0 = load q1, q1* %q, align 4
    // CHECK: %1 = h q1 %0
    // CHECK: store q1 %1, q1* %q, align 4
    h -> q;
}

void test_h_gate_2() {

    // CHECK: %q = alloca q1, align 4
    qbit q;

    // CHECK: %q1 = alloca q1, align 4
    qbit q1;

    // CHECK: %0 = load q1, q1* %q, align 4
    // CHECK: %1 = h q1 %0
    // CHECK: store q1 %1, q1* %q, align 4
    h -> q;
    
    // CHECK: %2 = load q1, q1* %q1, align 4
    // CHECK: %3 = h q1 %2
    // CHECK: store q1 %3, q1* %q1, align 4
    h -> q1;
}