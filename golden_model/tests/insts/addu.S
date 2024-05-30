.text
main:
  addiu $t0, $zero, 1
  addiu $t1, $zero, 2
  addu $t2, $t0, $t1
  sll $t3, $t2, 2
  sll $t4, $t2, 3
  addu $t0, $t3, $t4
  subu $t1, $t4, $t3