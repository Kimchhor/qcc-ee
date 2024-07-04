// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific swap gate3.


void test_swap_3() {
    // CHECK: %q = alloca q1, align 4
    // CHECK: %q1 = alloca q1, align 4
    qbit q, q1;
    
    // CHECK: %c = alloca i32, align 4
    // CHECK: %c1 = alloca i32, align 4
    int c, c1;
    
    // CHECK: %0 = load q1, q1* %q, align 4
    // CHECK: %1 = z q1 %0
    // CHECK: store q1 %1, q1* %q, align 4
    z -> q;
    
    // CHECK: %2 = load q1, q1* %q, align 4
    // CHECK: %3 = load q1, q1* %q1, align 4
    // CHECK: %4 = ctrl q1 %2
    // CHECK: store q1 %4, q1* %q, align 4
    // CHECK: %5 = swap q1 %3, %4
    // CHECK: store q1 %5, q1* %q1, align 4
    swap -> q, q1;
    
    // CHECK: %6 = load q1, q1* %q, align 4
    // CHECK: %7 = measure_x q1 %6
    // CHECK: store i32 %7, i32* %c, align 4
    c = measureX -> q;
    
    // CHECK: %8 = load q1, q1* %q1, align 4
    // CHECK: %9 = measure q1 %8
    // CHECK: store i32 %9, i32* %c, align 4
    c = measure -> q1;    


}