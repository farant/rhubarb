/* probatio_navigator_paginae.c - articulus in paginas (navigator N3.1):
 * textus et clausulae ad litteram, notae eadem verba tegunt post
 * translationem, figurae et adiuncta absunt, Rhubarb verus paginatur
 * (lector_paginare) et idem bis. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "codex.h"
#include "articulus.h"
#include "vicipaedia.h"
#include "lector_paginae.h"
#include "navigator_paginae.h"
#include "credo.h"
#include <string.h>

#define PUNCTUM "\xE2\x80\xA2"   /* U+2022 */

interior ArticulusSectio*
sectionem_addere (
             Articulus* a,
    constans character* titulus,
               Piscina* p)
{
    ArticulusSectio* s = (ArticulusSectio*)xar_addere(a->sectiones);

    s->titulus    = chorda_ex_literis(titulus, p);
    s->ancora     = chorda_ex_literis(titulus, p);
    s->clausulae  = xar_creare(p, magnitudo(ArticulusClausula));
    s->adiuncta   = xar_creare(p, magnitudo(ArticulusAdiunctum));
    redde s;
}

interior ArticulusClausula*
clausulam_addere (
                    Xar* clausulae,
         ArticulusGenus  genus,
                    s32  gradus,
     constans character* clavis,
     constans character* textus,
                Piscina* p)
{
    ArticulusClausula* k = (ArticulusClausula*)xar_addere(clausulae);

    k->genus   = genus;
    k->gradus  = gradus;
    k->clavis  = chorda_ex_literis(clavis, p);
    k->textus  = chorda_ex_literis(textus, p);
    k->notae   = xar_creare(p, magnitudo(ArticulusNota));
    redde k;
}

interior vacuum
notam_addere (
     ArticulusClausula* k,
    ArticulusNotaGenus  genus,
                   i32  initium,
                   i32  finis,
    constans character* scopus,
               Piscina* p)
{
    ArticulusNota* n = (ArticulusNota*)xar_addere(k->notae);

    n->genus    = genus;
    n->initium  = initium;
    n->finis    = finis;
    n->scopus   = chorda_ex_literis(scopus, p);
}

/* exemplar: prooemium (paragraphus cum nexu et linea nova, figura,
 * adiunctum), sectio "Usus" (titulus III, index II graduum cum
 * emphasi, citatio, paragraphus vacuus) */
interior Articulus*
exemplar (
    Piscina* p)
{
             Articulus* a;
       ArticulusSectio* s;
     ArticulusClausula* k;
    ArticulusAdiunctum* d;

    a           = (Articulus*)piscina_allocare(p, magnitudo(Articulus));
    a->titulus  = chorda_ex_literis("Rheum", p);
    a->fons      =
        chorda_ex_literis("https://la.wikipedia.org/wiki/Rheum",
        p);
    a->sectiones = xar_creare(p, magnitudo(ArticulusSectio));

    s = sectionem_addere(a, "", p);
    k = clausulam_addere(s->clausulae, ARTICULUS_PARAGRAPHUS, ZEPHYRUM,
        "", "Rheum est planta\nperennis.", p);
    notam_addere(k, ARTICULUS_NEXUS, ZEPHYRUM, V,
        "https://la.wikipedia.org/wiki/Rheum_(planta)", p);
    notam_addere(k, ARTICULUS_FORTIS, X, XVII, "", p);
    clausulam_addere(s->clausulae, ARTICULUS_FIGURA, ZEPHYRUM,
        "https://upload.example/flores.jpg", "Flores rubri", p);
    d             = (ArticulusAdiunctum*)xar_addere(s->adiuncta);
    d->titulus    = chorda_ex_literis("Taxon", p);
    d->clausulae  = xar_creare(p, magnitudo(ArticulusClausula));
    clausulam_addere(d->clausulae, ARTICULUS_PAR, ZEPHYRUM, "Familia",
        "Polygonaceae", p);

    s = sectionem_addere(a, "Usus", p);
    clausulam_addere(s->clausulae, ARTICULUS_TITULUS, III, "",
        "Coquina",
        p);
    k = clausulam_addere(s->clausulae, ARTICULUS_INDEX, I, "",
        "Placenta dulcis", p);
    notam_addere(k, ARTICULUS_EMPHASIS, ZEPHYRUM, VIII, "", p);
    k = clausulam_addere(s->clausulae, ARTICULUS_INDEX, II, "",
        "Cum fragis", p);
    notam_addere(k, ARTICULUS_NEXUS, IV, X,
        "https://la.wikipedia.org/wiki/Fragum", p);
    clausulam_addere(s->clausulae, ARTICULUS_PARAGRAPHUS, ZEPHYRUM, "",
        "", p);
    clausulam_addere(s->clausulae, ARTICULUS_CITATIO, ZEPHYRUM, "",
        "Bonum est.", p);
    redde a;
}

interior constans CodexClausula*
clausula (
    constans NavigatorTextus* t,
                         i32  i)
{
    redde (constans CodexClausula*)xar_obtinere(t->caput.clausulae, i);
}

/* clausula i: genus et textus exspectata */
interior b32
clausula_est (
    constans NavigatorTextus* t,
                         i32  i,
                  CodexGenus  genus,
          constans character* textus)
{
    constans CodexClausula* k = clausula(t, i);

    redde k && k->genus == genus && chorda_aequalis_literis(
        chorda_sectio(t->caput.textus, k->initium, k->finis), textus);
}

interior constans ArticulusNota*
nota (
    constans NavigatorTextus* t,
                         i32  i)
{
    redde (constans ArticulusNota*)xar_obtinere(t->notae, i);
}

/* nota i: genus, verba tecta, scopus */
interior b32
nota_est (
    constans NavigatorTextus* t,
                         i32  i,
          ArticulusNotaGenus  genus,
          constans character* verba,
          constans character* scopus)
{
    constans ArticulusNota* n = nota(t, i);

    redde n && n->genus == genus && chorda_aequalis_literis(
        chorda_sectio(t->caput.textus, n->initium, n->finis), verba)
        && chorda_aequalis_literis(n->scopus, scopus);
}

interior vacuum
_probare_textus (
    CredoContextus* c)
{
    NavigatorTextus* t;

    t = navigator_textum_facere(exemplar(c->piscina), c->piscina);
    CREDO_NECESSE_NON_NIHIL(t);

    CREDO_GRADUS("textus ad litteram (\"\\n\\n\" inter clausulas)");
    CREDO_CHORDA_AEQUALIS_LITERIS(t->caput.textus,
        "Rheum\n\n"
        "Rheum est planta perennis.\n\n"
        "Usus\n\n"
        "Coquina\n\n"
        PUNCTUM " Placenta dulcis\n\n"
        "  " PUNCTUM " Cum fragis\n\n"
        "Bonum est.");

    CREDO_GRADUS("clausulae: genera et fines");
    CREDO_AEQUALIS_I32(xar_numerus(t->caput.clausulae), VII);
    CREDO_VERUM(clausula_est(t, ZEPHYRUM, CODEX_TITULUS, "Rheum"));
    CREDO_VERUM(clausula_est(t, I, CODEX_PARAGRAPHUS,
        "Rheum est planta perennis."));
    CREDO_VERUM(clausula_est(t, II, CODEX_TITULUS, "Usus"));
    CREDO_VERUM(clausula_est(t, III, CODEX_TITULUS, "Coquina"));
    CREDO_VERUM(clausula_est(t, IV, CODEX_PARAGRAPHUS,
        PUNCTUM " Placenta dulcis"));
    CREDO_VERUM(clausula_est(t, V, CODEX_PARAGRAPHUS,
        "  " PUNCTUM " Cum fragis"));
    CREDO_VERUM(clausula_est(t, VI, CODEX_PARAGRAPHUS, "Bonum est."));
}

interior vacuum
_probare_notae (
    CredoContextus* c)
{
    NavigatorTextus* t;

    t = navigator_textum_facere(exemplar(c->piscina), c->piscina);
    CREDO_NECESSE_NON_NIHIL(t);

    CREDO_GRADUS("notae ordine textus, verba eadem tecta");
    CREDO_AEQUALIS_I32(xar_numerus(t->notae), IV);
    CREDO_VERUM(nota_est(t, ZEPHYRUM, ARTICULUS_NEXUS, "Rheum",
        "https://la.wikipedia.org/wiki/Rheum_(planta)"));
    CREDO_VERUM(nota_est(t, I, ARTICULUS_FORTIS, "planta ", ""));
    CREDO_GRADUS("in indice: post praefixum");
    CREDO_VERUM(nota_est(t, II, ARTICULUS_EMPHASIS, "Placenta", ""));
    CREDO_VERUM(nota_est(t, III, ARTICULUS_NEXUS, "fragis",
        "https://la.wikipedia.org/wiki/Fragum"));
}

interior vacuum
_probare_omissa (
    CredoContextus* c)
{
    NavigatorTextus* t;

    t = navigator_textum_facere(exemplar(c->piscina), c->piscina);
    CREDO_NECESSE_NON_NIHIL(t);
    CREDO_GRADUS("figura et adiunctum absunt");
    CREDO_FALSUM(chorda_continet(t->caput.textus, chorda_ex_literis(
        "Flores", c->piscina)));
    CREDO_FALSUM(chorda_continet(t->caput.textus, chorda_ex_literis(
        "Polygonaceae", c->piscina)));
    CREDO_FALSUM(chorda_continet(t->caput.textus, chorda_ex_literis(
        "Taxon", c->piscina)));
}

/* Rhubarb verus: paginatio, initia intra textum et crescentia, notae
 * intra textum, idem bis */
interior vacuum
_probare_rheum (
    CredoContextus* c)
{
             chorda  html;
          Articulus* a;
    NavigatorTextus* t;
                Xar* paginae;
                Xar* iterum;
                i32  i;
                i32  mala = ZEPHYRUM;

    html =
        filum_legere_totum("probationes/fixa/vicipaedia/rhubarb.html",
        c->piscina);
    a = vicipaedia_articulus(html,
        "https://en.wikipedia.org/wiki/Rhubarb",
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(a);
    t = navigator_textum_facere(a, c->piscina);
    CREDO_NECESSE_NON_NIHIL(t);

    CREDO_GRADUS("textus incipit a titulo, prooemio sequente");
    CREDO_VERUM(clausula_est(t, ZEPHYRUM, CODEX_TITULUS, "Rhubarb"));
    CREDO_VERUM(xar_numerus(t->notae) > XX);

    CREDO_GRADUS("notae intra textum, non vacuae");
    per (i = ZEPHYRUM; i < xar_numerus(t->notae); i++)
    {
        si (   nota(t, i)->initium >= nota(t, i)->finis
            || nota(t, i)->finis > (i32)t->caput.textus.mensura)
        {
            mala++;
        }
    }
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);

    CREDO_GRADUS("paginae plures; initia crescentia intra textum");
    paginae = lector_paginare(&t->caput, D, DC, c->piscina);
    CREDO_NECESSE_NON_NIHIL(paginae);
    CREDO_VERUM(xar_numerus(paginae) > III);
    per (i = ZEPHYRUM; i < xar_numerus(paginae); i++)
    {
        constans LectorPagina* pg =
            (constans LectorPagina*)xar_obtinere(
            paginae, i);

        si (   pg->initium >= (i32)t->caput.textus.mensura
            || (i > ZEPHYRUM && pg->initium <= ((constans LectorPagina*)
                xar_obtinere(paginae, i - I))->initium))
        {
            mala++;
        }
    }
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);

    CREDO_GRADUS("eadem area: eaedem paginae");
    iterum = lector_paginare(&navigator_textum_facere(a,
        c->piscina)->caput,
        D, DC, c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(iterum), xar_numerus(paginae));
    per (i = ZEPHYRUM; i < xar_numerus(paginae); i++)
    {
        si (((constans LectorPagina*)xar_obtinere(paginae, i))->initium
            != ((constans LectorPagina*)xar_obtinere(iterum,
            i))->initium)
        {
            mala++;
        }
    }
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "textus", _probare_textus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "notae", _probare_notae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "omissa", _probare_omissa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rheum", _probare_rheum, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("navigator_paginae", SECTIONES);
}
