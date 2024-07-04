// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific If-else statement3.


void test_if_else_3(int basis) {
    // CHECK: %basis.addr = alloca i32, align 4
    // CHECK: %q = alloca q1, align 4
    // CHECK: store i32 %basis, i32* %basis.addr, align 4
    qbit q;
    // CHECK: %0 = load i32, i32* %basis.addr, align 4
    // CHECK: %cmp = icmp sle i32 %0, 0
    // CHECK: br i1 %cmp, label %if.then, label %if.else

    // CHECK: if.then:                         
    // CHECK: %1 = load q1, q1* %q, align 4
    // CHECK: %2 = x q1 %1
    // CHECK: store q1 %2, q1* %q, align 4
    // CHECK: br label %if.end4
    if (basis <= 0){
        x -> q;
    }
    // CHECK: if.else:                                     
    // CHECK: %3 = load i32, i32* %basis.addr, align 4
    // CHECK: %cmp1 = icmp sge i32 %3, 1
    // CHECK: br i1 %cmp1, label %if.then2, label %if.else3

    // CHECK: if.then2:                             
    // CHECK: %4 = load q1, q1* %q, align 4
    // CHECK: %5 = z q1 %4
    // CHECK: store q1 %5, q1* %q, align 4
    // CHECK: br label %if.end
    else if (basis >= 1){
        z -> q;
    }
    // CHECK: if.else3:                                       
    // CHECK: %6 = load q1, q1* %q, align 4
    // CHECK: %7 = z q1 %6
    // CHECK: store q1 %7, q1* %q, align 4
    // CHECK: br label %if.end
    else {
        z -> q;
    }

    // CHECK: if.end:                                          
    // CHECK: br label %if.end4

    // CHECK: if.end4:                                     
    // CHECK: %8 = load q1, q1* %q, align 4
    // CHECK: %9 = h q1 %8
    // CHECK: store q1 %9, q1* %q, align 4
    h -> q;
}