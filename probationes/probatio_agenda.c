/* probatio_agenda.c - agenda (agenda-plan A1a): res, dies, rubricae,
 * notae, perfectio, deletio; revocare/reficere; aperire ex volumine
 * cum checkpointis (intervallum III), verificare. Textus notae per
 * DUOS circuitus (scribere -> legere) idem. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "volumen.h"
#include "fasti.h"
#include "agenda.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* mundus: volumen temporarium et documentum in eo */
nomen structura {
                Volumen* volumen;
    InternamentumChorda* intern;
       AgendaDocumentum* doc;
} Mundus;

/* parare: mundus in c->datum (volumen clauditur in purgare, etiam
 * post NECESSE - volumina temporaria numquam relinquuntur) */
interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(m, ZEPHYRUM, magnitudo(Mundus));
    m->intern   = internamentum_creare(c->piscina);
    m->volumen  = volumen_temporarium(c->piscina, "probatio_agenda");
    si (m->volumen)
    {
        m->doc = agenda_aperire(c->piscina, m->intern, m->volumen, "",
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

interior b32
dies_aequales (
    Dies a,
    Dies b)
{
    redde a.annus == b.annus && a.mensis == b.mensis
        && a.dies == b.dies;
}

/* rubricae rei = "a b ..." ? */
interior b32
rubricae_sunt (
    constans AgendaRes* r,
    constans character* exspectatae)
{
    character  buffer[CCLVI];
          i32  i;
          i32  k;
       chorda* c;

    k                 = ZEPHYRUM;
    buffer[ZEPHYRUM]  = '\0';
    per (i = ZEPHYRUM; r && i < xar_numerus(r->rubricae); i++)
    {
        c = (chorda*)xar_obtinere(r->rubricae, i);
        si (i > ZEPHYRUM)
        {
            buffer[k++] = ' ';
        }
        memcpy(buffer + k, c->datum, (size_t)c->mensura);
        k          += c->mensura;
        buffer[k]  = '\0';
    }
    redde r && strcmp(buffer, exspectatae) == ZEPHYRUM;
}

/* ids max + 1; titulus vacuus recusatur */
interior vacuum
_probare_creationem (
    CredoContextus* c)
{
                Mundus* m;
    constans AgendaRes* r;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    CREDO_AEQUALIS_I32(agenda_numerus_rerum(m->doc), ZEPHYRUM);
    CREDO_AEQUALIS_S32(agenda_rem_creare(m->doc, lit("panem emere",
        c->piscina)), I);
    CREDO_AEQUALIS_S32(agenda_rem_creare(m->doc,
        lit("epistulam scribere",
        c->piscina)), II);
    CREDO_AEQUALIS_S32(agenda_rem_creare(m->doc, lit("", c->piscina)),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32(agenda_numerus_rerum(m->doc), II);
    r = agenda_res(m->doc, II);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->titulus, "epistulam scribere");
    CREDO_FALSUM(r->dies_posita);
    CREDO_FALSUM(r->perfecta);
    CREDO_NIHIL(agenda_res(m->doc, III));
    CREDO_VERUM(agenda_titulum_ponere(m->doc, I, lit("panem et vinum",
        c->piscina)) > ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_res(m->doc, I)->titulus,
        "panem et vinum");
    CREDO_AEQUALIS_S64(agenda_titulum_ponere(m->doc, I, lit("",
        c->piscina)), ZEPHYRUM);
    CREDO_AEQUALIS_S64(agenda_titulum_ponere(m->doc, IX, lit("x",
        c->piscina)), ZEPHYRUM);
}

/* dies: ponere, mutare, tollere; dies mala recusatur */
interior vacuum
_probare_dies (
    CredoContextus* c)
{
    Mundus* m;
      Dies  d;
       s32  id;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    id  = agenda_rem_creare(m->doc, lit("res", c->piscina));
    d   = fasti_dies(MMXXVI, X, XIV);
    CREDO_VERUM(agenda_diem_ponere(m->doc, id, &d) > ZEPHYRUM);
    CREDO_VERUM(agenda_res(m->doc, id)->dies_posita);
    CREDO_VERUM(dies_aequales(agenda_res(m->doc, id)->dies, d));
    d = fasti_dies(MMXXVI, X, XVI);
    CREDO_VERUM(agenda_diem_ponere(m->doc, id, &d) > ZEPHYRUM);
    CREDO_VERUM(dies_aequales(agenda_res(m->doc, id)->dies, d));
    CREDO_VERUM(agenda_diem_ponere(m->doc, id, NIHIL) > ZEPHYRUM);
    CREDO_FALSUM(agenda_res(m->doc, id)->dies_posita);
    d = fasti_dies(MMXXVI, XIII, I);
    CREDO_AEQUALIS_S64(agenda_diem_ponere(m->doc, id, &d), ZEPHYRUM);
}

/* rubricae: copia ordinata sine iteratis; nomen malum recusatur */
interior vacuum
_probare_rubricas (
    CredoContextus* c)
{
    Mundus* m;
       s32  id;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    id = agenda_rem_creare(m->doc, lit("res", c->piscina));
    CREDO_VERUM(agenda_rubricas_ponere(m->doc, id,
        lit("opus domus opus",
        c->piscina)) > ZEPHYRUM);
    CREDO_VERUM(rubricae_sunt(agenda_res(m->doc, id), "domus opus"));
    CREDO_VERUM(agenda_rubricas_ponere(m->doc, id, lit(
        "house-projects  domus", c->piscina)) > ZEPHYRUM);
    CREDO_VERUM(rubricae_sunt(agenda_res(m->doc, id),
        "domus house-projects"));
    CREDO_AEQUALIS_S64(agenda_rubricas_ponere(m->doc, id, lit("Domus",
        c->piscina)), ZEPHYRUM);
    CREDO_AEQUALIS_S64(agenda_rubricas_ponere(m->doc, id, lit("a_b",
        c->piscina)), ZEPHYRUM);
    CREDO_VERUM(rubricae_sunt(agenda_res(m->doc, id),
        "domus house-projects"));
    /* copia vacua licet */
    CREDO_VERUM(agenda_rubricas_ponere(m->doc, id, lit("", c->piscina))
        > ZEPHYRUM);
    CREDO_VERUM(rubricae_sunt(agenda_res(m->doc, id), ""));
}

#define TEXTUS_ARDUUS "<a & \"b\">\n  'c' &amp; ]]>"
#define TITULUS_ARDUUS "emere \"bonum\" <cafeum> & lac"

/* notae: ordo, dies, textus arduus per DUOS circuitus */
interior vacuum
_probare_notas (
    CredoContextus* c)
{
                Mundus* m;
      AgendaDocumentum* altera;
      AgendaDocumentum* tertia;
    constans AgendaRes* r;
            AgendaNota* n;
                   s32  id;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    id = agenda_rem_creare(m->doc, lit("res", c->piscina));
    CREDO_VERUM(agenda_notam_addere(m->doc, id, fasti_dies(MMXXVI, X,
        XII),
        lit("prima", c->piscina)) > ZEPHYRUM);
    CREDO_VERUM(agenda_notam_addere(m->doc, id, fasti_dies(MMXXVI, X,
        XIII), lit(TEXTUS_ARDUUS, c->piscina)) > ZEPHYRUM);
    CREDO_AEQUALIS_S64(agenda_notam_addere(m->doc, id,
        fasti_dies(MMXXVI,
        X, XIII), lit("", c->piscina)), ZEPHYRUM);
    r = agenda_res(m->doc, id);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_AEQUALIS_I32(xar_numerus(r->notae), II);
    n = (AgendaNota*)xar_obtinere(r->notae, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(n->textus, "prima");
    CREDO_VERUM(dies_aequales(n->dies, fasti_dies(MMXXVI, X, XII)));
    /* circuitus I et II: ex volumine bis */
    altera = agenda_aperire(c->piscina, m->intern, m->volumen, "", III);
    CREDO_NECESSE_NON_NIHIL(altera);
    n = (AgendaNota*)xar_obtinere(agenda_res(altera, id)->notae, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(n->textus, TEXTUS_ARDUUS);
    CREDO_VERUM(agenda_titulum_ponere(altera, id, lit(TITULUS_ARDUUS,
        c->piscina)) > ZEPHYRUM);
    tertia = agenda_aperire(c->piscina, m->intern, m->volumen, "", III);
    CREDO_NECESSE_NON_NIHIL(tertia);
    n = (AgendaNota*)xar_obtinere(agenda_res(tertia, id)->notae, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(n->textus, TEXTUS_ARDUUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_res(tertia, id)->titulus,
        TITULUS_ARDUUS);
    /* circuitus II pro titulo */
    altera = agenda_aperire(c->piscina, m->intern, m->volumen, "", III);
    CREDO_NECESSE_NON_NIHIL(altera);
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_res(altera, id)->titulus,
        TITULUS_ARDUUS);
}

/* perfectio: dies servatur; aperta tollit */
interior vacuum
_probare_perfectionem (
    CredoContextus* c)
{
    Mundus* m;
       s32  id;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    id = agenda_rem_creare(m->doc, lit("res", c->piscina));
    CREDO_VERUM(agenda_perficere(m->doc, id, fasti_dies(MMXXVI, X,
        XIII))
        > ZEPHYRUM);
    CREDO_VERUM(agenda_res(m->doc, id)->perfecta);
    CREDO_VERUM(dies_aequales(agenda_res(m->doc, id)->dies_perfectionis,
        fasti_dies(MMXXVI, X, XIII)));
    CREDO_VERUM(agenda_perfectionem_tollere(m->doc, id) > ZEPHYRUM);
    CREDO_FALSUM(agenda_res(m->doc, id)->perfecta);
    CREDO_AEQUALIS_S64(agenda_perficere(m->doc, VII, fasti_dies(MMXXVI,
        X,
        XIII)), ZEPHYRUM);
}

/* deletio: res abit, id numquam iteratur, acta in deleta recusantur */
interior vacuum
_probare_deletionem (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    CREDO_AEQUALIS_S32(agenda_rem_creare(m->doc, lit("a", c->piscina)),
        I);
    CREDO_AEQUALIS_S32(agenda_rem_creare(m->doc, lit("b", c->piscina)),
        II);
    CREDO_VERUM(agenda_rem_delere(m->doc, II) > ZEPHYRUM);
    CREDO_NIHIL(agenda_res(m->doc, II));
    CREDO_AEQUALIS_I32(agenda_numerus_rerum(m->doc), I);
    CREDO_AEQUALIS_S64(agenda_titulum_ponere(m->doc, II, lit("x",
        c->piscina)), ZEPHYRUM);
    CREDO_AEQUALIS_S64(agenda_rem_delere(m->doc, II), ZEPHYRUM);
    CREDO_AEQUALIS_S32(agenda_rem_creare(m->doc, lit("c", c->piscina)),
        III);
    CREDO_AEQUALIS_S32(agenda_res_index(m->doc, I)->id, III);
}

/* revocare et reficere */
interior vacuum
_probare_historiam (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    CREDO_AEQUALIS_S32(agenda_rem_creare(m->doc, lit("vetus",
        c->piscina)), I);
    CREDO_VERUM(agenda_titulum_ponere(m->doc, I, lit("novus",
        c->piscina))
        > ZEPHYRUM);
    CREDO_VERUM(agenda_revocare(m->doc));
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_res(m->doc, I)->titulus,
        "vetus");
    CREDO_VERUM(agenda_reficere(m->doc));
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_res(m->doc, I)->titulus,
        "novus");
    CREDO_VERUM(agenda_revocare(m->doc));
    CREDO_VERUM(agenda_revocare(m->doc));
    CREDO_AEQUALIS_I32(agenda_numerus_rerum(m->doc), ZEPHYRUM);
    CREDO_FALSUM(agenda_revocare(m->doc));
    /* actum novum post revocationem: id rursus I (res prior mortua) */
    CREDO_AEQUALIS_S32(agenda_rem_creare(m->doc, lit("alia",
        c->piscina)),
        I);
}

/* aperire: status idem ex volumine (checkpoints, intervallum III);
 * verificare */
interior vacuum
_probare_aperitionem (
    CredoContextus* c)
{
              Mundus* m;
    AgendaDocumentum* altera;
                 s32  i;
                 s32  id;
                Dies  d;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    per (i = ZEPHYRUM; i < X; i++)
    {
        id  = agenda_rem_creare(m->doc, lit("res", c->piscina));
        d   = fasti_dies(MMXXVI, X, (s32)(I + i));
        (vacuum)agenda_diem_ponere(m->doc, id, &d);
        si (i % II == ZEPHYRUM)
        {
            (vacuum)agenda_rubricas_ponere(m->doc, id, lit("opus",
                c->piscina));
        }
    }
    (vacuum)agenda_rem_delere(m->doc, IV);
    (vacuum)agenda_perficere(m->doc, V, fasti_dies(MMXXVI, X, XIII));
    CREDO_VERUM(agenda_verificare(m->doc));
    altera = agenda_aperire(c->piscina, m->intern, m->volumen, "", III);
    CREDO_NECESSE_NON_NIHIL(altera);
    CREDO_CHORDA_AEQUALIS(agenda_sigillum_hex(altera, c->piscina),
        agenda_sigillum_hex(m->doc, c->piscina));
    CREDO_AEQUALIS_I32(agenda_numerus_rerum(altera), IX);
    CREDO_VERUM(agenda_res(altera, V)->perfecta);
    CREDO_VERUM(rubricae_sunt(agenda_res(altera, III), "opus"));
    CREDO_VERUM(agenda_verificare(altera));
    /* id post aperitionem: max + 1 deletis numeratis */
    CREDO_AEQUALIS_S32(agenda_rem_creare(altera, lit("ultima",
        c->piscina)), XI);
}

/* ids rerum in Xar (constans AgendaRes*) = "1 2 ..." ? */
interior b32
ids_sunt (
                   Xar* l,
    constans character* exspectati)
{
    character buffer[CXXVIII];
          i32 i;
          i32 k;

    si (!l)
    {
        redde FALSUM;
    }
    k                 = ZEPHYRUM;
    buffer[ZEPHYRUM]  = '\0';
    per (i = ZEPHYRUM; i < xar_numerus(l); i++)
    {
        k += (i32)sprintf(buffer + k, i > ZEPHYRUM ? " %d" : "%d",
            (integer)(*(constans AgendaRes**)xar_obtinere(l, i))->id);
    }
    redde strcmp(buffer, exspectati) == ZEPHYRUM;
}

interior b32
signum_est (
     constans AgendaSignum* s,
                       i32  assignatae,
                       i32  apertae,
                       i32  perfectae)
{
    redde s->assignatae == assignatae && s->apertae == apertae
        && s->perfectae == perfectae;
}

/* res diei, perfectae die (etiam alibi assignatae), signa mensis;
 * deletae absunt; dies ultra mensem nulli; bissextilis */
interior vacuum
_probare_quaestiones (
    CredoContextus* c)
{
           Mundus* m;
     AgendaSignum  signa[XXXI];
             Dies  d;
             Dies  hodie;
              s32  id;
              i32  i;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    hodie = fasti_dies(MMXXVI, X, XIV);
    /* 1: hodie; 2: hodie, perfecta hodie; 3: die XX, perfecta hodie;
     * 4: hodie, deleta; 5: sine die, perfecta XV; 6: mense alio */
    per (i = I; i <= VI; i++)
    {
        id = agenda_rem_creare(m->doc, lit("res", c->piscina));
        CREDO_NECESSE(id == (s32)i);
    }
    (vacuum)agenda_diem_ponere(m->doc, I, &hodie);
    (vacuum)agenda_diem_ponere(m->doc, II, &hodie);
    (vacuum)agenda_perficere(m->doc, II, hodie);
    d = fasti_dies(MMXXVI, X, XX);
    (vacuum)agenda_diem_ponere(m->doc, III, &d);
    (vacuum)agenda_perficere(m->doc, III, hodie);
    (vacuum)agenda_diem_ponere(m->doc, IV, &hodie);
    (vacuum)agenda_rem_delere(m->doc, IV);
    (vacuum)agenda_perficere(m->doc, V, fasti_dies(MMXXVI, X, XV));
    d = fasti_dies(MMXXVI, XI, I);
    (vacuum)agenda_diem_ponere(m->doc, VI, &d);

    CREDO_VERUM(ids_sunt(agenda_res_diei(m->doc, hodie, c->piscina),
        "1 2"));
    CREDO_VERUM(ids_sunt(agenda_perfectae_die(m->doc, hodie,
        c->piscina),
        "2 3"));
    CREDO_VERUM(ids_sunt(agenda_perfectae_die(m->doc, fasti_dies(MMXXVI,
        X, XV), c->piscina), "5"));
    CREDO_VERUM(ids_sunt(agenda_res_diei(m->doc, fasti_dies(MMXXVI, X,
        I), c->piscina), ""));

    memset(signa, VII, magnitudo(signa));
    CREDO_VERUM(agenda_signa_mensis(m->doc, MMXXVI, X, signa));
    CREDO_VERUM(signum_est(&signa[XIII], II, I, II));
    CREDO_VERUM(signum_est(&signa[XIX], I, ZEPHYRUM, ZEPHYRUM));
    CREDO_VERUM(signum_est(&signa[XIV], ZEPHYRUM, ZEPHYRUM, I));
    CREDO_VERUM(signum_est(&signa[ZEPHYRUM], ZEPHYRUM, ZEPHYRUM,
        ZEPHYRUM));
    CREDO_VERUM(signum_est(&signa[XXX], ZEPHYRUM, ZEPHYRUM, ZEPHYRUM));
    CREDO_VERUM(agenda_signa_mensis(m->doc, MMXXVI, XI, signa));
    CREDO_VERUM(signum_est(&signa[ZEPHYRUM], I, I, ZEPHYRUM));

    /* bissextilis: XXIX dies, ceteri nulli */
    d = fasti_dies(MMXXVIII, II, XXIX);
    (vacuum)agenda_diem_ponere(m->doc, I, &d);
    memset(signa, VII, magnitudo(signa));
    CREDO_VERUM(agenda_signa_mensis(m->doc, MMXXVIII, II, signa));
    CREDO_VERUM(signum_est(&signa[XXVIII], I, I, ZEPHYRUM));
    CREDO_VERUM(signum_est(&signa[XXIX], ZEPHYRUM, ZEPHYRUM, ZEPHYRUM));
    CREDO_VERUM(signum_est(&signa[XXX], ZEPHYRUM, ZEPHYRUM, ZEPHYRUM));
    CREDO_FALSUM(agenda_signa_mensis(m->doc, MMXXVI, XIII, signa));
    CREDO_FALSUM(agenda_signa_mensis(m->doc, MMXXVI, ZEPHYRUM, signa));
}

/* A4: apertae (sine perfectis et deletis, sine die incluse) et
 * rubricae distinctae ordine canonico */
interior vacuum
_probare_apertas (
    CredoContextus* c)
{
       Mundus* m;
          Xar* l;
         Dies  d;
          i32  i;
    character  visae[LXIV];
          i32  k;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    /* 1 domus (XIV); 2 perfecta; 3 sine die "opus domus"; 4 XX sine
     * rubrica; 5 deleta "zeta" */
    per (i = ZEPHYRUM; i < V; i++)
    {
        (vacuum)agenda_rem_creare(m->doc, lit("res", c->piscina));
    }
    d = fasti_dies(MMXXVI, X, XIV);
    (vacuum)agenda_diem_ponere(m->doc, I, &d);
    (vacuum)agenda_rubricas_ponere(m->doc, I, lit("domus", c->piscina));
    (vacuum)agenda_perficere(m->doc, II, d);
    (vacuum)agenda_rubricas_ponere(m->doc, III, lit("opus domus",
        c->piscina));
    d = fasti_dies(MMXXVI, X, XX);
    (vacuum)agenda_diem_ponere(m->doc, IV, &d);
    (vacuum)agenda_rubricas_ponere(m->doc, V, lit("zeta", c->piscina));
    (vacuum)agenda_rem_delere(m->doc, V);
    l = agenda_apertae(m->doc, c->piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), III);
    CREDO_AEQUALIS_S32((*(constans AgendaRes**)xar_obtinere(l,
        ZEPHYRUM))
        ->id, I);
    CREDO_AEQUALIS_S32((*(constans AgendaRes**)xar_obtinere(l, I))->id,
        III);
    CREDO_AEQUALIS_S32((*(constans AgendaRes**)xar_obtinere(l, II))->id,
        IV);
    l = agenda_rubricae(m->doc, c->piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    k                = ZEPHYRUM;
    visae[ZEPHYRUM]  = '\0';
    per (i = ZEPHYRUM; i < xar_numerus(l); i++)
    {
        chorda* r;

        r = (chorda*)xar_obtinere(l, i);
        si (i > ZEPHYRUM)
        {
            visae[k++] = ' ';
        }
        memcpy(visae + k, r->datum, (size_t)r->mensura);
        k         += r->mensura;
        visae[k]  = '\0';
    }
    /* deleta (zeta) abest; domus semel */
    CREDO_VERUM(strcmp(visae, "domus opus") == ZEPHYRUM);
}

/* agenda_perfectae (A6a): perfectae vivae, perfectione novissima
 * primum, deinde id descendens; deletae et reapertae absunt */
interior vacuum
_probare_perfectas (
    CredoContextus* c)
{
     Mundus* m;
        Xar* l;
       Dies  d;
        i32  i;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    /* 1 perfecta XII; 2 perfecta XIV; 3 perfecta XIV; 4 aperta; 5
     * perfecta XIII deinde reaperta; 6 perfecta XIV deinde deleta */
    per (i = ZEPHYRUM; i < VI; i++)
    {
        (vacuum)agenda_rem_creare(m->doc, lit("res", c->piscina));
    }
    (vacuum)agenda_perficere(m->doc, I, fasti_dies(MMXXVI, X, XII));
    d = fasti_dies(MMXXVI, X, XIV);
    (vacuum)agenda_perficere(m->doc, II, d);
    (vacuum)agenda_perficere(m->doc, III, d);
    (vacuum)agenda_perficere(m->doc, V, fasti_dies(MMXXVI, X, XIII));
    (vacuum)agenda_perfectionem_tollere(m->doc, V);
    (vacuum)agenda_perficere(m->doc, VI, d);
    (vacuum)agenda_rem_delere(m->doc, VI);
    l = agenda_perfectae(m->doc, c->piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), III);
    CREDO_AEQUALIS_S32((*(constans AgendaRes**)xar_obtinere(l,
        ZEPHYRUM))->id, III);
    CREDO_AEQUALIS_S32((*(constans AgendaRes**)xar_obtinere(l, I))->id,
        II);
    CREDO_AEQUALIS_S32((*(constans AgendaRes**)xar_obtinere(l, II))->id,
        I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "creatio",      _probare_creationem,     mundum_parare,
        mundum_purgare,
        NIHIL, NIHIL },
    { "dies",         _probare_dies,           mundum_parare,
        mundum_purgare,
        NIHIL, NIHIL },
    { "rubricae",     _probare_rubricas,       mundum_parare,
        mundum_purgare,
        NIHIL, NIHIL },
    { "notae",        _probare_notas,          mundum_parare,
        mundum_purgare,
        NIHIL, NIHIL },
    { "perfectio",    _probare_perfectionem,   mundum_parare,
        mundum_purgare,
        NIHIL, NIHIL },
    { "deletio",      _probare_deletionem,     mundum_parare,
        mundum_purgare,
        NIHIL, NIHIL },
    { "revocatio",    _probare_historiam,   mundum_parare,
        mundum_purgare,
        NIHIL, NIHIL },
    { "aperitio",     _probare_aperitionem,    mundum_parare,
        mundum_purgare,
        NIHIL, NIHIL },
    { "quaestiones",  _probare_quaestiones,    mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "perfectae",    _probare_perfectas,      mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "apertae",      _probare_apertas,        mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("agenda", SECTIONES);
}
