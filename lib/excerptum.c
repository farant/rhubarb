/* excerptum.c - Vide excerptum.h. */

#include "excerptum.h"
#include "utf8.h"

/* figurae lineae, ordine INVERSO (unitates primae); reddit numerum */
interior i32
_figurae (
                  i32  linea,
            character* figurae)
{
    i32 n      = ZEPHYRUM;
    i32 valor  = linea;

    fac
    {
        figurae[n]  = (character)('0' + (valor % X));
        valor       = valor / X;
        n++;
    } dum (valor > ZEPHYRUM && n < XV);
    redde n;
}

/* latitudo marginis unius lineae: figurae, minimum IV */
interior i32
_latitudo (
    i32 linea)
{
    character figurae[XVI];
          i32 n = _figurae(linea, figurae);

    redde n < IV ? (i32)IV : n;
}

/* margo: figurae (aut spatia) ad latitudinem DATAM, deinde " | ".
 * Latitudo ab extra venit ut saeptum sedium multiplicium unam pro
 * omnibus lineis habeat. */
interior vacuum
_marginem (
    ChordaAedificator* exitus,
                  i32  linea,
                  b32  numerus,
                  i32  latitudo)
{
    character figurae[XVI];
          i32 n = _figurae(linea, figurae);
          i32 k;

    /* i32 INSIGNATUS est: latitudo minor quam n hic sine custode
     * subtractionem circumvolveret */
    si (latitudo < n)
    {
        latitudo = n;
    }
    per (k = ZEPHYRUM; k < latitudo - (numerus ? n : ZEPHYRUM); k++)
    {
        chorda_aedificator_appendere_character(exitus, ' ');
    }
    si (numerus)
    {
        per (k = n; k > ZEPHYRUM; k--)
        {
            chorda_aedificator_appendere_character(exitus,
                figurae[k - I]);
        }
    }
    chorda_aedificator_appendere_literis(exitus, " | ");
}

/* fines lineae quae octetum 'initium' continet; CR ultimo sublato */
interior vacuum
_lineae_fines (
    constans character* fons,
                   i32  mensura,
                   s32  initium,
                   s32* principium,
                   s32* terminus)
{
    s32 p = initium;
    s32 t = initium;

    dum (p > ZEPHYRUM && fons[p - I] != '\n')
    {
        p--;
    }
    dum (t < (s32)mensura && fons[t] != '\n')
    {
        t++;
    }
    si (t > p && fons[t - I] == '\r')
    {
        t--;
    }
    *principium  = p;
    *terminus    = t;
}

/* linea fontis ipsa: moderatores praeter tabulam ut spatium */
interior vacuum
_lineam_scribere (
     ChordaAedificator* exitus,
    constans character* fons,
                   s32  principium,
                   s32  terminus,
                   i32  linea,
                   i32  latitudo)
{
    s32 i;

    _marginem(exitus, linea, VERUM, latitudo);
    per (i = principium; i < terminus; i++)
    {
        character c = fons[i];

        si (c != '\t' && (i32)(insignatus character)c < 0x20)
        {
            c = ' ';
        }
        chorda_aedificator_appendere_character(exitus, c);
    }
    chorda_aedificator_appendere_character(exitus, '\n');
}

/* signum sub linea: '^' ad initium, '~' per CHARACTEREM UTF-8 usque
 * ad finem tractus aut lineae; tabula in margine servatur, ergo
 * columna vera manet. Nota (si adest) post signum. */
interior vacuum
_signum_scribere (
     ChordaAedificator* exitus,
    constans character* fons,
                   s32  principium,
                   s32  terminus,
                   s32  initium,
                   s32  finis,
                   i32  linea,
                   i32  latitudo,
    constans character* nota)
{
    s32 finis_lineae;
    s32 i;

    _marginem(exitus, linea, FALSUM, latitudo);
    per (i = principium; i < initium && i < terminus; i++)
    {
        si (fons[i] == '\t')
        {
            chorda_aedificator_appendere_character(exitus, '\t');
        }
        alioquin si (!utf8_est_continuatio((i8)fons[i]))
        {
            chorda_aedificator_appendere_character(exitus, ' ');
        }
    }
    chorda_aedificator_appendere_character(exitus, '^');
    finis_lineae  = finis < terminus ? finis : terminus;
    i             = initium;
    si (i < finis_lineae)
    {
        i++;
        dum (i < finis_lineae && utf8_est_continuatio((i8)fons[i]))
        {
            i++;
        }
    }
    per (; i < finis_lineae; i++)
    {
        si (!utf8_est_continuatio((i8)fons[i]))
        {
            chorda_aedificator_appendere_character(exitus, '~');
        }
    }
    si (nota != NIHIL)
    {
        chorda_aedificator_appendere_character(exitus, ' ');
        chorda_aedificator_appendere_literis(exitus, nota);
    }
    chorda_aedificator_appendere_character(exitus, '\n');
}

b32
excerptum_scribere (
     ChordaAedificator* exitus,
    constans character* fons,
                   i32  mensura,
                   s32  initium,
                   s32  finis,
                   i32  linea)
{
    s32 principium;
    s32 terminus;
    i32 latitudo;

    si (   exitus == NIHIL || fons == NIHIL || linea == ZEPHYRUM
        || initium < ZEPHYRUM || initium > (s32)mensura)
    {
        redde FALSUM;
    }
    si (finis < initium)
    {
        finis = initium;
    }
    latitudo = _latitudo(linea);
    _lineae_fines(fons, mensura, initium, &principium, &terminus);
    _lineam_scribere(exitus, fons, principium, terminus, linea,
        latitudo);
    _signum_scribere(exitus, fons, principium, terminus, initium,
        finis, linea, latitudo, NIHIL);
    redde VERUM;
}

b32
excerptum_scribere_multa (
          ChordaAedificator* exitus,
         constans character* fons,
                        i32  mensura,
    constans ExcerptumSedes* sedes,
                        i32  numerus)
{
    i32 latitudo = (i32)IV;
    i32 k;

    si (   exitus  == NIHIL || fons == NIHIL || sedes == NIHIL
        || numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        i32 l;

        si (   sedes[k].linea == ZEPHYRUM
            || sedes[k].initium < ZEPHYRUM
            || sedes[k].initium > (s32)mensura)
        {
            redde FALSUM;
        }
        si (   k > ZEPHYRUM
            && sedes[k].initium < sedes[k - I].initium)
        {
            redde FALSUM;
        }
        l = _latitudo(sedes[k].linea);
        si (l > latitudo)
        {
            latitudo = l;
        }
    }
    k = ZEPHYRUM;
    dum (k < numerus)
    {
        s32 principium;
        s32 terminus;
        i32 j;

        _lineae_fines(fons, mensura, sedes[k].initium, &principium,
            &terminus);
        _lineam_scribere(exitus, fons, principium, terminus,
            sedes[k].linea, latitudo);
        j = k;
        dum (j < numerus && sedes[j].linea == sedes[k].linea)
        {
            s32 finis = sedes[j].finis < sedes[j].initium
                ? sedes[j].initium : sedes[j].finis;

            _signum_scribere(exitus, fons, principium, terminus,
                sedes[j].initium, finis, sedes[j].linea, latitudo,
                sedes[j].nota);
            j++;
        }
        k = j;
    }
    redde VERUM;
}
