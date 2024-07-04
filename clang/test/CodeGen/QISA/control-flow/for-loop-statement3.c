
// RUN: %clang_cc1 -emit-llvm %s -o - \
// RUN:     | FileCheck %s

// Test QCC specific for-loop statement3.


void test_for_loop_3(){

    //CHECK: %q = alloca [3 x q1], align 4
    //CHECK: %c = alloca [3 x i32], align 4
    //CHECK: %i = alloca i32, align 4
    //CHECK: %i3 = alloca i32, align 4
    //CHECK: store i32 0, i32* %i, align 4
    //CHECK: br label %for.cond
    qbit q[3];
    int c[3];
    
    //CHECK: for.cond:
    //CHECK: %0 = load i32, i32* %i, align 4
    //CHECK: %cmp = icmp slt i32 %0, 3
    //CHECK: br i1 %cmp, label %for.body, label %for.end

    //CHECK: for.body:
    //CHECK:    %1 = load i32, i32* %i, align 4
    //CHECK:    %idxprom = sext i32 %1 to i64
    //CHECK:    %arrayidx = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 %idxprom
    //CHECK:    %2 = load q1, q1* %arrayidx, align 4
    //CHECK:    %3 = load i32, i32* %i, align 4
    //CHECK:    %idxprom1 = sext i32 %3 to i64
    for (int i = 0; i < 3; i++)
        //CHECK:    %arrayidx2 = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 %idxprom1
        //CHECK:    %4 = h q1 %2
        //CHECK:    store q1 %4, q1* %arrayidx2, align 4
        h -> q[i];

    //CHECK:    br label %for.inc

    //CHECK: for.inc:                                     
    //CHECK:    %5 = load i32, i32* %i, align 4
    //CHECK:    %inc = add nsw i32 %5, 1
    //CHECK:    store i32 %inc, i32* %i, align 4
    //CHECK:    br label %for.cond

    //CHECK: for.end:                                       
    //CHECK:    store i32 0, i32* %i3, align 4
    //CHECK:    br label %for.cond4

    //CHECK: for.cond4:                                      
    //CHECK:    %6 = load i32, i32* %i3, align 4
    //CHECK:    %cmp5 = icmp slt i32 %6, 3
    //CHECK:    br i1 %cmp5, label %for.body6, label %for.end13

    //CHECK: for.body6:                                       
    //CHECK:    %7 = load i32, i32* %i3, align 4
    //CHECK:    %idxprom7 = sext i32 %7 to i64
    //CHECK:    %arrayidx8 = getelementptr inbounds [3 x q1], [3 x q1]* %q, i64 0, i64 %idxprom7
    for (int i = 0; i < 3; i++)
        //CHECK:    %8 = load q1, q1* %arrayidx8, align 4
        //CHECK:    %9 = measure q1 %8
        c[i] = measure -> q[i];

    //CHECK:    %10 = load i32, i32* %i3, align 4
    //CHECK:    %idxprom9 = sext i32 %10 to i64
    //CHECK:    %arrayidx10 = getelementptr inbounds [3 x i32], [3 x i32]* %c, i64 0, i64 %idxprom9
    //CHECK:    store i32 %9, i32* %arrayidx10, align 4
    //CHECK:    br label %for.inc11


}