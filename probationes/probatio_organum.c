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
        g = stml_attributum_capere(d, "genus");
        CREDO_VERUM(g && chorda_aequalis_literis(*g, "ephemera"));
    }
    /* XI campi insulae cum scriptore sine aspectu */
    CREDO_AEQUALIS_S32((s32)numerus, XI);
    CREDO_NON_NIHIL(scriptor_domini(domini, "zoom"));
    CREDO_CHORDA_AEQUALIS_LITERIS(*scriptor_domini(domini,
        "stratum_activum"), "strata.agere");
    /* S5a: referentia -> systema relatio; ceteri sine systematibus */
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
        alioquin
        {
            CREDO_NIHIL(sy);
        }
    }
    /* aspectus systematis est */
    CREDO_NIHIL(scriptor_domini(domini, "palette"));
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
        CREDO_NON_NIHIL(vetus);
        si (vetus)
        {
            CREDO_CHORDA_AEQUALIS(*vetus,
                *stml_attributum_capere(d, "scriptor"));
        }
        novi++;
    }
    CREDO_AEQUALIS_S32(novi, XVI);

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
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("organum", SECTIONES);
}
