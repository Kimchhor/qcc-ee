// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific If-else statement1.

void test_if_else_1(int basis) {
    // CHECK: %basis.addr = alloca i32, align 4
    // CHECK: %q = alloca q1, align 4
    qbit q;

    // CHECK: store i32 %basis, i32* %basis.addr, align 4
    // CHECK: %0 = load i32, i32* %basis.addr, align 4
    // CHECK: %cmp = icmp eq i32 %0, 0
    // CHECK: br i1 %cmp, label %if.then, label %if.end

    // CHECK: if.then:                            
    if (basis == 0)
    // CHECK:   %1 = load q1, q1* %q, align 4
    // CHECK:   %2 = x q1 %1
    // CHECK:   store q1 %2, q1* %q, align 4
        x -> q;
    // CHECK:   br label %if.end
    

    // CHECK: if.end:                                          
    // CHECK:   %3 = load q1, q1* %q, align 4
    // CHECK:   %4 = h q1 %3
    // CHECK:   store q1 %4, q1* %q, align 4
    h -> q;
}