name:   aalok sathe
class:  cs301
what:   programming assignment 1


--Testing--

Test 1: (test1.mach)
    Contains valid machine language instructions that are implemented
    in the program. Expect output to match inst.out for successful run.
    This test file contains at least one instruction of each of the
    supported types. For each instruction requiring register operands,
    arbitrary register numbers are chosen and tested. For each instruction
    involving an immediate field, positive, negative, and 0 values are tested.

Test 2: (test2.mach)
    Contains invalid characters (non-0 or 1). Expect to break early and
    declare format of the file incorrect.

Test 3: (test3.mach)
    Contains some lines that are smaller as well as some that are longer
    than 32 bits. Expect to detect incorrect format and fail gracefully.

Test 4: (test4.mach)
    Contains a non-implemented instruction. Expect to detect that instruction
    is not supported and fail gracefully.
