	.file	"test.cpp"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.def	__main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
.LC0:
	.ascii "m 1.5 100 10\0"
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
.LFB1:
	.file 1 "D:/Code-Lib/mmcar/controller/Modules/pcode/demo/test.cpp"
	.loc 1 8 1
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rdi
	.seh_pushreg	%rdi
	.cfi_def_cfa_offset 24
	.cfi_offset 5, -24
	subq	$168, %rsp
	.seh_stackalloc	168
	.cfi_def_cfa_offset 192
	leaq	128(%rsp), %rbp
	.seh_setframe	%rbp, 128
	.cfi_def_cfa 6, 64
	.seh_endprologue
	.loc 1 8 1
	call	__main
.LVL0:
	.loc 1 9 13
	leaq	-96(%rbp), %rdx
	movl	$0, %eax
	movl	$16, %ecx
	movq	%rdx, %rdi
	rep stosq
	.loc 1 11 16
	leaq	-96(%rbp), %rax
	movq	%rax, %rdx
	leaq	.LC0(%rip), %rcx
	call	_Z11test_clientPKcPh
	.loc 1 14 16
	leaq	-96(%rbp), %rax
	movq	%rax, %rcx
	call	_Z11test_serverPKh
	.loc 1 15 1
	movl	$0, %eax
	addq	$168, %rsp
	popq	%rdi
	.cfi_restore 5
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, -152
	ret
	.cfi_endproc
.LFE1:
	.seh_endproc
.Letext0:
	.file 2 "E:/Code_Lib/compilers/mingw64/lib/gcc/x86_64-w64-mingw32/8.1.0/include/c++/cstdint"
	.file 3 "E:/Code_Lib/compilers/mingw64/lib/gcc/x86_64-w64-mingw32/8.1.0/include/c++/x86_64-w64-mingw32/bits/c++config.h"
	.file 4 "E:/Code_Lib/compilers/mingw64/x86_64-w64-mingw32/include/crtdefs.h"
	.file 5 "E:/Code_Lib/compilers/mingw64/x86_64-w64-mingw32/include/stdint.h"
	.file 6 "<built-in>"
	.section	.debug_info,"dr"
.Ldebug_info0:
	.long	0x51a
	.word	0x4
	.secrel32	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.ascii "GNU C++14 8.1.0 -mtune=core2 -march=nocona -g\0"
	.byte	0x4
	.ascii "D:\\Code-Lib\\mmcar\\controller\\Modules\\pcode\\demo\\test.cpp\0"
	.ascii "D:\\\\Code-Lib\\\\mmcar\\\\controller\\\\build\0"
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.secrel32	.Ldebug_line0
	.uleb128 0x2
	.ascii "std\0"
	.byte	0x6
	.byte	0
	.long	0x1b1
	.uleb128 0x3
	.ascii "__cxx11\0"
	.byte	0x3
	.word	0x104
	.byte	0x41
	.uleb128 0x4
	.byte	0x3
	.word	0x104
	.byte	0x41
	.long	0xba
	.uleb128 0x5
	.byte	0x2
	.byte	0x30
	.byte	0xb
	.long	0x2bd
	.uleb128 0x5
	.byte	0x2
	.byte	0x31
	.byte	0xb
	.long	0x2eb
	.uleb128 0x5
	.byte	0x2
	.byte	0x32
	.byte	0xb
	.long	0x319
	.uleb128 0x5
	.byte	0x2
	.byte	0x33
	.byte	0xb
	.long	0x33a
	.uleb128 0x5
	.byte	0x2
	.byte	0x35
	.byte	0xb
	.long	0x40d
	.uleb128 0x5
	.byte	0x2
	.byte	0x36
	.byte	0xb
	.long	0x436
	.uleb128 0x5
	.byte	0x2
	.byte	0x37
	.byte	0xb
	.long	0x461
	.uleb128 0x5
	.byte	0x2
	.byte	0x38
	.byte	0xb
	.long	0x48c
	.uleb128 0x5
	.byte	0x2
	.byte	0x3a
	.byte	0xb
	.long	0x35b
	.uleb128 0x5
	.byte	0x2
	.byte	0x3b
	.byte	0xb
	.long	0x386
	.uleb128 0x5
	.byte	0x2
	.byte	0x3c
	.byte	0xb
	.long	0x3b3
	.uleb128 0x5
	.byte	0x2
	.byte	0x3d
	.byte	0xb
	.long	0x3e0
	.uleb128 0x5
	.byte	0x2
	.byte	0x3f
	.byte	0xb
	.long	0x4b7
	.uleb128 0x5
	.byte	0x2
	.byte	0x40
	.byte	0xb
	.long	0x20e
	.uleb128 0x5
	.byte	0x2
	.byte	0x42
	.byte	0xb
	.long	0x2db
	.uleb128 0x5
	.byte	0x2
	.byte	0x43
	.byte	0xb
	.long	0x308
	.uleb128 0x5
	.byte	0x2
	.byte	0x44
	.byte	0xb
	.long	0x329
	.uleb128 0x5
	.byte	0x2
	.byte	0x45
	.byte	0xb
	.long	0x34a
	.uleb128 0x5
	.byte	0x2
	.byte	0x47
	.byte	0xb
	.long	0x421
	.uleb128 0x5
	.byte	0x2
	.byte	0x48
	.byte	0xb
	.long	0x44b
	.uleb128 0x5
	.byte	0x2
	.byte	0x49
	.byte	0xb
	.long	0x476
	.uleb128 0x5
	.byte	0x2
	.byte	0x4a
	.byte	0xb
	.long	0x4a1
	.uleb128 0x5
	.byte	0x2
	.byte	0x4c
	.byte	0xb
	.long	0x370
	.uleb128 0x5
	.byte	0x2
	.byte	0x4d
	.byte	0xb
	.long	0x39c
	.uleb128 0x5
	.byte	0x2
	.byte	0x4e
	.byte	0xb
	.long	0x3c9
	.uleb128 0x5
	.byte	0x2
	.byte	0x4f
	.byte	0xb
	.long	0x3f6
	.uleb128 0x5
	.byte	0x2
	.byte	0x51
	.byte	0xb
	.long	0x4c8
	.uleb128 0x5
	.byte	0x2
	.byte	0x52
	.byte	0xb
	.long	0x21f
	.byte	0
	.uleb128 0x6
	.ascii "__gnu_cxx\0"
	.byte	0x3
	.word	0x106
	.byte	0xb
	.long	0x1db
	.uleb128 0x3
	.ascii "__cxx11\0"
	.byte	0x3
	.word	0x108
	.byte	0x41
	.uleb128 0x4
	.byte	0x3
	.word	0x108
	.byte	0x41
	.long	0x1c4
	.byte	0
	.uleb128 0x7
	.byte	0x1
	.byte	0x6
	.ascii "char\0"
	.uleb128 0x7
	.byte	0x8
	.byte	0x7
	.ascii "long long unsigned int\0"
	.uleb128 0x7
	.byte	0x8
	.byte	0x5
	.ascii "long long int\0"
	.uleb128 0x8
	.ascii "intptr_t\0"
	.byte	0x4
	.byte	0x3e
	.byte	0x21
	.long	0x1fd
	.uleb128 0x8
	.ascii "uintptr_t\0"
	.byte	0x4
	.byte	0x4b
	.byte	0x2a
	.long	0x1e3
	.uleb128 0x7
	.byte	0x2
	.byte	0x7
	.ascii "short unsigned int\0"
	.uleb128 0x7
	.byte	0x4
	.byte	0x5
	.ascii "int\0"
	.uleb128 0x7
	.byte	0x4
	.byte	0x5
	.ascii "long int\0"
	.uleb128 0x7
	.byte	0x2
	.byte	0x7
	.ascii "wchar_t\0"
	.uleb128 0x7
	.byte	0x4
	.byte	0x7
	.ascii "unsigned int\0"
	.uleb128 0x7
	.byte	0x4
	.byte	0x7
	.ascii "long unsigned int\0"
	.uleb128 0x7
	.byte	0x1
	.byte	0x8
	.ascii "unsigned char\0"
	.uleb128 0x7
	.byte	0x10
	.byte	0x4
	.ascii "long double\0"
	.uleb128 0x9
	.ascii "decltype(nullptr)\0"
	.uleb128 0x8
	.ascii "int8_t\0"
	.byte	0x5
	.byte	0x23
	.byte	0x15
	.long	0x2cc
	.uleb128 0x7
	.byte	0x1
	.byte	0x6
	.ascii "signed char\0"
	.uleb128 0x8
	.ascii "uint8_t\0"
	.byte	0x5
	.byte	0x24
	.byte	0x17
	.long	0x28a
	.uleb128 0x8
	.ascii "int16_t\0"
	.byte	0x5
	.byte	0x25
	.byte	0xf
	.long	0x2fb
	.uleb128 0x7
	.byte	0x2
	.byte	0x5
	.ascii "short int\0"
	.uleb128 0x8
	.ascii "uint16_t\0"
	.byte	0x5
	.byte	0x26
	.byte	0x18
	.long	0x231
	.uleb128 0x8
	.ascii "int32_t\0"
	.byte	0x5
	.byte	0x27
	.byte	0xd
	.long	0x247
	.uleb128 0x8
	.ascii "uint32_t\0"
	.byte	0x5
	.byte	0x28
	.byte	0x12
	.long	0x265
	.uleb128 0x8
	.ascii "int64_t\0"
	.byte	0x5
	.byte	0x29
	.byte	0x21
	.long	0x1fd
	.uleb128 0x8
	.ascii "uint64_t\0"
	.byte	0x5
	.byte	0x2a
	.byte	0x2a
	.long	0x1e3
	.uleb128 0x8
	.ascii "int_least8_t\0"
	.byte	0x5
	.byte	0x2d
	.byte	0x15
	.long	0x2cc
	.uleb128 0x8
	.ascii "uint_least8_t\0"
	.byte	0x5
	.byte	0x2e
	.byte	0x17
	.long	0x28a
	.uleb128 0x8
	.ascii "int_least16_t\0"
	.byte	0x5
	.byte	0x2f
	.byte	0xf
	.long	0x2fb
	.uleb128 0x8
	.ascii "uint_least16_t\0"
	.byte	0x5
	.byte	0x30
	.byte	0x18
	.long	0x231
	.uleb128 0x8
	.ascii "int_least32_t\0"
	.byte	0x5
	.byte	0x31
	.byte	0xd
	.long	0x247
	.uleb128 0x8
	.ascii "uint_least32_t\0"
	.byte	0x5
	.byte	0x32
	.byte	0x12
	.long	0x265
	.uleb128 0x8
	.ascii "int_least64_t\0"
	.byte	0x5
	.byte	0x33
	.byte	0x21
	.long	0x1fd
	.uleb128 0x8
	.ascii "uint_least64_t\0"
	.byte	0x5
	.byte	0x34
	.byte	0x2a
	.long	0x1e3
	.uleb128 0x8
	.ascii "int_fast8_t\0"
	.byte	0x5
	.byte	0x3a
	.byte	0x15
	.long	0x2cc
	.uleb128 0x8
	.ascii "uint_fast8_t\0"
	.byte	0x5
	.byte	0x3b
	.byte	0x17
	.long	0x28a
	.uleb128 0x8
	.ascii "int_fast16_t\0"
	.byte	0x5
	.byte	0x3c
	.byte	0xf
	.long	0x2fb
	.uleb128 0x8
	.ascii "uint_fast16_t\0"
	.byte	0x5
	.byte	0x3d
	.byte	0x18
	.long	0x231
	.uleb128 0x8
	.ascii "int_fast32_t\0"
	.byte	0x5
	.byte	0x3e
	.byte	0xd
	.long	0x247
	.uleb128 0x8
	.ascii "uint_fast32_t\0"
	.byte	0x5
	.byte	0x3f
	.byte	0x16
	.long	0x265
	.uleb128 0x8
	.ascii "int_fast64_t\0"
	.byte	0x5
	.byte	0x40
	.byte	0x21
	.long	0x1fd
	.uleb128 0x8
	.ascii "uint_fast64_t\0"
	.byte	0x5
	.byte	0x41
	.byte	0x2a
	.long	0x1e3
	.uleb128 0x8
	.ascii "intmax_t\0"
	.byte	0x5
	.byte	0x44
	.byte	0x21
	.long	0x1fd
	.uleb128 0x8
	.ascii "uintmax_t\0"
	.byte	0x5
	.byte	0x45
	.byte	0x2a
	.long	0x1e3
	.uleb128 0xa
	.ascii "main\0"
	.byte	0x1
	.byte	0x7
	.byte	0x5
	.long	0x247
	.quad	.LFB1
	.quad	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.long	0x511
	.uleb128 0xb
	.ascii "buffer\0"
	.byte	0x1
	.byte	0x9
	.byte	0xd
	.long	0x511
	.uleb128 0x3
	.byte	0x91
	.sleb128 -160
	.byte	0
	.uleb128 0xc
	.long	0x2db
	.uleb128 0xd
	.long	0x1e3
	.byte	0x7f
	.byte	0
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
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
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
	.uleb128 0x8
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
	.uleb128 0x9
	.uleb128 0x3b
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0xa
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
	.uleb128 0xb
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
	.uleb128 0xc
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"dr"
	.long	0x2c
	.word	0x2
	.secrel32	.Ldebug_info0
	.byte	0x8
	.byte	0
	.word	0
	.word	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0
	.quad	0
	.section	.debug_line,"dr"
.Ldebug_line0:
	.section	.debug_str,"dr"
	.ident	"GCC: (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0"
	.def	_Z11test_clientPKcPh;	.scl	2;	.type	32;	.endef
	.def	_Z11test_serverPKh;	.scl	2;	.type	32;	.endef
