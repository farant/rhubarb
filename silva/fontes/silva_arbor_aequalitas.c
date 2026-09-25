/* silva_arbor_aequalitas.c - comparator arborum silvae
 *
 * Plagula PROPRIA, non pars silva_arbor.c: consumptores ultra
 * arborem iam visibiles sunt (portae mutationis rogant 'an haec
 * transformatio arborem servaverit'), et comparator a scriptore
 * et lectore prorsus independens est - solas arbores tangit.
 *
 * SUPER MATERIAM (silva-migratio T10d, 2026-09-25): ambulatio
 * arboris et comparatio lexematis materiae sunt
 * (materia_arbor_aequalis_fronte, materia_arbor_lexemata_aequalia_
 * fronte); hic manent uncus C89 super caudam lexematis (standard,
 * longitudo, scissurae) et comparator PARSURARUM (regiones, rami,
 * directivae - formae C89).
 *
 * Contractus plenus (modi, quid conferatur, quid CONSULTO non
 * videatur) in silva_arbor.h et materia_arbor.h. Lege eos PRIUS
 * quam quicquam hic mutes - praesertim notam de dominio gemino
 * triviorum, quae explicat cur comparatorem 'emendare' ut plus
 * capiat oraculum alterum T6 destrueret.
 */

#include "silva_arbor.h"
#include "xar.h"
#include <string.h>


/* ==================================================
 * Uncus C89 - cauda lexematis
 *
 * Vocatur a materia pro OMNI lexemate collato, trivia inclusa
 * (silva vetus scissuras et standard triviorum quoque conferebat).
 * ================================================== */

interior b32
_silvae_scissurae_aequales (
      constans SilvaToken*  a,
      constans SilvaToken*  b,
       constans character** campus)
{
    Xar* xa         = silva_token_scissurae(a);
    Xar* xb         = silva_token_scissurae(b);
    i32  numerus_a  = xa != NIHIL ? xar_numerus(xa) : (i32)ZEPHYRUM;
    i32  numerus_b  = xb != NIHIL ? xar_numerus(xb) : (i32)ZEPHYRUM;
    i32  i;

    si (numerus_a != numerus_b)
    {
        *campus = "scissurae/numerus";
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus_a; i++)
    {
        SilvaScissura* sa = (SilvaScissura*)xar_obtinere(xa, i);
        SilvaScissura* sb = (SilvaScissura*)xar_obtinere(xb, i);

        si (sa == NIHIL || sb == NIHIL)
        {
            *campus = "scissura/nihil";
            redde FALSUM;
        }
        si (sa->offset != sb->offset)
        {
            *campus = "scissura/offset";
            redde FALSUM;
        }
        si (sa->crlf != sb->crlf)
        {
            *campus = "scissura/crlf";
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_silvae_lexemata_conferre (
                         vacuum*  datum,
          constans MateriaToken*  a,
          constans MateriaToken*  b,
    MateriaArborComparatioModus   modus,
             constans character** campus)
{
    b32 cauda_a = materia_token_cauda(a) != NIHIL;
    b32 cauda_b = materia_token_cauda(b) != NIHIL;

    (vacuum)datum;

    /* Lexema sine cauda C89 (forma alia creatum) accessores silvae
     * frangeret - nominatur, non legitur. */
    si (cauda_a != cauda_b)
    {
        *campus = "lexema/cauda";
        redde FALSUM;
    }
    si (!cauda_a)
    {
        redde VERUM;
    }
    si (silva_token_standard(a) != silva_token_standard(b))
    {
        *campus = "lexema/standard";
        redde FALSUM;
    }
    si (   modus == MATERIA_ARBOR_COMPARATIO_FIDELITAS
        && silva_token_longitudo(a) != silva_token_longitudo(b))
    {
        *campus = "lexema/longitudo";
        redde FALSUM;
    }
    redde _silvae_scissurae_aequales(a, b, campus);
}

hic_manens constans MateriaArborComparatioFrons FRONS_C89 = {
    NIHIL,
    _silvae_lexemata_conferre
};

b32
silva_arbor_aequalis (
          constans SilvaNodus* a,
          constans SilvaNodus* b,
    SilvaArborComparatioModus  modus,
        SilvaArborDifferentia* differentia)
{
    redde materia_arbor_aequalis_fronte(a, b, modus, &FRONS_C89,
        differentia);
}


/* ==================================================
 * Status comparatoris parsurarum
 * ================================================== */

nomen structura {
     SilvaArborComparatioModus  modus;
         SilvaArborDifferentia* differentia;
} ArborComparator;

/* Divergentiam nominare. Semper FALSUM reddit, ut vocantes
 * 'redde _arbor_divergere(...)' scribere possint. Nodi et lexemata
 * hic semper NIHIL: divergentiae formae parsurae sunt; quae intra
 * arborem aut lexema cadunt a materia nominantur. */
interior b32
_arbor_divergere (
        ArborComparator* comparator,
     constans character* campus,
                    s32  locus,
                    s32  index)
{
    SilvaArborDifferentia* differentia = comparator->differentia;

    si (differentia != NIHIL && differentia->campus == NIHIL)
    {
        differentia->campus    = campus;
        differentia->nodus_a   = NIHIL;
        differentia->nodus_b   = NIHIL;
        differentia->lexema_a  = NIHIL;
        differentia->lexema_b  = NIHIL;
        differentia->locus     = locus;
        differentia->index     = index;
        differentia->via[0]    = '\0';
    }
    redde FALSUM;
}

/* Lexemata extra arborem per materiam; sedes vocantis (locus,
 * index) superponitur ut silva vetus eam nominabat - index trivii
 * servatur ubi trivium divergit. */
interior b32
_arbor_lexemata_aequalia (
        ArborComparator* comparator,
    constans SilvaToken* a,
    constans SilvaToken* b,
                    s32  locus,
                    s32  index)
{
    SilvaArborDifferentia* differentia = comparator->differentia;

    si (materia_arbor_lexemata_aequalia_fronte(a, b, comparator->modus,
            &FRONS_C89, differentia))
    {
        redde VERUM;
    }
    si (differentia != NIHIL)
    {
        differentia->locus = locus;
        si (differentia->index < ZEPHYRUM)
        {
            differentia->index = index;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Comparator parsurarum (M2 §2 T4)
 *
 * DIAGNOSIS, non verdictum - vide silva_arbor.h.
 * ================================================== */

/* Laminam lexematum conferre (linea directivae, lamina cruda).
 * Nodi NIHIL sunt: haec lexemata ARBORI non pertinent. */
interior b32
_arbor_lamina_aequalis (
         ArborComparator* comparator,
                     Xar* a,
                     Xar* b,
      constans character* campus,
                     s32  index)
{
    i32 numerus_a;
    i32 numerus_b;
    i32 i;

    numerus_a = a != NIHIL ? xar_numerus(a) : (i32)ZEPHYRUM;
    numerus_b = b != NIHIL ? xar_numerus(b) : (i32)ZEPHYRUM;
    si (numerus_a != numerus_b)
    {
        redde _arbor_divergere(comparator, campus, -I, index);
    }
    per (i = ZEPHYRUM; i < numerus_a; i++)
    {
        si (!_arbor_lexemata_aequalia(comparator,
                 *(SilvaToken**)xar_obtinere(a, i),
                 *(SilvaToken**)xar_obtinere(b, i),
                 index, (s32)i))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_arbor_regiones_aequales (
    ArborComparator*,
                Xar*,
                Xar*);

interior b32
_arbor_regio_aequalis (
          ArborComparator* comparator,
      constans SilvaRegio* a,
      constans SilvaRegio* b,
                      s32  index)
{
    i32 numerus_a;
    i32 numerus_b;
    i32 i;

    si ((a == NIHIL) != (b == NIHIL))
    {
        redde _arbor_divergere(comparator, "regio/nullitas", -I, index);
    }
    si (a == NIHIL)
    {
        redde VERUM;
    }
    si (a->est_texta != b->est_texta)
    {
        redde _arbor_divergere(comparator, "regio/texta", -I, index);
    }
    numerus_a = a->rami != NIHIL ? xar_numerus(a->rami) : (i32)ZEPHYRUM;
    numerus_b = b->rami != NIHIL ? xar_numerus(b->rami) : (i32)ZEPHYRUM;
    si (numerus_a != numerus_b)
    {
        redde _arbor_divergere(comparator, "regio/numerus-ramorum",
            -I, index);
    }
    per (i = ZEPHYRUM; i < numerus_a; i++)
    {
        constans SilvaRamus* ra;
        constans SilvaRamus* rb;

        ra = *(SilvaRamus**)xar_obtinere(a->rami, i);
        rb = *(SilvaRamus**)xar_obtinere(b->rami, i);
        si ((ra == NIHIL) != (rb == NIHIL))
        {
            redde _arbor_divergere(comparator, "ramus/nullitas",
                -I, (s32)i);
        }
        si (ra == NIHIL)
        {
            perge;
        }
        si (ra->genus != rb->genus)
        {
            redde _arbor_divergere(comparator, "ramus/genus", -I,
                (s32)i);
        }
        si (ra->conditio_id != rb->conditio_id)
        {
            redde _arbor_divergere(comparator, "ramus/conditio",
                -I, (s32)i);
        }
        /* est_sumptum NON confertur: structurale est (vide caput) */
        si (!_arbor_lamina_aequalis(comparator, ra->directiva,
                 rb->directiva, "ramus/directiva", (s32)i))
        {
            redde FALSUM;
        }
        si (!_arbor_lamina_aequalis(comparator, ra->lexemata_cruda,
                 rb->lexemata_cruda, "ramus/cruda", (s32)i))
        {
            redde FALSUM;
        }
    }
    si (!_arbor_lamina_aequalis(comparator, a->directiva_finis,
             b->directiva_finis, "regio/finis", index))
    {
        redde FALSUM;
    }
    /* Filiae per PLANATIONEM iam tectae - recursio hic duplicaret */
    redde VERUM;
}

/* Regiones DEGRADATAS solas planare, ordine praefixo.
 *
 * LACUNA REPRAESENTATIONALIS NOMINATA: regiones TEXTAE in
 * expansione ONERATA non renascuntur, quia lineae earum ex ARBORE
 * emittuntur - laminae earum lexemata EADEM monstrant quae nodus
 * conditionalis fert, et ea seorsum reficere IDENTITATEM frangeret
 * (duplicatio mentiretur - lex duplex, spec v1 §6). Ut serventur
 * referentiae '#id' TRANS SECTIONES opus essent - eadem machina
 * quam T6 pro origine introducit.
 *
 * Ergo comparator id confert QUOD FORMA REPRAESENTAT: seriem
 * regionum degradatarum. Recensio T5 lacunam NUMERABIT. */
interior b32
_arbor_regiones_planare (
    Xar* regiones,
    Xar* acervus)
{
    i32 i;

    si (regiones == NIHIL)
    {
        redde VERUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(regiones); i++)
    {
        SilvaRegio*  regio;
        SilvaRegio** sedes;

        regio = *(SilvaRegio**)xar_obtinere(regiones, i);
        si (regio == NIHIL)
        {
            perge;
        }
        si (!regio->est_texta)
        {
            sedes = (SilvaRegio**)xar_addere(acervus);
            si (sedes == NIHIL)
            {
                redde FALSUM;
            }
            *sedes = regio;
        }
        si (!_arbor_regiones_planare(regio->filiae, acervus))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_arbor_regiones_aequales (
    ArborComparator* comparator,
                Xar* a,
                Xar* b)
{
     Piscina* piscina;
         Xar* plana_a;
         Xar* plana_b;
         i32  numerus_a;
         i32  numerus_b;
         i32  i;

    piscina = piscina_generare_dynamicum("aequalitas_regionum",
        1048576);
    si (piscina == NIHIL)
    {
        redde VERUM;
    }
    plana_a = xar_creare(piscina, magnitudo(SilvaRegio*));
    plana_b = xar_creare(piscina, magnitudo(SilvaRegio*));
    si (   plana_a == NIHIL || plana_b == NIHIL
        || !_arbor_regiones_planare(a, plana_a)
        || !_arbor_regiones_planare(b, plana_b))
    {
        piscina_destruere(piscina);
        redde VERUM;
    }

    numerus_a = xar_numerus(plana_a);
    numerus_b = xar_numerus(plana_b);
    si (numerus_a != numerus_b)
    {
        piscina_destruere(piscina);
        redde _arbor_divergere(comparator, "regiones/numerus",
            -I, (s32)numerus_a);
    }
    per (i = ZEPHYRUM; i < numerus_a; i++)
    {
        si (!_arbor_regio_aequalis(comparator,
                 *(SilvaRegio**)xar_obtinere(plana_a, i),
                 *(SilvaRegio**)xar_obtinere(plana_b, i), (s32)i))
        {
            piscina_destruere(piscina);
            redde FALSUM;
        }
    }
    piscina_destruere(piscina);
    redde VERUM;
}

/* Fons laminae directivae = fons lexematis primi eius. */
interior s32
_laminae_fons_index (
    constans Xar* lamina)
{
    constans SilvaToken** sedes;

    si (lamina == NIHIL || xar_numerus(lamina) == ZEPHYRUM)
    {
        redde -I;
    }
    sedes = (constans SilvaToken**)xar_obtinere(lamina, ZEPHYRUM);
    si (sedes == NIHIL || *sedes == NIHIL)
    {
        redde -I;
    }
    redde (*sedes)->fons_index;
}

/* Numerus directivarum FONTIS DATI (vide notam ad DIRECTIVAE). */
interior i32
_directivae_fontis (
    constans SilvaParsura* p,
                      s32  fons)
{
    i32 numerus;
    i32 k;

    si (p == NIHIL || p->directivae == NIHIL)
    {
        redde ZEPHYRUM;
    }
    numerus = ZEPHYRUM;
    per (k = ZEPHYRUM; k < xar_numerus(p->directivae); k++)
    {
        si (_laminae_fons_index(*(Xar**)xar_obtinere(p->directivae, k))
                == fons)
        {
            numerus++;
        }
    }
    redde numerus;
}

/* Directiva n-esima FONTIS DATI; NIHIL si abest. */
interior Xar*
_directivam_fontis (
    constans SilvaParsura* p,
                      s32  fons,
                      i32  n)
{
    i32 visae;
    i32 k;

    si (p == NIHIL || p->directivae == NIHIL)
    {
        redde NIHIL;
    }
    visae = ZEPHYRUM;
    per (k = ZEPHYRUM; k < xar_numerus(p->directivae); k++)
    {
        Xar* lamina = *(Xar**)xar_obtinere(p->directivae, k);

        si (_laminae_fons_index(lamina) != fons)
        {
            perge;
        }
        si (visae == n)
        {
            redde lamina;
        }
        visae++;
    }
    redde NIHIL;
}

b32
silva_arbor_parsurae_aequales (
        constans SilvaParsura* a,
        constans SilvaParsura* b,
    SilvaArborComparatioModus  modus,
        SilvaArborDifferentia* differentia)
{
    ArborComparator comparator;
                i32 numerus_a;
                i32 numerus_b;
                i32 i;

    si (differentia != NIHIL)
    {
        differentia->campus    = NIHIL;
        differentia->nodus_a   = NIHIL;
        differentia->nodus_b   = NIHIL;
        differentia->lexema_a  = NIHIL;
        differentia->lexema_b  = NIHIL;
        differentia->locus     = -I;
        differentia->index     = -I;
        differentia->via[0]    = '\0';
    }

    comparator.modus        = modus;
    comparator.differentia  = differentia;

    si ((a == NIHIL) != (b == NIHIL))
    {
        redde _arbor_divergere(&comparator, "parsura/nullitas", -I, -I);
    }
    si (a == NIHIL)
    {
        redde VERUM;
    }
    si ((a->commissio == NIHIL) != (b->commissio == NIHIL))
    {
        redde _arbor_divergere(&comparator, "commissio/nullitas", -I,
            -I);
    }

    /* ARBOR: per nodum supremum. Radix LISTA est, ergo profunditas
     * a I incipit - aliter comparator patrem ad RADICEM conferret,
     * ubi responsum EXTRA comparationem iacet (vitium M1 T6, CIX
     * divergentiae falsae). */
    si (a->commissio != NIHIL)
    {
        numerus_a = silva_valor_lista_numerus(a->commissio->radix);
        numerus_b = silva_valor_lista_numerus(b->commissio->radix);
        si (numerus_a != numerus_b)
        {
            redde _arbor_divergere(&comparator, "radix/numerus",
                -I, (s32)numerus_a);
        }
        per (i = ZEPHYRUM; i < numerus_a; i++)
        {
            SilvaValor* ea;
            SilvaValor* eb;

            ea = silva_valor_lista_obtinere(a->commissio->radix, i);
            eb = silva_valor_lista_obtinere(b->commissio->radix, i);
            si (ea == NIHIL || eb == NIHIL)
            {
                perge;
            }
            si (ea->genus != eb->genus)
            {
                redde _arbor_divergere(&comparator, "radix/genus",
                    -I, (s32)i);
            }
            si (ea->genus != SILVA_VALOR_NODUS)
            {
                perge;
            }
            si (!silva_arbor_aequalis(ea->datum.nodus,
                     eb->datum.nodus, modus, differentia))
            {
                si (   differentia != NIHIL
                    && differentia->index < ZEPHYRUM)
                {
                    differentia->index = (s32)i;
                }
                redde FALSUM;
            }
        }
    }

    /* DIRECTIVAE - FONTIS PRINCIPIS SOLIUS.
     *
     * Documentum PLAGULAE proiectio est, non PARSURAE (spec 1), et
     * scriptor directivas per fontem filtrat
     * (_parsura_reinserendum_addere ... fons_index). Ergo parsura
     * originalis directivas OMNIUM plagularum fert (capita quoque)
     * dum lecta principis solius: comparare summas est poma piris
     * conferre.
     *
     * MENSURATUM (T7): A=DCXCVII, B=VII. Discrimen tantum non
     * subtile erat, et tamen CXLIX plagulae octetim exactae -
     * quod ipsum probat directivas deesse EMISSIONI non obstare.
     * Comparator ergo causam VERAM harum IV plagularum CELABAT:
     * primam divergentiam nuntiat, et haec falsa prima erat.
     *
     * LEX: instrumentum diagnosticum quod rem aliam comparat quam
     * documentum repraesentat ductum falsum dat, non nullum -
     * quod peius est. */
    numerus_a = _directivae_fontis(a, a->fons_princeps);
    numerus_b = _directivae_fontis(b, b->fons_princeps);
    si (numerus_a != numerus_b)
    {
        redde _arbor_divergere(&comparator, "directivae/numerus",
            -I, (s32)numerus_a);
    }
    per (i = ZEPHYRUM; i < numerus_a; i++)
    {
        si (!_arbor_lamina_aequalis(&comparator,
                 _directivam_fontis(a, a->fons_princeps, i),
                 _directivam_fontis(b, b->fons_princeps, i),
                 "directiva", (s32)i))
        {
            redde FALSUM;
        }
    }

    /* REGIONES */
    si (a->expansio != NIHIL && b->expansio != NIHIL)
    {
        si (!_arbor_regiones_aequales(&comparator,
                 a->expansio->regiones, b->expansio->regiones))
        {
            redde FALSUM;
        }
    }

    /* CAUDA: trivia caudae campus est qui tacite cadere solet */
    si (!_arbor_lexemata_aequalia(&comparator, a->lexema_finis,
             b->lexema_finis, -I, -I))
    {
        redde FALSUM;
    }
    redde VERUM;
}
