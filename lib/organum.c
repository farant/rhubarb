/* organum.c - manifestum organi: lectio, leges oneratoris, canon
 * insulae et domini generati (include/organum.h). */

#include "organum.h"
#include "organum_canon.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <string.h>

structura Organum {
    chorda  genus;
       Xar* campi;        /* Xar de OrganumCampus */
       Xar* ligamina;     /* Xar de OrganumLigamen */
       Xar* actiones;     /* Xar de OrganumActio */
       Xar* portae;       /* Xar de OrganumPorta */
       Xar* montationes;  /* Xar de OrganumMontatio (semitae R0) */
       Xar* cognomina;    /* Xar de OrganumCognomen (semitae R4e) */
};


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
vacua (
    vacuum)
{
    chorda c;

    c.mensura  = ZEPHYRUM;
    c.datum    = NIHIL;
    redde c;
}

/* attributum ut chorda; vacua si abest */
interior chorda
attributum (
             StmlNodus* nodus,
    constans character* titulus)
{
    chorda* a;

    a = stml_attributum_capere(nodus, titulus);
    redde a ? *a : vacua();
}

interior b32
attributum_est (
             StmlNodus* nodus,
    constans character* titulus,
    constans character* valor)
{
    chorda* a;

    a = stml_attributum_capere(nodus, titulus);
    redde a && chorda_aequalis_literis(*a, valor);
}

/* vitium nominatum: "linea N: campus 'x': nuntius" */
interior vacuum
vitium_addere (
                   Xar* vitia,
               Piscina* piscina,
             StmlNodus* nodus,
                chorda  titulus,
    constans character* nuntius)
{
    character buffer[DXII];

    sprintf(buffer, "linea %d: campus '%.*s': %s",
        (integer)(nodus ? nodus->linea : ZEPHYRUM),
        (integer)titulus.mensura, (constans character*)titulus.datum,
        nuntius);
    *(chorda*)xar_addere(vitia) = chorda_ex_literis(buffer, piscina);
}

/* semitae R4e: vitium cognominis (semita nominatur) */
interior vacuum
vitium_cognominis (
                   Xar* vitia,
               Piscina* piscina,
             StmlNodus* nodus,
                chorda  semita,
    constans character* nuntius)
{
    character buffer[DXII];

    sprintf(buffer, "linea %d: cognomen '%.*s': %s",
        (integer)(nodus ? nodus->linea : ZEPHYRUM),
        (integer)semita.mensura, (constans character*)semita.datum,
        nuntius);
    *(chorda*)xar_addere(vitia) = chorda_ex_literis(buffer, piscina);
}

/* liberi elementares titulo dato */
interior Xar*
liberi (
             StmlNodus* nodus,
    constans character* titulus,
               Piscina* piscina)
{
    redde stml_invenire_omnes_liberos(nodus, titulus, piscina);
}

interior StmlNodus*
liberum_ad (
    Xar* l,
    i32  i)
{
    redde *(StmlNodus**)xar_obtinere(l, i);
}


/* ==================================================
 * Valores typorum (absens)
 * ================================================== */

interior b32
est_numerus (
    chorda c)
{
    i32 i;

    i = ZEPHYRUM;
    si (   c.mensura > ZEPHYRUM && (c.datum[ZEPHYRUM] == '-'
        || c.datum[ZEPHYRUM] == '+'))
    {
        i = I;
    }
    si (i >= c.mensura)
    {
        redde FALSUM;
    }
    per (; i < c.mensura; i++)
    {
        si (c.datum[i] < '0' || c.datum[i] > '9')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* nomen canonis: [A-Za-z0-9_*]+ */
interior b32
est_nomen (
    chorda c)
{
    i32 i;

    si (c.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        character k;

        k = (character)c.datum[i];
        si (!((k >= 'a' && k <= 'z') || (k >= 'A' && k <= 'Z')
            || (k >= '0' && k <= '9') || k == '_' || k == '*'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* dies canonis: yyyy | yyyy-mm | yyyy-mm-dd (forma sola) */
interior b32
est_dies (
    chorda c)
{
    i32 i;

    si (c.mensura != IV && c.mensura != VII && c.mensura != X)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        b32 lineola;

        lineola = (i == IV || i == VII);
        si (lineola ? c.datum[i] != '-'
                    : (c.datum[i] < '0' || c.datum[i] > '9'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* semitae R4e: valores symbolici typi dies (ad limen resolvuntur) */
hic_manens constans character* constans symbola_diei[] = {
    "hodie", "heri", "cras", NIHIL
};

interior b32
symbolum_typi (
    chorda typus,
    chorda valor)
{
    i32 i;

    si (!chorda_aequalis_literis(typus, "dies"))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; symbola_diei[i]; i++)
    {
        si (chorda_aequalis_literis(valor, symbola_diei[i]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* valor typo campi congruit? (electio: inter optiones) - absens
 * manifesti et valores semitae (organum_valorem_iudicare) */
interior b32
valor_validum (
     constans OrganumCampus* k,
                     chorda  valor,
                        Xar* optiones,
                    Piscina* piscina)
{
    i32 i;

    si (chorda_aequalis_literis(k->typus, "numerus"))
    {
        redde est_numerus(valor);
    }
    si (chorda_aequalis_literis(k->typus, "veritas"))
    {
        redde chorda_aequalis_literis(valor, "verum")
            || chorda_aequalis_literis(valor, "falsum");
    }
    si (chorda_aequalis_literis(k->typus, "nomen"))
    {
        redde est_nomen(valor);
    }
    si (chorda_aequalis_literis(k->typus, "dies"))
    {
        redde est_dies(valor);
    }
    si (chorda_aequalis_literis(k->typus, "electio"))
    {
        per (i = ZEPHYRUM; i < xar_numerus(optiones); i++)
        {
            si (chorda_aequalis(stml_textus_normalizatus(
                    liberum_ad(optiones, i), piscina), valor))
            {
                redde VERUM;
            }
        }
        redde FALSUM;
    }
    redde VERUM;   /* textus, series */
}


/* ==================================================
 * Lectio
 * ================================================== */

/* ligamen declaratum? */
interior b32
ligamen_declaratum (
    constans Organum* o,
              chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(o->ligamina); i++)
    {
        si (chorda_aequalis(((OrganumLigamen*)xar_obtinere(o->ligamina,
                i))->titulus, titulus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* leges oneratoris super campum unum */
interior vacuum
legibus_iudicare (
          constans Organum* o,
    constans OrganumCampus* k,
                       Xar* vitia,
                   Piscina* piscina)
{
         Xar* optiones;
         b32  electio;
         i32  i;
         s32  sedes_lineolae;

    si (k->sedes == ORGANUM_SEDES_MOTUS)
    {
        si (k->rhythmus != ORGANUM_RHYTHMUS_MOBILIS)
        {
            vitium_addere(vitia, piscina, k->nodus, k->titulus,
                "sedes motus postulat rhythmus mobilis");
        }
        si (k->vita != ORGANUM_VITA_TRANSIENS)
        {
            vitium_addere(vitia, piscina, k->nodus, k->titulus,
                "sedes motus postulat vita transiens");
        }
    }
    si (   chorda_aequalis_literis(k->typus, "series")
        && k->sedes != ORGANUM_SEDES_MOTUS)
    {
        vitium_addere(vitia, piscina, k->nodus, k->titulus,
            "typus series solum sedis motus (insula series non fert)");
    }
    si (   k->orbatus.mensura > ZEPHYRUM
        && k->referentia.mensura == ZEPHYRUM)
    {
        vitium_addere(vitia, piscina, k->nodus, k->titulus,
            "orbatus sine referentia");
    }
    si (k->referentia.mensura > ZEPHYRUM)
    {
        sedes_lineolae = -I;
        per (i = ZEPHYRUM; i < k->referentia.mensura; i++)
        {
            si (k->referentia.datum[i] == '/')
            {
                sedes_lineolae = (s32)i;
                frange;
            }
        }
        si (   sedes_lineolae          <= ZEPHYRUM
            || (i32)sedes_lineolae + I >= k->referentia.mensura)
        {
            vitium_addere(vitia, piscina, k->nodus, k->titulus,
                "referentia formae 'ligamen/genus' esse debet");
        }
        alioquin
        {
               chorda prima;
            character buffer[CCLVI];

            prima.datum    = k->referentia.datum;
            prima.mensura  = (i32)sedes_lineolae;
            si (!ligamen_declaratum(o, prima))
            {
                sprintf(buffer, "referentia ligamen ignotum: %.*s",
                    (integer)prima.mensura,
                    (constans character*)prima.datum);
                vitium_addere(vitia, piscina, k->nodus, k->titulus,
                    buffer);
            }
        }
    }

    optiones  = liberi(k->nodus, "optio", piscina);
    electio   = chorda_aequalis_literis(k->typus, "electio");
    si (electio && xar_numerus(optiones) == ZEPHYRUM)
    {
        vitium_addere(vitia, piscina, k->nodus, k->titulus,
            "typus electio sine optione");
    }
    si (!electio && xar_numerus(optiones) > ZEPHYRUM)
    {
        vitium_addere(vitia, piscina, k->nodus, k->titulus,
            "optio sine typo electio");
    }
    /* semitae R4: semita non in sede motus, non vitae probationis
     * (transiens licet - restitutio post initium et semita res
     * diversae); R0: ambitus et historia solum cum semita; ambitus
     * solum statui (ligatio se ipsam non resettit) */
    si (   k->semita != ORGANUM_SEMITA_NULLA
        && k->sedes  == ORGANUM_SEDES_MOTUS)
    {
        vitium_addere(vitia, piscina, k->nodus, k->titulus,
            "semita in sede motus vetita");
    }
    si (   k->semita != ORGANUM_SEMITA_NULLA
        && k->vita   == ORGANUM_VITA_PROBATIO)
    {
        vitium_addere(vitia, piscina, k->nodus, k->titulus,
            "semita vetita campo probationis");
    }
    si (   k->semita == ORGANUM_SEMITA_NULLA
        && stml_attributum_capere(k->nodus, "ambitus"))
    {
        vitium_addere(vitia, piscina, k->nodus, k->titulus,
            "ambitus sine semita");
    }
    si (   k->semita == ORGANUM_SEMITA_NULLA
        && stml_attributum_capere(k->nodus, "historia"))
    {
        vitium_addere(vitia, piscina, k->nodus, k->titulus,
            "historia sine semita");
    }
    si (   k->semita == ORGANUM_SEMITA_LIGATIO
        && stml_attributum_capere(k->nodus, "ambitus"))
    {
        vitium_addere(vitia, piscina, k->nodus, k->titulus,
            "ambitus solum semitae status (ligatio se non resettit)");
    }
    si (   k->habet_absens
        && !valor_validum(k, k->absens, optiones, piscina))
    {
        character buffer[CCLVI];

        sprintf(buffer, "absens '%.*s' typo %.*s non congruit%s",
            (integer)k->absens.mensura,
            (constans character*)k->absens.datum,
            (integer)k->typus.mensura,
            (constans character*)k->typus.datum,
            electio ? " (non inter optiones)" : "");
        vitium_addere(vitia, piscina, k->nodus, k->titulus, buffer);
    }
}

interior vacuum
campum_legere (
    OrganumCampus* k,
        StmlNodus* n)
{
    chorda* a;

    memset(k, ZEPHYRUM, magnitudo(*k));
    k->nodus    = n;
    k->titulus  = attributum(n, "nomen");
    k->typus    = attributum(n, "typus");
    k->vita       = attributum_est(n, "vita", "transiens")
                  ? ORGANUM_VITA_TRANSIENS
                  : attributum_est(n, "vita", "probatio")
                  ? ORGANUM_VITA_PROBATIO : ORGANUM_VITA_PERSISTENS;
    k->rhythmus   = attributum_est(n, "rhythmus", "mobilis")
                  ? ORGANUM_RHYTHMUS_MOBILIS
                  : ORGANUM_RHYTHMUS_DISCRETUS;
    k->sedes      = attributum_est(n, "sedes", "motus")
                  ? ORGANUM_SEDES_MOTUS : ORGANUM_SEDES_INSULA;
    a                = stml_attributum_capere(n, "absens");
    k->habet_absens  = a != NIHIL;
    k->absens        = a ? *a : vacua();
    k->scriptor      = attributum(n, "scriptor");
    k->aspectus      = attributum(n, "aspectus");
    k->referentia    = attributum(n, "referentia");
    k->orbatus       = attributum(n, "orbatus");
    k->semita        = attributum_est(n, "semita", "ligatio")
                     ? ORGANUM_SEMITA_LIGATIO
                     : attributum_est(n, "semita", "status")
                     ? ORGANUM_SEMITA_STATUS : ORGANUM_SEMITA_NULLA;
    k->ambitus       = attributum_est(n, "ambitus", "ligatio")
                     ? ORGANUM_AMBITUS_LIGATIO
                     : ORGANUM_AMBITUS_ORGANUM;
    k->historia      = attributum_est(n, "historia", "impelle")
                     ? ORGANUM_HISTORIA_IMPELLE
                     : ORGANUM_HISTORIA_SUBSTITUE;
}

/* vitia canonis organi in nuntios */
interior b32
canone_iudicare (
              StmlNodus* radix,
                    Xar* vitia,
                Piscina* piscina,
    InternamentumChorda* intern)
{
        Canon* canon;
          Xar* cv;
       chorda  causa;
          i32  i;
    character  buffer[DXII];

    causa = vacua();
    canon = canon_legere(organum_canon_textus(piscina), piscina, intern,
        &causa);
    si (!canon)
    {
        sprintf(buffer, "canon organi infixus fractus: %.*s",
            (integer)causa.mensura, (constans character*)causa.datum);
        *(chorda*)xar_addere(vitia) = chorda_ex_literis(buffer,
            piscina);
        redde FALSUM;
    }
    cv = canon_iudicare(canon, radix, piscina);
    per (i = ZEPHYRUM; i < xar_numerus(cv); i++)
    {
        CanonVitium* v;

        v = (CanonVitium*)xar_obtinere(cv, i);
        sprintf(buffer, "linea %d: <%.*s> %s%s%.*s",
            (integer)(v->nodus ? v->nodus->linea : ZEPHYRUM),
            (integer)(v->elementum ? v->elementum->mensura : ZEPHYRUM),
            v->elementum ? (constans character*)v->elementum->datum
                         : "",
            canon_nuntius(v->genus),
            v->detail ? ": " : "",
            (integer)(v->detail ? v->detail->mensura : ZEPHYRUM),
            v->detail ? (constans character*)v->detail->datum : "");
        *(chorda*)xar_addere(vitia) = chorda_ex_literis(buffer,
            piscina);
    }
    redde xar_numerus(cv) == ZEPHYRUM;
}

Organum*
organum_legere (
                 chorda   fons,
                Piscina*  piscina,
    InternamentumChorda*  intern,
                    Xar** vitia)
{
    StmlResultus  r;
         Organum* o;
       StmlNodus* radix;
       StmlNodus* sectio;
             Xar* l;
             Xar* v;
             i32  i;
       character  buffer[DXII];

    v = xar_creare(piscina, (i32)magnitudo(chorda));
    si (vitia)
    {
        *vitia = v;
    }
    r = stml_legere(fons, piscina, intern);
    si (!r.successus || !r.elementum_radix)
    {
        sprintf(buffer, "linea %d: STML non legitur: %.*s",
            (integer)r.linea_erroris, (integer)r.error.mensura,
            (constans character*)r.error.datum);
        *(chorda*)xar_addere(v) = chorda_ex_literis(buffer, piscina);
        redde NIHIL;
    }
    si (!canone_iudicare(r.radix, v, piscina, intern))
    {
        redde NIHIL;
    }

    radix = r.elementum_radix;
    o = (Organum*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(Organum));
    o->genus     = attributum(radix, "genus");
    o->campi     = xar_creare(piscina, (i32)magnitudo(OrganumCampus));
    o->ligamina  = xar_creare(piscina, (i32)magnitudo(OrganumLigamen));
    o->actiones  = xar_creare(piscina, (i32)magnitudo(OrganumActio));
    o->portae    = xar_creare(piscina, (i32)magnitudo(OrganumPorta));
    o->montationes = xar_creare(piscina,
        (i32)magnitudo(OrganumMontatio));
    o->cognomina = xar_creare(piscina, (i32)magnitudo(OrganumCognomen));

    sectio  = stml_invenire_liberum(radix, "ligamina");
    l       = sectio ? liberi(sectio, "ligamen", piscina) : NIHIL;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        OrganumLigamen* g;

        g             = (OrganumLigamen*)xar_addere(o->ligamina);
        g->titulus    = attributum(liberum_ad(l, i), "nomen");
        g->genus      = attributum(liberum_ad(l, i), "genus");
        g->a_parente  = attributum_est(liberum_ad(l, i), "a", "parens");
    }
    sectio  = stml_invenire_liberum(radix, "portae");
    l       = sectio ? liberi(sectio, "porta", piscina) : NIHIL;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        OrganumPorta* q;

        q           = (OrganumPorta*)xar_addere(o->portae);
        q->titulus  = attributum(liberum_ad(l, i), "nomen");
        q->modus    = attributum(liberum_ad(l, i), "modus");
    }
    sectio  = stml_invenire_liberum(radix, "actiones");
    l       = sectio ? liberi(sectio, "actio", piscina) : NIHIL;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        OrganumActio* a;

        a           = (OrganumActio*)xar_addere(o->actiones);
        a->titulus  = attributum(liberum_ad(l, i), "nomen");
        a->systema = attributum_est(liberum_ad(l, i), "systema",
            "verum");
    }
    sectio  = stml_invenire_liberum(radix, "montationes");
    l       = sectio ? liberi(sectio, "montatio", piscina) : NIHIL;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        OrganumMontatio* m;

        m           = (OrganumMontatio*)xar_addere(o->montationes);
        m->titulus  = attributum(liberum_ad(l, i), "nomen");
        m->genus    = attributum(liberum_ad(l, i), "genus");
    }
    /* semitae R4e: cognomina; leges: fixum et typatum non simul,
     * typatum cum "{}", "{}" solum typato, semita generis proprii */
    sectio  = stml_invenire_liberum(radix, "cognomina");
    l       = sectio ? liberi(sectio, "cognomen", piscina) : NIHIL;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
         OrganumCognomen* g;
               StmlNodus* n;
                     b32  locus;

        n           = liberum_ad(l, i);
        g           = (OrganumCognomen*)xar_addere(o->cognomina);
        g->titulus  = attributum(n, "nomen");
        g->typus    = attributum(n, "typus");
        g->semita   = attributum(n, "semita");
        locus       = chorda_continet(g->semita, chorda_ex_literis("{}",
            piscina));
        si (   g->titulus.mensura > ZEPHYRUM
            && g->typus.mensura > ZEPHYRUM)
        {
            vitium_cognominis(v, piscina, n, g->semita,
                "cognomen fixum et typatum simul");
        }
        si (g->typus.mensura > ZEPHYRUM && !locus)
        {
            vitium_cognominis(v, piscina, n, g->semita,
                "cognomen typatum sine {}");
        }
        si (g->typus.mensura == ZEPHYRUM && locus)
        {
            vitium_cognominis(v, piscina, n, g->semita, "{} sine typo");
        }
        si (   g->semita.mensura < o->genus.mensura
            || memcmp(g->semita.datum, o->genus.datum,
                   (size_t)o->genus.mensura) != ZEPHYRUM
            || (   g->semita.mensura > o->genus.mensura
                && g->semita.datum[o->genus.mensura] != '/'
                && g->semita.datum[o->genus.mensura] != '?'))
        {
            vitium_cognominis(v, piscina, n, g->semita,
                "cognomen alieni generis");
        }
    }
    sectio  = stml_invenire_liberum(radix, "status");
    l       = sectio ? liberi(sectio, "campus", piscina) : NIHIL;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        campum_legere((OrganumCampus*)xar_addere(o->campi),
            liberum_ad(l, i));
    }
    per (i = ZEPHYRUM; i < xar_numerus(o->campi); i++)
    {
        legibus_iudicare(o, (OrganumCampus*)xar_obtinere(o->campi, i),
            v,
            piscina);
    }
    redde xar_numerus(v) == ZEPHYRUM ? o : NIHIL;
}

b32
organum_basim_addere (
             Organum* organum,
    constans Organum* basis)
{
    Xar* novi;
    i32  i;
    i32  j;

    si (!organum || !basis)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(basis->campi); i++)
    {
        si (organum_campus(organum, ((OrganumCampus*)xar_obtinere(
                basis->campi, i))->titulus))
        {
            redde FALSUM;
        }
    }
    /* basis ANTE proprios: Xar novus */
    novi = xar_creare(organum->campi->piscina,
        (i32)magnitudo(OrganumCampus));
    per (i = ZEPHYRUM; i < xar_numerus(basis->campi); i++)
    {
        *(OrganumCampus*)xar_addere(novi) =
            *(OrganumCampus*)xar_obtinere(basis->campi, i);
    }
    per (i = ZEPHYRUM; i < xar_numerus(organum->campi); i++)
    {
        *(OrganumCampus*)xar_addere(novi) =
            *(OrganumCampus*)xar_obtinere(organum->campi, i);
    }
    organum->campi = novi;
    /* actiones basis quae desunt */
    per (i = ZEPHYRUM; i < xar_numerus(basis->actiones); i++)
    {
        OrganumActio* a;
                 b32  adest;

        a      = (OrganumActio*)xar_obtinere(basis->actiones, i);
        adest  = FALSUM;
        per (j = ZEPHYRUM; j < xar_numerus(organum->actiones); j++)
        {
            si (chorda_aequalis(((OrganumActio*)xar_obtinere(
                    organum->actiones, j))->titulus, a->titulus))
            {
                adest = VERUM;
            }
        }
        si (!adest)
        {
            *(OrganumActio*)xar_addere(organum->actiones) = *a;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Quaestiones
 * ================================================== */

chorda
organum_genus (
    constans Organum* organum)
{
    redde organum ? organum->genus : vacua();
}

i32
organum_numerus_camporum (
    constans Organum* organum)
{
    redde organum ? xar_numerus(organum->campi) : ZEPHYRUM;
}

constans OrganumCampus*
organum_campus_index (
     constans Organum* organum,
                  i32  index)
{
    si (!organum || index >= xar_numerus(organum->campi))
    {
        redde NIHIL;
    }
    redde (constans OrganumCampus*)xar_obtinere(organum->campi, index);
}

constans OrganumCampus*
organum_campus (
     constans Organum* organum,
               chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; organum && i < xar_numerus(organum->campi); i++)
    {
        constans OrganumCampus* k;

        k = (constans OrganumCampus*)xar_obtinere(organum->campi, i);
        si (chorda_aequalis(k->titulus, titulus))
        {
            redde k;
        }
    }
    redde NIHIL;
}

i32
organum_numerus_ligaminum (
    constans Organum* organum)
{
    redde organum ? xar_numerus(organum->ligamina) : ZEPHYRUM;
}

constans OrganumLigamen*
organum_ligamen_index (
     constans Organum* organum,
                  i32  index)
{
    si (!organum || index >= xar_numerus(organum->ligamina))
    {
        redde NIHIL;
    }
    redde (constans OrganumLigamen*)xar_obtinere(organum->ligamina,
        index);
}

i32
organum_numerus_actionum (
    constans Organum* organum)
{
    redde organum ? xar_numerus(organum->actiones) : ZEPHYRUM;
}

constans OrganumActio*
organum_actio_index (
     constans Organum* organum,
                  i32  index)
{
    si (!organum || index >= xar_numerus(organum->actiones))
    {
        redde NIHIL;
    }
    redde (constans OrganumActio*)xar_obtinere(organum->actiones,
        index);
}

i32
organum_numerus_portarum (
    constans Organum* organum)
{
    redde organum ? xar_numerus(organum->portae) : ZEPHYRUM;
}

constans OrganumPorta*
organum_porta_index (
     constans Organum* organum,
                  i32  index)
{
    si (!organum || index >= xar_numerus(organum->portae))
    {
        redde NIHIL;
    }
    redde (constans OrganumPorta*)xar_obtinere(organum->portae, index);
}

i32
organum_numerus_montationum (
    constans Organum* organum)
{
    redde organum ? xar_numerus(organum->montationes) : ZEPHYRUM;
}

constans OrganumMontatio*
organum_montatio_index (
     constans Organum* organum,
                  i32  index)
{
    si (!organum || index >= xar_numerus(organum->montationes))
    {
        redde NIHIL;
    }
    redde (constans OrganumMontatio*)xar_obtinere(organum->montationes,
        index);
}

constans OrganumMontatio*
organum_montatio (
     constans Organum* organum,
               chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; organum && i < xar_numerus(organum->montationes);
         i++)
    {
        constans OrganumMontatio* m;
                             Xar* omnes;

        omnes  = organum->montationes;
        m      = (constans OrganumMontatio*)xar_obtinere(omnes, i);
        si (chorda_aequalis(m->titulus, titulus))
        {
            redde m;
        }
    }
    redde NIHIL;
}

i32
organum_numerus_cognominum (
    constans Organum* organum)
{
    redde (organum && organum->cognomina)
        ? xar_numerus(organum->cognomina) : ZEPHYRUM;
}

constans OrganumCognomen*
organum_cognomen_index (
     constans Organum* organum,
                  i32  index)
{
    si (   !organum || !organum->cognomina
        || index >= xar_numerus(organum->cognomina))
    {
        redde NIHIL;
    }
    redde (constans OrganumCognomen*)xar_obtinere(organum->cognomina,
        index);
}

b32
organum_typum_iudicare (
      chorda  typus,
      chorda  valor,
     Piscina* piscina)
{
    OrganumCampus k;

    si (   chorda_aequalis_literis(typus, "electio")
        || chorda_aequalis_literis(typus, "series"))
    {
        redde FALSUM;
    }
    si (symbolum_typi(typus, valor))
    {
        redde VERUM;
    }
    memset(&k, ZEPHYRUM, magnitudo(k));
    k.typus = typus;
    redde valor_validum(&k, valor, NIHIL, piscina);
}

b32
organum_valor_symbolicus (
    constans OrganumCampus* campus,
                    chorda  valor)
{
    redde campus && symbolum_typi(campus->typus, valor);
}

b32
organum_valorem_iudicare (
     constans OrganumCampus* campus,
                     chorda  valor,
                    Piscina* piscina)
{
    si (!campus)
    {
        redde FALSUM;
    }
    /* semitae R4e: valor symbolicus typi (resolvendus ad limen) */
    si (symbolum_typi(campus->typus, valor))
    {
        redde VERUM;
    }
    redde valor_validum(campus, valor, campus->nodus ? liberi(
        campus->nodus, "optio", piscina) : NIHIL, piscina);
}


/* ==================================================
 * Generatio
 * ================================================== */

chorda
organum_initium (
    constans Organum* organum,
             Piscina* piscina)
{
    ChordaAedificator* a;
                  i32  i;

    si (!organum)
    {
        redde vacua();
    }
    a = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
    per (i = ZEPHYRUM; i < xar_numerus(organum->campi); i++)
    {
        constans OrganumCampus* k;

        k = (constans OrganumCampus*)xar_obtinere(organum->campi, i);
        si (!k->habet_absens || k->sedes != ORGANUM_SEDES_INSULA)
        {
            perge;
        }
        chorda_aedificator_appendere_literis(a, " ");
        chorda_aedificator_appendere_chorda(a, k->titulus);
        chorda_aedificator_appendere_literis(a, "=\"");
        chorda_aedificator_appendere_chorda(a, k->absens);
        chorda_aedificator_appendere_literis(a, "\"");
    }
    redde chorda_aedificator_finire(a);
}

/* textus effugitus pro <optio> (& < >) */
interior vacuum
effugitum_appendere (
    ChordaAedificator* a,
               chorda  c)
{
    i32 i;

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        commutatio (c.datum[i])
        {
            casus '&': chorda_aedificator_appendere_literis(a, "&amp;");
                       frange;
            casus '<': chorda_aedificator_appendere_literis(a, "&lt;");
                       frange;
            casus '>': chorda_aedificator_appendere_literis(a, "&gt;");
                       frange;
            ordinarius:
                chorda_aedificator_appendere_character(a,
                    (character)c.datum[i]);
                frange;
        }
    }
}

Canon*
organum_canonem_generare (
       constans Organum* organum,
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda* causa)
{
    ChordaAedificator* a;
                  i32  i;
                  i32  j;

    si (!organum)
    {
        redde NIHIL;
    }
    a = chorda_aedificator_creare(piscina, (memoriae_index)MMXLVIII);
    chorda_aedificator_appendere_literis(a, "<canon dialectus=\"");
    chorda_aedificator_appendere_chorda(a, organum->genus);
    chorda_aedificator_appendere_literis(a,
        "-insula\" versio=\"1\"><elementum nomen=\"");
    chorda_aedificator_appendere_chorda(a, organum->genus);
    chorda_aedificator_appendere_literis(a, "\" radix=\"verum\">");
    per (i = ZEPHYRUM; i < xar_numerus(organum->campi); i++)
    {
        constans OrganumCampus* k;
                           Xar* optiones;

        k = (constans OrganumCampus*)xar_obtinere(organum->campi, i);
        si (k->sedes != ORGANUM_SEDES_INSULA)
        {
            perge;
        }
        chorda_aedificator_appendere_literis(a, "<attributum nomen=\"");
        chorda_aedificator_appendere_chorda(a, k->titulus);
        chorda_aedificator_appendere_literis(a, "\" genus=\"");
        chorda_aedificator_appendere_chorda(a, k->typus);
        si (!chorda_aequalis_literis(k->typus, "electio"))
        {
            chorda_aedificator_appendere_literis(a, "\"/>");
            perge;
        }
        chorda_aedificator_appendere_literis(a, "\">");
        optiones = liberi(k->nodus, "optio", piscina);
        per (j = ZEPHYRUM; j < xar_numerus(optiones); j++)
        {
            chorda_aedificator_appendere_literis(a, "<optio>");
            effugitum_appendere(a, stml_textus_normalizatus(
                liberum_ad(optiones, j), piscina));
            chorda_aedificator_appendere_literis(a, "</optio>");
        }
        chorda_aedificator_appendere_literis(a, "</attributum>");
    }
    chorda_aedificator_appendere_literis(a, "</elementum></canon>");
    redde canon_legere(chorda_aedificator_finire(a), piscina, intern,
        causa);
}

StmlNodus*
organum_dominos_generare (
       constans Organum* organum,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    StmlNodus* domini;
          i32  i;

    si (!organum)
    {
        redde NIHIL;
    }
    domini = stml_elementum_creare(piscina, intern, "domini");
    per (i = ZEPHYRUM; i < xar_numerus(organum->campi); i++)
    {
        constans OrganumCampus* k;
                     StmlNodus* d;

        k = (constans OrganumCampus*)xar_obtinere(organum->campi, i);
        si (   k->sedes            != ORGANUM_SEDES_INSULA
            || k->scriptor.mensura == ZEPHYRUM)
        {
            perge;
        }
        d = stml_elementum_creare(piscina, intern, "dominus");
        (vacuum)stml_attributum_addere_chorda(d, piscina, intern,
            "attributum", k->titulus);
        (vacuum)stml_attributum_addere_chorda(d, piscina, intern,
            "scriptor", k->scriptor);
        /* S5a: referentia - systema relatio quoque reparat; S5c:
         * aspectus popover - systema dimissio claudit */
        {
            constans character* systemata;
                           b32  rel;
                           b32  pop;

            rel = k->referentia.mensura > ZEPHYRUM;
            pop = chorda_aequalis_literis(k->aspectus, "popover");
            systemata = (rel && pop) ? "relatio dimissio"
                      : rel ? "relatio" : pop ? "dimissio" : NIHIL;
            si (systemata)
            {
                (vacuum)stml_attributum_addere(d, piscina, intern,
                    "systemata", systemata);
            }
        }
        (vacuum)stml_liberum_addere(domini, d);
    }
    redde domini;
}
