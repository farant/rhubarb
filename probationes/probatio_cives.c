/* probatio_cives.c - cives (ianua T1): personae rationum - nomina
 * (regula, normatio), creatio cum ULID, renominatio (id manet, nomen
 * vetus reservatum), deletio (status, nomen reservatum), acta
 * (clavibus exactis, sine secretis), restitutio (volumen bis
 * reapertum), revolutio (transactio vocatoris).
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "filum.h"
#include "json.h"
#include "moneta.h"
#include "volumen.h"
#include "xar.h"
#include "cives.h"
#include <stdio.h>
#include <string.h>

hic_manens s64 g_nunc = (s64)1760000000 * M;

interior s64
_nunc (vacuum)
{
    redde g_nunc;
}

interior chorda
_c (
    constans character* s,
               Piscina* p)
{
    redde chorda_ex_literis(s, p);
}

interior Cives*
_cives (
    CredoContextus*  c,
           Volumen** v)
{
    *v = volumen_temporarium(c->piscina, "cives");
    si (!*v)
    {
        redde NIHIL;
    }
    redde cives_aperire(*v, _nunc, c->piscina);
}

/* creatio in transactione propria (committitur aut revolvitur) */
interior b32
_creare (
                Volumen* v,
                  Cives* cv,
     constans character* nomen_usoris,
     constans character* titulus,
                 chorda* id,
                Piscina* p)
{
    b32 bene;

    si (!volumen_transactionem_incipere(v))
    {
        redde FALSUM;
    }
    bene = cives_creare(cv, _c(nomen_usoris, p), _c(titulus, p), id, p);
    si (bene)
    {
        redde volumen_transactionem_committere(v);
    }
    (vacuum)volumen_transactionem_revolvere(v);
    redde FALSUM;
}

/* acta generis dati (Xar de VolumenActum) */
interior i32
_acta_numerare (
                Volumen* v,
     constans character* genus,
                Piscina* p)
{
    Xar* acta  = volumen_acta_legere(v, ZEPHYRUM, p);
    i32  n     = ZEPHYRUM;
    i32  k;

    per (k = ZEPHYRUM; acta && k < xar_numerus(acta); k++)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(acta, k);

        si (chorda_aequalis_literis(a->genus, genus))
        {
            n++;
        }
    }
    redde n;
}

/* claves acti ultimi generis dati, commatibus iunctae ("" si nullum) */
interior constans character*
_claves_acti (
                Volumen* v,
     constans character* genus,
                Piscina* p)
{
                      Xar* acta = volumen_acta_legere(v, ZEPHYRUM, p);
                      i32  k;
                character* s = (character*)piscina_allocare(p, CCLVI);
    constans VolumenActum* ultimum = NIHIL;

    s[ZEPHYRUM] = '\0';
    per (k = ZEPHYRUM; acta && k < xar_numerus(acta); k++)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(acta, k);

        si (chorda_aequalis_literis(a->genus, genus))
        {
            ultimum = a;
        }
    }
    si (ultimum)
    {
                 JsonResultus  r = json_legere(ultimum->datum, p);
         JsonObjectumIterator  it;
                       chorda  clavis;
                    JsonValor* valor;

        si (!r.successus)
        {
            redde "?";
        }
        it = json_objectum_iterator(r.radix);
        dum (json_objectum_iterator_proxima(&it, &clavis, &valor))
        {
            si (s[ZEPHYRUM] != '\0')
            {
                strcat(s, ",");
            }
            strncat(s, (constans character*)clavis.datum,
                (size_t)clavis.mensura);
        }
    }
    redde s;
}


/* ==================================================
 * sectiones
 * ================================================== */

interior vacuum
_sectio_nomina (CredoContextus* c)
{
    Piscina* p = c->piscina;

    CREDO_VERUM(cives_nomen_validum(_c("fran", p)));
    CREDO_VERUM(cives_nomen_validum(_c("a", p)));
    CREDO_VERUM(cives_nomen_validum(_c("a.b_c-9", p)));
    CREDO_VERUM(cives_nomen_validum(_c(
        "abcdefghijklmnopqrstuvwxyz012345", p)));
    CREDO_FALSUM(cives_nomen_validum(_c("", p)));
    CREDO_FALSUM(cives_nomen_validum(_c(
        "abcdefghijklmnopqrstuvwxyz0123456", p)));
    CREDO_FALSUM(cives_nomen_validum(_c("Fran", p)));
    CREDO_FALSUM(cives_nomen_validum(_c("fr an", p)));
    CREDO_FALSUM(cives_nomen_validum(_c("fr\xc3\xa4n", p)));
    CREDO_FALSUM(cives_nomen_validum(_c("fran/", p)));
    CREDO_CHORDA_AEQUALIS_LITERIS(cives_nomen_normare(_c("FrAn", p), p),
        "fran");
    CREDO_CHORDA_AEQUALIS_LITERIS(cives_nomen_normare(_c("a.B_C-9", p),
        p), "a.b_c-9");
}

interior vacuum
_sectio_creare (CredoContextus* c)
{
     Piscina* p = c->piscina;
     Volumen* v;
       Cives* cv = _cives(c, &v);
      chorda  id;
      chorda  id_alterum;
       Civis  x;

    CREDO_NECESSE_NON_NIHIL(cv);
    CREDO_VERUM(_creare(v, cv, "fran", "Fran", &id, p));
    CREDO_AEQUALIS_I32(id.mensura, CIVES_ID_MENSURA);
    CREDO_VERUM(cives_per_id(cv, id, &x, p));
    CREDO_CHORDA_AEQUALIS(x.id, id);
    CREDO_CHORDA_AEQUALIS_LITERIS(x.nomen_usoris, "fran");
    CREDO_CHORDA_AEQUALIS_LITERIS(x.titulus, "Fran");
    CREDO_AEQUALIS_S32((s32)x.status, (s32)CIVIS_ACTIVUS);
    CREDO_AEQUALIS_S64(x.creatus_ms, g_nunc);
    CREDO_VERUM(cives_per_nomen(cv, _c("fran", p), &x, p));
    CREDO_CHORDA_AEQUALIS(x.id, id);
    /* nomen captum; nomen invalidum */
    CREDO_FALSUM(_creare(v, cv, "fran", "Alius", &id_alterum, p));
    CREDO_FALSUM(_creare(v, cv, "Fran", "Fran", &id_alterum, p));
    CREDO_FALSUM(cives_per_nomen(cv, _c("amanda", p), &x, p));
    CREDO_VERUM(_creare(v, cv, "amanda", "Amanda", &id_alterum, p));
    CREDO_FALSUM(chorda_aequalis(id, id_alterum));
    volumen_claudere(v);
}

interior vacuum
_sectio_nomen_mutare (CredoContextus* c)
{
     Piscina* p = c->piscina;
     Volumen* v;
       Cives* cv = _cives(c, &v);
      chorda  id;
      chorda  id_alterum;
       Civis  x;

    CREDO_NECESSE_NON_NIHIL(cv);
    CREDO_VERUM(_creare(v, cv, "fran", "Fran", &id, p));
    CREDO_VERUM(_creare(v, cv, "amanda", "Amanda", &id_alterum, p));
    CREDO_NECESSE(volumen_transactionem_incipere(v));
    CREDO_VERUM(cives_nomen_mutare(cv, id, _c("franciscus", p)));
    /* nomen alienum aut invalidum */
    CREDO_FALSUM(cives_nomen_mutare(cv, id, _c("amanda", p)));
    CREDO_FALSUM(cives_nomen_mutare(cv, id, _c("Franciscus", p)));
    CREDO_NECESSE(volumen_transactionem_committere(v));
    CREDO_FALSUM(cives_per_nomen(cv, _c("fran", p), &x, p));
    CREDO_VERUM(cives_per_nomen(cv, _c("franciscus", p), &x, p));
    CREDO_CHORDA_AEQUALIS(x.id, id);
    /* nomen vetus reservatum (P4) */
    CREDO_FALSUM(_creare(v, cv, "fran", "Alius", &id_alterum, p));
    volumen_claudere(v);
}

interior vacuum
_sectio_delere (CredoContextus* c)
{
     Piscina* p = c->piscina;
     Volumen* v;
       Cives* cv = _cives(c, &v);
      chorda  id;
      chorda  id_alterum;
       Civis  x;

    CREDO_NECESSE_NON_NIHIL(cv);
    CREDO_VERUM(_creare(v, cv, "fran", "Fran", &id, p));
    CREDO_NECESSE(volumen_transactionem_incipere(v));
    CREDO_VERUM(cives_statum_ponere(cv, id, CIVIS_SUSPENSUS));
    CREDO_NECESSE(volumen_transactionem_committere(v));
    CREDO_VERUM(cives_per_id(cv, id, &x, p));
    CREDO_AEQUALIS_S32((s32)x.status, (s32)CIVIS_SUSPENSUS);
    CREDO_NECESSE(volumen_transactionem_incipere(v));
    CREDO_VERUM(cives_delere(cv, id));
    CREDO_NECESSE(volumen_transactionem_committere(v));
    CREDO_VERUM(cives_per_id(cv, id, &x, p));
    CREDO_AEQUALIS_S32((s32)x.status, (s32)CIVIS_DELETUS);
    CREDO_FALSUM(_creare(v, cv, "fran", "Alius", &id_alterum, p));
    /* id ignotum */
    CREDO_NECESSE(volumen_transactionem_incipere(v));
    CREDO_FALSUM(cives_delere(cv, _c("01AAAAAAAAAAAAAAAAAAAAAAAA", p)));
    CREDO_FALSUM(cives_nomen_mutare(cv, _c("01AAAAAAAAAAAAAAAAAAAAAAAA",
        p), _c("nemo", p)));
    CREDO_NECESSE(volumen_transactionem_committere(v));
    volumen_claudere(v);
}

interior vacuum
_sectio_acta (CredoContextus* c)
{
     Piscina* p = c->piscina;
     Volumen* v;
       Cives* cv = _cives(c, &v);
      chorda  id;
      chorda  id_alterum;

    CREDO_NECESSE_NON_NIHIL(cv);
    CREDO_VERUM(_creare(v, cv, "fran", "Fran", &id, p));
    /* creatio refutata nullum actum scribit */
    CREDO_FALSUM(_creare(v, cv, "fran", "Alius", &id_alterum, p));
    CREDO_AEQUALIS_I32(_acta_numerare(v, "civis.creatus", p), I);
    CREDO_NECESSE(volumen_transactionem_incipere(v));
    CREDO_VERUM(cives_nomen_mutare(cv, id, _c("franciscus", p)));
    CREDO_VERUM(cives_statum_ponere(cv, id, CIVIS_SUSPENSUS));
    CREDO_VERUM(cives_delere(cv, id));
    CREDO_NECESSE(volumen_transactionem_committere(v));
    CREDO_CHORDA_AEQUALIS_LITERIS(_c(_claves_acti(v, "civis.creatus",
        p),
        p), "id,nomen_usoris,titulus");
    CREDO_CHORDA_AEQUALIS_LITERIS(_c(_claves_acti(v,
        "civis.renominatus",
        p), p), "id,nomen_usoris");
    CREDO_CHORDA_AEQUALIS_LITERIS(_c(_claves_acti(v, "civis.status", p),
        p), "id,status");
    CREDO_CHORDA_AEQUALIS_LITERIS(_c(_claves_acti(v, "civis.deletus",
        p),
        p), "id");
    volumen_claudere(v);
}

interior vacuum
_sectio_restitutio (CredoContextus* c)
{
      Piscina* p = c->piscina;
    character  via[CXXVIII];
    character  n[MONETA_NANOID_PLAGULAE_MENSURA];
    character  via_diarii[CXXXVI];
    character  via_communis[CXXXVI];
      Volumen* v;
        Cives* cv;
       chorda  id;
       chorda  id_amandae;
        Civis  x;
          i32  vices;

    (vacuum)moneta_nanoid_plagulae(n);
    sprintf(via, "/tmp/cives_%s.volumen", n);
    sprintf(via_diarii, "%s-wal", via);
    sprintf(via_communis, "%s-shm", via);
    v   = volumen_aperire_aut_creare(p, via);
    cv  = v ? cives_aperire(v, _nunc, p) : NIHIL;
    CREDO_NECESSE_NON_NIHIL(cv);
    CREDO_VERUM(_creare(v, cv, "fran", "Fran", &id, p));
    CREDO_VERUM(_creare(v, cv, "amanda", "Amanda", &id_amandae, p));
    CREDO_NECESSE(volumen_transactionem_incipere(v));
    CREDO_VERUM(cives_statum_ponere(cv, id_amandae, CIVIS_SUSPENSUS));
    CREDO_NECESSE(volumen_transactionem_committere(v));
    volumen_claudere(v);
    per (vices = ZEPHYRUM; vices < II; vices++)
    {
        v   = volumen_aperire(p, via);
        cv  = v ? cives_aperire(v, _nunc, p) : NIHIL;
        CREDO_NECESSE_NON_NIHIL(cv);
        CREDO_VERUM(cives_per_nomen(cv, _c("fran", p), &x, p));
        CREDO_CHORDA_AEQUALIS(x.id, id);
        CREDO_CHORDA_AEQUALIS_LITERIS(x.titulus, "Fran");
        CREDO_AEQUALIS_S32((s32)x.status, (s32)CIVIS_ACTIVUS);
        CREDO_VERUM(cives_per_id(cv, id_amandae, &x, p));
        CREDO_AEQUALIS_S32((s32)x.status, (s32)CIVIS_SUSPENSUS);
        volumen_claudere(v);
    }
    (vacuum)filum_delere(via);
    (vacuum)filum_delere(via_diarii);
    (vacuum)filum_delere(via_communis);
}

interior vacuum
_sectio_revolutio (CredoContextus* c)
{
     Piscina* p = c->piscina;
     Volumen* v;
       Cives* cv = _cives(c, &v);
      chorda  id;
       Civis  x;

    CREDO_NECESSE_NON_NIHIL(cv);
    CREDO_NECESSE(volumen_transactionem_incipere(v));
    CREDO_VERUM(cives_creare(cv, _c("fran", p), _c("Fran", p), &id, p));
    CREDO_NECESSE(volumen_transactionem_revolvere(v));
    CREDO_FALSUM(cives_per_nomen(cv, _c("fran", p), &x, p));
    CREDO_FALSUM(cives_per_id(cv, id, &x, p));
    CREDO_AEQUALIS_I32(_acta_numerare(v, "civis.creatus", p), ZEPHYRUM);
    CREDO_VERUM(_creare(v, cv, "fran", "Fran", &id, p));
    volumen_claudere(v);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "nomina", _sectio_nomina, NIHIL, NIHIL, NIHIL, NIHIL },
    { "creare", _sectio_creare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nomen_mutare", _sectio_nomen_mutare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "delere", _sectio_delere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "acta", _sectio_acta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "restitutio", _sectio_restitutio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "revolutio", _sectio_revolutio, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("cives", SECTIONES);
}
