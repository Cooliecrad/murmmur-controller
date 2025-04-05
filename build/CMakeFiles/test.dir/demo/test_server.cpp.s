	.file	"test_server.cpp"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.section	.text$_Z6printfPKcz,"x"
	.linkonce discard
	.globl	_Z6printfPKcz
	.def	_Z6printfPKcz;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z6printfPKcz
_Z6printfPKcz:
.LFB8:
	.file 1 "E:/Code_Lib/compilers/mingw64/x86_64-w64-mingw32/include/stdio.h"
	.loc 1 346 1
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.seh_pushreg	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$56, %rsp
	.seh_stackalloc	56
	.cfi_def_cfa_offset 80
	leaq	128(%rsp), %rbp
	.seh_setframe	%rbp, 128
	.cfi_def_cfa 6, -48
	.seh_endprologue
	movq	%rcx, -48(%rbp)
	movq	%rdx, -40(%rbp)
	movq	%r8, -32(%rbp)
	movq	%r9, -24(%rbp)
	.loc 1 348 53
	leaq	-40(%rbp), %rax
	movq	%rax, -96(%rbp)
	.loc 1 349 30
	movq	-96(%rbp), %rbx
	movl	$1, %ecx
	movq	__imp___acrt_iob_func(%rip), %rax
	call	*%rax
.LVL0:
	movq	%rbx, %r8
	movq	-48(%rbp), %rdx
	movq	%rax, %rcx
	call	__mingw_vfprintf
	movl	%eax, -84(%rbp)
	.loc 1 351 10
	movl	-84(%rbp), %eax
	.loc 1 352 1
	addq	$56, %rsp
	popq	%rbx
	.cfi_restore 3
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, -40
	ret
	.cfi_endproc
.LFE8:
	.seh_endproc
	.section	.text$_ZN2ps5pcall6ServerC1EPKNS0_5pcallE,"x"
	.linkonce discard
	.align 2
	.globl	_ZN2ps5pcall6ServerC1EPKNS0_5pcallE
	.def	_ZN2ps5pcall6ServerC1EPKNS0_5pcallE;	.scl	2;	.type	32;	.endef
	.seh_proc	_ZN2ps5pcall6ServerC1EPKNS0_5pcallE
_ZN2ps5pcall6ServerC1EPKNS0_5pcallE:
.LFB60:
	.file 2 "D:/Code-Lib/mmcar/controller/Modules/pcode/include/pcall_class.h"
	.loc 2 132 5
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	%rdx, 24(%rbp)
.LBB2:
	.loc 2 132 37
	movq	16(%rbp), %rax
	movq	24(%rbp), %rdx
	movq	%rdx, (%rax)
.LBE2:
	.loc 2 132 40
	nop
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE60:
	.seh_endproc
	.section	.text$_ZN2ps5pcall6Server5parseEPKhi,"x"
	.linkonce discard
	.align 2
	.globl	_ZN2ps5pcall6Server5parseEPKhi
	.def	_ZN2ps5pcall6Server5parseEPKhi;	.scl	2;	.type	32;	.endef
	.seh_proc	_ZN2ps5pcall6Server5parseEPKhi
_ZN2ps5pcall6Server5parseEPKhi:
.LFB61:
	.loc 2 133 10
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movl	%r8d, 32(%rbp)
	.loc 2 134 21
	movq	24(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	.loc 2 135 9
	cmpl	$0, -4(%rbp)
	je	.L5
	.loc 2 137 17
	addq	$4, 24(%rbp)
	.loc 2 138 20
	movl	32(%rbp), %eax
	subl	$4, %eax
	movl	%eax, 32(%rbp)
	.loc 2 140 13
	movq	16(%rbp), %rax
	movq	(%rax), %rdx
	.loc 2 140 22
	movl	-4(%rbp), %eax
	subl	$1, %eax
	movl	%eax, %eax
	.loc 2 140 24
	salq	$3, %rax
	addq	%rdx, %rax
	.loc 2 140 26
	movq	(%rax), %rax
	.loc 2 140 29
	movl	32(%rbp), %edx
	movq	24(%rbp), %rcx
	call	*%rax
.LVL1:
	.loc 2 141 20
	movl	$1, %eax
	jmp	.L6
.L5:
	.loc 2 143 16
	movl	$0, %eax
.L6:
	.loc 2 144 5
	addq	$48, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE61:
	.seh_endproc
	.text
	.def	_ZN12_GLOBAL__N_112__pcall_moveEPKhi;	.scl	3;	.type	32;	.endef
	.seh_proc	_ZN12_GLOBAL__N_112__pcall_moveEPKhi
_ZN12_GLOBAL__N_112__pcall_moveEPKhi:
.LFB62:
	.file 3 "D:/Code-Lib/mmcar/controller/Modules/pcode/demo/table.h"
	.loc 3 24 56
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movl	%edx, 24(%rbp)
	.loc 3 24 69
	movl	24(%rbp), %eax
	cltq
	.loc 3 24 58
	cmpq	$3, %rax
	ja	.L8
	.loc 3 24 103 discriminator 1
	movl	$0, %eax
	jmp	.L10
.L8:
	.loc 3 24 128 discriminator 2
	movq	16(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	.loc 3 24 163 discriminator 2
	addq	$4, 16(%rbp)
	.loc 3 24 185 discriminator 2
	movl	-4(%rbp), %eax
	movd	%eax, %xmm0
	call	_Z4movef
	.loc 3 24 200 discriminator 2
	movl	$1, %eax
.L10:
	.loc 3 24 206 discriminator 1
	addq	$48, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE62:
	.seh_endproc
	.def	_ZN12_GLOBAL__N_118__pcall_arm_x_moveEPKhi;	.scl	3;	.type	32;	.endef
	.seh_proc	_ZN12_GLOBAL__N_118__pcall_arm_x_moveEPKhi
_ZN12_GLOBAL__N_118__pcall_arm_x_moveEPKhi:
.LFB63:
	.loc 3 25 62
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movl	%edx, 24(%rbp)
	.loc 3 25 75
	movl	24(%rbp), %eax
	cltq
	.loc 3 25 64
	cmpq	$7, %rax
	ja	.L12
	.loc 3 25 151 discriminator 1
	movl	$0, %eax
	jmp	.L14
.L12:
	.loc 3 25 176 discriminator 2
	movq	16(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	.loc 3 25 211 discriminator 2
	addq	$4, 16(%rbp)
	.loc 3 25 250 discriminator 2
	movq	16(%rbp), %rax
	movzwl	(%rax), %eax
	movw	%ax, -6(%rbp)
	.loc 3 25 288 discriminator 2
	addq	$2, 16(%rbp)
	.loc 3 25 330 discriminator 2
	movq	16(%rbp), %rax
	movzwl	(%rax), %eax
	movw	%ax, -8(%rbp)
	.loc 3 25 368 discriminator 2
	addq	$2, 16(%rbp)
	.loc 3 25 399 discriminator 2
	movzwl	-8(%rbp), %eax
	movzbl	%al, %edx
	movzwl	-6(%rbp), %eax
	movzwl	%ax, %eax
	movl	-4(%rbp), %ecx
	movl	%edx, %r8d
	movl	%eax, %edx
	movd	%ecx, %xmm0
	call	_Z10arm_x_movefth
	.loc 3 25 426 discriminator 2
	movl	$1, %eax
.L14:
	.loc 3 25 432 discriminator 1
	addq	$48, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE63:
	.seh_endproc
	.def	_ZN12_GLOBAL__N_118__pcall_arm_z_moveEPKhi;	.scl	3;	.type	32;	.endef
	.seh_proc	_ZN12_GLOBAL__N_118__pcall_arm_z_moveEPKhi
_ZN12_GLOBAL__N_118__pcall_arm_z_moveEPKhi:
.LFB64:
	.loc 3 26 62
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movl	%edx, 24(%rbp)
	.loc 3 26 75
	movl	24(%rbp), %eax
	cltq
	.loc 3 26 64
	cmpq	$6, %rax
	ja	.L16
	.loc 3 26 150 discriminator 1
	movl	$0, %eax
	jmp	.L18
.L16:
	.loc 3 26 175 discriminator 2
	movq	16(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	.loc 3 26 210 discriminator 2
	addq	$4, 16(%rbp)
	.loc 3 26 249 discriminator 2
	movq	16(%rbp), %rax
	movzwl	(%rax), %eax
	movw	%ax, -6(%rbp)
	.loc 3 26 287 discriminator 2
	addq	$2, 16(%rbp)
	.loc 3 26 328 discriminator 2
	movq	16(%rbp), %rax
	movzbl	(%rax), %eax
	movb	%al, -7(%rbp)
	.loc 3 26 365 discriminator 2
	addq	$1, 16(%rbp)
	.loc 3 26 395 discriminator 2
	movzbl	-7(%rbp), %eax
	movzbl	%al, %edx
	movzwl	-6(%rbp), %eax
	movzwl	%ax, %eax
	movl	-4(%rbp), %ecx
	movl	%edx, %r8d
	movl	%eax, %edx
	movd	%ecx, %xmm0
	call	_Z10arm_z_movefth
	.loc 3 26 422 discriminator 2
	movl	$1, %eax
.L18:
	.loc 3 26 428 discriminator 1
	addq	$48, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE64:
	.seh_endproc
	.def	_ZN12_GLOBAL__N_118__pcall_arm_r_moveEPKhi;	.scl	3;	.type	32;	.endef
	.seh_proc	_ZN12_GLOBAL__N_118__pcall_arm_r_moveEPKhi
_ZN12_GLOBAL__N_118__pcall_arm_r_moveEPKhi:
.LFB65:
	.loc 3 27 62
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movl	%edx, 24(%rbp)
	.loc 3 27 75
	movl	24(%rbp), %eax
	cltq
	.loc 3 27 64
	cmpq	$6, %rax
	ja	.L20
	.loc 3 27 150 discriminator 1
	movl	$0, %eax
	jmp	.L22
.L20:
	.loc 3 27 175 discriminator 2
	movq	16(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	.loc 3 27 210 discriminator 2
	addq	$4, 16(%rbp)
	.loc 3 27 249 discriminator 2
	movq	16(%rbp), %rax
	movzwl	(%rax), %eax
	movw	%ax, -6(%rbp)
	.loc 3 27 287 discriminator 2
	addq	$2, 16(%rbp)
	.loc 3 27 328 discriminator 2
	movq	16(%rbp), %rax
	movzbl	(%rax), %eax
	movb	%al, -7(%rbp)
	.loc 3 27 365 discriminator 2
	addq	$1, 16(%rbp)
	.loc 3 27 395 discriminator 2
	movzbl	-7(%rbp), %eax
	movzbl	%al, %edx
	movzwl	-6(%rbp), %eax
	movzwl	%ax, %eax
	movl	-4(%rbp), %ecx
	movl	%edx, %r8d
	movl	%eax, %edx
	movd	%ecx, %xmm0
	call	_Z10arm_r_movefth
	.loc 3 27 422 discriminator 2
	movl	$1, %eax
.L22:
	.loc 3 27 428 discriminator 1
	addq	$48, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE65:
	.seh_endproc
	.data
	.align 32
_ZN12_GLOBAL__N_111PCALL_TABLEE:
	.quad	_ZN12_GLOBAL__N_112__pcall_moveEPKhi
	.quad	_ZN12_GLOBAL__N_118__pcall_arm_x_moveEPKhi
	.quad	_ZN12_GLOBAL__N_118__pcall_arm_z_moveEPKhi
	.quad	_ZN12_GLOBAL__N_118__pcall_arm_r_moveEPKhi
	.text
	.def	_ZN12_GLOBAL__N_110pcall_initEv;	.scl	3;	.type	32;	.endef
	.seh_proc	_ZN12_GLOBAL__N_110pcall_initEv
_ZN12_GLOBAL__N_110pcall_initEv:
.LFB66:
	.loc 3 40 35
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	.loc 3 40 55
	leaq	-8(%rbp), %rax
	leaq	_ZN12_GLOBAL__N_111PCALL_TABLEE(%rip), %rdx
	movq	%rax, %rcx
	call	_ZN2ps5pcall6ServerC1EPKNS0_5pcallE
	.loc 3 40 74
	movq	-8(%rbp), %rax
	.loc 3 40 76
	addq	$48, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE66:
	.seh_endproc
	.section .rdata,"dr"
.LC0:
	.ascii "\345\211\215\350\277\233 dist = %f\12\0"
	.text
	.globl	_Z4movef
	.def	_Z4movef;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z4movef
_Z4movef:
.LFB67:
	.file 4 "D:/Code-Lib/mmcar/controller/Modules/pcode/demo/test_server.cpp"
	.loc 4 15 1
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movss	%xmm0, 16(%rbp)
	.loc 4 16 11
	cvtss2sd	16(%rbp), %xmm0
	movq	%xmm0, %rax
	movq	%rax, %rdx
	movq	%rdx, %xmm1
	movq	%rax, %rdx
	leaq	.LC0(%rip), %rcx
	call	_Z6printfPKcz
	.loc 4 17 1
	nop
	addq	$32, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE67:
	.seh_endproc
	.section .rdata,"dr"
	.align 8
.LC1:
	.ascii "X\347\247\273\345\212\250 dist = %f, spd = %d, acc = %d\12\0"
	.text
	.globl	_Z10arm_x_movefth
	.def	_Z10arm_x_movefth;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z10arm_x_movefth
_Z10arm_x_movefth:
.LFB68:
	.loc 4 20 1
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movss	%xmm0, 16(%rbp)
	movl	%edx, %eax
	movl	%r8d, %edx
	movw	%ax, 24(%rbp)
	movl	%edx, %eax
	movb	%al, 32(%rbp)
	.loc 4 21 11
	movzbl	32(%rbp), %edx
	movzwl	24(%rbp), %eax
	cvtss2sd	16(%rbp), %xmm0
	movq	%xmm0, %rcx
	movq	%rcx, %r8
	movq	%r8, %r10
	movl	%edx, %r9d
	movl	%eax, %r8d
	movq	%r10, %xmm1
	movq	%rcx, %rdx
	leaq	.LC1(%rip), %rcx
	call	_Z6printfPKcz
	.loc 4 22 1
	nop
	addq	$32, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE68:
	.seh_endproc
	.section .rdata,"dr"
	.align 8
.LC2:
	.ascii "Z\347\247\273\345\212\250 dist = %f, spd = %d, acc = %d\12\0"
	.text
	.globl	_Z10arm_z_movefth
	.def	_Z10arm_z_movefth;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z10arm_z_movefth
_Z10arm_z_movefth:
.LFB69:
	.loc 4 25 1
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movss	%xmm0, 16(%rbp)
	movl	%edx, %eax
	movl	%r8d, %edx
	movw	%ax, 24(%rbp)
	movl	%edx, %eax
	movb	%al, 32(%rbp)
	.loc 4 26 11
	movzbl	32(%rbp), %edx
	movzwl	24(%rbp), %eax
	cvtss2sd	16(%rbp), %xmm0
	movq	%xmm0, %rcx
	movq	%rcx, %r8
	movq	%r8, %r10
	movl	%edx, %r9d
	movl	%eax, %r8d
	movq	%r10, %xmm1
	movq	%rcx, %rdx
	leaq	.LC2(%rip), %rcx
	call	_Z6printfPKcz
	.loc 4 27 1
	nop
	addq	$32, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE69:
	.seh_endproc
	.section .rdata,"dr"
	.align 8
.LC3:
	.ascii "R\346\227\213\350\275\254 dist = %f, spd = %d, acc = %d\12\0"
	.text
	.globl	_Z10arm_r_movefth
	.def	_Z10arm_r_movefth;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z10arm_r_movefth
_Z10arm_r_movefth:
.LFB70:
	.loc 4 30 1
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movss	%xmm0, 16(%rbp)
	movl	%edx, %eax
	movl	%r8d, %edx
	movw	%ax, 24(%rbp)
	movl	%edx, %eax
	movb	%al, 32(%rbp)
	.loc 4 31 11
	movzbl	32(%rbp), %edx
	movzwl	24(%rbp), %eax
	cvtss2sd	16(%rbp), %xmm0
	movq	%xmm0, %rcx
	movq	%rcx, %r8
	movq	%r8, %r10
	movl	%edx, %r9d
	movl	%eax, %r8d
	movq	%r10, %xmm1
	movq	%rcx, %rdx
	leaq	.LC3(%rip), %rcx
	call	_Z6printfPKcz
	.loc 4 32 1
	nop
	addq	$32, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE70:
	.seh_endproc
	.globl	_Z11test_serverPKh
	.def	_Z11test_serverPKh;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z11test_serverPKh
_Z11test_serverPKh:
.LFB71:
	.loc 4 39 1
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	.loc 4 40 30
	call	_ZN12_GLOBAL__N_110pcall_initEv
	movq	%rax, -8(%rbp)
	.loc 4 41 17
	leaq	-8(%rbp), %rax
	movl	$120, %r8d
	movq	16(%rbp), %rdx
	movq	%rax, %rcx
	call	_ZN2ps5pcall6Server5parseEPKhi
	.loc 4 42 1
	nop
	addq	$48, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE71:
	.seh_endproc
.Letext0:
	.file 5 "E:/Code_Lib/compilers/mingw64/lib/gcc/x86_64-w64-mingw32/8.1.0/include/c++/cstdint"
	.file 6 "E:/Code_Lib/compilers/mingw64/lib/gcc/x86_64-w64-mingw32/8.1.0/include/c++/cstdio"
	.file 7 "E:/Code_Lib/compilers/mingw64/lib/gcc/x86_64-w64-mingw32/8.1.0/include/c++/cstring"
	.file 8 "E:/Code_Lib/compilers/mingw64/lib/gcc/x86_64-w64-mingw32/8.1.0/include/c++/x86_64-w64-mingw32/bits/c++config.h"
	.file 9 "E:/Code_Lib/compilers/mingw64/x86_64-w64-mingw32/include/crtdefs.h"
	.file 10 "E:/Code_Lib/compilers/mingw64/x86_64-w64-mingw32/include/stdint.h"
	.file 11 "E:/Code_Lib/compilers/mingw64/x86_64-w64-mingw32/include/string.h"
	.file 12 "<built-in>"
	.file 13 "D:/Code-Lib/mmcar/controller/Modules/pcode/include/pcall_config.h"
	.section	.debug_info,"dr"
.Ldebug_info0:
	.long	0x1460
	.word	0x4
	.secrel32	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.ascii "GNU C++14 8.1.0 -mtune=core2 -march=nocona -g\0"
	.byte	0x4
	.ascii "D:\\Code-Lib\\mmcar\\controller\\Modules\\pcode\\demo\\test_server.cpp\0"
	.ascii "D:\\\\Code-Lib\\\\mmcar\\\\controller\\\\build\0"
	.secrel32	.Ldebug_ranges0+0
	.quad	0
	.secrel32	.Ldebug_line0
	.uleb128 0x2
	.ascii "std\0"
	.byte	0xc
	.byte	0
	.long	0x31c
	.uleb128 0x3
	.ascii "__cxx11\0"
	.byte	0x8
	.word	0x104
	.byte	0x41
	.uleb128 0x4
	.byte	0x8
	.word	0x104
	.byte	0x41
	.long	0xbd
	.uleb128 0x5
	.byte	0x5
	.byte	0x30
	.byte	0xb
	.long	0x482
	.uleb128 0x5
	.byte	0x5
	.byte	0x31
	.byte	0xb
	.long	0x4b5
	.uleb128 0x5
	.byte	0x5
	.byte	0x32
	.byte	0xb
	.long	0x4e3
	.uleb128 0x5
	.byte	0x5
	.byte	0x33
	.byte	0xb
	.long	0x504
	.uleb128 0x5
	.byte	0x5
	.byte	0x35
	.byte	0xb
	.long	0x5d7
	.uleb128 0x5
	.byte	0x5
	.byte	0x36
	.byte	0xb
	.long	0x600
	.uleb128 0x5
	.byte	0x5
	.byte	0x37
	.byte	0xb
	.long	0x62b
	.uleb128 0x5
	.byte	0x5
	.byte	0x38
	.byte	0xb
	.long	0x656
	.uleb128 0x5
	.byte	0x5
	.byte	0x3a
	.byte	0xb
	.long	0x525
	.uleb128 0x5
	.byte	0x5
	.byte	0x3b
	.byte	0xb
	.long	0x550
	.uleb128 0x5
	.byte	0x5
	.byte	0x3c
	.byte	0xb
	.long	0x57d
	.uleb128 0x5
	.byte	0x5
	.byte	0x3d
	.byte	0xb
	.long	0x5aa
	.uleb128 0x5
	.byte	0x5
	.byte	0x3f
	.byte	0xb
	.long	0x681
	.uleb128 0x5
	.byte	0x5
	.byte	0x40
	.byte	0xb
	.long	0x3cd
	.uleb128 0x5
	.byte	0x5
	.byte	0x42
	.byte	0xb
	.long	0x4a0
	.uleb128 0x5
	.byte	0x5
	.byte	0x43
	.byte	0xb
	.long	0x4d2
	.uleb128 0x5
	.byte	0x5
	.byte	0x44
	.byte	0xb
	.long	0x4f3
	.uleb128 0x5
	.byte	0x5
	.byte	0x45
	.byte	0xb
	.long	0x514
	.uleb128 0x5
	.byte	0x5
	.byte	0x47
	.byte	0xb
	.long	0x5eb
	.uleb128 0x5
	.byte	0x5
	.byte	0x48
	.byte	0xb
	.long	0x615
	.uleb128 0x5
	.byte	0x5
	.byte	0x49
	.byte	0xb
	.long	0x640
	.uleb128 0x5
	.byte	0x5
	.byte	0x4a
	.byte	0xb
	.long	0x66b
	.uleb128 0x5
	.byte	0x5
	.byte	0x4c
	.byte	0xb
	.long	0x53a
	.uleb128 0x5
	.byte	0x5
	.byte	0x4d
	.byte	0xb
	.long	0x566
	.uleb128 0x5
	.byte	0x5
	.byte	0x4e
	.byte	0xb
	.long	0x593
	.uleb128 0x5
	.byte	0x5
	.byte	0x4f
	.byte	0xb
	.long	0x5c0
	.uleb128 0x5
	.byte	0x5
	.byte	0x51
	.byte	0xb
	.long	0x692
	.uleb128 0x5
	.byte	0x5
	.byte	0x52
	.byte	0xb
	.long	0x3de
	.uleb128 0x5
	.byte	0x6
	.byte	0x62
	.byte	0xb
	.long	0x734
	.uleb128 0x5
	.byte	0x6
	.byte	0x63
	.byte	0xb
	.long	0x741
	.uleb128 0x5
	.byte	0x6
	.byte	0x65
	.byte	0xb
	.long	0x755
	.uleb128 0x5
	.byte	0x6
	.byte	0x66
	.byte	0xb
	.long	0x773
	.uleb128 0x5
	.byte	0x6
	.byte	0x67
	.byte	0xb
	.long	0x78d
	.uleb128 0x5
	.byte	0x6
	.byte	0x68
	.byte	0xb
	.long	0x7a5
	.uleb128 0x5
	.byte	0x6
	.byte	0x69
	.byte	0xb
	.long	0x7bf
	.uleb128 0x5
	.byte	0x6
	.byte	0x6a
	.byte	0xb
	.long	0x7d9
	.uleb128 0x5
	.byte	0x6
	.byte	0x6b
	.byte	0xb
	.long	0x7f2
	.uleb128 0x5
	.byte	0x6
	.byte	0x6c
	.byte	0xb
	.long	0x818
	.uleb128 0x5
	.byte	0x6
	.byte	0x6d
	.byte	0xb
	.long	0x83b
	.uleb128 0x5
	.byte	0x6
	.byte	0x6e
	.byte	0xb
	.long	0x85f
	.uleb128 0x5
	.byte	0x6
	.byte	0x71
	.byte	0xb
	.long	0x897
	.uleb128 0x5
	.byte	0x6
	.byte	0x72
	.byte	0xb
	.long	0x8c1
	.uleb128 0x5
	.byte	0x6
	.byte	0x73
	.byte	0xb
	.long	0x8e6
	.uleb128 0x5
	.byte	0x6
	.byte	0x74
	.byte	0xb
	.long	0x91c
	.uleb128 0x5
	.byte	0x6
	.byte	0x75
	.byte	0xb
	.long	0x93f
	.uleb128 0x5
	.byte	0x6
	.byte	0x76
	.byte	0xb
	.long	0x965
	.uleb128 0x5
	.byte	0x6
	.byte	0x78
	.byte	0xb
	.long	0x97e
	.uleb128 0x5
	.byte	0x6
	.byte	0x79
	.byte	0xb
	.long	0x996
	.uleb128 0x5
	.byte	0x6
	.byte	0x7e
	.byte	0xb
	.long	0x9a7
	.uleb128 0x5
	.byte	0x6
	.byte	0x7f
	.byte	0xb
	.long	0x9bd
	.uleb128 0x5
	.byte	0x6
	.byte	0x83
	.byte	0xb
	.long	0xa44
	.uleb128 0x5
	.byte	0x6
	.byte	0x84
	.byte	0xb
	.long	0xa5e
	.uleb128 0x5
	.byte	0x6
	.byte	0x85
	.byte	0xb
	.long	0xa7d
	.uleb128 0x5
	.byte	0x6
	.byte	0x86
	.byte	0xb
	.long	0xa93
	.uleb128 0x5
	.byte	0x6
	.byte	0x87
	.byte	0xb
	.long	0xaba
	.uleb128 0x5
	.byte	0x6
	.byte	0x88
	.byte	0xb
	.long	0xad5
	.uleb128 0x5
	.byte	0x6
	.byte	0x89
	.byte	0xb
	.long	0xaff
	.uleb128 0x5
	.byte	0x6
	.byte	0x8a
	.byte	0xb
	.long	0xb31
	.uleb128 0x5
	.byte	0x6
	.byte	0x8b
	.byte	0xb
	.long	0xb62
	.uleb128 0x5
	.byte	0x6
	.byte	0x8d
	.byte	0xb
	.long	0xb73
	.uleb128 0x5
	.byte	0x6
	.byte	0x8f
	.byte	0xb
	.long	0xb8d
	.uleb128 0x5
	.byte	0x6
	.byte	0x90
	.byte	0xb
	.long	0xbac
	.uleb128 0x5
	.byte	0x6
	.byte	0x91
	.byte	0xb
	.long	0xbeb
	.uleb128 0x5
	.byte	0x6
	.byte	0x92
	.byte	0xb
	.long	0xc1b
	.uleb128 0x5
	.byte	0x6
	.byte	0xb9
	.byte	0x16
	.long	0xc54
	.uleb128 0x5
	.byte	0x6
	.byte	0xba
	.byte	0x16
	.long	0xc8e
	.uleb128 0x5
	.byte	0x6
	.byte	0xbb
	.byte	0x16
	.long	0xccb
	.uleb128 0x5
	.byte	0x6
	.byte	0xbc
	.byte	0x16
	.long	0xcf9
	.uleb128 0x5
	.byte	0x6
	.byte	0xbd
	.byte	0x16
	.long	0xd3a
	.uleb128 0x5
	.byte	0x7
	.byte	0x52
	.byte	0xb
	.long	0xd72
	.uleb128 0x5
	.byte	0x7
	.byte	0x55
	.byte	0xb
	.long	0xd91
	.uleb128 0x5
	.byte	0x7
	.byte	0x5b
	.byte	0xb
	.long	0xdac
	.uleb128 0x5
	.byte	0x7
	.byte	0x5c
	.byte	0xb
	.long	0xdca
	.byte	0
	.uleb128 0x6
	.ascii "__gnu_cxx\0"
	.byte	0x8
	.word	0x106
	.byte	0xb
	.long	0x36e
	.uleb128 0x3
	.ascii "__cxx11\0"
	.byte	0x8
	.word	0x108
	.byte	0x41
	.uleb128 0x4
	.byte	0x8
	.word	0x108
	.byte	0x41
	.long	0x32f
	.uleb128 0x5
	.byte	0x6
	.byte	0xaf
	.byte	0xb
	.long	0xc54
	.uleb128 0x5
	.byte	0x6
	.byte	0xb0
	.byte	0xb
	.long	0xc8e
	.uleb128 0x5
	.byte	0x6
	.byte	0xb1
	.byte	0xb
	.long	0xccb
	.uleb128 0x5
	.byte	0x6
	.byte	0xb2
	.byte	0xb
	.long	0xcf9
	.uleb128 0x5
	.byte	0x6
	.byte	0xb3
	.byte	0xb
	.long	0xd3a
	.byte	0
	.uleb128 0x7
	.byte	0x8
	.ascii "__builtin_va_list\0"
	.long	0x386
	.uleb128 0x8
	.byte	0x1
	.byte	0x6
	.ascii "char\0"
	.uleb128 0x9
	.long	0x386
	.uleb128 0xa
	.ascii "size_t\0"
	.byte	0x9
	.byte	0x23
	.byte	0x2a
	.long	0x3a2
	.uleb128 0x8
	.byte	0x8
	.byte	0x7
	.ascii "long long unsigned int\0"
	.uleb128 0x8
	.byte	0x8
	.byte	0x5
	.ascii "long long int\0"
	.uleb128 0xa
	.ascii "intptr_t\0"
	.byte	0x9
	.byte	0x3e
	.byte	0x21
	.long	0x3bc
	.uleb128 0xa
	.ascii "uintptr_t\0"
	.byte	0x9
	.byte	0x4b
	.byte	0x2a
	.long	0x3a2
	.uleb128 0x8
	.byte	0x2
	.byte	0x7
	.ascii "short unsigned int\0"
	.uleb128 0x8
	.byte	0x4
	.byte	0x5
	.ascii "int\0"
	.uleb128 0x8
	.byte	0x4
	.byte	0x5
	.ascii "long int\0"
	.uleb128 0xb
	.byte	0x8
	.long	0x386
	.uleb128 0x8
	.byte	0x2
	.byte	0x7
	.ascii "wchar_t\0"
	.uleb128 0x8
	.byte	0x4
	.byte	0x7
	.ascii "unsigned int\0"
	.uleb128 0x8
	.byte	0x4
	.byte	0x7
	.ascii "long unsigned int\0"
	.uleb128 0x8
	.byte	0x1
	.byte	0x8
	.ascii "unsigned char\0"
	.uleb128 0x8
	.byte	0x10
	.byte	0x4
	.ascii "long double\0"
	.uleb128 0xc
	.ascii "decltype(nullptr)\0"
	.uleb128 0xa
	.ascii "int8_t\0"
	.byte	0xa
	.byte	0x23
	.byte	0x15
	.long	0x491
	.uleb128 0x8
	.byte	0x1
	.byte	0x6
	.ascii "signed char\0"
	.uleb128 0xa
	.ascii "uint8_t\0"
	.byte	0xa
	.byte	0x24
	.byte	0x17
	.long	0x44f
	.uleb128 0x9
	.long	0x4a0
	.uleb128 0xa
	.ascii "int16_t\0"
	.byte	0xa
	.byte	0x25
	.byte	0xf
	.long	0x4c5
	.uleb128 0x8
	.byte	0x2
	.byte	0x5
	.ascii "short int\0"
	.uleb128 0xa
	.ascii "uint16_t\0"
	.byte	0xa
	.byte	0x26
	.byte	0x18
	.long	0x3f0
	.uleb128 0xa
	.ascii "int32_t\0"
	.byte	0xa
	.byte	0x27
	.byte	0xd
	.long	0x406
	.uleb128 0xa
	.ascii "uint32_t\0"
	.byte	0xa
	.byte	0x28
	.byte	0x12
	.long	0x42a
	.uleb128 0xa
	.ascii "int64_t\0"
	.byte	0xa
	.byte	0x29
	.byte	0x21
	.long	0x3bc
	.uleb128 0xa
	.ascii "uint64_t\0"
	.byte	0xa
	.byte	0x2a
	.byte	0x2a
	.long	0x3a2
	.uleb128 0xa
	.ascii "int_least8_t\0"
	.byte	0xa
	.byte	0x2d
	.byte	0x15
	.long	0x491
	.uleb128 0xa
	.ascii "uint_least8_t\0"
	.byte	0xa
	.byte	0x2e
	.byte	0x17
	.long	0x44f
	.uleb128 0xa
	.ascii "int_least16_t\0"
	.byte	0xa
	.byte	0x2f
	.byte	0xf
	.long	0x4c5
	.uleb128 0xa
	.ascii "uint_least16_t\0"
	.byte	0xa
	.byte	0x30
	.byte	0x18
	.long	0x3f0
	.uleb128 0xa
	.ascii "int_least32_t\0"
	.byte	0xa
	.byte	0x31
	.byte	0xd
	.long	0x406
	.uleb128 0xa
	.ascii "uint_least32_t\0"
	.byte	0xa
	.byte	0x32
	.byte	0x12
	.long	0x42a
	.uleb128 0xa
	.ascii "int_least64_t\0"
	.byte	0xa
	.byte	0x33
	.byte	0x21
	.long	0x3bc
	.uleb128 0xa
	.ascii "uint_least64_t\0"
	.byte	0xa
	.byte	0x34
	.byte	0x2a
	.long	0x3a2
	.uleb128 0xa
	.ascii "int_fast8_t\0"
	.byte	0xa
	.byte	0x3a
	.byte	0x15
	.long	0x491
	.uleb128 0xa
	.ascii "uint_fast8_t\0"
	.byte	0xa
	.byte	0x3b
	.byte	0x17
	.long	0x44f
	.uleb128 0xa
	.ascii "int_fast16_t\0"
	.byte	0xa
	.byte	0x3c
	.byte	0xf
	.long	0x4c5
	.uleb128 0xa
	.ascii "uint_fast16_t\0"
	.byte	0xa
	.byte	0x3d
	.byte	0x18
	.long	0x3f0
	.uleb128 0xa
	.ascii "int_fast32_t\0"
	.byte	0xa
	.byte	0x3e
	.byte	0xd
	.long	0x406
	.uleb128 0xa
	.ascii "uint_fast32_t\0"
	.byte	0xa
	.byte	0x3f
	.byte	0x16
	.long	0x42a
	.uleb128 0xa
	.ascii "int_fast64_t\0"
	.byte	0xa
	.byte	0x40
	.byte	0x21
	.long	0x3bc
	.uleb128 0xa
	.ascii "uint_fast64_t\0"
	.byte	0xa
	.byte	0x41
	.byte	0x2a
	.long	0x3a2
	.uleb128 0xa
	.ascii "intmax_t\0"
	.byte	0xa
	.byte	0x44
	.byte	0x21
	.long	0x3bc
	.uleb128 0xa
	.ascii "uintmax_t\0"
	.byte	0xa
	.byte	0x45
	.byte	0x2a
	.long	0x3a2
	.uleb128 0xd
	.ascii "_iobuf\0"
	.byte	0x30
	.byte	0x1
	.byte	0x1a
	.byte	0xa
	.long	0x734
	.uleb128 0xe
	.ascii "_ptr\0"
	.byte	0x1
	.byte	0x1b
	.byte	0xb
	.long	0x419
	.byte	0
	.uleb128 0xe
	.ascii "_cnt\0"
	.byte	0x1
	.byte	0x1c
	.byte	0x9
	.long	0x406
	.byte	0x8
	.uleb128 0xe
	.ascii "_base\0"
	.byte	0x1
	.byte	0x1d
	.byte	0xb
	.long	0x419
	.byte	0x10
	.uleb128 0xe
	.ascii "_flag\0"
	.byte	0x1
	.byte	0x1e
	.byte	0x9
	.long	0x406
	.byte	0x18
	.uleb128 0xe
	.ascii "_file\0"
	.byte	0x1
	.byte	0x1f
	.byte	0x9
	.long	0x406
	.byte	0x1c
	.uleb128 0xe
	.ascii "_charbuf\0"
	.byte	0x1
	.byte	0x20
	.byte	0x9
	.long	0x406
	.byte	0x20
	.uleb128 0xe
	.ascii "_bufsiz\0"
	.byte	0x1
	.byte	0x21
	.byte	0x9
	.long	0x406
	.byte	0x24
	.uleb128 0xe
	.ascii "_tmpfname\0"
	.byte	0x1
	.byte	0x22
	.byte	0xb
	.long	0x419
	.byte	0x28
	.byte	0
	.uleb128 0xa
	.ascii "FILE\0"
	.byte	0x1
	.byte	0x24
	.byte	0x19
	.long	0x6a4
	.uleb128 0xa
	.ascii "fpos_t\0"
	.byte	0x1
	.byte	0x68
	.byte	0x23
	.long	0x3bc
	.uleb128 0x9
	.long	0x741
	.uleb128 0xf
	.ascii "clearerr\0"
	.byte	0x1
	.word	0x242
	.byte	0x23
	.long	0x76d
	.uleb128 0x10
	.long	0x76d
	.byte	0
	.uleb128 0xb
	.byte	0x8
	.long	0x734
	.uleb128 0x11
	.ascii "fclose\0"
	.byte	0x1
	.word	0x243
	.byte	0x22
	.long	0x406
	.long	0x78d
	.uleb128 0x10
	.long	0x76d
	.byte	0
	.uleb128 0x11
	.ascii "feof\0"
	.byte	0x1
	.word	0x24a
	.byte	0x22
	.long	0x406
	.long	0x7a5
	.uleb128 0x10
	.long	0x76d
	.byte	0
	.uleb128 0x11
	.ascii "ferror\0"
	.byte	0x1
	.word	0x24b
	.byte	0x22
	.long	0x406
	.long	0x7bf
	.uleb128 0x10
	.long	0x76d
	.byte	0
	.uleb128 0x11
	.ascii "fflush\0"
	.byte	0x1
	.word	0x24c
	.byte	0x22
	.long	0x406
	.long	0x7d9
	.uleb128 0x10
	.long	0x76d
	.byte	0
	.uleb128 0x11
	.ascii "fgetc\0"
	.byte	0x1
	.word	0x24d
	.byte	0x22
	.long	0x406
	.long	0x7f2
	.uleb128 0x10
	.long	0x76d
	.byte	0
	.uleb128 0x11
	.ascii "fgetpos\0"
	.byte	0x1
	.word	0x24f
	.byte	0x22
	.long	0x406
	.long	0x812
	.uleb128 0x10
	.long	0x76d
	.uleb128 0x10
	.long	0x812
	.byte	0
	.uleb128 0xb
	.byte	0x8
	.long	0x741
	.uleb128 0x11
	.ascii "fgets\0"
	.byte	0x1
	.word	0x251
	.byte	0x24
	.long	0x419
	.long	0x83b
	.uleb128 0x10
	.long	0x419
	.uleb128 0x10
	.long	0x406
	.uleb128 0x10
	.long	0x76d
	.byte	0
	.uleb128 0x11
	.ascii "fopen\0"
	.byte	0x1
	.word	0x258
	.byte	0x24
	.long	0x76d
	.long	0x859
	.uleb128 0x10
	.long	0x859
	.uleb128 0x10
	.long	0x859
	.byte	0
	.uleb128 0xb
	.byte	0x8
	.long	0x38e
	.uleb128 0x12
	.ascii "fprintf\0"
	.byte	0x1
	.word	0x14e
	.byte	0x5
	.ascii "_Z7fprintfP6_iobufPKcz\0"
	.long	0x406
	.long	0x897
	.uleb128 0x10
	.long	0x76d
	.uleb128 0x10
	.long	0x859
	.uleb128 0x13
	.byte	0
	.uleb128 0x11
	.ascii "fread\0"
	.byte	0x1
	.word	0x25d
	.byte	0x25
	.long	0x393
	.long	0x8bf
	.uleb128 0x10
	.long	0x8bf
	.uleb128 0x10
	.long	0x393
	.uleb128 0x10
	.long	0x393
	.uleb128 0x10
	.long	0x76d
	.byte	0
	.uleb128 0x14
	.byte	0x8
	.uleb128 0x11
	.ascii "freopen\0"
	.byte	0x1
	.word	0x25e
	.byte	0x24
	.long	0x76d
	.long	0x8e6
	.uleb128 0x10
	.long	0x859
	.uleb128 0x10
	.long	0x859
	.uleb128 0x10
	.long	0x76d
	.byte	0
	.uleb128 0x12
	.ascii "fscanf\0"
	.byte	0x1
	.word	0x121
	.byte	0x5
	.ascii "_Z6fscanfP6_iobufPKcz\0"
	.long	0x406
	.long	0x91c
	.uleb128 0x10
	.long	0x76d
	.uleb128 0x10
	.long	0x859
	.uleb128 0x13
	.byte	0
	.uleb128 0x11
	.ascii "fseek\0"
	.byte	0x1
	.word	0x261
	.byte	0x22
	.long	0x406
	.long	0x93f
	.uleb128 0x10
	.long	0x76d
	.uleb128 0x10
	.long	0x40d
	.uleb128 0x10
	.long	0x406
	.byte	0
	.uleb128 0x11
	.ascii "fsetpos\0"
	.byte	0x1
	.word	0x25f
	.byte	0x22
	.long	0x406
	.long	0x95f
	.uleb128 0x10
	.long	0x76d
	.uleb128 0x10
	.long	0x95f
	.byte	0
	.uleb128 0xb
	.byte	0x8
	.long	0x750
	.uleb128 0x11
	.ascii "ftell\0"
	.byte	0x1
	.word	0x262
	.byte	0x23
	.long	0x40d
	.long	0x97e
	.uleb128 0x10
	.long	0x76d
	.byte	0
	.uleb128 0x11
	.ascii "getc\0"
	.byte	0x1
	.word	0x28f
	.byte	0x22
	.long	0x406
	.long	0x996
	.uleb128 0x10
	.long	0x76d
	.byte	0
	.uleb128 0x15
	.ascii "getchar\0"
	.byte	0x1
	.word	0x290
	.byte	0x22
	.long	0x406
	.uleb128 0xf
	.ascii "perror\0"
	.byte	0x1
	.word	0x296
	.byte	0x23
	.long	0x9bd
	.uleb128 0x10
	.long	0x859
	.byte	0
	.uleb128 0x16
	.ascii "printf\0"
	.byte	0x1
	.word	0x159
	.byte	0x5
	.ascii "_Z6printfPKcz\0"
	.long	0x406
	.quad	.LFB8
	.quad	.LFE8-.LFB8
	.uleb128 0x1
	.byte	0x9c
	.long	0xa44
	.uleb128 0x13
	.uleb128 0x17
	.ascii "__format\0"
	.byte	0x1
	.word	0x159
	.byte	0x19
	.long	0x859
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x13
	.uleb128 0x18
	.ascii "__retval\0"
	.byte	0x1
	.word	0x15b
	.byte	0x7
	.long	0x406
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x18
	.ascii "__local_argv\0"
	.byte	0x1
	.word	0x15c
	.byte	0x15
	.long	0x36e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x19
	.quad	.LVL0
	.long	0x1457
	.byte	0
	.uleb128 0x11
	.ascii "remove\0"
	.byte	0x1
	.word	0x2a4
	.byte	0x22
	.long	0x406
	.long	0xa5e
	.uleb128 0x10
	.long	0x859
	.byte	0
	.uleb128 0x11
	.ascii "rename\0"
	.byte	0x1
	.word	0x2a5
	.byte	0x22
	.long	0x406
	.long	0xa7d
	.uleb128 0x10
	.long	0x859
	.uleb128 0x10
	.long	0x859
	.byte	0
	.uleb128 0xf
	.ascii "rewind\0"
	.byte	0x1
	.word	0x2ab
	.byte	0x23
	.long	0xa93
	.uleb128 0x10
	.long	0x76d
	.byte	0
	.uleb128 0x12
	.ascii "scanf\0"
	.byte	0x1
	.word	0x116
	.byte	0x5
	.ascii "_Z5scanfPKcz\0"
	.long	0x406
	.long	0xaba
	.uleb128 0x10
	.long	0x859
	.uleb128 0x13
	.byte	0
	.uleb128 0xf
	.ascii "setbuf\0"
	.byte	0x1
	.word	0x2ad
	.byte	0x23
	.long	0xad5
	.uleb128 0x10
	.long	0x76d
	.uleb128 0x10
	.long	0x419
	.byte	0
	.uleb128 0x11
	.ascii "setvbuf\0"
	.byte	0x1
	.word	0x2b1
	.byte	0x22
	.long	0x406
	.long	0xaff
	.uleb128 0x10
	.long	0x76d
	.uleb128 0x10
	.long	0x419
	.uleb128 0x10
	.long	0x406
	.uleb128 0x10
	.long	0x393
	.byte	0
	.uleb128 0x12
	.ascii "sprintf\0"
	.byte	0x1
	.word	0x164
	.byte	0x5
	.ascii "_Z7sprintfPcPKcz\0"
	.long	0x406
	.long	0xb31
	.uleb128 0x10
	.long	0x419
	.uleb128 0x10
	.long	0x859
	.uleb128 0x13
	.byte	0
	.uleb128 0x12
	.ascii "sscanf\0"
	.byte	0x1
	.word	0x10b
	.byte	0x5
	.ascii "_Z6sscanfPKcS0_z\0"
	.long	0x406
	.long	0xb62
	.uleb128 0x10
	.long	0x859
	.uleb128 0x10
	.long	0x859
	.uleb128 0x13
	.byte	0
	.uleb128 0x15
	.ascii "tmpfile\0"
	.byte	0x1
	.word	0x2cb
	.byte	0x24
	.long	0x76d
	.uleb128 0x11
	.ascii "tmpnam\0"
	.byte	0x1
	.word	0x2cc
	.byte	0x24
	.long	0x419
	.long	0xb8d
	.uleb128 0x10
	.long	0x419
	.byte	0
	.uleb128 0x11
	.ascii "ungetc\0"
	.byte	0x1
	.word	0x2cd
	.byte	0x22
	.long	0x406
	.long	0xbac
	.uleb128 0x10
	.long	0x406
	.uleb128 0x10
	.long	0x76d
	.byte	0
	.uleb128 0x12
	.ascii "vfprintf\0"
	.byte	0x1
	.word	0x16f
	.byte	0x5
	.ascii "_Z8vfprintfP6_iobufPKcPc\0"
	.long	0x406
	.long	0xbeb
	.uleb128 0x10
	.long	0x76d
	.uleb128 0x10
	.long	0x859
	.uleb128 0x10
	.long	0x36e
	.byte	0
	.uleb128 0x12
	.ascii "vprintf\0"
	.byte	0x1
	.word	0x176
	.byte	0x5
	.ascii "_Z7vprintfPKcPc\0"
	.long	0x406
	.long	0xc1b
	.uleb128 0x10
	.long	0x859
	.uleb128 0x10
	.long	0x36e
	.byte	0
	.uleb128 0x12
	.ascii "vsprintf\0"
	.byte	0x1
	.word	0x17d
	.byte	0x5
	.ascii "_Z8vsprintfPcPKcS_\0"
	.long	0x406
	.long	0xc54
	.uleb128 0x10
	.long	0x419
	.uleb128 0x10
	.long	0x859
	.uleb128 0x10
	.long	0x36e
	.byte	0
	.uleb128 0x12
	.ascii "snprintf\0"
	.byte	0x1
	.word	0x184
	.byte	0x5
	.ascii "_Z8snprintfPcyPKcz\0"
	.long	0x406
	.long	0xc8e
	.uleb128 0x10
	.long	0x419
	.uleb128 0x10
	.long	0x393
	.uleb128 0x10
	.long	0x859
	.uleb128 0x13
	.byte	0
	.uleb128 0x12
	.ascii "vfscanf\0"
	.byte	0x1
	.word	0x140
	.byte	0x5
	.ascii "_Z7vfscanfP6_iobufPKcPc\0"
	.long	0x406
	.long	0xccb
	.uleb128 0x10
	.long	0x76d
	.uleb128 0x10
	.long	0x859
	.uleb128 0x10
	.long	0x36e
	.byte	0
	.uleb128 0x12
	.ascii "vscanf\0"
	.byte	0x1
	.word	0x139
	.byte	0x5
	.ascii "_Z6vscanfPKcPc\0"
	.long	0x406
	.long	0xcf9
	.uleb128 0x10
	.long	0x859
	.uleb128 0x10
	.long	0x36e
	.byte	0
	.uleb128 0x12
	.ascii "vsnprintf\0"
	.byte	0x1
	.word	0x18f
	.byte	0x5
	.ascii "_Z9vsnprintfPcyPKcS_\0"
	.long	0x406
	.long	0xd3a
	.uleb128 0x10
	.long	0x419
	.uleb128 0x10
	.long	0x393
	.uleb128 0x10
	.long	0x859
	.uleb128 0x10
	.long	0x36e
	.byte	0
	.uleb128 0x12
	.ascii "vsscanf\0"
	.byte	0x1
	.word	0x132
	.byte	0x5
	.ascii "_Z7vsscanfPKcS0_Pc\0"
	.long	0x406
	.long	0xd72
	.uleb128 0x10
	.long	0x859
	.uleb128 0x10
	.long	0x859
	.uleb128 0x10
	.long	0x36e
	.byte	0
	.uleb128 0x1a
	.ascii "strcoll\0"
	.byte	0xb
	.byte	0x48
	.byte	0x22
	.long	0x406
	.long	0xd91
	.uleb128 0x10
	.long	0x859
	.uleb128 0x10
	.long	0x859
	.byte	0
	.uleb128 0x1a
	.ascii "strerror\0"
	.byte	0xb
	.byte	0x52
	.byte	0x24
	.long	0x419
	.long	0xdac
	.uleb128 0x10
	.long	0x406
	.byte	0
	.uleb128 0x1a
	.ascii "strtok\0"
	.byte	0xb
	.byte	0x61
	.byte	0x24
	.long	0x419
	.long	0xdca
	.uleb128 0x10
	.long	0x419
	.uleb128 0x10
	.long	0x859
	.byte	0
	.uleb128 0x1a
	.ascii "strxfrm\0"
	.byte	0xb
	.byte	0x68
	.byte	0x25
	.long	0x393
	.long	0xdee
	.uleb128 0x10
	.long	0x419
	.uleb128 0x10
	.long	0x859
	.uleb128 0x10
	.long	0x393
	.byte	0
	.uleb128 0x1b
	.ascii "ps\0"
	.byte	0x2
	.byte	0x11
	.byte	0xb
	.long	0xee1
	.uleb128 0x1c
	.ascii "pcall\0"
	.byte	0x2
	.byte	0x13
	.byte	0xb
	.uleb128 0xd
	.ascii "pcall\0"
	.byte	0x8
	.byte	0x2
	.byte	0x1b
	.byte	0x8
	.long	0xe20
	.uleb128 0xe
	.ascii "dec\0"
	.byte	0x2
	.byte	0x24
	.byte	0x34
	.long	0xf03
	.byte	0
	.byte	0
	.uleb128 0x9
	.long	0xe03
	.uleb128 0x1d
	.ascii "Server\0"
	.byte	0x8
	.byte	0x2
	.byte	0x81
	.byte	0x7
	.long	0xecb
	.uleb128 0x1e
	.ascii "Server\0"
	.byte	0x2
	.byte	0x84
	.byte	0x5
	.ascii "_ZN2ps5pcall6ServerC4EPKNS0_5pcallE\0"
	.byte	0x1
	.long	0xe6d
	.long	0xe78
	.uleb128 0x1f
	.long	0xf09
	.uleb128 0x10
	.long	0xf14
	.byte	0
	.uleb128 0x20
	.ascii "parse\0"
	.byte	0x2
	.byte	0x85
	.byte	0xa
	.ascii "_ZN2ps5pcall6Server5parseEPKhi\0"
	.long	0xef5
	.byte	0x1
	.long	0xeae
	.long	0xebe
	.uleb128 0x1f
	.long	0xf09
	.uleb128 0x10
	.long	0xefd
	.uleb128 0x10
	.long	0x406
	.byte	0
	.uleb128 0xe
	.ascii "pc\0"
	.byte	0x2
	.byte	0x92
	.byte	0x12
	.long	0xf14
	.byte	0
	.byte	0
	.uleb128 0xa
	.ascii "pcall_num_t\0"
	.byte	0xd
	.byte	0xd
	.byte	0x1d
	.long	0x4f3
	.byte	0
	.byte	0
	.uleb128 0x21
	.long	0xef5
	.long	0xef5
	.uleb128 0x10
	.long	0xefd
	.uleb128 0x10
	.long	0x406
	.byte	0
	.uleb128 0x8
	.byte	0x1
	.byte	0x2
	.ascii "bool\0"
	.uleb128 0xb
	.byte	0x8
	.long	0x4b0
	.uleb128 0xb
	.byte	0x8
	.long	0xee1
	.uleb128 0xb
	.byte	0x8
	.long	0xe25
	.uleb128 0x9
	.long	0xf09
	.uleb128 0xb
	.byte	0x8
	.long	0xe20
	.uleb128 0x22
	.long	0xfe5
	.uleb128 0x23
	.ascii "PCALL_TABLE\0"
	.byte	0x3
	.byte	0x23
	.byte	0x1e
	.long	0xfed
	.uleb128 0x24
	.ascii "pcall_init\0"
	.byte	0x3
	.byte	0x28
	.byte	0x16
	.long	0xe25
	.uleb128 0x25
	.ascii "__pcall_arm_r_move\0"
	.byte	0x3
	.byte	0x1b
	.byte	0xa
	.long	0xef5
	.long	0xf70
	.uleb128 0x10
	.long	0xefd
	.uleb128 0x10
	.long	0x406
	.byte	0
	.uleb128 0x25
	.ascii "__pcall_arm_z_move\0"
	.byte	0x3
	.byte	0x1a
	.byte	0xa
	.long	0xef5
	.long	0xf9a
	.uleb128 0x10
	.long	0xefd
	.uleb128 0x10
	.long	0x406
	.byte	0
	.uleb128 0x25
	.ascii "__pcall_arm_x_move\0"
	.byte	0x3
	.byte	0x19
	.byte	0xa
	.long	0xef5
	.long	0xfc4
	.uleb128 0x10
	.long	0xefd
	.uleb128 0x10
	.long	0x406
	.byte	0
	.uleb128 0x26
	.ascii "__pcall_move\0"
	.byte	0x3
	.byte	0x18
	.byte	0xa
	.long	0xef5
	.uleb128 0x10
	.long	0xefd
	.uleb128 0x10
	.long	0x406
	.byte	0
	.byte	0
	.uleb128 0x27
	.byte	0x3
	.byte	0x17
	.byte	0x1
	.long	0xf1a
	.uleb128 0x28
	.long	0xe03
	.long	0xffd
	.uleb128 0x29
	.long	0x3a2
	.byte	0x3
	.byte	0
	.uleb128 0x2a
	.long	0xf1f
	.uleb128 0x9
	.byte	0x3
	.quad	_ZN12_GLOBAL__N_111PCALL_TABLEE
	.uleb128 0x2b
	.ascii "test_server\0"
	.byte	0x4
	.byte	0x26
	.byte	0x6
	.ascii "_Z11test_serverPKh\0"
	.quad	.LFB71
	.quad	.LFE71-.LFB71
	.uleb128 0x1
	.byte	0x9c
	.long	0x1068
	.uleb128 0x2c
	.ascii "data\0"
	.byte	0x4
	.byte	0x26
	.byte	0x21
	.long	0xefd
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x2d
	.ascii "server\0"
	.byte	0x4
	.byte	0x28
	.byte	0xa
	.long	0xe25
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x2b
	.ascii "arm_r_move\0"
	.byte	0x4
	.byte	0x1d
	.byte	0x6
	.ascii "_Z10arm_r_movefth\0"
	.quad	.LFB70
	.quad	.LFE70-.LFB70
	.uleb128 0x1
	.byte	0x9c
	.long	0x10ce
	.uleb128 0x2c
	.ascii "dist\0"
	.byte	0x4
	.byte	0x1d
	.byte	0x17
	.long	0x10ce
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x2c
	.ascii "spd\0"
	.byte	0x4
	.byte	0x1d
	.byte	0x26
	.long	0x4d2
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0x2c
	.ascii "acc\0"
	.byte	0x4
	.byte	0x1d
	.byte	0x33
	.long	0x4a0
	.uleb128 0x2
	.byte	0x91
	.sleb128 16
	.byte	0
	.uleb128 0x8
	.byte	0x4
	.byte	0x4
	.ascii "float\0"
	.uleb128 0x2b
	.ascii "arm_z_move\0"
	.byte	0x4
	.byte	0x18
	.byte	0x6
	.ascii "_Z10arm_z_movefth\0"
	.quad	.LFB69
	.quad	.LFE69-.LFB69
	.uleb128 0x1
	.byte	0x9c
	.long	0x113d
	.uleb128 0x2c
	.ascii "dist\0"
	.byte	0x4
	.byte	0x18
	.byte	0x17
	.long	0x10ce
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x2c
	.ascii "spd\0"
	.byte	0x4
	.byte	0x18
	.byte	0x26
	.long	0x4d2
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0x2c
	.ascii "acc\0"
	.byte	0x4
	.byte	0x18
	.byte	0x33
	.long	0x4a0
	.uleb128 0x2
	.byte	0x91
	.sleb128 16
	.byte	0
	.uleb128 0x2b
	.ascii "arm_x_move\0"
	.byte	0x4
	.byte	0x13
	.byte	0x6
	.ascii "_Z10arm_x_movefth\0"
	.quad	.LFB68
	.quad	.LFE68-.LFB68
	.uleb128 0x1
	.byte	0x9c
	.long	0x11a3
	.uleb128 0x2c
	.ascii "dist\0"
	.byte	0x4
	.byte	0x13
	.byte	0x17
	.long	0x10ce
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x2c
	.ascii "spd\0"
	.byte	0x4
	.byte	0x13
	.byte	0x26
	.long	0x4d2
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0x2c
	.ascii "acc\0"
	.byte	0x4
	.byte	0x13
	.byte	0x33
	.long	0x4a0
	.uleb128 0x2
	.byte	0x91
	.sleb128 16
	.byte	0
	.uleb128 0x2b
	.ascii "move\0"
	.byte	0x4
	.byte	0xe
	.byte	0x6
	.ascii "_Z4movef\0"
	.quad	.LFB67
	.quad	.LFE67-.LFB67
	.uleb128 0x1
	.byte	0x9c
	.long	0x11dc
	.uleb128 0x2c
	.ascii "dist\0"
	.byte	0x4
	.byte	0xe
	.byte	0x11
	.long	0x10ce
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.byte	0
	.uleb128 0x2e
	.long	0xf33
	.quad	.LFB66
	.quad	.LFE66-.LFB66
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x2f
	.long	0xf46
	.quad	.LFB65
	.quad	.LFE65-.LFB65
	.uleb128 0x1
	.byte	0x9c
	.long	0x125f
	.uleb128 0x2c
	.ascii "data\0"
	.byte	0x3
	.byte	0x1b
	.byte	0x2c
	.long	0xefd
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x30
	.secrel32	.LASF0
	.byte	0x3
	.byte	0x1b
	.byte	0x36
	.long	0x406
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0x2d
	.ascii "tmp3\0"
	.byte	0x3
	.byte	0x1b
	.byte	0xa3
	.long	0x10ce
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x2d
	.ascii "tmp2\0"
	.byte	0x3
	.byte	0x1b
	.byte	0xed
	.long	0x4d2
	.uleb128 0x2
	.byte	0x91
	.sleb128 -22
	.uleb128 0x31
	.ascii "tmp1\0"
	.byte	0x3
	.byte	0x1b
	.word	0x13c
	.long	0x4a0
	.uleb128 0x2
	.byte	0x91
	.sleb128 -23
	.byte	0
	.uleb128 0x2f
	.long	0xf70
	.quad	.LFB64
	.quad	.LFE64-.LFB64
	.uleb128 0x1
	.byte	0x9c
	.long	0x12cb
	.uleb128 0x2c
	.ascii "data\0"
	.byte	0x3
	.byte	0x1a
	.byte	0x2c
	.long	0xefd
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x30
	.secrel32	.LASF0
	.byte	0x3
	.byte	0x1a
	.byte	0x36
	.long	0x406
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0x2d
	.ascii "tmp3\0"
	.byte	0x3
	.byte	0x1a
	.byte	0xa3
	.long	0x10ce
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x2d
	.ascii "tmp2\0"
	.byte	0x3
	.byte	0x1a
	.byte	0xed
	.long	0x4d2
	.uleb128 0x2
	.byte	0x91
	.sleb128 -22
	.uleb128 0x31
	.ascii "tmp1\0"
	.byte	0x3
	.byte	0x1a
	.word	0x13c
	.long	0x4a0
	.uleb128 0x2
	.byte	0x91
	.sleb128 -23
	.byte	0
	.uleb128 0x2f
	.long	0xf9a
	.quad	.LFB63
	.quad	.LFE63-.LFB63
	.uleb128 0x1
	.byte	0x9c
	.long	0x1337
	.uleb128 0x2c
	.ascii "data\0"
	.byte	0x3
	.byte	0x19
	.byte	0x2c
	.long	0xefd
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x30
	.secrel32	.LASF0
	.byte	0x3
	.byte	0x19
	.byte	0x36
	.long	0x406
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0x2d
	.ascii "tmp3\0"
	.byte	0x3
	.byte	0x19
	.byte	0xa4
	.long	0x10ce
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x2d
	.ascii "tmp2\0"
	.byte	0x3
	.byte	0x19
	.byte	0xee
	.long	0x4d2
	.uleb128 0x2
	.byte	0x91
	.sleb128 -22
	.uleb128 0x31
	.ascii "tmp1\0"
	.byte	0x3
	.byte	0x19
	.word	0x13e
	.long	0x4d2
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x2f
	.long	0xfc4
	.quad	.LFB62
	.quad	.LFE62-.LFB62
	.uleb128 0x1
	.byte	0x9c
	.long	0x1382
	.uleb128 0x2c
	.ascii "data\0"
	.byte	0x3
	.byte	0x18
	.byte	0x26
	.long	0xefd
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x30
	.secrel32	.LASF0
	.byte	0x3
	.byte	0x18
	.byte	0x30
	.long	0x406
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0x2d
	.ascii "tmp1\0"
	.byte	0x3
	.byte	0x18
	.byte	0x74
	.long	0x10ce
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x32
	.long	0xe78
	.long	0x13a1
	.quad	.LFB61
	.quad	.LFE61-.LFB61
	.uleb128 0x1
	.byte	0x9c
	.long	0x13df
	.uleb128 0x33
	.ascii "this\0"
	.long	0xf0f
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x2c
	.ascii "dst\0"
	.byte	0x2
	.byte	0x85
	.byte	0x1f
	.long	0xefd
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0x30
	.secrel32	.LASF0
	.byte	0x2
	.byte	0x85
	.byte	0x28
	.long	0x406
	.uleb128 0x2
	.byte	0x91
	.sleb128 16
	.uleb128 0x2d
	.ascii "number\0"
	.byte	0x2
	.byte	0x86
	.byte	0x15
	.long	0xecb
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x34
	.long	0xe35
	.long	0x13ed
	.byte	0x2
	.long	0x1403
	.uleb128 0x35
	.ascii "this\0"
	.long	0xf0f
	.uleb128 0x36
	.ascii "pc\0"
	.byte	0x2
	.byte	0x84
	.byte	0x19
	.long	0xf14
	.byte	0
	.uleb128 0x37
	.long	0x13df
	.ascii "_ZN2ps5pcall6ServerC1EPKNS0_5pcallE\0"
	.long	0x1446
	.quad	.LFB60
	.quad	.LFE60-.LFB60
	.uleb128 0x1
	.byte	0x9c
	.long	0x1457
	.uleb128 0x38
	.long	0x13ed
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x38
	.long	0x13f7
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.byte	0
	.uleb128 0x39
	.secrel32	.LASF1
	.secrel32	.LASF1
	.byte	0x1
	.byte	0x52
	.byte	0x42
	.byte	0
	.section	.debug_abbrev,"dr"
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0x8
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x1b
	.uleb128 0x8
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x39
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x39
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x89
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x3a
	.byte	0
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x18
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x8
	.byte	0
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x18
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x39
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x3b
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x6e
	.uleb128 0x8
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x6e
	.uleb128 0x8
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x4109
	.byte	0
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x39
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0x39
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x2
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x6e
	.uleb128 0x8
	.uleb128 0x32
	.uleb128 0xb
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1f
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x20
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x6e
	.uleb128 0x8
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x32
	.uleb128 0xb
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x21
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x22
	.uleb128 0x39
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x23
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x24
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x25
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x26
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x27
	.uleb128 0x3a
	.byte	0
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x18
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x28
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x29
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x2a
	.uleb128 0x34
	.byte	0
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x2b
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x6e
	.uleb128 0x8
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2c
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x2d
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x2e
	.uleb128 0x2e
	.byte	0
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x2f
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x30
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x31
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x32
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x33
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0x19
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x34
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x35
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x36
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x37
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x6e
	.uleb128 0x8
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x39
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x6e
	.uleb128 0xe
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"dr"
	.long	0x5c
	.word	0x2
	.secrel32	.Ldebug_info0
	.byte	0x8
	.byte	0
	.word	0
	.word	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	.LFB8
	.quad	.LFE8-.LFB8
	.quad	.LFB60
	.quad	.LFE60-.LFB60
	.quad	.LFB61
	.quad	.LFE61-.LFB61
	.quad	0
	.quad	0
	.section	.debug_ranges,"dr"
.Ldebug_ranges0:
	.quad	.Ltext0
	.quad	.Letext0
	.quad	.LFB8
	.quad	.LFE8
	.quad	.LFB60
	.quad	.LFE60
	.quad	.LFB61
	.quad	.LFE61
	.quad	0
	.quad	0
	.section	.debug_line,"dr"
.Ldebug_line0:
	.section	.debug_str,"dr"
.LASF1:
	.ascii "__acrt_iob_func\0"
.LASF0:
	.ascii "length\0"
	.ident	"GCC: (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0"
	.def	__mingw_vfprintf;	.scl	2;	.type	32;	.endef
