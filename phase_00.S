
./bomb:     file format elf64-x86-64


Disassembly of section .init:

Disassembly of section .plt:

Disassembly of section .plt.sec:

Disassembly of section .text:

0000000000402d40 <phase_00>:
  402d40:	endbr64
  402d44:	push   %rbp
  402d45:	mov    %rsp,%rbp
  402d48:	push   %r12
  402d4a:	mov    %rdi,%r12
  402d4d:	push   %rbx
  402d4e:	mov    %rsi,%rbx
  402d51:	mov    $0x2,%esi
  402d56:	sub    $0x10,%rsp
  402d5a:	call   402c60 <valid_u32_fields>
  402d5f:	test   %al,%al
  402d61:	jne    402d70 <phase_00+0x30>
  402d63:	add    $0x10,%rsp
  402d67:	xor    %eax,%eax
  402d69:	pop    %rbx
  402d6a:	pop    %r12
  402d6c:	pop    %rbp
  402d6d:	ret
  402d6e:	xchg   %ax,%ax
  402d70:	xor    %eax,%eax
  402d72:	lea    -0x14(%rbp),%rcx
  402d76:	lea    -0x18(%rbp),%rdx
  402d7a:	mov    $0x403186,%esi
  402d7f:	mov    %r12,%rdi
  402d82:	call   4011e0 <__isoc99_sscanf@plt>
  402d87:	cmp    $0x2,%eax
  402d8a:	jne    402d63 <phase_00+0x23>
  402d8c:	shr    $0x5,%rbx
  402d90:	movzbl %bl,%edi
  402d93:	xor    $0x2a,%edi
  402d96:	cmp    %edi,-0x18(%rbp)
  402d99:	jne    402d63 <phase_00+0x23>
  402d9b:	call   402d30 <tutorial_mix>
  402da0:	cmp    %eax,-0x14(%rbp)
  402da3:	sete   %al
  402da6:	add    $0x10,%rsp
  402daa:	pop    %rbx
  402dab:	pop    %r12
  402dad:	pop    %rbp
  402dae:	ret

Disassembly of section .fini:
