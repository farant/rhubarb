/* probatio_membrum__praecepta.c - tabula praeceptorum (nomenclator-plan
 * U2): verificatio, formae clavium, praeceptum ad clavem per visum,
 * status (absens / obscurum / praesto), invocatio per nomen */
#include "latina.h"
#include "piscina.h"
#include "eventus.h"
#include "membrum__praecepta.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VISUS_A     I
#define VISUS_B     II
#define REQ_ELECTA  I

/* contextus hospitis ficti */
nomen structura {
    i32 vocationes;   /* functiones vocatae */
    b32 electa;       /* requisitum REQ_ELECTA */
} Hospes;

interior b32
fac_verum (
    vacuum* ctx)
{
    ((Hospes*)ctx)->vocationes++;
    redde VERUM;
}

interior b32
fac_falsum (
    vacuum* ctx)
{
    ((Hospes*)ctx)->vocationes++;
    redde FALSUM;
}

interior b32
requisita (
       i32  r,
    vacuum* ctx)
{
    redde !(r & REQ_ELECTA) || ((Hospes*)ctx)->electa;
}

hic_manens constans Praeceptum TABULA[] = {
    { "t.a", "A", "a", VISUS_A, ZEPHYRUM, fac_verum },
    { "t.b", "B", "X", VISUS_A | VISUS_B, REQ_ELECTA, fac_verum },
    { "t.c", "C", "Enter", VISUS_B, ZEPHYRUM, fac_verum },
    { "t.d", "D", "Esc", VISUS_A, ZEPHYRUM, fac_verum },
    { "t.e", "E", "C-r", VISUS_A, ZEPHYRUM, fac_verum },
    { "t.f", "F", NIHIL, VISUS_A | VISUS_B, ZEPHYRUM, fac_falsum },
    { "t.g", "G", "a", VISUS_B, ZEPHYRUM, fac_verum },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

/* titulus iteratus */
hic_manens constans Praeceptum MALA_TITULUS[] = {
    { "t.a", "A", "a", VISUS_A, ZEPHYRUM, fac_verum },
    { "t.a", "A2", "b", VISUS_A, ZEPHYRUM, fac_verum },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

/* clavis bis in eodem visu */
hic_manens constans Praeceptum MALA_CLAVIS[] = {
    { "t.a", "A", "a", VISUS_A, ZEPHYRUM, fac_verum },
    { "t.b", "B", "a", VISUS_A | VISUS_B, ZEPHYRUM, fac_verum },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

hic_manens constans Praeceptum MALA_CAMPI[] = {
    { "t.a", NIHIL, "a", VISUS_A, ZEPHYRUM, fac_verum },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

hic_manens constans Praeceptum MALA_VISUS[] = {
    { "t.a", "A", "a", ZEPHYRUM, ZEPHYRUM, fac_verum },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

hic_manens constans Praeceptum MALA_FN[] = {
    { "t.a", "A", "a", VISUS_A, ZEPHYRUM, NIHIL },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

/* eventus clavis: codex, character productus, runa, modificantes */
interior Eventus
clavis (
    clavis_t codex,
         s32 producta,
         s32 runa,
         i32 modificantes)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                      = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis        = codex;
    e.datum.clavis.producta      = producta;
    e.datum.clavis.runa          = runa;
    e.datum.clavis.modificantes  = modificantes;
    redde e;
}

interior vacuum
_probare_verificationem (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(praecepta_verificare(TABULA));
    CREDO_FALSUM(praecepta_verificare(MALA_TITULUS));
    CREDO_FALSUM(praecepta_verificare(MALA_CLAVIS));
    CREDO_FALSUM(praecepta_verificare(MALA_CAMPI));
    CREDO_FALSUM(praecepta_verificare(MALA_VISUS));
    CREDO_FALSUM(praecepta_verificare(MALA_FN));
    CREDO_VERUM(praecepta_invenire(TABULA, "t.e") == &TABULA[IV]);
    CREDO_NIHIL(praecepta_invenire(TABULA, "t.z"));
}

interior vacuum
_probare_claves (
    CredoContextus* c)
{
    Eventus e;

    (vacuum)c;
    e = clavis((clavis_t)'a', 'a', 'a', ZEPHYRUM);
    CREDO_VERUM(praecepta_clavis_congruit("a", &e));
    CREDO_FALSUM(praecepta_clavis_congruit("b", &e));
    e = clavis((clavis_t)'a', 'a', 'a', (i32)MOD_IMPERIUM);
    CREDO_FALSUM(praecepta_clavis_congruit("a", &e));
    e = clavis((clavis_t)'a', 'a', 'a', (i32)MOD_SUPER);
    CREDO_FALSUM(praecepta_clavis_congruit("a", &e));
    e = clavis((clavis_t)'X', 'X', 'x', (i32)MOD_SHIFT);
    CREDO_VERUM(praecepta_clavis_congruit("X", &e));
    CREDO_FALSUM(praecepta_clavis_congruit("x", &e));
    e = clavis(CLAVIS_REDITUS, XIII, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(praecepta_clavis_congruit("Enter", &e));
    CREDO_FALSUM(praecepta_clavis_congruit("Esc", &e));
    e = clavis(CLAVIS_EFFUGIUM, XXVII, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(praecepta_clavis_congruit("Esc", &e));
    e = clavis((clavis_t)'r', ZEPHYRUM, 'r', (i32)MOD_IMPERIUM);
    CREDO_VERUM(praecepta_clavis_congruit("C-r", &e));
    e = clavis((clavis_t)'r', 'r', 'r', ZEPHYRUM);
    CREDO_FALSUM(praecepta_clavis_congruit("C-r", &e));
    CREDO_VERUM(praecepta_clavis_congruit("r", &e));
}

/* ad clavem per visum: "a" in A = t.a, in B = t.g; Enter solum in B */
interior vacuum
_probare_ad_clavem (
    CredoContextus* c)
{
    Eventus e;

    (vacuum)c;
    e = clavis((clavis_t)'a', 'a', 'a', ZEPHYRUM);
    CREDO_VERUM(praecepta_ad_clavem(TABULA, VISUS_A, &e)
        == &TABULA[ZEPHYRUM]);
    CREDO_VERUM(praecepta_ad_clavem(TABULA, VISUS_B, &e)
        == &TABULA[VI]);
    e = clavis(CLAVIS_REDITUS, XIII, ZEPHYRUM, ZEPHYRUM);
    CREDO_NIHIL(praecepta_ad_clavem(TABULA, VISUS_A, &e));
    CREDO_VERUM(praecepta_ad_clavem(TABULA, VISUS_B, &e)
        == &TABULA[II]);
    e = clavis((clavis_t)'q', 'q', 'q', ZEPHYRUM);
    CREDO_NIHIL(praecepta_ad_clavem(TABULA, VISUS_A, &e));
}

/* status: absens extra visum, obscurum sine requisito, praesto */
interior vacuum
_probare_statum (
    CredoContextus* c)
{
    Hospes h;

    (vacuum)c;
    memset(&h, ZEPHYRUM, magnitudo(h));
    CREDO_VERUM(praecepta_status(&TABULA[II], VISUS_A, requisita, &h)
        == PRAECEPTUM_ABSENS);
    CREDO_VERUM(praecepta_status(&TABULA[I], VISUS_A, requisita, &h)
        == PRAECEPTUM_OBSCURUM);
    h.electa = VERUM;
    CREDO_VERUM(praecepta_status(&TABULA[I], VISUS_A, requisita, &h)
        == PRAECEPTUM_PRAESTO);
    h.electa = FALSUM;
    CREDO_VERUM(praecepta_status(&TABULA[I], VISUS_A, NIHIL, &h)
        == PRAECEPTUM_PRAESTO);
    CREDO_AEQUALIS_I32(h.vocationes, ZEPHYRUM);
}

/* invocare: praesto vocat; absens, obscurum, ignotum non vocant; fn
 * FALSUM -> FALSUM (vocata tamen) */
interior vacuum
_probare_invocare (
    CredoContextus* c)
{
    Hospes h;

    (vacuum)c;
    memset(&h, ZEPHYRUM, magnitudo(h));
    CREDO_VERUM(praecepta_invocare(TABULA, "t.a", VISUS_A, requisita,
        &h));
    CREDO_AEQUALIS_I32(h.vocationes, I);
    CREDO_FALSUM(praecepta_invocare(TABULA, "t.c", VISUS_A, requisita,
        &h));
    CREDO_FALSUM(praecepta_invocare(TABULA, "t.b", VISUS_A, requisita,
        &h));
    CREDO_FALSUM(praecepta_invocare(TABULA, "t.z", VISUS_A, requisita,
        &h));
    CREDO_AEQUALIS_I32(h.vocationes, I);
    CREDO_FALSUM(praecepta_invocare(TABULA, "t.f", VISUS_B, requisita,
        &h));
    CREDO_AEQUALIS_I32(h.vocationes, II);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "verificatio", _probare_verificationem, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "claves", _probare_claves, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ad_clavem", _probare_ad_clavem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "status", _probare_statum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "invocatio", _probare_invocare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("membrum__praecepta", SECTIONES);
}
