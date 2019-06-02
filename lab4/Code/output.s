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
  addi $sp, $sp, -4
  sw $s8, 0($sp)
  move $s8, $sp
  addi $sp, $sp, -4096
  la $t0, -80($s8)
  li $t0, 12
  add $t1, $t0, $t0
  sw $t1, -80($s8)
  li $t0, 55
  move $t1, $t0
  sw $t1, -84($s8)
  lw $t0, -84($s8)
  sw $t0, -80($s8)
  la $t0, -80($s8)
  li $t0, 12
  add $t1, $t0, $t0
  sw $t1, -88($s8)
  lw $t0, -88($s8)
  sw $t0, -92($s8)
  lw $t0, -92($s8)
  move $a0, $t0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $v0, $0
  li $t0, 0
  move $v0, $t0
  move $sp, $s8
  lw $s8, 0($sp)
  addi $sp, $sp, 4
  jr $ra
