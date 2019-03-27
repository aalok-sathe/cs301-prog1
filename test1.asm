add $0, $0, $0 # double the value in $v0 for no reason
addi $12, $25, -4242
addi $3, $9, 3141
addi $1, $8, 0
sub  $2, $8, $15
mult $10, $11
mfhi $7
srl $20, $19, 967
sra $17, $22, 2
slti  $1, $3, -3141
slti  $4, $7, 4242
slti  $8, $7, 0
lw $a2, 0($18)
lw $a1, 1197($17)
lw $a0, -4532($12)
beq $4, $5, 0x314
j label 0x400200
