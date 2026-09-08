/* oratio_vocabularium_la.c - Vide oratio_vocabularium_la.h. */

#include "oratio_vocabularium_la.h"
#include "oratio_glossarium.h"
#include "nuntium.h"
#include "tabula_dispersa.h"
#include <stdlib.h>
#include <string.h>


#define STIRPS_MAXIMA   18
#define FINIS_MAXIMUS   7

/* nodus catenae: index recordi et clavis stirpis; proximus -I = finis */
nomen structura {
    s32 index;
    i32 clavis;
    s32 proximus;
} Nodus;

/* caput catenae in tabula dispersa (valor = indicator ad Caput) */
nomen structura {
    s32 primus;
} Caput;

structura OratioVocabulariumLa {
    Xar* stirpes;          /* OratioStirps */
    Xar* flexiones;        /* OratioFlexio */
    Xar* additamenta;      /* OratioAdditamentum */
    Xar* unica;            /* OratioUnicum */
    Xar* nodi;             /* Nodus (catenae omnium tabularum) */
    TabulaDispersa* per_stirpem;     /* stirps plicata -> Caput */
    TabulaDispersa* per_finem;       /* finis plicatus (non vacuus) -> Caput */
    TabulaDispersa* per_unicum;      /* forma unica plicata -> Caput */
        Xar* flexiones_vacuae;           /* s32: flexiones fine vacuo */
    Xar* stirpes_vacuae;             /* Nodus: stirpes vacuae (sum: '' + 'essem'; Bdl WORDS) */

        Xar* tackones;                   /* s32: additamenta TACKON basi X */
    OratioVocabulariumCensus census;
    constans OratioGlossarium* glossarium;   /* T9: fons primus, NIHIL = nullum */
};


/* ==================================================
 * Plicatio
 * ================================================== */

interior chorda
_chorda (
     i8* datum,
    i32  mensura)
{
    chorda c;

    c.datum    = datum;
    c.mensura  = mensura;
    redde c;
}

/* octetum secundum sequentiae C4/C5 in litteram planam; 0 = non
 * mappatum */
interior i8
_macron_planum (
    i8 primus,
    i8 secundus)
{
    si (primus == 0xC4)
    {
        commutatio (secundus)
        {
            casus 0x80: casus 0x81: casus 0x82: casus 0x83: redde 'a';
            casus 0x92: casus 0x93: casus 0x94: casus 0x95: redde 'e';
            casus 0xAA: casus 0xAB: casus 0xAC: casus 0xAD: redde 'i';
            ordinarius: redde ZEPHYRUM;
        }
    }
    si (primus == 0xC5)
    {
        commutatio (secundus)
        {
            casus 0x8C: casus 0x8D: casus 0x8E: casus 0x8F: redde 'o';
            casus 0xAA: casus 0xAB: casus 0xAC: casus 0xAD: redde 'u';
            ordinarius: redde ZEPHYRUM;
        }
    }
    redde ZEPHYRUM;
}

chorda
oratio_vocabularium_la_plicare (
    Piscina* piscina,
     chorda  forma)
{
    i8* datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)forma.mensura + I);
    i32 i = ZEPHYRUM;
    i32 o = ZEPHYRUM;

    si (datum == NIHIL)
    {
        redde _chorda(NIHIL, ZEPHYRUM);
    }
    dum (i < forma.mensura)
    {
        i8 c = forma.datum[i];

        si (c < 0x80)
        {
            si (c >= 'A' && c <= 'Z')
            {
                c = (i8)(c - 'A' + 'a');
            }
            si (c == 'v')
            {
                c = 'u';
            }
            alioquin si (c == 'j')
            {
                c = 'i';
            }
            datum[o]  = c;
            o         = o + I;
            i         = i + I;
            perge;
        }
        si (i + I < forma.mensura)
        {
            i8 d = forma.datum[i + I];

            /* ligaturae: C3 86/A6 ae, C5 92/93 oe */
            si (c == 0xC3 && (d == 0x86 || d == 0xA6))
            {
                datum[o]      = 'a';
                datum[o + I]  = 'e';
                o             = o + (i32)II;
                i             = i + (i32)II;
                perge;
            }
            si (c == 0xC5 && (d == 0x92 || d == 0x93))
            {
                datum[o]      = 'o';
                datum[o + I]  = 'e';
                o             = o + (i32)II;
                i             = i + (i32)II;
                perge;
            }
            si (_macron_planum(c, d) != ZEPHYRUM)
            {
                datum[o]  = _macron_planum(c, d);
                o         = o + I;
                i         = i + (i32)II;
                perge;
            }
        }
        datum[o]  = c;
        o         = o + I;
        i         = i + I;
    }
    redde _chorda(datum, o);
}


/* ==================================================
 * Onus
 * ================================================== */

interior b32
_catenare (
    OratioVocabulariumLa* voc,
          TabulaDispersa* tabula,
                  chorda  clavis,
                     s32  index,
                     i32  clavis_stirpis)
{
    vacuum* valor = NIHIL;
     Caput* caput;
     Nodus* nodus;
       s32  n = (s32)xar_numerus(voc->nodi);

    nodus = (Nodus*)xar_addere(voc->nodi);
    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    nodus->index   = index;
    nodus->clavis  = clavis_stirpis;
    si (tabula_dispersa_invenire(tabula, clavis, &valor))
    {
        caput            = (Caput*)valor;
        nodus->proximus  = caput->primus;
        caput->primus    = n;
        redde VERUM;
    }
    caput = (Caput*)piscina_allocare(voc->per_stirpem->piscina,
        (memoriae_index)magnitudo(Caput));
    si (caput == NIHIL)
    {
        redde FALSUM;
    }
    caput->primus    = n;
    nodus->proximus  = (s32)-I;
    redde tabula_dispersa_inserere(tabula, clavis, caput);
}

interior s32
_primus (
    constans OratioVocabulariumLa* voc,
                   TabulaDispersa* tabula,
                           chorda  clavis)
{
    vacuum* valor = NIHIL;

    (vacuum)voc;
    si (!tabula_dispersa_invenire(tabula, clavis, &valor))
    {
        redde (s32)-I;
    }
    redde ((Caput*)valor)->primus;
}

interior constans Nodus*
_nodus (
    constans OratioVocabulariumLa* voc,
                              s32  i)
{
    redde (constans Nodus*)xar_obtinere(voc->nodi, (i32)i);
}

interior b32
_aequalis_literis (
                chorda  c,
    constans character* literae)
{
    i32 l = (i32)strlen(literae);

    redde (b32)(c.mensura == l
        && (l == ZEPHYRUM
        || memcmp(c.datum, literae, (size_t)l) == ZEPHYRUM));
}

OratioVocabulariumLa*
oratio_vocabularium_la_onerare (
                     Piscina* piscina,
                      chorda  tabula,
    OratioVocabulariumVitium* vitium)
{
    OratioVocabulariumLa* voc;
           NuntiumLector* l;
                     i32  tag;
                     i32  wire;
                     s32  i;

    si (!oratio_vocabularium_recensere(piscina, tabula, NIHIL, vitium))
    {
        redde NIHIL;
    }
    voc = (OratioVocabulariumLa*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(OratioVocabulariumLa));
    si (voc == NIHIL)
    {
        redde NIHIL;
    }
    memset(voc, ZEPHYRUM, magnitudo(*voc));
    voc->stirpes    = xar_creare(piscina,
        (i32)magnitudo(OratioStirps));
    voc->flexiones  = xar_creare(piscina,
        (i32)magnitudo(OratioFlexio));
    voc->additamenta      = xar_creare(piscina,
        (i32)magnitudo(OratioAdditamentum));
    voc->unica = xar_creare(piscina,
        (i32)magnitudo(OratioUnicum));
    voc->nodi = xar_creare(piscina, (i32)magnitudo(Nodus));
        voc->flexiones_vacuae = xar_creare(piscina,
            (i32)magnitudo(s32));
    voc->stirpes_vacuae = xar_creare(piscina, (i32)magnitudo(Nodus));

    voc->tackones          = xar_creare(piscina, (i32)magnitudo(s32));
    voc->per_stirpem = tabula_dispersa_creare_chorda(piscina,
        (i32)131072);
    voc->per_finem = tabula_dispersa_creare_chorda(piscina,
        (i32)4096);
    voc->per_unicum = tabula_dispersa_creare_chorda(piscina,
        (i32)256);
    si (   voc->stirpes == NIHIL || voc->flexiones == NIHIL
        || voc->additamenta == NIHIL || voc->unica == NIHIL
        || voc->nodi == NIHIL || voc->flexiones_vacuae == NIHIL
        || voc->stirpes_vacuae == NIHIL

        || voc->tackones == NIHIL || voc->per_stirpem == NIHIL
        || voc->per_finem == NIHIL || voc->per_unicum == NIHIL)
    {
        redde NIHIL;
    }

    /* recorda (recensio iam probavit fluxum) */
    l = nuntium_lector_creare(piscina, tabula);
    si (l == NIHIL)
    {
        redde NIHIL;
    }
    dum (nuntium_legere_field(l, &tag, &wire))
    {
        chorda nidus = nuntium_legere_nuntium(l);

        commutatio (tag)
        {
            casus ORATIO_VOC_STIRPS:
            {
                OratioStirps* r =
                    (OratioStirps*)xar_addere(voc->stirpes);

                si (   r == NIHIL
                    || !oratio_vocabularium_stirps_legere(piscina,
                    nidus, r))
                {
                    redde NIHIL;
                }
                frange;
            }
            casus ORATIO_VOC_FLEXIO:
            {
                OratioFlexio* r =
                    (OratioFlexio*)xar_addere(voc->flexiones);

                si (   r == NIHIL
                    || !oratio_vocabularium_flexio_legere(piscina,
                    nidus, r))
                {
                    redde NIHIL;
                }
                frange;
            }
            casus ORATIO_VOC_ADDITAMENTUM:
            {
                OratioAdditamentum* r =
                    (OratioAdditamentum*)xar_addere(voc->additamenta);

                si (   r == NIHIL
                    || !oratio_vocabularium_additamentum_legere(piscina,
                    nidus,
                        r))
                {
                    redde NIHIL;
                }
                frange;
            }
            casus ORATIO_VOC_UNICUM:
            {
                OratioUnicum* r = (OratioUnicum*)xar_addere(voc->unica);

                si (   r == NIHIL
                    || !oratio_vocabularium_unicum_legere(piscina,
                    nidus, r))
                {
                    redde NIHIL;
                }
                frange;
            }
            ordinarius:
                frange;
        }
    }
    voc->census.stirpes      = xar_numerus(voc->stirpes);
    voc->census.flexiones    = xar_numerus(voc->flexiones);
    voc->census.additamenta  = xar_numerus(voc->additamenta);
    voc->census.unica        = xar_numerus(voc->unica);
    voc->census.versio       = (i32)ORATIO_VOCABULARIUM_VERSIO;

    /* tabulae per clavem plicatam */
    per (i = ZEPHYRUM; i < (s32)xar_numerus(voc->stirpes); i++)
    {
        constans OratioStirps* r = (constans OratioStirps*)xar_obtinere(
            voc->stirpes, (i32)i);
        i32 k;

        per (k = ZEPHYRUM; k < (i32)IV; k++)
        {
                        chorda s = r->stirpes[k];

            si (_aequalis_literis(s, "zzz"))
            {
                perge;
            }
                        si (s.mensura == ZEPHYRUM)
                        {
                /* stirps vacua INTERNA (sum: 's' '' 'fu' 'fut' - 'essem' 'sit'
                 * clave II) in lista propria, ut WORDS Bdl(1); stirpes vacuae
                 * FINALES (nomina stirpibus II) nihil sunt, non stirpes */
                Nodus* nd;
                  i32  j;
                  b32  interna = FALSUM;

                per (j = k + I; j < (i32)IV; j++)
                {
                    si (   r->stirpes[j].mensura > ZEPHYRUM
                        && !_aequalis_literis(r->stirpes[j], "zzz"))
                    {
                        interna = VERUM;
                    }
                }
                si (!interna)
                {
                    perge;
                }

                nd = (Nodus*)xar_addere(voc->stirpes_vacuae);
                si (nd == NIHIL)
                {
                    redde NIHIL;
                }
                nd->index     = i;
                nd->clavis    = k + I;
                nd->proximus  = (s32)-I;
                perge;
                        }

            si (!_catenare(voc, voc->per_stirpem,
                    oratio_vocabularium_la_plicare(piscina, s), i, k
                        + I))
            {
                redde NIHIL;
            }
        }
    }
    per (i = ZEPHYRUM; i < (s32)xar_numerus(voc->flexiones); i++)
    {
        constans OratioFlexio* r = (constans OratioFlexio*)xar_obtinere(
            voc->flexiones, (i32)i);

        si (r->finis.mensura == ZEPHYRUM)
        {
            s32* locus = (s32*)xar_addere(voc->flexiones_vacuae);

            si (locus == NIHIL)
            {
                redde NIHIL;
            }
            *locus = i;
        }
        alioquin si (!_catenare(voc, voc->per_finem,
                     oratio_vocabularium_la_plicare(piscina, r->finis),
                     i,
                     ZEPHYRUM))
        {
            redde NIHIL;
        }
    }
    per (i = ZEPHYRUM; i < (s32)xar_numerus(voc->unica); i++)
    {
        constans OratioUnicum* r = (constans OratioUnicum*)xar_obtinere(
            voc->unica, (i32)i);

        si (!_catenare(voc, voc->per_unicum,
                oratio_vocabularium_la_plicare(piscina, r->forma), i,
                ZEPHYRUM))
        {
            redde NIHIL;
        }
    }
    per (i = ZEPHYRUM; i < (s32)xar_numerus(voc->additamenta); i++)
    {
        constans OratioAdditamentum* r =
            (constans OratioAdditamentum*)xar_obtinere(voc->additamenta,
            (i32)i);

        si (   r->genus == ORATIO_PARS_TACKON
            && _aequalis_literis(r->descriptio, "X"))
        {
            s32* locus = (s32*)xar_addere(voc->tackones);

            si (locus == NIHIL)
            {
                redde NIHIL;
            }
            *locus = i;
        }
    }
    redde voc;
}


/* ==================================================
 * Congruentia stirpis et flexionis (Reduce_Stem_List transcripta)
 * ================================================== */

/* lexema n (0-basatum) chordae spatiis separatae; vacua si deest */
interior chorda
_lexema_n (
    chorda c,
       i32 n)
{
    i32 i = ZEPHYRUM;
    i32 k = ZEPHYRUM;

    dum (i < c.mensura)
    {
        i32 a;

        dum (i < c.mensura && c.datum[i] == ' ')
        {
            i = i + I;
        }
        si (i >= c.mensura)
        {
            frange;
        }
        a = i;
        dum (i < c.mensura && c.datum[i] != ' ')
        {
            i = i + I;
        }
        si (k == n)
        {
            redde _chorda(c.datum + a, i - a);
        }
        k = k + I;
    }
    redde _chorda(NIHIL, ZEPHYRUM);
}

interior i8
_littera_prima (
    chorda c)
{
    redde c.mensura > ZEPHYRUM ? c.datum[ZEPHYRUM] : (i8)'X';
}

interior OratioPars
_pars_effectiva (
    OratioPars pars)
{
    si (pars == ORATIO_PARS_VPAR || pars == ORATIO_PARS_SUPINE)
    {
        redde ORATIO_PARS_V;
    }
    redde pars;
}

/* declinatio/varians: flexio (dextra) 0 0 = quaelibet praeter IX;
 * d 0 = declinatio d */
interior b32
_decn_congruit (
    constans OratioStirps* st,
    constans OratioFlexio* fl)
{
    si (fl->declinatio == ZEPHYRUM && fl->varians == ZEPHYRUM)
    {
        redde (b32)(st->declinatio != (i32)IX);
    }
    si (fl->varians == ZEPHYRUM)
    {
        redde (b32)(st->declinatio == fl->declinatio);
    }
    redde (b32)(st->declinatio == fl->declinatio
        && st->varians == fl->varians);
}

/* gradus ex clave stirpis (Adj_Comp_From_Key / Adv_Comp_From_Key): stirps
 * X = gradus omnes per stirpes suas - ADJ I II POS, III COMP, IV SUPER;
 * ADV I POS, II COMP, III SUPER. Sine hoc 'amare' ADV X quater
 * rediret (X POS COMP SUPER clave I omnes) - WORDS per clavem gradum
 * ponit et duplicata premit. */
interior i8
_gradus_ex_clave (
    OratioPars pars,
           i32 clavis)
{
    si (pars == ORATIO_PARS_ADV)
    {
        redde clavis == I ? 'P' : clavis == (i32)II ? 'C' : 'S';
    }
    redde clavis <= (i32)II ? 'P' : clavis == (i32)III ? 'C' : 'S';
}

interior b32
_gradus_congruit (
    OratioPars pars,
           i32 clavis,
            i8 stirpis,
            i8 flexionis)
{
    si (stirpis == 'X')
    {
        redde (b32)(flexionis == 'X'
            || flexionis == _gradus_ex_clave(pars, clavis));
    }
    redde (b32)(stirpis == flexionis || flexionis == 'X');
}

interior b32
_congruit (
    constans OratioStirps* st,
                      i32  clavis,
    constans OratioFlexio* fl)
{
    OratioPars pars = _pars_effectiva(fl->pars);

    /* pars: PACK dictionarii flexiones PRON accipit */
    si (!(st->pars == pars
          || (st->pars == ORATIO_PARS_PACK
              && pars == ORATIO_PARS_PRON)))
    {
        redde FALSUM;
    }
    /* clavis stirpis (numerale: exacta) */
    si (st->pars == ORATIO_PARS_NUM)
    {
        si (clavis != fl->clavis)
        {
            redde FALSUM;
        }
    }
    alioquin si (fl->clavis != ZEPHYRUM && clavis != fl->clavis)
    {
        redde FALSUM;
    }
    commutatio (st->pars)
    {
        casus ORATIO_PARS_N:
        {
            i8 g_st = _littera_prima(_lexema_n(st->species, ZEPHYRUM));
            i8 g_fl = _littera_prima(_lexema_n(fl->accidentia,
                (i32)II));

            redde (b32)(_decn_congruit(st, fl)
                && (g_st == g_fl || g_fl == 'X'
                    || (g_fl == 'C' && g_st != 'N')));
        }
        casus ORATIO_PARS_PRON:
        casus ORATIO_PARS_PACK:
        casus ORATIO_PARS_NUM:
        casus ORATIO_PARS_V:
            redde _decn_congruit(st, fl);
                casus ORATIO_PARS_ADJ:
            redde (b32)(_decn_congruit(st, fl)
                && _gradus_congruit(ORATIO_PARS_ADJ, clavis,
                    _littera_prima(_lexema_n(st->species, ZEPHYRUM)),
                    _littera_prima(_lexema_n(fl->accidentia,
                    (i32)III))));
                casus ORATIO_PARS_ADV:
        {
            /* flexiones ADV: X clavibus I-III (entria X: gradus ex clave),
             * POS/COMP/SUPER clave I (entria gradus expressi). Exacte,
             * ne 'amare' bis (X et POS) rediret */
            i8 g_st = _littera_prima(_lexema_n(st->species, ZEPHYRUM));
            i8 g_fl = _littera_prima(_lexema_n(fl->accidentia,
                ZEPHYRUM));

            (vacuum)clavis;
            redde (b32)(g_st == 'X' ? g_fl == 'X' : g_fl == g_st);
        }


        casus ORATIO_PARS_PREP:
        {
            chorda c_st = _lexema_n(st->species, ZEPHYRUM);
            chorda c_fl = _lexema_n(fl->accidentia, ZEPHYRUM);

            redde (b32)(c_st.mensura == c_fl.mensura
                && memcmp(c_st.datum, c_fl.datum, (size_t)c_st.mensura)
                    == ZEPHYRUM);
        }
        ordinarius:
            redde VERUM;   /* CONJ INTERJ: pars sola */
    }
}


/* ==================================================
 * Quaesitio
 * ================================================== */

interior b32
_addere (
                    Xar* exitus,
    OratioAnalysisGenus  genus,
                    s32  stirps,
                    s32  flexio,
                    s32  unicum,
                    s32  tackon,
                    i32  clavis,
                    i32  mensura_stirpis)
{
    OratioAnalysis* a = (OratioAnalysis*)xar_addere(exitus);

    si (a == NIHIL)
    {
        redde FALSUM;
    }
        a->genus        = genus;
    a->stirps           = stirps;
    a->flexio           = flexio;
    a->unicum           = unicum;
    a->tackon           = tackon;
        a->glossarium   = (s32)-I;
    a->clavis           = clavis;
    a->mensura_stirpis  = mensura_stirpis;
    a->varians          = (s32)-I;
    redde VERUM;
}

/* glossarium domus: fons primus - formae eius ante omnia */
interior b32
_glossarium_quaerere (
                          Piscina* piscina,
    constans OratioVocabulariumLa* voc,
                           chorda  f,
                              Xar* exitus)
{
    Xar* formae;
    i32  i;

    si (voc->glossarium == NIHIL)
    {
        redde VERUM;
    }
    formae = oratio_glossarium_quaerere(piscina, voc->glossarium, f);
    si (formae == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(formae); i++)
    {
        OratioAnalysis* a;

        si (!_addere(exitus, ORATIO_ANALYSIS_GLOSSARIUM, (s32)-I,
            (s32)-I,
                (s32)-I, (s32)-I, ZEPHYRUM, f.mensura))
        {
            redde FALSUM;
        }
        a = (OratioAnalysis*)xar_obtinere(exitus, xar_numerus(exitus)
            - I);
        a->glossarium = *(s32*)xar_obtinere(formae, i);
    }
    redde VERUM;
}

interior b32
_unica_quaerere (
    constans OratioVocabulariumLa* voc,
                           chorda  f,
                              Xar* exitus,
                              s32  tackon)
{
    s32 n = _primus(voc, voc->per_unicum, f);

    dum (n >= ZEPHYRUM)
    {
        constans Nodus* nd = _nodus(voc, n);

        si (!_addere(exitus, ORATIO_ANALYSIS_UNICUM, (s32)-I, (s32)-I,
                nd->index, tackon, ZEPHYRUM, f.mensura))
        {
            redde FALSUM;
        }
        n = nd->proximus;
    }
    redde VERUM;
}

interior b32
_flexa_quaerere (
    constans OratioVocabulariumLa* voc,
                           chorda  f,
                              Xar* exitus,
                              s32  tackon)
{
    i32 z;

        per (z = ZEPHYRUM; z <= (i32)FINIS_MAXIMUS
            && z <= f.mensura; z++)
        {
           i32 ms      = f.mensura - z;
        chorda stirps  = _chorda(f.datum, ms);
        chorda finis   = _chorda(f.datum + ms, z);
           s32 ns;

        si (ms > (i32)STIRPS_MAXIMA)
        {
            perge;
        }
        si (ms == ZEPHYRUM)
        {
            /* forma tota finis: stirpes vacuae (sum) */
            s32 nf = _primus(voc, voc->per_finem, finis);

            dum (nf >= ZEPHYRUM)
            {
                       constans Nodus* ndf = _nodus(voc, nf);
                constans OratioFlexio* fl =
                    oratio_vocabularium_la_flexio(voc,
                    ndf->index);
                i32 v;

                per (v = ZEPHYRUM; v
                    < xar_numerus(voc->stirpes_vacuae); v++)
                {
                    constans Nodus* nd =
                        (constans Nodus*)xar_obtinere(voc->stirpes_vacuae,
                        v);

                    si (   _congruit(oratio_vocabularium_la_stirps(voc,
                        nd->index),
                            nd->clavis, fl)
                        && !_addere(exitus, ORATIO_ANALYSIS_STIRPS,
                        nd->index,
                            ndf->index, (s32)-I, tackon, nd->clavis,
                            ZEPHYRUM))
                    {
                        redde FALSUM;
                    }
                }
                nf = ndf->proximus;
            }
            perge;
        }
        ns = _primus(voc, voc->per_stirpem, stirps);
        si (ns < ZEPHYRUM)
        {
            perge;
        }
        si (z == ZEPHYRUM)

        {
            i32 v;

            per (v = ZEPHYRUM; v
                < xar_numerus(voc->flexiones_vacuae); v++)
            {
                s32 fi = *(s32*)xar_obtinere(voc->flexiones_vacuae, v);
                constans OratioFlexio* fl =
                    oratio_vocabularium_la_flexio(voc,
                    fi);
                s32 n = ns;

                dum (n >= ZEPHYRUM)
                {
                    constans Nodus* nd = _nodus(voc, n);

                    si (   _congruit(oratio_vocabularium_la_stirps(voc,
                        nd->index),
                            nd->clavis, fl)
                        && !_addere(exitus, ORATIO_ANALYSIS_STIRPS,
                        nd->index,
                            fi, (s32)-I, tackon, nd->clavis, ms))
                    {
                        redde FALSUM;
                    }
                    n = nd->proximus;
                }
            }
        }
        alioquin
        {
            s32 nf = _primus(voc, voc->per_finem, finis);

            dum (nf >= ZEPHYRUM)
            {
                       constans Nodus* ndf = _nodus(voc, nf);
                constans OratioFlexio* fl =
                    oratio_vocabularium_la_flexio(voc,
                    ndf->index);
                s32 n = ns;

                dum (n >= ZEPHYRUM)
                {
                    constans Nodus* nd = _nodus(voc, n);

                    si (   _congruit(oratio_vocabularium_la_stirps(voc,
                        nd->index),
                            nd->clavis, fl)
                        && !_addere(exitus, ORATIO_ANALYSIS_STIRPS,
                        nd->index,
                            ndf->index, (s32)-I, tackon, nd->clavis,
                            ms))
                    {
                        redde FALSUM;
                    }
                    n = nd->proximus;
                }
                nf = ndf->proximus;
            }
        }
        }
    redde VERUM;
}

/* ordo WORDS intra [ab, ad): stirps ordine plagulae, finis longior
 * prior, flexio ordine plagulae; unica et tackon manent ubi sunt */
interior b32
_ante (
    constans OratioVocabulariumLa* voc,
          constans OratioAnalysis* a,
          constans OratioAnalysis* b)
{
    i32 la;
    i32 lb;

    si (   a->genus != ORATIO_ANALYSIS_STIRPS
        || b->genus != ORATIO_ANALYSIS_STIRPS)
    {
        redde FALSUM;
    }
    si (a->stirps != b->stirps)
    {
        redde (b32)(a->stirps < b->stirps);
    }
    la = oratio_vocabularium_la_flexio(voc, a->flexio)->finis.mensura;
    lb = oratio_vocabularium_la_flexio(voc, b->flexio)->finis.mensura;
    si (la != lb)
    {
        redde (b32)(la > lb);
    }
    redde (b32)(a->flexio < b->flexio);
}

interior vacuum
_ordinare (
    constans OratioVocabulariumLa* voc,
                              Xar* exitus,
                              i32  ab,
                              i32  ad)
{
    i32 i;

    per (i = ab + I; i < ad; i++)
    {
        OratioAnalysis x = *(OratioAnalysis*)xar_obtinere(exitus, i);
                   i32 j = i;

        dum (   j > ab
             && _ante(voc, &x, (OratioAnalysis*)xar_obtinere(exitus, j
                - I)))
        {
            *(OratioAnalysis*)xar_obtinere(exitus, j) =
                *(OratioAnalysis*)xar_obtinere(exitus, j - I);
            j = j - I;
        }
        *(OratioAnalysis*)xar_obtinere(exitus, j) = x;
    }
}

/* CORRESPONDENTIAE orthographiae mediae (T22, contractus IV) ordine
 * praecisionis censae (scratch orthographia.py, chartae LLCT: formae
 * ignotae MMMCXLV occurrentiae; per correspondentiam 'classis aurea
 * inter lectiones recuperatas / recuperatae'). RECUSATAE ex censu: t > d
 * V/CXL (petia non pedia), h sublata LXXXVI/CLIX, u > o CXIII/CXXII
 * tectae sed XXXI primae, ci > ti IX/XIV, geminata simplex XXXI/XXXIV,
 * h post c/p/t inserta XI/XVII classis et 0 lemmata (tecum -> thecum).
 * Praecisio lemmatum (recuperatae/classis/lemma): e-ae DLXXXI/DLXIII/DI,
 * e-i CDXXXII/CCCLXXI/CCXCVI, h-praefixa CCXII/CCVIII/CXCVIII, geminatio
 * CLVII/CXXXV/CXXI, o-u CVIII/XCVII/XCIV, inp-imp XCVII/XCI/XCI, p-b
 * XLVIII/XXXIX/XXXIX, i-e LIX/L/XXXV, d-t XXXVIII/XXXVII/V, b-p
 * CIV/CIV/I (lemma 'desub' aliter scriptum). 'activa' = interruptor;
 * mensura: ORATIO_ORTHOGRAPHIA_SOLA=titulus (una activa) aut
 * ORATIO_ORTHOGRAPHIA_SINE=titulus (una inactiva), semel lectae. */
constans OratioOrthographia ORATIO_ORTHOGRAPHIA[] = {
    /* ordo = 'h-prima' censu lemmatum (orthographia2.py): lemmata recta
     * MCCCVII de MDIV recuperatis contra MCCVII ordine praecisionis
     * classium (abere -> abire ante habere). MENSURATUM thesauris V
     * (primarium / coactae permille; basis Seneca 834/764, chartae
     * 840/710 + 833/711, EWT 786/913 + 789/913), quaeque SOLA:
     * h-praefixa 834/765 845/711 837/712; e-ae 834/764 852/714 845/715;
     * inp-imp 834/764 842/711 835/712; b-p 834/764 842/710 835/712;
     * e-i SOLA 834/764 847/703 840/704 (coactae cadunt) sed POST
     * h-praefixa et e-ae (quae verba eius prius capiunt) 863/712
     * 856/713 - servata; RECUSATAE (thesaurum deprimunt): geminatio
     * 844/712 835/712 sed EWT dev coactae 912; d-t EWT 911/912; o-u
     * chartae dev coactae 709; i-e 708; p-b EWT dev 912, chartae 709;
     * inb-imb nihil mutat. Quinque simul (sine capitalibus): Seneca
     * 835/764, chartae 863/712 + 856/713, EWT immotae. */
    { "h-praefixa", ORATIO_ORTHOGRAPHIA_PRAEFIXUM, "", "h", VERUM },
    { "e-ae", ORATIO_ORTHOGRAPHIA_SUBSTITUTIO, "e", "ae", VERUM },
    { "inp-imp", ORATIO_ORTHOGRAPHIA_SUBSTITUTIO, "inp", "imp", VERUM },
    { "b-p", ORATIO_ORTHOGRAPHIA_SUBSTITUTIO, "b", "p", VERUM },
    { "e-i", ORATIO_ORTHOGRAPHIA_SUBSTITUTIO, "e", "i", VERUM },
    { "geminatio", ORATIO_ORTHOGRAPHIA_GEMINATIO, "lmnrstcp", "",
        FALSUM },
    { "d-t", ORATIO_ORTHOGRAPHIA_SUBSTITUTIO, "d", "t", FALSUM },
    { "o-u", ORATIO_ORTHOGRAPHIA_SUBSTITUTIO, "o", "u", FALSUM },
    { "i-e", ORATIO_ORTHOGRAPHIA_SUBSTITUTIO, "i", "e", FALSUM },
    { "p-b", ORATIO_ORTHOGRAPHIA_SUBSTITUTIO, "p", "b", FALSUM }
};
constans i32 ORATIO_ORTHOGRAPHIA_NUMERUS =
    (i32)(magnitudo(ORATIO_ORTHOGRAPHIA)
        / magnitudo(ORATIO_ORTHOGRAPHIA[0]));

#define VARIANS_MAXIMA 160

/* an correspondentia c activa sit: tabula, deinde ambitus mensurae
 * (ORATIO_ORTHOGRAPHIA_SOLA / _SINE), semel lectus */
interior b32
_activa (
    i32 c)
{
                   hic_manens i32  lectum  = ZEPHYRUM;
    hic_manens constans character* sola    = NIHIL;
    hic_manens constans character* sine    = NIHIL;

    si (!lectum)
    {
        sola    = getenv("ORATIO_ORTHOGRAPHIA_SOLA");
        sine    = getenv("ORATIO_ORTHOGRAPHIA_SINE");
        lectum  = I;
    }
    si (!ORATIO_ORTHOGRAPHIA[c].activa)
    {
        redde FALSUM;
    }
    si (sola != NIHIL)
    {
        redde (b32)(strcmp(sola, ORATIO_ORTHOGRAPHIA[c].titulus)
            == ZEPHYRUM);
    }
    si (sine != NIHIL)
    {
        redde (b32)(strcmp(sine, ORATIO_ORTHOGRAPHIA[c].titulus)
            != ZEPHYRUM);
    }
    redde VERUM;
}

/* varians k-ta (sinistra prima) correspondentiae c formae plicatae f in
 * buffer; FALSUM = k-ta nulla */
interior b32
_varians (
    constans OratioOrthographia* c,
                         chorda  f,
                            i32  k,
                             i8* buffer,
                            i32* mensura)
{
    i32 la = (i32)strlen(c->a);
    i32 lb = (i32)strlen(c->b);
    i32 i;
    i32 visae = ZEPHYRUM;

    si (f.mensura + lb + I >= (i32)VARIANS_MAXIMA)
    {
        redde FALSUM;
    }
    si (c->modus == ORATIO_ORTHOGRAPHIA_PRAEFIXUM)
    {
        si (k > ZEPHYRUM)
        {
            redde FALSUM;
        }
        memcpy(buffer, c->b, (size_t)lb);
        memcpy(buffer + lb, f.datum, (size_t)f.mensura);
        *mensura = f.mensura + lb;
        redde VERUM;
    }
    per (i = ZEPHYRUM; i < f.mensura; i++)
    {
        b32 hic = FALSUM;

        si (c->modus == ORATIO_ORTHOGRAPHIA_SUBSTITUTIO)
        {
            hic = (b32)(   i + la <= f.mensura
                        && memcmp(f.datum + i, c->a, (size_t)la)
                            == ZEPHYRUM);
        }
        alioquin
        {
            hic = (b32)(strchr(c->a, (integer)(character)f.datum[i])
                != NIHIL && f.datum[i] != ZEPHYRUM);
        }
        si (!hic)
        {
            perge;
        }
        si (visae < k)
        {
            visae = visae + I;
            perge;
        }
        si (c->modus == ORATIO_ORTHOGRAPHIA_SUBSTITUTIO)
        {
            memcpy(buffer, f.datum, (size_t)i);
            memcpy(buffer + i, c->b, (size_t)lb);
            memcpy(buffer + i + lb, f.datum + i + la,
                (size_t)(f.mensura - i - la));
            *mensura = f.mensura - la + lb;
        }
        alioquin si (c->modus == ORATIO_ORTHOGRAPHIA_INSERTIO)
        {
            memcpy(buffer, f.datum, (size_t)(i + I));
            memcpy(buffer + i + I, c->b, (size_t)lb);
            memcpy(buffer + i + I + lb, f.datum + i + I,
                (size_t)(f.mensura - i - I));
            *mensura = f.mensura + lb;
        }
        alioquin   /* GEMINATIO */
        {
            memcpy(buffer, f.datum, (size_t)(i + I));
            buffer[i + I] = f.datum[i];
            memcpy(buffer + i + (i32)II, f.datum + i + I,
                (size_t)(f.mensura - i - I));
            *mensura = f.mensura + I;
        }
        redde VERUM;
    }
    redde FALSUM;
}

Xar*
oratio_vocabularium_la_quaerere_variantes (
                          Piscina* piscina,
    constans OratioVocabulariumLa* voc,
                           chorda  forma,
                              s32* varians)
{
    Xar* exitus = xar_creare(piscina, (i32)magnitudo(OratioAnalysis));
    chorda f;
    i32 c;

    *varians = (s32)-I;
    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    f = oratio_vocabularium_la_plicare(piscina, forma);
    si (   f.mensura < (i32)II
        || f.mensura + (i32)IV >= (i32)VARIANS_MAXIMA)
    {
        redde exitus;
    }
    per (c = ZEPHYRUM; c < ORATIO_ORTHOGRAPHIA_NUMERUS; c++)
    {
         i8 buffer[VARIANS_MAXIMA];
        i32 mensura;
        i32 k;

                si (!_activa(c))
                {
            perge;
                }
        per (k = ZEPHYRUM; _varians(&ORATIO_ORTHOGRAPHIA[c], f, k,
            buffer,
                &mensura); k++)
        {
            Xar* inventae = oratio_vocabularium_la_quaerere(piscina,
                voc,
                _chorda(buffer, mensura));
            i32 j;

            si (inventae == NIHIL)
            {
                redde NIHIL;
            }
            si (xar_numerus(inventae) == ZEPHYRUM)
            {
                perge;
            }
            per (j = ZEPHYRUM; j < xar_numerus(inventae); j++)
            {
                ((OratioAnalysis*)xar_obtinere(inventae, j))->varians =
                    (s32)c;
            }
            *varians = (s32)c;
            redde inventae;
        }
    }
    redde exitus;
}

Xar*
oratio_vocabularium_la_quaerere (
                          Piscina* piscina,
    constans OratioVocabulariumLa* voc,
                           chorda  forma)
{
    Xar* exitus = xar_creare(piscina, (i32)magnitudo(OratioAnalysis));
    chorda f;
    i32 directae;

    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
        f = oratio_vocabularium_la_plicare(piscina, forma);
    si (f.mensura == ZEPHYRUM)
    {
        redde exitus;
    }
    si (   !_glossarium_quaerere(piscina, voc, f, exitus)
        || !_unica_quaerere(voc, f, exitus, (s32)-I)
        || !_flexa_quaerere(voc, f, exitus, (s32)-I))
    {
        redde NIHIL;
    }
    directae = xar_numerus(exitus);
    _ordinare(voc, exitus, ZEPHYRUM, directae);

    si (directae == ZEPHYRUM)
    {
        i32 t;

        per (t = ZEPHYRUM; t < xar_numerus(voc->tackones); t++)
        {
               s32 ti = *(s32*)xar_obtinere(voc->tackones, t);
            chorda forma_t = oratio_vocabularium_la_plicare(piscina,
                oratio_vocabularium_la_additamentum(voc, ti)->forma);
               i32 ante;
            chorda reliquum;

            si (   forma_t.mensura == ZEPHYRUM
                || f.mensura       <= forma_t.mensura
                || memcmp(f.datum + f.mensura - forma_t.mensura,
                forma_t.datum,
                    (size_t)forma_t.mensura) != ZEPHYRUM)
            {
                perge;
            }
            reliquum  = _chorda(f.datum, f.mensura - forma_t.mensura);
            ante      = xar_numerus(exitus);
            /* locus tackonis ante hospites */
            si (!_addere(exitus, ORATIO_ANALYSIS_TACKON, (s32)-I,
                (s32)-I,
                    (s32)-I, ti, ZEPHYRUM, reliquum.mensura))
            {
                redde NIHIL;
            }
            si (   !_unica_quaerere(voc, reliquum, exitus, ti)
                || !_flexa_quaerere(voc, reliquum, exitus, ti))
            {
                redde NIHIL;
            }
            si (xar_numerus(exitus) == ante + I)
            {
                xar_truncare(exitus, ante);   /* hospes nullus: tackon tollitur */
            }
            alioquin
            {
                _ordinare(voc, exitus, ante + I, xar_numerus(exitus));
            }
        }
    }
    redde exitus;
}


/* ==================================================
 * Accessus
 * ================================================== */

constans OratioStirps*
oratio_vocabularium_la_stirps (
    constans OratioVocabulariumLa* voc,
                              s32  i)
{
    redde (constans OratioStirps*)xar_obtinere(voc->stirpes, (i32)i);
}

constans OratioFlexio*
oratio_vocabularium_la_flexio (
    constans OratioVocabulariumLa* voc,
                              s32  i)
{
    redde (constans OratioFlexio*)xar_obtinere(voc->flexiones, (i32)i);
}

constans OratioAdditamentum*
oratio_vocabularium_la_additamentum (
    constans OratioVocabulariumLa* voc,
                              s32  i)
{
    redde (constans OratioAdditamentum*)xar_obtinere(voc->additamenta,
        (i32)i);
}

constans OratioUnicum*
oratio_vocabularium_la_unicum (
    constans OratioVocabulariumLa* voc,
                              s32  i)
{
    redde (constans OratioUnicum*)xar_obtinere(voc->unica, (i32)i);
}

OratioVocabulariumCensus
oratio_vocabularium_la_census (
    constans OratioVocabulariumLa* voc)
{
    redde voc->census;
}

i32
oratio_vocabularium_la_tackones (
    constans OratioVocabulariumLa* voc)
{
    redde xar_numerus(voc->tackones);
}

vacuum
oratio_vocabularium_la_glossarium_ponere (
         OratioVocabulariumLa* voc,
    constans OratioGlossarium* glossarium)
{
    voc->glossarium = glossarium;
}

constans OratioGlossarium*
oratio_vocabularium_la_glossarium (
    constans OratioVocabulariumLa* voc)
{
    redde voc->glossarium;
}


/* ==================================================
 * Lemma v1 - forma prima
 * ================================================== */

interior b32
_incipit_literis (
                chorda  c,
    constans character* literae)
{
    i32 l = (i32)strlen(literae);

    redde (b32)(c.mensura >= l
        && memcmp(c.datum, literae, (size_t)l) == ZEPHYRUM);
}

interior b32
_flexio_prima_est (
    constans OratioStirps* st,
    constans OratioFlexio* fl)
{
    commutatio (st->pars)
    {
        casus ORATIO_PARS_N:
            redde _incipit_literis(fl->accidentia, "NOM S");
        casus ORATIO_PARS_ADJ:
        casus ORATIO_PARS_PRON:
        casus ORATIO_PARS_PACK:
        casus ORATIO_PARS_NUM:
            redde _incipit_literis(fl->accidentia, "NOM S M");
        casus ORATIO_PARS_V:
        {
            chorda species = _lexema_n(st->species, ZEPHYRUM);

            si (fl->pars != ORATIO_PARS_V)
            {
                redde FALSUM;
            }
            si (_aequalis_literis(species, "IMPERS"))
            {
                redde _incipit_literis(fl->accidentia,
                    "PRES ACTIVE IND 3 S");
            }
            si (   _aequalis_literis(species, "DEP")
                || _aequalis_literis(species, "SEMIDEP"))
            {
                redde _incipit_literis(fl->accidentia,
                    "PRES PASSIVE IND 1 S");
            }
            redde _incipit_literis(fl->accidentia,
                "PRES ACTIVE IND 1 S");
        }
        ordinarius:
            redde (b32)(fl->finis.mensura == ZEPHYRUM);
    }
}

chorda
oratio_vocabularium_la_lemma (
                          Piscina* piscina,
    constans OratioVocabulariumLa* voc,
                              s32  stirps)
{
    constans OratioStirps* st = oratio_vocabularium_la_stirps(voc,
        stirps);
                   chorda  stirps_prima  = st->stirpes[ZEPHYRUM];
                   chorda  finis         = _chorda(NIHIL, ZEPHYRUM);
                      i32  i;
                       i8* datum;

    per (i = ZEPHYRUM; i < xar_numerus(voc->flexiones); i++)
    {
        constans OratioFlexio* fl = oratio_vocabularium_la_flexio(voc,
            (s32)i);

        si (   fl->clavis == I && _congruit(st, I, fl)
            && _flexio_prima_est(st, fl))
        {
            finis = fl->finis;
            frange;
        }
    }
    datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)(stirps_prima.mensura + finis.mensura) + I);
    si (datum == NIHIL)
    {
        redde _chorda(NIHIL, ZEPHYRUM);
    }
    memcpy(datum, stirps_prima.datum, (size_t)stirps_prima.mensura);
    si (finis.mensura > ZEPHYRUM)
    {
        memcpy(datum + stirps_prima.mensura, finis.datum,
            (size_t)finis.mensura);
    }
    redde _chorda(datum, stirps_prima.mensura + finis.mensura);
}
