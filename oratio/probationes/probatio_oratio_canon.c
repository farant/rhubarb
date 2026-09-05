/* probatio_oratio_canon.c - Porta canonis orationis (T4; speculum probatio_md_canon)
 *
 * css/grammatica/css.canon MANU SCRIPTUM est (decretum Frani
 * 2026-09-01: via generata cum registro manu scripto periit - nulla
 * grammatica css.stml exsistit). Canon manu scriptus SILENTER
 * putrescit: regula absens vitium nullum parit, quia canon de
 * elemento quod non describit nihil dicit... immo dicit 'ignotum'
 * - sed regula quae genus renominatum adhuc describit nihil
 * custodit. Ergo DUO custodes hic, praeter iudicium ipsum:
 *
 *  I.  CUSTOS DERIVAE (bidirectionalis): canon ut STML legitur et
 *      regulae eius contra TRES tabulas retiuntur - registrum
 *      generum (genus quodque regulam globalem UNAM), loci (locus
 *      quisque regulam intra= genus suum UNAM), lexicon (genus
 *      lexematis quodque regulam 'lex-' UNAM) - et involucrum
 *      (arbor, ante, post). REVERSUM quoque: regula omnis canonis
 *      uni tabularum congruere debet, ne regula rancida post
 *      renominationem taceat.
 *  II. PINNA SIGILLI: 'registrum-sigillum' in canone ELECTIO unius
 *      optionis est; optio ea contra materia_arbor_sigillum VIVUM
 *      conferitur. Tabula mutata sine canone mutato rubet hic ET
 *      in iudicio (documentum sigillum vivum fert, canon vetus
 *      poscit).
 *
 * IUDICIUM: documentum quodque quod porta B6 parit (corpus VI
 * plagularum + casus inlineares) contra canonem iudicatur - vitia
 * ZEPHYRUM, et numerus documentorum se metitur.
 *
 * TAG LEXEMATIS hic iterum mangulatur ('lex-' + minusculae, '_'
 * -> '-'): regula duplicata consulto - si scriptor aliter
 * mangulet, iudicium corporis rubet (elementum ignotum), si hic
 * aliter, custos derivae rubet (regula absens). Ambo simul
 * mentiri non possunt.
 */

#include "latina.h"
#include "credo.h"
#include "canon.h"
#include "chorda.h"
#include "oratio_arbor.h"
#include "oratio_stml.h"
#include "oratio_lexicon.h"
#include "oratio_lexema.h"
#include "oratio_registrum.h"
#include "materia_token.h"
#include "internamentum.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "piscina.h"
#include "stml.h"
#include "xar.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Casus inlineares: formae quas corpus forte non fert */
hic_manens constans character* INLINEARES[] = {
    "",
    "\n\n",
    "Hello world. This is it.\n\nSecond para.\n",
    "  Indented start.\n\n\n  Next one\n",
    "Mr. Smith went home. He slept.",
    "\"Hello!\" she said. Then left.",
    "It cost $3.50. Cheap. Wait... What?",
    "See e.g. the docs. J. Smith came. (Really.) Yes.",
    "well-known don't e.g B2 1980s 1,024 3.14 x",
    "a.\r\n\r\nb.\r\n",
    "\xC3\x86r\xC3\xB8 \xC2\xA0x \xE2\x80\x9Cq\xE2\x80\x9D \xE2\x80\x94 don\xE2\x80\x99t \x01\x02.",
    "text </or-litterae> in textu\n",
    "\n\n \t\n"
};

#define REGULAE_MAXIMAE 512


/* Chorda contra literas C - sine cast, sine allocatione */
interior b32
_aequalis_literis (
                chorda  c,
    constans character* literae)
{
    i32 mensura = (i32)strlen(literae);

    si (c.mensura != mensura)
    {
        redde FALSUM;
    }
    si (mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (b32)(memcmp(c.datum, literae, (size_t)mensura) == ZEPHYRUM);
}


/* Regula canonis, lecta ex STML canonis ipsius */
nomen structura {
     StmlNodus* nodus;
        chorda  titulus_regulae;
        chorda  intra;         /* mensura ZEPHYRUM = globalis */
           b32  congruens;     /* custos reversus */
} RegulaCanonis;


interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);

    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }

    *mensura = (i32)longitudo;
    redde memoria;
}

/* Liberum elementare titulo dato invenire (primum) */
interior StmlNodus*
_liberum_invenire (
             StmlNodus* parens,
    constans character* titulus)
{
    i32 n;
    i32 i;

    si (parens == NIHIL || parens->liberi == NIHIL)
    {
        redde NIHIL;
    }
    n = xar_numerus(parens->liberi);
    per (i = ZEPHYRUM; i < n; i++)
    {
        StmlNodus* l = *(StmlNodus**)xar_obtinere(parens->liberi, i);

        si (   l->genus == STML_NODUS_ELEMENTUM && l->titulus != NIHIL
            && _aequalis_literis(*l->titulus, titulus))
        {
            redde l;
        }
    }
    redde NIHIL;
}

/* Liberum 'attributum' cuius nomen= datum est */
interior StmlNodus*
_attributum_regulae_invenire (
             StmlNodus* regula,
    constans character* titulus)
{
    i32 n;
    i32 i;

    n = xar_numerus(regula->liberi);
    per (i = ZEPHYRUM; i < n; i++)
    {
        StmlNodus* l = *(StmlNodus**)xar_obtinere(regula->liberi, i);
           chorda* attributi_titulus;

        si (l->genus != STML_NODUS_ELEMENTUM || l->titulus == NIHIL)
        {
            perge;
        }
        si (!_aequalis_literis(*l->titulus, "attributum"))
        {
            perge;
        }
        attributi_titulus = stml_attributum_capere(l, "nomen");
        si (   attributi_titulus != NIHIL
            && _aequalis_literis(*attributi_titulus, titulus))
        {
            redde l;
        }
    }
    redde NIHIL;
}

interior i32
_regulas_colligere (
        StmlNodus* radix,
    RegulaCanonis* regulae)
{
    i32 n;
    i32 i;
    i32 numerus = ZEPHYRUM;

    n = xar_numerus(radix->liberi);
    per (i = ZEPHYRUM; i < n && numerus < REGULAE_MAXIMAE; i++)
    {
        StmlNodus* l = *(StmlNodus**)xar_obtinere(radix->liberi, i);
           chorda* titulus_regulae;
           chorda* intra;

        si (l->genus != STML_NODUS_ELEMENTUM || l->titulus == NIHIL)
        {
            perge;
        }
        si (!_aequalis_literis(*l->titulus, "elementum"))
        {
            perge;
        }
        titulus_regulae = stml_attributum_capere(l, "nomen");
        si (titulus_regulae == NIHIL)
        {
            perge;
        }
        intra = stml_attributum_capere(l, "intra");
        regulae[numerus].nodus = l;
        regulae[numerus].titulus_regulae = *titulus_regulae;
        si (intra != NIHIL)
        {
            regulae[numerus].intra = *intra;
        }
        alioquin
        {
            regulae[numerus].intra.datum    = NIHIL;
            regulae[numerus].intra.mensura  = ZEPHYRUM;
        }
        regulae[numerus].congruens  = FALSUM;
        numerus                     = numerus + I;
    }
    redde numerus;
}

/* Quot regulae (titulus, intra) congruant; congruentes notantur.
 * intra NIHIL = regula globalis postulatur. */
interior i32
_regulam_numerare (
         RegulaCanonis* regulae,
                   i32  numerus,
    constans character* titulus,
    constans character* intra)
{
    i32 summa = ZEPHYRUM;
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        b32 titulus_idem;
        b32 intra_idem;

        titulus_idem = _aequalis_literis(regulae[i].titulus_regulae,
            titulus);
        si (intra == NIHIL)
        {
            intra_idem = (b32)(regulae[i].intra.mensura == ZEPHYRUM);
        }
        alioquin
        {
            intra_idem = _aequalis_literis(regulae[i].intra, intra);
        }
        si (titulus_idem && intra_idem)
        {
            regulae[i].congruens  = VERUM;
            summa                 = summa + I;
        }
    }
    redde summa;
}

/* 'lex-' + minusculae, '_' -> '-' (speculum materia_arbor) */
interior vacuum
_tag_lexematis (
             character* buffer,
    constans character* praefixum,
    constans character* titulus)
{
    i32 i;
    i32 j;

    j = ZEPHYRUM;
    per (i = ZEPHYRUM; praefixum[i] != '\0'; i++)
    {
        buffer[j]  = praefixum[i];
        j          = j + I;
    }
    per (i = ZEPHYRUM; titulus[i] != '\0'; i++)
    {
        character c = titulus[i];

        si (c == '_')
        {
            c = '-';
        }
        alioquin
        {
            c = (character)tolower((insignatus character)c);
        }
        buffer[j]  = c;
        j          = j + I;
    }
    buffer[j] = '\0';
}

/* STML datum legere et iudicare (pars posterior _documentum_iudicare);
 * -I = fractura */
interior s32
_stml_iudicare (
                Piscina* piscina,
    InternamentumChorda* intern,
                  Canon* canon,
                 chorda  textus,
     constans character* titulus)
{
    StmlResultus  res;
             Xar* vitia;
             i32  n;
             i32  i;

    res = stml_legere(textus, piscina, intern);
    si (!res.successus || res.elementum_radix == NIHIL)
    {
        imprimere("  %s: stml_legere fractum\n", titulus);
        redde (s32)-I;
    }
    vitia = canon_iudicare(canon, res.elementum_radix, piscina);
    si (vitia == NIHIL)
    {
        imprimere("  %s: canon_iudicare NIHIL (fractura)\n", titulus);
        redde (s32)-I;
    }
    n = xar_numerus(vitia);
    per (i = ZEPHYRUM; i < n && i < X; i++)
    {
        CanonVitium* v = (CanonVitium*)xar_obtinere(vitia, i);

        imprimere("  %s: VITIUM %s", titulus, canon_nuntius(v->genus));
        si (v->elementum != NIHIL)
        {
            imprimere(" <%.*s>", (integer)v->elementum->mensura,
                (constans character*)v->elementum->datum);
        }
        imprimere("\n");
    }
    redde (s32)n;
}

hic_manens constans MateriaTokenForma FORMA_T11 = { ZEPHYRUM };

/* T11: documentum manu structum cum analysi (analysis-substantivi:
 * lemma derivatum, lingua, casus) in vocabulo 'a' - proiectum et
 * iudicatum; deinde MUTATUM (casus -> gradus, quod substantivo non
 * licet) iudicium vitium ferre debet. Redde textum scriptum. */
interior chorda
_documentum_analysis_struere (
                           Piscina* piscina,
    constans MateriaArborConsilium* consilium)
{
    MateriaNodus* documentum;
    MateriaNodus* paragraphus;
    MateriaNodus* sententia;
    MateriaNodus* vocabulum;
    MateriaNodus* analysis;
    MateriaToken* litterae;
    MateriaArborScriptura s;
    chorda nihil;

    nihil.datum    = NIHIL;
    nihil.mensura  = ZEPHYRUM;
    litterae = materia_token_creare(piscina, &FORMA_T11,
        (s32)ORATIO_LEX_LITTERAE, chorda_ex_literis("a", piscina),
        ZEPHYRUM, (i32)I, (i32)I, ORATIO_FONS_PLAGULAE);
    vocabulum = materia_nodus_creare(piscina,
        (s32)ORATIO_GENUS_VOCABULUM,
        ORATIO_REGISTRUM.genera[ORATIO_GENUS_VOCABULUM].loci_numerus);
    analysis = materia_nodus_creare(piscina,
        (s32)ORATIO_GENUS_ANALYSIS_SUBSTANTIVI,
        ORATIO_REGISTRUM.genera[ORATIO_GENUS_ANALYSIS_SUBSTANTIVI]
            .loci_numerus);
    si (   litterae == NIHIL || vocabulum == NIHIL || analysis == NIHIL
        || !materia_nodus_appendere(piscina, vocabulum,
            (i32)ORATIO_VOCABULUM_PARTES, materia_valor_token(litterae),
            MATERIA_LOCUS_LISTA_TOKEN)
        || !materia_nodus_appendere(piscina, vocabulum,
            (i32)ORATIO_VOCABULUM_CAUDA,
            materia_valor_token(materia_token_creare(piscina,
            &FORMA_T11,
                (s32)ORATIO_LEX_LINEA, chorda_ex_literis("\n", piscina),
                (s32)I, (i32)I, (i32)II, ORATIO_FONS_PLAGULAE)),
            MATERIA_LOCUS_LISTA_TOKEN)
        || !materia_nodus_ponere(analysis, (i32)ORATIO_ANALYSIS_LEMMA,
            materia_valor_token(oratio_lexema_derivatum(piscina,
                (s32)ORATIO_LEX_DERIVATUM, chorda_ex_literis("a",
                piscina),
                litterae)), MATERIA_LOCUS_TOKEN)
        || !materia_nodus_ponere(analysis, (i32)ORATIO_ANALYSIS_LINGUA,
            materia_valor_index((s32)ORATIO_LINGUA_LATINA),
            MATERIA_LOCUS_INDEX)
        || !materia_nodus_ponere(analysis,
            (i32)ORATIO_ANALYSIS_SUBSTANTIVI_CASUS,
            materia_valor_index((s32)ORATIO_CASUS_ABLATIVUS),
            MATERIA_LOCUS_INDEX)
        || !materia_nodus_appendere(piscina, vocabulum,
            (i32)ORATIO_VOCABULUM_ANALYSES,
            materia_valor_nodus(analysis),
            MATERIA_LOCUS_LISTA_NODUS))
    {
        redde nihil;
    }
    sententia = materia_nodus_creare(piscina,
        (s32)ORATIO_GENUS_SENTENTIA,
        (i32)I);
    paragraphus = materia_nodus_creare(piscina,
        (s32)ORATIO_GENUS_PARAGRAPHUS,
        ORATIO_REGISTRUM.genera[ORATIO_GENUS_PARAGRAPHUS].loci_numerus);
    documentum = materia_nodus_creare(piscina,
        (s32)ORATIO_GENUS_DOCUMENTUM,
        (i32)III);
    si (   sententia  == NIHIL || paragraphus == NIHIL
        || documentum == NIHIL
        || !materia_nodus_appendere(piscina, sententia,
            (i32)ORATIO_SENTENTIA_ELEMENTA,
            materia_valor_nodus(vocabulum),
            MATERIA_LOCUS_LISTA_NODUS)
        || !materia_nodus_appendere(piscina, paragraphus,
            (i32)ORATIO_PARAGRAPHUS_SENTENTIAE,
            materia_valor_nodus(sententia), MATERIA_LOCUS_LISTA_NODUS)
        || !materia_nodus_appendere(piscina, documentum,
            (i32)ORATIO_DOCUMENTUM_PARAGRAPHI,
            materia_valor_nodus(paragraphus), MATERIA_LOCUS_LISTA_NODUS)
        || !materia_nodus_ponere(documentum,
        (i32)ORATIO_DOCUMENTUM_FINIS,
            materia_valor_token(materia_token_creare(piscina,
            &FORMA_T11,
                (s32)ORATIO_LEX_FINIS, chorda_ex_literis("", piscina),
                (s32)II, (i32)II, (i32)I, ORATIO_FONS_PLAGULAE)),
            MATERIA_LOCUS_TOKEN))
    {
        redde nihil;
    }
    s = materia_arbor_scribere_nodum(piscina, documentum, consilium);
    si (!s.successus)
    {
        imprimere("  analysis: scriptura STML recusata: %s\n",
            s.causa ? s.causa : "?");
        redde nihil;
    }
    redde s.textus;
}

/* copia textus cum 'vetus' -> 'novus' ubique (mensurae aequales non
 * necessariae) */
interior chorda
_substituere (
               Piscina* piscina,
                chorda  textus,
    constans character* vetus,
    constans character* novus)
{
       i32 lv  = (i32)strlen(vetus);
       i32 ln  = (i32)strlen(novus);
       i32 i   = ZEPHYRUM;
       i32 o   = ZEPHYRUM;
    chorda c;

    c.datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)(textus.mensura * (i32)II) + I);
    dum (i < textus.mensura)
    {
        si (   i + lv <= textus.mensura
            && memcmp(textus.datum + i, vetus, (size_t)lv) == ZEPHYRUM)
        {
            memcpy(c.datum + o, novus, (size_t)ln);
            o = o + ln;
            i = i + lv;
        }
        alioquin
        {
            c.datum[o]  = textus.datum[i];
            o           = o + I;
            i           = i + I;
        }
    }
    c.mensura = o;
    redde c;
}

/* parsare -> STML -> legere -> iudicare. Redde numerum vitiorum;
 * -I = fractura ante iudicium (quae ipsa rubet). */
interior s32
_documentum_iudicare (
                           Piscina* piscina,
               InternamentumChorda* intern,
                             Canon* canon,
    constans MateriaArborConsilium* consilium,
                constans character* fons,
                               i32  mensura,
                constans character* titulus)
{
    MateriaNodus* radix;
    MateriaArborScriptura scriptura;
    StmlResultus res;
    Xar* vitia;
    i32 n;
    i32 i;

    radix = oratio_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    {
        imprimere("  %s: parsator NIHIL\n", titulus);
        redde (s32)-I;
    }
    scriptura = materia_arbor_scribere_nodum(piscina, radix, consilium);
    si (!scriptura.successus)
    {
        imprimere("  %s: scriptura STML recusata: %s\n", titulus,
            scriptura.causa ? scriptura.causa : "?");
        redde (s32)-I;
    }
    res = stml_legere(scriptura.textus, piscina, intern);
    si (!res.successus || res.elementum_radix == NIHIL)
    {
        imprimere("  %s: stml_legere fractum\n", titulus);
        redde (s32)-I;
    }
    vitia = canon_iudicare(canon, res.elementum_radix, piscina);
    si (vitia == NIHIL)
    {
        imprimere("  %s: canon_iudicare NIHIL (fractura)\n", titulus);
        redde (s32)-I;
    }
    n = xar_numerus(vitia);
    per (i = ZEPHYRUM; i < n && i < X; i++)
    {
        CanonVitium* v = (CanonVitium*)xar_obtinere(vitia, i);

        imprimere("  %s: VITIUM %s", titulus, canon_nuntius(v->genus));
        si (v->elementum != NIHIL)
        {
            imprimere(" <%.*s>", (integer)v->elementum->mensura,
                (constans character*)v->elementum->datum);
        }
        si (v->detail != NIHIL)
        {
            imprimere(" '%.*s'", (integer)v->detail->mensura,
                (constans character*)v->detail->datum);
        }
        imprimere("\n");
    }
    redde (s32)n;
}

integer
principale (vacuum)
{
                     Piscina* piscina;
         InternamentumChorda* intern;
          constans character* radix_viae;
                       Canon* canon;
                      chorda  fons_canonis;
                      chorda  causa;
         MateriaLexiconRatum  ratum;
          MateriaLexIudicium  iudicium;
       MateriaArborConsilium  consilium;
               RegulaCanonis  regulae[REGULAE_MAXIMAE];
                         i32  numerus_regularum;
                         i32  documenta_iudicata;
                         b32  praeteritus;
                         i32  i;

    piscina = piscina_generare_dynamicum("probatio_oratio_canon",
        (memoriae_index)64 * M * M);
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }
    si (!materia_lexicon_ratum_facere(&ratum, &ORATIO_LEXICON,
        &iudicium))
    {
        imprimere("LEXICON ORATIONIS RECUSATUM\n");
        redde I;
    }
    oratio_stml_consilium(&consilium, &ratum);


    /* ==================================================
     * CANONEM ONERARE
     * ================================================== */

    imprimere("\n--- Canonem onerans ---\n");
    {
        character  via[DXII];
        character* textus;
              i32  mensura = ZEPHYRUM;

        sprintf(via, "%s/oratio/grammatica/oratio.canon", radix_viae);
        textus = _plagulam_legere(piscina, via, &mensura);
        CREDO_NON_NIHIL (textus);
        si (textus == NIHIL)
        {
            imprimere("  canon ABEST: %s\n", via);
            credo_imprimere_compendium();
            redde I;
        }
        fons_canonis.datum    = (i8*)textus;
        fons_canonis.mensura  = mensura;
    }
    causa.datum = NIHIL;
    causa.mensura = ZEPHYRUM;
    canon = canon_legere(fons_canonis, piscina, intern, &causa);
    CREDO_NON_NIHIL (canon);
    si (canon == NIHIL)
    {
        si (causa.datum != NIHIL)
        {
            imprimere("  CAUSA: %.*s\n", (integer)causa.mensura,
                (constans character*)causa.datum);
        }
        credo_imprimere_compendium();
        redde I;
    }


    /* ==================================================
     * CUSTOS DERIVAE: regulae contra tres tabulas, utrimque
     * ================================================== */

    imprimere("\n--- Custos derivae ---\n");
    {
        StmlResultus res;

        res = stml_legere(fons_canonis, piscina, intern);
        CREDO_VERUM (res.successus);
        numerus_regularum = _regulas_colligere(res.elementum_radix,
            regulae);
        CREDO_MAIOR_I32 (numerus_regularum, ZEPHYRUM);
        CREDO_MINOR_I32 (numerus_regularum, (i32)REGULAE_MAXIMAE);
    }

    /* (a) genera: regula globalis una quaeque */
    per (i = ZEPHYRUM; i < ORATIO_REGISTRUM.numerus_generum; i++)
    {
        constans character* titulus;
                       i32  quot;

        titulus = ORATIO_REGISTRUM.genera[i].titulus;
        quot    = _regulam_numerare(regulae, numerus_regularum, titulus,
            NIHIL);

        si (quot != I)
        {
            imprimere("  genus '%s': regulae %d (I exspectata)\n",
                titulus, (integer)quot);
        }
        CREDO_AEQUALIS_I32 (quot, I);
    }
    CREDO_AEQUALIS_I32 (ORATIO_REGISTRUM.numerus_generum,
        (i32)ORATIO_GENUS_NUMERUS_GENERUM);

    /* (b) loci: regula intra= genus suum una quaeque */
    per (i = ZEPHYRUM; i < ORATIO_REGISTRUM.numerus_generum; i++)
    {
        constans MateriaTabGenus* genus = &ORATIO_REGISTRUM.genera[i];
                             i32  j;

        per (j = ZEPHYRUM; j < genus->loci_numerus; j++)
        {
            constans character* locus =
                ORATIO_REGISTRUM.loci[genus->loci_offset + j].titulus;
            i32 quot = _regulam_numerare(regulae, numerus_regularum,
                locus, genus->titulus);

            si (quot != I)
            {
                imprimere("  locus '%s' intra '%s': regulae %d\n",
                    locus, genus->titulus, (integer)quot);
            }
            CREDO_AEQUALIS_I32 (quot, I);
        }
    }

    /* (c) lexemata: regula 'lex-' una quaeque */
    per (i = ZEPHYRUM; i < ORATIO_LEXICON.numerus_generum; i++)
    {
        character tag[LXIV];
              i32 quot;

        _tag_lexematis(tag, ORATIO_LEXICON.praefixum_tagi,
            ORATIO_LEXICON.genera[i].titulus);
        quot = _regulam_numerare(regulae, numerus_regularum, tag,
            NIHIL);
        si (quot != I)
        {
            imprimere("  lexema '%s': regulae %d\n", tag,
                (integer)quot);
        }
        CREDO_AEQUALIS_I32 (quot, I);
    }
    CREDO_AEQUALIS_I32 (ORATIO_LEXICON.numerus_generum,
        (i32)ORATIO_LEX_NUMERUS_GENERUM);

    /* (d) involucrum */
    CREDO_AEQUALIS_I32 (_regulam_numerare(regulae, numerus_regularum,
        "arbor", NIHIL), I);
    /* trivia NULLA: ante/post consulto NON declarantur */
    CREDO_AEQUALIS_I32 (_regulam_numerare(regulae, numerus_regularum,
        "ante", NIHIL), ZEPHYRUM);

    /* (e) REVERSUM: regula omnis alicui tabulae congruit */
    {
        i32 congruentes = ZEPHYRUM;

        per (i = ZEPHYRUM; i < numerus_regularum; i++)
        {
            si (regulae[i].congruens)
            {
                congruentes = congruentes + I;
            }
            alioquin
            {
                imprimere("  regula RANCIDA: '%.*s'",
                    (integer)regulae[i].titulus_regulae.mensura,
                    (constans character*)
                        regulae[i].titulus_regulae.datum);
                si (regulae[i].intra.mensura > ZEPHYRUM)
                {
                    imprimere(" intra '%.*s'",
                        (integer)regulae[i].intra.mensura,
                        (constans character*)regulae[i].intra.datum);
                }
                imprimere("\n");
            }
        }
        CREDO_AEQUALIS_I32 (congruentes, numerus_regularum);
        imprimere("  regulae %d, omnes congruentes\n",
            (integer)numerus_regularum);
    }

    /* (II) PINNA SIGILLI: optio canonis == sigillum vivum */
    {
           chorda  sigillum_vivum;
        StmlNodus* arbor = NIHIL;
        StmlNodus* attributum;
        StmlNodus* optio;

        sigillum_vivum = materia_arbor_sigillum(piscina,
            &ORATIO_REGISTRUM);
        per (i = ZEPHYRUM; i < numerus_regularum; i++)
        {
            si (   regulae[i].intra.mensura == ZEPHYRUM
                && _aequalis_literis(regulae[i].titulus_regulae,
                                     "arbor"))
            {
                arbor = regulae[i].nodus;
            }
        }
        CREDO_NON_NIHIL (arbor);
        attributum = arbor ? _attributum_regulae_invenire(arbor,
            "registrum-sigillum") : NIHIL;
        CREDO_NON_NIHIL (attributum);
        optio = attributum ? _liberum_invenire(attributum, "optio")
                           : NIHIL;
        CREDO_NON_NIHIL (optio);
        si (optio != NIHIL)
        {
            chorda pinna = stml_textus_normalizatus(optio, piscina);

            imprimere("  sigillum: pinna %.*s, vivum %.*s\n",
                (integer)pinna.mensura,
                (constans character*)pinna.datum,
                (integer)sigillum_vivum.mensura,
                (constans character*)sigillum_vivum.datum);
            CREDO_VERUM (chorda_aequalis(pinna, sigillum_vivum));
        }
    }


    /* ==================================================
     * IUDICIUM: documenta inlinearia + corpus
     * ================================================== */

    documenta_iudicata = ZEPHYRUM;

    imprimere("\n--- Iudicans casus inlineares ---\n");
    {
        i32 numerus = (i32)(magnitudo(INLINEARES)
                          / magnitudo(INLINEARES[0]));

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            character titulus[LXIV];
                  s32 vitia;

            sprintf(titulus, "inlinearis %d", (integer)i);
            vitia = _documentum_iudicare(piscina, intern, canon,
                &consilium, INLINEARES[i], (i32)strlen(INLINEARES[i]),
                titulus);
            CREDO_AEQUALIS_S32 (vitia, (s32)ZEPHYRUM);
            documenta_iudicata = documenta_iudicata + I;
        }
    }

        imprimere("\n--- Iudicans analysin manu structam (T11) ---\n");
    {
        chorda textus = _documentum_analysis_struere(piscina,
            &consilium);

        CREDO_NON_NIHIL (textus.datum);
        si (textus.datum != NIHIL)
        {
            chorda mutatus;

            CREDO_NON_NIHIL (strstr((character*)textus.datum,
                "<analysis-substantivi"));
            CREDO_NON_NIHIL (strstr((character*)textus.datum,
                "<casus"));
            CREDO_NON_NIHIL (strstr((character*)textus.datum,
                "<lemma"));
            CREDO_NON_NIHIL (strstr((character*)textus.datum,
                "<lingua"));
            CREDO_AEQUALIS_S32 (_stml_iudicare(piscina, intern, canon,
                textus, "analysis"), (s32)ZEPHYRUM);
            /* gradus substantivo non licet: casus -> gradus rubet */
            mutatus = _substituere(piscina, textus, "casus", "gradus");
            CREDO_VERUM (_stml_iudicare(piscina, intern, canon, mutatus,
                "analysis mutata") > ZEPHYRUM);
            /* genus analysis alienum in analyses: analysis-adverbii cum
             * casu (adverbium casum non habet) */
            mutatus = _substituere(piscina, textus,
                "analysis-substantivi",
                "analysis-adverbii");
            CREDO_VERUM (_stml_iudicare(piscina, intern, canon, mutatus,
                "analysis aliena") > ZEPHYRUM);
            documenta_iudicata = documenta_iudicata + I;
        }
    }

    imprimere("\n--- Iudicans corpus (plagulae md totae ut oratio) ---\n");
    {
        character via[DXII];
             FILE* lista;
              i32 lectae   = ZEPHYRUM;
             i32 vitiosae = ZEPHYRUM;
             i32 numerus_lineae = ZEPHYRUM;

        sprintf(via, "%s/oratio/build/corpus_md.txt", radix_viae);
        lista = fopen(via, "r");
        si (lista == NIHIL)
        {
            CREDO_CULPA ("oratio/build/corpus_md.txt absens - e radice per cursorem curre");
        }
        alioquin
        {
            character linea[DXII];

            dum (fgets(linea, (integer)magnitudo(linea), lista)
                != NIHIL)
            {
                     Piscina* p;
                   character* textus;
                         i32  mensura  = ZEPHYRUM;
                      size_t  l        = strlen(linea);
                         s32  vitia;

                dum (   l > ZEPHYRUM
                     && (linea[l - I] == '\n' || linea[l - I] == '\r'))
                {
                    linea[l - I]  = '\0';
                    l             = l - I;
                }
                si (l == ZEPHYRUM)
                {
                    perge;
                }
                /* specimen: plagula quinta quaeque (corpus totum:
                 * ORATIO_CORPUS_TOTUS=1) - circuitus STML arboris
                 * verborum XL s super MCXXXIV plagulas est */
                numerus_lineae++;
                si (   getenv("ORATIO_CORPUS_TOTUS") == NIHIL
                    && (numerus_lineae % V)          != ZEPHYRUM)
                {
                    perge;
                }
                sprintf(via, "%s/%s", radix_viae, linea);
                p = piscina_generare_dynamicum("corpus_oratio_canon",
                    4194304);
                textus = _plagulam_legere(p, via, &mensura);
                si (textus == NIHIL)
                {
                    piscina_destruere(p);
                    perge;
                }
                lectae = lectae + I;
                vitia = _documentum_iudicare(p, intern, canon,
                    &consilium,
                    textus, mensura, linea);
                si (vitia != ZEPHYRUM)
                {
                    vitiosae = vitiosae + I;
                }
                documenta_iudicata = documenta_iudicata + I;
                piscina_destruere(p);
            }
            fclose(lista);
            imprimere("  plagulae %d, vitiosae %d\n", (integer)lectae,
                (integer)vitiosae);
            CREDO_AEQUALIS_I32 (vitiosae, ZEPHYRUM);
            CREDO_MAIOR_I32 (lectae, (i32)200);
        }
    }
    /* se metiens */
    CREDO_MAIOR_I32 (documenta_iudicata, (i32)200);

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
