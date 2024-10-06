.data
  num1: .word 23
  num2: .word 6

.text
main:
  addiu $t0, $zero, 1
  addiu $t1, $zero, 2
  addu $t2, $t1, $t0

  li $t4, num1
  li $t5, num2

  lw $t4, 0($t4)
  lw $t5, 0($t5)

  beq $t4, $t5, end

func:
  addiu $t0, $t4, 0
  addiu $t1, $t5, 0
  subu $t2, $t1, $t0

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

end:
  nop
  nop