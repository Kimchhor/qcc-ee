	.text
kernel:                                 # -- Begin function kernel
                                        # @kernel
# %bb.0:                                # %entry
	LDD R0, [r31, #4]
	QMOV QR0, R0
	INIT_Z QR0
	HADAMARD QR0
	MEASURE_Z QR0
	FMR R0, QR0
	STD [r31], R0
	MOV R1, 1
	CMP R0, R1
	BRGE .BB0_2
	BRI .BB0_1
.BB0_2:                                 # %if.else
	LDD R0, [r31]
	DEC R0, 1
	STD [r31], R0
.BB0_3:                                 # %if.end
	LDD R2, [r31]
	JMP R10
.BB0_1:                                 # %if.then
	LDD R0, [r31]
	INC R0, 1
	STD [r31], R0
	BRI .BB0_3
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
