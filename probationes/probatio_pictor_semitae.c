/* probatio_pictor_semitae.c - semitae R4c (semitae-spec.md): campus
 * pictura (ligatio) in montatione et cum pictura mutata
 * scribitur; pictor_argumentum_ponere - URI picturam exstantem
 * ostendit (numquam condit), cognomen ut ante (condere: ignota
 * conditur); semita a vico applicata documentum mutat solum post
 * pictor_semitam_secuta. Montatio in ramo <pictor id="p1">, pictura
 * altera "flores" in volumine temporario. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "pictor_documentum.h"
#include "pictor_applicatio.h"
#include "semita.h"
#include "organum_montatio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

nomen structura {
     InternamentumChorda* intern;
                 Volumen* volumen;
      InsulaRepositorium* repo;
          PictorMontatio  m;
        constans Organum* organum;
        PictorDocumentum* flores;
} Mundus;

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(x, ZEPHYRUM, magnitudo(Mundus));
    thema_initiare();
    x->intern   = internamentum_creare(c->piscina);
    x->volumen  = volumen_temporarium(c->piscina,
        "probatio_pictor_semitae");
    c->datum    = x;
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    si (x && x->volumen)
    {
        volumen_claudere(x->volumen);
    }
}

/* pictura "flores" (C x LX) in volumine, deinde pictor in <pictor
 * id="p1"> */
interior b32
mundum_struere (
     Mundus* x,
    Piscina* p)
{
    si (!x->volumen)
    {
        redde FALSUM;
    }
    x->flores = pictor_documentum_creare(p, x->intern, x->volumen,
        "flores", C, LX, LXIV);
    x->repo = insula_repositorium_creare(p, x->intern,
        "<schirmata focus=\"pagina\"/>");
    redde x->flores && x->repo && pictor_montare(&x->m, p, x->intern,
        x->volumen, x->repo, "p1", CDLXXX, CDLXXX, NIHIL);
}

interior b32
campus_est (
                 Mundus* x,
     constans character* titulus,
     constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(&x->m.ramus, titulus);
    redde a && chorda_aequalis_literis(*a, valor);
}

/* organum pictoris pro semita_legere; ctx = Mundus* */
interior constans Organum*
organa_pictoris (
     vacuum* ctx,
     chorda  genus)
{
    redde chorda_aequalis_literis(genus, "pictor")
        ? ((Mundus*)ctx)->organum : NIHIL;
}

interior vacuum
organum_parare (
     Mundus* x,
    Piscina* p)
{
    si (!x->organum)
    {
        x->organum = organum_legere_genus(p, x->intern, "pictor",
            "probatio");
    }
}

/* semita currens ut URI aequat exspectatam? */
interior b32
semita_est (
                 Mundus* x,
                Piscina* p,
     constans character* exspectata)
{
    chorda uri;

    organum_parare(x, p);
    uri = semita_ad_uri(organa_pictoris, x, semita_legere(
        organa_pictoris, x, &x->m.ramus, "pictor", p, x->intern), p);
    si (!chorda_aequalis_literis(uri, exspectata))
    {
        imprimere("semita: '%.*s', exspectata '%s'\n", (int)uri.mensura,
            (constans character*)uri.datum, exspectata);
        redde FALSUM;
    }
    redde VERUM;
}

/* semitam applicare ut vicus (historia): scriptura directa */
interior b32
semitam_applicare (
                 Mundus* x,
                Piscina* p,
     constans character* uri)
{
     StmlNodus* semita;
        chorda  vitium;

    organum_parare(x, p);
    semita = semita_ex_uri(organa_pictoris, x, chorda_ex_literis(uri,
        p), p, x->intern, &vitium);
    redde semita && semita_applicare(organa_pictoris, x, &x->m.ramus,
        semita, p, &vitium);
}

/* documentum ostensum = flores? (C x LX; sine memoria communi
 * documentum iterum aperitur - indicator non idem) */
interior b32
flores_ostensae (
    Mundus* x)
{
    redde x->m.doc && x->m.doc->latitudo == C
        && x->m.doc->altitudo == LX
        && x->m.actiones_ctx.doc == x->m.doc;
}

/* campus pictura: montatio scribit id; pictura mutata scribit
 * spatium; spatium sine pictura nihil mutat */
interior vacuum
_probare_campum (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c->piscina));
    CREDO_VERUM(campus_est(x, "pictura", "p1"));
    CREDO_VERUM(semita_est(x, c->piscina, "pictor/p1"));
    CREDO_VERUM(pictor_picturam_ponere(&x->m, "flores"));
    CREDO_VERUM(campus_est(x, "pictura", "flores"));
    CREDO_FALSUM(pictor_picturam_ponere(&x->m, "nihil"));
    CREDO_VERUM(campus_est(x, "pictura", "flores"));
    CREDO_VERUM(semita_est(x, c->piscina, "pictor/flores"));
}

/* argumentum: URI exstantem ostendit, ignotam recusat (numquam
 * condit); cognomen exstantem ostendit, ignotam condit solum si
 * condere */
interior vacuum
_probare_argumentum (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c->piscina));
    CREDO_VERUM(pictor_argumentum_ponere(&x->m, "pictor/flores",
        VERUM));
    CREDO_VERUM(flores_ostensae(x));
    CREDO_VERUM(campus_est(x, "pictura", "flores"));
    /* URI: pictura ignota recusatur, etiam cum condere */
    CREDO_FALSUM(pictor_argumentum_ponere(&x->m, "pictor/novum",
        VERUM));
    CREDO_VERUM(flores_ostensae(x));
    CREDO_VERUM(campus_est(x, "pictura", "flores"));
    /* URI: campus extra semitam */
    CREDO_FALSUM(pictor_argumentum_ponere(&x->m, "pictor/p1?zoom=2",
        VERUM));
    CREDO_VERUM(campus_est(x, "pictura", "flores"));
    /* cognomen */
    CREDO_VERUM(pictor_argumentum_ponere(&x->m, "p1", FALSUM));
    CREDO_VERUM(campus_est(x, "pictura", "p1"));
    CREDO_FALSUM(pictor_argumentum_ponere(&x->m, "novum", FALSUM));
    CREDO_VERUM(campus_est(x, "pictura", "p1"));
    CREDO_VERUM(pictor_argumentum_ponere(&x->m, "novum", VERUM));
    CREDO_VERUM(campus_est(x, "pictura", "novum"));
    CREDO_VERUM(semita_est(x, c->piscina, "pictor/novum"));
    CREDO_FALSUM(flores_ostensae(x));
}

/* semita a vico applicata campum mutat; documentum sequitur solum post
 * pictor_semitam_secuta */
interior vacuum
_probare_semitam_secutam (
    CredoContextus* c)
{
              Mundus* x;
    PictorDocumentum* proprius;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c->piscina));
    proprius = x->m.doc;
    CREDO_NECESSE(semitam_applicare(x, c->piscina, "pictor/flores"));
    CREDO_VERUM(campus_est(x, "pictura", "flores"));
    CREDO_VERUM(x->m.doc == proprius);
    CREDO_VERUM(pictor_semitam_secuta(&x->m));
    CREDO_VERUM(flores_ostensae(x));
    CREDO_NECESSE(semitam_applicare(x, c->piscina, "pictor/p1"));
    CREDO_VERUM(pictor_semitam_secuta(&x->m));
    CREDO_VERUM(x->m.doc == proprius);
    /* pictura ignota (campus manu scriptus): FALSUM, documentum
     * manet */
    CREDO_NECESSE(semitam_applicare(x, c->piscina, "pictor/nemo"));
    CREDO_FALSUM(pictor_semitam_secuta(&x->m));
    CREDO_VERUM(x->m.doc == proprius);
    CREDO_FALSUM(pictor_semitam_secuta(NIHIL));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "campus", _probare_campum, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "argumentum", _probare_argumentum, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "semita_secuta", _probare_semitam_secutam, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pictor_semitae", SECTIONES);
}
