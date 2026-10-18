/* probatio_sancti.c - bibliotheca sanctorum (project-specs/
 * sancti-plan.md S0): lectio plagulae plenae et dierum incertorum,
 * vitia lectoris nominata (canon, id, fontes, dies, ortus post
 * obitum, nomen principale), corpus (ordo, inventio, dies, quaestio,
 * iunctura sanctoralis), vitia corporis (id duplex, plagula mala, id
 * ignotum in iunctura). Fixturae: probationes/fixa/sancti{,_vitia}. */
#include "latina.h"
#include "credo.h"
#include "sancti.h"
#include "filum.h"
#include <string.h>

#define FIXA "probationes/fixa/sancti"

interior Sanctus*
_legere_fixam (
        CredoContextus* c,
    constans character* via,
                   Xar* vitia)
{
    chorda textus;

    textus = filum_legere_totum(via, c->piscina);
    redde sancti_legere(textus, chorda_ex_literis(via, c->piscina),
        c->piscina, internamentum_creare(c->piscina), vitia);
}

/* vitia legere textum: NIHIL exspectatur, vitium cum verbo */
interior b32
_vitium_nominat (
        CredoContextus* c,
    constans character* textus,
    constans character* via,
    constans character* verbum)
{
     Xar* vitia;
     i32  i;

    vitia = xar_creare(c->piscina, (i32)magnitudo(SanctiVitium));
    si (sancti_legere(chorda_ex_literis(textus, c->piscina),
            chorda_ex_literis(via, c->piscina), c->piscina,
            internamentum_creare(c->piscina), vitia) != NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(vitia); i++)
    {
        SanctiVitium* v = (SanctiVitium*)xar_obtinere(vitia, i);

        si (chorda_continet(v->nuntius, chorda_ex_literis(verbum,
                c->piscina)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_sectio_plena (
    CredoContextus* c)
{
          Xar* vitia;
      Sanctus* s;
 SanctiFestum* f;
   SanctiFons* fons;

    vitia  = xar_creare(c->piscina, (i32)magnitudo(SanctiVitium));
    s      = _legere_fixam(c, FIXA "/a/agnes-of-rome.sanctus", vitia);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_AEQUALIS_I32(xar_numerus(vitia), ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(s->id, "agnes-of-rome");
    CREDO_VERUM(s->genus == SANCTI_PERSONA);
    CREDO_VERUM(s->status == SANCTI_SANCTUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(s->titulus, "Agnes of Rome");
    CREDO_CHORDA_AEQUALIS_LITERIS(s->titulus_latinus, "Agnes");
    CREDO_AEQUALIS_I32(xar_numerus(s->nomina), II);
    CREDO_VERUM(!s->ortus.notus);
    CREDO_VERUM(s->obitus.notus);
    CREDO_VERUM(s->martyr);
    CREDO_AEQUALIS_S32(s->obitus.annus, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(s->obitus.locus, "Rome");
    CREDO_VERUM(s->canonizatio
        == SANCTI_CANONIZATIO_ANTE_CONGREGATIONEM);
    CREDO_AEQUALIS_I32(xar_numerus(s->festa), II);
    f = (SanctiFestum*)xar_obtinere(s->festa, ZEPHYRUM);
    CREDO_VERUM(f->calendarium == SANCTI_CALENDARIUM_GENERALE);
    CREDO_AEQUALIS_S32(f->mensis, I);
    CREDO_AEQUALIS_S32(f->dies, XXI);
    CREDO_VERUM(f->gradus == SANCTI_GRADUS_MEMORIA);
    f = (SanctiFestum*)xar_obtinere(s->festa, I);
    CREDO_VERUM(f->calendarium == SANCTI_CALENDARIUM_MARTYROLOGIUM);
    CREDO_VERUM(f->gradus == SANCTI_GRADUS_NULLUS);
    CREDO_AEQUALIS_I32(xar_numerus(s->munera), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(s->munera, I),
        "martyr");
    CREDO_AEQUALIS_I32(xar_numerus(s->paragraphi), II);
    CREDO_VERUM(chorda_incipit(*(chorda*)xar_obtinere(s->paragraphi,
        ZEPHYRUM), chorda_ex_literis("No virgin martyr of Rome",
        c->piscina)));
    CREDO_AEQUALIS_I32(xar_numerus(s->fontes), III);
    fons = (SanctiFons*)xar_obtinere(s->fontes, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(fons->id, "ce");
    CREDO_CHORDA_AEQUALIS_LITERIS(fons->opus, "ce1913");
    CREDO_CHORDA_AEQUALIS_LITERIS(fons->locus, "St. Agnes of Rome");
    CREDO_VERUM(chorda_incipit(fons->url, chorda_ex_literis("https://",
        c->piscina)));
    CREDO_AEQUALIS_I32(xar_numerus(fons->excerpta), VI);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(fons->excerpta,
        II), "St. Ambrose gives her age as twelve");
}

interior vacuum
_sectio_dies (
    CredoContextus* c)
{
          Xar* vitia;
      Sanctus* s;
 SanctiFestum* f;

    vitia  = xar_creare(c->piscina, (i32)magnitudo(SanctiVitium));
    s      = _legere_fixam(c, FIXA "/t/thomas-aquinas.sanctus", vitia);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_AEQUALIS_I32(xar_numerus(s->nomina), III);
    CREDO_VERUM(s->ortus.notus);
    CREDO_AEQUALIS_S32(s->ortus.annus, MCCXXV);
    CREDO_VERUM(s->ortus.circa);
    CREDO_AEQUALIS_S32(s->ortus.mensis, ZEPHYRUM);
    CREDO_AEQUALIS_S32(s->obitus.annus, MCCLXXIV);
    CREDO_AEQUALIS_S32(s->obitus.mensis, III);
    CREDO_AEQUALIS_S32(s->obitus.dies, VII);
    CREDO_VERUM(!s->obitus.circa);
    CREDO_VERUM(!s->martyr);
    CREDO_VERUM(s->canonizatio == SANCTI_CANONIZATIO_FORMALIS);
    CREDO_AEQUALIS_S32(s->canonizatio_dies.annus, MCCCXXIII);
    CREDO_AEQUALIS_S32(s->canonizatio_dies.dies, XVIII);
    CREDO_CHORDA_AEQUALIS_LITERIS(s->canonizatio_papa, "John XXII");
    CREDO_VERUM(!s->beatificatio.notus);
    f = (SanctiFestum*)xar_obtinere(s->festa, I);
    CREDO_VERUM(f->calendarium == SANCTI_CALENDARIUM_VETUS);
    CREDO_AEQUALIS_I32(xar_numerus(s->patrocinia), I);
    s = _legere_fixam(c,
        FIXA "/c/charles-lwanga-and-companions.sanctus",
        vitia);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_VERUM(s->genus == SANCTI_COETUS);
    CREDO_AEQUALIS_S32(s->obitus.saeculum, XIX);
    CREDO_VERUM(chorda_vacua(s->titulus_latinus));
    CREDO_AEQUALIS_I32(xar_numerus(s->socii), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(((SanctiSocius*)xar_obtinere(s->socii,
        ZEPHYRUM))->ad, "charles-lwanga");
    CREDO_VERUM(chorda_vacua(((SanctiSocius*)xar_obtinere(s->socii,
        I))->ad));
    CREDO_AEQUALIS_I32(xar_numerus(vitia), ZEPHYRUM);
}

#define CAPUT "<sanctus id=\"x\" versio=\"1\" genus=\"persona\" " \
              "status=\"sanctus\">\n"
#define NOMEN "  <nomen lingua=\"en\" principale=\"verum\">X</nomen>\n"
#define FONS  "  <fons id=\"f\" opus=\"sedes\" locus=\"l\">" \
              "<excerptum>e</excerptum></fons>\n</sanctus>\n"

interior vacuum
_sectio_vitia (
    CredoContextus* c)
{
    /* sanum ipsum transit (controla) */
    CREDO_VERUM(sancti_legere(chorda_ex_literis(CAPUT NOMEN FONS,
        c->piscina), chorda_ex_literis("s/x.sanctus", c->piscina),
        c->piscina, internamentum_creare(c->piscina),
        xar_creare(c->piscina, (i32)magnitudo(SanctiVitium))) != NIHIL);
    CREDO_VERUM(_vitium_nominat(c,
        "<sanctus id=\"x\" versio=\"1\" genus=\"persona\" "
        "status=\"beatissimus\">\n" NOMEN FONS, "s/x.sanctus",
        "status"));
    CREDO_VERUM(_vitium_nominat(c, CAPUT NOMEN FONS, "s/alius.sanctus",
        "id"));
    CREDO_VERUM(_vitium_nominat(c, CAPUT NOMEN
        "  <munus fontes=\"f g\">martyr</munus>\n" FONS, "s/x.sanctus",
        "fontes"));
    CREDO_VERUM(_vitium_nominat(c, CAPUT NOMEN
        "  <festum calendarium=\"generale\" mensis=\"4\" dies=\"31\" "
        "fontes=\"f\"/>\n" FONS, "s/x.sanctus", "dies"));
    CREDO_VERUM(_vitium_nominat(c, CAPUT NOMEN
        "  <ortus annus=\"1300\" fontes=\"f\"/>\n"
        "  <obitus annus=\"1274\" fontes=\"f\"/>\n" FONS, "s/x.sanctus",
        "ortus"));
    CREDO_VERUM(_vitium_nominat(c, CAPUT NOMEN
        "  <nomen lingua=\"en\" principale=\"verum\">Y</nomen>\n" FONS,
        "s/x.sanctus", "principale"));
    CREDO_VERUM(_vitium_nominat(c, CAPUT
        "  <nomen lingua=\"en\">X</nomen>\n" FONS, "s/x.sanctus",
        "principale"));
    CREDO_VERUM(_vitium_nominat(c, "<sanctus id=\"x\"", "s/x.sanctus",
        "STML"));
}

interior b32
_ids_sunt (
                   Xar* sancti,
    constans character* a,
    constans character* b)
{
    si (xar_numerus(sancti) != (b ? II : (a ? I : ZEPHYRUM)))
    {
        redde FALSUM;
    }
    si (   a && !chorda_aequalis_literis((*(constans Sanctus**)
            xar_obtinere(sancti, ZEPHYRUM))->id, a))
    {
        redde FALSUM;
    }
    redde !b || chorda_aequalis_literis((*(constans Sanctus**)
        xar_obtinere(sancti, I))->id, b);
}

interior vacuum
_sectio_corpus (
    CredoContextus* c)
{
    SanctiCorpus* corpus;

    corpus = sancti_corpus_aperire(FIXA, c->piscina,
        internamentum_creare(c->piscina));
    CREDO_NECESSE_NON_NIHIL(corpus);
    CREDO_AEQUALIS_I32(xar_numerus(sancti_vitia(corpus)), ZEPHYRUM);
    CREDO_AEQUALIS_I32(sancti_numerus(corpus), III);
    CREDO_CHORDA_AEQUALIS_LITERIS(sancti_ad_indicem(corpus,
        ZEPHYRUM)->id, "agnes-of-rome");
    CREDO_CHORDA_AEQUALIS_LITERIS(sancti_ad_indicem(corpus, I)->id,
        "charles-lwanga-and-companions");
    CREDO_CHORDA_AEQUALIS_LITERIS(sancti_ad_indicem(corpus, II)->id,
        "thomas-aquinas");
    CREDO_VERUM(sancti_ad_indicem(corpus, III) == NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(sancti_invenire(corpus,
        chorda_ex_literis("thomas-aquinas", c->piscina))->titulus,
        "Thomas Aquinas");
    CREDO_VERUM(sancti_invenire(corpus, chorda_ex_literis("nemo",
        c->piscina)) == NIHIL);
    CREDO_VERUM(_ids_sunt(sancti_diei(corpus,
        SANCTI_CALENDARIUM_GENERALE, I, XXI, c->piscina),
        "agnes-of-rome", NIHIL));
    CREDO_VERUM(_ids_sunt(sancti_diei(corpus,
        SANCTI_CALENDARIUM_MARTYROLOGIUM, I, XXI, c->piscina),
        "agnes-of-rome", NIHIL));
    CREDO_VERUM(_ids_sunt(sancti_diei(corpus,
        SANCTI_CALENDARIUM_GENERALE, I, XXII, c->piscina), NIHIL,
        NIHIL));
    CREDO_VERUM(_ids_sunt(sancti_diei(corpus,
        SANCTI_CALENDARIUM_VETUS, III, VII, c->piscina),
        "thomas-aquinas", NIHIL));
    /* dies alterius calendarii: generale VII III vacuum */
    CREDO_VERUM(_ids_sunt(sancti_diei(corpus,
        SANCTI_CALENDARIUM_GENERALE, III, VII, c->piscina), NIHIL,
        NIHIL));
    /* quaestio per nomina omnia, sine maiusculis */
    CREDO_VERUM(_ids_sunt(sancti_quaerere(corpus,
        chorda_ex_literis("AQUIN", c->piscina), c->piscina),
        "thomas-aquinas", NIHIL));
    CREDO_VERUM(_ids_sunt(sancti_quaerere(corpus,
        chorda_ex_literis("tommaso", c->piscina), c->piscina),
        "thomas-aquinas", NIHIL));
    CREDO_VERUM(_ids_sunt(sancti_quaerere(corpus,
        chorda_ex_literis("a", c->piscina), c->piscina),
        "agnes-of-rome", "charles-lwanga-and-companions") == FALSUM);
    CREDO_AEQUALIS_I32(xar_numerus(sancti_quaerere(corpus,
        chorda_ex_literis("a", c->piscina), c->piscina)), III);
}

interior vacuum
_sectio_celebratio (
    CredoContextus* c)
{
    SanctiCorpus* corpus;
             Xar* s;

    corpus = sancti_corpus_aperire(FIXA, c->piscina,
        internamentum_creare(c->piscina));
    CREDO_NECESSE_NON_NIHIL(corpus);
    CREDO_VERUM(_ids_sunt(sancti_celebrationis(corpus, I, XXI,
        chorda_ex_literis("Saint Agnes", c->piscina), c->piscina),
        "agnes-of-rome", NIHIL));
    s = sancti_celebrationis(corpus, II, II, chorda_ex_literis(
        "The Presentation of the Lord", c->piscina), c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_AEQUALIS_I32(xar_numerus(s), ZEPHYRUM);
    CREDO_VERUM(sancti_celebrationis(corpus, I, I, chorda_ex_literis(
        "Saint Agnes", c->piscina), c->piscina) == NIHIL);
    /* mensis alius, dies idem */
    CREDO_VERUM(sancti_celebrationis(corpus, II, XXI, chorda_ex_literis(
        "Saint Agnes", c->piscina), c->piscina) == NIHIL);
    CREDO_VERUM(sancti_celebrationis(corpus, I, XXI, chorda_ex_literis(
        "Saint Nemo", c->piscina), c->piscina) == NIHIL);
}

interior vacuum
_sectio_corpus_vitia (
    CredoContextus* c)
{
     SanctiCorpus* corpus;
              i32  i;
              b32  geminum;
              b32  malus;
              b32  iunctura;

    corpus = sancti_corpus_aperire("probationes/fixa/sancti_vitia",
        c->piscina, internamentum_creare(c->piscina));
    CREDO_NECESSE_NON_NIHIL(corpus);
    CREDO_AEQUALIS_I32(sancti_numerus(corpus), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(sancti_ad_indicem(corpus,
        ZEPHYRUM)->titulus, "Duplex Primus");
    geminum = malus = iunctura = FALSUM;
    per (i = ZEPHYRUM; i < xar_numerus(sancti_vitia(corpus)); i++)
    {
        constans SanctiVitium* v = (constans SanctiVitium*)
            xar_obtinere(sancti_vitia(corpus), i);

        si (   chorda_continet(v->via, chorda_ex_literis("b/duplex",
                c->piscina))
            && chorda_continet(v->nuntius, chorda_ex_literis("duplex",
                c->piscina)))
        {
            geminum = VERUM;
        }
        si (   chorda_continet(v->via, chorda_ex_literis("m/malus",
                c->piscina)) && v->linea > ZEPHYRUM)
        {
            malus = VERUM;
        }
        si (   chorda_continet(v->via,
            chorda_ex_literis("sanctorale.tsv",
                c->piscina))
            && chorda_continet(v->nuntius, chorda_ex_literis(
                "nemo-ignotus", c->piscina))
            && v->linea == II)
        {
            iunctura = VERUM;
        }
    }
    CREDO_VERUM(geminum);
    CREDO_VERUM(malus);
    CREDO_VERUM(iunctura);
    CREDO_AEQUALIS_I32(xar_numerus(sancti_vitia(corpus)), III);
    CREDO_VERUM(sancti_corpus_aperire("probationes/fixa/nusquam",
        c->piscina, internamentum_creare(c->piscina)) == NIHIL);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "plena", _sectio_plena, NIHIL, NIHIL, NIHIL, NIHIL },
    { "dies", _sectio_dies, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitia", _sectio_vitia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "corpus", _sectio_corpus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "celebratio", _sectio_celebratio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "corpus_vitia", _sectio_corpus_vitia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("sancti", SECTIONES);
}
