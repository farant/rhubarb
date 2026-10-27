/* probatio_communio_servus.c - communio_servus (communio-plan-2):
 * T3 registrum apparatuum - membra spatii, apparatus per digestum
 * tesserae (tessera ipsa numquam servatur), verificatio, revocatio,
 * replica ligata. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"
#include "cives.h"
#include "moneta.h"
#include "scrinium.h"
#include "communio_servus.h"
#include "credo.h"
#include <string.h>

nomen structura {
           Volumen* volumen;
             Cives* cives;
    CommunioServus* servus;
            chorda  franciscus;
            chorda  amanda;
} Mundus;

hic_manens Piscina* piscina;

interior chorda
lit (
    constans character* s)
{
    redde chorda_ex_literis(s, piscina);
}

interior vacuum
_parare (
    CredoContextus* c)
{
    Mundus* m;

    piscina  = c->piscina;
    m        = (Mundus*)piscina_allocare(piscina, magnitudo(Mundus));
    memset(m, ZEPHYRUM, magnitudo(Mundus));
    c->datum = m;
    m->volumen = volumen_temporarium(piscina,
        "probatio_communio_servus");
    si (!m->volumen)
    {
        redde;
    }
    m->cives = cives_aperire(m->volumen, NIHIL, piscina);
    si (!m->cives || !volumen_transactionem_incipere(m->volumen))
    {
        redde;
    }
    (vacuum)cives_creare(m->cives, lit("franciscus"), lit("Franciscus"),
        &m->franciscus, piscina);
    (vacuum)cives_creare(m->cives, lit("amanda"), lit("Amanda"),
        &m->amanda, piscina);
    (vacuum)volumen_transactionem_committere(m->volumen);
    m->servus = communio_servum_creare(piscina, m->volumen, m->cives,
        NIHIL);
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    si (m && m->volumen)
    {
        volumen_claudere(m->volumen);
    }
    piscina = NIHIL;
}

/* apparatum Francisci in familia: tessera et id in exitus */
interior b32
apparatum_primum (
       Mundus* m,
    character* tessera,
    character* id)
{
    character digestum[LXV];

    redde communio_membrum_addere(m->servus, lit("familia"),
               m->franciscus)
        && communio_tesseram_facere(tessera, digestum)
        && communio_apparatum_addere(m->servus, m->franciscus,
               lit("familia"), lit("Macintosh"), lit(digestum), id);
}

interior vacuum
_sectio_apparatus (
    CredoContextus* c)
{
               Mundus* m;
            character  tessera[XLIV];
            character  alia[XLIV];
            character  digestum[LXV];
            character  id[MONETA_ULID_MENSURA];
    CommunioApparatus  a;
               chorda  t;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    CREDO_NECESSE(apparatum_primum(m, tessera, id));
    CREDO_AEQUALIS_I32((i32)strlen(tessera), XLIII);
    CREDO_AEQUALIS_I32((i32)strlen(id), XXVI);

    CREDO_GRADUS("tessera vera");
    CREDO_VERUM(communio_apparatum_verificare(m->servus, lit(tessera),
        &a, piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(a.id, id);
    CREDO_CHORDA_AEQUALIS(a.civis, m->franciscus);
    CREDO_CHORDA_AEQUALIS_LITERIS(a.spatium, "familia");
    CREDO_CHORDA_AEQUALIS_LITERIS(a.titulus, "Macintosh");
    CREDO_AEQUALIS_I32(a.replica.mensura, ZEPHYRUM);
    CREDO_FALSUM(a.revocatus);

    CREDO_GRADUS("tesserae falsae");
    CREDO_NECESSE(communio_tesseram_facere(alia, digestum));
    CREDO_FALSUM(communio_apparatum_verificare(m->servus, lit(alia), &a,
        piscina));
    memcpy(alia, tessera, (size_t)XLIV);
    alia[XLII] = alia[XLII] == 'A' ? 'B' : 'A';
    CREDO_FALSUM(communio_apparatum_verificare(m->servus, lit(alia), &a,
        piscina));
    t          = lit(tessera);
    t.mensura  = XLII;
    CREDO_FALSUM(communio_apparatum_verificare(m->servus, t, &a,
        piscina));
    CREDO_FALSUM(communio_apparatum_verificare(m->servus, lit(""), &a,
        piscina));
}

interior vacuum
_sectio_revocatus (
    CredoContextus* c)
{
               Mundus* m;
            character  tessera[XLIV];
            character  id[MONETA_ULID_MENSURA];
    CommunioApparatus  a;
                  Xar* l;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    CREDO_NECESSE(apparatum_primum(m, tessera, id));
    CREDO_VERUM(communio_apparatum_revocare(m->servus, lit(id)));
    CREDO_FALSUM(communio_apparatum_verificare(m->servus, lit(tessera),
        &a, piscina));
    l = communio_apparatus_enumerare(m->servus, piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), I);
    CREDO_VERUM(((CommunioApparatus*)xar_obtinere(l, ZEPHYRUM))
        ->revocatus);
    CREDO_FALSUM(communio_apparatum_revocare(m->servus,
        lit("01ZZZZZZZZZZZZZZZZZZZZZZZZ")));
}

interior vacuum
_sectio_suspensa (
    CredoContextus* c)
{
               Mundus* m;
            character  tessera[XLIV];
            character  id[MONETA_ULID_MENSURA];
    CommunioApparatus  a;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    CREDO_NECESSE(apparatum_primum(m, tessera, id));
    CREDO_NECESSE(volumen_transactionem_incipere(m->volumen));
    CREDO_NECESSE(cives_statum_ponere(m->cives, m->franciscus,
        CIVIS_SUSPENSUS));
    CREDO_NECESSE(volumen_transactionem_committere(m->volumen));
    CREDO_FALSUM(communio_apparatum_verificare(m->servus, lit(tessera),
        &a, piscina));
}

interior vacuum
_sectio_non_membrum (
    CredoContextus* c)
{
       Mundus* m;
    character  tessera[XLIV];
    character  digestum[LXV];
    character  id[MONETA_ULID_MENSURA];

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    CREDO_NECESSE(communio_membrum_addere(m->servus, lit("familia"),
        m->franciscus));
    CREDO_NECESSE(communio_tesseram_facere(tessera, digestum));
    CREDO_FALSUM(communio_apparatum_addere(m->servus, m->amanda,
        lit("familia"), lit("iPhone"), lit(digestum), id));
    CREDO_FALSUM(communio_apparatum_addere(m->servus, m->franciscus,
        lit("alia"), lit("iPhone"), lit(digestum), id));
    CREDO_FALSUM(communio_apparatum_addere(m->servus,
        lit("01ZZZZZZZZZZZZZZZZZZZZZZZZ"), lit("familia"),
        lit("iPhone"),
        lit(digestum), id));
    CREDO_FALSUM(communio_membrum_addere(m->servus, lit("familia"),
        lit("01ZZZZZZZZZZZZZZZZZZZZZZZZ")));

    CREDO_GRADUS("membrum iteratum: idem");
    CREDO_VERUM(communio_membrum_addere(m->servus, lit("familia"),
        m->franciscus));
    CREDO_VERUM(communio_membrum_addere(m->servus, lit("familia"),
        m->amanda));
    CREDO_VERUM(communio_apparatum_addere(m->servus, m->amanda,
        lit("familia"), lit("iPhone"), lit(digestum), id));
}

interior vacuum
_sectio_digestum (
    CredoContextus* c)
{
       Mundus* m;
    character  tessera[XLIV];
    character  digestum[LXV];
    character  id[MONETA_ULID_MENSURA];
    character  maiusculum[LXV];
          i32  i;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    CREDO_NECESSE(communio_membrum_addere(m->servus, lit("familia"),
        m->franciscus));
    CREDO_NECESSE(communio_tesseram_facere(tessera, digestum));
    CREDO_NECESSE(communio_apparatum_addere(m->servus, m->franciscus,
        lit("familia"), lit("Macintosh"), lit(digestum), id));
    CREDO_FALSUM(communio_apparatum_addere(m->servus, m->franciscus,
        lit("familia"), lit("Alius"), lit(digestum), id));

    CREDO_GRADUS("digestum malum");
    CREDO_NECESSE(communio_tesseram_facere(tessera, digestum));
    memcpy(maiusculum, digestum, (size_t)LXV);
    per (i = ZEPHYRUM; i < LXIV; i++)
    {
        si (maiusculum[i] >= 'a' && maiusculum[i] <= 'f')
        {
            maiusculum[i] = (character)(maiusculum[i] - 'a' + 'A');
        }
    }
    CREDO_FALSUM(communio_apparatum_addere(m->servus, m->franciscus,
        lit("familia"), lit("Alius"), lit(maiusculum), id));
    digestum[LXIII] = '\0';
    CREDO_FALSUM(communio_apparatum_addere(m->servus, m->franciscus,
        lit("familia"), lit("Alius"), lit(digestum), id));
}

interior vacuum
_sectio_ligare (
    CredoContextus* c)
{
       Mundus* m;
    character  tessera[XLIV];
    character  digestum[LXV];
    character  primum[MONETA_ULID_MENSURA];
    character  secundum[MONETA_ULID_MENSURA];
    character  r1[MONETA_ULID_MENSURA];
    character  r2[MONETA_ULID_MENSURA];

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    CREDO_NECESSE(apparatum_primum(m, tessera, primum));
    CREDO_NECESSE(communio_tesseram_facere(tessera, digestum));
    CREDO_NECESSE(communio_apparatum_addere(m->servus, m->franciscus,
        lit("familia"), lit("iPhone"), lit(digestum), secundum));
    moneta_ulid(r1);
    moneta_ulid(r2);
    CREDO_AEQUALIS_I32((i32)communio_replicam_ligare(m->servus,
        lit(primum), lit(r1)), ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)communio_replicam_ligare(m->servus,
        lit(primum), lit(r1)), ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)communio_replicam_ligare(m->servus,
        lit(primum), lit(r2)), I);
    CREDO_AEQUALIS_I32((i32)communio_replicam_ligare(m->servus,
        lit(secundum), lit(r1)), II);
    CREDO_VERUM(communio_replicam_ligare(m->servus, lit(secundum),
        lit("minuscula")) < ZEPHYRUM);
    CREDO_VERUM(communio_replicam_ligare(m->servus,
        lit("01ZZZZZZZZZZZZZZZZZZZZZZZZ"), lit(r2)) < ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)communio_replicam_ligare(m->servus,
        lit(secundum), lit(r2)), ZEPHYRUM);
}

/* chorda continet subchordam? */
interior b32
continet (
     chorda c,
     chorda sub)
{
    i32 i;

    per (i = ZEPHYRUM; sub.mensura > ZEPHYRUM
         && i + sub.mensura <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, sub.datum, (size_t)sub.mensura)
            == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_sectio_secretum (
    CredoContextus* c)
{
                Mundus* m;
             character  tessera[XLIV];
             character  id[MONETA_ULID_MENSURA];
             character  r1[MONETA_ULID_MENSURA];
     CommunioApparatus  a;
                   Xar* acta;
    ScriniumEnuntiatum* e;
                   i32  i;
                   i32  k;
                   i32  additi;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    CREDO_NECESSE(apparatum_primum(m, tessera, id));
    moneta_ulid(r1);
    (vacuum)communio_replicam_ligare(m->servus, lit(id), lit(r1));
    (vacuum)communio_apparatum_verificare(m->servus, lit(tessera), &a,
        piscina);
    (vacuum)communio_apparatum_revocare(m->servus, lit(id));

    CREDO_GRADUS("acta: nec tessera nec digestum");
    acta = volumen_acta_legere(m->volumen, ZEPHYRUM, piscina);
    CREDO_NECESSE_NON_NIHIL(acta);
    additi = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(acta); i++)
    {
        VolumenActum* x = (VolumenActum*)xar_obtinere(acta, i);

        CREDO_FALSUM(continet(x->datum, lit(tessera)));
        si (chorda_aequalis_literis(x->genus,
                "communio.apparatus-additus"))
        {
            additi++;
            CREDO_FALSUM(continet(x->datum, lit("digestum")));
        }
    }
    CREDO_AEQUALIS_I32(additi, I);

    CREDO_GRADUS("tabula: tessera nusquam");
    e = scrinium_praeparare(volumen_scrinium(m->volumen),
        "SELECT id, civis, spatium, titulus, digestum, replica"
        " FROM communio_apparatus");
    CREDO_NECESSE_NON_NIHIL(e);
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        per (k = ZEPHYRUM; k < VI; k++)
        {
            CREDO_FALSUM(continet(scrinium_columna_textus(e, (integer)k,
                piscina), lit(tessera)));
        }
    }
    scrinium_finire(e);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "apparatus", _sectio_apparatus, _parare, _purgare, NIHIL, NIHIL },
    { "revocatus", _sectio_revocatus, _parare, _purgare, NIHIL, NIHIL },
    { "persona suspensa", _sectio_suspensa, _parare, _purgare, NIHIL,
        NIHIL },
    { "non membrum", _sectio_non_membrum, _parare, _purgare, NIHIL,
        NIHIL },
    { "digestum iteratum et malum", _sectio_digestum, _parare, _purgare,
        NIHIL, NIHIL },
    { "ligare", _sectio_ligare, _parare, _purgare, NIHIL, NIHIL },
    { "secretum nusquam", _sectio_secretum, _parare, _purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("communio_servus", SECTIONES);
}
