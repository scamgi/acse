        .global _start
        .data
l_a:    .space 40
l_b:    .space 40
l_v:    .space 4
l_e:    .space 4
l_i:    .space 4
        .text
_start: li     s0, 0                                    # tests/all/all.src:4
        la     s1, l_i
        sw     s0, 0(s1)
l_6:    la     s0, l_i                                  # tests/all/all.src:5
        lw     s0, 0(s0)
        li     s1, 10
        slt    s0, s0, s1
        beq    s0, zero, l_7
        la     s0, l_i                                  # tests/all/all.src:6
        lw     s0, 0(s0)
        la     s1, l_i
        lw     s1, 0(s1)
        li     s2, 1
        add    s1, s1, s2
        la     s2, l_a
        li     s3, 4
        mul    s0, s0, s3
        add    s2, s2, s0
        sw     s1, 0(s2)
        la     s0, l_i                                  # tests/all/all.src:7
        lw     s0, 0(s0)
        li     s1, 20
        la     s2, l_b
        li     s3, 4
        mul    s0, s0, s3
        add    s2, s2, s0
        sw     s1, 0(s2)
        la     s0, l_i                                  # tests/all/all.src:8
        lw     s0, 0(s0)
        li     s1, 1
        add    s0, s0, s1
        la     s1, l_i
        sw     s0, 0(s1)
        j      l_6                                      # tests/all/all.src:9
l_7:    li     a0, 1                                    # tests/all/all.src:12
        li     s0, 0
l_8:    li     s1, 10
        bge    s0, s1, l_9
        la     s1, l_a
        li     s2, 4
        mul    s2, s0, s2
        add    s1, s1, s2
        lw     s1, 0(s1)
        la     s2, l_e
        sw     s1, 0(s2)
        la     s1, l_e
        lw     s1, 0(s1)
        li     s2, 0
        slt    s1, s2, s1
        addi   s0, s0, 1
        bne    s1, zero, l_8
        li     a0, 0
l_9:    li     a7, 1
        addi   a0, a0, 0
        ecall
        li     a0, 10
        li     a7, 11
        addi   a0, a0, 0
        ecall
        li     a0, 1                                    # tests/all/all.src:14
        li     s0, 0
l_10:   li     s1, 10
        bge    s0, s1, l_11
        la     s1, l_a
        li     s2, 4
        mul    s2, s0, s2
        add    s1, s1, s2
        lw     s1, 0(s1)
        la     s2, l_e
        sw     s1, 0(s2)
        la     s1, l_e
        lw     s1, 0(s1)
        li     s2, 5
        slt    s1, s1, s2
        addi   s0, s0, 1
        bne    s1, zero, l_10
        li     a0, 0
l_11:   li     a7, 1
        addi   a0, a0, 0
        ecall
        li     a0, 10
        li     a7, 11
        addi   a0, a0, 0
        ecall
        li     s0, 3                                    # tests/all/all.src:17
        li     s1, 1
        li     s2, 0
l_12:   li     s3, 10
        bge    s2, s3, l_13
        la     s3, l_a
        li     s4, 4
        mul    s4, s2, s4
        add    s3, s3, s4
        lw     s3, 0(s3)
        la     s4, l_e
        sw     s3, 0(s4)
        la     s3, l_e
        lw     s3, 0(s3)
        li     s4, 5
        slt    s3, s4, s3
        addi   s2, s2, 1
        bne    s3, zero, l_12
        li     s1, 0
l_13:   li     s2, 2
        div    s1, s1, s2
        add    s0, s0, s1
        li     a7, 1
        addi   a0, s0, 0
        ecall
        li     a0, 10
        li     a7, 11
        addi   a0, a0, 0
        ecall
        li     a0, 1                                    # tests/all/all.src:20
        li     s0, 0
l_14:   li     s1, 10
        bge    s0, s1, l_15
        la     s1, l_a
        li     s2, 4
        mul    s2, s0, s2
        add    s1, s1, s2
        lw     s1, 0(s1)
        la     s2, l_e
        sw     s1, 0(s2)
        la     s1, l_e
        lw     s1, 0(s1)
        li     s2, 1
        sub    s1, s1, s2
        la     s2, l_b
        li     s3, 4
        mul    s1, s1, s3
        add    s2, s2, s1
        lw     s2, 0(s2)
        li     s1, 15
        slt    s2, s2, s1
        addi   s0, s0, 1
        bne    s2, zero, l_14
        li     a0, 0
l_15:   li     a7, 1
        addi   a0, a0, 0
        ecall
        li     a0, 10
        li     a7, 11
        addi   a0, a0, 0
        ecall
        li     a7, 10
        ecall
