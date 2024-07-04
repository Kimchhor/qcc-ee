	.text
kernel:                                 # -- Begin function kernel
                                        # @kernel
# %bb.0:                                # %entry
	LDD R0, [r31, #8]
	QMOV QR0, R0
	INIT_Z QR0
	HADAMARD QR0
	STD [r31, #12], R2
	MOV R0, 0
	CMP R2, R0
	BRNE .BB0_2
	BRI .BB0_1
.BB0_2:                                 # %if.else
	MOV R0, 1
	LDD R1, [r31, #12]
	CMP R1, R0
	BRNE .BB0_4
	BRI .BB0_3
.BB0_3:                                 # %if.then2
	LDD R0, [r31, #8]
	QMOV QR0, R0
	INIT_Z QR0
	Z QR0
	BRI .BB0_5
.BB0_1:                                 # %if.then
	LDD R0, [r31, #8]
	QMOV QR0, R0
	INIT_Z QR0
	X QR0
	BRI .BB0_5
.BB0_4:                                 # %if.else3
	LDD R0, [r31, #8]
	QMOV QR0, R0
	INIT_Z QR0
	HADAMARD QR0
.BB0_5:                                 # %if.end4
	LDD R0, [r31, #8]
	QMOV QR0, R0
	INIT_Z QR0
	MEASURE_Z QR0
	FMR R2, QR0
	STD [r31, #4], R2
	JMP R10
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
