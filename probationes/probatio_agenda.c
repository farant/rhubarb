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
                    i32  numerator;   /* moneta_numerator: "1", "2" */
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
        agenda_monetam_ponere(m->doc, moneta_numerator, &m->numerator);
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

/* D3: ULID = XXVI signa Crockford base32 (0-9 A-Z sine I L O U) */
interior b32
ulid_est (
    chorda id)
{
    i32 i;

    si (id.mensura != XXVI)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < id.mensura; i++)
    {
        si (   !strchr("0123456789ABCDEFGHJKMNPQRSTVWXYZ",
                (integer)id.datum[i])
            || id.datum[i] == '\0')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* id rei i in Xar (constans AgendaRes*) */
interior chorda
id_in (
    Xar* l,
    i32  i)
{
    redde (*(constans AgendaRes**)xar_obtinere(l, i))->id;
}

/* id numeratoris (moneta_numerator): n -> "n" */
interior chorda
id_ex (
         s32  n,
     Piscina* p)
{
    redde chorda_ex_s32(n, p);
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

/* ids numeratoris ("1", "2"); titulus vacuus recusatur */
interior vacuum
_probare_creationem (
    CredoContextus* c)
{
                Mundus* m;
    constans AgendaRes* r;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    CREDO_AEQUALIS_I32(agenda_numerus_rerum(m->doc), ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_rem_creare(m->doc,
        lit("panem emere",
        c->piscina)), "1");
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_rem_creare(m->doc,
        lit("epistulam scribere",
        c->piscina)), "2");
    CREDO_AEQUALIS_I32(agenda_rem_creare(m->doc, lit("",
        c->piscina)).mensura,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32(agenda_numerus_rerum(m->doc), II);
    r = agenda_res(m->doc, id_ex(II, c->piscina));
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->titulus, "epistulam scribere");
    CREDO_FALSUM(r->dies_posita);
    CREDO_FALSUM(r->perfecta);
    CREDO_NIHIL(agenda_res(m->doc, id_ex(III, c->piscina)));
    CREDO_VERUM(agenda_titulum_ponere(m->doc, id_ex(I, c->piscina),
        lit("panem et vinum",
        c->piscina)) > ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_res(m->doc, id_ex(I,
        c->piscina))->titulus,
        "panem et vinum");
    CREDO_AEQUALIS_S64(agenda_titulum_ponere(m->doc, id_ex(I,
        c->piscina), lit("",
        c->piscina)), ZEPHYRUM);
    CREDO_AEQUALIS_S64(agenda_titulum_ponere(m->doc, id_ex(IX,
        c->piscina), lit("x",
        c->piscina)), ZEPHYRUM);
}

/* dies: ponere, mutare, tollere; dies mala recusatur */
interior vacuum
_probare_dies (
    CredoContextus* c)
{
    Mundus* m;
      Dies  d;
    chorda  id;

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
    chorda  id;

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
                chorda  id;

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
    chorda  id;

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
    CREDO_AEQUALIS_S64(agenda_perficere(m->doc, id_ex(VII, c->piscina),
        fasti_dies(MMXXVI,
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
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_rem_creare(m->doc, lit("a",
        c->piscina)), "1");
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_rem_creare(m->doc, lit("b",
        c->piscina)), "2");
    CREDO_VERUM(agenda_rem_delere(m->doc, id_ex(II, c->piscina))
        > ZEPHYRUM);
    CREDO_NIHIL(agenda_res(m->doc, id_ex(II, c->piscina)));
    CREDO_AEQUALIS_I32(agenda_numerus_rerum(m->doc), I);
    CREDO_AEQUALIS_S64(agenda_titulum_ponere(m->doc, id_ex(II,
        c->piscina), lit("x",
        c->piscina)), ZEPHYRUM);
    CREDO_AEQUALIS_S64(agenda_rem_delere(m->doc, id_ex(II, c->piscina)),
        ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_rem_creare(m->doc, lit("c",
        c->piscina)), "3");
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_res_index(m->doc, I)->id, "3");
}

/* revocare et reficere */
interior vacuum
_probare_historiam (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_rem_creare(m->doc, lit("vetus",
        c->piscina)), "1");
    CREDO_VERUM(agenda_titulum_ponere(m->doc, id_ex(I, c->piscina),
        lit("novus",
        c->piscina))
        > ZEPHYRUM);
    CREDO_VERUM(agenda_revocare(m->doc));
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_res(m->doc, id_ex(I,
        c->piscina))->titulus,
        "vetus");
    CREDO_VERUM(agenda_reficere(m->doc));
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_res(m->doc, id_ex(I,
        c->piscina))->titulus,
        "novus");
    CREDO_VERUM(agenda_revocare(m->doc));
    CREDO_VERUM(agenda_revocare(m->doc));
    CREDO_AEQUALIS_I32(agenda_numerus_rerum(m->doc), ZEPHYRUM);
    CREDO_FALSUM(agenda_revocare(m->doc));
    /* actum novum post revocationem: fons pergit ("2"; res prior
     * mortua) */
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_rem_creare(m->doc, lit("alia",
        c->piscina)), "2");
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
              chorda  id;
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
    (vacuum)agenda_rem_delere(m->doc, id_ex(IV, c->piscina));
    (vacuum)agenda_perficere(m->doc, id_ex(V, c->piscina),
        fasti_dies(MMXXVI, X, XIII));
    CREDO_VERUM(agenda_verificare(m->doc));
    altera = agenda_aperire(c->piscina, m->intern, m->volumen, "", III);
    CREDO_NECESSE_NON_NIHIL(altera);
    CREDO_CHORDA_AEQUALIS(agenda_sigillum_hex(altera, c->piscina),
        agenda_sigillum_hex(m->doc, c->piscina));
    CREDO_AEQUALIS_I32(agenda_numerus_rerum(altera), IX);
    CREDO_VERUM(agenda_res(altera, id_ex(V, c->piscina))->perfecta);
    CREDO_VERUM(rubricae_sunt(agenda_res(altera, id_ex(III,
        c->piscina)), "opus"));
    CREDO_VERUM(agenda_verificare(altera));
    /* D3: documentum sine fonte posito ULID cudit (moneta_ulid) */
    CREDO_VERUM(ulid_est(agenda_rem_creare(altera, lit("ultima",
        c->piscina))));
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
        k += (i32)sprintf(buffer + k, i > ZEPHYRUM ? " %.*s" : "%.*s",
            (integer)id_in(l, i).mensura, (constans character*)id_in(l,
            i).datum);
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
           chorda  id;
              i32  i;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->doc);
    hodie = fasti_dies(MMXXVI, X, XIV);
    /* 1: hodie; 2: hodie, perfecta hodie; 3: die XX, perfecta hodie;
     * 4: hodie, deleta; 5: sine die, perfecta XV; 6: mense alio */
    per (i = I; i <= VI; i++)
    {
        id = agenda_rem_creare(m->doc, lit("res", c->piscina));
        CREDO_NECESSE(chorda_aequalis(id, id_ex((s32)i, c->piscina)));
    }
    (vacuum)agenda_diem_ponere(m->doc, id_ex(I, c->piscina), &hodie);
    (vacuum)agenda_diem_ponere(m->doc, id_ex(II, c->piscina), &hodie);
    (vacuum)agenda_perficere(m->doc, id_ex(II, c->piscina), hodie);
    d = fasti_dies(MMXXVI, X, XX);
    (vacuum)agenda_diem_ponere(m->doc, id_ex(III, c->piscina), &d);
    (vacuum)agenda_perficere(m->doc, id_ex(III, c->piscina), hodie);
    (vacuum)agenda_diem_ponere(m->doc, id_ex(IV, c->piscina), &hodie);
    (vacuum)agenda_rem_delere(m->doc, id_ex(IV, c->piscina));
    (vacuum)agenda_perficere(m->doc, id_ex(V, c->piscina),
        fasti_dies(MMXXVI, X, XV));
    d = fasti_dies(MMXXVI, XI, I);
    (vacuum)agenda_diem_ponere(m->doc, id_ex(VI, c->piscina), &d);

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
    (vacuum)agenda_diem_ponere(m->doc, id_ex(I, c->piscina), &d);
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
    (vacuum)agenda_diem_ponere(m->doc, id_ex(I, c->piscina), &d);
    (vacuum)agenda_rubricas_ponere(m->doc, id_ex(I, c->piscina),
        lit("domus", c->piscina));
    (vacuum)agenda_perficere(m->doc, id_ex(II, c->piscina), d);
    (vacuum)agenda_rubricas_ponere(m->doc, id_ex(III, c->piscina),
        lit("opus domus",
        c->piscina));
    d = fasti_dies(MMXXVI, X, XX);
    (vacuum)agenda_diem_ponere(m->doc, id_ex(IV, c->piscina), &d);
    (vacuum)agenda_rubricas_ponere(m->doc, id_ex(V, c->piscina),
        lit("zeta", c->piscina));
    (vacuum)agenda_rem_delere(m->doc, id_ex(V, c->piscina));
    l = agenda_apertae(m->doc, c->piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), III);
    CREDO_CHORDA_AEQUALIS_LITERIS(id_in(l, ZEPHYRUM), "1");
    CREDO_CHORDA_AEQUALIS_LITERIS(id_in(l, I), "3");
    CREDO_CHORDA_AEQUALIS_LITERIS(id_in(l, II), "4");
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
    (vacuum)agenda_perficere(m->doc, id_ex(I, c->piscina),
        fasti_dies(MMXXVI, X, XII));
    d = fasti_dies(MMXXVI, X, XIV);
    (vacuum)agenda_perficere(m->doc, id_ex(II, c->piscina), d);
    (vacuum)agenda_perficere(m->doc, id_ex(III, c->piscina), d);
    (vacuum)agenda_perficere(m->doc, id_ex(V, c->piscina),
        fasti_dies(MMXXVI, X, XIII));
    (vacuum)agenda_perfectionem_tollere(m->doc, id_ex(V, c->piscina));
    (vacuum)agenda_perficere(m->doc, id_ex(VI, c->piscina), d);
    (vacuum)agenda_rem_delere(m->doc, id_ex(VI, c->piscina));
    l = agenda_perfectae(m->doc, c->piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), III);
    CREDO_CHORDA_AEQUALIS_LITERIS(id_in(l, ZEPHYRUM), "3");
    CREDO_CHORDA_AEQUALIS_LITERIS(id_in(l, I), "2");
    CREDO_CHORDA_AEQUALIS_LITERIS(id_in(l, II), "1");
}

/* D3: actum crudum in volumine (forma veterum actorum) */
interior vacuum
actum_serere (
               Mundus* m,
    constans character* textus,
               Piscina* p)
{
    CREDO_NECESSE(volumen_actum_appendere(m->volumen, "agenda",
        lit(textus, p)) > ZEPHYRUM);
}

/* D3: fons qui id malum cudit */
interior vacuum
fons_malus (
       vacuum* ctx,
    character* effusio)
{
    (vacuum)ctx;
    strcpy(effusio, "a-b");
}

/* D3: sine fonte posito ids ULID (moneta_ulid), distincti, crescentes;
 * ex volumine iterum inveniuntur */
interior vacuum
_probare_identitates (
    CredoContextus* c)
{
               Mundus* m;
     AgendaDocumentum* nuda;
     AgendaDocumentum* altera;
               chorda  a;
               chorda  b;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->volumen);
    nuda = agenda_aperire(c->piscina, m->intern, m->volumen, "nuda",
        III);
    CREDO_NECESSE_NON_NIHIL(nuda);
    a = agenda_rem_creare(nuda, lit("prima", c->piscina));
    b = agenda_rem_creare(nuda, lit("secunda", c->piscina));
    CREDO_VERUM(ulid_est(a));
    CREDO_VERUM(ulid_est(b));
    CREDO_VERUM(chorda_comparare(a, b) < ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS(agenda_res_index(nuda, ZEPHYRUM)->id, a);
    CREDO_AEQUALIS_I32(agenda_res_index(nuda, I)->ordo, II);
    CREDO_VERUM(agenda_titulum_ponere(nuda, b, lit("altera",
        c->piscina)) > ZEPHYRUM);
    altera = agenda_aperire(c->piscina, m->intern, m->volumen, "nuda",
        III);
    CREDO_NECESSE_NON_NIHIL(altera);
    CREDO_NECESSE_NON_NIHIL(agenda_res(altera, a));
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_res(altera, b)->titulus,
        "altera");
}

/* D3: acta vetera (id decimale) ut textus valent; ordo creationis non
 * ordo textus ("9" ante "17"); acta cum id malo praetereuntur; id
 * cusum iam praesens (etiam deletum) aut malum recusatur */
interior vacuum
_probare_ids_veteres (
    CredoContextus* c)
{
               Mundus* m;
     AgendaDocumentum* doc;
     AgendaDocumentum* altera;
                  Xar* l;
                  i32  numerus;
               chorda  nova;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->volumen);
    actum_serere(m, "<res actio=\"nova\" id=\"9\"><@titulus=>novem</>"
        "</res>", c->piscina);
    actum_serere(m, "<res actio=\"nova\" id=\"17\"><@titulus=>"
        "septemdecim</></res>", c->piscina);
    actum_serere(m,
        "<res actio=\"dies\" id=\"9\" dies=\"2026-10-14\"/>",
        c->piscina);
    actum_serere(m, "<res actio=\"nova\" id=\"a-b\"><@titulus=>mala</>"
        "</res>", c->piscina);
    actum_serere(m, "<res actio=\"nova\" id=\"01jc\"><@titulus=>"
        "minuscula</></res>", c->piscina);
    doc = agenda_aperire(c->piscina, m->intern, m->volumen, "", III);
    CREDO_NECESSE_NON_NIHIL(doc);
    CREDO_AEQUALIS_I32(agenda_numerus_rerum(doc), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_res_index(doc, ZEPHYRUM)->id,
        "9");
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_res_index(doc, I)->id, "17");
    CREDO_VERUM(agenda_res(doc, lit("9", c->piscina))->dies_posita);
    CREDO_VERUM(agenda_perficere(doc, lit("17", c->piscina),
        fasti_dies(MMXXVI, X, XIV)) > ZEPHYRUM);
    CREDO_VERUM(agenda_perficere(doc, lit("9", c->piscina),
        fasti_dies(MMXXVI, X, XIV)) > ZEPHYRUM);
    /* perfectae die aequali: creatione descendens ("17" ante "9") */
    l = agenda_perfectae(doc, c->piscina);
    CREDO_NECESSE(l && xar_numerus(l) == II);
    CREDO_CHORDA_AEQUALIS_LITERIS(id_in(l, ZEPHYRUM), "17");
    /* res nova post vetera: ULID, ordine creationis ultima */
    nova = agenda_rem_creare(doc, lit("nova", c->piscina));
    CREDO_VERUM(ulid_est(nova));
    CREDO_CHORDA_AEQUALIS(agenda_res_index(doc, II)->id, nova);
    /* id cusum iam praesens recusatur, etiam deletum */
    numerus = VIII;
    agenda_monetam_ponere(doc, moneta_numerator, &numerus);
    CREDO_AEQUALIS_I32(agenda_rem_creare(doc, lit("iterata",
        c->piscina)).mensura, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_rem_creare(doc, lit("decem",
        c->piscina)), "10");
    CREDO_VERUM(agenda_rem_delere(doc, lit("17", c->piscina))
        > ZEPHYRUM);
    numerus = XVI;
    CREDO_AEQUALIS_I32(agenda_rem_creare(doc, lit("rediviva",
        c->piscina)).mensura, ZEPHYRUM);
    /* id malum cusum recusatur */
    agenda_monetam_ponere(doc, fons_malus, NIHIL);
    CREDO_AEQUALIS_I32(agenda_rem_creare(doc, lit("mala",
        c->piscina)).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(agenda_numerus_rerum(doc), III);
    /* ex volumine (checkpoints): ordo et ids idem */
    CREDO_VERUM(agenda_verificare(doc));
    altera = agenda_aperire(c->piscina, m->intern, m->volumen, "", III);
    CREDO_NECESSE_NON_NIHIL(altera);
    CREDO_CHORDA_AEQUALIS(agenda_sigillum_hex(altera, c->piscina),
        agenda_sigillum_hex(doc, c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_res_index(altera,
        ZEPHYRUM)->id,
        "9");
    CREDO_CHORDA_AEQUALIS(agenda_res_index(altera, I)->id, nova);
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
    { "D3: identitates ULID", _probare_identitates, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "D3: ids veteres", _probare_ids_veteres, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("agenda", SECTIONES);
}
