.text
main:
  addiu $t0, $zero, 1
  addiu $t1, $zero, 2
  beq $t0, $t1, label_1
  addu $t2, $t0, $t1
  lw $t5, 12($zero)
  sw $t5, 16($zero)
  
label_1:
  subu $t2, $t1, $t0
  sw $t2, 8($zero)
