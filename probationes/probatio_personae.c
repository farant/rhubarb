/* probatio_personae.c - personae (nomenclator-plan P1b): creatio,
 * nomina et genus (nomen ostensum alterum si vacuum), clavis et
 * littera, rubricae, telephona et epistulae, natalis et aetas,
 * deletio, revocatio, aperitio ex volumine (BIS - scribere/legere
 * duo circuitus) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "volumen.h"
#include "fasti.h"
#include "personae.h"
#include "fasti_fontes.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

nomen structura {
                Volumen* volumen;
    InternamentumChorda* intern;
     PersonaeDocumentum* doc;
} Mundus;

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(m, ZEPHYRUM, magnitudo(Mundus));
    m->intern   = internamentum_creare(c->piscina);
    m->volumen  = volumen_temporarium(c->piscina, "probatio_personae");
    si (m->volumen)
    {
        m->doc = personae_aperire(c->piscina, m->intern, m->volumen, "",
            III);
    }
    c->datum = m;
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    si (m && m->volumen)
    {
        volumen_claudere(m->volumen);
    }
}

interior chorda
lit (
    constans character* s,
               Piscina* p)
{
    redde chorda_ex_literis(s, p);
}

/* Xar de chorda ex textibus (NIHIL finit) */
interior Xar*
index_ex (
               Piscina* p,
    constans character* a,
    constans character* b,
    constans character* d)
{
    Xar* l;

    l = xar_creare(p, (i32)magnitudo(chorda));
    si (a)
    {
        *(chorda*)xar_addere(l) = lit(a, p);
    }
    si (b)
    {
        *(chorda*)xar_addere(l) = lit(b, p);
    }
    si (d)
    {
        *(chorda*)xar_addere(l) = lit(d, p);
    }
    redde l;
}

/* index (Xar de chorda) = "a|b" ? */
interior b32
index_est (
          constans Xar* l,
    constans character* exspectatus)
{
    character  buffer[CCLVI];
          i32  i;
          i32  k;
       chorda* c;

    k                 = ZEPHYRUM;
    buffer[ZEPHYRUM]  = '\0';
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        c = (chorda*)xar_obtinere(l, i);
        si (i > ZEPHYRUM)
        {
            buffer[k++] = '|';
        }
        memcpy(buffer + k, c->datum, (size_t)c->mensura);
        k          += c->mensura;
        buffer[k]  = '\0';
    }
    si (strcmp(buffer, exspectatus) != ZEPHYRUM)
    {
        imprimere("  index: '%s'\n", buffer);
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
nomen_est (
      constans Persona* p,
    constans character* exspectatum,
               Piscina* piscina)
{
    chorda n;

    si (!p)
    {
        redde FALSUM;
    }
    n = persona_nomen(p, piscina);
    redde chorda_aequalis_literis(n, exspectatum);
}

interior b32
clavis_est (
                Mundus* m,
                   s32  id,
    constans character* exspectata,
               Piscina* piscina)
{
    constans Persona* p;

    p = personae_persona(m->doc, id);
    redde p && chorda_aequalis_literis(persona_clavis(p, piscina),
        exspectata);
}

/* creatio: ids, vacua recusata, campi */
interior vacuum
_probare_creationem (
    CredoContextus* c)
{
              Mundus* m;
    constans Persona* p;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    CREDO_AEQUALIS_S32(personae_hominem_creare(m->doc, lit("Anna",
        c->piscina), lit("Rossi", c->piscina)), I);
    CREDO_AEQUALIS_S32(personae_societatem_creare(m->doc, lit(
        "Rossi e Figli", c->piscina)), II);
    CREDO_AEQUALIS_S32(personae_hominem_creare(m->doc, lit("",
        c->piscina), lit("  ", c->piscina)), ZEPHYRUM);
    CREDO_AEQUALIS_S32(personae_societatem_creare(m->doc, lit("",
        c->piscina)), ZEPHYRUM);
    CREDO_AEQUALIS_S32(personae_hominem_creare(m->doc, lit("Cher",
        c->piscina), lit("", c->piscina)), III);
    CREDO_AEQUALIS_I32(personae_numerus(m->doc), III);
    p = personae_persona(m->doc, I);
    CREDO_NECESSE_NON_NIHIL(p);
    CREDO_VERUM(p->genus == PERSONA_HOMO);
    CREDO_CHORDA_AEQUALIS_LITERIS(p->praenomen, "Anna");
    CREDO_CHORDA_AEQUALIS_LITERIS(p->cognomen, "Rossi");
    CREDO_FALSUM(p->natalis_positus);
    CREDO_AEQUALIS_I32(xar_numerus(p->telephona), ZEPHYRUM);
    CREDO_VERUM(nomen_est(p, "Anna Rossi", c->piscina));
    CREDO_VERUM(personae_persona(m->doc, II)->genus
        == PERSONA_SOCIETAS);
    CREDO_VERUM(nomen_est(personae_persona(m->doc, II), "Rossi e Figli",
        c->piscina));
    CREDO_VERUM(nomen_est(personae_persona(m->doc, III), "Cher",
        c->piscina));
    CREDO_NIHIL(personae_persona(m->doc, IV));
    CREDO_NIHIL(personae_index(m->doc, III));
    CREDO_AEQUALIS_S32(personae_index(m->doc, I)->id, II);
}

/* nomina et genus: mutare; genus nomina non mutat, nomen alterum si
 * vacuum */
interior vacuum
_probare_nomina (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    CREDO_AEQUALIS_S32(personae_hominem_creare(m->doc, lit("Anna",
        c->piscina), lit("Rossi", c->piscina)), I);
    CREDO_VERUM(personae_nomen_ponere(m->doc, I, lit("Anna",
        c->piscina),
        lit("Rossi-Bianchi", c->piscina)) > ZEPHYRUM);
    CREDO_VERUM(nomen_est(personae_persona(m->doc, I),
        "Anna Rossi-Bianchi", c->piscina));
    CREDO_AEQUALIS_S64(personae_nomen_ponere(m->doc, I, lit("",
        c->piscina), lit("", c->piscina)), ZEPHYRUM);
    /* societas sine titulo: nomen hominis */
    CREDO_VERUM(personae_genus_ponere(m->doc, I, PERSONA_SOCIETAS)
        > ZEPHYRUM);
    CREDO_VERUM(personae_persona(m->doc, I)->genus == PERSONA_SOCIETAS);
    CREDO_VERUM(nomen_est(personae_persona(m->doc, I),
        "Anna Rossi-Bianchi", c->piscina));
    CREDO_VERUM(personae_titulum_ponere(m->doc, I, lit("Studio Rossi",
        c->piscina)) > ZEPHYRUM);
    CREDO_VERUM(nomen_est(personae_persona(m->doc, I), "Studio Rossi",
        c->piscina));
    /* homo iterum: nomina servata */
    CREDO_VERUM(personae_genus_ponere(m->doc, I, PERSONA_HOMO)
        > ZEPHYRUM);
    CREDO_VERUM(nomen_est(personae_persona(m->doc, I),
        "Anna Rossi-Bianchi", c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(personae_persona(m->doc, I)->titulus,
        "Studio Rossi");
    CREDO_AEQUALIS_S64(personae_titulum_ponere(m->doc, IX, lit("x",
        c->piscina)), ZEPHYRUM);
}

/* clavis, littera, ordo */
interior vacuum
_probare_ordinem (
    CredoContextus* c)
{
    Mundus* m;
       Xar* l;
       i32  i;
 character  ids[LXIV];
       i32  k;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    (vacuum)personae_hominem_creare(m->doc, lit("Marco", c->piscina),
        lit("Rossi", c->piscina));
    (vacuum)personae_hominem_creare(m->doc, lit("Anna", c->piscina),
        lit("Rossi", c->piscina));
    (vacuum)personae_societatem_creare(m->doc, lit("Banca d'Italia",
        c->piscina));
    (vacuum)personae_hominem_creare(m->doc, lit("Cher", c->piscina),
        lit("", c->piscina));
    (vacuum)personae_societatem_creare(m->doc, lit("3M", c->piscina));
    (vacuum)personae_hominem_creare(m->doc, lit("Luca", c->piscina),
        lit("bianchi", c->piscina));
    /* 7: clavis aequalis 2 - ordo id servatur (stabilis) */
    (vacuum)personae_hominem_creare(m->doc, lit("Anna", c->piscina),
        lit("Rossi", c->piscina));
    /* ids: 1 Marco Rossi, 2 Anna Rossi, 3 Banca, 4 Cher, 5 3M, 6 Luca
     * bianchi */
    CREDO_VERUM(clavis_est(m, II, "rossi anna", c->piscina));
    CREDO_VERUM(clavis_est(m, III, "banca d'italia", c->piscina));
    CREDO_VERUM(clavis_est(m, IV, "cher", c->piscina));
    CREDO_VERUM(persona_littera(personae_persona(m->doc, II)) == 'R');
    CREDO_VERUM(persona_littera(personae_persona(m->doc, VI)) == 'B');
    CREDO_VERUM(persona_littera(personae_persona(m->doc, V)) == '#');
    /* "3m" < "banca" < "bianchi luca" < "cher" < "rossi anna" (2, 7)
     * < "rossi marco" */
    l = personae_ordinatae(m->doc, c->piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    k              = ZEPHYRUM;
    ids[ZEPHYRUM]  = '\0';
    per (i = ZEPHYRUM; i < xar_numerus(l); i++)
    {
        k += (i32)sprintf(ids + k, i > ZEPHYRUM ? " %d" : "%d",
            (integer)(*(constans Persona**)xar_obtinere(l, i))->id);
    }
    CREDO_VERUM(strcmp(ids, "5 3 6 4 2 7 1") == ZEPHYRUM);
}

/* rubricae: copia ordinata, mala recusata, distinctae */
interior vacuum
_probare_rubricas (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    (vacuum)personae_hominem_creare(m->doc, lit("Anna", c->piscina),
        lit("Rossi", c->piscina));
    (vacuum)personae_hominem_creare(m->doc, lit("Luca", c->piscina),
        lit("Bianchi", c->piscina));
    CREDO_VERUM(personae_rubricas_ponere(m->doc, I, lit("roma familia",
        c->piscina)) > ZEPHYRUM);
    CREDO_VERUM(index_est(personae_persona(m->doc, I)->rubricae,
        "familia|roma"));
    CREDO_AEQUALIS_S64(personae_rubricas_ponere(m->doc, I, lit("Roma",
        c->piscina)), ZEPHYRUM);
    CREDO_VERUM(personae_rubricas_ponere(m->doc, II, lit("opus roma",
        c->piscina)) > ZEPHYRUM);
    CREDO_VERUM(index_est(personae_rubricae(m->doc, c->piscina),
        "familia|opus|roma"));
}

/* telephona et epistulae: index totus, vacua omissa, NIHIL purgat */
interior vacuum
_probare_indices (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    (vacuum)personae_hominem_creare(m->doc, lit("Anna", c->piscina),
        lit("Rossi", c->piscina));
    CREDO_VERUM(personae_telephona_ponere(m->doc, I,
        index_ex(c->piscina,
        "+39 06 1234567", "", "333 1234567")) > ZEPHYRUM);
    CREDO_VERUM(index_est(personae_persona(m->doc, I)->telephona,
        "+39 06 1234567|333 1234567"));
    CREDO_VERUM(personae_epistulas_ponere(m->doc, I,
        index_ex(c->piscina,
        "anna@example.org", NIHIL, NIHIL)) > ZEPHYRUM);
    CREDO_VERUM(index_est(personae_persona(m->doc, I)->epistulae,
        "anna@example.org"));
    CREDO_VERUM(personae_telephona_ponere(m->doc, I, NIHIL) > ZEPHYRUM);
    CREDO_VERUM(index_est(personae_persona(m->doc, I)->telephona, ""));
    CREDO_VERUM(index_est(personae_persona(m->doc, I)->epistulae,
        "anna@example.org"));
}

/* natalis: cum anno, sine anno (29 II licet), mali recusati; aetas */
interior vacuum
_probare_natalem (
    CredoContextus* c)
{
              Mundus* m;
    constans Persona* p;
                Dies  d;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    (vacuum)personae_hominem_creare(m->doc, lit("Anna", c->piscina),
        lit("Rossi", c->piscina));
    d = fasti_dies(MCMLXXXVI, III, II);
    CREDO_VERUM(personae_natalem_ponere(m->doc, I, &d) > ZEPHYRUM);
    p = personae_persona(m->doc, I);
    CREDO_VERUM(p->natalis_positus && p->natalis.annus == MCMLXXXVI);
    CREDO_AEQUALIS_S32(persona_aetas(p, fasti_dies(MMXXVI, III, I)),
        XXXIX);
    CREDO_AEQUALIS_S32(persona_aetas(p, fasti_dies(MMXXVI, III, II)),
        XL);
    CREDO_AEQUALIS_S32(persona_aetas(p, fasti_dies(MCMLXXX, I, I)), -I);
    /* sine anno: 29 II licet; aetas nulla */
    d.annus   = ZEPHYRUM;
    d.mensis  = II;
    d.dies    = XXIX;
    CREDO_VERUM(personae_natalem_ponere(m->doc, I, &d) > ZEPHYRUM);
    p = personae_persona(m->doc, I);
    CREDO_VERUM(p->natalis_positus && p->natalis.annus == ZEPHYRUM
        && p->natalis.mensis == II && p->natalis.dies == XXIX);
    CREDO_AEQUALIS_S32(persona_aetas(p, fasti_dies(MMXXVI, III, I)),
        -I);
    /* mali: mensis XIII, 30 II, 29 II anni non bissextilis */
    d.mensis = XIII;
    CREDO_AEQUALIS_S64(personae_natalem_ponere(m->doc, I, &d),
        ZEPHYRUM);
    d.mensis  = II;
    d.dies    = XXX;
    CREDO_AEQUALIS_S64(personae_natalem_ponere(m->doc, I, &d),
        ZEPHYRUM);
    d = fasti_dies(MMXXIII, II, XXIX);
    CREDO_AEQUALIS_S64(personae_natalem_ponere(m->doc, I, &d),
        ZEPHYRUM);
    /* NIHIL: nullus */
    CREDO_VERUM(personae_natalem_ponere(m->doc, I, NIHIL) > ZEPHYRUM);
    p = personae_persona(m->doc, I);
    CREDO_FALSUM(p->natalis_positus);
    CREDO_AEQUALIS_S32(persona_aetas(p, fasti_dies(MMXXVI, III, I)),
        -I);
}

/* deletio: absens, acta recusata, id non iteratur */
interior vacuum
_probare_deletionem (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    (vacuum)personae_hominem_creare(m->doc, lit("Anna", c->piscina),
        lit("Rossi", c->piscina));
    CREDO_VERUM(personae_delere(m->doc, I) > ZEPHYRUM);
    CREDO_NIHIL(personae_persona(m->doc, I));
    CREDO_AEQUALIS_I32(personae_numerus(m->doc), ZEPHYRUM);
    CREDO_AEQUALIS_S64(personae_rubricas_ponere(m->doc, I, lit("a",
        c->piscina)), ZEPHYRUM);
    CREDO_AEQUALIS_S64(personae_delere(m->doc, I), ZEPHYRUM);
    CREDO_AEQUALIS_S32(personae_societatem_creare(m->doc, lit("Nova",
        c->piscina)), II);
}

/* revocare et reficere */
interior vacuum
_probare_historiam (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    (vacuum)personae_hominem_creare(m->doc, lit("Anna", c->piscina),
        lit("Rossi", c->piscina));
    (vacuum)personae_nomen_ponere(m->doc, I, lit("Anna", c->piscina),
        lit("Bianchi", c->piscina));
    CREDO_VERUM(personae_revocare(m->doc));
    CREDO_VERUM(nomen_est(personae_persona(m->doc, I), "Anna Rossi",
        c->piscina));
    CREDO_VERUM(personae_reficere(m->doc));
    CREDO_VERUM(nomen_est(personae_persona(m->doc, I), "Anna Bianchi",
        c->piscina));
}

/* aperire bis ex volumine (checkpoints, intervallum III): omnia campi,
 * nomina cum '"' et '<', sigilla aequalia, verificare */
interior vacuum
_probare_aperitionem (
    CredoContextus* c)
{
                Mundus* m;
    PersonaeDocumentum* altera;
    PersonaeDocumentum* tertia;
      constans Persona* p;
                   s32  i;
                  Dies  d;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    per (i = ZEPHYRUM; i < VII; i++)
    {
        (vacuum)personae_hominem_creare(m->doc, lit("Anna", c->piscina),
            lit("Rossi", c->piscina));
    }
    (vacuum)personae_hominem_creare(m->doc, lit("Jo \"JJ\"",
        c->piscina),
        lit("O'Brien <x>", c->piscina));
    (vacuum)personae_societatem_creare(m->doc, lit("A & B \"srl\"",
        c->piscina));
    (vacuum)personae_rubricas_ponere(m->doc, VIII, lit("amici roma",
        c->piscina));
    (vacuum)personae_telephona_ponere(m->doc, VIII, index_ex(c->piscina,
        "+1 \"555\" 0100", "<int> 22", NIHIL));
    (vacuum)personae_epistulas_ponere(m->doc, VIII, index_ex(c->piscina,
        "jo@example.org", NIHIL, NIHIL));
    d.annus   = ZEPHYRUM;
    d.mensis  = XII;
    d.dies    = XXV;
    (vacuum)personae_natalem_ponere(m->doc, VIII, &d);
    (vacuum)personae_genus_ponere(m->doc, VIII, PERSONA_SOCIETAS);
    (vacuum)personae_delere(m->doc, III);
    CREDO_VERUM(personae_verificare(m->doc));
    altera = personae_aperire(c->piscina, m->intern, m->volumen, "",
        III);
    CREDO_NECESSE_NON_NIHIL(altera);
    CREDO_CHORDA_AEQUALIS(personae_sigillum_hex(altera, c->piscina),
        personae_sigillum_hex(m->doc, c->piscina));
    tertia = personae_aperire(c->piscina, m->intern, m->volumen, "",
        III);
    CREDO_NECESSE_NON_NIHIL(tertia);
    CREDO_CHORDA_AEQUALIS(personae_sigillum_hex(tertia, c->piscina),
        personae_sigillum_hex(m->doc, c->piscina));
    CREDO_AEQUALIS_I32(personae_numerus(tertia), VIII);
    p = personae_persona(tertia, VIII);
    CREDO_NECESSE_NON_NIHIL(p);
    CREDO_VERUM(p->genus == PERSONA_SOCIETAS);
    CREDO_CHORDA_AEQUALIS_LITERIS(p->praenomen, "Jo \"JJ\"");
    CREDO_CHORDA_AEQUALIS_LITERIS(p->cognomen, "O'Brien <x>");
    CREDO_VERUM(index_est(p->rubricae, "amici|roma"));
    CREDO_VERUM(index_est(p->telephona, "+1 \"555\" 0100|<int> 22"));
    CREDO_VERUM(index_est(p->epistulae, "jo@example.org"));
    CREDO_VERUM(p->natalis_positus && p->natalis.annus == ZEPHYRUM
        && p->natalis.mensis == XII && p->natalis.dies == XXV);
    CREDO_CHORDA_AEQUALIS_LITERIS(personae_persona(tertia, IX)->titulus,
        "A & B \"srl\"");
    CREDO_VERUM(personae_verificare(tertia));
    CREDO_AEQUALIS_S32(personae_hominem_creare(tertia, lit("Ultima",
        c->piscina), lit("", c->piscina)), X);
}

/* P6: introitus fontis dierum cum referentia id; NIHIL si abest */
interior constans FastiIntroitus*
introitus_personae (
    Xar* l,
    s32  id)
{
    i32 i;

    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        si (((constans FastiIntroitus*)xar_obtinere(l, i))->referentia
            == id)
        {
            redde (constans FastiIntroitus*)xar_obtinere(l, i);
        }
    }
    redde NIHIL;
}

interior b32
introitus_est (
    constans FastiIntroitus* t,
                        s32  annus,
                        s32  mensis,
                        s32  dies,
         constans character* titulus,
                        s32  origo)
{
    si (   !t || t->dies.annus != annus || t->dies.mensis != mensis
        || t->dies.dies != dies || !chorda_aequalis_literis(t->genus,
            FASTI_GENUS_NATALIS) || !chorda_aequalis_literis(t->titulus,
            titulus) || t->annus_originis != origo)
    {
        si (t)
        {
            imprimere("  introitus %d: %d-%d-%d '%.*s' %d\n",
                (integer)t->referentia, (integer)t->dies.annus,
                (integer)t->dies.mensis, (integer)t->dies.dies,
                (integer)t->titulus.mensura,
                (constans character*)t->titulus.datum,
                (integer)t->annus_originis);
        }
        redde FALSUM;
    }
    redde VERUM;
}

/* P6: fons dierum - natales in intervallo (annus ignotus: origo 0;
 * societas sine natali; deleta et nondum nata absunt; 29 II in anno
 * communi 28 II) */
interior vacuum
_probare_fastos (
    CredoContextus* c)
{
    Mundus* m;
       Xar* l;
      Dies  d;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    (vacuum)personae_hominem_creare(m->doc, lit("Anna", c->piscina),
        lit("Rossi", c->piscina));
    (vacuum)personae_hominem_creare(m->doc, lit("Luca", c->piscina),
        lit("Bianchi", c->piscina));
    (vacuum)personae_societatem_creare(m->doc, lit("Banca",
        c->piscina));
    (vacuum)personae_hominem_creare(m->doc, lit("Bruna", c->piscina),
        lit("Verdi", c->piscina));
    (vacuum)personae_hominem_creare(m->doc, lit("Carlo", c->piscina),
        lit("Neri", c->piscina));
    (vacuum)personae_hominem_creare(m->doc, lit("Futurus", c->piscina),
        lit("Novus", c->piscina));
    d = fasti_dies(MCMLXXXVI, III, II);
    CREDO_NECESSE(personae_natalem_ponere(m->doc, I, &d) > ZEPHYRUM);
    d = fasti_dies(ZEPHYRUM, XII, XXV);
    CREDO_NECESSE(personae_natalem_ponere(m->doc, II, &d) > ZEPHYRUM);
    d = fasti_dies(MM, II, XXIX);
    CREDO_NECESSE(personae_natalem_ponere(m->doc, IV, &d) > ZEPHYRUM);
    d = fasti_dies(MCMXC, III, II);
    CREDO_NECESSE(personae_natalem_ponere(m->doc, V, &d) > ZEPHYRUM);
    CREDO_NECESSE(personae_delere(m->doc, V) > ZEPHYRUM);
    d = fasti_dies(MMXXVII, III, X);
    CREDO_NECESSE(personae_natalem_ponere(m->doc, VI, &d) > ZEPHYRUM);
    /* Martius MMXXVI: Anna sola */
    l = xar_creare(c->piscina, (i32)magnitudo(FastiIntroitus));
    CREDO_NECESSE(personae_fasti_legere(m->doc, fasti_dies(MMXXVI, III,
        I), fasti_dies(MMXXVI, III, XXXI), l, c->piscina));
    CREDO_AEQUALIS_I32(xar_numerus(l), I);
    CREDO_VERUM(introitus_est(introitus_personae(l, I), MMXXVI, III, II,
        "Anna Rossi", MCMLXXXVI));
    /* XII MMXXVI - III MMXXVII: Luca (sine anno), Bruna (28 II),
     * Anna, Futurus */
    l = xar_creare(c->piscina, (i32)magnitudo(FastiIntroitus));
    CREDO_NECESSE(personae_fasti_legere(m->doc, fasti_dies(MMXXVI, XII,
        I), fasti_dies(MMXXVII, III, XXXI), l, c->piscina));
    CREDO_AEQUALIS_I32(xar_numerus(l), IV);
    CREDO_VERUM(introitus_est(introitus_personae(l, II), MMXXVI, XII,
        XXV, "Luca Bianchi", ZEPHYRUM));
    CREDO_VERUM(introitus_est(introitus_personae(l, IV), MMXXVII, II,
        XXVIII, "Bruna Verdi", MM));
    CREDO_VERUM(introitus_est(introitus_personae(l, I), MMXXVII, III,
        II, "Anna Rossi", MCMLXXXVI));
    CREDO_VERUM(introitus_est(introitus_personae(l, VI), MMXXVII, III,
        X, "Futurus Novus", MMXXVII));
    /* bissextilis: 29 II */
    l = xar_creare(c->piscina, (i32)magnitudo(FastiIntroitus));
    CREDO_NECESSE(personae_fasti_legere(m->doc, fasti_dies(MMXXVIII, II,
        I), fasti_dies(MMXXVIII, II, XXIX), l, c->piscina));
    CREDO_VERUM(introitus_est(introitus_personae(l, IV), MMXXVIII, II,
        XXIX, "Bruna Verdi", MM));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "creatio", _probare_creationem, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "nomina", _probare_nomina, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "ordo", _probare_ordinem, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "rubricae", _probare_rubricas, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "indices", _probare_indices, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "natalis", _probare_natalem, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "deletio", _probare_deletionem, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "revocatio", _probare_historiam, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "aperitio", _probare_aperitionem, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "fasti", _probare_fastos, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("personae", SECTIONES);
}
