/* Blake2s.c -- BLAKE2sp hash for RAR5 support
2024-05-18 : Igor Pavlov : Public domain
2015-2019 : Samuel Neves : original code : CC0 1.0 Universal (CC0 1.0). */

#include "Precomp.h"

#include <string.h>

#include "Blake2.h"

#define BLAKE2S_NUM_ROUNDS 10
#define BLAKE2S_FINAL_FLAG (~(UInt32)0)
#define BLAKE2SP_SUPER_BLOCK_SIZE (Z7_BLAKE2S_BLOCK_SIZE * Z7_BLAKE2SP_PARALLEL_DEGREE)

#define GET_UI32(p) \
    ((UInt32)((const Byte *)(p))[0] | \
    ((UInt32)((const Byte *)(p))[1] << 8) | \
    ((UInt32)((const Byte *)(p))[2] << 16) | \
    ((UInt32)((const Byte *)(p))[3] << 24))

#define SET_UI32(p, v) \
    { \
        Byte *blake2_dst = (Byte *)(p); \
        UInt32 blake2_value = (UInt32)(v); \
        blake2_dst[0] = (Byte)blake2_value; \
        blake2_dst[1] = (Byte)(blake2_value >> 8); \
        blake2_dst[2] = (Byte)(blake2_value >> 16); \
        blake2_dst[3] = (Byte)(blake2_value >> 24); \
    }

#define ROTR32(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

typedef struct
{
    UInt32 h[8];
    UInt32 t[2];
    UInt32 f[2];
    Byte buf[Z7_BLAKE2S_BLOCK_SIZE];
    UInt32 buf_pos;
    UInt32 last_node_f1;
    UInt32 dummy[2];
} CBlake2sState;

typedef struct
{
    CBlake2sState states[Z7_BLAKE2SP_PARALLEL_DEGREE];
    unsigned buf_pos;
} CBlake2spState;

typedef char assert_blake2sp_state_fits[(sizeof(CBlake2spState) <= sizeof(CBlake2sp)) ? 1 : -1];

static const UInt32 BLAKE2S_IV[8] =
{
    0x6A09E667UL, 0xBB67AE85UL, 0x3C6EF372UL, 0xA54FF53AUL,
    0x510E527FUL, 0x9B05688CUL, 0x1F83D9ABUL, 0x5BE0CD19UL
};

static const Byte BLAKE2S_SIGMA[BLAKE2S_NUM_ROUNDS][16] =
{
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
    { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 },
    { 11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4 },
    { 7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8 },
    { 9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13 },
    { 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 },
    { 12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11 },
    { 13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10 },
    { 6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5 },
    { 10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0 }
};

static void blake2s_init0(CBlake2sState *state)
{
    unsigned i;

    for (i = 0; i < 8; i++)
        state->h[i] = BLAKE2S_IV[i];

    state->t[0] = 0;
    state->t[1] = 0;
    state->f[0] = 0;
    state->f[1] = 0;
    state->buf_pos = 0;
    state->last_node_f1 = 0;
}

static void blake2s_increment_counter(CBlake2sState *state, UInt32 value)
{
    state->t[0] += value;
    state->t[1] += state->t[0] < value;
}

static void blake2s_compress(CBlake2sState *state)
{
    UInt32 m[16];
    UInt32 v[16];
    unsigned i;
    unsigned r;

    for (i = 0; i < 16; i++)
        m[i] = GET_UI32(state->buf + i * sizeof(UInt32));

    for (i = 0; i < 8; i++)
        v[i] = state->h[i];

    v[8] = BLAKE2S_IV[0];
    v[9] = BLAKE2S_IV[1];
    v[10] = BLAKE2S_IV[2];
    v[11] = BLAKE2S_IV[3];
    v[12] = state->t[0] ^ BLAKE2S_IV[4];
    v[13] = state->t[1] ^ BLAKE2S_IV[5];
    v[14] = state->f[0] ^ BLAKE2S_IV[6];
    v[15] = state->f[1] ^ BLAKE2S_IV[7];

#define G(a, b, c, d, x, y) \
    a = a + b + x; \
    d = ROTR32(d ^ a, 16); \
    c = c + d; \
    b = ROTR32(b ^ c, 12); \
    a = a + b + y; \
    d = ROTR32(d ^ a, 8); \
    c = c + d; \
    b = ROTR32(b ^ c, 7)

#define ROUND(s) \
    G(v[0], v[4], v[8], v[12], m[s[0]], m[s[1]]); \
    G(v[1], v[5], v[9], v[13], m[s[2]], m[s[3]]); \
    G(v[2], v[6], v[10], v[14], m[s[4]], m[s[5]]); \
    G(v[3], v[7], v[11], v[15], m[s[6]], m[s[7]]); \
    G(v[0], v[5], v[10], v[15], m[s[8]], m[s[9]]); \
    G(v[1], v[6], v[11], v[12], m[s[10]], m[s[11]]); \
    G(v[2], v[7], v[8], v[13], m[s[12]], m[s[13]]); \
    G(v[3], v[4], v[9], v[14], m[s[14]], m[s[15]])

    for (r = 0; r < BLAKE2S_NUM_ROUNDS; r++)
        ROUND(BLAKE2S_SIGMA[r]);

#undef ROUND
#undef G

    for (i = 0; i < 8; i++)
        state->h[i] ^= v[i] ^ v[i + 8];
}

static void blake2s_update(CBlake2sState *state, const Byte *data, size_t size)
{
    while (size != 0)
    {
        unsigned pos = (unsigned)state->buf_pos;
        unsigned rem = Z7_BLAKE2S_BLOCK_SIZE - pos;

        if (size <= rem)
        {
            memcpy(state->buf + pos, data, size);
            state->buf_pos += (UInt32)size;
            return;
        }

        memcpy(state->buf + pos, data, rem);
        blake2s_increment_counter(state, Z7_BLAKE2S_BLOCK_SIZE);
        blake2s_compress(state);
        state->buf_pos = 0;
        data += rem;
        size -= rem;
    }
}

static void blake2s_final(CBlake2sState *state, Byte *digest)
{
    unsigned i;

    blake2s_increment_counter(state, state->buf_pos);
    state->f[0] = BLAKE2S_FINAL_FLAG;
    state->f[1] = state->last_node_f1;

    memset(state->buf + state->buf_pos, 0, Z7_BLAKE2S_BLOCK_SIZE - state->buf_pos);
    blake2s_compress(state);

    for (i = 0; i < 8; i++)
        SET_UI32(digest + sizeof(UInt32) * i, state->h[i]);
}

static void blake2sp_init_spec(CBlake2sState *state, unsigned node_offset, unsigned node_depth)
{
    blake2s_init0(state);
    state->h[0] ^= (UInt32)Z7_BLAKE2S_DIGEST_SIZE |
        ((UInt32)Z7_BLAKE2SP_PARALLEL_DEGREE << 16) |
        ((UInt32)2 << 24);
    state->h[2] ^= (UInt32)node_offset;
    state->h[3] ^= ((UInt32)node_depth << 16) |
        ((UInt32)Z7_BLAKE2S_DIGEST_SIZE << 24);
}

static CBlake2spState *blake2sp_state(CBlake2sp *state)
{
    return (CBlake2spState *)(void *)state;
}

BoolInt Blake2sp_SetFunction(CBlake2sp *state, unsigned algo)
{
    UNUSED_VAR(state)
    UNUSED_VAR(algo)
    return True;
}

void Blake2sp_Init(CBlake2sp *state)
{
    CBlake2spState *ctx = blake2sp_state(state);
    unsigned i;

    ctx->buf_pos = 0;

    for (i = 0; i < Z7_BLAKE2SP_PARALLEL_DEGREE; i++)
        blake2sp_init_spec(&ctx->states[i], i, 0);

    ctx->states[Z7_BLAKE2SP_PARALLEL_DEGREE - 1].last_node_f1 = BLAKE2S_FINAL_FLAG;
}

void Blake2sp_InitState(CBlake2sp *state)
{
    Blake2sp_Init(state);
}

void Blake2sp_Update(CBlake2sp *state, const Byte *data, size_t size)
{
    CBlake2spState *ctx = blake2sp_state(state);
    unsigned pos = ctx->buf_pos;

    while (size != 0)
    {
        unsigned index = pos / Z7_BLAKE2S_BLOCK_SIZE;
        unsigned rem = Z7_BLAKE2S_BLOCK_SIZE - (pos & (Z7_BLAKE2S_BLOCK_SIZE - 1));

        if (rem > size)
            rem = (unsigned)size;

        blake2s_update(&ctx->states[index], data, rem);
        data += rem;
        size -= rem;
        pos += rem;
        pos &= BLAKE2SP_SUPER_BLOCK_SIZE - 1;
    }

    ctx->buf_pos = pos;
}

void Blake2sp_Final(CBlake2sp *state, Byte *digest)
{
    CBlake2spState *ctx = blake2sp_state(state);
    CBlake2sState root;
    unsigned i;

    blake2sp_init_spec(&root, 0, 1);
    root.last_node_f1 = BLAKE2S_FINAL_FLAG;

    for (i = 0; i < Z7_BLAKE2SP_PARALLEL_DEGREE; i++)
    {
        Byte hash[Z7_BLAKE2S_DIGEST_SIZE];

        blake2s_final(&ctx->states[i], hash);
        blake2s_update(&root, hash, Z7_BLAKE2S_DIGEST_SIZE);
    }

    blake2s_final(&root, digest);
}

void z7_Black2sp_Prepare(void)
{
}
