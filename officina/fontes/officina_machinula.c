/* officina_machinula.c - Machinula: interpres medullae (M2b)
 *
 * Ansa dispensationis: commutatio in operatione; verba registrorum
 * 64-bit; CANONICUM-IN-SCRIPTURA (vide caput). Arithmetica integralis
 * in i64 INSIGNATO fit (involutio definita), tum canonicatur;
 * divisio/residuum signum discernunt (divisio per zephyrum = halitus
 * honestus). Gyrus dexter signatus arithmeticus est (arm64/clang;
 * C89 implementation-defined - electio notata).
 *
 * Tabulata (frames) in area stivae regionis: [registra][arcae...];
 * cursor per tabulatum restauratur in redde. Probatio spatii in
 * vocare ET in arca.
 *
 * Aedificata: tabula statica; ligatio in creare (decipulae notae ->
 * AEDIFICATUM in situ); CANALIS UNUS _aedificatum_vocare (sedes
 * memoriae/reddendi v2). Formator: ambulatio directivarum, quaeque
 * per snprintf hospitis emittitur (%s manualiter - sine limite
 * longitudinis); %.*s / latitudo * sustentae.
 */

#include "officina_machinula.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>

#define ANULUS_MENSURA        4096
#define ANULUS_LARVA          (ANULUS_MENSURA - 1)
#define ARGUMENTA_MAXIMA      32
#define STIVA_MARGO           256   /* octeti custodiae in vocare */

/* ==================================================
 * Typi interni
 * ================================================== */

nomen structura {
    constans MedullaFunctio* functio;
    s32            modulus_index;
    s32            bloccus;
    i32            instructio;      /* proxima exsecutanda */
    i64*           registra;
    memoriae_index basis_stivae;    /* cursor restaurandus in redde */
    s32            destinatio_vocantis;   /* registrum vocantis; -I */
} Tabulatum;

nomen structura {
    constans MedullaFunctio* functio;
    s32 modulus_index;
    s32 bloccus;
    i32 instructio;
} AnulusFigura;

nomen b32 (*MachinulaPons)(Machinula* machinula,
    constans i64* argumenta, s32 numerus, i64* fructus);

structura Machinula {
    Piscina*  piscina;
    Conexio*  conexio;
    Regio*    regio;
    Xar*      tabulata;             /* Tabulatum valore */
    Xar*      lineae_modulorum;     /* MedullaLineae* (NIHIL licet) */
    i8*       stiva_basis;
    memoriae_index stiva_magnitudo;
    memoriae_index stiva_cursor;
    /* halitus */
    b32       currens;
    s32       halitus_genus;
    s64       halitus_codex;
    chorda    halitus_nuntius;
    /* anulus volatus (SEMPER activus) */
    AnulusFigura anulus[ANULUS_MENSURA];
    i64       anulus_cursor;
    /* numeratores */
    i64       numeri_op[MEDULLA_OP_NUMERUS];
    i64       summa_instructionum;
    i64       numerus_vocationum;
    i64       numerus_aedificatorum;
    memoriae_index apex_stivae;
    /* cellae externae captae */
    s64*      cella_errno;          /* NIHIL licet */
};

/* ==================================================
 * Canonicum + figurae fluitantes
 * ================================================== */

interior i64
_canonicum (i64 verbum, s32 typus)
{
    commutatio (typus)
    {
    casus MEDULLA_TYPUS_I8:
        redde verbum & (i64)0xFF;
    casus MEDULLA_TYPUS_I16:
        redde verbum & (i64)0xFFFF;
    casus MEDULLA_TYPUS_I32:
    casus MEDULLA_TYPUS_F32:
        redde verbum & (i64)0xFFFFFFFF;
    casus MEDULLA_TYPUS_S8:
        verbum &= (i64)0xFF;
        redde (verbum & (i64)0x80) ? (verbum | ~(i64)0xFF) : verbum;
    casus MEDULLA_TYPUS_S16:
        verbum &= (i64)0xFFFF;
        redde (verbum & (i64)0x8000)
            ? (verbum | ~(i64)0xFFFF) : verbum;
    casus MEDULLA_TYPUS_S32:
        verbum &= (i64)0xFFFFFFFF;
        redde (verbum & (i64)0x80000000)
            ? (verbum | ~(i64)0xFFFFFFFF) : verbum;
    ordinarius:
        redde verbum;   /* i64/s64/f64/nihil */
    }
}

interior f64
_f64_de_verbo (i64 verbum)
{
    unio { i64 v; f64 f; } u;

    u.v = verbum;
    redde u.f;
}

interior i64
_verbum_de_f64 (f64 fluitans_valor)
{
    unio { i64 v; f64 f; } u;

    u.f = fluitans_valor;
    redde u.v;
}

interior f32
_f32_de_verbo (i64 verbum)
{
    unio { i32 v; f32 f; } u;

    u.v = (i32)(verbum & (i64)0xFFFFFFFF);
    redde u.f;
}

interior i64
_verbum_de_f32 (f32 fluitans_valor)
{
    unio { i32 v; f32 f; } u;

    u.f = fluitans_valor;
    redde (i64)u.v;
}

interior b32
_typus_signatus (s32 typus)
{
    redde (b32)(typus >= MEDULLA_TYPUS_S8
        && typus <= MEDULLA_TYPUS_S64);
}

interior s32
_latitudo_typi (s32 typus)
{
    commutatio (typus)
    {
    casus MEDULLA_TYPUS_I8:
    casus MEDULLA_TYPUS_S8:
        redde 8;
    casus MEDULLA_TYPUS_I16:
    casus MEDULLA_TYPUS_S16:
        redde 16;
    casus MEDULLA_TYPUS_I32:
    casus MEDULLA_TYPUS_S32:
    casus MEDULLA_TYPUS_F32:
        redde 32;
    ordinarius:
        redde 64;
    }
}

/* ==================================================
 * Halitus + relatio
 * ================================================== */

interior vacuum
_halitum_ponere (Machinula* m, s32 genus, s64 codex,
    constans character* nuntius)
{
    m->currens = FALSUM;
    m->halitus_genus = genus;
    m->halitus_codex = codex;
    si (nuntius != NIHIL)
    {
        m->halitus_nuntius = chorda_ex_literis(nuntius, m->piscina);
    }
}

interior vacuum
_vitium (Machinula* m, constans character* nuntius)
{
    _halitum_ponere(m, MACHINULA_VITIUM, I, nuntius);
}

interior constans MedullaModulus*
_modulum (constans Machinula* m, s32 modulus_index)
{
    redde conexio_modulum_obtinere(m->conexio, modulus_index);
}

interior vacuum
_positionem_imprimere (constans Machinula* m,
    constans MedullaFunctio* functio, s32 modulus_index,
    s32 bloccus_index, i32 instructio_index)
{
    constans MedullaBloccus* bloccus;
    constans MedullaInstructio* instructio = NIHIL;

    fprintf(stderr, "    %.*s",
        (int)functio->titulus.mensura,
        (constans character*)functio->titulus.datum);
    bloccus = medulla_bloccum_obtinere(functio, bloccus_index);
    si (bloccus != NIHIL)
    {
        fprintf(stderr, " @%.*s+%d",
            (int)bloccus->titulus.mensura,
            (constans character*)bloccus->titulus.datum,
            (int)instructio_index);
        instructio = (constans MedullaInstructio*)xar_obtinere_s(
            bloccus->instructiones, (s32)instructio_index);
    }
    si (instructio != NIHIL && instructio->origo != NIHIL
        && m->lineae_modulorum != NIHIL)
    {
        MedullaLineae** lineae = (MedullaLineae**)xar_obtinere_s(
            m->lineae_modulorum, modulus_index);
        chorda via;
        i32 linea = ZEPHYRUM;

        si (lineae != NIHIL && *lineae != NIHIL
            && medulla_lineam_quaerere(*lineae, instructio->origo,
                   &via, &linea))
        {
            fprintf(stderr, "  [%.*s:%u]", (int)via.mensura,
                (constans character*)via.datum,
                (insignatus integer)linea);
        }
    }
    fprintf(stderr, "\n");
}

/* relatio halitus (recordator volatus): positio + stiva vocationum
 * + cauda anuli - ad stderr (ruinas superstat) */
interior vacuum
_relationem_imprimere (constans Machinula* m)
{
    constans character* genera[] = {
        "BENE", "SISTERE", "DECIPULA", "VITIUM"
    };
    i32 numerus_tabulatorum = xar_numerus(m->tabulata);
    i32 i;

    fprintf(stderr, "\n=== MACHINULA HALITUS: %s ===\n",
        genera[m->halitus_genus]);
    si (m->halitus_nuntius.mensura > ZEPHYRUM)
    {
        fprintf(stderr, "nuntius: %.*s\n",
            (int)m->halitus_nuntius.mensura,
            (constans character*)m->halitus_nuntius.datum);
    }
    fprintf(stderr, "stiva vocationum (%ld tabulata, cacumen "
        "primum):\n", (long)numerus_tabulatorum);
    per (i = (i32)numerus_tabulatorum; i > ZEPHYRUM; i--)
    {
        constans Tabulatum* t = (constans Tabulatum*)xar_obtinere(
            m->tabulata, i - I);

        si ((i32)numerus_tabulatorum - i >= XXXII)
        {
            fprintf(stderr, "    ... (%ld reliqua)\n", (long)i);
            frange;
        }
        _positionem_imprimere(m, t->functio, t->modulus_index,
            t->bloccus, t->instructio);
    }
    fprintf(stderr, "cauda anuli (novissima prima):\n");
    per (i = ZEPHYRUM; i < XVI; i++)
    {
        i64 index = m->anulus_cursor - I - (i64)i;
        constans AnulusFigura* figura;

        si ((s64)index < ZEPHYRUM)
        {
            frange;
        }
        figura = &m->anulus[index & (i64)ANULUS_LARVA];
        si (figura->functio == NIHIL)
        {
            frange;
        }
        _positionem_imprimere(m, figura->functio,
            figura->modulus_index, figura->bloccus,
            figura->instructio);
    }
    fprintf(stderr, "instructiones exsecutae: %llu\n",
        (insignatus longus longus)m->summa_instructionum);
}

/* ==================================================
 * Formator (pons printf-familiae)
 * ================================================== */

nomen structura {
    i8*            regio_buf;   /* NIHIL -> fluxus */
    memoriae_index cap;
    memoriae_index scriptum;    /* longitudo vera (etiam ultra cap) */
    FILE*          fluxus;
} Formatio;

interior vacuum
_f_emittere (Formatio* f, constans character* octeti,
    memoriae_index numerus)
{
    si (f->fluxus != NIHIL)
    {
        (vacuum)fwrite(octeti, I, numerus, f->fluxus);
    }
    alioquin si (f->regio_buf != NIHIL && f->scriptum < f->cap)
    {
        memoriae_index spatium = f->cap - f->scriptum;
        memoriae_index n = (numerus < spatium) ? numerus : spatium;

        memcpy(f->regio_buf + f->scriptum, octeti, n);
    }
    f->scriptum += numerus;
}

/* -I = vitium (halitus iam positus) */
interior s64
_formare (Machinula* m, Formatio* f, constans character* forma,
    constans i64* argumenta, s32 numerus)
{
    memoriae_index i = ZEPHYRUM;
    memoriae_index longitudo_formae = strlen(forma);
    s32 arg = ZEPHYRUM;
    character dirbuf[XLVIII];
    character tempus[DXII];

    dum (i < longitudo_formae)
    {
        /* cursus litteralis */
        memoriae_index initium = i;

        dum (i < longitudo_formae && forma[i] != '%')
        {
            i++;
        }
        si (i > initium)
        {
            _f_emittere(f, forma + initium, i - initium);
        }
        si (i >= longitudo_formae)
        {
            frange;
        }
        /* directiva */
        {
            i32 d = ZEPHYRUM;
            character conversio;
            character longitudo_mod = '\0';
            b32 longitudo_ll = FALSUM;
            b32 sinistra = FALSUM;
            s32 latitudo = -I;
            s32 praecisio = -I;

            dirbuf[d] = '%';
            d++;
            i++;   /* post % */
            /* vexilla */
            dum (i < longitudo_formae && (forma[i] == '-'
                || forma[i] == '+' || forma[i] == ' '
                || forma[i] == '0' || forma[i] == '#'))
            {
                si (forma[i] == '-')
                {
                    sinistra = VERUM;
                }
                si (d < (i32)XL)
                {
                    dirbuf[d] = forma[i];
                    d++;
                }
                i++;
            }
            /* latitudo */
            si (i < longitudo_formae && forma[i] == '*')
            {
                si (arg >= numerus)
                {
                    _vitium(m, "forma: argumenta exhausta");
                    redde -I;
                }
                latitudo = (s32)_canonicum(argumenta[arg],
                    MEDULLA_TYPUS_S32);
                arg++;
                si (latitudo < ZEPHYRUM)
                {
                    sinistra = VERUM;
                    latitudo = -latitudo;
                }
                d += (i32)sprintf(dirbuf + d, "%s%d",
                    sinistra ? "-" : "", (int)latitudo);
                i++;
            }
            alioquin
            {
                dum (i < longitudo_formae && forma[i] >= '0'
                    && forma[i] <= '9')
                {
                    latitudo = ((latitudo < ZEPHYRUM)
                        ? ZEPHYRUM : latitudo * X)
                        + (s32)(forma[i] - '0');
                    si (d < (i32)XL)
                    {
                        dirbuf[d] = forma[i];
                        d++;
                    }
                    i++;
                }
            }
            /* praecisio */
            si (i < longitudo_formae && forma[i] == '.')
            {
                i++;
                praecisio = ZEPHYRUM;
                si (i < longitudo_formae && forma[i] == '*')
                {
                    si (arg >= numerus)
                    {
                        _vitium(m, "forma: argumenta exhausta");
                        redde -I;
                    }
                    praecisio = (s32)_canonicum(argumenta[arg],
                        MEDULLA_TYPUS_S32);
                    arg++;
                    si (praecisio < ZEPHYRUM)
                    {
                        praecisio = -I;
                    }
                    i++;
                }
                alioquin
                {
                    dum (i < longitudo_formae && forma[i] >= '0'
                        && forma[i] <= '9')
                    {
                        praecisio = praecisio * X
                            + (s32)(forma[i] - '0');
                        i++;
                    }
                }
                si (praecisio >= ZEPHYRUM)
                {
                    d += (i32)sprintf(dirbuf + d, ".%d",
                        (int)praecisio);
                }
            }
            /* modificator longitudinis */
            dum (i < longitudo_formae && (forma[i] == 'l'
                || forma[i] == 'h' || forma[i] == 'z'))
            {
                si (forma[i] == 'l' && longitudo_mod == 'l')
                {
                    longitudo_ll = VERUM;
                }
                longitudo_mod = forma[i];
                i++;
            }
            si (i >= longitudo_formae)
            {
                _vitium(m, "forma trunca");
                redde -I;
            }
            conversio = forma[i];
            i++;

            si (conversio == '%')
            {
                _f_emittere(f, "%", I);
                perge;
            }
            si (conversio == 's')
            {
                constans character* littera;
                memoriae_index longitudo_litterae;

                si (arg >= numerus)
                {
                    _vitium(m, "forma: argumenta exhausta");
                    redde -I;
                }
                littera = (constans character*)(memoriae_index)
                    argumenta[arg];
                arg++;
                si (littera == NIHIL)
                {
                    littera = "(nihil)";
                }
                longitudo_litterae = strlen(littera);
                si (praecisio >= ZEPHYRUM
                    && (memoriae_index)praecisio
                        < longitudo_litterae)
                {
                    longitudo_litterae = (memoriae_index)praecisio;
                }
                si (latitudo > ZEPHYRUM && !sinistra
                    && (memoriae_index)latitudo
                        > longitudo_litterae)
                {
                    memoriae_index k = (memoriae_index)latitudo
                        - longitudo_litterae;

                    dum (k > ZEPHYRUM)
                    {
                        _f_emittere(f, " ", I);
                        k--;
                    }
                }
                _f_emittere(f, littera, longitudo_litterae);
                si (latitudo > ZEPHYRUM && sinistra
                    && (memoriae_index)latitudo
                        > longitudo_litterae)
                {
                    memoriae_index k = (memoriae_index)latitudo
                        - longitudo_litterae;

                    dum (k > ZEPHYRUM)
                    {
                        _f_emittere(f, " ", I);
                        k--;
                    }
                }
                perge;
            }
            /* conversiones numericae: per snprintf hospitis */
            {
                i64 verbum = ZEPHYRUM;
                int n = ZEPHYRUM;
                b32 sumit = (b32)(conversio != '%');

                si (sumit)
                {
                    si (arg >= numerus)
                    {
                        _vitium(m, "forma: argumenta exhausta");
                        redde -I;
                    }
                    verbum = argumenta[arg];
                    arg++;
                }
                /* modificator in dirbuf */
                si (longitudo_ll)
                {
                    dirbuf[d] = 'l';
                    d++;
                    dirbuf[d] = 'l';
                    d++;
                }
                alioquin si (longitudo_mod == 'l')
                {
                    dirbuf[d] = 'l';
                    d++;
                }
                alioquin si (longitudo_mod == 'z')
                {
                    dirbuf[d] = 'z';
                    d++;
                }
                dirbuf[d] = conversio;
                d++;
                dirbuf[d] = '\0';

                commutatio (conversio)
                {
                casus 'd':
                casus 'i':
                    si (longitudo_ll)
                    {
                        n = snprintf(tempus, DXII, dirbuf,
                            (signatus longus longus)(s64)verbum);
                    }
                    alioquin si (longitudo_mod == 'l')
                    {
                        n = snprintf(tempus, DXII, dirbuf,
                            (longus)(s64)verbum);
                    }
                    alioquin si (longitudo_mod == 'z')
                    {
                        n = snprintf(tempus, DXII, dirbuf,
                            (memoriae_index)verbum);
                    }
                    alioquin
                    {
                        n = snprintf(tempus, DXII, dirbuf,
                            (int)(s64)verbum);
                    }
                    frange;
                casus 'u':
                casus 'o':
                casus 'x':
                casus 'X':
                    si (longitudo_ll)
                    {
                        n = snprintf(tempus, DXII, dirbuf,
                            (insignatus longus longus)verbum);
                    }
                    alioquin si (longitudo_mod == 'l')
                    {
                        n = snprintf(tempus, DXII, dirbuf,
                            (insignatus longus)verbum);
                    }
                    alioquin si (longitudo_mod == 'z')
                    {
                        n = snprintf(tempus, DXII, dirbuf,
                            (memoriae_index)verbum);
                    }
                    alioquin
                    {
                        n = snprintf(tempus, DXII, dirbuf,
                            (insignatus integer)(verbum
                                & (i64)0xFFFFFFFF));
                    }
                    frange;
                casus 'c':
                    n = snprintf(tempus, DXII, dirbuf,
                        (int)(s64)_canonicum(verbum,
                            MEDULLA_TYPUS_S32));
                    frange;
                casus 'f':
                casus 'e':
                casus 'E':
                casus 'g':
                casus 'G':
                    n = snprintf(tempus, DXII, dirbuf,
                        _f64_de_verbo(verbum));
                    frange;
                casus 'p':
                    n = snprintf(tempus, DXII, dirbuf,
                        (vacuum*)(memoriae_index)verbum);
                    frange;
                ordinarius:
                    _vitium(m, "forma: conversio ignota");
                    redde -I;
                }
                si (n > ZEPHYRUM)
                {
                    _f_emittere(f, tempus,
                        (n < (int)(DXII - I))
                            ? (memoriae_index)n
                            : (memoriae_index)(DXII - I));
                }
            }
        }
    }
    /* NUL snprintf-modi */
    si (f->regio_buf != NIHIL && f->cap > ZEPHYRUM)
    {
        memoriae_index finis = (f->scriptum < f->cap - I)
            ? f->scriptum : f->cap - I;

        f->regio_buf[finis] = ZEPHYRUM;
    }
    redde (s64)f->scriptum;
}

/* ==================================================
 * Aedificata (pavimentum Undae 0)
 * ================================================== */

interior b32
_aed_malloc (Machinula* m, constans i64* argumenta, s32 numerus,
    i64* fructus)
{
    (vacuum)numerus;
    *fructus = (i64)(memoriae_index)regio_allocare(m->regio,
        (memoriae_index)argumenta[ZEPHYRUM]);
    redde VERUM;
}

interior b32
_aed_free (Machinula* m, constans i64* argumenta, s32 numerus,
    i64* fructus)
{
    (vacuum)numerus;
    *fructus = ZEPHYRUM;
    si (argumenta[ZEPHYRUM] == ZEPHYRUM)
    {
        redde VERUM;   /* free(NULL) licet */
    }
    si (!regio_liberare(m->regio,
            (vacuum*)(memoriae_index)argumenta[ZEPHYRUM]))
    {
        _vitium(m, "free: monstrator insanus");
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_aed_realloc (Machinula* m, constans i64* argumenta, s32 numerus,
    i64* fructus)
{
    (vacuum)numerus;
    *fructus = (i64)(memoriae_index)regio_reallocare(m->regio,
        (vacuum*)(memoriae_index)argumenta[ZEPHYRUM],
        (memoriae_index)argumenta[I]);
    redde VERUM;
}

interior b32
_aed_strlen (Machinula* m, constans i64* argumenta, s32 numerus,
    i64* fructus)
{
    (vacuum)m;
    (vacuum)numerus;
    *fructus = (i64)strlen(
        (constans character*)(memoriae_index)argumenta[ZEPHYRUM]);
    redde VERUM;
}

interior b32
_aed_strcpy (Machinula* m, constans i64* argumenta, s32 numerus,
    i64* fructus)
{
    (vacuum)m;
    (vacuum)numerus;
    strcpy((character*)(memoriae_index)argumenta[ZEPHYRUM],
        (constans character*)(memoriae_index)argumenta[I]);
    *fructus = argumenta[ZEPHYRUM];
    redde VERUM;
}

interior b32
_aed_memcpy (Machinula* m, constans i64* argumenta, s32 numerus,
    i64* fructus)
{
    (vacuum)m;
    (vacuum)numerus;
    memcpy((vacuum*)(memoriae_index)argumenta[ZEPHYRUM],
        (constans vacuum*)(memoriae_index)argumenta[I],
        (memoriae_index)argumenta[II]);
    *fructus = argumenta[ZEPHYRUM];
    redde VERUM;
}

interior b32
_aed_memset (Machinula* m, constans i64* argumenta, s32 numerus,
    i64* fructus)
{
    (vacuum)m;
    (vacuum)numerus;
    memset((vacuum*)(memoriae_index)argumenta[ZEPHYRUM],
        (int)(s64)_canonicum(argumenta[I], MEDULLA_TYPUS_S32),
        (memoriae_index)argumenta[II]);
    *fructus = argumenta[ZEPHYRUM];
    redde VERUM;
}

interior b32
_aed_memcmp (Machinula* m, constans i64* argumenta, s32 numerus,
    i64* fructus)
{
    (vacuum)m;
    (vacuum)numerus;
    *fructus = (i64)(s64)memcmp(
        (constans vacuum*)(memoriae_index)argumenta[ZEPHYRUM],
        (constans vacuum*)(memoriae_index)argumenta[I],
        (memoriae_index)argumenta[II]);
    redde VERUM;
}

interior b32
_aed_fabs (Machinula* m, constans i64* argumenta, s32 numerus,
    i64* fructus)
{
    f64 valor = _f64_de_verbo(argumenta[ZEPHYRUM]);

    (vacuum)m;
    (vacuum)numerus;
    *fructus = _verbum_de_f64((valor < 0.0) ? -valor : valor);
    redde VERUM;
}

interior b32
_aed_exit (Machinula* m, constans i64* argumenta, s32 numerus,
    i64* fructus)
{
    (vacuum)numerus;
    *fructus = ZEPHYRUM;
    _halitum_ponere(m, MACHINULA_BENE,
        (s64)_canonicum(argumenta[ZEPHYRUM], MEDULLA_TYPUS_S32),
        NIHIL);
    redde FALSUM;   /* ansa frangitur; BENE - sine relatione */
}

interior b32
_aed_printf (Machinula* m, constans i64* argumenta, s32 numerus,
    i64* fructus)
{
    Formatio f;
    s64 n;

    f.regio_buf = NIHIL;
    f.cap = ZEPHYRUM;
    f.scriptum = ZEPHYRUM;
    f.fluxus = stdout;
    n = _formare(m,&f,
        (constans character*)(memoriae_index)argumenta[ZEPHYRUM],
        argumenta + I, numerus - I);
    *fructus = (i64)n;
    redde (b32)(n >= ZEPHYRUM);
}

interior b32
_aed_fprintf (Machinula* m, constans i64* argumenta, s32 numerus,
    i64* fructus)
{
    Formatio f;
    s64 n;

    f.regio_buf = NIHIL;
    f.cap = ZEPHYRUM;
    f.scriptum = ZEPHYRUM;
    si (argumenta[ZEPHYRUM] == I)
    {
        f.fluxus = stdout;
    }
    alioquin si (argumenta[ZEPHYRUM] == II)
    {
        f.fluxus = stderr;
    }
    alioquin
    {
        _vitium(m, "fprintf: ansa ignota");
        redde FALSUM;
    }
    n = _formare(m, &f,
        (constans character*)(memoriae_index)argumenta[I],
        argumenta + II, numerus - II);
    *fructus = (i64)n;
    redde (b32)(n >= ZEPHYRUM);
}

interior b32
_aed_snprintf (Machinula* m, constans i64* argumenta, s32 numerus,
    i64* fructus)
{
    Formatio f;
    s64 n;

    f.regio_buf = (i8*)(memoriae_index)argumenta[ZEPHYRUM];
    f.cap = (memoriae_index)argumenta[I];
    f.scriptum = ZEPHYRUM;
    f.fluxus = NIHIL;
    n = _formare(m, &f,
        (constans character*)(memoriae_index)argumenta[II],
        argumenta + III, numerus - III);
    *fructus = (i64)n;
    redde (b32)(n >= ZEPHYRUM);
}

interior b32
_aed_sprintf (Machinula* m, constans i64* argumenta, s32 numerus,
    i64* fructus)
{
    Formatio f;
    s64 n;

    f.regio_buf = (i8*)(memoriae_index)argumenta[ZEPHYRUM];
    f.cap = (memoriae_index)0x7FFFFFFF;
    f.scriptum = ZEPHYRUM;
    f.fluxus = NIHIL;
    n = _formare(m, &f,
        (constans character*)(memoriae_index)argumenta[I],
        argumenta + II, numerus - II);
    *fructus = (i64)n;
    redde (b32)(n >= ZEPHYRUM);
}

/* scriptura-retro errno (interview Q1/M2b): post aedificata quae
 * errno hospitis ponunt, cella VM renovatur */
interior vacuum
_errno_retro (Machinula* m)
{
    si (m->cella_errno != NIHIL)
    {
        *m->cella_errno = (s64)errno;
    }
}

interior b32
_aed_strcmp (Machinula* m, constans i64* a, s32 n, i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = (i64)(s64)strcmp(
        (constans character*)(memoriae_index)a[ZEPHYRUM],
        (constans character*)(memoriae_index)a[I]);
    redde VERUM;
}

interior b32
_aed_strncmp (Machinula* m, constans i64* a, s32 n, i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = (i64)(s64)strncmp(
        (constans character*)(memoriae_index)a[ZEPHYRUM],
        (constans character*)(memoriae_index)a[I],
        (memoriae_index)a[II]);
    redde VERUM;
}

interior b32
_aed_strchr (Machinula* m, constans i64* a, s32 n, i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = (i64)(memoriae_index)strchr(
        (constans character*)(memoriae_index)a[ZEPHYRUM],
        (int)(s64)_canonicum(a[I], MEDULLA_TYPUS_S32));
    redde VERUM;
}

interior b32
_aed_strstr (Machinula* m, constans i64* a, s32 n, i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = (i64)(memoriae_index)strstr(
        (constans character*)(memoriae_index)a[ZEPHYRUM],
        (constans character*)(memoriae_index)a[I]);
    redde VERUM;
}

interior b32
_aed_strncpy (Machinula* m, constans i64* a, s32 n, i64* fr)
{
    (vacuum)m; (vacuum)n;
    strncpy((character*)(memoriae_index)a[ZEPHYRUM],
        (constans character*)(memoriae_index)a[I],
        (memoriae_index)a[II]);
    *fr = a[ZEPHYRUM];
    redde VERUM;
}

interior b32
_aed_memmove (Machinula* m, constans i64* a, s32 n, i64* fr)
{
    (vacuum)m; (vacuum)n;
    memmove((vacuum*)(memoriae_index)a[ZEPHYRUM],
        (constans vacuum*)(memoriae_index)a[I],
        (memoriae_index)a[II]);
    *fr = a[ZEPHYRUM];
    redde VERUM;
}

interior b32
_aed_atoi (Machinula* m, constans i64* a, s32 n, i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = (i64)(s64)atoi(
        (constans character*)(memoriae_index)a[ZEPHYRUM]);
    redde VERUM;
}

interior b32
_aed_atof (Machinula* m, constans i64* a, s32 n, i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = _verbum_de_f64(atof(
        (constans character*)(memoriae_index)a[ZEPHYRUM]));
    redde VERUM;
}

interior b32
_aed_abs (Machinula* m, constans i64* a, s32 n, i64* fr)
{
    s64 v = (s64)_canonicum(a[ZEPHYRUM], MEDULLA_TYPUS_S32);

    (vacuum)m; (vacuum)n;
    *fr = _canonicum((i64)((v < ZEPHYRUM) ? -v : v),
        MEDULLA_TYPUS_S32);
    redde VERUM;
}

interior b32
_aed_strtod (Machinula* m, constans i64* a, s32 n, i64* fr)
{
    character* finis = NIHIL;
    f64 v;

    (vacuum)n;
    errno = ZEPHYRUM;
    v = strtod((constans character*)(memoriae_index)a[ZEPHYRUM],
        &finis);
    si (a[I] != ZEPHYRUM)
    {
        i64 verbum = (i64)(memoriae_index)finis;

        memcpy((vacuum*)(memoriae_index)a[I], &verbum,
            magnitudo(i64));
    }
    _errno_retro(m);
    *fr = _verbum_de_f64(v);
    redde VERUM;
}

interior b32
_aed_strtol (Machinula* m, constans i64* a, s32 n, i64* fr)
{
    character* finis = NIHIL;
    longus v;

    (vacuum)n;
    errno = ZEPHYRUM;
    v = strtol((constans character*)(memoriae_index)a[ZEPHYRUM],
        &finis, (int)(s64)_canonicum(a[II], MEDULLA_TYPUS_S32));
    si (a[I] != ZEPHYRUM)
    {
        i64 verbum = (i64)(memoriae_index)finis;

        memcpy((vacuum*)(memoriae_index)a[I], &verbum,
            magnitudo(i64));
    }
    _errno_retro(m);
    *fr = (i64)(s64)v;
    redde VERUM;
}

interior b32
_aed_strtoul (Machinula* m, constans i64* a, s32 n, i64* fr)
{
    character* finis = NIHIL;
    insignatus longus v;

    (vacuum)n;
    errno = ZEPHYRUM;
    v = strtoul((constans character*)(memoriae_index)a[ZEPHYRUM],
        &finis, (int)(s64)_canonicum(a[II], MEDULLA_TYPUS_S32));
    si (a[I] != ZEPHYRUM)
    {
        i64 verbum = (i64)(memoriae_index)finis;

        memcpy((vacuum*)(memoriae_index)a[I], &verbum,
            magnitudo(i64));
    }
    _errno_retro(m);
    *fr = (i64)v;
    redde VERUM;
}

#define AED_CTYPE(TITULUS, FUNCTIO) \
interior b32 \
TITULUS (Machinula* m, constans i64* a, s32 n, i64* fr) \
{ \
    (vacuum)m; (vacuum)n; \
    *fr = (i64)(s64)FUNCTIO( \
        (int)(s64)_canonicum(a[ZEPHYRUM], MEDULLA_TYPUS_S32)); \
    redde VERUM; \
}

AED_CTYPE(_aed_isalpha, isalpha)
AED_CTYPE(_aed_isdigit, isdigit)
AED_CTYPE(_aed_islower, islower)
AED_CTYPE(_aed_isupper, isupper)
AED_CTYPE(_aed_isspace, isspace)
AED_CTYPE(_aed_iscntrl, iscntrl)
AED_CTYPE(_aed_tolower, tolower)
AED_CTYPE(_aed_toupper, toupper)

#define AED_MATH1(TITULUS, FUNCTIO) \
interior b32 \
TITULUS (Machinula* m, constans i64* a, s32 n, i64* fr) \
{ \
    (vacuum)m; (vacuum)n; \
    *fr = _verbum_de_f64(FUNCTIO(_f64_de_verbo(a[ZEPHYRUM]))); \
    redde VERUM; \
}

AED_MATH1(_aed_sin, sin)
AED_MATH1(_aed_cos, cos)
AED_MATH1(_aed_sqrt, sqrt)
AED_MATH1(_aed_floor, floor)

interior b32
_aed_pow (Machinula* m, constans i64* a, s32 n, i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = _verbum_de_f64(pow(_f64_de_verbo(a[ZEPHYRUM]),
        _f64_de_verbo(a[I])));
    redde VERUM;
}

interior b32
_aed_fmod (Machinula* m, constans i64* a, s32 n, i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = _verbum_de_f64(fmod(_f64_de_verbo(a[ZEPHYRUM]),
        _f64_de_verbo(a[I])));
    redde VERUM;
}

interior b32
_aed_ldexp (Machinula* m, constans i64* a, s32 n, i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = _verbum_de_f64(ldexp(_f64_de_verbo(a[ZEPHYRUM]),
        (int)(s64)_canonicum(a[I], MEDULLA_TYPUS_S32)));
    redde VERUM;
}

interior b32
_aed_fflush (Machinula* m, constans i64* a, s32 n, i64* fr)
{
    (vacuum)m;
    *fr = ZEPHYRUM;
    si (n < I || a[ZEPHYRUM] == ZEPHYRUM)
    {
        (vacuum)fflush(NIHIL);
    }
    alioquin si (a[ZEPHYRUM] == I)
    {
        (vacuum)fflush(stdout);
    }
    alioquin
    {
        (vacuum)fflush(stderr);
    }
    redde VERUM;
}

nomen structura {
    constans character* titulus;
    MachinulaPons       pons;
} AedificatumNota;

interior constans AedificatumNota AEDIFICATA[] = {
    { "malloc",   _aed_malloc },
    { "free",     _aed_free },
    { "realloc",  _aed_realloc },
    { "strlen",   _aed_strlen },
    { "strcpy",   _aed_strcpy },
    { "memcpy",   _aed_memcpy },
    { "memset",   _aed_memset },
    { "memcmp",   _aed_memcmp },
    { "fabs",     _aed_fabs },
    { "exit",     _aed_exit },
    { "printf",   _aed_printf },
    { "fprintf",  _aed_fprintf },
    { "snprintf", _aed_snprintf },
    { "sprintf",  _aed_sprintf },
    { "strcmp",   _aed_strcmp },
    { "strncmp",  _aed_strncmp },
    { "strchr",   _aed_strchr },
    { "strstr",   _aed_strstr },
    { "strncpy",  _aed_strncpy },
    { "memmove",  _aed_memmove },
    { "atoi",     _aed_atoi },
    { "atof",     _aed_atof },
    { "abs",      _aed_abs },
    { "strtod",   _aed_strtod },
    { "strtol",   _aed_strtol },
    { "strtoul",  _aed_strtoul },
    { "isalpha",  _aed_isalpha },
    { "isdigit",  _aed_isdigit },
    { "islower",  _aed_islower },
    { "isupper",  _aed_isupper },
    { "isspace",  _aed_isspace },
    { "iscntrl",  _aed_iscntrl },
    { "tolower",  _aed_tolower },
    { "toupper",  _aed_toupper },
    { "sin",      _aed_sin },
    { "cos",      _aed_cos },
    { "sqrt",     _aed_sqrt },
    { "floor",    _aed_floor },
    { "pow",      _aed_pow },
    { "fmod",     _aed_fmod },
    { "ldexp",    _aed_ldexp },
    { "fflush",   _aed_fflush }
};
#define AEDIFICATA_NUMERUS 44

/* CANALIS UNUS aedificatorum - sedes memoriae/reddendi v2 (Q12) */
interior b32
_aedificatum_vocare (Machinula* m, s64 index,
    constans i64* argumenta, s32 numerus, i64* fructus)
{
    m->numerus_aedificatorum += I;
    redde AEDIFICATA[index].pons(m, argumenta, numerus, fructus);
}

/* ==================================================
 * Vita + ligatio
 * ================================================== */

Machinula*
machinula_creare (Piscina* piscina, Conexio* conexio, Regio* regio)
{
    Machinula* m;
    i32 numerus_symbolorum;
    i32 numerus_modulorum;
    i32 i;

    si (piscina == NIHIL || conexio == NIHIL || regio == NIHIL)
    {
        redde NIHIL;
    }
    m = piscina_allocare(piscina, magnitudo(Machinula));
    si (m == NIHIL)
    {
        redde NIHIL;
    }
    memset(m, ZEPHYRUM, magnitudo(Machinula));
    m->piscina = piscina;
    m->conexio = conexio;
    m->regio = regio;
    m->tabulata = xar_creare(piscina, (i32)magnitudo(Tabulatum));
    m->lineae_modulorum = xar_creare(piscina,
        (i32)magnitudo(MedullaLineae*));
    m->stiva_basis = (i8*)regio_stiva_initium(regio);
    m->stiva_magnitudo = regio_stiva_magnitudo_octetorum(regio);
    si (m->tabulata == NIHIL || m->lineae_modulorum == NIHIL)
    {
        redde NIHIL;
    }
    numerus_modulorum = (i32)conexio_numerus_modulorum(conexio);
    per (i = ZEPHYRUM; i < numerus_modulorum; i++)
    {
        MedullaLineae** locellus = xar_addere(m->lineae_modulorum);

        si (locellus != NIHIL)
        {
            *locellus = NIHIL;
        }
    }

    /* ligatio: decipulae notae -> AEDIFICATUM (signum in situ) */
    numerus_symbolorum = (i32)conexio_numerus_symbolorum(conexio);
    per (i = ZEPHYRUM; i < numerus_symbolorum; i++)
    {
        constans ConexioSymbolum* symbolum =
            conexio_symbolum_obtinere(conexio, (s32)i);
        i32 a;

        si (symbolum->genus != CONEXIO_SYMBOLUM_DECIPULA)
        {
            perge;
        }
        per (a = ZEPHYRUM; a < (i32)AEDIFICATA_NUMERUS; a++)
        {
            si (chorda_aequalis_literis(symbolum->titulus,
                    AEDIFICATA[a].titulus))
            {
                ConexioDescriptor* descriptor =
                    (ConexioDescriptor*)symbolum->sedes;

                descriptor->signum = CONEXIO_SIGNUM_AEDIFICATUM;
                descriptor->index = (s64)a;
                frange;
            }
        }
    }

    /* cellae captae */
    {
        chorda titulus;
        unio { constans character* c; i8* m; } u;

        u.c = "errno";
        titulus.datum = u.m;
        titulus.mensura = V;
        m->cella_errno = conexio_sedes_quaerere(conexio, titulus);
    }
    redde m;
}

vacuum
machinula_lineas_praebere (Machinula* machinula, s32 modulus_index,
    constans MedullaLineae* lineae)
{
    MedullaLineae** locellus = xar_obtinere_s(
        machinula->lineae_modulorum, modulus_index);

    si (locellus != NIHIL)
    {
        unio { constans MedullaLineae* c; MedullaLineae* m; } u;

        u.c = lineae;
        *locellus = u.m;
    }
}

/* ==================================================
 * Tabulata
 * ================================================== */

interior b32
_tabulatum_impellere (Machinula* m,
    constans MedullaFunctio* functio, s32 modulus_index,
    s32 destinatio_vocantis, constans i64* argumenta, s32 numerus)
{
    Tabulatum* t;
    i32 numerus_registrorum = xar_numerus(functio->registra);
    memoriae_index opus = ((memoriae_index)numerus_registrorum
        * magnitudo(i64) + XV) & ~(memoriae_index)XV;
    i32 i;
    i32 numerus_parametrorum = xar_numerus(functio->parametra);

    si (m->stiva_cursor + opus + STIVA_MARGO > m->stiva_magnitudo)
    {
        _vitium(m, "stiva exhausta");
        redde FALSUM;
    }
    si ((i32)xar_numerus(m->tabulata) >= (i32)1048576)
    {
        _vitium(m, "stiva vocationum exhausta (recursio?)");
        redde FALSUM;
    }
    t = xar_addere(m->tabulata);
    si (t == NIHIL)
    {
        _vitium(m, "tabulata exhausta");
        redde FALSUM;
    }
    t->functio = functio;
    t->modulus_index = modulus_index;
    t->bloccus = ZEPHYRUM;
    t->instructio = ZEPHYRUM;
    t->registra = (i64*)(vacuum*)(m->stiva_basis + m->stiva_cursor);
    t->basis_stivae = m->stiva_cursor;
    t->destinatio_vocantis = destinatio_vocantis;
    m->stiva_cursor += opus;
    si (m->stiva_cursor > m->apex_stivae)
    {
        m->apex_stivae = m->stiva_cursor;
    }
    memset(t->registra, ZEPHYRUM,
        (memoriae_index)numerus_registrorum * magnitudo(i64));
    per (i = ZEPHYRUM; i < numerus_parametrorum
        && i < (i32)numerus; i++)
    {
        constans MedullaParametrum* parametrum =
            (constans MedullaParametrum*)xar_obtinere(
                functio->parametra, i);

        t->registra[parametrum->index] = _canonicum(argumenta[i],
            parametrum->typus);
    }
    m->numerus_vocationum += I;
    redde VERUM;
}

/* ==================================================
 * Exsecutio
 * ================================================== */

interior i64
_valor_operandi (Machinula* m, constans Tabulatum* t,
    constans MedullaOperandum* operandum, s32 typus)
{
    commutatio (operandum->genus)
    {
    casus MEDULLA_OPERANDUM_REGISTRUM:
        redde t->registra[operandum->datum.index];
    casus MEDULLA_OPERANDUM_IMMEDIATUM:
        redde _canonicum((i64)operandum->datum.immediatum, typus);
    casus MEDULLA_OPERANDUM_IMMEDIATUM_F:
        si (typus == MEDULLA_TYPUS_F32)
        {
            redde _verbum_de_f32(
                (f32)operandum->datum.immediatum_f);
        }
        redde _verbum_de_f64(operandum->datum.immediatum_f);
    casus MEDULLA_OPERANDUM_SYMBOLUM:
    {
        /* symbolum ut valor = inscriptio sedis (argumenta
         * vocationum manu scripta: $forma) */
        s32 globale = conexio_symbolum_globale(m->conexio,
            t->modulus_index, operandum->datum.index);

        si (globale < ZEPHYRUM)
        {
            _vitium(m, "operandum: symbolum insanum");
            redde ZEPHYRUM;
        }
        redde (i64)(memoriae_index)conexio_symbolum_obtinere(
            m->conexio, globale)->sedes;
    }
    ordinarius:
        _vitium(m, "operandum insanum");
        redde ZEPHYRUM;
    }
}

/* lectura canonica: suffixus operationis IPSE auctoritas est.
 * Operationes amputantes (addere/multiplicare/et...) registra
 * ut-sunt legere possunt (bits inferiores solum fluunt); operationes
 * latitudini sensibiles (comparationes, divisio, dextrorsum,
 * fluitare, redde) valorem ad latitudinem suffixi reducere DEBENT -
 * aliter semantica instructionis ab historia scriptoris pendet
 * (inventum muri M2c: inaequalis.i32 inter registrum s32-canonicum
 * et i32-canonicum verba plena comparabat - ramus falsus). */
interior i64
_valor_canonicus (Machinula* m, constans Tabulatum* t,
    constans MedullaOperandum* operandum, s32 typus)
{
    redde _canonicum(_valor_operandi(m, t, operandum, typus),
        typus);
}

interior b32
_memoriam_probare (Machinula* m, i64 inscriptio,
    memoriae_index numerus)
{
    si (inscriptio < (i64)4096)
    {
        _vitium(m, "memoria: monstrator nullus/humilis");
        redde FALSUM;
    }
    si (regio_custodia(m->regio)
        && (!regio_continet(m->regio,
                (constans vacuum*)(memoriae_index)inscriptio)
            || !regio_continet(m->regio,
                (constans vacuum*)((memoriae_index)inscriptio
                    + numerus - I))))
    {
        _vitium(m, "memoria: extra regionem");
        redde FALSUM;
    }
    redde VERUM;
}

MachinulaExitus
machinula_currere (Machinula* m, chorda titulus_functionis)
{
    MachinulaExitus exitus;
    i64 argumenta[ARGUMENTA_MAXIMA];

    memset(&exitus, ZEPHYRUM, magnitudo(MachinulaExitus));
    exitus.genus = MACHINULA_VITIUM;

    /* status purgatus - currere iterabile */
    xar_truncare(m->tabulata, ZEPHYRUM);
    m->stiva_cursor = ZEPHYRUM;
    m->currens = VERUM;
    m->halitus_genus = MACHINULA_BENE;
    m->halitus_codex = ZEPHYRUM;
    m->halitus_nuntius.datum = NIHIL;
    m->halitus_nuntius.mensura = ZEPHYRUM;

    /* initium */
    {
        s32 index = conexio_symbolum_quaerere(m->conexio,
            titulus_functionis);
        constans ConexioSymbolum* symbolum;
        constans ConexioDescriptor* descriptor;
        constans ConexioFunctioNexa* nexa;

        si (index < ZEPHYRUM)
        {
            _vitium(m, "initium: functio non inventa");
        }
        alioquin
        {
            symbolum = conexio_symbolum_obtinere(m->conexio, index);
            si (symbolum->genus != CONEXIO_SYMBOLUM_FUNCTIO)
            {
                _vitium(m, "initium: non functio");
            }
            alioquin
            {
                descriptor = symbolum->sedes;
                nexa = conexio_functionem_obtinere(m->conexio,
                    descriptor->index);
                (vacuum)_tabulatum_impellere(m, nexa->functio,
                    nexa->modulus_index, -I, NIHIL, ZEPHYRUM);
            }
        }
    }

    /* ansa dispensationis */
    dum (m->currens)
    {
        Tabulatum* t = xar_obtinere(m->tabulata,
            (i32)xar_numerus(m->tabulata) - I);
        constans MedullaBloccus* bloccus =
            medulla_bloccum_obtinere(t->functio, t->bloccus);
        constans MedullaInstructio* instructio;
        s32 op;

        si (bloccus == NIHIL || t->instructio
            >= (i32)xar_numerus(bloccus->instructiones))
        {
            _vitium(m, "fluxus extra bloccum");
            frange;
        }
        instructio = (constans MedullaInstructio*)xar_obtinere(
            bloccus->instructiones, t->instructio);
        op = instructio->op;

        /* anulus + numeratores (semper) */
        {
            AnulusFigura* figura =
                &m->anulus[m->anulus_cursor & (i64)ANULUS_LARVA];

            figura->functio = t->functio;
            figura->modulus_index = t->modulus_index;
            figura->bloccus = t->bloccus;
            figura->instructio = t->instructio;
            m->anulus_cursor += I;
        }
        m->numeri_op[op] += I;
        m->summa_instructionum += I;

        commutatio (op)
        {
        casus MEDULLA_OP_MOVERE:
            t->registra[instructio->destinatio] = _canonicum(
                _valor_operandi(m, t, &instructio->a,
                    instructio->typus), instructio->typus);
            t->instructio += I;
            frange;

        casus MEDULLA_OP_ADDERE:
        casus MEDULLA_OP_SUBTRAHERE:
        casus MEDULLA_OP_MULTIPLICARE:
        casus MEDULLA_OP_DIVIDERE:
        casus MEDULLA_OP_RESIDUUM:
        {
            s32 typus = instructio->typus;
            i64 a = _valor_canonicus(m, t, &instructio->a, typus);
            i64 b = _valor_canonicus(m, t, &instructio->b, typus);
            i64 fructus = ZEPHYRUM;

            si (typus == MEDULLA_TYPUS_F64)
            {
                f64 fa = _f64_de_verbo(a);
                f64 fb = _f64_de_verbo(b);
                f64 fr = 0.0;

                commutatio (op)
                {
                casus MEDULLA_OP_ADDERE:       fr = fa + fb; frange;
                casus MEDULLA_OP_SUBTRAHERE:   fr = fa - fb; frange;
                casus MEDULLA_OP_MULTIPLICARE: fr = fa * fb; frange;
                ordinarius:                    fr = fa / fb; frange;
                }
                fructus = _verbum_de_f64(fr);
            }
            alioquin si (typus == MEDULLA_TYPUS_F32)
            {
                f32 fa = _f32_de_verbo(a);
                f32 fb = _f32_de_verbo(b);
                f32 fr = 0.0f;

                commutatio (op)
                {
                casus MEDULLA_OP_ADDERE:       fr = fa + fb; frange;
                casus MEDULLA_OP_SUBTRAHERE:   fr = fa - fb; frange;
                casus MEDULLA_OP_MULTIPLICARE: fr = fa * fb; frange;
                ordinarius:                    fr = fa / fb; frange;
                }
                fructus = _verbum_de_f32(fr);
            }
            alioquin si (op == MEDULLA_OP_DIVIDERE
                || op == MEDULLA_OP_RESIDUUM)
            {
                si (b == ZEPHYRUM)
                {
                    _vitium(m, "divisio per zephyrum");
                    frange;
                }
                si (_typus_signatus(typus))
                {
                    s64 sa = (s64)a;
                    s64 sb = (s64)b;

                    si (sb == (s64)-I
                        && sa == (s64)((i64)I << 63))
                    {
                        _vitium(m, "divisio: superfluxus");
                        frange;
                    }
                    fructus = (op == MEDULLA_OP_DIVIDERE)
                        ? (i64)(sa / sb) : (i64)(sa % sb);
                }
                alioquin
                {
                    fructus = (op == MEDULLA_OP_DIVIDERE)
                        ? a / b : a % b;
                }
                fructus = _canonicum(fructus, typus);
            }
            alioquin
            {
                commutatio (op)
                {
                casus MEDULLA_OP_ADDERE:       fructus = a + b; frange;
                casus MEDULLA_OP_SUBTRAHERE:   fructus = a - b; frange;
                ordinarius:                    fructus = a * b; frange;
                }
                fructus = _canonicum(fructus, typus);
            }
            si (m->currens)
            {
                t->registra[instructio->destinatio] = fructus;
                t->instructio += I;
            }
            frange;
        }

        casus MEDULLA_OP_NEGARE:
        {
            s32 typus = instructio->typus;
            i64 a = _valor_operandi(m, t, &instructio->a, typus);

            si (typus == MEDULLA_TYPUS_F64)
            {
                t->registra[instructio->destinatio] =
                    _verbum_de_f64(-_f64_de_verbo(a));
            }
            alioquin si (typus == MEDULLA_TYPUS_F32)
            {
                t->registra[instructio->destinatio] =
                    _verbum_de_f32(-_f32_de_verbo(a));
            }
            alioquin
            {
                t->registra[instructio->destinatio] = _canonicum(
                    (i64)ZEPHYRUM - a, typus);
            }
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_ET:
        casus MEDULLA_OP_VEL:
        casus MEDULLA_OP_AUT:
        {
            s32 typus = instructio->typus;
            i64 a = _valor_operandi(m, t, &instructio->a, typus);
            i64 b = _valor_operandi(m, t, &instructio->b, typus);
            i64 fructus;

            si (op == MEDULLA_OP_ET)
            {
                fructus = a & b;
            }
            alioquin si (op == MEDULLA_OP_VEL)
            {
                fructus = a | b;
            }
            alioquin
            {
                fructus = a ^ b;
            }
            t->registra[instructio->destinatio] = _canonicum(
                fructus, typus);
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_COMPLEMENTUM:
        {
            s32 typus = instructio->typus;
            i64 a = _valor_operandi(m, t, &instructio->a, typus);

            t->registra[instructio->destinatio] = _canonicum(~a,
                typus);
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_SINISTRORSUM:
        casus MEDULLA_OP_DEXTRORSUM:
        {
            s32 typus = instructio->typus;
            i64 a = _valor_canonicus(m, t, &instructio->a, typus);
            i64 b = _valor_operandi(m, t, &instructio->b,
                MEDULLA_TYPUS_S32);
            i64 n = b & (i64)(_latitudo_typi(typus) - I);
            i64 fructus;

            si (op == MEDULLA_OP_SINISTRORSUM)
            {
                fructus = a << n;
            }
            alioquin si (_typus_signatus(typus))
            {
                fructus = (i64)((s64)a >> n);   /* arithmeticum */
            }
            alioquin
            {
                fructus = a >> n;               /* logicum */
            }
            t->registra[instructio->destinatio] = _canonicum(
                fructus, typus);
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_AEQUALIS:
        casus MEDULLA_OP_INAEQUALIS:
        casus MEDULLA_OP_MINOR:
        casus MEDULLA_OP_MINOR_AEQUALIS:
        casus MEDULLA_OP_MAIOR:
        casus MEDULLA_OP_MAIOR_AEQUALIS:
        {
            s32 typus = instructio->typus;
            i64 a = _valor_canonicus(m, t, &instructio->a, typus);
            i64 b = _valor_canonicus(m, t, &instructio->b, typus);
            b32 verum_est = FALSUM;

            si (typus == MEDULLA_TYPUS_F64
                || typus == MEDULLA_TYPUS_F32)
            {
                f64 fa = (typus == MEDULLA_TYPUS_F32)
                    ? (f64)_f32_de_verbo(a) : _f64_de_verbo(a);
                f64 fb = (typus == MEDULLA_TYPUS_F32)
                    ? (f64)_f32_de_verbo(b) : _f64_de_verbo(b);

                commutatio (op)
                {
                casus MEDULLA_OP_AEQUALIS:
                    verum_est = (b32)(fa == fb); frange;
                casus MEDULLA_OP_INAEQUALIS:
                    verum_est = (b32)(fa != fb); frange;
                casus MEDULLA_OP_MINOR:
                    verum_est = (b32)(fa < fb); frange;
                casus MEDULLA_OP_MINOR_AEQUALIS:
                    verum_est = (b32)(fa <= fb); frange;
                casus MEDULLA_OP_MAIOR:
                    verum_est = (b32)(fa > fb); frange;
                ordinarius:
                    verum_est = (b32)(fa >= fb); frange;
                }
            }
            alioquin si (_typus_signatus(typus))
            {
                s64 sa = (s64)a;
                s64 sb = (s64)b;

                commutatio (op)
                {
                casus MEDULLA_OP_AEQUALIS:
                    verum_est = (b32)(sa == sb); frange;
                casus MEDULLA_OP_INAEQUALIS:
                    verum_est = (b32)(sa != sb); frange;
                casus MEDULLA_OP_MINOR:
                    verum_est = (b32)(sa < sb); frange;
                casus MEDULLA_OP_MINOR_AEQUALIS:
                    verum_est = (b32)(sa <= sb); frange;
                casus MEDULLA_OP_MAIOR:
                    verum_est = (b32)(sa > sb); frange;
                ordinarius:
                    verum_est = (b32)(sa >= sb); frange;
                }
            }
            alioquin
            {
                commutatio (op)
                {
                casus MEDULLA_OP_AEQUALIS:
                    verum_est = (b32)(a == b); frange;
                casus MEDULLA_OP_INAEQUALIS:
                    verum_est = (b32)(a != b); frange;
                casus MEDULLA_OP_MINOR:
                    verum_est = (b32)(a < b); frange;
                casus MEDULLA_OP_MINOR_AEQUALIS:
                    verum_est = (b32)(a <= b); frange;
                casus MEDULLA_OP_MAIOR:
                    verum_est = (b32)(a > b); frange;
                ordinarius:
                    verum_est = (b32)(a >= b); frange;
                }
            }
            t->registra[instructio->destinatio] =
                verum_est ? (i64)I : (i64)ZEPHYRUM;
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_EXTENDERE:
            t->registra[instructio->destinatio] = _canonicum(
                _valor_operandi(m, t, &instructio->a,
                    instructio->typus), instructio->typus);
            t->instructio += I;
            frange;

        casus MEDULLA_OP_AMPUTARE:
            t->registra[instructio->destinatio] = _canonicum(
                _valor_operandi(m, t, &instructio->a,
                    instructio->typus), instructio->typus_secundus);
            t->instructio += I;
            frange;

        casus MEDULLA_OP_FLUITARE:
        {
            i64 a = _valor_canonicus(m, t, &instructio->a,
                instructio->typus);
            f64 valor;

            si (_typus_signatus(instructio->typus))
            {
                valor = (f64)(s64)a;
            }
            alioquin
            {
                valor = (f64)a;
            }
            si (instructio->typus_secundus == MEDULLA_TYPUS_F32)
            {
                t->registra[instructio->destinatio] =
                    _verbum_de_f32((f32)valor);
            }
            alioquin
            {
                t->registra[instructio->destinatio] =
                    _verbum_de_f64(valor);
            }
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_INTEGRARE:
        {
            i64 a = _valor_operandi(m, t, &instructio->a,
                instructio->typus);
            f64 valor = (instructio->typus == MEDULLA_TYPUS_F32)
                ? (f64)_f32_de_verbo(a) : _f64_de_verbo(a);
            i64 fructus;

            si (_typus_signatus(instructio->typus_secundus))
            {
                fructus = (i64)(s64)valor;
            }
            alioquin
            {
                fructus = (i64)valor;
            }
            t->registra[instructio->destinatio] = _canonicum(
                fructus, instructio->typus_secundus);
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_DILATARE:
            t->registra[instructio->destinatio] = _verbum_de_f64(
                (f64)_f32_de_verbo(_valor_operandi(m, t,
                    &instructio->a, MEDULLA_TYPUS_F32)));
            t->instructio += I;
            frange;

        casus MEDULLA_OP_ARTARE:
            t->registra[instructio->destinatio] = _verbum_de_f32(
                (f32)_f64_de_verbo(_valor_operandi(m, t,
                    &instructio->a, MEDULLA_TYPUS_F64)));
            t->instructio += I;
            frange;

        casus MEDULLA_OP_LEGERE:
        {
            i64 inscriptio = _valor_operandi(m, t, &instructio->a,
                MEDULLA_TYPUS_I64);
            s32 typus = instructio->typus;
            memoriae_index latitudo = (memoriae_index)
                (_latitudo_typi(typus) / 8);
            i64 verbum = ZEPHYRUM;

            si (!_memoriam_probare(m, inscriptio, latitudo))
            {
                frange;
            }
            memcpy(&verbum, (constans vacuum*)(memoriae_index)
                inscriptio, latitudo);
            t->registra[instructio->destinatio] = _canonicum(
                verbum, typus);
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_SCRIBERE:
        {
            i64 inscriptio = _valor_operandi(m, t, &instructio->a,
                MEDULLA_TYPUS_I64);
            s32 typus = instructio->typus;
            memoriae_index latitudo = (memoriae_index)
                (_latitudo_typi(typus) / 8);
            i64 verbum = _valor_operandi(m, t, &instructio->b,
                typus);

            si (!_memoriam_probare(m, inscriptio, latitudo))
            {
                frange;
            }
            memcpy((vacuum*)(memoriae_index)inscriptio, &verbum,
                latitudo);
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_ARCA:
        {
            memoriae_index mag = (memoriae_index)_valor_operandi(m,
                t, &instructio->a, MEDULLA_TYPUS_I64);
            memoriae_index ordinatio = (memoriae_index)
                _valor_operandi(m, t, &instructio->b,
                    MEDULLA_TYPUS_I64);
            memoriae_index cursor;

            si (ordinatio == ZEPHYRUM)
            {
                ordinatio = I;
            }
            si (ordinatio < XVI)
            {
                ordinatio = XVI;   /* arcae XVI-ordinatae semper */
            }
            cursor = (m->stiva_cursor + ordinatio - I)
                & ~(ordinatio - I);
            si (cursor + mag + STIVA_MARGO > m->stiva_magnitudo)
            {
                _vitium(m, "stiva exhausta (arca)");
                frange;
            }
            t->registra[instructio->destinatio] =
                (i64)(memoriae_index)(m->stiva_basis + cursor);
            m->stiva_cursor = cursor + mag;
            si (m->stiva_cursor > m->apex_stivae)
            {
                m->apex_stivae = m->stiva_cursor;
            }
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_LOCUS:
        {
            s32 globale = conexio_symbolum_globale(m->conexio,
                t->modulus_index, instructio->a.datum.index);
            constans ConexioSymbolum* symbolum;

            si (globale < ZEPHYRUM)
            {
                _vitium(m, "locus: symbolum insanum");
                frange;
            }
            symbolum = conexio_symbolum_obtinere(m->conexio,
                globale);
            t->registra[instructio->destinatio] =
                (i64)(memoriae_index)symbolum->sedes;
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_COPIA:
        {
            i64 finis = _valor_operandi(m, t, &instructio->a,
                MEDULLA_TYPUS_I64);
            i64 fons = _valor_operandi(m, t, &instructio->b,
                MEDULLA_TYPUS_I64);
            memoriae_index mag = (memoriae_index)_valor_operandi(m,
                t, &instructio->c, MEDULLA_TYPUS_I64);

            si (!_memoriam_probare(m, finis, mag)
                || !_memoriam_probare(m, fons, mag))
            {
                frange;
            }
            memmove((vacuum*)(memoriae_index)finis,
                (constans vacuum*)(memoriae_index)fons, mag);
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_SALIRE:
            t->bloccus = instructio->a.datum.index;
            t->instructio = ZEPHYRUM;
            frange;

        casus MEDULLA_OP_RAMUS:
        {
            i64 conditio = _valor_operandi(m, t, &instructio->a,
                MEDULLA_TYPUS_I64);

            t->bloccus = (conditio != ZEPHYRUM)
                ? instructio->b.datum.index
                : instructio->c.datum.index;
            t->instructio = ZEPHYRUM;
            frange;
        }

        casus MEDULLA_OP_REDDE:
        {
            i64 verbum = ZEPHYRUM;
            s32 destinatio = t->destinatio_vocantis;
            b32 valorem_habet =
                (b32)(instructio->a.genus
                    != MEDULLA_OPERANDUM_NIHIL);

            si (valorem_habet)
            {
                /* valor reditus canonicus per typum signaturae -
                 * symmetria cum parametris (vide _tabulatum_addere) */
                verbum = _valor_canonicus(m, t, &instructio->a,
                    t->functio->typus_reditus);
            }
            /* tabulatum solvere */
            m->stiva_cursor = t->basis_stivae;
            si (xar_numerus(m->tabulata) == I)
            {
                _halitum_ponere(m, MACHINULA_BENE, (s64)verbum,
                    NIHIL);
                frange;
            }
            (vacuum)xar_removere_ultimum(m->tabulata);
            si (destinatio >= ZEPHYRUM && valorem_habet)
            {
                Tabulatum* vocans = xar_obtinere(m->tabulata,
                    (i32)xar_numerus(m->tabulata) - I);

                vocans->registra[destinatio] = verbum;
            }
            frange;
        }

        casus MEDULLA_OP_VOCARE:
        {
            constans ConexioDescriptor* descriptor = NIHIL;
            s32 numerus_argumentorum = instructio->extra_numerus;
            s32 k;

            si (instructio->a.genus == MEDULLA_OPERANDUM_SYMBOLUM)
            {
                s32 globale = conexio_symbolum_globale(m->conexio,
                    t->modulus_index, instructio->a.datum.index);
                constans ConexioSymbolum* symbolum;

                si (globale < ZEPHYRUM)
                {
                    _vitium(m, "vocatio: symbolum insanum");
                    frange;
                }
                symbolum = conexio_symbolum_obtinere(m->conexio,
                    globale);
                si (symbolum->genus != CONEXIO_SYMBOLUM_FUNCTIO
                    && symbolum->genus != CONEXIO_SYMBOLUM_DECIPULA)
                {
                    _vitium(m, "vocatio: non functio");
                    frange;
                }
                descriptor = symbolum->sedes;
            }
            alioquin
            {
                i64 inscriptio = _valor_operandi(m, t,
                    &instructio->a, MEDULLA_TYPUS_I64);

                si (!_memoriam_probare(m, inscriptio,
                        magnitudo(ConexioDescriptor)))
                {
                    frange;
                }
                descriptor = (constans ConexioDescriptor*)
                    (memoriae_index)inscriptio;
                si (descriptor->signum
                        != CONEXIO_SIGNUM_INTERPRETATUM
                    && descriptor->signum != CONEXIO_SIGNUM_DECIPULA
                    && descriptor->signum
                        != CONEXIO_SIGNUM_AEDIFICATUM)
                {
                    _vitium(m,
                        "vocatio per monstratorem insanum");
                    frange;
                }
            }

            si (numerus_argumentorum > (s32)ARGUMENTA_MAXIMA)
            {
                _vitium(m, "vocatio: argumenta nimia");
                frange;
            }
            {
                /* signatura ante argumenta: argumentum per typum
                 * PARAMETRI aestimatur ubi nota (litera f32 ut
                 * IMMEDIATUM_F sub S64 figuram f64 pareret -
                 * dimidium falsum in parametro f32; inventum
                 * nuntium EXITUS-1). Ignota (aedificata/decipulae/
                 * argumenta supra parametra) -> S64 ut ante
                 * (promotiones variadicae: f64 recte). */
                constans MedullaFunctio* functio_nexae = NIHIL;

                si (descriptor->signum
                    == CONEXIO_SIGNUM_INTERPRETATUM)
                {
                    functio_nexae = conexio_functionem_obtinere(
                        m->conexio, descriptor->index)->functio;
                }
                per (k = ZEPHYRUM; k < numerus_argumentorum; k++)
                {
                    constans MedullaOperandum* operandum =
                        (constans MedullaOperandum*)xar_obtinere(
                            t->functio->operanda,
                            (i32)(instructio->extra_index + k));
                    s32 typus_argumenti = MEDULLA_TYPUS_S64;

                    si (functio_nexae != NIHIL
                        && k < (s32)xar_numerus(
                               functio_nexae->parametra))
                    {
                        typus_argumenti =
                            ((constans MedullaParametrum*)
                                xar_obtinere(
                                    functio_nexae->parametra,
                                    (i32)k))->typus;
                    }
                    argumenta[k] = _valor_operandi(m, t, operandum,
                        typus_argumenti);
                }
            }
            si (!m->currens)
            {
                frange;   /* vitium in argumentis */
            }

            si (descriptor->signum == CONEXIO_SIGNUM_INTERPRETATUM)
            {
                constans ConexioFunctioNexa* nexa =
                    conexio_functionem_obtinere(m->conexio,
                        descriptor->index);

                t->instructio += I;   /* resumptio post redde */
                si (!_tabulatum_impellere(m, nexa->functio,
                        nexa->modulus_index, instructio->destinatio,
                        argumenta, numerus_argumentorum))
                {
                    frange;
                }
            }
            alioquin si (descriptor->signum
                == CONEXIO_SIGNUM_AEDIFICATUM)
            {
                i64 fructus = ZEPHYRUM;

                si (!_aedificatum_vocare(m, descriptor->index,
                        argumenta, numerus_argumentorum, &fructus))
                {
                    frange;   /* halitus iam positus */
                }
                si (instructio->destinatio >= ZEPHYRUM)
                {
                    t->registra[instructio->destinatio] =
                        _canonicum(fructus, instructio->typus);
                }
                t->instructio += I;
            }
            alioquin
            {
                constans chorda* titulus = conexio_decipulam_obtinere(
                    m->conexio, (s32)descriptor->index);
                character nuntius[CXXVIII];

                sprintf(nuntius, "decipula vocata: %.*s",
                    (titulus != NIHIL && titulus->mensura < C)
                        ? (int)titulus->mensura : ZEPHYRUM,
                    (titulus != NIHIL)
                        ? (constans character*)titulus->datum : "");
                _vitium(m, nuntius);
                m->halitus_genus = MACHINULA_DECIPULA;
            }
            frange;
        }

        casus MEDULLA_OP_SISTERE:
        {
            constans MedullaModulus* modulus = _modulum(m,
                t->modulus_index);
            constans chorda* causa = medulla_causam_obtinere(
                modulus, instructio->a.datum.index);

            _halitum_ponere(m, MACHINULA_SISTERE, I, NIHIL);
            si (causa != NIHIL)
            {
                m->halitus_nuntius = *causa;
            }
            frange;
        }

        ordinarius:
            _vitium(m, "operatio ignota");
            frange;
        }
    }

    si (m->halitus_genus != MACHINULA_BENE)
    {
        _relationem_imprimere(m);
    }
    exitus.genus = m->halitus_genus;
    exitus.codex = m->halitus_codex;
    exitus.nuntius = m->halitus_nuntius;
    redde exitus;
}

/* ==================================================
 * Census
 * ================================================== */

i64
machinula_numerus_instructionum (constans Machinula* machinula)
{
    redde machinula->summa_instructionum;
}

i64
machinula_numerus_op (constans Machinula* machinula, s32 op)
{
    si (op < ZEPHYRUM || op >= (s32)MEDULLA_OP_NUMERUS)
    {
        redde ZEPHYRUM;
    }
    redde machinula->numeri_op[op];
}

i64
machinula_numerus_vocationum (constans Machinula* machinula)
{
    redde machinula->numerus_vocationum;
}

i64
machinula_numerus_aedificatorum (constans Machinula* machinula)
{
    redde machinula->numerus_aedificatorum;
}

memoriae_index
machinula_stiva_apex (constans Machinula* machinula)
{
    redde machinula->apex_stivae;
}
