.data
  num1: .word 18
  num2: .word 24

.text
main:
  # load data
  li $t1, num1
  li $t2, num2

  lw $t3, 0($t1)
  lw $t4, 0($t2)

  addu $t5, $t3, $t4

  # store data
  addiu $t6, $t5, 4

  sw $t5, 0($t1)
  sw $t6, 0($t2)

  lw $t3, 0($t1)
  lw $t4, 0($t2)

  addu $t7, $t3, $t4

  # write led
  lui $a0, 0xffff
  addiu $a0, $a0, 0

  xor $t0, $t0, $t0
  addiu $t0, $t0, 0xf

  sw $t0, 0($a0)

end:
  nop
  nop