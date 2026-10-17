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

// Start from 405B60
static uint32_t const P5_MEM[64] = {
    0x00405c20, 0x00000000, 0x00405bf0, 0x00000000,
    0x00405bc0, 0x00000000, 0x00405c00, 0x00000000,
    0x00405be0, 0x00000000, 0x00405c40, 0x00000000,
    0x00405bd0, 0x00000000, 0x00405bb0, 0x00000000,
    0x00000000, 0x00000000, 0x00000624, 0x0000afc3,
    0x00405c50, 0x00000000, 0x00000538, 0x0000c8c6,
    0x00405c00, 0x00000000, 0x0000084f, 0x00005a63,
    0x00405bb0, 0x00000000, 0x0000025f, 0x0000f928,
    0x00405c40, 0x00000000, 0x00000409, 0x0000d347,
    0x00405bc0, 0x00000000, 0x0000000e, 0x00008d6d,
    0x00405be0, 0x00000000, 0x0000034c, 0x0000e907,
    0x00405c30, 0x00000000, 0x00000125, 0x00008a03,
    0x00405bf0, 0x00000000, 0x00000926, 0x0000c21a,
    0x00405ba0, 0x00000000, 0x00000748, 0x0000f6fd,
    0x00405bd0, 0x00000000, 0x00000b50, 0x00005b57,
    0x00405c10, 0x00000000, 0x00000a46, 0x00001477
};

int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;

    count = 2;

    uint64_t rax, rcx, rdx, rsi, rdi;

    // rsi -> r8 -> values[1]
    rdi = challenge;

    // Traverse
    rax = rdi;
    rcx = rdi;
    rax = rax & 0x7;
    rcx = rcx >> 0x3;
    rdx = P5_MEM[rax << 1];

    rax = rdi;
    rdi >>= 0x20;
    rax >>= 0x6;
    rax = (rdi ^ rax) & 0xFFFFFFFF;
    rsi = (P5_MEM[(rdx + 0x8 - 0x405B60) >> 2]);
    rdi = rcx & 0xFFFFFFFF;
    rax = ((rax & 0xFFFFFFFF) ^ 0x2CBC);
    rax = (rax + P5_MEM[(rdx + 0xC - 0x405B60) >> 2]) & 0xFFFFFFFF;
    rax = (rax * 5) & 0xFFFFFFFF;
    values[1] = rsi & 0xFFFFFFFF;
    rax = (rax ^ rsi) & 0xFFFFFFFF;
    rdi = rdi & 0x3;

    for (rcx = 0x1; rcx <= rdi; rcx++) {
        rdx = P5_MEM[(rdx - 0x405B60) >> 2];
        // printf("%lx\n", rdx);
        rsi = P5_MEM[(rdx + 0x8 - 0x405B60) >> 2];
        rax = (rax + P5_MEM[(rdx + 0xC - 0x405B60) >> 2]) & 0xFFFFFFFF;
        rax = (rax * 5) & 0xFFFFFFFF;
        values[1] = rsi & 0xFFFFFFFF;
        rax = (rax ^ rsi) & 0xFFFFFFFF;
    }

    values[0] = rax;
    
    return write_answer(out, cap, values, count);
}

typedef uint32_t (*P6OP)(uint32_t);

uint32_t p6_op_0(uint32_t x) { return ((x << 7) + 0xCD08) ^ (x * 7); }
uint32_t p6_op_1(uint32_t x) { return (x ^ 0xDEF0) + (x << 1); }
uint32_t p6_op_2(uint32_t x) { return ((x << 2) | (x >> 30)) ^ 0xE16B; }
uint32_t p6_op_3(uint32_t x) { return (x >> 5) | (x << 27); }
uint32_t p6_op_4(uint32_t x) { return ((x >> 1) ^ x) + 0x5179; }
uint32_t p6_op_5(uint32_t x) { return (x + 0xAE45) ^ (x >> 3); }

static P6OP const P6_OP[] = {
    p6_op_1,
    p6_op_4,
    p6_op_1,
    p6_op_0,
    p6_op_3,
    p6_op_5,
    p6_op_2,
    p6_op_3
};

int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;

    count = 1;

    uint64_t rax, rbx, rdi;

    rdi = challenge;

    // P5 Dispatch
    rbx = rdi;
    rdi = rdi >> 6;
    rax = rbx;
    rax = rax >> 0x20;
    rdi = (rdi ^ rax) & 0xFFFFFFFF;
    rax = rbx;
    rbx = rbx & 0x38;
    rax = rax & 0x7;
    rax = P6_OP[rax](rdi);
    rax = rax ^ 0x6969;
    rdi = rax & 0xFFFFFFFF;
    rax = P6_OP[rbx >> 3](rdi);
    rax = rax + 0x1EED;

    values[0] = rax;

    return write_answer(out, cap, values, count);
}

size_t int_to_string(char** dest_pt, uint32_t num) {
    size_t res = 0;
    if (num / 10 != 0) {
        int_to_string(dest_pt, num / 10);
    }
    **dest_pt = '0' + (char)(num % 10);
    (*dest_pt)++;
    return res + 1;
}

int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    (void)challenge;
    (void)context;
    (void)out;
    (void)cap;
    // TODO（选做）: 根据 secret_gate、secret_copy 和 secret_phase 的汇编构造完整的第七行，写入 out 后返回 0。
    // 第七行不一定只有整数，可以直接用 snprintf 写入；还没完成时保持返回 1，测试会显示 not attempted。
    
    // printf("%lx %lx\n", challenge, context);

/*
    DRAFTS:

    -> secretGate (length check?) -> secretCopy -> secretPhase

    36 <= strlen < 128

    secret copy

    check if [32:36] == "OPEN"

    r12 = challenge, r13 = r14 = context

    ans = *^{32}OPEN arr[0] arr[1] arr[2]

*/

    uint64_t rax, rcx, rdx, rsi, rdi, r9, r8;
    uint32_t arr[3] = {0, 0, 0};

    rcx = context & 0xFFFFFFFF;
    rdx = 0;
    rcx = rcx & 3;
    arr[0] = rcx;
    rsi = challenge;
    r9 = ((rcx & 0xFFFFFFFF) * 0x4EF9) & 0xFFFFFFFF;
    r8 = context;
    rsi = rsi >> 0x20;
    r8 = r8 >> 0x20;
    rsi = (rsi ^ challenge) & 0xFFFFFFFF;
    r8 = (r8 ^ context) & 0xFFFFFFFF;
    context = context >> 0x10;
    rdi = rsi & 0xFFFFFFFF;
    r9 = (r9 ^ context) & 0xFFFFFFFF;
    rdi = (rdi ^ r8) & 0xFFFFFFFF;
    // Secret Mix Begin
    rax = ((rdi * 5) + 0x4EF9) & 0xFFFFFFFF;
    rax = rax ^ 0xF111;
    // Secret Mix End
    rax = (rax ^ r9) & 0xFFFFFFFF;
    arr[1] = rax;
    rsi = ((rsi & 0xFFFFFFFF) >> 3);
    rdx = ((r8 + rcx + 0xF111) & 0xFFFFFFFF);
    rdx = ((rdx & 0xFFFFFFFF) + (rsi & 0xFFFFFFFF)) & 0xFFFFFFFF;
    rax = (rax ^ rdx) & 0xFFFFFFFF;
    arr[2] = rax;

    // printf("%u %u %u\n", arr[0], arr[1], arr[2]);

    static const char payload[] = "76543210765432107654321076543210OPEN";
    
    char* pt = out;
    size_t used = 0;
    for (size_t i = 0; i < sizeof(payload) - 1; i++) {
        used++;
        *pt = payload[i];
        pt++;
    }
    *pt = ' ';
    pt++;
    used++;
    used += int_to_string(&pt, arr[0]);
    *pt = ' ';
    pt++;
    used++;
    used += int_to_string(&pt, arr[1]);
    *pt = ' ';
    pt++;
    used++;
    used += int_to_string(&pt, arr[2]);
    *pt = '\0';
    pt++;
    used++;

    return used > cap;
}
