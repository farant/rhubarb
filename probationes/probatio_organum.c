/* probatio_organum.c - manifestum organi (status-plan S1): lectio
 * manifesti pictoris (probationes/fixa/organum/pictor.stml = S0),
 * vitia canonis, leges oneratoris, canon insulae et domini generati,
 * basis communis. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "internamentum.h"
#include "stml.h"
#include "canon.h"
#include "organum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens constans character via_pictoris[] =
    "probationes/fixa/organum/pictor.stml";
hic_manens constans character via_communis[] =
    "probationes/fixa/organum/commune.stml";

/* manifestum ex plagula fixa */
interior Organum*
legere_plagulam (
                Piscina*  p,
    InternamentumChorda*  in,
     constans character*  via,
                    Xar** vitia)
{
    redde organum_legere(filum_legere_totum(via, p), p, in, vitia);
}

/* manifestum ex literis */
interior Organum*
legere_literas (
                Piscina*  p,
    InternamentumChorda*  in,
     constans character*  fons,
                    Xar** vitia)
{
    redde organum_legere(chorda_ex_literis(fons, p), p, in, vitia);
}

/* vitium aliquod acum continet? */
interior b32
vitia_continent (
                   Xar* vitia,
    constans character* acus,
               Piscina* p)
{
    i32 i;

    si (!vitia)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(vitia); i++)
    {
        si (chorda_continet(*(chorda*)xar_obtinere(vitia, i),
                chorda_ex_literis(acus, p)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* vitia imprimere (diagnosis cum assertio frangitur) */
interior vacuum
vitia_imprimere (
    Xar* vitia)
{
    i32 i;

    per (i = ZEPHYRUM; vitia && i < xar_numerus(vitia); i++)
    {
        chorda* c;

        c = (chorda*)xar_obtinere(vitia, i);
        printf("  vitium: %.*s\n", (integer)c->mensura,
            (constans character*)c->datum);
    }
}

/* manifestum minimum cum campo uno (attributa campi) et ligamine
 * 'documentum' et actione 'a.b' */
interior Organum*
legere_campum (
                Piscina*  p,
    InternamentumChorda*  in,
     constans character*  campus,
                    Xar** vitia)
{
    character fons[MMXLVIII];

    sprintf(fons,
        "<organum genus=\"probatio\" versio=\"1\">"
        "<ligamina><ligamen nomen=\"documentum\" genus=\"doc\"/>"
        "</ligamina>"
        "<actiones><actio nomen=\"a.b\"/></actiones>"
        "<status>%s</status></organum>", campus);
    redde legere_literas(p, in, fons, vitia);
}

/* arbor insulae <pictor .../> contra canonem: numerus vitiorum (-1 =
 * arbor non legitur) */
interior s32
vitia_insulae (
                  Canon* canon,
     constans character* insula,
                Piscina* p,
    InternamentumChorda* in)
{
    StmlResultus r;

    r = stml_legere_ex_literis(insula, p, in);
    si (!r.successus)
    {
        redde -I;
    }
    redde (s32)xar_numerus(canon_iudicare(canon, r.radix, p));
}


/* ==================================================
 * I. manifestum pictoris (S0) legitur
 * ================================================== */

interior vacuum
_probare_pictorem (
    CredoContextus* c)
{
           InternamentumChorda* in;
                       Organum* o;
                           Xar* vitia;
        constans OrganumCampus* k;
       constans OrganumLigamen* l;
         constans OrganumActio* a;
                           i32  i;
                           s32  systemata;

    in  = internamentum_creare(c->piscina);
    o   = legere_plagulam(c->piscina, in, via_pictoris, &vitia);
    vitia_imprimere(vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    CREDO_AEQUALIS_I32((i32)xar_numerus(vitia), ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(organum_genus(o), "pictor");
    CREDO_AEQUALIS_I32(organum_numerus_camporum(o), XX);

    /* persistens mobilis cum absente */
    k = organum_campus(o, chorda_ex_literis("zoom", c->piscina));
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_VERUM(k->vita == ORGANUM_VITA_PERSISTENS);
    CREDO_VERUM(k->rhythmus == ORGANUM_RHYTHMUS_MOBILIS);
    CREDO_VERUM(k->sedes == ORGANUM_SEDES_INSULA);
    CREDO_VERUM(k->habet_absens);
    CREDO_CHORDA_AEQUALIS_LITERIS(k->absens, "1");
    CREDO_CHORDA_AEQUALIS_LITERIS(k->scriptor, "motus");
    CREDO_CHORDA_AEQUALIS_LITERIS(k->typus, "numerus");

    /* discretus sine absente */
    k = organum_campus(o, chorda_ex_literis("instrumentum",
        c->piscina));
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_VERUM(k->rhythmus == ORGANUM_RHYTHMUS_DISCRETUS);
    CREDO_FALSUM(k->habet_absens);
    CREDO_CHORDA_AEQUALIS_LITERIS(k->typus, "electio");

    /* absens negativus */
    k = organum_campus(o, chorda_ex_literis("color_secundus",
        c->piscina));
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_CHORDA_AEQUALIS_LITERIS(k->absens, "-1");

    /* referentia */
    k = organum_campus(o, chorda_ex_literis("stratum_activum",
        c->piscina));
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_CHORDA_AEQUALIS_LITERIS(k->referentia, "documentum/stratum");
    CREDO_CHORDA_AEQUALIS_LITERIS(k->orbatus, "summum");

    /* aspectus */
    k = organum_campus(o, chorda_ex_literis("palette", c->piscina));
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_VERUM(k->vita == ORGANUM_VITA_TRANSIENS);
    CREDO_CHORDA_AEQUALIS_LITERIS(k->aspectus, "popover");
    CREDO_CHORDA_AEQUALIS_LITERIS(k->scriptor, "palette.aperire");

    /* sedes motus */
    k = organum_campus(o, chorda_ex_literis("captura", c->piscina));
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_VERUM(k->sedes == ORGANUM_SEDES_MOTUS);
    CREDO_VERUM(k->vita == ORGANUM_VITA_TRANSIENS);

    /* probatio */
    k = organum_campus(o, chorda_ex_literis("fuga", c->piscina));
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_VERUM(k->vita == ORGANUM_VITA_PROBATIO);
    CREDO_CHORDA_VACUA(k->scriptor);

    CREDO_NIHIL(organum_campus(o, chorda_ex_literis("selectio_x",
        c->piscina)));

    /* ligamina, portae, actiones */
    CREDO_AEQUALIS_I32(organum_numerus_ligaminum(o), I);
    l = organum_ligamen_index(o, ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_CHORDA_AEQUALIS_LITERIS(l->titulus, "documentum");
    CREDO_CHORDA_AEQUALIS_LITERIS(l->genus, "pictor-documentum");
    CREDO_VERUM(l->a_parente);
    CREDO_AEQUALIS_I32(organum_numerus_portarum(o), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(organum_porta_index(o,
        ZEPHYRUM)->modus,
        "petitio");
    CREDO_AEQUALIS_I32(organum_numerus_actionum(o), XIII);
    systemata = ZEPHYRUM;
    per (i = ZEPHYRUM; i < organum_numerus_actionum(o); i++)
    {
        a = organum_actio_index(o, i);
        si (a && a->systema)
        {
            systemata++;
        }
    }
    CREDO_AEQUALIS_S32(systemata, II);
}


/* ==================================================
 * II. canon organi iudicat (forma)
 * ================================================== */

interior vacuum
_probare_canonem_organi (
    CredoContextus* c)
{
    InternamentumChorda* in;
                    Xar* vitia;

    in = internamentum_creare(c->piscina);

    /* sanum minimum */
    CREDO_NON_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"x\" typus=\"numerus\" vita=\"persistens\""
        " scriptor=\"a.b\"/>", &vitia));

    /* vita ignota */
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"x\" typus=\"numerus\" vita=\"fugax\"/>",
        &vitia));
    CREDO_VERUM(vitia_continent(vitia, "vita", c->piscina));

    /* scriptor actionem non declaratam citat */
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"x\" typus=\"numerus\" vita=\"persistens\""
        " scriptor=\"nulla.actio\"/>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "nulla.actio", c->piscina));

    /* campus bis */
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"x\" typus=\"numerus\" vita=\"persistens\"/>"
        "<campus nomen=\"x\" typus=\"textus\" vita=\"persistens\"/>",
        &vitia));
    CREDO_VERUM(xar_numerus(vitia) > ZEPHYRUM);

    /* non STML */
    CREDO_NIHIL(legere_literas(c->piscina, in, "<organum", &vitia));
    CREDO_VERUM(xar_numerus(vitia) > ZEPHYRUM);
}


/* ==================================================
 * III. leges oneratoris (canon eas consulto non tenet)
 * ================================================== */

interior vacuum
_probare_leges (
    CredoContextus* c)
{
    InternamentumChorda* in;
                    Xar* vitia;

    in = internamentum_creare(c->piscina);

    /* sedes motus sine mobilis */
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"m\" typus=\"numerus\" vita=\"transiens\""
        " sedes=\"motus\"/>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "m", c->piscina));
    CREDO_VERUM(vitia_continent(vitia, "mobilis", c->piscina));

    /* sedes motus sed persistens */
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"m\" typus=\"numerus\" vita=\"persistens\""
        " rhythmus=\"mobilis\" sedes=\"motus\"/>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "transiens", c->piscina));

    /* sedes motus recta */
    CREDO_NON_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"m\" typus=\"series\" vita=\"transiens\""
        " rhythmus=\"mobilis\" sedes=\"motus\"/>", &vitia));

    /* series in insula */
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"s\" typus=\"series\" vita=\"transiens\"/>",
        &vitia));
    CREDO_VERUM(vitia_continent(vitia, "series", c->piscina));

    /* orbatus sine referentia */
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"r\" typus=\"numerus\" vita=\"persistens\""
        " orbatus=\"summum\"/>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "orbatus", c->piscina));

    /* referentia ad ligamen ignotum */
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"r\" typus=\"numerus\" vita=\"persistens\""
        " referentia=\"pagina/stratum\"/>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "pagina", c->piscina));

    /* referentia sine genere post '/' */
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"r\" typus=\"numerus\" vita=\"persistens\""
        " referentia=\"documentum\"/>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "referentia", c->piscina));

    /* referentia recta */
    CREDO_NON_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"r\" typus=\"numerus\" vita=\"persistens\""
        " referentia=\"documentum/stratum\" orbatus=\"nullum\"/>",
        &vitia));

    /* absens extra typum */
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"n\" typus=\"numerus\" vita=\"persistens\""
        " absens=\"unus\"/>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "absens", c->piscina));
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"v\" typus=\"veritas\" vita=\"persistens\""
        " absens=\"forsitan\"/>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "absens", c->piscina));

    /* absens electionis extra optiones */
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"e\" typus=\"electio\" vita=\"persistens\""
        " absens=\"tertia\"><optio>prima</optio><optio>secunda</optio>"
        "</campus>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "tertia", c->piscina));
    CREDO_NON_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"e\" typus=\"electio\" vita=\"persistens\""
        " absens=\"secunda\"><optio>prima</optio><optio>secunda</optio>"
        "</campus>", &vitia));

    /* electio sine optione; optio sine electione */
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"e\" typus=\"electio\" vita=\"persistens\"/>",
        &vitia));
    CREDO_VERUM(vitia_continent(vitia, "optio", c->piscina));
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"t\" typus=\"textus\" vita=\"persistens\">"
        "<optio>prima</optio></campus>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "optio", c->piscina));
}


/* ==================================================
 * IV. canon insulae generatus
 * ================================================== */

interior vacuum
_probare_canonem_generatum (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Organum* o;
                Organum* basis;
                  Canon* canon;
                    Xar* vitia;
                 chorda  causa;

    in  = internamentum_creare(c->piscina);
    o   = legere_plagulam(c->piscina, in, via_pictoris, &vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    causa.mensura = ZEPHYRUM;
    causa.datum = NIHIL;
    canon = organum_canonem_generare(o, c->piscina, in, &causa);
    CREDO_NECESSE_NON_NIHIL(canon);

    /* insula sana: persistens, transiens, probatio */
    CREDO_AEQUALIS_S32((s32)vitia_insulae(canon,
        "<pictor instrumentum=\"linea\" zoom=\"2\""
        " color_secundus=\"-1\""
        " palette=\"color_primus\" stratum_activum=\"3\" puncta=\"4\""
        " fuga=\"x\"/>", c->piscina, in), ZEPHYRUM);

    /* attributum ignotum */
    CREDO_MAIOR_S32((s32)vitia_insulae(canon,
        "<pictor colorem=\"1\"/>", c->piscina, in), ZEPHYRUM);
    /* typus malus */
    CREDO_MAIOR_S32((s32)vitia_insulae(canon,
        "<pictor zoom=\"magnus\"/>", c->piscina, in), ZEPHYRUM);
    /* optio extra electionem */
    CREDO_MAIOR_S32((s32)vitia_insulae(canon,
        "<pictor instrumentum=\"malleus\"/>", c->piscina, in),
        ZEPHYRUM);
    /* campus motus in insula non declaratur */
    CREDO_MAIOR_S32((s32)vitia_insulae(canon,
        "<pictor captura=\"tabula\"/>", c->piscina, in), ZEPHYRUM);
    CREDO_MAIOR_S32((s32)vitia_insulae(canon,
        "<pictor semen=\"7\"/>", c->piscina, in), ZEPHYRUM);
    /* radix = genus organi */
    CREDO_MAIOR_S32((s32)vitia_insulae(canon,
        "<scriba zoom=\"2\"/>", c->piscina, in), ZEPHYRUM);
    /* campi communes sine basi ignoti */
    CREDO_MAIOR_S32((s32)vitia_insulae(canon,
        "<pictor id=\"t_d_pictor\" focus=\"tabula\"/>", c->piscina,
        in), ZEPHYRUM);

    /* cum basi: noti */
    basis = legere_plagulam(c->piscina, in, via_communis, &vitia);
    CREDO_NECESSE_NON_NIHIL(basis);
    CREDO_NECESSE(organum_basim_addere(o, basis));
    canon = organum_canonem_generare(o, c->piscina, in, &causa);
    CREDO_NECESSE_NON_NIHIL(canon);
    CREDO_AEQUALIS_S32((s32)vitia_insulae(canon,
        "<pictor id=\"t_d_pictor\" focus=\"tabula\" zoom=\"2\"/>",
        c->piscina, in), ZEPHYRUM);
}


/* ==================================================
 * V. domini generati
 * ================================================== */

/* scriptor domini attributi in arbore <domini>; NIHIL si nullus */
interior chorda*
scriptor_domini (
             StmlNodus* domini,
    constans character* attributum)
{
    i32 i;

    per (i = ZEPHYRUM; domini->liberi
        && i < xar_numerus(domini->liberi);
         i++)
    {
        StmlNodus* d;
           chorda* a;

        d = *(StmlNodus**)xar_obtinere(domini->liberi, i);
        si (d->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        a = stml_attributum_capere(d, "attributum");
        si (a && chorda_aequalis_literis(*a, attributum))
        {
            redde stml_attributum_capere(d, "scriptor");
        }
    }
    redde NIHIL;
}

interior vacuum
_probare_dominos (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Organum* o;
                    Xar* vitia;
              StmlNodus* domini;
              StmlNodus* hodie;
           StmlResultus  r;
                 chorda  fons;
                    i32  numerus;
                    i32  congruentes;
                    i32  i;

    in  = internamentum_creare(c->piscina);
    o   = legere_plagulam(c->piscina, in, via_pictoris, &vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    domini = organum_dominos_generare(o, c->piscina, in);
    CREDO_NECESSE_NON_NIHIL(domini);
    CREDO_NECESSE(domini->titulus
        && chorda_aequalis_literis(*domini->titulus, "domini"));

    numerus = ZEPHYRUM;
    per (i = ZEPHYRUM; domini->liberi
        && i < xar_numerus(domini->liberi);
         i++)
    {
        StmlNodus* d;
           chorda* g;

        d = *(StmlNodus**)xar_obtinere(domini->liberi, i);
        si (d->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        numerus++;
        /* C2: insula una - linea sine 'genus' */
        g = stml_attributum_capere(d, "genus");
        CREDO_NIHIL(g);
    }
    /* XII campi insulae cum scriptore (S5c: palette quoque) */
    CREDO_AEQUALIS_S32((s32)numerus, XII);
    CREDO_NON_NIHIL(scriptor_domini(domini, "zoom"));
    CREDO_CHORDA_AEQUALIS_LITERIS(*scriptor_domini(domini,
        "stratum_activum"), "strata.agere");
    /* S5a: referentia -> systema relatio; S5c: popover -> dimissio;
     * ceteri sine systematibus */
    per (i = ZEPHYRUM; i < xar_numerus(domini->liberi); i++)
    {
        StmlNodus* d;
           chorda* a;
           chorda* sy;

        d   = *(StmlNodus**)xar_obtinere(domini->liberi, i);
        a   = stml_attributum_capere(d, "attributum");
        sy  = stml_attributum_capere(d, "systemata");
        si (a && chorda_aequalis_literis(*a, "stratum_activum"))
        {
            CREDO_VERUM(sy && chorda_aequalis_literis(*sy, "relatio"));
        }
        alioquin si (a && chorda_aequalis_literis(*a, "palette"))
        {
            CREDO_VERUM(sy && chorda_aequalis_literis(*sy, "dimissio"));
        }
        alioquin
        {
            CREDO_NIHIL(sy);
        }
    }
    /* S5c: aspectus popover - dominus aperiens, systema claudit */
    CREDO_CHORDA_AEQUALIS_LITERIS(*scriptor_domini(domini, "palette"),
        "palette.aperire");
    /* motus extra insulam */
    CREDO_NIHIL(scriptor_domini(domini, "semen"));
    /* sine scriptore */
    CREDO_NIHIL(scriptor_domini(domini, "fuga"));

    /* congruit cum domini.stml hodierno pro attributis communibus */
    /* tabula manu scripta ante S2, congelata */
    fons = filum_legere_totum(
        "probationes/fixa/organum/domini_ante_s2.stml", c->piscina);
    CREDO_NECESSE(fons.mensura > ZEPHYRUM);
    r = stml_legere(fons, c->piscina, in);
    CREDO_NECESSE(r.successus);
    hodie = stml_invenire_liberum(r.radix, "domini");
    CREDO_NECESSE_NON_NIHIL(hodie);
    congruentes = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(hodie->liberi); i++)
    {
        StmlNodus* d;
           chorda* a;
           chorda* s;
           chorda* novus;

        d = *(StmlNodus**)xar_obtinere(hodie->liberi, i);
        si (d->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        a = stml_attributum_capere(d, "attributum");
        s = stml_attributum_capere(d, "scriptor");
        novus = a ? scriptor_domini(domini,
            chorda_ut_cstr(*a, c->piscina)) : NIHIL;
        si (novus && s)
        {
            CREDO_CHORDA_AEQUALIS(*novus, *s);
            congruentes++;
        }
    }
    CREDO_AEQUALIS_S32((s32)congruentes, XI);
}


/* ==================================================
 * VIIb. S6b: manifestum VIVUM scribae = tabula ante S6b
 * ================================================== */

interior vacuum
_probare_scribam_vivam (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Organum* o;
                Organum* basis;
                    Xar* vitia;
              StmlNodus* domini;
              StmlNodus* ante;
           StmlResultus  r;
                    i32  i;
                    s32  novi;
                    s32  veteres;

    in    = internamentum_creare(c->piscina);
    o     = legere_plagulam(c->piscina, in, "apps/scriba/organum.stml",
        &vitia);
    vitia_imprimere(vitia);
    basis = legere_plagulam(c->piscina, in, "apps/organum_commune.stml",
        &vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    CREDO_NECESSE_NON_NIHIL(basis);
    CREDO_NECESSE(organum_basim_addere(o, basis));
    domini = organum_dominos_generare(o, c->piscina, in);
    CREDO_NECESSE_NON_NIHIL(domini);
    r = stml_legere(filum_legere_totum(
        "probationes/fixa/organum/scriba_domini_ante_s6.stml",
        c->piscina),
        c->piscina, in);
    CREDO_NECESSE(r.successus);
    ante = stml_invenire_liberum(r.radix, "domini");
    CREDO_NECESSE_NON_NIHIL(ante);
    /* novus -> idem in vetere; vetus -> idem in novo (aequales) */
    novi = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(domini->liberi); i++)
    {
        StmlNodus* d;
           chorda* a;
           chorda* vetus;

        d = *(StmlNodus**)xar_obtinere(domini->liberi, i);
        a = stml_attributum_capere(d, "attributum");
        novi++;
        /* D5 S3c / comites P2-P3: campi 'pagina', 'liber',
         * 'liber_titulus' post S6 additi - in imagine vetere desunt */
        si (   chorda_aequalis_literis(*a, "pagina")
            || chorda_aequalis_literis(*a, "liber")
            || chorda_aequalis_literis(*a, "liber_titulus"))
        {
            CREDO_CHORDA_AEQUALIS_LITERIS(*stml_attributum_capere(d,
                "scriptor"), "pagina.clavis");
            perge;
        }
        vetus  = scriptor_domini(ante, chorda_ut_cstr(*a, c->piscina));
        CREDO_NON_NIHIL(vetus);
        si (vetus)
        {
            CREDO_CHORDA_AEQUALIS(*vetus,
                *stml_attributum_capere(d, "scriptor"));
        }
    }
    veteres = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(ante->liberi); i++)
    {
        StmlNodus* d;
           chorda* a;

        d = *(StmlNodus**)xar_obtinere(ante->liberi, i);
        si (d->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        a = stml_attributum_capere(d, "attributum");
        CREDO_NON_NIHIL(scriptor_domini(domini, chorda_ut_cstr(*a,
            c->piscina)));
        veteres++;
    }
    CREDO_AEQUALIS_S32(novi, veteres + III);
    CREDO_AEQUALIS_S32(novi, XIX);
}


/* ==================================================
 * VIII. S4: valores initiales ex absentibus
 * ================================================== */

interior vacuum
_probare_initium (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Organum* o;
                    Xar* vitia;

    in  = internamentum_creare(c->piscina);
    o   = legere_plagulam(c->piscina, in, via_pictoris, &vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    /* fixa S0: absentes ordine manifesti; instrumentum ibi sine
     * absente; semen/palette/motus/probatio nulli */
    CREDO_CHORDA_AEQUALIS_LITERIS(organum_initium(o, c->piscina),
        " color_primus=\"0\" color_secundus=\"-1\" exemplar=\"0\""
        " magnitudo_penicilli=\"1\" magnitudo_aspergilli=\"1\""
        " magnitudo_lineae=\"1\" zoom=\"1\" pan_x=\"0\" pan_y=\"0\"");

    /* nullus absens: vacua */
    o = legere_campum(c->piscina, in,
        "<campus nomen=\"x\" typus=\"numerus\" vita=\"persistens\"/>",
        &vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    CREDO_CHORDA_VACUA(organum_initium(o, c->piscina));

    /* sedes motus cum absente: extra insulam, non initialis */
    o = legere_campum(c->piscina, in,
        "<campus nomen=\"m\" typus=\"numerus\" vita=\"transiens\""
        " rhythmus=\"mobilis\" sedes=\"motus\" absens=\"3\"/>"
        "<campus nomen=\"n\" typus=\"numerus\" vita=\"transiens\""
        " absens=\"4\"/>", &vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    CREDO_CHORDA_AEQUALIS_LITERIS(organum_initium(o, c->piscina),
        " n=\"4\"");
}


/* ==================================================
 * VII. S2: manifestum VIVUM pictoris = tabula ante S2
 * ================================================== */

/* manifestum vivum (apps/pictor/organum.stml + basis communis) dominos
 * generat qui tabulam manu scriptam ante S2 aequant: omnis dominus
 * novus idem scriptor; veteres absentes soli mortui (cursus, gradus,
 * sigillum - census IX inventum 7) */
interior vacuum
_probare_pictorem_vivum (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Organum* o;
                Organum* basis;
                    Xar* vitia;
              StmlNodus* domini;
              StmlNodus* ante;
           StmlResultus  r;
                    i32  i;
                    s32  novi;
                    s32  absentes;

    in    = internamentum_creare(c->piscina);
    o     = legere_plagulam(c->piscina, in, "apps/pictor/organum.stml",
        &vitia);
    vitia_imprimere(vitia);
    basis = legere_plagulam(c->piscina, in, "apps/organum_commune.stml",
        &vitia);
    vitia_imprimere(vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    CREDO_NECESSE_NON_NIHIL(basis);
    CREDO_NECESSE(organum_basim_addere(o, basis));
    domini = organum_dominos_generare(o, c->piscina, in);
    CREDO_NECESSE_NON_NIHIL(domini);
    r = stml_legere(filum_legere_totum(
        "probationes/fixa/organum/domini_ante_s2.stml", c->piscina),
        c->piscina, in);
    CREDO_NECESSE(r.successus);
    ante = stml_invenire_liberum(r.radix, "domini");
    CREDO_NECESSE_NON_NIHIL(ante);

    /* novus -> idem in tabula vetere */
    novi = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(domini->liberi); i++)
    {
        StmlNodus* d;
           chorda* a;
           chorda* vetus;

        d = *(StmlNodus**)xar_obtinere(domini->liberi, i);
        a = stml_attributum_capere(d, "attributum");
        CREDO_NECESSE_NON_NIHIL(a);
        vetus = scriptor_domini(ante, chorda_ut_cstr(*a, c->piscina));
        /* S5c: palette dominum NOVUM habet (olim sine domino);
         * glutinatio (2026-10-13) et pagina_pinacothecae (pinacotheca
         * T5, 2026-10-26) campi novi post S2 */
        si (   chorda_aequalis_literis(*a, "palette")
            || chorda_aequalis_literis(*a, "glutinatio")
            || chorda_aequalis_literis(*a, "pagina_pinacothecae"))
        {
            CREDO_NIHIL(vetus);
            novi++;
            perge;
        }
        CREDO_NON_NIHIL(vetus);
        si (vetus)
        {
            CREDO_CHORDA_AEQUALIS(*vetus,
                *stml_attributum_capere(d, "scriptor"));
        }
        novi++;
    }
    CREDO_AEQUALIS_S32(novi, XIX);

    /* vetus sine novo: soli mortui */
    absentes = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(ante->liberi); i++)
    {
        StmlNodus* d;
           chorda* a;

        d = *(StmlNodus**)xar_obtinere(ante->liberi, i);
        si (d->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        a = stml_attributum_capere(d, "attributum");
        si (   a && !scriptor_domini(domini, chorda_ut_cstr(*a,
                c->piscina)))
        {
            CREDO_VERUM(chorda_aequalis_literis(*a, "cursus")
                || chorda_aequalis_literis(*a, "gradus")
                || chorda_aequalis_literis(*a, "sigillum"));
            absentes++;
        }
    }
    CREDO_AEQUALIS_S32(absentes, III);
}


/* ==================================================
 * VI. basis
 * ================================================== */

interior vacuum
_probare_basim (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Organum* o;
                Organum* basis;
                Organum* gemina;
                    Xar* vitia;
                    i32  n;

    in     = internamentum_creare(c->piscina);
    o      = legere_plagulam(c->piscina, in, via_pictoris, &vitia);
    basis  = legere_plagulam(c->piscina, in, via_communis, &vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    CREDO_NECESSE_NON_NIHIL(basis);
    n = organum_numerus_camporum(o);

    CREDO_VERUM(organum_basim_addere(o, basis));
    CREDO_AEQUALIS_I32(organum_numerus_camporum(o), n + II);
    /* basis ANTE proprios */
    CREDO_CHORDA_AEQUALIS_LITERIS(organum_campus_index(o,
        ZEPHYRUM)->titulus, "id");
    CREDO_NON_NIHIL(organum_campus(o, chorda_ex_literis("focus",
        c->piscina)));

    /* iterum: titulus bis - nihil additum */
    CREDO_FALSUM(organum_basim_addere(o, basis));
    CREDO_AEQUALIS_I32(organum_numerus_camporum(o), n + II);

    /* basis campum proprium geminans */
    gemina = legere_literas(c->piscina, in,
        "<organum genus=\"g\" versio=\"1\"><status>"
        "<campus nomen=\"zoom\" typus=\"numerus\" vita=\"persistens\"/>"
        "<campus nomen=\"aliud\" typus=\"numerus\""
        " vita=\"persistens\"/>"
        "</status></organum>", &vitia);
    CREDO_NECESSE_NON_NIHIL(gemina);
    CREDO_FALSUM(organum_basim_addere(o, gemina));
    CREDO_NIHIL(organum_campus(o, chorda_ex_literis("aliud",
        c->piscina)));
}


/* ==================================================
 * semitae R0 (project-specs/semitae-spec.md): attributa semita,
 * ambitus, historia; leges oneratoris; montationes legibiles
 * ================================================== */

interior vacuum
_probare_semitam (
    CredoContextus* c)
{
       InternamentumChorda* in;
                       Xar* vitia;
                   Organum* o;
    constans OrganumCampus* k;

    in = internamentum_creare(c->piscina);

    /* absentes: nulla, organum, substitue */
    o = legere_campum(c->piscina, in,
        "<campus nomen=\"x\" typus=\"numerus\" vita=\"persistens\"/>",
        &vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    k = organum_campus(o, chorda_ex_literis("x", c->piscina));
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_VERUM(k->semita == ORGANUM_SEMITA_NULLA);
    CREDO_VERUM(k->ambitus == ORGANUM_AMBITUS_ORGANUM);
    CREDO_VERUM(k->historia == ORGANUM_HISTORIA_SUBSTITUE);

    /* ligatio cum historia impelle */
    o = legere_campum(c->piscina, in,
        "<campus nomen=\"d\" typus=\"dies\" vita=\"persistens\""
        " semita=\"ligatio\" historia=\"impelle\"/>", &vitia);
    vitia_imprimere(vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    k = organum_campus(o, chorda_ex_literis("d", c->piscina));
    CREDO_VERUM(k->semita == ORGANUM_SEMITA_LIGATIO);
    CREDO_VERUM(k->historia == ORGANUM_HISTORIA_IMPELLE);
    CREDO_VERUM(k->ambitus == ORGANUM_AMBITUS_ORGANUM);

    /* status cum ambitu ligationis */
    o = legere_campum(c->piscina, in,
        "<campus nomen=\"p\" typus=\"numerus\" vita=\"persistens\""
        " semita=\"status\" ambitus=\"ligatio\""
        " historia=\"substitue\"/>", &vitia);
    vitia_imprimere(vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    k = organum_campus(o, chorda_ex_literis("p", c->piscina));
    CREDO_VERUM(k->semita == ORGANUM_SEMITA_STATUS);
    CREDO_VERUM(k->ambitus == ORGANUM_AMBITUS_LIGATIO);
    CREDO_VERUM(k->historia == ORGANUM_HISTORIA_SUBSTITUE);

    /* canon: valor ignotus */
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"x\" typus=\"numerus\" vita=\"persistens\""
        " semita=\"via\"/>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "semita", c->piscina));

    /* semitae R4: vita transiens licet (restitutio post initium et
     * semita res diversae) */
    o = legere_campum(c->piscina, in,
        "<campus nomen=\"t\" typus=\"dies\" vita=\"transiens\""
        " semita=\"ligatio\"/>", &vitia);
    vitia_imprimere(vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    k = organum_campus(o, chorda_ex_literis("t", c->piscina));
    CREDO_VERUM(k->semita == ORGANUM_SEMITA_LIGATIO);
    CREDO_VERUM(k->vita == ORGANUM_VITA_TRANSIENS);

    /* lex: semita non in sede motus */
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"m\" typus=\"numerus\" vita=\"transiens\""
        " rhythmus=\"mobilis\" sedes=\"motus\" semita=\"status\"/>",
        &vitia));
    CREDO_VERUM(vitia_continent(vitia, "'m'", c->piscina));
    CREDO_VERUM(vitia_continent(vitia, "semita in sede motus",
        c->piscina));

    /* lex: semita non vitae probationis */
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"q\" typus=\"numerus\" vita=\"probatio\""
        " semita=\"status\"/>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "'q'", c->piscina));
    CREDO_VERUM(vitia_continent(vitia,
        "semita vetita campo probationis",
        c->piscina));

    /* lex: ambitus et historia solum cum semita */
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"a\" typus=\"numerus\" vita=\"persistens\""
        " ambitus=\"ligatio\"/>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "ambitus", c->piscina));
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"h\" typus=\"numerus\" vita=\"persistens\""
        " historia=\"impelle\"/>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "historia", c->piscina));

    /* lex: ambitus solum semitae status (ligatio se ipsam non
     * resettit) */
    CREDO_NIHIL(legere_campum(c->piscina, in,
        "<campus nomen=\"l\" typus=\"numerus\" vita=\"persistens\""
        " semita=\"ligatio\" ambitus=\"ligatio\"/>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "'l'", c->piscina));
    CREDO_VERUM(vitia_continent(vitia, "ambitus", c->piscina));
}

interior vacuum
_probare_montationes (
    CredoContextus* c)
{
         InternamentumChorda* in;
                         Xar* vitia;
                     Organum* o;
    constans OrganumMontatio* m;

    in = internamentum_creare(c->piscina);
    o  = legere_literas(c->piscina, in,
        "<organum genus=\"probatio\" versio=\"1\">"
        "<status><campus nomen=\"x\" typus=\"numerus\""
        " vita=\"persistens\"/></status>"
        "<montationes>"
        "<montatio nomen=\"notae\" genus=\"charta\"/>"
        "<montatio nomen=\"index\" genus=\"album\"/>"
        "</montationes></organum>", &vitia);
    vitia_imprimere(vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    CREDO_AEQUALIS_I32(organum_numerus_montationum(o), II);
    m = organum_montatio_index(o, ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_CHORDA_AEQUALIS_LITERIS(m->titulus, "notae");
    CREDO_CHORDA_AEQUALIS_LITERIS(m->genus, "charta");
    m = organum_montatio(o, chorda_ex_literis("index", c->piscina));
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_CHORDA_AEQUALIS_LITERIS(m->genus, "album");
    CREDO_NIHIL(organum_montatio(o, chorda_ex_literis("nulla",
        c->piscina)));
    CREDO_NIHIL(organum_montatio_index(o, II));

    /* nomen bis: canon recusat */
    CREDO_NIHIL(legere_literas(c->piscina, in,
        "<organum genus=\"probatio\" versio=\"1\">"
        "<status><campus nomen=\"x\" typus=\"numerus\""
        " vita=\"persistens\"/></status>"
        "<montationes>"
        "<montatio nomen=\"notae\" genus=\"charta\"/>"
        "<montatio nomen=\"notae\" genus=\"album\"/>"
        "</montationes></organum>", &vitia));
    CREDO_VERUM(xar_numerus(vitia) > ZEPHYRUM);

    /* sine montationibus: nullae */
    o = legere_campum(c->piscina, in,
        "<campus nomen=\"x\" typus=\"numerus\" vita=\"persistens\"/>",
        &vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    CREDO_AEQUALIS_I32(organum_numerus_montationum(o), ZEPHYRUM);
    CREDO_NIHIL(organum_montatio_index(o, ZEPHYRUM));
}

/* semitae R4e: cognomina (fixum, typatum, neutrum) legibilia; leges
 * oneratoris; typus solus; valores symbolici typi dies */
interior vacuum
_probare_cognomina (
    CredoContextus* c)
{
         InternamentumChorda* in;
                         Xar* vitia;
                     Organum* o;
    constans OrganumCognomen* g;
      constans OrganumCampus* dies;
      constans OrganumCampus* numerus;

    in = internamentum_creare(c->piscina);
    o  = legere_literas(c->piscina, in,
        "<organum genus=\"probatio\" versio=\"1\">"
        "<status><campus nomen=\"d\" typus=\"dies\""
        " vita=\"transiens\" semita=\"ligatio\"/>"
        "<campus nomen=\"n\" typus=\"numerus\""
        " vita=\"persistens\"/></status>"
        "<cognomina>"
        "<cognomen nomen=\"agenda\" semita=\"probatio/x\"/>"
        "<cognomen typus=\"dies\" semita=\"probatio/{}\"/>"
        "<cognomen semita=\"probatio/\"/>"
        "</cognomina></organum>", &vitia);
    vitia_imprimere(vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    CREDO_AEQUALIS_I32(organum_numerus_cognominum(o), III);
    g = organum_cognomen_index(o, ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(g);
    CREDO_CHORDA_AEQUALIS_LITERIS(g->titulus, "agenda");
    CREDO_AEQUALIS_I32(g->typus.mensura, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(g->semita, "probatio/x");
    g = organum_cognomen_index(o, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(g->typus, "dies");
    CREDO_AEQUALIS_I32(g->titulus.mensura, ZEPHYRUM);
    g = organum_cognomen_index(o, II);
    CREDO_AEQUALIS_I32(g->titulus.mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(g->typus.mensura, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(g->semita, "probatio/");
    CREDO_NIHIL(organum_cognomen_index(o, III));

    /* leges oneratoris */
    CREDO_NIHIL(legere_literas(c->piscina, in,
        "<organum genus=\"probatio\" versio=\"1\"><status/>"
        "<cognomina><cognomen nomen=\"a\" typus=\"numerus\""
        " semita=\"probatio/{}\"/></cognomina></organum>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "fixum et typatum", c->piscina));
    CREDO_NIHIL(legere_literas(c->piscina, in,
        "<organum genus=\"probatio\" versio=\"1\"><status/>"
        "<cognomina><cognomen typus=\"numerus\""
        " semita=\"probatio/x\"/></cognomina></organum>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "typatum sine {}", c->piscina));
    CREDO_NIHIL(legere_literas(c->piscina, in,
        "<organum genus=\"probatio\" versio=\"1\"><status/>"
        "<cognomina><cognomen nomen=\"a\""
        " semita=\"probatio/{}\"/></cognomina></organum>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "{} sine typo", c->piscina));
    CREDO_NIHIL(legere_literas(c->piscina, in,
        "<organum genus=\"probatio\" versio=\"1\"><status/>"
        "<cognomina><cognomen nomen=\"a\""
        " semita=\"sancti/index\"/></cognomina></organum>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "alieni generis", c->piscina));
    /* canon: typus electio non admittitur */
    CREDO_NIHIL(legere_literas(c->piscina, in,
        "<organum genus=\"probatio\" versio=\"1\"><status/>"
        "<cognomina><cognomen typus=\"electio\""
        " semita=\"probatio/{}\"/></cognomina></organum>", &vitia));
    CREDO_VERUM(vitia_continent(vitia, "typus", c->piscina));

    /* typus solus */
    CREDO_VERUM(organum_typum_iudicare(chorda_ex_literis("numerus",
        c->piscina), chorda_ex_literis("7", c->piscina), c->piscina));
    CREDO_FALSUM(organum_typum_iudicare(chorda_ex_literis("numerus",
        c->piscina), chorda_ex_literis("x", c->piscina), c->piscina));
    CREDO_VERUM(organum_typum_iudicare(chorda_ex_literis("dies",
        c->piscina), chorda_ex_literis("hodie", c->piscina),
        c->piscina));
    CREDO_FALSUM(organum_typum_iudicare(chorda_ex_literis("electio",
        c->piscina), chorda_ex_literis("x", c->piscina), c->piscina));
    CREDO_FALSUM(organum_typum_iudicare(chorda_ex_literis("series",
        c->piscina), chorda_ex_literis("x", c->piscina), c->piscina));

    /* valores symbolici: dies solum */
    dies     = organum_campus(o, chorda_ex_literis("d", c->piscina));
    numerus  = organum_campus(o, chorda_ex_literis("n", c->piscina));
    CREDO_NECESSE(dies && numerus);
    CREDO_VERUM(organum_valor_symbolicus(dies,
        chorda_ex_literis("hodie",
        c->piscina)));
    CREDO_VERUM(organum_valor_symbolicus(dies, chorda_ex_literis("heri",
        c->piscina)));
    CREDO_VERUM(organum_valor_symbolicus(dies, chorda_ex_literis("cras",
        c->piscina)));
    CREDO_FALSUM(organum_valor_symbolicus(dies, chorda_ex_literis(
        "2026-10-19", c->piscina)));
    CREDO_FALSUM(organum_valor_symbolicus(numerus, chorda_ex_literis(
        "hodie", c->piscina)));
    CREDO_VERUM(organum_valorem_iudicare(dies, chorda_ex_literis("cras",
        c->piscina), c->piscina));
    CREDO_FALSUM(organum_valorem_iudicare(dies,
        chorda_ex_literis("nunc",
        c->piscina), c->piscina));
}

/* comites P5a: aditus (relatio visus) - iter 'quaestio' legibile
 * (semita, advocare; semita absens = latus ipsum); nomen ignotum et
 * iter iteratum recusantur; nullus declaratus = NIHIL */
interior vacuum
_probare_aditus (
    CredoContextus* c)
{
     InternamentumChorda* in;
                     Xar* vitia;
                 Organum* o;
    constans OrganumIter* it;

    in = internamentum_creare(c->piscina);
    o  = legere_literas(c->piscina, in,
        "<organum genus=\"probatio\" versio=\"1\"><status/>"
        "<aditus><iter nomen=\"quaestio\""
        " semita=\"indagatio?genus=res\" advocare=\"verum\"/>"
        "</aditus></organum>", &vitia);
    vitia_imprimere(vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    it = organum_iter(o, "quaestio");
    CREDO_NECESSE_NON_NIHIL(it);
    CREDO_CHORDA_AEQUALIS_LITERIS(it->titulus, "quaestio");
    CREDO_CHORDA_AEQUALIS_LITERIS(it->semita, "indagatio?genus=res");
    CREDO_VERUM(it->advocare);
    CREDO_NIHIL(organum_iter(o, "hodie"));
    /* semita absens: latus ipsum; advocare absens: FALSUM */
    o = legere_literas(c->piscina, in,
        "<organum genus=\"probatio\" versio=\"1\"><status/>"
        "<aditus><iter nomen=\"quaestio\"/></aditus></organum>",
        &vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    it = organum_iter(o, "quaestio");
    CREDO_NECESSE_NON_NIHIL(it);
    CREDO_AEQUALIS_I32(it->semita.mensura, ZEPHYRUM);
    CREDO_FALSUM(it->advocare);
    /* sine aditu: NIHIL */
    o = legere_literas(c->piscina, in,
        "<organum genus=\"probatio\" versio=\"1\"><status/>"
        "</organum>", &vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    CREDO_NIHIL(organum_iter(o, "quaestio"));
    /* canon: nomen ignotum, iter iteratum */
    CREDO_NIHIL(legere_literas(c->piscina, in,
        "<organum genus=\"probatio\" versio=\"1\"><status/>"
        "<aditus><iter nomen=\"ignotum\"/></aditus></organum>",
        &vitia));
    CREDO_NIHIL(legere_literas(c->piscina, in,
        "<organum genus=\"probatio\" versio=\"1\"><status/>"
        "<aditus><iter nomen=\"quaestio\"/><iter nomen=\"quaestio\"/>"
        "</aditus></organum>", &vitia));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "pictor",            _probare_pictorem,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "canon_organi",      _probare_canonem_organi,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "leges",             _probare_leges,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "canon_generatus",   _probare_canonem_generatum,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "domini",            _probare_dominos,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "basis",             _probare_basim,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "pictor_vivus",      _probare_pictorem_vivum,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "initium",           _probare_initium,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "scriba_viva",       _probare_scribam_vivam,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "semita",            _probare_semitam,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "cognomina",         _probare_cognomina,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "montationes",       _probare_montationes,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "aditus",            _probare_aditus,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("organum", SECTIONES);
}
