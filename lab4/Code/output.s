.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra

write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra

main:
  move $s8, $sp
  addi $sp, $sp, -128
  li $t0, 0
  move $t1, $t0
  sw $t1, -8($s8)
  li $t0, 1
  move $t1, $t0
  sw $t1, -12($s8)
  li $t0, 0
  move $t1, $t0
  sw $t1, -16($s8)
  move $s8, $sp
  addi $sp, $sp, -4
  sw $ra, -4($s8)
  jal read
  lw $ra, -4($s8)
  move $sp, $s8
  addi $s8, $s8, 128
  move $t0, $v0
  sw $t0, -20($s8)
  lw $t0, -20($s8)
  move $t1, $t0
  sw $t1, -24($s8)
label1:
  lw $t0, -16($s8)
  lw $t1, -24($s8)
  bge $t0, $t1, label2
  lw $t0, -8($s8)
  lw $t1, -12($s8)
  add $t2, $t0, $t1
  sw $t2, -28($s8)
  move $s8, $sp
  addi $sp, $sp, -4
  lw $t0, -12($s8)
  move $a0, $t0
  sw $ra, -4($s8)
  jal write
  lw $ra, -4($s8)
  move $sp, $s8
  addi $s8, $s8, 128
  lw $t0, -12($s8)
  move $t1, $t0
  sw $t1, -8($s8)
  lw $t0, -28($s8)
  move $t1, $t0
  sw $t1, -12($s8)
  lw $t0, -16($s8)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -16($s8)
  j label1
label2:
  li $t0, 0
  move $v0, $t0
  jr $ra
