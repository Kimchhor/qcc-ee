	
OPENQASM 2.0;
include "qelib1.inc";

    qreg q[2];
    creg c[2];
	x q[0];                                 // @kernel
                                        // %entry
	h q[0];
	swap q[0], q[1];

