	.text
kernel:                                 # -- Begin function kernel
                                        # @kernel
# %bb.0:                                # %entry
	STD [r31, #12], R2
.BB0_1:                                 # %for.cond
                                        # =>This Inner Loop Header: Depth=1
	LDD R0, [r31, #12]
	LDD R1, [r31, #4]
	CMP R1, R0
	BRGE .BB0_4
	BRI .BB0_2
.BB0_2:                                 # %for.body
                                        #   in Loop: Header=BB0_1 Depth=1
	LDD R0, [r31, #8]
	QMOV QR0, R0
	INIT_Z QR0
	HADAMARD QR0
# %bb.3:                                # %for.inc
                                        #   in Loop: Header=BB0_1 Depth=1
	LDD R0, [r31, #4]
	INC R0, 1
	STD [r31, #4], R0
	BRI .BB0_1
.BB0_4:                                 # %for.end
	LDD R0, [r31, #8]
	QMOV QR0, R0
	INIT_Z QR0
	MEASURE_Z QR0
	FMR R2, QR0
	JMP R10
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
