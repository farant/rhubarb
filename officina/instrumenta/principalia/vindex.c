/* vindex.c - VINDEX: debugger TUI officinae (M3 chunk 6)
 *
 * App super amalgamata TRIA (officina + silva + tessera; exemplar
 * saltuarii). Positio OMNIS per indicium LECTOREM - numquam per
 * lineas memoriae (forma-primum visibiliter: plagula scripta,
 * reclusa, sola auctoritas).
 *
 * Usus: ./officina/vindex.sh <suita.c> [-plagulae a,b,c]
 *           [-imago <via>:<linea>]
 *   -plagulae: filtra lib/ (defalta: totus lib/ - lentior)
 *   -imago:    sine terminale - punctum pone, curre ad pausam,
 *              effigiem unam pinge et ut textum in stdout funde
 *              (fumus probationis; sessio viva terminale poscit)
 *
 * Claves: g gradus | s super lineam | i intra lineam | f fini |
 * p perge | v/a/u/m tabula dextra | sursum/deorsum volumen |
 * q exi | ':' imperia (punctum via:linea | punctum functio |
 * dele ... | memoria hex | curre | exi)
 */

#include "vindex_onerator.h"
#include "vindex_visum.h"

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#define CUSTOS_GRADUUM   2000000L
#define IMPERIUM_MAXIMUM 200
#define PLAGULAE_MAXIMAE 32

/* punctum app-lateris (pro tabula + delendo) */
nomen structura {
    s32 functio_index;
    i32 instructio;
    chorda descriptio;             /* "via:linea" aut "functio" */
} VindexPunctum;

nomen structura {
    VindexOnerator* onerator;
    Machinula* machinula;
    IndiciumLector* lector;
    Piscina* piscina_perpetua;     /* puncta, fons cachatus */
    Piscina* piscina_effigiei;     /* vacatur quaque effigie */
    Xar* puncta;                   /* VindexPunctum valore */
    /* fons cachatus (via una) */
    chorda via_fontis;
    chorda* lineae_fontis;
    i32 numerus_linearum;
    /* status visus */
    i32 linea_prima;
    s32 tabula_dextra;
    i64 memoria_basis;             /* 0 = nulla */
    character imperium[IMPERIUM_MAXIMUM];
    i32 imperium_mensura;
    b32 imperium_apertum;
    character status[256];
    constans character* radix;
    /* captura effusionis interpretatae (modus TUI): printf mundi
     * interpretati in TERMINALE IPSUM scriberet - volumen physicum
     * quod diff tesserae nescit (phantasmata; inventum Franis,
     * sessio prima). fd 1/2 circum exsecutionem VM permutantur. */
    integer fd_tty_stdout;         /* -1 = captura inactiva */
    integer fd_tty_stderr;
    integer fd_capturae;
} Vindex;

interior vacuum
_vm_capere (Vindex* v)
{
    si (v->fd_capturae < 0) redde;
    fflush(stdout);
    fflush(stderr);
    (vacuum)dup2(v->fd_capturae, 1);
    (vacuum)dup2(v->fd_capturae, 2);
}

interior vacuum
_vm_solvere (Vindex* v)
{
    si (v->fd_capturae < 0) redde;
    fflush(stdout);
    fflush(stderr);
    (vacuum)dup2(v->fd_tty_stdout, 1);
    (vacuum)dup2(v->fd_tty_stderr, 2);
}

interior chorda
_ch (constans character* literis)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c = literis;
    c.datum = u.m;
    c.mensura = (i32)strlen(literis);
    redde c;
}

interior OfficinaChorda
_och (chorda c)
{
    OfficinaChorda oc;

    oc.datum = (insignatus character*)c.datum;
    oc.mensura = c.mensura;
    redde oc;
}

interior chorda
_ch_de_off (OfficinaChorda oc)
{
    chorda c;

    c.datum = (i8*)oc.datum;
    c.mensura = oc.mensura;
    redde c;
}

/* ==================================================
 * fons cachatus (via una; recargatur mutatione viae)
 * ================================================== */

interior vacuum
_fontem_curare (Vindex* v, chorda via)
{
    character via_plena[1024];
    FILE* pl;
    long mensura_l;
    i8* textus;
    i32 numerus;
    i32 i;
    i32 initium;

    si (via.mensura == ZEPHYRUM) redde;
    si (v->via_fontis.mensura == via.mensura
        && memcmp(v->via_fontis.datum, via.datum,
               (memoriae_index)via.mensura) == 0)
    {
        redde;   /* iam cachatus */
    }
    sprintf(via_plena, "%s/%.*s", v->radix, (int)via.mensura,
        (constans character*)via.datum);
    pl = fopen(via_plena, "rb");
    si (pl == NIHIL) redde;
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    textus = (i8*)piscina_allocare(v->piscina_perpetua,
        (memoriae_index)(mensura_l + 1L));
    si (textus == NIHIL
        || fread(textus, I, (memoriae_index)mensura_l, pl)
            != (memoriae_index)mensura_l)
    {
        fclose(pl);
        redde;
    }
    fclose(pl);
    /* scindere in lineas */
    numerus = I;
    per (i = ZEPHYRUM; i < (i32)mensura_l; i++)
    {
        si (textus[i] == '\n') numerus++;
    }
    v->lineae_fontis = (chorda*)piscina_allocare(
        v->piscina_perpetua,
        (memoriae_index)numerus * magnitudo(chorda));
    si (v->lineae_fontis == NIHIL) redde;
    v->numerus_linearum = ZEPHYRUM;
    initium = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= (i32)mensura_l; i++)
    {
        si (i == (i32)mensura_l || textus[i] == '\n')
        {
            chorda* linea = &v->lineae_fontis[
                v->numerus_linearum];

            linea->datum = textus + initium;
            linea->mensura = i - initium;
            v->numerus_linearum++;
            initium = i + I;
            si (v->numerus_linearum >= numerus) frange;
        }
    }
    {
        chorda copia;

        copia.datum = (i8*)piscina_allocare(v->piscina_perpetua,
            (memoriae_index)(via.mensura > 0U ? via.mensura : 1U));
        si (copia.datum != NIHIL)
        {
            memcpy(copia.datum, via.datum,
                (memoriae_index)via.mensura);
            copia.mensura = via.mensura;
            v->via_fontis = copia;
        }
    }
}

/* ==================================================
 * positio per lectorem (auctoritas = plagula)
 * ================================================== */

interior b32
_positionem_quaerere (Vindex* v, s32* functio_out, i32* instr_out)
{
    i32 numerus = (i32)machinula_tabulata_numerus(v->machinula);

    si (numerus == ZEPHYRUM) redde FALSUM;
    redde (b32)machinula_positionem_inspicere(v->machinula,
        numerus - I, functio_out, instr_out);
}

interior i32
_lineam_radicis (Vindex* v, s32 functio_index, i32 instructio,
    chorda* via_out)
{
    constans IndiciumLinea* acies = NIHIL;
    i32 n = (i32)indicium_lineas_de_instructione(v->lector,
        (insignatus integer)functio_index,
        (insignatus integer)instructio, &acies);

    si (n == ZEPHYRUM) redde ZEPHYRUM;
    si (via_out != NIHIL)
    {
        *via_out = _ch_de_off(indicium_via_chorda(v->lector,
            acies[ZEPHYRUM].via));
    }
    redde (i32)acies[ZEPHYRUM].linea;
}

/* ==================================================
 * gressus (ansae vectis 5: radix mutatur + custos profunditatis)
 * ================================================== */

interior vacuum
_gradus_unus (Vindex* v)
{
    _vm_capere(v);
    (vacuum)machinula_gradus(v->machinula);
    _vm_solvere(v);
}

interior vacuum
_super_lineam (Vindex* v, b32 intra)
{
    s32 functio = -I;
    i32 instructio = ZEPHYRUM;
    i32 linea_initii;
    chorda via_initii;
    i32 profunditas_introitus =
        (i32)machinula_tabulata_numerus(v->machinula);
    long facti = 0L;

    via_initii.datum = NIHIL;
    via_initii.mensura = ZEPHYRUM;
    si (!_positionem_quaerere(v, &functio, &instructio)) redde;
    linea_initii = _lineam_radicis(v, functio, instructio,
        &via_initii);
    _vm_capere(v);
    dum (facti < CUSTOS_GRADUUM)
    {
        si (!machinula_gradus(v->machinula))
        {
            _vm_solvere(v);
            redde;   /* halitus */
        }
        facti++;
        si (!intra && (i32)machinula_tabulata_numerus(v->machinula)
            > profunditas_introitus)
        {
            perge;
        }
        si (!_positionem_quaerere(v, &functio, &instructio))
        {
            _vm_solvere(v);
            redde;
        }
        {
            chorda via_nova;
            i32 linea = _lineam_radicis(v, functio, instructio,
                &via_nova);

            si (linea == ZEPHYRUM) perge;
            si (linea != linea_initii
                || via_nova.mensura != via_initii.mensura
                || (via_nova.mensura > 0U
                    && memcmp(via_nova.datum, via_initii.datum,
                           (memoriae_index)via_nova.mensura) != 0))
            {
                _vm_solvere(v);
                redde;
            }
        }
    }
    _vm_solvere(v);
}

interior vacuum
_fini (Vindex* v)
{
    i32 profunditas =
        (i32)machinula_tabulata_numerus(v->machinula);
    long facti = 0L;

    si (profunditas <= I) redde;
    _vm_capere(v);
    dum ((i32)machinula_tabulata_numerus(v->machinula)
        >= profunditas && facti < CUSTOS_GRADUUM)
    {
        si (!machinula_gradus(v->machinula)) frange;
        facti++;
    }
    _vm_solvere(v);
}

/* ==================================================
 * imperia (':' linea)
 * ================================================== */

interior b32
_punctum_de_argumento (Vindex* v, chorda argumentum,
    s32* functio_out, i32* instructio_out, chorda* descriptio_out)
{
    i32 i;
    s32 colon = -I;

    per (i = ZEPHYRUM; i < argumentum.mensura; i++)
    {
        si (argumentum.datum[i] == ':') colon = (s32)i;
    }
    si (colon >= ZEPHYRUM)
    {
        /* via:linea per tabulam RETRO */
        chorda via;
        i32 linea = ZEPHYRUM;
        constans IndiciumSitus* situs = NIHIL;

        via.datum = argumentum.datum;
        via.mensura = (i32)colon;
        per (i = (i32)colon + I; i < argumentum.mensura; i++)
        {
            si (argumentum.datum[i] < '0'
                || argumentum.datum[i] > '9')
            {
                redde FALSUM;
            }
            linea = linea * X + (i32)(argumentum.datum[i] - '0');
        }
        si (indicium_situs_de_linea(v->lector, _och(via),
                (insignatus integer)linea, &situs) == 0U)
        {
            redde FALSUM;
        }
        *functio_out = (s32)situs[ZEPHYRUM].functio;
        *instructio_out = (i32)situs[ZEPHYRUM].instructio;
    }
    alioquin
    {
        /* functio nomine */
        s32 functio = (s32)indicium_functionem_quaerere(v->lector,
            _och(argumentum));

        si (functio < ZEPHYRUM) redde FALSUM;
        *functio_out = functio;
        *instructio_out = ZEPHYRUM;
    }
    *descriptio_out = argumentum;
    redde VERUM;
}

interior vacuum
_imperium_exsequi (Vindex* v)
{
    chorda imperium;
    chorda verbum;
    chorda argumentum;
    s32 spatium = -I;
    i32 i;

    imperium.datum = (i8*)v->imperium;
    imperium.mensura = v->imperium_mensura;
    per (i = ZEPHYRUM; i < imperium.mensura; i++)
    {
        si (imperium.datum[i] == ' ')
        {
            spatium = (s32)i;
            frange;
        }
    }
    verbum.datum = imperium.datum;
    verbum.mensura = (spatium >= ZEPHYRUM) ? (i32)spatium
        : imperium.mensura;
    argumentum.datum = (spatium >= ZEPHYRUM)
        ? imperium.datum + (i32)spatium + I : NIHIL;
    argumentum.mensura = (spatium >= ZEPHYRUM)
        ? imperium.mensura - (i32)spatium - I : ZEPHYRUM;

    si (chorda_aequalis_literis(verbum, "punctum")
        && argumentum.mensura > ZEPHYRUM)
    {
        s32 functio = -I;
        i32 instructio = ZEPHYRUM;
        chorda descriptio;

        si (_punctum_de_argumento(v, argumentum, &functio,
                &instructio, &descriptio)
            && machinula_punctum_ponere(v->machinula,
                   (integer)functio,
                   (insignatus integer)instructio))
        {
            VindexPunctum* punctum = xar_addere(v->puncta);

            si (punctum != NIHIL)
            {
                chorda copia;

                copia.datum = (i8*)piscina_allocare(
                    v->piscina_perpetua,
                    (memoriae_index)descriptio.mensura);
                copia.mensura = ZEPHYRUM;
                si (copia.datum != NIHIL)
                {
                    memcpy(copia.datum, descriptio.datum,
                        (memoriae_index)descriptio.mensura);
                    copia.mensura = descriptio.mensura;
                }
                punctum->functio_index = functio;
                punctum->instructio = instructio;
                punctum->descriptio = copia;
            }
            sprintf(v->status, "punctum positum: %.*s",
                (int)argumentum.mensura,
                (constans character*)argumentum.datum);
        }
        alioquin
        {
            sprintf(v->status, "punctum NON positum: %.*s",
                (int)argumentum.mensura,
                (constans character*)argumentum.datum);
        }
    }
    alioquin si (chorda_aequalis_literis(verbum, "dele")
        && argumentum.mensura > ZEPHYRUM)
    {
        i32 numerus = xar_numerus(v->puncta);
        b32 deletum = FALSUM;

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            VindexPunctum* punctum = xar_obtinere(v->puncta, i);

            si (chorda_aequalis(punctum->descriptio, argumentum))
            {
                (vacuum)machinula_punctum_tollere(v->machinula,
                    (integer)punctum->functio_index,
                    (insignatus integer)punctum->instructio);
                (vacuum)xar_removere_cum_ultimo(v->puncta, i);
                deletum = VERUM;
                frange;
            }
        }
        sprintf(v->status, deletum ? "punctum deletum"
            : "punctum non inventum");
    }
    alioquin si (chorda_aequalis_literis(verbum, "memoria")
        && argumentum.mensura > ZEPHYRUM)
    {
        i64 basis = ZEPHYRUM;

        per (i = ZEPHYRUM; i < argumentum.mensura; i++)
        {
            i8 c = argumentum.datum[i];
            i64 cifra;

            si (c >= '0' && c <= '9') cifra = (i64)(c - '0');
            alioquin si (c >= 'a' && c <= 'f')
                cifra = (i64)(c - 'a') + X;
            alioquin si (c >= 'A' && c <= 'F')
                cifra = (i64)(c - 'A') + X;
            alioquin si (c == 'x' || c == 'X') perge;
            alioquin frange;
            basis = basis * XVI + cifra;
        }
        v->memoria_basis = basis;
        v->tabula_dextra = (s32)VINDEX_TABULA_MEMORIA;
        sprintf(v->status, "memoria: 0x%llx",
            (insignatus longus longus)basis);
    }
    alioquin si (chorda_aequalis_literis(verbum, "curre"))
    {
        (vacuum)machinula_aperire(v->machinula, _och(_ch("main")));
        sprintf(v->status, "cursus novus (main)");
    }
    alioquin
    {
        sprintf(v->status, "imperium ignotum: %.*s",
            (int)verbum.mensura,
            (constans character*)verbum.datum);
    }
}

/* ==================================================
 * conspectum aedificare (machinula + lector -> snapshot purum)
 * ================================================== */

#define ORDINES_MAXIMI 64

nomen structura {
    VindexOrdoTabulati tabulata[ORDINES_MAXIMI];
    VindexOrdoVariabilis variabilia[ORDINES_MAXIMI];
    chorda anulus[ORDINES_MAXIMI];
    chorda puncta[ORDINES_MAXIMI];
    chorda memoria[ORDINES_MAXIMI];
    i32 puncta_lineae[ORDINES_MAXIMI];
    character positio[512];
    character modus[32];
} ConspectusPenus;

interior chorda
_ch_in_piscinam (Piscina* piscina, constans character* literis)
{
    redde chorda_ex_literis(literis, piscina);
}

interior vacuum
_valorem_formare (character* cella, s32 typus_medulla, i64 verbum)
{
    commutatio (typus_medulla)
    {
    casus 4: casus 5: casus 6: casus 7:   /* S8..S64 */
        sprintf(cella, "%lld", (longus longus)verbum);
        frange;
    casus 8:                               /* F32 */
    {
        unio { insignatus integer i; fluitans f; } u;

        u.i = (insignatus integer)(verbum & (i64)0xFFFFFFFF);
        sprintf(cella, "%g", (duplex)u.f);
        frange;
    }
    casus 9:                               /* F64 */
    {
        unio { i64 i; duplex d; } u;

        u.i = verbum;
        sprintf(cella, "%g", u.d);
        frange;
    }
    casus 0: casus 1: casus 2: casus 3:    /* I8..I64 */
        sprintf(cella, "%llu",
            (insignatus longus longus)verbum);
        frange;
    ordinarius:
        sprintf(cella, "0x%llx",
            (insignatus longus longus)verbum);
        frange;
    }
}

interior vacuum
_conspectum_aedificare (Vindex* v, VindexConspectus* conspectus,
    ConspectusPenus* penus, s32 halitus_novissimus)
{
    s32 functio = -I;
    i32 instructio = ZEPHYRUM;
    chorda via_currens;
    i32 linea_currens = ZEPHYRUM;
    i32 numerus_tabulatorum =
        (i32)machinula_tabulata_numerus(v->machinula);
    i32 k;

    piscina_vacare(v->piscina_effigiei);
    memset(conspectus, ZEPHYRUM, magnitudo(VindexConspectus));
    via_currens.datum = NIHIL;
    via_currens.mensura = ZEPHYRUM;

    si (_positionem_quaerere(v, &functio, &instructio))
    {
        linea_currens = _lineam_radicis(v, functio, instructio,
            &via_currens);
        _fontem_curare(v, via_currens);
    }

    /* modus */
    commutatio (halitus_novissimus)
    {
    casus 4:  sprintf(penus->modus, "PAUSA"); frange;   /* PAUSA */
    casus 0:  sprintf(penus->modus, "BENE");  frange;
    casus 1:  sprintf(penus->modus, "SISTERE"); frange;
    casus 2:  sprintf(penus->modus, "DECIPULA"); frange;
    ordinarius: sprintf(penus->modus, "VITIUM"); frange;
    }
    conspectus->modus = _ch(penus->modus);

    /* positio + acies macronum (per lectorem solum) */
    penus->positio[ZEPHYRUM] = '\0';
    si (functio >= ZEPHYRUM)
    {
        constans IndiciumLinea* acies = NIHIL;
        i32 n = (i32)indicium_lineas_de_instructione(v->lector,
            (insignatus integer)functio,
            (insignatus integer)instructio, &acies);
        integer cursor = 0;

        per (k = ZEPHYRUM; k < n && cursor < 400; k++)
        {
            chorda via_k = _ch_de_off(indicium_via_chorda(
                v->lector, acies[k].via));
            chorda macro_k = _ch_de_off(indicium_chorda(v->lector,
                acies[k].nomen_macro));

            si (k > ZEPHYRUM)
            {
                /* framea argumenti (spelled AD invocationem):
                 * eadem via:linea ac framea prior - "per MACRO"
                 * sufficit, positio iterata = strepitus (inventum
                 * Franis, sessio prima) */
                b32 eadem_positio = (b32)(
                    acies[k].linea == acies[k - I].linea
                    && acies[k].via == acies[k - I].via);

                cursor += sprintf(penus->positio + cursor,
                    "  per %.*s", (int)macro_k.mensura,
                    (constans character*)macro_k.datum);
                si (eadem_positio)
                {
                    perge;
                }
                cursor += sprintf(penus->positio + cursor,
                    " <- ");
            }
            cursor += sprintf(penus->positio + cursor, "%.*s:%u",
                (int)via_k.mensura,
                (constans character*)via_k.datum,
                (insignatus integer)acies[k].linea);
        }
    }
    conspectus->positio = _ch(penus->positio);

    /* fons */
    conspectus->lineae = v->lineae_fontis;
    conspectus->numerus_linearum = v->numerus_linearum;
    conspectus->titulus_fontis = v->via_fontis;
    conspectus->linea_currens = linea_currens;
    si (linea_currens > ZEPHYRUM)
    {
        i32 fenestra = 20;

        si (linea_currens < v->linea_prima + III
            || linea_currens > v->linea_prima + fenestra)
        {
            v->linea_prima = (linea_currens > X)
                ? linea_currens - X : I;
        }
    }
    si (v->linea_prima < I) v->linea_prima = I;
    conspectus->linea_prima = v->linea_prima;

    /* puncta huius viae (lineae) + ordines punctorum */
    {
        i32 numerus = xar_numerus(v->puncta);
        i32 n_lineae = ZEPHYRUM;
        i32 n_ordines = ZEPHYRUM;

        per (k = ZEPHYRUM; k < numerus && k < ORDINES_MAXIMI; k++)
        {
            VindexPunctum* punctum = xar_obtinere(v->puncta, k);
            chorda via_p;
            i32 linea_p = _lineam_radicis(v,
                punctum->functio_index, punctum->instructio,
                &via_p);

            si (linea_p > ZEPHYRUM
                && via_p.mensura == v->via_fontis.mensura
                && memcmp(via_p.datum, v->via_fontis.datum,
                       (memoriae_index)via_p.mensura) == 0)
            {
                penus->puncta_lineae[n_lineae] = linea_p;
                n_lineae++;
            }
            penus->puncta[n_ordines] = punctum->descriptio;
            n_ordines++;
        }
        conspectus->puncta_lineae = penus->puncta_lineae;
        conspectus->puncta_numerus = n_lineae;
        conspectus->puncta = penus->puncta;
        conspectus->puncta_ordines_numerus = n_ordines;
    }

    /* stiva */
    {
        i32 n = ZEPHYRUM;

        per (k = numerus_tabulatorum; k > ZEPHYRUM
            && n < ORDINES_MAXIMI; k--)
        {
            s32 functio_k = -I;
            i32 instructio_k = ZEPHYRUM;

            si (!machinula_positionem_inspicere(v->machinula,
                    (insignatus integer)(k - I), &functio_k,
                    &instructio_k))
            {
                frange;
            }
            {
                constans IndiciumFunctio* ifu = indicium_functio(
                    v->lector, (insignatus integer)functio_k);
                chorda via_k;
                i32 linea_k = _lineam_radicis(v, functio_k,
                    instructio_k, &via_k);
                character cella[256];

                penus->tabulata[n].functio = (ifu != NIHIL)
                    ? _ch_de_off(indicium_chorda(v->lector,
                          ifu->titulus))
                    : _ch("?");
                si (linea_k > ZEPHYRUM)
                {
                    sprintf(cella, "%.*s:%d",
                        (int)((via_k.mensura < 200U)
                            ? via_k.mensura : 200U),
                        (constans character*)via_k.datum,
                        (integer)linea_k);
                }
                alioquin
                {
                    sprintf(cella, "<%d>",
                        (integer)instructio_k);
                }
                penus->tabulata[n].positio = _ch_in_piscinam(
                    v->piscina_effigiei, cella);
                n++;
            }
        }
        conspectus->tabulata = penus->tabulata;
        conspectus->tabulata_numerus = n;
    }

    /* variabilia (functionis summae) */
    si (functio >= ZEPHYRUM)
    {
        constans IndiciumVariabile* varia = NIHIL;
        i32 numerus = (i32)indicium_variabilia_functionis(
            v->lector, (insignatus integer)functio, &varia);
        i32 n = ZEPHYRUM;

        per (k = ZEPHYRUM; k < numerus && n < ORDINES_MAXIMI; k++)
        {
            insignatus longus longus verbum = 0ULL;

            si (machinula_registrum_legere(v->machinula,
                    (insignatus integer)(numerus_tabulatorum - I),
                    varia[k].index_registri, &verbum))
            {
                character cella[64];

                penus->variabilia[n].titulus = _ch_de_off(
                    indicium_chorda(v->lector, varia[k].titulus));
                penus->variabilia[n].typus = _ch_de_off(
                    indicium_chorda(v->lector,
                        varia[k].typus_scriptus));
                _valorem_formare(cella, (s32)varia[k].typus_medulla,
                    (i64)verbum);
                penus->variabilia[n].valor = _ch_in_piscinam(
                    v->piscina_effigiei, cella);
                n++;
            }
        }
        conspectus->variabilia = penus->variabilia;
        conspectus->variabilia_numerus = n;
    }

    /* anulus (cauda novissima prima) */
    {
        i32 n = ZEPHYRUM;

        per (k = ZEPHYRUM; k < ORDINES_MAXIMI; k++)
        {
            integer functio_k = -1;
            insignatus integer instructio_k = 0U;

            si (!machinula_anulum_inspicere(v->machinula,
                    (insignatus integer)k, &functio_k,
                    &instructio_k))
            {
                frange;
            }
            {
                constans IndiciumFunctio* ifu = indicium_functio(
                    v->lector, (insignatus integer)functio_k);
                chorda titulus = (ifu != NIHIL)
                    ? _ch_de_off(indicium_chorda(v->lector,
                          ifu->titulus))
                    : _ch("?");
                chorda via_k;
                i32 linea_k = _lineam_radicis(v, (s32)functio_k,
                    (i32)instructio_k, &via_k);
                character cella[256];

                si (linea_k > ZEPHYRUM)
                {
                    sprintf(cella, "%.*s +%u  %.*s:%d",
                        (int)((titulus.mensura < 40U)
                            ? titulus.mensura : 40U),
                        (constans character*)titulus.datum,
                        instructio_k,
                        (int)((via_k.mensura < 120U)
                            ? via_k.mensura : 120U),
                        (constans character*)via_k.datum,
                        (integer)linea_k);
                }
                alioquin
                {
                    sprintf(cella, "%.*s +%u",
                        (int)((titulus.mensura < 40U)
                            ? titulus.mensura : 40U),
                        (constans character*)titulus.datum,
                        instructio_k);
                }
                penus->anulus[n] = _ch_in_piscinam(
                    v->piscina_effigiei, cella);
                n++;
            }
        }
        conspectus->anulus = penus->anulus;
        conspectus->anulus_numerus = n;
    }

    /* memoria (si basis posita) */
    si (v->memoria_basis != ZEPHYRUM)
    {
        i32 n = ZEPHYRUM;

        per (k = ZEPHYRUM; k < XVI && n < ORDINES_MAXIMI; k++)
        {
            i64 basis = v->memoria_basis + (i64)k * VIII;
            character cella[128];
            integer cursor;
            i32 b;

            cursor = sprintf(cella, "%08llx  ",
                (insignatus longus longus)basis);
            per (b = ZEPHYRUM; b < VIII; b++)
            {
                insignatus character octetus =
                    *(volatilis insignatus character*)
                        (memoriae_index)(basis + (i64)b);

                cursor += sprintf(cella + cursor, "%02x ",
                    (insignatus integer)octetus);
            }
            cursor += sprintf(cella + cursor, " ");
            per (b = ZEPHYRUM; b < VIII; b++)
            {
                insignatus character octetus =
                    *(volatilis insignatus character*)
                        (memoriae_index)(basis + (i64)b);

                cella[cursor] = (octetus >= 32U && octetus < 127U)
                    ? (character)octetus : '.';
                cursor++;
            }
            cella[cursor] = '\0';
            penus->memoria[n] = _ch_in_piscinam(
                v->piscina_effigiei, cella);
            n++;
        }
        conspectus->memoria = penus->memoria;
        conspectus->memoria_numerus = n;
    }

    /* status + imperium */
    conspectus->status = _ch(v->status);
    conspectus->imperium_apertum = v->imperium_apertum;
    {
        chorda imperium;

        imperium.datum = (i8*)v->imperium;
        imperium.mensura = v->imperium_mensura;
        conspectus->imperium = imperium;
    }
    conspectus->tabula_dextra = v->tabula_dextra;
}

/* ==================================================
 * principale
 * ================================================== */

s32
principale (integer argc, character** argv)
{
    Vindex v;
    constans character* via_suitae = NIHIL;
    constans character* plagulae[PLAGULAE_MAXIMAE];
    i32 numerus_plagularum = ZEPHYRUM;
    constans character* imago_argumentum = NIHIL;
    integer a;
    s32 halitus_novissimus = (s32)4;   /* PAUSA initio */

    memset(&v, ZEPHYRUM, magnitudo(Vindex));
    v.radix = ".";
    v.fd_tty_stdout = -1;
    v.fd_tty_stderr = -1;
    v.fd_capturae = -1;
    per (a = 1; a < argc; a++)
    {
        si (strcmp(argv[a], "-plagulae") == 0 && a + 1 < argc)
        {
            character* cursor = argv[++a];

            dum (cursor != NIHIL && *cursor != '\0'
                && numerus_plagularum < (i32)PLAGULAE_MAXIMAE)
            {
                character* virgula = strchr(cursor, ',');

                si (virgula != NIHIL) *virgula = '\0';
                plagulae[numerus_plagularum] = cursor;
                numerus_plagularum++;
                cursor = (virgula != NIHIL) ? virgula + 1 : NIHIL;
            }
        }
        alioquin si (strcmp(argv[a], "-imago") == 0
            && a + 1 < argc)
        {
            imago_argumentum = argv[++a];
        }
        alioquin si (strcmp(argv[a], "-radix") == 0
            && a + 1 < argc)
        {
            v.radix = argv[++a];
        }
        alioquin
        {
            via_suitae = argv[a];
        }
    }
    si (via_suitae == NIHIL)
    {
        fprintf(stderr, "usus: vindex.sh <suita.c> [-plagulae"
            " a,b,c] [-imago via:linea]\n");
        redde II;
    }

    v.piscina_perpetua = piscina_generare_dynamicum(
        "vindex_perpetua", 16777216);
    v.piscina_effigiei = piscina_generare_dynamicum(
        "vindex_effigies", 4194304);
    si (v.piscina_perpetua == NIHIL || v.piscina_effigiei == NIHIL)
    {
        fprintf(stderr, "vindex: piscinae desunt\n");
        redde II;
    }
    v.puncta = xar_creare(v.piscina_perpetua,
        (i32)magnitudo(VindexPunctum));
    sprintf(v.status,
        "g gradus | s super | i intra | f fini | p perge |"
        " v/a/u/m tabulae | : imperium | q exi");

    /* onus mundi */
    fprintf(stderr, "vindex: mundum onerans...\n");
    v.onerator = vindex_onerator_creare(v.radix);
    si (v.onerator == NIHIL)
    {
        fprintf(stderr, "vindex: onerator fractus (curre ex"
            " radice repositorii)\n");
        redde II;
    }
    {
        character via_lib[600];
        DIR* dir;
        structura dirent* introitus;
        character* viae[512];
        integer numerus_viarum = 0;
        integer i;

        sprintf(via_lib, "%s/lib", v.radix);
        dir = opendir(via_lib);
        si (dir == NIHIL)
        {
            fprintf(stderr, "vindex: lib/ deest\n");
            redde II;
        }
        dum ((introitus = readdir(dir)) != NIHIL
            && numerus_viarum < 512)
        {
            memoriae_index m = strlen(introitus->d_name);
            b32 congruit = (b32)(numerus_plagularum == ZEPHYRUM);
            i32 p;

            si (m < III || introitus->d_name[m - II] != '.'
                || introitus->d_name[m - I] != 'c')
            {
                perge;
            }
            per (p = ZEPHYRUM; p < numerus_plagularum; p++)
            {
                si (strstr(introitus->d_name, plagulae[p])
                    != NIHIL)
                {
                    congruit = VERUM;
                }
            }
            si (!congruit) perge;
            {
                character* via = piscina_allocare(
                    v.piscina_perpetua, m + VI);

                sprintf(via, "lib/%s", introitus->d_name);
                viae[numerus_viarum] = via;
                numerus_viarum++;
            }
        }
        closedir(dir);
        per (i = 0; i < numerus_viarum; i++)
        {
            si (!vindex_onerator_demittere(v.onerator, viae[i]))
            {
                fprintf(stderr, "vindex: [demissio fracta] %s\n",
                    viae[i]);
            }
        }
    }
    si (!vindex_onerator_demittere(v.onerator, via_suitae))
    {
        fprintf(stderr, "vindex: suita non demissa: %s\n",
            via_suitae);
        redde II;
    }
    {
        character via_indicii[600];

        sprintf(via_indicii, "%s/officina/build/vindex.indicium",
            v.radix);
        v.machinula = vindex_onerator_nectere(v.onerator,
            via_indicii);
    }
    si (v.machinula == NIHIL)
    {
        OfficinaChorda querela = vindex_onerator_querela(
            v.onerator);

        fprintf(stderr, "vindex: nexus fractus (%.*s) - amplia"
            " -plagulae\n", (int)querela.mensura,
            (constans character*)querela.datum);
        redde II;
    }
    v.lector = vindex_onerator_lector(v.onerator);
    si (!machinula_aperire(v.machinula, _och(_ch("main"))))
    {
        fprintf(stderr, "vindex: main non inventum\n");
        redde II;
    }

    /* -imago: fumus sine terminale */
    si (imago_argumentum != NIHIL)
    {
        TesseraPiscina* tp = tessera_piscina_generare_dynamicum(
            "vindex_imago", 4194304);
        TesseraPonsMemoriae* pm = tessera_pons_memoriae_creare(tp,
            120U, 40U);
        TesseraOpus* opus = tessera_aperire(tp, &pm->pons);
        VindexConspectus conspectus;
        interior ConspectusPenus penus;

        si (opus == NIHIL)
        {
            fprintf(stderr, "vindex: opus imaginis deest\n");
            redde II;
        }
        {
            s32 functio = -I;
            i32 instructio = ZEPHYRUM;
            chorda descriptio;

            si (!_punctum_de_argumento(&v, _ch(imago_argumentum),
                    &functio, &instructio, &descriptio)
                || !machinula_punctum_ponere(v.machinula,
                       (integer)functio,
                       (insignatus integer)instructio))
            {
                fprintf(stderr, "vindex: punctum -imago non"
                    " positum: %s\n", imago_argumentum);
                redde II;
            }
            {
                VindexPunctum* punctum = xar_addere(v.puncta);

                punctum->functio_index = functio;
                punctum->instructio = instructio;
                punctum->descriptio = _ch(imago_argumentum);
            }
        }
        halitus_novissimus = (s32)machinula_pergere(v.machinula);
        _conspectum_aedificare(&v, &conspectus, &penus,
            halitus_novissimus);
        vindex_visum_pingere(&conspectus, opus);
        /* effigies -> stdout ut textus */
        {
            integer y;
            integer x;

            per (y = 0; y < (integer)tessera_altitudo(opus); y++)
            {
                per (x = 0; x < (integer)tessera_latitudo(opus);
                    x++)
                {
                    TesseraCellula cellula =
                        tessera_cellulam_legere(opus, x, y);

                    putchar((cellula.signum >= 32U
                        && cellula.signum < 127U)
                        ? (integer)cellula.signum : (integer)' ');
                }
                putchar('\n');
            }
        }
        redde ZEPHYRUM;
    }

    /* ansa TUI (terminal verum) */
    {
        TesseraPiscina* tp = tessera_piscina_generare_dynamicum(
            "vindex_tui", 8388608);
        TesseraPons* pons = tessera_pons_posix_creare(tp);
        TesseraOpus* opus;
        TesseraLector* lector_clavium;
        b32 currendum = VERUM;
        interior ConspectusPenus penus;

        si (pons == NIHIL)
        {
            fprintf(stderr, "vindex: terminal deest (proba"
                " -imago pro fumo sine terminale)\n");
            redde II;
        }
        /* captura effusionis interpretatae (phantasmata voluminis
         * aliter - vide _vm_capere) */
        {
            character via_capturae[600];

            sprintf(via_capturae,
                "%s/officina/build/vindex.stdout", v.radix);
            v.fd_capturae = open(via_capturae,
                O_WRONLY | O_CREAT | O_TRUNC, 0644);
            si (v.fd_capturae >= 0)
            {
                v.fd_tty_stdout = dup(1);
                v.fd_tty_stderr = dup(2);
                si (v.fd_tty_stdout < 0 || v.fd_tty_stderr < 0)
                {
                    v.fd_capturae = -1;   /* sine captura */
                }
                alioquin
                {
                    sprintf(v.status, "effusio interpretata ->"
                        " officina/build/vindex.stdout |"
                        " g/s/i/f/p claves | : imperium | q exi");
                }
            }
        }
        opus = tessera_aperire(tp, pons);
        lector_clavium = tessera_lector_creare(tp, pons);
        si (opus == NIHIL || lector_clavium == NIHIL)
        {
            fprintf(stderr, "vindex: tessera fracta\n");
            redde II;
        }
        dum (currendum)
        {
            VindexConspectus conspectus;
            TesseraEventum eventum;
            TesseraEventumGenus genus;

            _conspectum_aedificare(&v, &conspectus, &penus,
                halitus_novissimus);
            vindex_visum_pingere(&conspectus, opus);
            (vacuum)tessera_praesentare(opus);
            genus = tessera_eventum_expectare(lector_clavium,
                &eventum, 250);
            si (genus == TESSERA_EVENTUM_AMPLITUDO
                || genus == TESSERA_EVENTUM_RESUMPTUM)
            {
                (vacuum)tessera_magnitudinem_renovare(opus);
                perge;
            }
            si (genus != TESSERA_EVENTUM_CLAVIS) perge;

            si (v.imperium_apertum)
            {
                si (eventum.clavis == TESSERA_CLAVIS_REDITUS)
                {
                    v.imperium[v.imperium_mensura] = '\0';
                    v.imperium_apertum = FALSUM;
                    _imperium_exsequi(&v);
                    v.imperium_mensura = ZEPHYRUM;
                }
                alioquin si (eventum.clavis == TESSERA_CLAVIS_FUGA)
                {
                    v.imperium_apertum = FALSUM;
                    v.imperium_mensura = ZEPHYRUM;
                }
                alioquin si (eventum.clavis
                    == TESSERA_CLAVIS_RETRORSUM)
                {
                    si (v.imperium_mensura > ZEPHYRUM)
                    {
                        v.imperium_mensura--;
                    }
                }
                alioquin si (eventum.runa >= 32
                    && eventum.runa < 127
                    && v.imperium_mensura
                        < (i32)(IMPERIUM_MAXIMUM - I))
                {
                    v.imperium[v.imperium_mensura] =
                        (character)eventum.runa;
                    v.imperium_mensura++;
                }
                perge;
            }

            si (eventum.clavis == TESSERA_CLAVIS_SURSUM)
            {
                si (v.linea_prima > I) v.linea_prima--;
                perge;
            }
            si (eventum.clavis == TESSERA_CLAVIS_DEORSUM)
            {
                v.linea_prima++;
                perge;
            }
            commutatio (eventum.runa)
            {
            casus 'q':
                currendum = FALSUM;
                frange;
            casus ':':
                v.imperium_apertum = VERUM;
                v.imperium_mensura = ZEPHYRUM;
                frange;
            casus 'g':
                _gradus_unus(&v);
                halitus_novissimus = (s32)4;
                frange;
            casus 's':
                _super_lineam(&v, FALSUM);
                halitus_novissimus = (s32)4;
                frange;
            casus 'i':
                _super_lineam(&v, VERUM);
                halitus_novissimus = (s32)4;
                frange;
            casus 'f':
                _fini(&v);
                halitus_novissimus = (s32)4;
                frange;
            casus 'p':
                _vm_capere(&v);
                halitus_novissimus =
                    (s32)machinula_pergere(v.machinula);
                _vm_solvere(&v);
                frange;
            casus 'v':
                v.tabula_dextra = (s32)VINDEX_TABULA_VARIABILIA;
                frange;
            casus 'a':
                v.tabula_dextra = (s32)VINDEX_TABULA_ANULUS;
                frange;
            casus 'u':
                v.tabula_dextra = (s32)VINDEX_TABULA_PUNCTA;
                frange;
            casus 'm':
                v.tabula_dextra = (s32)VINDEX_TABULA_MEMORIA;
                frange;
            ordinarius:
                frange;
            }
        }
        tessera_claudere(opus);
    }
    redde ZEPHYRUM;
}
