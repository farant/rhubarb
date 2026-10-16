/* probatio_mensura_cursus.c - cursus in diario mensurae (analytica T1):
 * initium et finis lineae, conditio in volumen, coniunctio per id.
 *
 * Cursus = res cum initio, fine, exitu. Lineae DUAE in diario: initium
 * (contextus: titulus, argv, vocator, commissum, sordidum, ramus, arbor,
 * machina) et finis (exitus, duratio, rusage, acta). Cursus sine fine =
 * INTERRUPTUS (necatus, pendens, concha fracta) - visibilis, non
 * absens. Spec: project-specs/analytica-spec.md par. II.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "credo.h"
#include "volumen.h"
#include "mensura.h"
#include <stdio.h>
#include <string.h>

interior CursusInitium
_initium_fictum (
    constans character* id,
    constans character* parens,
    constans character* titulus,
                   s64  momentum,
               Piscina* piscina)
{
    CursusInitium i;

    memset(&i, ZEPHYRUM, magnitudo(i));
    i.id       = chorda_ex_literis(id, piscina);
    i.parens   = chorda_ex_literis(parens, piscina);
    i.sessio   = chorda_ex_literis("0199aabbccdd", piscina);
    i.titulus  = chorda_ex_literis(titulus, piscina);
    i.argv       =
        chorda_ex_literis("./md/compile_probationes.sh lexema",
        piscina);
    i.vocator    = chorda_ex_literis("homo", piscina);
    i.commissum  = chorda_ex_literis("6e754864", piscina);
    i.sordidum   = VERUM;
    i.ramus      = chorda_ex_literis("rhubarb-quinta", piscina);
    i.arbor      = chorda_ex_literis("/radix/rhubarb-quinta", piscina);
    i.machina    = chorda_ex_literis("9f0debc091664d38", piscina);
    i.initium    = momentum;
    redde i;
}

interior CursusFinis
_finem_fictum (
    constans character* id,
                   s64  momentum,
                   s32  exitus,
               Piscina* piscina)
{
    CursusFinis f;

    memset(&f, ZEPHYRUM, magnitudo(f));
    f.id                 = chorda_ex_literis(id, piscina);
    f.momentum           = momentum;
    f.duratio_ms         = (s64)MCCXXXIV;
    f.exitus             = exitus;
    f.rss_maxima         = (s64)4096 * (s64)M;
    f.cpu_usoris_ms      = (s64)DCCC;
    f.cpu_systematis_ms  = (s64)CC;
    f.acta               = chorda_ex_literis("ab12cd", piscina);
    f.acta_mensura       = (s64)MMXLVIII;
    redde f;
}

/* I. initium: lineae circuitus, omnes campi; argv cum TAB, linea nova,
 * backslash effugitur et redditur */
interior vacuum
_sectio_initium (
    CredoContextus* c)
{
     CursusInitium i = _initium_fictum("c1", "c0",
         "md/compile_probationes.sh",
         (s64)1791900000000LL, c->piscina);
     CursusInitium lectum;
       CursusFinis finis_nullus;
               b32 est_finis = VERUM;
            chorda linea;

    i.argv  = chorda_ex_literis("a\tb\nc\\d", c->piscina);
    linea   = mensura_cursus_initium_ad_lineam(&i, c->piscina);
    CREDO_NECESSE(linea.mensura > ZEPHYRUM);
    CREDO_CHORDA_INCIPIT(linea, chorda_ex_literis("cursus\tinitium\t",
        c->piscina));
    CREDO_NECESSE(mensura_cursus_ex_linea(linea, &lectum, &finis_nullus,
        &est_finis, c->piscina));
    CREDO_FALSUM(est_finis);
    CREDO_CHORDA_AEQUALIS(lectum.id, i.id);
    CREDO_CHORDA_AEQUALIS(lectum.parens, i.parens);
    CREDO_CHORDA_AEQUALIS(lectum.titulus, i.titulus);
    CREDO_CHORDA_AEQUALIS(lectum.argv, i.argv);
    CREDO_CHORDA_AEQUALIS(lectum.vocator, i.vocator);
    CREDO_CHORDA_AEQUALIS(lectum.commissum, i.commissum);
    CREDO_VERUM(lectum.sordidum);
    CREDO_CHORDA_AEQUALIS(lectum.ramus, i.ramus);
    CREDO_CHORDA_AEQUALIS(lectum.arbor, i.arbor);
    CREDO_CHORDA_AEQUALIS(lectum.machina, i.machina);
    CREDO_AEQUALIS_S64(lectum.initium, i.initium);
}

/* II. finis: lineae circuitus; exitus signi (CXXXVII = SIGKILL) */
interior vacuum
_sectio_finis (
    CredoContextus* c)
{
     CursusFinis f = _finem_fictum("c1", (s64)1791900001234LL,
         (s32)CXXXVII, c->piscina);
     CursusFinis lectum;
   CursusInitium initium_nullum;
             b32 est_finis = FALSUM;
          chorda linea;

    linea = mensura_cursus_finis_ad_lineam(&f, c->piscina);
    CREDO_NECESSE(linea.mensura > ZEPHYRUM);
    CREDO_CHORDA_INCIPIT(linea, chorda_ex_literis("cursus\tfinis\t",
        c->piscina));
    CREDO_NECESSE(mensura_cursus_ex_linea(linea, &initium_nullum,
        &lectum,
        &est_finis, c->piscina));
    CREDO_VERUM(est_finis);
    CREDO_CHORDA_AEQUALIS(lectum.id, f.id);
    CREDO_AEQUALIS_S64(lectum.momentum, f.momentum);
    CREDO_AEQUALIS_S64(lectum.duratio_ms, f.duratio_ms);
    CREDO_AEQUALIS_S32(lectum.exitus, f.exitus);
    CREDO_AEQUALIS_S64(lectum.rss_maxima, f.rss_maxima);
    CREDO_AEQUALIS_S64(lectum.cpu_usoris_ms, f.cpu_usoris_ms);
    CREDO_AEQUALIS_S64(lectum.cpu_systematis_ms, f.cpu_systematis_ms);
    CREDO_CHORDA_AEQUALIS(lectum.acta, f.acta);
    CREDO_AEQUALIS_S64(lectum.acta_mensura, f.acta_mensura);
}

/* VI. nota (T3): TAB, linea nova, backslash circuitu redduntur; nota
 * longa ad CURSUS_NOTA_MAXIMA praeciditur, numquam in medio characteris
 * UTF-8 (octetus continuationis 10xxxxxx non ultimus relinquitur) */
interior vacuum
_sectio_nota (
    CredoContextus* c)
{
       CursusFinis f = _finem_fictum("c1", (s64)1791900001234LL, (s32)I,
           c->piscina);
       CursusFinis lectum;
     CursusInitium initium_nullum;
               b32 est_finis = FALSUM;
            chorda linea;
         character longa[DCCC + I];
               i32 k;

    f.nota  = chorda_ex_literis("toml: 12/13\tFRACTA\nprobatio_x \\ y",
        c->piscina);
    linea   = mensura_cursus_finis_ad_lineam(&f, c->piscina);
    CREDO_NECESSE(mensura_cursus_ex_linea(linea, &initium_nullum,
        &lectum,
        &est_finis, c->piscina));
    CREDO_CHORDA_AEQUALIS(lectum.nota, f.nota);
    /* "bb" deinde 'a' 'é' (C3 A9) iterum: octetus CCLV = C3, CCLVI =
     * A9 - limes VERE in medio 'é' cadit (sine "bb" in initio 'a'
     * caderet et retractio numquam probaretur: planta N2 T3 superfuit) */
    longa[ZEPHYRUM]  = 'b';
    longa[I]         = 'b';
    per (k = II; k + II < (i32)DCCC; k += III)
    {
        longa[k]       = 'a';
        longa[k + I]   = (character)0xC3;
        longa[k + II]  = (character)0xA9;
    }
    longa[k]  = '\0';
    f.nota    = chorda_ex_literis(longa, c->piscina);
    linea     = mensura_cursus_finis_ad_lineam(&f, c->piscina);
    CREDO_NECESSE(mensura_cursus_ex_linea(linea, &initium_nullum,
        &lectum,
        &est_finis, c->piscina));
    CREDO_NECESSE(lectum.nota.mensura > ZEPHYRUM);
    CREDO_VERUM(lectum.nota.mensura <= (i32)CURSUS_NOTA_MAXIMA);
    CREDO_VERUM(lectum.nota.mensura > (i32)CURSUS_NOTA_MAXIMA - III);
    /* finis integer: 'a', aut 'é' totum (C3 A9) - numquam C3 solum */
    CREDO_VERUM(   lectum.nota.datum[lectum.nota.mensura - I] == 'a'
                || (   (insignatus character)lectum.nota.datum[
                           lectum.nota.mensura - I] == 0xA9
                    && (insignatus character)lectum.nota.datum[
                           lectum.nota.mensura - II] == 0xC3));
}

/* III. campi impuri (TAB in titulo aut arbore) recusantur - linea
 * vacua, numquam forma fracta; lineae malae non leguntur */
interior vacuum
_sectio_recusatio (
    CredoContextus* c)
{
     CursusInitium i = _initium_fictum("c1", "", "a\tb", (s64)1,
         c->piscina);
     CursusInitium li;
       CursusFinis lf;
               b32 est_finis;

    CREDO_AEQUALIS_I32(mensura_cursus_initium_ad_lineam(&i,
        c->piscina).mensura, ZEPHYRUM);
    i        = _initium_fictum("c1", "", "t", (s64)1, c->piscina);
    i.arbor  = chorda_ex_literis("/via\ncum/linea", c->piscina);
    CREDO_AEQUALIS_I32(mensura_cursus_initium_ad_lineam(&i,
        c->piscina).mensura, ZEPHYRUM);
    CREDO_FALSUM(mensura_cursus_ex_linea(chorda_ex_literis(
        "cursus\tinitium\tc1\tparum", c->piscina), &li, &lf, &est_finis,
        c->piscina));
    CREDO_FALSUM(mensura_cursus_ex_linea(chorda_ex_literis(
        "cursus\tfinis\tc1\tnon_numerus\t1\t0\t0\t0\t0\t\t0",
        c->piscina),
        &li, &lf, &est_finis, c->piscina));
    CREDO_FALSUM(mensura_cursus_ex_linea(chorda_ex_literis(
        "cursus\tquid\tc1", c->piscina), &li, &lf, &est_finis,
        c->piscina));
}

/* diarium in area sectionis */
interior constans character*
_diarium (
    CredoContextus* c)
{
    character* via = (character*)piscina_allocare(c->piscina,
        (memoriae_index)CD);

    sprintf(via, "%s/diarium.tsv", credo_area(c));
    redde via;
}

/* IV. diarium mixtum (mensura + cursus) conditur: omnia conduntur,
 * cursus genere 'cursus', diarium truncatur */
interior vacuum
_sectio_conditio (
    CredoContextus* c)
{
     constans character* diarium = _diarium(c);
          CursusInitium  i = _initium_fictum("c1", "", "frigida",
              (s64)1000, c->piscina);
            CursusFinis f = _finem_fictum("c1", (s64)2000, ZEPHYRUM,
                c->piscina);
                Mensura  m;
                Volumen* vol;
         MensuraFructus  fructus;
                    Xar* acta;
                    i32  cursus_n = ZEPHYRUM;
                    i32  k;

    memset(&m, ZEPHYRUM, magnitudo(m));
    m.sessio    = chorda_ex_literis("0199aabbccdd", c->piscina);
    m.id        = chorda_ex_literis("m1", c->piscina);
    m.parens    = chorda_ex_literis("c1", c->piscina);
    m.titulus   = chorda_ex_literis("compilatio", c->piscina);
    m.valor     = 1.5;
    m.unitas    = chorda_ex_literis("secunda", c->piscina);
    m.momentum  = (s64)1500;
    CREDO_NECESSE(mensura_cursum_incipere(diarium, &i, c->piscina));
    CREDO_NECESSE(mensura_annotare(diarium, &m, c->piscina));
    CREDO_NECESSE(mensura_cursum_finire(diarium, &f, c->piscina));
    vol = volumen_temporarium(c->piscina, "probatio_mensura_cursus");
    CREDO_NECESSE_NON_NIHIL(vol);
    fructus = mensura_condere(diarium, vol, c->piscina);
    CREDO_VERUM(fructus.successus);
    CREDO_AEQUALIS_I32(fructus.conditae, III);
    acta = volumen_acta_legere(vol, (s64)0, c->piscina);
    per (k = ZEPHYRUM; acta != NIHIL && k < xar_numerus(acta); k++)
    {
        si (chorda_aequalis_literis(((VolumenActum*)xar_obtinere(acta,
                k))->genus, "cursus"))
        {
            cursus_n++;
        }
    }
    CREDO_AEQUALIS_I32(cursus_n, II);
    CREDO_AEQUALIS_I32(mensura_condere(diarium, vol,
        c->piscina).conditae,
        ZEPHYRUM);
    volumen_claudere(vol);
}

/* V. legere: initium et finis per id coniuncti, ordine initii; exitus 0
 * TRANSIIT, aliter FRACTUS; sine fine INTERRUPTUS (contextus tamen
 * servatus); finis sine initio praetermittitur; parens servatur */
interior vacuum
_sectio_legere (
    CredoContextus* c)
{
    constans character* diarium = _diarium(c);
          CursusInitium a = _initium_fictum("a", "", "radix", (s64)1000,
              c->piscina);
          CursusInitium b = _initium_fictum("b", "a", "probationes_md",
              (s64)1100, c->piscina);
          CursusInitium d = _initium_fictum("d", "", "uncus", (s64)1200,
              c->piscina);
            CursusFinis fb = _finem_fictum("b", (s64)1500, II,
                c->piscina);
            CursusFinis fa = _finem_fictum("a", (s64)1900, ZEPHYRUM,
                c->piscina);
            CursusFinis finis_orbus = _finem_fictum("z", (s64)1950,
                ZEPHYRUM,
                c->piscina);
                Volumen* vol;
                    Xar* cursus;
                 Cursus* r;

    CREDO_NECESSE(mensura_cursum_incipere(diarium, &a, c->piscina));
    CREDO_NECESSE(mensura_cursum_incipere(diarium, &b, c->piscina));
    CREDO_NECESSE(mensura_cursum_incipere(diarium, &d, c->piscina));
    CREDO_NECESSE(mensura_cursum_finire(diarium, &fb, c->piscina));
    CREDO_NECESSE(mensura_cursum_finire(diarium, &fa, c->piscina));
    CREDO_NECESSE(mensura_cursum_finire(diarium, &finis_orbus,
        c->piscina));
    vol = volumen_temporarium(c->piscina, "probatio_mensura_cursus_l");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(mensura_condere(diarium, vol, c->piscina).successus);
    cursus = mensura_cursus_legere(vol, c->piscina);
    CREDO_NECESSE_NON_NIHIL(cursus);
    CREDO_AEQUALIS_I32(xar_numerus(cursus), III);
    r = (Cursus*)xar_obtinere(cursus, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->initium.id, "a");
    CREDO_AEQUALIS_I32((i32)r->status, (i32)CURSUS_TRANSIIT);
    CREDO_AEQUALIS_S64(r->finis.momentum, (s64)1900);
    r = (Cursus*)xar_obtinere(cursus, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->initium.id, "b");
    CREDO_CHORDA_AEQUALIS_LITERIS(r->initium.parens, "a");
    CREDO_AEQUALIS_I32((i32)r->status, (i32)CURSUS_FRACTUS);
    CREDO_AEQUALIS_S32(r->finis.exitus, II);
    r = (Cursus*)xar_obtinere(cursus, II);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->initium.id, "d");
    CREDO_AEQUALIS_I32((i32)r->status, (i32)CURSUS_INTERRUPTUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->initium.titulus, "uncus");
    volumen_claudere(vol);
}

interior Mensura
_mensuram_fictam (
    constans character* id,
    constans character* parens,
    constans character* titulus,
                duplex  valor,
    constans character* unitas,
               Piscina* piscina)
{
    Mensura m;

    memset(&m, ZEPHYRUM, magnitudo(m));
    m.sessio    = chorda_ex_literis("0199aabbccdd", piscina);
    m.id        = chorda_ex_literis(id, piscina);
    m.parens    = chorda_ex_literis(parens, piscina);
    m.titulus   = chorda_ex_literis(titulus, piscina);
    m.valor     = valor;
    m.unitas    = chorda_ex_literis(unitas, piscina);
    m.momentum  = (s64)1500;
    redde m;
}

/* VII. facta cursuum (colloquia T8): mensurae ordinariae parenti
 * adhaerentes; mensura_mensuras_legere per praefixum tituli, ordine
 * actorum; cursus ipsi non inter mensuras */
interior vacuum
_sectio_mensurae (
    CredoContextus* c)
{
     constans character* diarium = _diarium(c);
          CursusInitium  i = _initium_fictum("c1", "", "porta:radix",
              (s64)1000, c->piscina);
                Mensura custos = _mensuram_fictam("m1", "c1",
                    MENSURA_CUSTOS, (duplex)MMMDCCCXCVIII, "pid",
                    c->piscina);
                Mensura alia = _mensuram_fictam("m2", "c1",
                    "compilatio",
                    1.5, "secunda", c->piscina);
                Mensura ambitus = _mensuram_fictam("m3", "c1",
                    MENSURA_AMBITUS, (duplex)CCCLI, "membra",
                    c->piscina);
                   Volumen* vol;
                       Xar* lectae;
          constans Mensura* m;

    CREDO_NECESSE(mensura_cursum_incipere(diarium, &i, c->piscina));
    CREDO_NECESSE(mensura_annotare(diarium, &custos, c->piscina));
    CREDO_NECESSE(mensura_annotare(diarium, &alia, c->piscina));
    CREDO_NECESSE(mensura_annotare(diarium, &ambitus, c->piscina));
    vol = volumen_temporarium(c->piscina, "probatio_mensura_cursus_m");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(mensura_condere(diarium, vol, c->piscina).successus);

    lectae = mensura_mensuras_legere(vol, chorda_ex_literis("cursus.",
        c->piscina), c->piscina);
    CREDO_NECESSE_NON_NIHIL(lectae);
    CREDO_AEQUALIS_I32(xar_numerus(lectae), I);
    m = (constans Mensura*)xar_obtinere(lectae, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(m->titulus, MENSURA_CUSTOS);
    CREDO_CHORDA_AEQUALIS_LITERIS(m->parens, "c1");
    CREDO_CHORDA_AEQUALIS_LITERIS(m->unitas, "pid");
    CREDO_VERUM(m->valor == (duplex)MMMDCCCXCVIII);

    lectae = mensura_mensuras_legere(vol, chorda_ex_literis("fabrica.",
        c->piscina), c->piscina);
    CREDO_NECESSE_NON_NIHIL(lectae);
    CREDO_AEQUALIS_I32(xar_numerus(lectae), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans Mensura*)xar_obtinere(
        lectae, ZEPHYRUM))->id, "m3");

    /* praefixum vacuum = omnes, ordine actorum; cursus non numeratus */
    lectae = mensura_mensuras_legere(vol, chorda_ex_literis("",
        c->piscina), c->piscina);
    CREDO_NECESSE_NON_NIHIL(lectae);
    CREDO_AEQUALIS_I32(xar_numerus(lectae), III);
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans Mensura*)xar_obtinere(
        lectae, ZEPHYRUM))->id, "m1");
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans Mensura*)xar_obtinere(
        lectae, I))->id, "m2");
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans Mensura*)xar_obtinere(
        lectae, II))->id, "m3");

    /* praefixum nulli congruens: Xar vacuum, non NIHIL */
    lectae = mensura_mensuras_legere(vol, chorda_ex_literis("nihil.",
        c->piscina), c->piscina);
    CREDO_NECESSE_NON_NIHIL(lectae);
    CREDO_AEQUALIS_I32(xar_numerus(lectae), ZEPHYRUM);
    CREDO_VERUM(mensura_mensuras_legere(NIHIL, chorda_ex_literis("",
        c->piscina), c->piscina) == NIHIL);
    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "initium: lineae circuitus", _sectio_initium, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "finis: lineae circuitus", _sectio_finis, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "campi impuri et lineae malae", _sectio_recusatio, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "conditio diarii mixti", _sectio_conditio, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "legere: coniunctio per id", _sectio_legere, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "finis: nota effugitur et praeciditur", _sectio_nota, NIHIL,
        NIHIL,
      NIHIL, NIHIL },
    { "mensurae: facta cursuum", _sectio_mensurae, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("mensura_cursus", SECTIONES);
}
