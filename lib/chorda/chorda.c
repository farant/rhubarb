#include "chorda.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define CHORDA_HASH_OFFSET  2166136261U
#define CHORDA_HASH_PRIME   16777619U


/* ==================================================
 * Constructores
 * ================================================== */

chorda
chorda_ex_literas (
    constans i8* cstr)
{
    si (!cstr)
    {
        redde (chorda){ ZEPHYRUM, NIHIL };
    }

    redde (chorda){
        strlen(cstr),
        (i8*)cstr
    }
}


chorda
chorda_ex_buffer (
       constans i8* buffer,
    memoriae_index  len)
{
    si (!buffer)
    {
        redde (chorda){ ZEPHYRUM, NIHIL };
    }

    redde (chorda){
        len,
        (i8*)buffer
    }
}


chorda
chorda_sectio (
            chorda s,
    memoriae_index initium, 
    memoriae_index finis)
{
    si (!s.data || initium > finis || finis > s.len)
    {
        redde (chorda){ ZEPHYRUM, NIHIL };
    }

    redde (chorda){
        finis - initium,
        s.data + initium
    }
}

chorda
chorda_transcribere (
     chorda  s,
    Piscina* piscina)
{
    si (!piscina || !s.data || s.len == ZEPHYRUM)
    {
        redde (chorda){ ZEPHYRUM, NIHIL };
    }

    i8* allocatus = (i8*)piscina_allocare(piscina, s.len + I);
    si (!allocatus)
    {
        redde (chorda){ ZEPHYRUM, NIHIL };
    }

    memcpy(allocatus, s.data, s.len);
    allocatus[s.len] = '\0';

    redde (chorda){
        s.len,
        allocatus
    };
}

/* ==================================================
 * Comparatio
 * ================================================== */


b32 
chorda_aequalis (
    chorda a,
    chorda b)
{
    si (a.len != b.len)
    {
        redde FALSUM;
    }

    si (a.data == b.data)
    {
        redde VERUM;
    }

    redde memcmp(a.data, b.data, a.len) == ZEPHYRUM;
}
 
b32
chorda_aequalis_case_insensitivus (
    chorda a,
    chorda b)
{
    si (a.len != b.len)
    {
        redde FALSUM;
    }

    per (memoriae_index i = ZEPHYRUM; i < a.len; i++)
    {
        si (tolower((i8)a.data[i]) != tolower((i8)b.data[i]))
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

s32
chorda_comparare (
    chorda a,
    chorda b)
{
    memoriae_index min_len = a.len < b.len ? a.len : b.len;
    s32 cmp_result = memcmp(a.data, b.data, min_len);

    si (cmp_result != ZEPHYRUM)
    {
        redde cmp_result;
    }

    si (a.len < b.len) redde -I;
    si (a.len > b.len) redde I;

    redde ZEPHYRUM;
}

/* ==================================================
 * Quaestio
 * ================================================== */

b32
chorda_continet (
    chorda hay,
    chorda needle)
{
    si (!hay.data || !needle.data || needle.len > hay.len)
    {
        redde FALSUM;
    }

    si (needle.len == ZEPHYRUM)
    {
        redde VERUM;
    }

    per (memoriae_index i = ZEPHYRUM; i <= hay.len - needle.len; i++)
    {
        si (memcmp(hay.data + i, needle.data, needle.len) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}


b32
chorda_incipit (
    chorda s,
    chorda prefixum)
{
    si (!s.data || !prefixum.data || prefixum.len > s.len)
    {
        redde FALSUM;
    }

    si (prefixum.len == ZEPHYRUM)
    {
        redde VERUM;
    }

    redde memcmp(s.data, prefixum.data, prefixum.len) == ZEPHYRUM;
}



b32
chorda_terminatur (
    chorda s,
    chorda suffixum)
{
    si (!s.data || !suffixum.data || suffixum.len > s.len)
    {
        redde FALSUM;
    }

    si (suffixum.len == ZEPHYRUM)
    {
        redde VERUM;
    }

    memoriae_index offset = s.len - suffixum.len;
    redde memcmp(s.data + offset, suffixum.data, suffixum.len) == ZEPHYRUM;
}
   

i8*
chorda_invenire (
    chorda hay,
    chorda needle)
{
    si (!hay.data || !needle.data || needle.len > hay.len)
    {
        redde NIHIL;
    }

    si (needle.len == ZEPHYRUM)
    {
        redde hay.data;
    }

    per (memoriae_index i = ZEPHYRUM; i <= hay.len - needle.len; i++)
    {
        si (memcmp(hay.data + i, needle.data, needle.len) == ZEPHYRUM)
        {
            redde hay.data + i;
        }
    }

    redde NIHIL;
}


memoriae_index
chorda_numerare_occurentia (
    chorda hay,
    chorda needle)
{
    si (!hay.data || !needle.data || needle.len > hay.len || needle.len == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    memoriae_index count    = ZEPHYRUM;
    memoriae_index positus  = ZEPHYRUM;

    dum (positus <= hay.len - needle.len)
    {
        si (memcmp(hay.data + positus, needle.data, needle.len) == ZEPHYRUM)
        {
            count++;
            positus += needle.len;
        }
        alioquin
        {
            positus++;
        }
    }

    redde count;
}

/* ==================================================
 * Manipulatio
 * ================================================== */


chorda
chorda_praecidere (
    chorda s)
{
    si (!s.data || s.len == ZEPHYRUM)
    {
        redde s;
    }

    memoriae_index initium = ZEPHYRUM;
    memoriae_index finis = s.len;

    /* Trim start */
    dum (initium < s.len && isspace((i8)s.data[initium]))
    {
        initium++;
    }

    /* Trim end */
    dum (finis > initium && isspace((i8)s.data[finis - I]))
    {
        finis--;
    }

    redde chorda_sectio(s, initium, finis);
}


chorda 
chorda_minuscula (
     chorda  s,
    Piscina* piscina)
{
    si (!piscina || !s.data || s.len == ZEPHYRUM)
    {
        redde (chorda){ ZEPHYRUM, NIHIL };
    }

    i8* allocatus = (i8*)piscina_allocare(piscina, s.len + I);
    si (!allocatus)
    {
        redde (chorda){ ZEPHYRUM, NIHIL };
    }

    per (memoriae_index i = ZEPHYRUM; i < s.len; i++)
    {
        allocatus[i] = (i8)tolower((i8)s.data[i]);
    }
    allocatus[s.len] = '\0';

    redde (chorda){
        s.len,
        allocatus
    };
}


chorda
chorda_maiuscula (
     chorda  s,
    Piscina* piscina)
{
    si (!piscina || !s.data || s.len == ZEPHYRUM)
    {
        redde (chorda){ ZEPHYRUM, NIHIL };
    }

    i8* allocatus = (i8*)piscina_allocare(piscina, s.len + I);
    si (!allocatus)
    {
        redde (chorda){ ZEPHYRUM, NIHIL };
    }

    per (memoriae_index i = ZEPHYRUM; i < s.len; i++)
    {
        allocatus[i] = toupper((i8)s.data[i]);
    }
    allocatus[s.len] = '\0';

    redde (chorda){
        s.len,
        allocatus
    };
}

/* ==================================================
 * Convenientia
 * ================================================== */


i8*
chorda_ut_cstr (
     chorda  s,
    Piscina* piscina)
{
    si (!s.data)
    {
        redde NIHIL;
    }

    /* Check if already null-terminated */
    si (s.len > ZEPHYRUM && s.data[s.len] == '\0')
    {
        redde s.data;
    }

    i8* allocatus = (i8*)piscina_allocare(piscina, s.len + I);
    si (!allocatus)
    {
        redde NIHIL;
    }

    memcpy(allocatus, s.data, s.len);
    allocatus[s.len] = '\0';

    redde allocatus;
}

b32
chorda_ut_integer (
    chorda  s,
       s32* output)
{
    si (!s.data || !output || s.len == ZEPHYRUM)
    {
        redde FALSUM;
    }

    i8* cstr_temporalis = (i8*)memoriae_allocare(s.len + I);
    si (!cstr_temporalis)
    {
        redde FALSUM;
    }

    memcpy(cstr_temporalis, s.data, s.len);
    cstr_temporalis[s.len] = '\0';

    i8* terminus;
    s32 valor = strtol(cstr_temporalis, &terminus, X);

    liberare(cstr_temporalis);

    si (terminus != cstr_temporalis && *terminus == '\0')
    {
        *output = valor;
        redde VERUM;
    }

    redde FALSUM;
}


b32
chorda_ut_size (
            chorda  s,
    memoriae_index* output)
{
    si (!s.data || !output || s.len == ZEPHYRUM)
    {
        redde FALSUM;
    }

    i8* cstr_temporalis = (i8*)memoriae_allocare(s.len + I);
    si (!cstr_temporalis)
    {
        redde FALSUM;
    }

    memcpy(cstr_temporalis, s.data, s.len);
    cstr_temporalis[s.len] = '\0';

    i8* terminus;
    memoriae_index valor = strtoul(cstr_temporalis, &terminus, X);

    liberare(cstr_temporalis);

    /* Check if entire string was parsed */
    si (terminus != cstr_temporalis && *terminus == '\0')
    {
        *output = valor;
        redde VERUM;
    }

    redde FALSUM;
}


/* ==================================================
 * HASHING
 * ================================================== */

memoriae_index 
chorda_hash (
    chorda s)
{
    /* FNV-1a hash algorithm */
    memoriae_index hash = CHORDA_HASH_OFFSET;

    per (memoriae_index i = ZEPHYRUM; i < s.len; i++)
    {
        hash ^= (i8)s.data[i];
        hash *= CHORDA_HASH_PRIME;
    }

    redde hash;
}






