	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.globl	_BOARD_Init             ## -- Begin function BOARD_Init
	.p2align	4, 0x90
_BOARD_Init:                            ## @BOARD_Init
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_BOARD_End              ## -- Begin function BOARD_End
	.p2align	4, 0x90
_BOARD_End:                             ## @BOARD_End
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	xorl	%edi, %edi
	callq	_exit
	.cfi_endproc
                                        ## -- End function
	.globl	_BOARD_GetPBClock       ## -- Begin function BOARD_GetPBClock
	.p2align	4, 0x90
_BOARD_GetPBClock:                      ## @BOARD_GetPBClock
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	$20000000, %eax         ## imm = 0x1312D00
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_BOARD_GetSysClock      ## -- Begin function BOARD_GetSysClock
	.p2align	4, 0x90
_BOARD_GetSysClock:                     ## @BOARD_GetSysClock
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	$80000000, %eax         ## imm = 0x4C4B400
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_SERIAL_Init            ## -- Begin function SERIAL_Init
	.p2align	4, 0x90
_SERIAL_Init:                           ## @SERIAL_Init
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function

.subsections_via_symbols
