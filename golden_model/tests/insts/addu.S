.text
main:
  addiu $t0, $zero, 1
  addiu $t1, $zero, 2
  addu $t2, $t0, $t1
  sll $t3, $t2, 2
  sll $t4, $t2, 3
  addu $t0, $t3, $t4
  subu $t1, $t4, $t3
  lw $t2, 0($zero)
  lw $t3, 4($zero)

  lw $t4, 8($zero)
  lw $t5, 12($zero)

  sw $t5, 16($zero)
  sw $t4, 20($zero)

  lw $t2, 16($zero)
  lw $t3, 20($zero)