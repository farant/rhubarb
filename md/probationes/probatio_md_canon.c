/* probatio_md_canon.c - Porta canonis markdown (A8b)
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
#include "md_arbor.h"
#include "md_stml.h"
#include "md_lexicon.h"
#include "md_registrum.h"
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
    "# T *e* `c`\n",
    "Setext\n===\n",
    "---\n",
    "```c\nx\n```\n",
    "    code\n\n    more\n",
    "<div>\nx\n</div>\n",
    "---\nname: x\n---\nbody\n",
    "- a\n  - b\n\n- c\n",
    "1) [ ] t\n2) [x] d\n",
    "> q\n> - i\n",
    "| a | b |\n| :-- | --: |\n| 1 |\n",
    "[foo]: /url \"t\"\n\n[foo] [bar][foo] ![i](/p) <http://x.y> www.x.com a@b.co\n",
    "\\*esc\\* &amp; ~~del~~ **strong** _em_ `a\nb` a  \nb\\\nc\n",
    "<b class=\"x\">y</b> <!-- c -->\n",
    "a\r\nb\r\n",
    "- [ ] `</md-textus>` in verbatim\n",
    "text </md-textus> in textu\n"
};

#define REGULAE_MAXIMAE 256


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

/* parsare -> STML -> legere -> iudicare. Redde numerum vitiorum;
 * -I = fractura ante iudicium (quae ipsa rubet). */
interior s32
_documentum_iudicare (
                           Piscina* piscina,
               InternamentumChorda* intern,
                    constans Canon* canon,
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

    radix = md_arbor_parsare(piscina, fons, mensura);
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

/* FIXURA SUITAE: internamentum, lexicon, consilium proiectionis et
 * canon (cum fonte suo) semel facta (parare_suitae; assertiones
 * onerationis ibi); sectiones ea per monstratorem constantem LEGUNT -
 * internatio in internamentum fixurae sola licet (decretum
 * 2026-10-19). */
nomen structura {
      InternamentumChorda* intern;
                    Canon* canon;
                   chorda  fons_canonis;
      MateriaLexiconRatum  ratum;
    MateriaArborConsilium  consilium;
} FixuraCanonis;

interior constans character*
_radix_viae (vacuum)
{
    constans character* radix_viae = getenv("RHUBARB_RADIX");

    redde radix_viae != NIHIL ? radix_viae : ".";
}

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
          FixuraCanonis* fx;
                 chorda  causa;
     MateriaLexIudicium  iudicium;

    fx = (FixuraCanonis*)piscina_allocare(c->piscina,
        magnitudo(FixuraCanonis));
    si (fx == NIHIL)
    {
        redde;   /* c->suita NIHIL: sectiones VACUAE, suita FRACTA */
    }
    fx->intern = internamentum_creare(c->piscina);
    si (!materia_lexicon_ratum_facere(&fx->ratum, &MD_LEXICON,
        &iudicium))
    {
        imprimere("LEXICON MD RECUSATUM\n");
        redde;
    }
    md_stml_consilium(&fx->consilium, &fx->ratum);

    /* canonem onerare */
    {
        character  via[DXII];
        character* textus;
              i32  mensura = ZEPHYRUM;

        sprintf(via, "%s/md/grammatica/md.canon", _radix_viae());
        textus = _plagulam_legere(c->piscina, via, &mensura);
        CREDO_NON_NIHIL (textus);
        si (textus == NIHIL)
        {
            imprimere("  canon ABEST: %s\n", via);
            redde;
        }
        fx->fons_canonis.datum    = (i8*)textus;
        fx->fons_canonis.mensura  = mensura;
    }
    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    fx->canon = canon_legere(fx->fons_canonis, c->piscina, fx->intern,
        &causa);
    CREDO_NON_NIHIL (fx->canon);
    si (fx->canon == NIHIL)
    {
        si (causa.datum != NIHIL)
        {
            imprimere("  CAUSA: %.*s\n", (integer)causa.mensura,
                (constans character*)causa.datum);
        }
        redde;
    }
    c->datum = fx;
}


/* ==================================================
 * CUSTOS DERIVAE: regulae contra tres tabulas, utrimque
 * ================================================== */

interior vacuum
_regulas_custodire (
    CredoContextus* c)
{
    constans FixuraCanonis* fx = (constans FixuraCanonis*)c->suita;
                   Piscina* piscina = c->piscina;
             RegulaCanonis  regulae[REGULAE_MAXIMAE];
                       i32  numerus_regularum;
                       i32  i;

    si (fx == NIHIL)
    {
        redde;
    }
    {
        StmlResultus res;

        res = stml_legere(fx->fons_canonis, piscina, fx->intern);
        CREDO_VERUM (res.successus);
        numerus_regularum = _regulas_colligere(res.elementum_radix,
            regulae);
        CREDO_MAIOR_I32 (numerus_regularum, ZEPHYRUM);
        CREDO_MINOR_I32 (numerus_regularum, (i32)REGULAE_MAXIMAE);
    }

    /* (a) genera: regula globalis una quaeque */
    per (i = ZEPHYRUM; i < MD_REGISTRUM.numerus_generum; i++)
    {
        constans character* titulus;
                       i32  quot;

        titulus = MD_REGISTRUM.genera[i].titulus;
        quot    = _regulam_numerare(regulae, numerus_regularum, titulus,
            NIHIL);

        si (quot != I)
        {
            imprimere("  genus '%s': regulae %d (I exspectata)\n",
                titulus, (integer)quot);
        }
        CREDO_AEQUALIS_I32 (quot, I);
    }
    CREDO_AEQUALIS_I32 (MD_REGISTRUM.numerus_generum,
        (i32)MD_GENUS_NUMERUS_GENERUM);

    /* (b) loci: regula intra= genus suum una quaeque */
    per (i = ZEPHYRUM; i < MD_REGISTRUM.numerus_generum; i++)
    {
        constans MateriaTabGenus* genus = &MD_REGISTRUM.genera[i];
                             i32  j;

        per (j = ZEPHYRUM; j < genus->loci_numerus; j++)
        {
            constans character* locus =
                MD_REGISTRUM.loci[genus->loci_offset + j].titulus;
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
    per (i = ZEPHYRUM; i < MD_LEXICON.numerus_generum; i++)
    {
        character tag[LXIV];
              i32 quot;

        _tag_lexematis(tag, MD_LEXICON.praefixum_tagi,
            MD_LEXICON.genera[i].titulus);
        quot = _regulam_numerare(regulae, numerus_regularum, tag,
            NIHIL);
        si (quot != I)
        {
            imprimere("  lexema '%s': regulae %d\n", tag,
                (integer)quot);
        }
        CREDO_AEQUALIS_I32 (quot, I);
    }
    CREDO_AEQUALIS_I32 (MD_LEXICON.numerus_generum,
        (i32)MD_LEX_NUMERUS_GENERUM);

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
            &MD_REGISTRUM);
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
}


/* ==================================================
 * IUDICIUM: documenta inlinearia + corpus - sectio UNA, quia
 * pavimentum 'documenta > M' utrumque numerat
 * ================================================== */

interior vacuum
_documenta_iudicare (
    CredoContextus* c)
{
    constans FixuraCanonis* fx = (constans FixuraCanonis*)c->suita;
                   Piscina* piscina = c->piscina;
                       i32  documenta_iudicata;
                       i32  i;

    si (fx == NIHIL)
    {
        redde;
    }
    documenta_iudicata = ZEPHYRUM;

    CREDO_GRADUS ("casus inlineares");
    {
        i32 numerus = (i32)(magnitudo(INLINEARES)
                          / magnitudo(INLINEARES[0]));

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            character titulus[LXIV];
                  s32 vitia;

            sprintf(titulus, "inlinearis %d", (integer)i);
            vitia = _documentum_iudicare(piscina, fx->intern, fx->canon,
                &fx->consilium, INLINEARES[i],
                (i32)strlen(INLINEARES[i]), titulus);
            CREDO_AEQUALIS_S32 (vitia, (s32)ZEPHYRUM);
            documenta_iudicata = documenta_iudicata + I;
        }
    }

    CREDO_GRADUS ("corpus");
    {
        character via[DXII];
             FILE* lista;
              i32  lectae   = ZEPHYRUM;
              i32  vitiosae = ZEPHYRUM;

        sprintf(via, "%s/md/build/corpus_md.txt", _radix_viae());
        lista = fopen(via, "r");
        si (lista == NIHIL)
        {
            CREDO_CULPA ("md/build/corpus_md.txt absens - e radice per cursorem curre");
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
                sprintf(via, "%s/%s", _radix_viae(), linea);
                p = piscina_generare_dynamicum("corpus_md_canon",
                    4194304);
                textus = _plagulam_legere(p, via, &mensura);
                si (textus == NIHIL)
                {
                    piscina_destruere(p);
                    perge;
                }
                lectae = lectae + I;
                /* Internamentum PER PLAGULAM, in piscina eius (cum ea
                 * liberatum) - non fixurae: chordae documentorum
                 * omnium MDLXXII in tabula una suitae cumulabantur et
                 * apex cum corpore crescebat (258.4 > CCLVI MiB post
                 * fusionem main, 2026-10-22). canon_iudicare chordas
                 * per contentum comparat, non per monstratorem
                 * internatum (identitas solum in expansione, quae
                 * internamentum ARBORIS ipsius poscit) - ergo tutum. */
                vitia = _documentum_iudicare(p,
                    internamentum_creare(p), fx->canon,
                    &fx->consilium, textus, mensura, linea);
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
            CREDO_MAIOR_I32 (lectae, (i32)1000);
        }
    }
    /* se metiens */
    CREDO_MAIOR_I32 (documenta_iudicata, (i32)1000);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "custos derivae", _regulas_custodire, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "iudicium: casus inlineares et corpus", _documenta_iudicare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("md_canon", SECTIONES,
        _suitam_parare, NIHIL);
}
