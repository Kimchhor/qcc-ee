	
OPENQASM 2.0;
include "qelib1.inc";

    qreg q[2];
    creg c[2];
	h q[1];                                 // @circuit-160
                                        // %entry
	cx q[1], q[1];
	measure q[1] -> c[1];
	measure q[1] -> c[1];
