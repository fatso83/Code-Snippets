	.file	"prg1_1.c"
gcc2_compiled.:
___gnu_compiled_c:
	.def	___main;	.scl	2;	.type	32;	.endef
.text
	.align 32
LC0:
	.ascii "a single argument (the directory name) is required!\0"
LC1:
	.ascii "can't open %s\0"
LC2:
	.ascii "%s\12\0"
	.align 4
.globl _main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	call ___main
	cmpl $2,8(%ebp)
	je L11
	addl $-12,%esp
	pushl $LC0
	call _err_quit
	addl $16,%esp
L11:
	addl $-12,%esp
	movl 12(%ebp),%eax
	addl $4,%eax
	movl (%eax),%edx
	pushl %edx
	call _opendir
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-4(%ebp)
	cmpl $0,-4(%ebp)
	jne L12
	addl $-8,%esp
	movl 12(%ebp),%eax
	addl $4,%eax
	movl (%eax),%edx
	pushl %edx
	pushl $LC1
	call _err_sys
	addl $16,%esp
L12:
	nop
	.align 4
L13:
	addl $-12,%esp
	movl -4(%ebp),%eax
	pushl %eax
	call _readdir
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-8(%ebp)
	cmpl $0,-8(%ebp)
	jne L15
	jmp L14
	.align 4
L15:
	addl $-8,%esp
	movl -8(%ebp),%eax
	addl $20,%eax
	pushl %eax
	pushl $LC2
	call _printf
	addl $16,%esp
	jmp L13
	.align 4
L14:
	addl $-12,%esp
	movl -4(%ebp),%eax
	pushl %eax
	call _closedir
	addl $16,%esp
	addl $-12,%esp
	pushl $0
	call _exit
	addl $16,%esp
	.align 4
L10:
	movl %ebp,%esp
	popl %ebp
	ret
	.def	_exit;	.scl	2;	.type	32;	.endef
	.def	_closedir;	.scl	2;	.type	32;	.endef
	.def	_printf;	.scl	2;	.type	32;	.endef
	.def	_readdir;	.scl	2;	.type	32;	.endef
	.def	_err_sys;	.scl	2;	.type	32;	.endef
	.def	_opendir;	.scl	2;	.type	32;	.endef
	.def	_err_quit;	.scl	2;	.type	32;	.endef
