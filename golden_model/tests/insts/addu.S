.text
start:
  li $zero, 0

  li $v0, 0
  li $v1, 0

  li $a0, 0
  li $a1, 0
  li $a2, 0
  li $a3, 0

  li $t0, 0
  li $t1, 0
  li $t2, 0
  li $t3, 0
  li $t4, 0
  li $t5, 0
  li $t6, 0
  li $t7, 0

  li $s0, 0
  li $s1, 0
  li $s2, 0
  li $s3, 0
  li $s4, 0
  li $s5, 0
  li $s6, 0
  li $s7, 0

  li $t8, 0 
  li $t9, 0

  li $k0, 0 
  li $k1, 0 
  
  li $gp, 0
  li $sp, 0
  li $fp, 0 

  li $ra, 0
  
main:
  addiu $t0, $zero, 1
  addiu $t1, $zero, 2
  addu $t2, $t1, $t0
  lw $t4, 8($zero)
  lw $t5, 12($zero)

  # beq $t4, $t5, L1
  li $t1, func
  jr $t1 
  subu $t3, $t1, $t0
  lw $t4, 0($zero)
  lw $t5, 4($zero)

L1:
  addu $t6, $t4, $t5

func:
  lw $t0, 0($zero)
  lw $t1, 4($zero)
  addu $t2, $t1, $t0
  slt $t2, $t0, $t1
  sltu $t2, $t0, $t1
  sll $t3, $t1, 4
  srl $t4, $t1, 4
  sra $t5, $t1, 4

  lui $t6, 8
  and $t2, $t0, $t1
  nor $t3, $t0, $t1
  or $t4, $t0, $t1
  xor $t5, $t0, $t1