# LU_simd
高斯消元simd加速，使用sse指令------
normal为平凡算法的高斯消元-----
sse_align_div是考虑对齐，只对除法部分向量化加速-----
sse_align_clear是考虑对齐，只对消元部分向量化加速-----
sse_align是考虑对齐，两部分都向量化加速----
sse_not_align是不考虑对齐，对两部分都向量化加速----
