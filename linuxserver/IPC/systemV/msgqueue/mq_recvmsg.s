	.file	"mq_recvmsg.c"
	.section	.rodata
.LC0:
	.string	"Hello world"
	.align 4
.LC1:
	.string	"Usage:./mqrecvmsg mqname priority"
.LC2:
	.string	"%s(%s-%d):%s\n"
.LC3:
	.string	"mq_recvmsg.c"
.LC4:
	.string	"recv:%s  type:%d\n"
	.text
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	subl	$112, %esp
	movl	$.LC0, 92(%esp)
	movl	$1684234849, 65(%esp)
	movl	$1751606885, 69(%esp)
	movl	$1818978921, 73(%esp)
	movl	$1886350957, 77(%esp)
	movl	$1953722993, 81(%esp)
	movl	$2021095029, 85(%esp)
	movw	$31353, 89(%esp)
	movb	$0, 91(%esp)
	movl	$0, 96(%esp)
	movl	$0, 100(%esp)
	movl	$2, 96(%esp)
	cmpl	$3, 8(%ebp)
	je	.L2
	movl	stderr, %eax
	movl	%eax, %edx
	movl	$.LC1, %eax
	movl	%edx, 12(%esp)
	movl	$33, 8(%esp)
	movl	$1, 4(%esp)
	movl	%eax, (%esp)
	call	fwrite
	movl	stderr, %eax
	movl	%eax, 4(%esp)
	movl	$10, (%esp)
	call	fputc
	movl	$-1, (%esp)
	call	exit
.L2:
	movl	12(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	movl	$1, 4(%esp)
	movl	%eax, (%esp)
	call	ftok
	movl	96(%esp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	msgget
	movl	%eax, 100(%esp)
	cmpl	$-1, 100(%esp)
	jne	.L3
	call	__errno_location
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	strerror
	movl	$.LC2, %ecx
	movl	stderr, %edx
	movl	%eax, 20(%esp)
	movl	$17, 16(%esp)
	movl	$__FUNCTION__.3146, 12(%esp)
	movl	$.LC3, 8(%esp)
	movl	%ecx, 4(%esp)
	movl	%edx, (%esp)
	call	fprintf
	movl	$-1, (%esp)
	call	exit
.L3:
	movl	12(%ebp), %eax
	addl	$8, %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	atoi
	movl	%eax, 104(%esp)
	movl	$0, 16(%esp)
	movl	104(%esp), %eax
	movl	%eax, 12(%esp)
	movl	$16, 8(%esp)
	leal	44(%esp), %eax
	movl	%eax, 4(%esp)
	movl	100(%esp), %eax
	movl	%eax, (%esp)
	call	msgrcv
	movl	%eax, 108(%esp)
	cmpl	$-1, 108(%esp)
	jne	.L4
	call	__errno_location
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	strerror
	movl	$.LC2, %ecx
	movl	stderr, %edx
	movl	%eax, 20(%esp)
	movl	$21, 16(%esp)
	movl	$__FUNCTION__.3146, 12(%esp)
	movl	$.LC3, 8(%esp)
	movl	%ecx, 4(%esp)
	movl	%edx, (%esp)
	call	fprintf
	movl	$-1, (%esp)
	call	exit
.L4:
	movl	108(%esp), %eax
	movb	$0, 48(%esp,%eax)
	movl	44(%esp), %edx
	movl	$.LC4, %eax
	movl	%edx, 8(%esp)
	leal	44(%esp), %edx
	addl	$4, %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	$0, %eax
	leave
	ret
	.size	main, .-main
	.section	.rodata
	.type	__FUNCTION__.3146, @object
	.size	__FUNCTION__.3146, 5
__FUNCTION__.3146:
	.string	"main"
	.ident	"GCC: (GNU) 4.4.7 20120313 (Red Hat 4.4.7-11)"
	.section	.note.GNU-stack,"",@progbits
