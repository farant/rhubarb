/* probatio_membrum__prompta.c - linea promptae (nomenclator-plan U3):
 * aperire, legere, claves vim, Reditus (textus tonsus, aperta manet),
 * Esc claudit, textus cum '"' (effugitus, circuitus duo), maximum,
 * figura */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "insula.h"
#include "eventus.h"
#include "mandatum.h"
#include "thema.h"
#include "membrum__prompta.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens constans character scriptor[] = "probatio.agere";

nomen structura {
     InternamentumChorda* intern;
      InsulaRepositorium* repo;
             InsulaRamus  ramus;
} Mundus;

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(x, ZEPHYRUM, magnitudo(Mundus));
    x->intern  = internamentum_creare(c->piscina);
    x->repo    = insula_repositorium_creare(c->piscina, x->intern,
        "<hospes><latus id=\"t\"/></hospes>");
    si (x->repo)
    {
        x->ramus = insula_ramus(x->repo, "latus", "t");
    }
    c->datum = x;
}

/* eventus clavis: codex et character productus */
interior Eventus
clavis (
    clavis_t codex,
         s32 producta)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis    = codex;
    e.datum.clavis.producta  = producta;
    e.datum.clavis.runa      = producta;
    redde e;
}

interior PromptaExitus
premere (
      Mundus* x,
    clavis_t  codex,
         s32  producta,
      chorda* textus,
     Piscina* p)
{
    Eventus e;

    e = clavis(codex, producta);
    redde prompta_clavem_agere(&x->ramus, scriptor, &e, textus, p);
}

interior PromptaExitus
typare (
                Mundus* x,
    constans character* s,
               Piscina* p)
{
    PromptaExitus r;
           chorda t;

    r = PROMPTA_NIHIL;
    per (; *s; s++)
    {
        r = premere(x, (clavis_t)*s, (s32)*s, &t, p);
    }
    redde r;
}

/* textus promptae = exspectatus ? */
interior b32
textus_est (
                 Mundus* x,
     constans character* exspectatus)
{
    character b[PROMPTA_MAXIMA];
          i32 n;

    n = prompta_textus(&x->ramus, b);
    si (   n                                 != (i32)strlen(exspectatus)
        || memcmp(b, exspectatus, (size_t)n) != ZEPHYRUM)
    {
        imprimere("  textus: '%.*s'\n", (integer)n, b);
        redde FALSUM;
    }
    redde VERUM;
}

/* aperire, legere, claudere */
interior vacuum
_probare_aperire (
    CredoContextus* c)
{
    Mundus* x;
    chorda  genus;
    chorda* a;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x->repo != NIHIL);
    CREDO_FALSUM(prompta_aperta(&x->ramus, NIHIL));
    CREDO_VERUM(prompta_aperire(&x->ramus, scriptor, "nova",
        chorda_ex_literis("abc", c->piscina), c->piscina));
    CREDO_VERUM(prompta_aperta(&x->ramus, &genus));
    CREDO_CHORDA_AEQUALIS_LITERIS(genus, "nova");
    CREDO_VERUM(textus_est(x, "abc"));
    a = insula_ramus_attributum(&x->ramus, "prompta_cursor");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "3"));
    CREDO_VERUM(prompta_claudere(&x->ramus, scriptor, c->piscina));
    CREDO_FALSUM(prompta_aperta(&x->ramus, NIHIL));
    CREDO_VERUM(textus_est(x, ""));
}

/* claves: inserere, sinistra, Home, Delete, End, Backspace; non clavis
 * NIHIL */
interior vacuum
_probare_claves (
    CredoContextus* c)
{
    Mundus* x;
    chorda  t;
   Eventus  e;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(prompta_aperire(&x->ramus, scriptor, "nova",
        chorda_ex_literis("abc", c->piscina), c->piscina));
    CREDO_VERUM(typare(x, "x", c->piscina) == PROMPTA_EDITA);
    CREDO_VERUM(textus_est(x, "abcx"));
    (vacuum)premere(x, CLAVIS_SINISTER, ZEPHYRUM, &t, c->piscina);
    (vacuum)typare(x, "y", c->piscina);
    CREDO_VERUM(textus_est(x, "abcyx"));
    (vacuum)premere(x, CLAVIS_DOMUS, ZEPHYRUM, &t, c->piscina);
    (vacuum)premere(x, CLAVIS_DELERE, ZEPHYRUM, &t, c->piscina);
    CREDO_VERUM(textus_est(x, "bcyx"));
    (vacuum)premere(x, CLAVIS_FINIS, ZEPHYRUM, &t, c->piscina);
    (vacuum)premere(x, CLAVIS_RETRORSUM, ZEPHYRUM, &t, c->piscina);
    CREDO_VERUM(textus_est(x, "bcy"));
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = EVENTUS_MUS_DEPRESSUS;
    CREDO_VERUM(prompta_clavem_agere(&x->ramus, scriptor, &e, &t,
        c->piscina) == PROMPTA_NIHIL);
}

/* Reditus: textus tonsus, aperta manet; Esc claudit */
interior vacuum
_probare_reditum (
    CredoContextus* c)
{
    Mundus* x;
    chorda  t;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(prompta_aperire(&x->ramus, scriptor, "titulus",
        chorda_ex_literis("panis", c->piscina), c->piscina));
    (vacuum)typare(x, "  ", c->piscina);
    CREDO_VERUM(premere(x, CLAVIS_REDITUS, XIII, &t, c->piscina)
        == PROMPTA_REDITUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(t, "panis");
    CREDO_VERUM(prompta_aperta(&x->ramus, NIHIL));
    CREDO_VERUM(premere(x, CLAVIS_EFFUGIUM, XXVII, &t, c->piscina)
        == PROMPTA_EFFUGIUM);
    CREDO_FALSUM(prompta_aperta(&x->ramus, NIHIL));
}

/* '"' et '<' in textu: effugitus, circuitus duo; maximum */
interior vacuum
_probare_textum (
    CredoContextus* c)
{
       Mundus* x;
    character  prolixus[PROMPTA_MAXIMA];

    x = (Mundus*)c->datum;
    CREDO_NECESSE(prompta_aperire(&x->ramus, scriptor, "nova",
        chorda_ex_literis("say \"hi\" <b>", c->piscina), c->piscina));
    CREDO_VERUM(textus_est(x, "say \"hi\" <b>"));
    (vacuum)typare(x, "!", c->piscina);
    CREDO_VERUM(textus_est(x, "say \"hi\" <b>!"));
    memset(prolixus, 'a', PROMPTA_MAXIMA - I);
    prolixus[PROMPTA_MAXIMA - I] = '\0';
    CREDO_NECESSE(prompta_aperire(&x->ramus, scriptor, "nova",
        chorda_ex_literis(prolixus, c->piscina), c->piscina));
    CREDO_VERUM(typare(x, "z", c->piscina) == PROMPTA_EDITA);
    CREDO_VERUM(textus_est(x, prolixus));
}

interior b32
rectangulum_coloris (
    constans Mandata* md,
          ColorThema  color,
                 b32  impletum)
{
    constans Mandatum* m;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        m = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   m->genus       == MANDATUM_RECTANGULUM
            && m->impletum    == impletum
            && m->color.genus == COLOR_MANDATI_THEMA
            && m->color.valor == (i32)color)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior constans Mandatum*
textus_mandatum (
      constans Mandata* md,
    constans character* s)
{
    constans Mandatum* m;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        m = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   m->genus == MANDATUM_TEXTUS
            && chorda_aequalis_literis(m->textus, s))
        {
            redde m;
        }
    }
    redde NIHIL;
}

/* figura: titulus obscurus, campus, textus, cursor */
interior vacuum
_probare_figuram (
    CredoContextus* c)
{
               Mundus* x;
              Mandata* md;
                Fines  f;
    constans Mandatum* t;

    x = (Mundus*)c->datum;
    thema_initiare();
    CREDO_NECESSE(prompta_aperire(&x->ramus, scriptor, "nova",
        chorda_ex_literis("lac", c->piscina), c->piscina));
    md          = mandata_creare(c->piscina, x->intern);
    f.x         = X;
    f.y         = XX;
    f.latitudo  = CC;
    f.altitudo  = XVI;
    prompta_pingere(md, f, "new:", &x->ramus);
    t = textus_mandatum(md, "new:");
    CREDO_NECESSE_NON_NIHIL(t);
    CREDO_VERUM(t->color.valor == (i32)COLOR_TEXT_DIM);
    CREDO_VERUM(t->fines.x >= X && t->fines.y >= XX);
    t = textus_mandatum(md, "lac");
    CREDO_NECESSE_NON_NIHIL(t);
    CREDO_VERUM(t->color.valor == (i32)COLOR_TEXT);
    CREDO_VERUM(rectangulum_coloris(md, COLOR_SUPERFICIES, VERUM));
    CREDO_VERUM(rectangulum_coloris(md, COLOR_CURSOR, VERUM));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "aperire", _probare_aperire, mundum_parare, NIHIL, NIHIL, NIHIL },
    { "claves", _probare_claves, mundum_parare, NIHIL, NIHIL, NIHIL },
    { "reditus", _probare_reditum, mundum_parare, NIHIL, NIHIL, NIHIL },
    { "textus", _probare_textum, mundum_parare, NIHIL, NIHIL, NIHIL },
    { "figura", _probare_figuram, mundum_parare, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("membrum__prompta", SECTIONES);
}
