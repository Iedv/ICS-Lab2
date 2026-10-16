#include "defuser.h"

#include <inttypes.h>
#include <stdio.h>

#define MAX_FIELDS 8

/*
 * 把 values 中的 count 个整数写成一行“空格分隔的无符号十进制整数”，存入 out。
 * 成功返回 0；count 为 0（还没有实现）、snprintf 出错或结果被截断时返回非零。
 */
static int write_answer(char *out, size_t cap, const uint32_t *values, size_t count) {
    size_t used = 0;
    for (size_t i = 0; i < count; ++i) {
        int size = snprintf(out + used, cap - used, "%s%" PRIu32, i ? " " : "", values[i]);
        if (size < 0 || (size_t)size >= cap - used) return 1;
        used += (size_t)size;
    }
    return count == 0;
}

int solve_phase_1(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    uint64_t rax, rdx, rsi;
    uint32_t r9d, r8d, ecx;
    rax = rdx = challenge;
    r8d = challenge & 0xFFFFFFFF;
    rax >>= 32;
    rdx >>= 16;
    r9d = rsi = rax & 0xFFFFFFFF;
    // STEP 0 BEGIN
    challenge = (challenge & 0xFFFFFFFF00000000) | ((challenge & 0xFFFFFFFF) - (rsi & 0xFFFFFFFF));
    rax = (rax & 0xFFFFFFFF00000000) | ((challenge * 3 + (0xF3A6)) & 0xFFFFFFFF);
    // STEP 0 END
    rsi = rdx;
    challenge = (challenge & 0xFFFFFFFF00000000) | (r9d);
    ecx = rax & 0xFFFFFFFF;
    // STEP 1 BEGIN
    rsi = (rsi & 0xFFFFFFFF00000000) | ((rsi & 0xFFFFFFFF) ^ 0x7377);
    rax = (rax & 0xFFFFFFFF00000000) | (((rsi + challenge * 8) & 0xFFFFFFFF));
    // STEP 1 END
    rsi = (rsi & 0xFFFFFFFF00000000) | (r8d);
    challenge = (challenge & 0xFFFFFFFF00000000) | (rax & 0xFFFFFFFF);
    // STEP 2 BEGIN
    rax = (rax & 0xFFFFFFFF00000000) | (((challenge + rsi * 8) & 0xFFFFFFFF) ^ 0x339E);
    // STEP 2 END
    rax = (rax & 0xFFFFFFFF00000000) | ((rax & 0xFFFFFFFF) ^ ecx);

    values[0] = rax & 0xFFFFFFFF;
    count = 1;


    return write_answer(out, cap, values, count);
}

int solve_phase_2(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;

    uint64_t rax, rdx, rsi;
    uint32_t ecx, edi;

    count = 6;

    // Set First Element
    rax = challenge;
    rax = rax >> 4;
    rax = ((rax & 0xFFFFFFFF) << 1) & 0xFFFFFFFF;
    rax = (rax & 0xFFFFFFFF) & 0x3E;
    rax = ((rax & 0xFFFFFFFF) + 0x8) & 0xFFFFFFFF;

    rsi = values[0] = rax;

    // Loop set others
    rdx = 0x1;
    ecx = 0x15;
    for (; rdx != 6; rdx += 1, ecx += 2) {
        rax = challenge;
        edi = rdx + 0xA;
        rsi = (rsi * 3) & 0xFFFFFFFF;
        rax = rax >> (ecx & 0xFF);
        rax = ((rax & 0xFFFFFFFF) & 0x1F);
        rax = ((rax & 0xFFFFFFFF) ^ edi);
        rax = (((rax & 0xFFFFFFFF) + (rsi & 0xFFFFFFFF))) & 0xFFFFFFFF;
        rsi = values[rdx] = rax;
    }

    return write_answer(out, cap, values, count);
}

int solve_phase_3(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;

    count = 2;

    uint64_t rdi, rax, rsi;

    rdi = challenge & 0xFFFFFFFF & 7;

    values[0] = rdi;

    rax = rsi = challenge;
    rax = rax >> 0x3;
    rsi = rsi >> 0x20;
    rsi = ((rsi & 0xFFFFFFFF) ^ (rax & 0xFFFFFFFF));

    // P3 Dispatch
    rax = (rdi & 0xFFFFFFFF);
    rdi = (rsi & 0xFFFFFFFF);

    switch (rax) {
        case 6: { // 1
            rax = (rdi * 5 + 0x489D6) & 0xFFFFFFFF;
            break;
        }
        case 7: { // 4
            rax = (rdi + rdi) & 0xFFFFFFFF;
            rax = rax ^ 0xEB29;
            break;
        }
        case 0: { // 2
            rax = rdi & 0xFFFFFFFF;
            rdi = (rdi & 0xFFFFFFFF) >> 1;
            rax = rax ^ 0x53A8;
            rax = ((rax & 0xFFFFFFFF) + (rdi & 0xFFFFFFFF)) & 0xFFFFFFFF;
            break;
        }
        case 1: { // 3
            rax = (rdi + rdi) & 0xFFFFFFFF;
            rax = rax ^ 0x185F;
            break;
        }
        case 2: { // 7
            rax = (rdi * 5 + 0xA26C) & 0xFFFFFFFF;
            break;
        }
        case 3: { // 6
            rax = rdi & 0xFFFFFFFF;
            rdi = (rdi & 0xFFFFFFFF) >> 0x2;
            rax = (rax & 0xFFFFFFFF) ^ 0xF4BE;
            rax = ((rax & 0xFFFFFFFF) + (rdi & 0xFFFFFFFF)) & 0xFFFFFFFF;
            break;
        }
        case 4: { // 5
            rax = (rdi * 5 + 0xEF42) & 0xFFFFFFFF;
            break;
        }
        case 5: { // 0
            rax = (rdi * 5 + 0xFECD) & 0xFFFFFFFF;
            break;
        }
    }

    values[1] = rax;

    return write_answer(out, cap, values, count);
}

void p4_walk(
    uint64_t* rax,
    uint64_t* rcx,
    uint64_t* rdx,
    uint64_t* rdi,
    uint64_t* rsi
) {
    // printf("%lu %lu %lu %lu %lu %lu\n", *rax, *rbx, *rcx, *rdx, *rdi, *rsi);
    *rax = (*rdi & 0xFFFFFFFF);
    *rdi = (*rdx & 0xFFFFFFFF);
    *rdi = ((*rdi & 0xFFFFFFFF) - (*rsi & 0xFFFFFFFF)) & 0xFFFFFFFF;
    *rdi = ((*rdi & 0xFFFFFFFF) >> 1);
    *rdi = ((*rdi & 0xFFFFFFFF) + (*rsi & 0xFFFFFFFF)) & 0xFFFFFFFF;
    if (*rax == *rdi) {
        *rax = (*rdi + 0xD8) & 0xFFFFFFFF;
        *rax = (*rax ^ (*rcx & 0xFFFFFFFF));
        // printf("%lu %lu %lu %lu %lu %lu\n", *rax, *rbx, *rcx, *rdx, *rdi, *rsi);
        return;
    }
    
    uint64_t rbx;

    rbx = (*rdi + *rcx) & 0xFFFFFFFF;

    if ((*rdi & 0xFFFFFFFF) > (*rax & 0xFFFFFFFF)) {
        // printf("UP\n");
        *rdx = (*rdi - 0x1) & 0xFFFFFFFF;
        *rdi = *rax & 0xFFFFFFFF;
        p4_walk(rax, rcx, rdx, rdi, rsi);
        *rax = (*rax * 5) & 0xFFFFFFFF;
        *rax = (*rax & 0xFFFFFFFF) ^ (rbx & 0xFFFFFFFF);
        *rax = *rax ^ 0x13;
        // printf("%lu %lu %lu %lu %lu %lu\n", *rax, *rbx, *rcx, *rdx, *rdi, *rsi);
        return;
    }

    // printf("DN\n");
    *rsi = (*rdi + 0x1) & 0xFFFFFFFF;
    *rdi = *rax & 0xFFFFFFFF;
    p4_walk(rax, rcx, rdx, rdi, rsi);
    *rax = (*rax * 5) & 0xFFFFFFFF;
    *rax = (*rax & 0xFFFFFFFF) ^ (rbx & 0xFFFFFFFF);
    *rax = *rax ^ 0xF;
    // printf("%lu %lu %lu %lu %lu %lu\n", *rax, *rbx, *rcx, *rdx, *rdi, *rsi);
    return;
}

int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;

    count = 1;

    uint64_t rax, rcx, rdi, rdx, rsi;

    rax = rcx = challenge;
    rdi = challenge & 0xFFFFFFFF;
    rsi = 0;
    rax = rax >> 0x20;
    rcx = rcx >> 0x7;
    rdi = rdi & 0x7F;
    rdx = 0x7F;
    rcx = (rax & 0xFFFFFFFF) ^ (rcx & 0xFFFFFFFF);

    // Walk
    p4_walk(&rax, &rcx, &rdx, &rdi, &rsi);

    values[0] = rax;

    return write_answer(out, cap, values, count);
}

static const uint64_t* P5_RO[7] = {
    0x405C20,
    0x405BF0,
    0x405BC0,
    0x405C00,
    0x405BE0,
    0x405C40,
    0x405BD0,
    0x405BB0
}

uint64_t P5_MEM[0x1000];

int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;

    count = 2;

    uint64_t rax, rbx, rcx, rdx, rsi, rdi, r8;

    rsi = 0x14;
    rdi = challenge;

    // Traverse
    rax = rdi;
    rcx = rdi;
    r8 = rsi;
    rax = rax & 0x7;
    rcx = rcx >> 0x3;
    rdx = P5_RO[rax - 0x405B00];

    rax = rdi;
    rdi >>= 0x20;
    rax >>= 0x6;
    rax = (rdi ^ rax) & 0xFFFFFFFF;
    rsi = (P5_MEM[rdx + 0x8]) & 0xFFFFFFFF;
    rdi = rcx & 0xFFFFFFFF;
    rax = ((rax & 0xFFFFFFFF) ^ 0x2CBC);
    rax = (rdx + 0xC) & 0xFFFFFFFF;
    rax = (rax * 5) & 0xFFFFFFFF;

 

    values[0] = rax;
    

    return write_answer(out, cap, values, count);
}

int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_06 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    return write_answer(out, cap, values, count);
}

int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    (void)challenge;
    (void)context;
    (void)out;
    (void)cap;
    // TODO（选做）: 根据 secret_gate、secret_copy 和 secret_phase 的汇编构造完整的第七行，写入 out 后返回 0。
    // 第七行不一定只有整数，可以直接用 snprintf 写入；还没完成时保持返回 1，测试会显示 not attempted。
    return 1;
}
