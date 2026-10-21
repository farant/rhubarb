/* probatio_html_md.c - Consumens consumentis (H10)
 *
 * md (cliens tertius materiae) markdown in HTML reddit per
 * compositionem STML (md_html_reddere); html (cliens quintus) id
 * HTML parsat. Ambo super materia: quod md scribit html legere debet
 * - octetim emittere, proicere, relegere, canone iudicare - et bene
  * formatum invenire: elementum-malum NULLUM (omnis tag clausurae
 * parem habet) - praeter unum PINNATUM (MALA_EXSPECTATA). Initus VII:
 * fixa md IV + prosa domus III (CLAUDE.md cum saeptis codicis, listis,
 * tabulis).
 *
 * TESTIS POSITIVUS: initus manu scriptus '<p>a</b></p>' malum UNUM
 * exspectatum - ne numerator mala non videat.
 *
 * O7a (2026-09-15): md CONTENTUM CORPORIS reddit (nec html nec
 * body), ergo ut FRAGMENTUM contextu 'body' parsatur: involucra non
 * finguntur, spatia inter commentaria ducentia textus manent. Ut
 * documentum parsatum spatia ante html malum darent (spec 'before
 * html' ea neglegit) - IV in sabaw.md, quae nihil de md dicunt.
 */

#include "latina.h"
#include "lectiones.h"   /* lectiones_fopen: canalis lectionum (examen 96) */
#include "credo.h"
#include "canon.h"
#include "chorda.h"
#include "html_arbor.h"
#include "html_registrum.h"
#include "html_lexicon.h"
#include "internamentum.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "md_html.h"
#include "piscina.h"
#include "stml.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


hic_manens constans character* INITUS[] = {
    "md/probationes/fixa/md/episodium.md",
    "md/probationes/fixa/md/piscina.md",
    "md/probationes/fixa/md/sabaw.md",
    "md/probationes/fixa/md/spica.md",
    "md/CLAUDE.md",
    "css/CLAUDE.md",
    "materia/CLAUDE.md"
};

#define NUMERUS_INITUUM ((i32)(magnitudo(INITUS)/magnitudo(INITUS[0])))

/* Mala exspectata per initum. sabaw.md UNUM: notatio sabaw
 * '<if⟨condition⟩>' ... '</if⟨⟩>' in blocco HTML quod md crudum
 * transmittit - tituli tagorum DIFFERUNT (condicio intra titulum),
 * ergo clausura parem apertum non habet. Inventum H10; proprietas
 * notationis, non parsatoris (md: blocci HTML cum titulis non-ASCII
 * - CommonMark ASCII solum - in listam politurae 01M1NBEVM0). */
hic_manens constans s32 MALA_EXSPECTATA[] = {
    ZEPHYRUM, ZEPHYRUM, (s32)I, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM
};

hic_manens character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = lectiones_fopen(via, "rb");
    si (f == NIHIL)
    { redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    { fclose(f); redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    { fclose(f); redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    { redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

/* Nodos generis dati numerare (ambulatio recursiva - arbores md
 * redditae parvae sunt). */
hic_manens i32
_genus_numerare (
    constans MateriaNodus* nodus,
                      s32  genus)
{
    i32 summa = ZEPHYRUM;
    i32 i;

    si (nodus == NIHIL)
    { redde ZEPHYRUM;
    }
    si (nodus->genus == genus)
    { summa = I;
    }
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans MateriaValor* v = &nodus->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            summa = summa + _genus_numerare(v->datum.nodus, genus);
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 n = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < n; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v,
                    j);

                si (e != NIHIL && e->genus == MATERIA_VALOR_NODUS)
                {
                    summa = summa + _genus_numerare(e->datum.nodus,
                        genus);
                }
            }
        }
    }
    redde summa;
}

/* Vitia canonis (s32 -I = fractura ante iudicium) */
hic_manens s32
_canone_iudicare (
                Piscina* piscina,
    InternamentumChorda* intern,
         constans Canon* canon,
                 chorda  textus)
{
    StmlResultus  res;
             Xar* vitia;

    res = stml_legere(textus, piscina, intern);
    si (!res.successus || res.elementum_radix == NIHIL)
    { redde (s32)-I;
    }
    vitia = canon_iudicare(canon, res.elementum_radix, piscina);
    si (vitia == NIHIL)
    { redde (s32)-I;
    }
    redde (s32)xar_numerus(vitia);
}

/* HTML unum per catenam totam: parsare, emittere == html, proicere,
 * relegere, comparare, canone iudicare. Numerum malorum reddit;
 * -I = fractura nominata (impressa). */
hic_manens s32
_html_probare (
                            Piscina* piscina,
                InternamentumChorda* intern,
                     constans Canon* canon,
     constans MateriaArborConsilium* consilium,
                 constans character* titulus,
                             chorda  html)
{
    MateriaNodus* radix;
    MateriaNodus* relecta;
    MateriaScripturaConsilium cs;
    MateriaScriptura emissa;
    MateriaArborScriptura s;
    MateriaArborVitium vitium;
    MateriaArborDifferentia d;
    s32 vitia;

    radix = html_arbor_parsare_fragmentum(piscina,
        (constans character*)html.datum, html.mensura,
        chorda_ex_literis("body", piscina), HTML_ALIENUM_NULLUM);
    si (radix == NIHIL)
    {
        imprimere("  %s: parsator NIHIL\n", titulus);
        redde (s32)-I;
    }
    materia_scriptura_consilium_nudum(&cs, &HTML_REGISTRUM);
    emissa = materia_scribere_nodum(piscina, radix, &cs);
    si (   !emissa.successus || emissa.textus.mensura != html.mensura
        || memcmp(emissa.textus.datum, html.datum,
               (size_t)html.mensura) != ZEPHYRUM)
    {
        imprimere("  %s: emissio a redditione dispar\n", titulus);
        redde (s32)-I;
    }
    s = materia_arbor_scribere_nodum(piscina, radix, consilium);
    si (!s.successus)
    {
        imprimere("  %s: scriptura STML recusata: %s\n", titulus,
            s.causa ? s.causa : "?");
        redde (s32)-I;
    }
    relecta = materia_arbor_legere(piscina, NIHIL, s.textus, consilium,
        &vitium);
    si (relecta == NIHIL)
    {
        imprimere("  %s: lectio recusata: %s\n", titulus,
            vitium.causa ? vitium.causa : "?");
        redde (s32)-I;
    }
    si (!materia_arbor_aequalis(radix, relecta,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d))
    {
        imprimere("  %s: arbor relecta dispar (%s)\n", titulus,
            d.campus ? d.campus : "?");
        redde (s32)-I;
    }
    vitia = _canone_iudicare(piscina, intern, canon, s.textus);
    si (vitia != ZEPHYRUM)
    {
        imprimere("  %s: canon vitia %d\n", titulus, (integer)vitia);
        redde (s32)-I;
    }
    redde (s32)_genus_numerare(radix, (s32)HTML_GENUS_ELEMENTUM_MALUM);
}

/* FIXURA SUITAE: internamentum, lexicon, consilium, canon html et
 * programma md semel lecta; sectiones LEGUNT (internatio in
 * internamentum fixurae sola licet, decretum 2026-10-19). */
nomen structura {
      InternamentumChorda* intern;
                    Canon* canon;
                   chorda  programma;
      MateriaLexiconRatum  ratum;
    MateriaArborConsilium  consilium;
} FixuraCanonis;

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
         FixuraCanonis* f;
    constans character* radix_viae = getenv("RHUBARB_RADIX");
    MateriaLexIudicium  iudicium;
             character  via[DXII];
             character* textus;
                   i32  mensura = ZEPHYRUM;
                chorda  fons;
                chorda  causa;

    f = (FixuraCanonis*)piscina_allocare(c->piscina,
        magnitudo(FixuraCanonis));
    si (f == NIHIL)
    {
        redde;   /* c->suita NIHIL: sectiones VACUAE, suita FRACTA */
    }
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }
    f->intern = internamentum_creare(c->piscina);
    si (!materia_lexicon_ratum_facere(&f->ratum, &HTML_LEXICON,
        &iudicium))
    {
        imprimere("LEXICON HTML RECUSATUM\n");
        redde;
    }
    materia_arbor_consilium_nudum(&f->consilium, &HTML_REGISTRUM,
        &f->ratum, "html");

    /* canon et programma md */
    sprintf(via, "%s/html/grammatica/html.canon", radix_viae);
    textus = _plagulam_legere(c->piscina, via, &mensura);
    CREDO_NON_NIHIL (textus);
    fons.datum     = (i8*)textus;
    fons.mensura   = mensura;
    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    f->canon = textus ? canon_legere(fons, c->piscina, f->intern,
        &causa)
                      : NIHIL;
    CREDO_NON_NIHIL (f->canon);

    sprintf(via, "%s/md/html/md-html.stml", radix_viae);
    textus = _plagulam_legere(c->piscina, via, &mensura);
    CREDO_NON_NIHIL (textus);
    f->programma.datum    = (i8*)textus;
    f->programma.mensura  = mensura;
    si (f->canon == NIHIL || textus == NIHIL)
    {
        redde;
    }
    c->datum = f;
}


/* ==================================================
 * TESTIS POSITIVUS: numerator mala videt
 * ================================================== */

interior vacuum
_testem_probare (
    CredoContextus* c)
{
    constans FixuraCanonis* f =
        (constans FixuraCanonis*)c->suita;
                        chorda html;

    si (f == NIHIL)
    {
        redde;
    }
    html = chorda_ex_literis("<p>a</b></p>", c->piscina);
    CREDO_AEQUALIS_S32 (_html_probare(c->piscina, f->intern, f->canon,
        &f->consilium, "testis", html), (s32)I);
}


/* ==================================================
 * PORTA: html ab md redditum per parsatorem html. Piscina 'opus' per
 * initum VACATUR (olim piscina una totum corpus cumulabat: CCLIX MiB
 * > tectum CCLVI, migratio credo v2 2026-10-21) - nihil initus ultra
 * iterationem suam vivit praeter numeros et internamentum fixurae.
 * ================================================== */

interior vacuum
_portam_probare (
    CredoContextus* c)
{
    constans FixuraCanonis* f =
        (constans FixuraCanonis*)c->suita;
            constans character* radix_viae = getenv("RHUBARB_RADIX");
                       Piscina* opus;
                           i32  i;
                           i32  redditi      = ZEPHYRUM;
                           i32  octeti_html  = ZEPHYRUM;

    si (f == NIHIL)
    {
        redde;
    }
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }
    opus = piscina_generare_dynamicum("probatio_html_md_opus",
        (memoriae_index)IV * M * M);
    si (opus == NIHIL)
    {
        redde;
    }
    imprimere("  initus md %d\n", (integer)NUMERUS_INITUUM);
    per (i = ZEPHYRUM; i < NUMERUS_INITUUM; i++)
    {
             character  via[DXII];
             character* textus;
                   i32  mensura = ZEPHYRUM;
        MdHtmlResultus  r;
                   s32  mala;

        piscina_vacare(opus);
        sprintf(via, "%s/%s", radix_viae, INITUS[i]);
        textus = _plagulam_legere(opus, via, &mensura);
        si (textus == NIHIL)
        {
            imprimere("  ABEST: %s\n", INITUS[i]);
            CREDO_CULPA ("initus abest");
            perge;
        }
        r = md_html_reddere(opus, f->intern, textus, mensura,
            f->programma);
        si (!r.successus)
        {
            imprimere("  %s: md redditio fracta: %.*s\n", INITUS[i],
                (integer)r.causa.mensura,
                (constans character*)r.causa.datum);
        }
        CREDO_VERUM (r.successus);
        si (!r.successus)
        { perge;
        }
        redditi      = redditi + I;
        octeti_html  = octeti_html + r.html.mensura;

        mala = _html_probare(opus, f->intern, f->canon, &f->consilium,
            INITUS[i], r.html);
        imprimere("  %-40s md %6d -> html %6d octeti  mala %d\n",
            INITUS[i], (integer)mensura, (integer)r.html.mensura,
            (integer)mala);
        CREDO_AEQUALIS_S32 (mala, MALA_EXSPECTATA[i]);
    }
    piscina_destruere(opus);

    /* se metiens */
    CREDO_AEQUALIS_I32 (redditi, NUMERUS_INITUUM);
    CREDO_MAIOR_I32 (octeti_html, ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "testis positivus: malum unum", _testem_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "porta: initus md per html", _portam_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("html_md", SECTIONES, _suitam_parare,
        NIHIL);
}
