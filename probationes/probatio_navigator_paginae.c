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

/* clausula i indicis nexuum: genus et textus */
interior b32
clausula_nexus_est (
    constans NavigatorTextus* t,
                         i32  i,
                  CodexGenus  genus,
          constans character* textus)
{
    constans CodexClausula* k = (constans CodexClausula*)xar_obtinere(
        t->nexus.clausulae, i);

    redde k && k->genus == genus && chorda_aequalis_literis(
        chorda_sectio(t->nexus.textus, k->initium, k->finis), textus);
}

/* nota i indicis: NEXUS, linea tecta, scopus */
interior b32
nota_nexus_est (
    constans NavigatorTextus* t,
                         i32  i,
          constans character* linea,
          constans character* scopus)
{
    constans ArticulusNota* n = (constans ArticulusNota*)xar_obtinere(
        t->notae_nexuum, i);

    redde n && n->genus == ARTICULUS_NEXUS && chorda_aequalis_literis(
        chorda_sectio(t->nexus.textus, n->initium, n->finis), linea)
        && chorda_aequalis_literis(n->scopus, scopus);
}

interior vacuum
_probare_nexus (
    CredoContextus* c)
{
    NavigatorTextus* t;

    t = navigator_textum_facere(exemplar(c->piscina), c->piscina);
    CREDO_NECESSE_NON_NIHIL(t);

    CREDO_GRADUS("index nexuum: titulus, linea per scopum");
    CREDO_CHORDA_AEQUALIS_LITERIS(t->nexus.textus,
        "Links\n\nRheum (planta)\n\nFragum");
    CREDO_AEQUALIS_I32(xar_numerus(t->nexus.clausulae), III);
    CREDO_VERUM(clausula_nexus_est(t, ZEPHYRUM, CODEX_TITULUS,
        "Links"));
    CREDO_VERUM(clausula_nexus_est(t, I, CODEX_PARAGRAPHUS,
        "Rheum (planta)"));
    CREDO_VERUM(clausula_nexus_est(t, II, CODEX_PARAGRAPHUS, "Fragum"));

    CREDO_GRADUS("linea tota nexus est");
    CREDO_AEQUALIS_I32(xar_numerus(t->notae_nexuum), II);
    CREDO_VERUM(nota_nexus_est(t, ZEPHYRUM, "Rheum (planta)",
        "https://la.wikipedia.org/wiki/Rheum_(planta)"));
    CREDO_VERUM(nota_nexus_est(t, I, "Fragum",
        "https://la.wikipedia.org/wiki/Fragum"));

    CREDO_GRADUS("articulus ipse non mutatus");
    CREDO_FALSUM(chorda_continet(t->caput.textus, chorda_ex_literis(
        "Links", c->piscina)));
}

/* scopi distincti sine #ancora, ordo primae apparitionis, hospites
 * alieni cum praefixo, decodicatio */
interior vacuum
_probare_nexus_distincti (
    CredoContextus* c)
{
            Articulus* a;
      ArticulusSectio* s;
    ArticulusClausula* k;
      NavigatorTextus* t;

    a = (Articulus*)piscina_allocare(c->piscina, magnitudo(Articulus));
    a->titulus = chorda_ex_literis("Alpha", c->piscina);
    a->fons      =
        chorda_ex_literis("https://la.wikipedia.org/wiki/Alpha",
        c->piscina);
    a->sectiones  = xar_creare(c->piscina, magnitudo(ArticulusSectio));
    s             = sectionem_addere(a, "", c->piscina);
    k = clausulam_addere(s->clausulae, ARTICULUS_PARAGRAPHUS, ZEPHYRUM,
        "", "a b c d e f g", c->piscina);
    notam_addere(k, ARTICULUS_NEXUS, ZEPHYRUM, I,
        "https://la.wikipedia.org/wiki/Alpha#x", c->piscina);
    notam_addere(k, ARTICULUS_NEXUS, II, III,
        "https://la.wikipedia.org/wiki/Caf%C3%A9", c->piscina);
    notam_addere(k, ARTICULUS_EMPHASIS, II, III, "", c->piscina);
    notam_addere(k, ARTICULUS_NEXUS, IV, V,
        "https://la.wikipedia.org/wiki/Alpha#y", c->piscina);
    notam_addere(k, ARTICULUS_NEXUS, VI, VII,
        "https://en.wikipedia.org/wiki/Rhubarb", c->piscina);
    notam_addere(k, ARTICULUS_NEXUS, VIII, IX,
        "https://www.example.com/a_b%20c", c->piscina);
    notam_addere(k, ARTICULUS_NEXUS, XII, XIII,
        "https://la.wikipedia.org/wiki/Alpha", c->piscina);
    t = navigator_textum_facere(a, c->piscina);
    CREDO_NECESSE_NON_NIHIL(t);

    CREDO_GRADUS("Alpha#x, Alpha#y, Alpha: linea una; sine emphasi");
    CREDO_CHORDA_AEQUALIS_LITERIS(t->nexus.textus,
        "Links\n\n"
        "Alpha\n\n"
        "Caf\xC3\xA9\n\n"
        "en.wikipedia.org: /wiki/Rhubarb\n\n"
        "www.example.com: /a_b c");
    CREDO_GRADUS("scopus = URL primae apparitionis (cum ancora)");
    CREDO_VERUM(nota_nexus_est(t, ZEPHYRUM, "Alpha",
        "https://la.wikipedia.org/wiki/Alpha#x"));
    CREDO_AEQUALIS_I32(xar_numerus(t->notae_nexuum), IV);
}

interior vacuum
_probare_nexus_nulli (
    CredoContextus* c)
{
          Articulus* a;
    ArticulusSectio* s;
    NavigatorTextus* t;

    a = (Articulus*)piscina_allocare(c->piscina, magnitudo(Articulus));
    a->titulus = chorda_ex_literis("Nihil", c->piscina);
    a->fons      =
        chorda_ex_literis("https://la.wikipedia.org/wiki/Nihil",
        c->piscina);
    a->sectiones  = xar_creare(c->piscina, magnitudo(ArticulusSectio));
    s             = sectionem_addere(a, "", c->piscina);
    clausulam_addere(s->clausulae, ARTICULUS_PARAGRAPHUS, ZEPHYRUM, "",
        "Sine nexibus.", c->piscina);
    t = navigator_textum_facere(a, c->piscina);
    CREDO_NECESSE_NON_NIHIL(t);
    CREDO_GRADUS("nullis nexibus: index vacuus");
    CREDO_AEQUALIS_I32((i32)t->nexus.textus.mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(t->nexus.clausulae), ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(t->notae_nexuum), ZEPHYRUM);
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

    CREDO_GRADUS("index nexuum: lineae = scopi distincti (via alia)");
    {
        Xar* visi = xar_creare(c->piscina, (i32)magnitudo(chorda));
        i32  j;
        i32  k;
        s32  p1;
        s32  p2;
        s32  p3;

        per (j = ZEPHYRUM; j < xar_numerus(t->notae); j++)
        {
            chorda s = nota(t, j)->scopus;
               s32 h = chorda_invenire_index(s,
                   chorda_ex_literis("#",
                   c->piscina));

            si (nota(t, j)->genus != ARTICULUS_NEXUS)
            {
                perge;
            }
            si (h >= ZEPHYRUM)
            {
                s = chorda_sectio(s, ZEPHYRUM, (i32)h);
            }
            per (k = ZEPHYRUM; k < xar_numerus(visi); k++)
            {
                si (chorda_aequalis(*(chorda*)xar_obtinere(visi, k), s))
                {
                    frange;
                }
            }
            si (k == xar_numerus(visi))
            {
                *(chorda*)xar_addere(visi) = s;
            }
        }
        CREDO_VERUM(xar_numerus(visi) > XX);
        CREDO_AEQUALIS_I32(xar_numerus(t->notae_nexuum),
            xar_numerus(visi));
        CREDO_AEQUALIS_I32(xar_numerus(t->nexus.clausulae),
            xar_numerus(visi) + I);
        p1 = chorda_invenire_index(t->nexus.textus, chorda_ex_literis(
            "\nPetiole (botany)\n", c->piscina));
        p2 = chorda_invenire_index(t->nexus.textus, chorda_ex_literis(
            "\nRheum (plant)\n", c->piscina));
        p3 = chorda_invenire_index(t->nexus.textus, chorda_ex_literis(
            "\nPolygonaceae\n", c->piscina));
        CREDO_VERUM(p1 > ZEPHYRUM && p1 < p2 && p2 < p3);
    }

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

/* N3.5: columnae laterales ex adiunctis - Rhubarb: taxi (prooemium)
 * et nutritionis (Uses), ancorae in sectionibus suis, "Clavis: valor",
 * nexus intra columnam */
interior vacuum
_probare_columnae (
    CredoContextus* c)
{
              chorda  html;
           Articulus* a;
     NavigatorTextus* t;
    NavigatorColumna* k;
    NavigatorColumna* n;
                 s32  usus;
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

    CREDO_GRADUS("columnae duae (prooemium, Uses), ordine textus");
    CREDO_NECESSE(t->columnae && xar_numerus(t->columnae) == II);
    k = (NavigatorColumna*)xar_obtinere(t->columnae, ZEPHYRUM);
    n = (NavigatorColumna*)xar_obtinere(t->columnae, I);
    CREDO_VERUM(k->ancora < n->ancora);

    CREDO_GRADUS("ancora: clausula prima sectionis (prooemium post "
        "titulum articuli; Uses = titulus eius)");
    CREDO_VERUM(k->ancora > ZEPHYRUM);
    usus = chorda_invenire_index(t->caput.textus, chorda_ex_literis(
        "\n\nUses\n", c->piscina));
    CREDO_VERUM(usus < ZEPHYRUM || (s32)n->ancora <= usus + II + IV);
    CREDO_VERUM(chorda_incipit(chorda_sectio(t->caput.textus, n->ancora,
        t->caput.textus.mensura), chorda_ex_literis("Uses",
        c->piscina)));

    CREDO_GRADUS("taxi: titulus, 'Kingdom: Plantae'; nutritio: "
        "'Dietary fiber: 1.8 g'");
    CREDO_VERUM(chorda_incipit(k->caput.textus, chorda_ex_literis(
        "Rhubarb (culinary hybrid)", c->piscina)));
    CREDO_VERUM(chorda_continet(k->caput.textus, chorda_ex_literis(
        "Kingdom: Plantae", c->piscina)));
    CREDO_VERUM(chorda_continet(k->caput.textus, chorda_ex_literis(
        "Scientific classification", c->piscina)));
    CREDO_VERUM(chorda_continet(n->caput.textus, chorda_ex_literis(
        "Dietary fiber: 1.8 g", c->piscina)));
    CREDO_FALSUM(chorda_continet(t->caput.textus, chorda_ex_literis(
        "Kingdom: Plantae", c->piscina)));

    CREDO_GRADUS("nexus columnae intra textum eius; paginatur");
    CREDO_VERUM(xar_numerus(k->notae) > ZEPHYRUM);
    per (i = ZEPHYRUM; i < xar_numerus(k->notae); i++)
    {
        constans ArticulusNota* q = (constans ArticulusNota*)
            xar_obtinere(k->notae, i);

        si (   q->initium >= q->finis
            || q->finis > k->caput.textus.mensura)
        {
            mala++;
        }
    }
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
    CREDO_VERUM(lector_paginare(&k->caput, D, DC, c->piscina) != NIHIL);

    CREDO_GRADUS("articulus sine adiunctis: columnae nullae");
    CREDO_VERUM(navigator_textum_facere(NIHIL, c->piscina)->columnae
        == NIHIL || xar_numerus(navigator_textum_facere(NIHIL,
        c->piscina)->columnae) == ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "textus", _probare_textus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "columnae", _probare_columnae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "notae", _probare_notae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "omissa", _probare_omissa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nexus", _probare_nexus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nexus distincti", _probare_nexus_distincti, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "nexus nulli", _probare_nexus_nulli, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rheum", _probare_rheum, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("navigator_paginae", SECTIONES);
}
