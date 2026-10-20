/* probatio_invitationes.c - invitationes (ianua T3): codices per
 * personam - creatio (XXII signa base64url, semel redditus), usus
 * semel aut usque ad usus_max, terminus vitae (horologium iniectum, limes
 * ms exactus), revocatio (auctoris solum), certamen (inspectio non
 * decernit - UPDATE custoditum), codex numquam in tabula nec actis
 * (solum digestum SHA-256), enumeratio per auctorem.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "scrinium.h"
#include "sigillum.h"
#include "volumen.h"
#include "xar.h"
#include "invitationes.h"
#include <string.h>

#define PRIMUS   "01PRIMUSAAAAAAAAAAAAAAAAAA"
#define SECUNDUS "01SECUNDUSAAAAAAAAAAAAAAAA"
#define TERTIUS  "01TERTIUSAAAAAAAAAAAAAAAAA"
#define DIES_MS ((s64)XXIV * LX * LX * M)

hic_manens s64 g_nunc = (s64)1760000000 * M;

interior s64
_nunc (vacuum)
{
    redde g_nunc;
}

interior vacuum
_horologium_parare (
    CredoContextus* c)
{
    (vacuum)c;
    g_nunc = (s64)1760000000 * M;
}

interior chorda
_c (
    constans character* s,
               Piscina* p)
{
    redde chorda_ex_literis(s, p);
}

interior Invitationes*
_invitationes (
    CredoContextus*  c,
           Volumen** v)
{
    *v = volumen_temporarium(c->piscina, "invitationes");
    si (!*v)
    {
        redde NIHIL;
    }
    redde invitationes_aperire(*v, _nunc, c->piscina);
}

/* creatio in transactione propria */
interior b32
_creare (
                Volumen* v,
           Invitationes* inv,
     constans character* auctor,
     constans character* nota,
                    s64  vita_ms,
                    i32  usus_max,
                 chorda* codex,
                 chorda* id,
                Piscina* p)
{
    b32 bene;

    si (!volumen_transactionem_incipere(v))
    {
        redde FALSUM;
    }
    bene = invitationes_creare(inv, _c(auctor, p), _c(nota, p), vita_ms,
        usus_max, codex, id, p);
    si (bene)
    {
        redde volumen_transactionem_committere(v);
    }
    (vacuum)volumen_transactionem_revolvere(v);
    redde FALSUM;
}

interior b32
_consumere (
               Volumen* v,
          Invitationes* inv,
                chorda  codex,
    constans character* civis,
               Piscina* p)
{
    b32 bene;

    si (!volumen_transactionem_incipere(v))
    {
        redde FALSUM;
    }
    bene = invitationes_consumere(inv, codex, _c(civis, p));
    si (bene)
    {
        redde volumen_transactionem_committere(v);
    }
    (vacuum)volumen_transactionem_revolvere(v);
    redde FALSUM;
}

interior b32
_revocare (
               Volumen* v,
          Invitationes* inv,
    constans character* auctor,
                chorda  id,
               Piscina* p)
{
    b32 bene;

    si (!volumen_transactionem_incipere(v))
    {
        redde FALSUM;
    }
    bene = invitationes_revocare(inv, _c(auctor, p), id);
    si (bene)
    {
        redde volumen_transactionem_committere(v);
    }
    (vacuum)volumen_transactionem_revolvere(v);
    redde FALSUM;
}

interior b32
_base64url (
    chorda s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        i8 x = s.datum[i];

        si (!((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z')
              || (x >= '0' && x <= '9') || x == '-' || x == '_'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* textus unus ex quaestione */
interior chorda
_textus (
                Volumen* v,
     constans character* sql,
                Piscina* p)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(volumen_scrinium(v),
        sql);
                 chorda t = chorda_ex_literis("", p);

    si (e && scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        t = scrinium_columna_textus(e, ZEPHYRUM, p);
    }
    si (e)
    {
        scrinium_finire(e);
    }
    redde t;
}


/* ==================================================
 * sectiones
 * ================================================== */

interior vacuum
_sectio_creare (CredoContextus* c)
{
         Piscina* p = c->piscina;
         Volumen* v;
    Invitationes* inv = _invitationes(c, &v);
          chorda  codex;
          chorda  codex_alter;
          chorda  id;
          chorda  id_alter;
       Invitatio  x;

    CREDO_NECESSE_NON_NIHIL(inv);
    CREDO_VERUM(_creare(v, inv, PRIMUS, "pro Amanda", ZEPHYRUM,
        ZEPHYRUM,
        &codex, &id, p));
    CREDO_AEQUALIS_I32(codex.mensura, INVITATIONES_CODEX_MENSURA);
    CREDO_VERUM(_base64url(codex));
    CREDO_AEQUALIS_I32(id.mensura, XXVI);
    CREDO_VERUM(invitationes_inspicere(inv, codex, &x, p));
    CREDO_CHORDA_AEQUALIS(x.id, id);
    CREDO_CHORDA_AEQUALIS_LITERIS(x.auctor, PRIMUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(x.nota, "pro Amanda");
    CREDO_AEQUALIS_S64(x.creata_ms, g_nunc);
    CREDO_AEQUALIS_S64(x.expirat_ms, g_nunc
        + INVITATIONES_VITA_ORDINARIA);
    CREDO_AEQUALIS_I32(x.usus_max, I);
    CREDO_AEQUALIS_I32(x.usus_numerus, ZEPHYRUM);
    CREDO_CHORDA_VACUA(x.usa_a);
    CREDO_AEQUALIS_S64(x.revocata_ms, ZEPHYRUM);
    CREDO_VERUM(_creare(v, inv, PRIMUS, "", ZEPHYRUM, ZEPHYRUM,
        &codex_alter, &id_alter, p));
    CREDO_FALSUM(chorda_aequalis(codex, codex_alter));
    CREDO_FALSUM(chorda_aequalis(id, id_alter));
    /* auctor vacuus = CLI (ianua P1); enumeratur sub auctore vacuo */
    CREDO_VERUM(_creare(v, inv, "", "prima persona", ZEPHYRUM, ZEPHYRUM,
        &codex_alter, &id_alter, p));
    CREDO_VERUM(invitationes_inspicere(inv, codex_alter, &x, p));
    CREDO_CHORDA_VACUA(x.auctor);
    CREDO_AEQUALIS_I32(xar_numerus(invitationes_enumerare(inv, _c("",
        p),
        p)), I);
    /* codex ignotus aut pravus */
    CREDO_FALSUM(invitationes_inspicere(inv,
        _c("AAAAAAAAAAAAAAAAAAAAAA",
        p), &x, p));
    CREDO_FALSUM(invitationes_inspicere(inv, _c("", p), &x, p));
    volumen_claudere(v);
}

interior vacuum
_sectio_semel (CredoContextus* c)
{
         Piscina* p = c->piscina;
         Volumen* v;
    Invitationes* inv = _invitationes(c, &v);
          chorda  codex;
          chorda  codex_ter;
          chorda  id;
       Invitatio  x;

    CREDO_NECESSE_NON_NIHIL(inv);
    CREDO_VERUM(_creare(v, inv, PRIMUS, "", ZEPHYRUM, ZEPHYRUM, &codex,
        &id, p));
    CREDO_VERUM(_consumere(v, inv, codex, SECUNDUS, p));
    CREDO_FALSUM(_consumere(v, inv, codex, TERTIUS, p));
    CREDO_FALSUM(invitationes_inspicere(inv, codex, &x, p));
    /* usa_a et numerus in enumeratione */
    {
        Xar* l = invitationes_enumerare(inv, _c(PRIMUS, p), p);

        CREDO_NECESSE_NON_NIHIL(l);
        CREDO_AEQUALIS_I32(xar_numerus(l), I);
        CREDO_CHORDA_AEQUALIS_LITERIS(((Invitatio*)xar_obtinere(l,
            ZEPHYRUM))->usa_a, SECUNDUS);
        CREDO_AEQUALIS_I32(((Invitatio*)xar_obtinere(l, ZEPHYRUM))
            ->usus_numerus, I);
    }
    /* usus_max III */
    CREDO_VERUM(_creare(v, inv, PRIMUS, "familia", ZEPHYRUM, III,
        &codex_ter, &id, p));
    CREDO_VERUM(_consumere(v, inv, codex_ter, SECUNDUS, p));
    CREDO_VERUM(_consumere(v, inv, codex_ter, TERTIUS, p));
    CREDO_VERUM(invitationes_inspicere(inv, codex_ter, &x, p));
    CREDO_AEQUALIS_I32(x.usus_numerus, II);
    CREDO_VERUM(_consumere(v, inv, codex_ter, PRIMUS, p));
    CREDO_FALSUM(_consumere(v, inv, codex_ter, TERTIUS, p));
    volumen_claudere(v);
}

interior vacuum
_sectio_terminus (CredoContextus* c)
{
         Piscina* p = c->piscina;
         Volumen* v;
    Invitationes* inv = _invitationes(c, &v);
          chorda  codex;
          chorda  id;
       Invitatio  x;
             s64  initium = g_nunc;

    CREDO_NECESSE_NON_NIHIL(inv);
    CREDO_VERUM(_creare(v, inv, PRIMUS, "", DIES_MS, ZEPHYRUM, &codex,
        &id,
        p));
    g_nunc = initium + DIES_MS - I;
    CREDO_VERUM(invitationes_inspicere(inv, codex, &x, p));
    g_nunc = initium + DIES_MS;
    CREDO_FALSUM(invitationes_inspicere(inv, codex, &x, p));
    CREDO_FALSUM(_consumere(v, inv, codex, SECUNDUS, p));
    g_nunc = initium + DIES_MS - I;
    CREDO_VERUM(_consumere(v, inv, codex, SECUNDUS, p));
    volumen_claudere(v);
}

interior vacuum
_sectio_revocare (CredoContextus* c)
{
         Piscina* p = c->piscina;
         Volumen* v;
    Invitationes* inv = _invitationes(c, &v);
          chorda  codex;
          chorda  id;
       Invitatio  x;

    CREDO_NECESSE_NON_NIHIL(inv);
    CREDO_VERUM(_creare(v, inv, PRIMUS, "", ZEPHYRUM, ZEPHYRUM, &codex,
        &id,
        p));
    CREDO_FALSUM(_revocare(v, inv, SECUNDUS, id, p));
    CREDO_VERUM(invitationes_inspicere(inv, codex, &x, p));
    CREDO_VERUM(_revocare(v, inv, PRIMUS, id, p));
    CREDO_FALSUM(invitationes_inspicere(inv, codex, &x, p));
    CREDO_FALSUM(_consumere(v, inv, codex, SECUNDUS, p));
    CREDO_FALSUM(_revocare(v, inv, PRIMUS, id, p));
    {
        Xar* l = invitationes_enumerare(inv, _c(PRIMUS, p), p);

        CREDO_NECESSE_NON_NIHIL(l);
        CREDO_AEQUALIS_S64(((Invitatio*)xar_obtinere(l, ZEPHYRUM))
            ->revocata_ms, g_nunc);
    }
    volumen_claudere(v);
}

interior vacuum
_sectio_certamen (CredoContextus* c)
{
         Piscina* p = c->piscina;
         Volumen* v;
    Invitationes* a = _invitationes(c, &v);
    Invitationes* b = a ? invitationes_aperire(v, _nunc, p) : NIHIL;
          chorda  codex;
          chorda  id;
       Invitatio  x;
             i32  victores = ZEPHYRUM;

    CREDO_NECESSE_NON_NIHIL(b);
    CREDO_VERUM(_creare(v, a, PRIMUS, "", ZEPHYRUM, ZEPHYRUM, &codex,
        &id,
        p));
    /* ambo inspiciunt, ambo putant validam */
    CREDO_VERUM(invitationes_inspicere(a, codex, &x, p));
    CREDO_VERUM(invitationes_inspicere(b, codex, &x, p));
    victores += _consumere(v, a, codex, SECUNDUS, p) ? I : ZEPHYRUM;
    victores += _consumere(v, b, codex, TERTIUS, p) ? I : ZEPHYRUM;
    CREDO_AEQUALIS_I32(victores, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(_textus(v,
        "SELECT CAST(usus_numerus AS TEXT) FROM invitationes", p), "1");
    volumen_claudere(v);
}

interior vacuum
_sectio_codex_occultus (CredoContextus* c)
{
         Piscina* p = c->piscina;
         Volumen* v;
    Invitationes* inv = _invitationes(c, &v);
          chorda  codex;
          chorda  id;
          chorda  columnae;
        Sigillum  s;
       character  hex[SIGILLUM_HEX_MENSURA];
             Xar* acta;
             i32  k;

    CREDO_NECESSE_NON_NIHIL(inv);
    CREDO_VERUM(_creare(v, inv, PRIMUS, "pro Amanda", ZEPHYRUM,
        ZEPHYRUM,
        &codex, &id, p));
    CREDO_VERUM(_consumere(v, inv, codex, SECUNDUS, p));
    /* invitatio usa adhuc revocabilis (vestigium) */
    CREDO_VERUM(_revocare(v, inv, PRIMUS, id, p));
    columnae = _textus(v, "SELECT group_concat(id || sigillum || auctor"
        " || nota || creata_ms || expirat_ms || usus_max || usus_numerus"
        " || usa_a || revocata_ms) FROM invitationes", p);
    CREDO_CHORDA_NON_VACUA(columnae);
    CREDO_FALSUM(chorda_continet(columnae, codex));
    s = sigillum_computare(codex.datum, (memoriae_index)codex.mensura);
    sigillum_hex(&s, hex);
    CREDO_CHORDA_CONTINET(columnae, _c(hex, p));
    acta = volumen_acta_legere(v, ZEPHYRUM, p);
    CREDO_NECESSE_NON_NIHIL(acta);
    per (k = ZEPHYRUM; k < xar_numerus(acta); k++)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(acta, k);

        CREDO_FALSUM(chorda_continet(a->datum, codex));
        CREDO_FALSUM(chorda_continet(a->datum, _c(hex, p)));
    }
    volumen_claudere(v);
}

interior vacuum
_sectio_enumerare (CredoContextus* c)
{
         Piscina* p = c->piscina;
         Volumen* v;
    Invitationes* inv = _invitationes(c, &v);
          chorda  codex;
          chorda  id_prima;
          chorda  id_ultima;
             Xar* l;

    CREDO_NECESSE_NON_NIHIL(inv);
    CREDO_VERUM(_creare(v, inv, PRIMUS, "prima", ZEPHYRUM, ZEPHYRUM,
        &codex, &id_prima, p));
    g_nunc += M;
    CREDO_VERUM(_creare(v, inv, SECUNDUS, "amandae", ZEPHYRUM, ZEPHYRUM,
        &codex, &id_ultima, p));
    CREDO_VERUM(_creare(v, inv, PRIMUS, "secunda", ZEPHYRUM, ZEPHYRUM,
        &codex, &id_ultima, p));
    g_nunc += M;
    CREDO_VERUM(_creare(v, inv, PRIMUS, "tertia", ZEPHYRUM, ZEPHYRUM,
        &codex, &id_ultima, p));
    l = invitationes_enumerare(inv, _c(PRIMUS, p), p);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), III);
    CREDO_CHORDA_AEQUALIS_LITERIS(((Invitatio*)xar_obtinere(l,
        ZEPHYRUM))
        ->nota, "tertia");
    CREDO_CHORDA_AEQUALIS(((Invitatio*)xar_obtinere(l, ZEPHYRUM))->id,
        id_ultima);
    CREDO_CHORDA_AEQUALIS_LITERIS(((Invitatio*)xar_obtinere(l,
        II))->nota,
        "prima");
    CREDO_CHORDA_AEQUALIS(((Invitatio*)xar_obtinere(l, II))->id,
        id_prima);
    l = invitationes_enumerare(inv, _c(SECUNDUS, p), p);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), I);
    l = invitationes_enumerare(inv, _c(TERTIUS, p), p);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), ZEPHYRUM);
    volumen_claudere(v);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "creare", _sectio_creare, _horologium_parare, NIHIL, NIHIL,
        NIHIL },
    { "semel", _sectio_semel, _horologium_parare, NIHIL, NIHIL, NIHIL },
    { "terminus", _sectio_terminus, _horologium_parare, NIHIL, NIHIL,
        NIHIL },
    { "revocare", _sectio_revocare, _horologium_parare, NIHIL, NIHIL,
        NIHIL },
    { "certamen", _sectio_certamen, _horologium_parare, NIHIL, NIHIL,
        NIHIL },
    { "codex_occultus", _sectio_codex_occultus, _horologium_parare,
        NIHIL, NIHIL, NIHIL },
    { "enumerare", _sectio_enumerare, _horologium_parare, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("invitationes", SECTIONES);
}
