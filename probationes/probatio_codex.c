/* probatio_codex.c - liber EPUB (lector L2): aperire et metadata,
 * capita ex indice, textus et clausulae, sententiae et emendationes.
 * Fixa in probationes/fixa/codex/ (fontes + .epub, zip Apple factae):
 * minimus (EPUB 3, nav), vetus (EPUB 2, NCX nidificata), sine_prooemio,
 * fractus (<p> non clausum in c2, index ad ancoram absentem c2#nusquam;
 * caput IV in membro absente),
 * sine_opf. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "filum.h"
#include "codex.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* fixum probationes/fixa/codex/<nomen>.epub apertum; NIHIL si non */
interior Codex*
fixum_aperire (
     constans character* titulus,
                Piscina* p,
                 chorda* vitium)
{
    character via[CXXVIII];

    strcpy(via, "probationes/fixa/codex/");
    strcat(via, titulus);
    strcat(via, ".epub");
    vitium->datum    = NIHIL;
    vitium->mensura  = ZEPHYRUM;
    redde codex_aperire(filum_legere_totum(via, p), p,
        internamentum_creare(p), vitium);
}

interior vacuum
_sectio_aperire (
    CredoContextus* c)
{
     Piscina* p;
       Codex* l;
      chorda  vitium;
      chorda  mala;
   character  x[L];

    p = c->piscina;

    CREDO_GRADUS("minimus: metadata (identitas ex unique-identifier)");
    l = fixum_aperire("minimus", p, &vitium);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_CHORDA_AEQUALIS_LITERIS(codex_identitas(l),
        "urn:rhubarb:minimus");
    CREDO_CHORDA_AEQUALIS_LITERIS(codex_titulus(l), "Minimus");
    CREDO_CHORDA_AEQUALIS_LITERIS(codex_auctor(l), "Anna Scriptrix");
    CREDO_CHORDA_AEQUALIS_LITERIS(codex_lingua(l), "la");

    CREDO_GRADUS("non arca ZIP: vitium");
    memset(x, 'x', magnitudo(x));
    mala.datum      = (i8*)x;
    mala.mensura    = L;
    vitium.mensura  = ZEPHYRUM;
    CREDO_NIHIL(codex_aperire(mala, p, internamentum_creare(p),
        &vitium));
    CREDO_VERUM(vitium.mensura > ZEPHYRUM);

    CREDO_GRADUS("arca sine META-INF/container.xml: vitium");
    vitium.mensura = ZEPHYRUM;
    CREDO_NIHIL(codex_aperire(filum_legere_totum(
        "probationes/fixa/sarcina/exemplum.zip", p), p,
        internamentum_creare(p), &vitium));
    CREDO_VERUM(chorda_continet(vitium, chorda_ex_literis("container",
        p)));

    CREDO_GRADUS("container ad OPF absentem: vitium");
    CREDO_NIHIL(fixum_aperire("sine_opf", p, &vitium));
    CREDO_VERUM(vitium.mensura > ZEPHYRUM);
}


/* capita ex indice: prooemium (si textus ante introitum primum), unum
 * per introitum; NCX nidificata plana; sine prooemio nullum vacuum */
interior b32
capita_sunt (
             Codex* l,
    constans character* constans* tituli,
                    i32  n)
{
    i32 k;

    si (codex_numerus_capitum(l) != n)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (!chorda_aequalis_literis(codex_caput_titulus(l, k),
                tituli[k]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_sectio_capita (
    CredoContextus* c)
{
     Piscina* p;
       Codex* l;
      chorda  vitium;
    constans character* constans quattuor[] = { "", "Caput I",
        "Caput II", "Caput III" };

    p = c->piscina;

    CREDO_GRADUS("minimus (nav): prooemium + III capita");
    l = fixum_aperire("minimus", p, &vitium);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(capita_sunt(l, quattuor, IV));
    CREDO_AEQUALIS_I32(codex_caput_titulus(l, IV).mensura, ZEPHYRUM);

    CREDO_GRADUS("vetus (EPUB 2, NCX nidificata): eadem IV");
    l = fixum_aperire("vetus", p, &vitium);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(capita_sunt(l, quattuor, IV));

    CREDO_GRADUS("sine_prooemio: III, nullum prooemium vacuum");
    l = fixum_aperire("sine_prooemio", p, &vitium);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(capita_sunt(l, quattuor + I, III));
}

/* textus capitis: clausulae "\n\n" iunctae, <br/> in titulo et versu
 * = '\n', spatia contracta, entitates decoctae; involucra omissa */
interior b32
clausula_est (
    constans CodexCaput* caput,
                    i32  i,
             CodexGenus  genus,
     constans character* textus)
{
    constans CodexClausula* b;

    si (!caput || i >= xar_numerus(caput->clausulae))
    {
        redde FALSUM;
    }
    b = (constans CodexClausula*)xar_obtinere(caput->clausulae, i);
    redde b->genus == genus && b->finis <= caput->textus.mensura
        && chorda_aequalis_literis(chorda_sectio(caput->textus,
        b->initium, b->finis), textus);
}

interior vacuum
_sectio_textus (
    CredoContextus* c)
{
        Piscina* p;
          Codex* l;
     CodexCaput* k;
         chorda  vitium;

    p = c->piscina;
    l = fixum_aperire("minimus", p, &vitium);
    CREDO_NECESSE_NON_NIHIL(l);

    CREDO_GRADUS("prooemium: solum corpus (titulus capitis omissus)");
    k = codex_caput_legere(l, ZEPHYRUM, p, &vitium);
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_CHORDA_AEQUALIS_LITERIS(k->textus, "Prooemium.");

    CREDO_GRADUS("caput I: titulus cum <br/>, paragraphus contractus, "
        "&amp; decoctum, finis ante cap2");
    k = codex_caput_legere(l, I, p, &vitium);
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_CHORDA_AEQUALIS_LITERIS(k->textus,
        "Caput I\nDe Cranfordia\n\n"
        "In primis Cranfordia ab Amazonibus tenetur & regitur.");
    CREDO_AEQUALIS_I32(xar_numerus(k->clausulae), II);
    CREDO_VERUM(clausula_est(k, ZEPHYRUM, CODEX_TITULUS,
        "Caput I\nDe Cranfordia"));
    CREDO_VERUM(clausula_est(k, I, CODEX_PARAGRAPHUS,
        "In primis Cranfordia ab Amazonibus tenetur & regitur."));
    CREDO_VERUM(chorda_aequalis_literis(k->titulus, "Caput I"));

    CREDO_GRADUS("caput II: ab ancora cap2; involucrum (\"back\") "
        "omissum");
    k = codex_caput_legere(l, II, p, &vitium);
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_CHORDA_AEQUALIS_LITERIS(k->textus,
        "Caput II\n\nSecundum caput.");

    CREDO_GRADUS("caput III: versus (poem > stanza) et paragraphus");
    k = codex_caput_legere(l, III, p, &vitium);
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_CHORDA_AEQUALIS_LITERIS(k->textus,
        "line one\nline two\n\nPrimum hic. Secundum ibi. Tertium!");
    CREDO_VERUM(clausula_est(k, ZEPHYRUM, CODEX_VERSUS,
        "line one\nline two"));
    CREDO_VERUM(clausula_est(k, I, CODEX_PARAGRAPHUS,
        "Primum hic. Secundum ibi. Tertium!"));

    CREDO_GRADUS("vetus caput III: stanzae II clausulae, &#8212; "
        "decoctum, spatium solum inter <i> servatum");
    l = fixum_aperire("vetus", p, &vitium);
    CREDO_NECESSE_NON_NIHIL(l);
    k = codex_caput_legere(l, III, p, &vitium);
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_CHORDA_AEQUALIS_LITERIS(k->textus,
        "Arma virumque\ncano\xe2\x80\x94Troiae\n\n"
        "qui primus ab oris\n\nPrimum hic. Secundum ibi. Tertium!");
    CREDO_VERUM(clausula_est(k, ZEPHYRUM, CODEX_VERSUS,
        "Arma virumque\ncano\xe2\x80\x94Troiae"));
    CREDO_VERUM(clausula_est(k, I, CODEX_VERSUS, "qui primus ab oris"));
    CREDO_VERUM(clausula_est(k, II, CODEX_PARAGRAPHUS,
        "Primum hic. Secundum ibi. Tertium!"));
    l = fixum_aperire("minimus", p, &vitium);
    CREDO_NECESSE_NON_NIHIL(l);

    CREDO_GRADUS("extra ambitum: NIHIL et vitium");
    vitium.mensura = ZEPHYRUM;
    CREDO_NIHIL(codex_caput_legere(l, IV, p, &vitium));
    CREDO_VERUM(vitium.mensura > ZEPHYRUM);

    CREDO_GRADUS("fractus: <p> non clausum toleratur; ancora "
        "c2#nusquam abest -> documentum totum");
    l = fixum_aperire("fractus", p, &vitium);
    CREDO_NECESSE_NON_NIHIL(l);
    k = codex_caput_legere(l, III, p, &vitium);
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_CHORDA_AEQUALIS_LITERIS(k->textus,
        "line one\nline two\n\nPrimum hic. Secundum ibi. Tertium!");

    CREDO_GRADUS("fractus: caput IV in membro absente -> NIHIL, "
        "vitium");
    vitium.mensura = ZEPHYRUM;
    CREDO_NIHIL(codex_caput_legere(l, IV, p, &vitium));
    CREDO_VERUM(chorda_continet(vitium, chorda_ex_literis("absens",
        p)));
}

/* sententiae: textus sententiarum ordine (fragmenta textus capitis) */
interior b32
sententiae_sunt (
    constans CodexCaput* caput,
                   Xar* sententiae,
    constans character* constans* textus,
                    i32  n)
{
    constans CodexSententia* t;
                        i32  k;

    si (!sententiae || xar_numerus(sententiae) != n)
    {
        imprimere("  sententiae: %u pro %u\n", sententiae
            ? xar_numerus(sententiae) : ZEPHYRUM, n);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        t = (constans CodexSententia*)xar_obtinere(sententiae, k);
        si (   t->finis > caput->textus.mensura || t->initium > t->finis
            || !chorda_aequalis_literis(chorda_sectio(caput->textus,
                t->initium, t->finis), textus[k]))
        {
            imprimere("  sententia %u: [%.*s] pro [%s]\n", k,
                (integer)(t->finis - t->initium),
                (constans character*)caput->textus.datum + t->initium,
                textus[k]);
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* offset primae occurrentiae litterarum in textu capitis */
interior i32
locus (
    constans CodexCaput* caput,
     constans character* litterae)
{
    i32 n;
    i32 i;

    n = (i32)strlen(litterae);
    per (i = ZEPHYRUM; i + n <= caput->textus.mensura; i++)
    {
        si (memcmp(caput->textus.datum + i, litterae, (size_t)n)
            == ZEPHYRUM)
        {
            redde i;
        }
    }
    redde caput->textus.mensura;
}

interior vacuum
_sectio_sententiae (
    CredoContextus* c)
{
             Piscina* p;
               Codex* l;
          CodexCaput* k;
          CodexCaput  motum;
      CodexEmendatio  e[III];
              chorda  vitium;
                 Xar* x;
    constans character* constans omnes[] = { "line one", "line two",
        "Primum hic.", "Secundum ibi.", "Tertium!" };
    constans character* constans iuncta[] = { "line one", "line two",
        "Primum hic. Secundum ibi.", "Tertium!" };
    constans character* constans fissa[] = { "line one", "line two",
        "Primum hic.", "Secundum", "ibi.", "Tertium!" };
    constans character* constans tres[] = { "line one", "line two",
        "Primum hic. Secundum ibi. Tertium!" };
    constans character* constans versus[] = { "line one\nline two",
        "Primum hic.", "Secundum ibi.", "Tertium!" };
    constans character* constans verbum[] = { "line one", "line two",
        "Primum hic.", "Secun", "dum ibi.", "Tertium!" };
    constans character* constans titulus[] = { "Caput I\nDe Cranfordia",
        "In primis Cranfordia ab Amazonibus tenetur & regitur." };
    constans character* constans strophae[] = { "Arma virumque",
        "cano\xe2\x80\x94Troiae", "qui primus ab oris", "Primum hic.",
        "Secundum ibi.", "Tertium!" };

    p = c->piscina;
    l = fixum_aperire("minimus", p, &vitium);
    CREDO_NECESSE_NON_NIHIL(l);
    k = codex_caput_legere(l, III, p, &vitium);
    CREDO_NECESSE_NON_NIHIL(k);

    CREDO_GRADUS("sine emendationibus: versus per lineas, paragraphus "
        "per sententias");
    x = codex_sententiae(k, NIHIL, ZEPHYRUM, p);
    CREDO_VERUM(sententiae_sunt(k, x, omnes, V));
    CREDO_AEQUALIS_I32(((CodexSententia*)xar_obtinere(x,
        ZEPHYRUM))->clausula, ZEPHYRUM);
    CREDO_AEQUALIS_I32(((CodexSententia*)xar_obtinere(x,
        IV))->clausula, I);

    CREDO_GRADUS("titulus = sententia una (cum '\\n')");
    x = codex_sententiae(codex_caput_legere(l, I, p, &vitium), NIHIL,
        ZEPHYRUM, p);
    CREDO_VERUM(sententiae_sunt(codex_caput_legere(l, I, p, &vitium), x,
        titulus, II));

    CREDO_GRADUS("iungere ad 'Primum': initium idem, 'Tertium' idem");
    e[ZEPHYRUM].genus    = CODEX_IUNGERE;
    e[ZEPHYRUM].positio  = locus(k, "Primum");
    x                    = codex_sententiae(k, e, I, p);
    CREDO_VERUM(sententiae_sunt(k, x, iuncta, IV));
    CREDO_AEQUALIS_I32(((CodexSententia*)xar_obtinere(x,
        II))->initium, locus(k, "Primum"));
    CREDO_AEQUALIS_I32(((CodexSententia*)xar_obtinere(x,
        III))->initium, locus(k, "Tertium"));

    CREDO_GRADUS("idem bis = semel");
    e[I] = e[ZEPHYRUM];
    CREDO_VERUM(sententiae_sunt(k, codex_sententiae(k, e, II, p),
        iuncta,
        IV));

    CREDO_GRADUS("iuncturae catenatae, ordo non refert");
    e[ZEPHYRUM].genus    = CODEX_IUNGERE;
    e[ZEPHYRUM].positio  = locus(k, "Primum");
    e[I].genus           = CODEX_IUNGERE;
    e[I].positio         = locus(k, "Secundum");
    CREDO_VERUM(sententiae_sunt(k, codex_sententiae(k, e, II, p), tres,
        III));
    e[II] = e[ZEPHYRUM];
    CREDO_VERUM(sententiae_sunt(k, codex_sententiae(k, e + I, II, p),
        tres, III));

    CREDO_GRADUS("findere ad 'ibi': initium = positio");
    e[ZEPHYRUM].genus    = CODEX_FINDERE;
    e[ZEPHYRUM].positio  = locus(k, "ibi");
    x                    = codex_sententiae(k, e, I, p);
    CREDO_VERUM(sententiae_sunt(k, x, fissa, VI));
    CREDO_AEQUALIS_I32(((CodexSententia*)xar_obtinere(x,
        IV))->initium, locus(k, "ibi"));

    CREDO_GRADUS("findere et iungere eodem loco, ordo non refert: "
        "fissa prius, deinde iuncta");
    e[I].genus    = CODEX_IUNGERE;
    e[I].positio  = locus(k, "Secundum");
    CREDO_VERUM(sententiae_sunt(k, codex_sententiae(k, e, II, p), omnes,
        V));
    e[II] = e[ZEPHYRUM];
    CREDO_VERUM(sententiae_sunt(k, codex_sententiae(k, e + I, II, p),
        omnes, V));

    CREDO_GRADUS("nihil mutant: iungere ultimam, iungere trans "
        "clausulas, findere ad finem sententiae, positio ignota");
    e[ZEPHYRUM].genus    = CODEX_IUNGERE;
    e[ZEPHYRUM].positio  = locus(k, "Tertium");
    e[I].genus           = CODEX_IUNGERE;
    e[I].positio         = locus(k, "line two");
    e[II].genus          = CODEX_FINDERE;
    e[II].positio        = locus(k, "Primum");
    CREDO_VERUM(sententiae_sunt(k, codex_sententiae(k, e, III, p),
        omnes, V));
    e[ZEPHYRUM].genus    = CODEX_FINDERE;
    e[ZEPHYRUM].positio  = locus(k, " Secundum");
    e[I].genus           = CODEX_IUNGERE;
    e[I].positio         = DCCC;
    e[II].genus          = CODEX_FINDERE;
    e[II].positio        = DCCC;
    CREDO_VERUM(sententiae_sunt(k, codex_sententiae(k, e, III, p),
        omnes, V));

    CREDO_GRADUS("versus iungi possunt intra clausulam");
    e[ZEPHYRUM].genus    = CODEX_IUNGERE;
    e[ZEPHYRUM].positio  = ZEPHYRUM;
    CREDO_VERUM(sententiae_sunt(k, codex_sententiae(k, e, I, p), versus,
        IV));

    CREDO_GRADUS("findere ad spatium: pars nova ad non-spatium "
        "incipit");
    e[ZEPHYRUM].genus    = CODEX_FINDERE;
    e[ZEPHYRUM].positio  = locus(k, " ibi");
    x                    = codex_sententiae(k, e, I, p);
    CREDO_VERUM(sententiae_sunt(k, x, fissa, VI));
    CREDO_AEQUALIS_I32(((CodexSententia*)xar_obtinere(x,
        IV))->initium, locus(k, "ibi"));

    CREDO_GRADUS("findere intra verbum: ubi iubetur");
    e[ZEPHYRUM].genus    = CODEX_FINDERE;
    e[ZEPHYRUM].positio  = locus(k, "dum ibi");
    CREDO_VERUM(sententiae_sunt(k, codex_sententiae(k, e, I, p), verbum,
        VI));

    CREDO_GRADUS("vetus caput III: stanzae, versus per lineas");
    l = fixum_aperire("vetus", p, &vitium);
    CREDO_NECESSE_NON_NIHIL(l);
    x = codex_sententiae(codex_caput_legere(l, III, p, &vitium), NIHIL,
        ZEPHYRUM, p);
    CREDO_VERUM(sententiae_sunt(codex_caput_legere(l, III, p, &vitium),
        x, strophae, VI));

    CREDO_GRADUS("reperire: congruit -> positio");
    CREDO_AEQUALIS_S32(codex_reperire(k, locus(k, "Tertium"),
        chorda_ex_literis("Tertium", p)), (s32)locus(k, "Tertium"));

    CREDO_GRADUS("reperire: textus III octetis motus -> positio nova");
    motum          = *k;
    motum.textus   = chorda_concatenare(chorda_ex_literis("abc", p),
        k->textus, p);
    CREDO_AEQUALIS_S32(codex_reperire(&motum, locus(k, "Tertium"),
        chorda_ex_literis("Tertium", p)), (s32)locus(k, "Tertium")
            + III);
    CREDO_AEQUALIS_S32(codex_reperire(&motum, ZEPHYRUM,
        chorda_ex_literis("line one", p)), III);

    CREDO_GRADUS("reperire: vestigium absens -> -1; positio extra -> "
        "quaeritur");
    CREDO_AEQUALIS_S32(codex_reperire(k, locus(k, "Tertium"),
        chorda_ex_literis("Quartum", p)), -I);
    CREDO_AEQUALIS_S32(codex_reperire(k, k->textus.mensura + X,
        chorda_ex_literis("Tertium", p)), (s32)locus(k, "Tertium"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "aperire", _sectio_aperire, NIHIL, NIHIL, NIHIL, NIHIL },
    { "capita", _sectio_capita, NIHIL, NIHIL, NIHIL, NIHIL },
    { "textus", _sectio_textus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sententiae", _sectio_sententiae, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("codex", SECTIONES);
}
