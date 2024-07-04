	.text
kernel:                                 # -- Begin function kernel
                                        # @kernel
# %bb.0:                                # %entry
	LDD R0, [r31, #4]
	QMOV QR0, R0
	INIT_Z QR0
	X QR0
	Z QR0
	HADAMARD QR0
	MEASURE_Z QR0
	FMR R2, QR0
	STD [r31], R2
	JMP R10
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
