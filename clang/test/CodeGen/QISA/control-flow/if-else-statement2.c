// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific If-else statement2.


void test_if_else_2(int basis) {
    // CHECK: %basis.addr = alloca i32, align 4
    // CHECK: %q = alloca q1, align 4
    // CHECK: store i32 %basis, i32* %basis.addr, align 4
    // CHECK: %0 = load i32, i32* %basis.addr, align 4
    qbit q;

    // CHECK: %cmp = icmp ne i32 %0, 0
    // CHECK: br i1 %cmp, label %if.then, label %if.else

    // CHECK: if.then:
    if (basis != 0)
    // CHECK: %1 = load q1, q1* %q, align 4
    // CHECK: %2 = x q1 %1
    // CHECK: store q1 %2, q1* %q, align 4
        x -> q;
    // CHECK: br label %if.end
    
    // CHECK: if.else:
    else
    // CHECK: %3 = load q1, q1* %q, align 4
    // CHECK: %4 = z q1 %3
    // CHECK: store q1 %4, q1* %q, align 4
        z -> q;
    // CHECK: br label %if.end
    
    // CHECK: if.end:                                    
    // CHECK: %5 = load q1, q1* %q, align 4
    // CHECK: %6 = h q1 %5
    // CHECK: store q1 %6, q1* %q, align 4

    h -> q;
}