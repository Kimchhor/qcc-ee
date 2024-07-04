	.text
OPENQASM 2.0;
include "qelib1.inc";
qreg q[2];                              # -- Begin function kernel
creg c[2];
                                        # @kernel
                                        # %entry
	x q[0];
	z q[0];

	h q[0];




	LDD	256, 40964

	x q[0];
	measure q[0] -> c[0];



	.section	".note.GNU-stack","",@progbits
