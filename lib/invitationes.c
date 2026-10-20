/* invitationes.c - codices invitationis (vide include/invitationes.h)
 *
 * Clavis tabulae = SHA-256 hex codicis: quaestio per digestum, codex
 * ipse numquam conditur. Codex longitudinis alienae statim invalidus.
 */

#include "postulata_posix.h"
#include "invitationes.h"
#include "base64.h"
#include "json.h"
#include "moneta.h"
#include "scrinium.h"
#include "sigillum.h"

#include <string.h>
#include <sys/time.h>

#define OCTETI_CODICIS XVI

structura Invitationes {
     Volumen* volumen;
    Scrinium* scrinium;
          s64  (*nunc_ms)(vacuum);
};

hic_manens constans character* constans SCHEMA[] = {
    "CREATE TABLE IF NOT EXISTS invitationes"
    " (id TEXT PRIMARY KEY, sigillum TEXT NOT NULL UNIQUE,"
    " auctor TEXT NOT NULL, nota TEXT NOT NULL,"
    " creata_ms INTEGER NOT NULL, expirat_ms INTEGER NOT NULL,"
    " usus_max INTEGER NOT NULL, usus_numerus INTEGER NOT NULL DEFAULT 0,"
    " usa_a TEXT NOT NULL DEFAULT '', revocata_ms INTEGER NOT NULL"
    " DEFAULT 0)",
    "CREATE INDEX IF NOT EXISTS invitationes_auctor"
    " ON invitationes (auctor, creata_ms)",
    NIHIL
};

#define COLUMNAE "id, auctor, nota, creata_ms, expirat_ms, usus_max," \
    " usus_numerus, usa_a, revocata_ms"


/* ==================================================
 * auxilia
 * ================================================== */

interior s64
_tempus_verum_ms (vacuum)
{
    structura timeval tv;

    si (gettimeofday(&tv, NIHIL) != ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde (s64)tv.tv_sec * M + (s64)(tv.tv_usec / M);
}

interior s64
_nunc (
    Invitationes* inv)
{
    redde inv->nunc_ms ? inv->nunc_ms() : _tempus_verum_ms();
}

interior chorda
_ch (
    constans character* s)
{
    chorda c;

    c.datum    = (i8*)(vacuum*)(memoriae_index)s;
    c.mensura  = (i32)strlen(s);
    redde c;
}

/* digestum hex codicis in effusionem (SIGILLUM_HEX_MENSURA); FALSUM si
 * codex longitudinis alienae */
interior b32
_digestum (
       chorda  codex,
    character* hex)
{
    Sigillum s;

    si (!codex.datum || codex.mensura != INVITATIONES_CODEX_MENSURA)
    {
        redde FALSUM;
    }
    s = sigillum_computare(codex.datum, (memoriae_index)codex.mensura);
    sigillum_hex(&s, hex);
    redde VERUM;
}

interior b32
_actum (
          Invitationes* inv,
    constans character* genus,
             JsonValor* d,
               Piscina* p)
{
    redde volumen_actum_appendere(inv->volumen, genus, json_scribere(d,
        p)) > ZEPHYRUM;
}

/* lineae mutatae sententiae ultimae */
interior s64
_mutatae (
    Invitationes* inv)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(inv->scrinium,
        "SELECT changes()");
                    s64 n = -I;

    si (e && scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        n = scrinium_columna_numerus(e, ZEPHYRUM);
    }
    si (e)
    {
        scrinium_finire(e);
    }
    redde n;
}

/* ordo -> Invitatio (columnae COLUMNAE) */
interior vacuum
_legere (
    ScriniumEnuntiatum* e,
             Invitatio* x,
               Piscina* p)
{
    x->id            = scrinium_columna_textus(e, ZEPHYRUM, p);
    x->auctor        = scrinium_columna_textus(e, I, p);
    x->nota          = scrinium_columna_textus(e, II, p);
    x->creata_ms     = scrinium_columna_numerus(e, III);
    x->expirat_ms    = scrinium_columna_numerus(e, IV);
    x->usus_max      = (i32)scrinium_columna_numerus(e, V);
    x->usus_numerus  = (i32)scrinium_columna_numerus(e, VI);
    x->usa_a         = scrinium_columna_textus(e, VII, p);
    x->revocata_ms   = scrinium_columna_numerus(e, VIII);
}


/* ==================================================
 * API
 * ================================================== */

Invitationes*
invitationes_aperire (
     Volumen* volumen,
         s64  (*nunc_ms)(vacuum),
     Piscina* piscina)
{
    Invitationes* inv;
             i32  k;

    si (!volumen || !piscina)
    {
        redde NIHIL;
    }
    inv = (Invitationes*)piscina_allocare(piscina,
        (i64)magnitudo(Invitationes));
    si (!inv)
    {
        redde NIHIL;
    }
    memset(inv, ZEPHYRUM, magnitudo(*inv));
    inv->volumen   = volumen;
    inv->scrinium  = volumen_scrinium(volumen);
    inv->nunc_ms   = nunc_ms;
    per (k = ZEPHYRUM; SCHEMA[k]; k++)
    {
        si (   !inv->scrinium
            || !scrinium_exsequi(inv->scrinium, SCHEMA[k]))
        {
            redde NIHIL;
        }
    }
    redde inv;
}

b32
invitationes_creare (
    Invitationes* invitationes,
          chorda  auctor,
          chorda  nota,
             s64  vita_ms,
             i32  usus_max,
         chorda* codex_exitus,
         chorda* id_exitus,
        Piscina* piscina)
{
                    i8  octeti[OCTETI_CODICIS];
             character  codex[INVITATIONES_CODEX_MENSURA + I];
             character  hex[SIGILLUM_HEX_MENSURA];
             character  ulid[SCRINIUM_ULID_MENSURA];
    ScriniumEnuntiatum* e;
                   s64  nunc;
                   s64  expirat;
                   b32  bene;
             JsonValor* d;

    si (!invitationes || !codex_exitus || !id_exitus || !piscina)
    {
        redde FALSUM;
    }
    /* vacuae licitae: auctor vacuus = CLI, nota optionalis */
    si (!auctor.datum)
    {
        auctor = _ch("");
    }
    si (!nota.datum)
    {
        nota = _ch("");
    }
    si (   !moneta_octeti_fortuiti(octeti, OCTETI_CODICIS)
        || base64url_scribere(octeti, OCTETI_CODICIS, codex,
               INVITATIONES_CODEX_MENSURA)
                   != INVITATIONES_CODEX_MENSURA)
    {
        memset(octeti, ZEPHYRUM, magnitudo(octeti));
        redde FALSUM;
    }
    memset(octeti, ZEPHYRUM, magnitudo(octeti));
    codex[INVITATIONES_CODEX_MENSURA] = '\0';
    (vacuum)_digestum(_ch(codex), hex);
    scrinium_ulid(ulid);
    nunc     = _nunc(invitationes);
    expirat  = nunc + (vita_ms > ZEPHYRUM ? vita_ms
        : INVITATIONES_VITA_ORDINARIA);
    usus_max = usus_max > ZEPHYRUM ? usus_max : I;
    e = scrinium_praeparare(invitationes->scrinium,
        "INSERT INTO invitationes (id, sigillum, auctor, nota, creata_ms,"
        " expirat_ms, usus_max) VALUES (?, ?, ?, ?, ?, ?, ?)");
    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, I, _ch(ulid));
    scrinium_ligare_textum(e, II, _ch(hex));
    scrinium_ligare_textum(e, III, auctor);
    scrinium_ligare_textum(e, IV, nota);
    scrinium_ligare_numerum(e, V, nunc);
    scrinium_ligare_numerum(e, VI, expirat);
    scrinium_ligare_numerum(e, VII, (s64)usus_max);
    bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
    scrinium_finire(e);
    si (!bene)
    {
        redde FALSUM;
    }
    d = json_objectum_creare(piscina);
    json_objectum_ponere(d, "id", json_chorda_creare(piscina,
        _ch(ulid)));
    json_objectum_ponere(d, "auctor", json_chorda_creare(piscina,
        auctor));
    json_objectum_ponere(d, "expirat_ms", json_integer_creare(piscina,
        expirat));
    json_objectum_ponere(d, "usus_max", json_integer_creare(piscina,
        (s64)usus_max));
    si (!_actum(invitationes, "invitatio.creata", d, piscina))
    {
        redde FALSUM;
    }
    *codex_exitus  = chorda_ex_literis(codex, piscina);
    *id_exitus     = chorda_ex_literis(ulid, piscina);
    memset(codex, ZEPHYRUM, magnitudo(codex));
    redde VERUM;
}

b32
invitationes_inspicere (
    Invitationes* invitationes,
          chorda  codex,
       Invitatio* exitus,
        Piscina* piscina)
{
             character  hex[SIGILLUM_HEX_MENSURA];
    ScriniumEnuntiatum* e;
                   b32  valida = FALSUM;

    si (!invitationes || !exitus || !piscina || !_digestum(codex, hex))
    {
        redde FALSUM;
    }
    e = scrinium_praeparare(invitationes->scrinium,
        "SELECT " COLUMNAE " FROM invitationes WHERE sigillum = ?"
        " AND usus_numerus < usus_max AND expirat_ms > ?"
        " AND revocata_ms = 0");
    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, I, _ch(hex));
    scrinium_ligare_numerum(e, II, _nunc(invitationes));
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        _legere(e, exitus, piscina);
        valida = VERUM;
    }
    scrinium_finire(e);
    redde valida;
}

b32
invitationes_consumere (
    Invitationes* invitationes,
          chorda  codex,
          chorda  civis)
{
             character  hex[SIGILLUM_HEX_MENSURA];
    ScriniumEnuntiatum* e;
               Piscina* p;
             JsonValor* d;
                chorda  id;
                   b32  bene;

    si (!invitationes || !civis.datum || !_digestum(codex, hex))
    {
        redde FALSUM;
    }
    e = scrinium_praeparare(invitationes->scrinium,
        "UPDATE invitationes SET usus_numerus = usus_numerus + 1,"
        " usa_a = ? WHERE sigillum = ? AND usus_numerus < usus_max"
        " AND expirat_ms > ? AND revocata_ms = 0");
    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, I, civis);
    scrinium_ligare_textum(e, II, _ch(hex));
    scrinium_ligare_numerum(e, III, _nunc(invitationes));
    bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
    scrinium_finire(e);
    si (!bene || _mutatae(invitationes) != I)
    {
        redde FALSUM;
    }
    p = piscina_generare_dynamicum("invitationes_usus", DXII);
    si (!p)
    {
        redde FALSUM;
    }
    e   = scrinium_praeparare(invitationes->scrinium,
        "SELECT id FROM invitationes WHERE sigillum = ?");
    id  = chorda_ex_literis("", p);
    si (e)
    {
        scrinium_ligare_textum(e, I, _ch(hex));
        si (scrinium_gradi(e) == SCRINIUM_ORDO)
        {
            id = scrinium_columna_textus(e, ZEPHYRUM, p);
        }
        scrinium_finire(e);
    }
    d = json_objectum_creare(p);
    json_objectum_ponere(d, "id", json_chorda_creare(p, id));
    json_objectum_ponere(d, "civis", json_chorda_creare(p, civis));
    bene = id.mensura > ZEPHYRUM
        && _actum(invitationes, "invitatio.usa",
        d, p);
    piscina_destruere(p);
    redde bene;
}

b32
invitationes_revocare (
    Invitationes* invitationes,
          chorda  auctor,
          chorda  id)
{
    ScriniumEnuntiatum* e;
               Piscina* p;
             JsonValor* d;
                   b32  bene;

    si (!invitationes || !auctor.datum || !id.datum)
    {
        redde FALSUM;
    }
    e = scrinium_praeparare(invitationes->scrinium,
        "UPDATE invitationes SET revocata_ms = ? WHERE id = ?"
        " AND auctor = ? AND revocata_ms = 0");
    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_numerum(e, I, _nunc(invitationes));
    scrinium_ligare_textum(e, II, id);
    scrinium_ligare_textum(e, III, auctor);
    bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
    scrinium_finire(e);
    si (!bene || _mutatae(invitationes) != I)
    {
        redde FALSUM;
    }
    p = piscina_generare_dynamicum("invitationes_revocatio", DXII);
    si (!p)
    {
        redde FALSUM;
    }
    d = json_objectum_creare(p);
    json_objectum_ponere(d, "id", json_chorda_creare(p, id));
    bene = _actum(invitationes, "invitatio.revocata", d, p);
    piscina_destruere(p);
    redde bene;
}

Xar*
invitationes_enumerare (
    Invitationes* invitationes,
          chorda  auctor,
        Piscina* piscina)
{
                   Xar* l;
    ScriniumEnuntiatum* e;

    si (!invitationes || !piscina)
    {
        redde NIHIL;
    }
    si (!auctor.datum)
    {
        auctor = _ch("");
    }
    l = xar_creare(piscina, (i32)magnitudo(Invitatio));
    e = l ? scrinium_praeparare(invitationes->scrinium,
        "SELECT " COLUMNAE " FROM invitationes WHERE auctor = ?"
        " ORDER BY creata_ms DESC, id DESC") : NIHIL;
    si (!e)
    {
        redde NIHIL;
    }
    scrinium_ligare_textum(e, I, auctor);
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        Invitatio* x = (Invitatio*)xar_addere(l);

        si (!x)
        {
            frange;
        }
        _legere(e, x, piscina);
    }
    scrinium_finire(e);
    redde l;
}
