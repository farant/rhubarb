/* indagatio.c - quaestio textus per omnia genera datorum
 * (include/indagatio.h; project-specs/indagatio.md, D5) */
#include "indagatio.h"
#include "scrinium.h"
#include "stml.h"
#include "internamentum.h"
#include "tabula_dispersa.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INDAGATIO_OPUS        (XVI * MXXIV)
#define INDAGATIO_CAPACITAS   X
#define SIGNUM_INITII         ((character)0x02)
#define SIGNUM_FINIS          ((character)0x03)

/* titulus gravior quam corpus in bm25 (columnae ix: titulus, corpus) */
#define ORDO_CONGRUENTIAE  "bm25(ix, 5.0, 1.0)"

nomen structura GenusQuaerendum GenusQuaerendum;

structura GenusQuaerendum {
              Indagatio* ix;
     constans character* titulus;      /* nomen declarationis */
                 chorda  nomen_c;     /* idem, chorda */
                 chorda  genus;       /* titulus gregis */
         TabulaDispersa* sordidae;    /* in ix->sordes (NIHIL) */
                    Xar* ordo;        /* chorda: sordidae ordine */
                    b32  omnes;       /* fons 'omnia' */
};

structura Indagatio {
                Piscina* piscina;
    ConspectusRegistrum* repertorium;
                    Xar* genera;      /* GenusQuaerendum* */
               Scrinium* index;       /* NIHIL donec aedificatus */
                Piscina* sordes;      /* claves sordidae */
                    i64  ordines;
                    i64  rescripta;
                    s64  versio;
                    b32  aedificatus;
                    b32  error_positus;
              character  error[CCLVI];
};


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior vacuum
errorem_ponere (
             Indagatio* ix,
    constans character* causa,
    constans character* res)
{
    sprintf(ix->error, "%.100s: %.140s", causa, res ? res : "?");
    ix->error_positus = VERUM;
}

interior b32
spatium_album (
    character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f'
        || c == '\v';
}

/* terminus verbum habet: littera aut cifra ASCII, aut octetus non
 * ASCII (UTF-8; classis Unicode deest - desideratum …K688S) */
interior b32
verbum_habet (
    constans i8* datum,
            i32  mensura)
{
    i32 i;

    per (i = ZEPHYRUM; i < mensura; i++)
    {
        insignatus character c = (insignatus character)datum[i];

        si (   (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
            || (c >= '0' && c <= '9') || c >= 0x80)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior GenusQuaerendum*
genus_invenire (
    Indagatio* ix,
       chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(ix->genera); i++)
    {
        GenusQuaerendum* g =
            *(GenusQuaerendum**)xar_obtinere(ix->genera,
            i);

        si (chorda_aequalis(g->nomen_c, titulus))
        {
            redde g;
        }
    }
    redde NIHIL;
}


/* ==================================================
 * Codex
 * ================================================== */

chorda
indagatio_conspectum_codificare (
             vacuum* ctx,
    constans vacuum* conspectus,
            Piscina* piscina)
{
    constans IndagatioConspectus* v;
             InternamentumChorda* intern;
                       StmlNodus* radix;
                             i32  i;

    (vacuum)ctx;
    v = (constans IndagatioConspectus*)conspectus;
    si (!v || !piscina)
    {
        redde vacua();
    }
    intern  = internamentum_creare(piscina);
    radix   = stml_elementum_creare(piscina, intern, "ordines");
    per (i = ZEPHYRUM; v->ordines && i < xar_numerus(v->ordines); i++)
    {
        constans IndagatioOrdo* o =
            (constans IndagatioOrdo*)xar_obtinere(
            v->ordines, i);
                     StmlNodus* n;

        n = stml_elementum_creare(piscina, intern, "ordo");
        (vacuum)stml_attributum_addere_chorda(n, piscina, intern,
            "titulus", o->titulus);
        (vacuum)stml_attributum_addere_chorda(n, piscina, intern,
            "contextus", o->contextus);
        (vacuum)stml_attributum_addere_chorda(n, piscina, intern,
            "semita", o->semita);
        (vacuum)stml_liberum_addere(n, stml_textum_creare_ex_chorda(
            piscina, intern, o->corpus));
        (vacuum)stml_liberum_addere(radix, n);
    }
    redde stml_scribere(radix, piscina, FALSUM);
}


/* ==================================================
 * Verba
 * ================================================== */

chorda
indagatio_verba_formare (
      chorda  verba,
     Piscina* piscina)
{
    ChordaAedificator* aed;
                  i32  i;
                  i32  initium;
                  i32  k;
                  i32  termini;

    si (!piscina || verba.mensura <= ZEPHYRUM || !verba.datum)
    {
        redde vacua();
    }
    aed = chorda_aedificator_creare(piscina,
        (memoriae_index)(verba.mensura * II + XVI));
    si (!aed)
    {
        redde vacua();
    }
    termini  = ZEPHYRUM;
    i        = ZEPHYRUM;
    dum (i < verba.mensura)
    {
        dum (   i < verba.mensura
             && spatium_album((character)verba.datum[i]))
        {
            i++;
        }
        initium = i;
        dum (   i < verba.mensura
             && !spatium_album((character)verba.datum[i]))
        {
            i++;
        }
        si (!verbum_habet(verba.datum + initium, i - initium))
        {
            perge;
        }
        si (termini > ZEPHYRUM)
        {
            (vacuum)chorda_aedificator_appendere_character(aed, ' ');
        }
        (vacuum)chorda_aedificator_appendere_character(aed, '"');
        per (k = initium; k < i; k++)
        {
            si (verba.datum[k] == '"')
            {
                (vacuum)chorda_aedificator_appendere_character(aed,
                    '"');
            }
            (vacuum)chorda_aedificator_appendere_character(aed,
                (character)verba.datum[k]);
        }
        (vacuum)chorda_aedificator_appendere_literis(aed, "\"*");
        termini++;
    }
    si (termini == ZEPHYRUM)
    {
        redde vacua();
    }
    redde chorda_aedificator_finire(aed);
}


/* ==================================================
 * Index: ordines scribere
 * ================================================== */

hic_manens constans character* constans SCHEMA_INDICIS =
    "CREATE TABLE ordines ("
    " id INTEGER PRIMARY KEY,"
    " conspectus TEXT NOT NULL, clavis TEXT NOT NULL,"
    " titulus TEXT, contextus TEXT, semita TEXT);"   /* vacua = NULL */
    "CREATE INDEX ordines_clavis ON ordines(conspectus, clavis);"
    "CREATE VIRTUAL TABLE ix USING fts5(titulus, corpus,"
    " tokenize='unicode61 remove_diacritics 2');";

interior b32
exsequi_cum (
              Indagatio* ix,
     constans character* sql,
                 chorda  a,
                 chorda  b,
                    b32  cum_b)
{
     ScriniumEnuntiatum* e;
                integer  gradus;

    e = scrinium_praeparare(ix->index, sql);
    si (!e)
    {
        errorem_ponere(ix, "praeparatio", scrinium_error(ix->index));
        redde FALSUM;
    }
    (vacuum)scrinium_ligare_textum(e, I, a);
    si (cum_b)
    {
        (vacuum)scrinium_ligare_textum(e, II, b);
    }
    gradus = scrinium_gradi(e);
    scrinium_finire(e);
    si (gradus == SCRINIUM_ERROR)
    {
        errorem_ponere(ix, "scriptura", scrinium_error(ix->index));
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
clavem_delere (
     Indagatio* ix,
        chorda  conspectus,
        chorda  clavis)
{
    redde exsequi_cum(ix,
        "DELETE FROM ix WHERE rowid IN (SELECT id FROM"
            " ordines WHERE conspectus = ?1 AND clavis = ?2)",
            conspectus, clavis, VERUM)
        && exsequi_cum(ix, "DELETE FROM ordines WHERE conspectus = ?1"
            " AND clavis = ?2", conspectus, clavis, VERUM);
}

interior b32
genus_delere (
     Indagatio* ix,
        chorda  conspectus)
{
    redde exsequi_cum(ix,
        "DELETE FROM ix WHERE rowid IN (SELECT id FROM"
            " ordines WHERE conspectus = ?1)", conspectus, vacua(),
            FALSUM)
        && exsequi_cum(ix, "DELETE FROM ordines WHERE conspectus = ?1",
            conspectus, vacua(), FALSUM);
}

/* ordines clavis per conspectum (f) inserere; rescripta numerantur;
 * FALSUM si scriptura deficit (causa in ix->error) */
interior b32
clavem_inserere (
          Indagatio* ix,
    GenusQuaerendum* g,
             chorda  clavis)
{
    constans IndagatioConspectus* v;
              ScriniumEnuntiatum* e;
                             i32  i;
                             s64  id;

    v = (constans IndagatioConspectus*)conspectus_legere(
        ix->repertorium, g->titulus, clavis);
    per (i = ZEPHYRUM; v && v->ordines && i < xar_numerus(v->ordines);
         i++)
    {
        constans IndagatioOrdo* o =
            (constans IndagatioOrdo*)xar_obtinere(
            v->ordines, i);

        e = scrinium_praeparare(ix->index, "INSERT INTO ordines"
            " (conspectus, clavis, titulus, contextus, semita)"
            " VALUES (?1, ?2, ?3, ?4, ?5)");
        si (!e)
        {
            errorem_ponere(ix, "praeparatio",
                scrinium_error(ix->index));
            redde FALSUM;
        }
        (vacuum)scrinium_ligare_textum(e, I, g->nomen_c);
        (vacuum)scrinium_ligare_textum(e, II, clavis);
        (vacuum)scrinium_ligare_textum(e, III, o->titulus);
        (vacuum)scrinium_ligare_textum(e, IV, o->contextus);
        (vacuum)scrinium_ligare_textum(e, V, o->semita);
        si (scrinium_gradi(e) == SCRINIUM_ERROR)
        {
            errorem_ponere(ix, "ordo", scrinium_error(ix->index));
            scrinium_finire(e);
            redde FALSUM;
        }
        scrinium_finire(e);
        id = scrinium_ultimum_id(ix->index);
        e = scrinium_praeparare(ix->index,
            "INSERT INTO ix (rowid, titulus, corpus)"
            " VALUES (?1, ?2, ?3)");
        si (!e)
        {
            errorem_ponere(ix, "praeparatio",
                scrinium_error(ix->index));
            redde FALSUM;
        }
        (vacuum)scrinium_ligare_numerum(e, I, id);
        (vacuum)scrinium_ligare_textum(e, II, o->titulus);
        (vacuum)scrinium_ligare_textum(e, III, o->corpus);
        si (scrinium_gradi(e) == SCRINIUM_ERROR)
        {
            errorem_ponere(ix, "textus", scrinium_error(ix->index));
            scrinium_finire(e);
            redde FALSUM;
        }
        scrinium_finire(e);
        ix->rescripta++;
    }
    redde VERUM;
}

/* genus totum: deletur, claves enumeratae inseruntur (scheda propria:
 * claves enumeratae solae eam tangunt) */
interior b32
genus_scribere (
          Indagatio* ix,
    GenusQuaerendum* g)
{
     Piscina* scheda;
         Xar* claves;
         i32  i;
         b32  bene;

    si (!genus_delere(ix, g->nomen_c))
    {
        redde FALSUM;
    }
    scheda = piscina_generare_dynamicum("indagatio_scheda",
        INDAGATIO_OPUS);
    si (!scheda)
    {
        errorem_ponere(ix, "memoria", "scheda non creata");
        redde FALSUM;
    }
    claves  = conspectus_enumerare(ix->repertorium, g->titulus, scheda);
    bene    = VERUM;
    per (i = ZEPHYRUM; bene && claves && i < xar_numerus(claves); i++)
    {
        bene = clavem_inserere(ix, g, *(chorda*)xar_obtinere(claves,
            i));
    }
    piscina_destruere(scheda);
    redde bene;
}

interior vacuum
ordines_numerare (
    Indagatio* ix)
{
    ScriniumEnuntiatum* e;

    e = scrinium_praeparare(ix->index, "SELECT count(*) FROM ordines");
    si (e && scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        ix->ordines = (i64)scrinium_columna_numerus(e, ZEPHYRUM);
    }
    si (e)
    {
        scrinium_finire(e);
    }
}

interior b32
aedificare (
    Indagatio* ix)
{
    i32 i;
    b32 bene;

    ix->index = scrinium_aperire(ix->piscina, ":memory:");
    si (!ix->index)
    {
        errorem_ponere(ix, "index", "scrinium in memoria non apertum");
        redde FALSUM;
    }
    si (!scrinium_exsequi(ix->index, SCHEMA_INDICIS))
    {
        errorem_ponere(ix, "schema (FTS5?)", scrinium_error(ix->index));
        scrinium_claudere(ix->index);
        ix->index = NIHIL;
        redde FALSUM;
    }
    bene = scrinium_incipere(ix->index);
    per (i = ZEPHYRUM; bene && i < xar_numerus(ix->genera); i++)
    {
        bene = genus_scribere(ix, *(GenusQuaerendum**)xar_obtinere(
            ix->genera, i));
    }
    bene = bene && scrinium_committere(ix->index);
    si (!bene)
    {
        /* index dimidius non servatur: quaestio proxima iterum */
        scrinium_claudere(ix->index);
        ix->index = NIHIL;
        redde FALSUM;
    }
    ix->aedificatus = VERUM;
    ix->versio++;
    ordines_numerare(ix);
    redde VERUM;
}


/* ==================================================
 * Auditor et sordidae
 * ================================================== */

interior vacuum
audire (
    vacuum* ctx,
    chorda  clavis)
{
     GenusQuaerendum* g;
           Indagatio* ix;
              chorda  copia;

    g   = (GenusQuaerendum*)ctx;
    ix  = g->ix;
    /* ante aedificationem: aedificatio omnia leget */
    si (!ix->aedificatus || g->omnes)
    {
        redde;
    }
    si (clavis.mensura == ZEPHYRUM)
    {
        g->omnes = VERUM;
        redde;
    }
    si (!g->sordidae)
    {
        g->sordidae  = tabula_dispersa_creare_chorda(ix->sordes, XVI);
        g->ordo      = xar_creare(ix->sordes, (i32)magnitudo(chorda));
    }
    si (   !g->sordidae || !g->ordo
        || tabula_dispersa_continet(g->sordidae, clavis))
    {
        redde;
    }
    copia = chorda_transcribere(clavis, ix->sordes);
    (vacuum)tabula_dispersa_inserere(g->sordidae, copia, g);
    *(chorda*)xar_addere(g->ordo) = copia;
}

i32
indagatio_renovare (
    Indagatio* ix)
{
     Piscina* vetus;
         i64  ante;
         i32  i;
         i32  k;
         b32  tacta;
         b32  bene;

    si (!ix)
    {
        redde ZEPHYRUM;
    }
    ante = ix->rescripta;
    (vacuum)conspectus_renovare(ix->repertorium);
    si (!ix->aedificatus)
    {
        (vacuum)aedificare(ix);
        redde (i32)(ix->rescripta - ante);
    }
    tacta = FALSUM;
    per (i = ZEPHYRUM; i < xar_numerus(ix->genera); i++)
    {
        GenusQuaerendum* g =
            *(GenusQuaerendum**)xar_obtinere(ix->genera,
            i);

        si (g->omnes || (g->ordo && xar_numerus(g->ordo) > ZEPHYRUM))
        {
            tacta = VERUM;
        }
    }
    si (!tacta)
    {
        redde ZEPHYRUM;
    }
    /* sordes novae ante exhaustionem: auditiones intra eam (nullae
     * exspectantur) in arenam novam cadunt */
    vetus       = ix->sordes;
    ix->sordes  = piscina_generare_dynamicum("indagatio_sordes",
        IV * MXXIV);
    si (!ix->sordes)
    {
        /* sordes veteres servantur: quaestio proxima iterum */
        ix->sordes = vetus;
        errorem_ponere(ix, "memoria", "arena sordium non creata");
        redde ZEPHYRUM;
    }
    bene = scrinium_incipere(ix->index);
    per (i = ZEPHYRUM; bene && i < xar_numerus(ix->genera); i++)
    {
        GenusQuaerendum* g =
            *(GenusQuaerendum**)xar_obtinere(ix->genera,
            i);
                    Xar* ordo   = g->ordo;
                    b32  omnes  = g->omnes;

        g->sordidae  = NIHIL;
        g->ordo      = NIHIL;
        g->omnes     = FALSUM;
        si (omnes)
        {
            bene = genus_scribere(ix, g);
            perge;
        }
        per (k = ZEPHYRUM; bene && ordo && k < xar_numerus(ordo); k++)
        {
            chorda clavis = *(chorda*)xar_obtinere(ordo, k);

            bene = clavem_delere(ix, g->nomen_c, clavis)
                && clavem_inserere(ix, g, clavis);
        }
    }
    bene = bene && scrinium_committere(ix->index);
    si (!bene)
    {
        (vacuum)scrinium_revolvere(ix->index);
    }
    piscina_destruere(vetus);
    ix->versio++;
    ordines_numerare(ix);
    redde (i32)(ix->rescripta - ante);
}


/* ==================================================
 * Constructio
 * ================================================== */

Indagatio*
indagatio_creare (
                Piscina* piscina,
    ConspectusRegistrum* repertorium)
{
    Indagatio* ix;
          Xar* quaerenda;
          i32  i;

    si (!piscina || !repertorium)
    {
        redde NIHIL;
    }
    ix = (Indagatio*)piscina_allocare(piscina, magnitudo(Indagatio));
    si (!ix)
    {
        redde NIHIL;
    }
    memset(ix, ZEPHYRUM, magnitudo(Indagatio));
    ix->piscina      = piscina;
    ix->repertorium  = repertorium;
    ix->genera       = xar_creare(piscina, (i32)magnitudo(
        GenusQuaerendum*));
    ix->sordes       = piscina_generare_dynamicum("indagatio_sordes",
        IV * MXXIV);
    quaerenda = conspectus_quaerenda(repertorium, piscina);
    si (!ix->genera || !ix->sordes || !quaerenda)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(quaerenda); i++)
    {
        ConspectusQuaerendum* q = (ConspectusQuaerendum*)xar_obtinere(
            quaerenda, i);
             GenusQuaerendum* g;

        g = (GenusQuaerendum*)piscina_allocare(piscina,
            magnitudo(GenusQuaerendum));
        memset(g, ZEPHYRUM, magnitudo(GenusQuaerendum));
        g->ix = ix;
        g->nomen_c = q->titulus;
        g->titulus = chorda_ut_cstr(q->titulus, piscina);
        g->genus = q->genus;
        *(GenusQuaerendum**)xar_addere(ix->genera) = g;
        (vacuum)conspectus_auditorem_ponere(repertorium, g->titulus,
            audire, g);
    }
    redde ix;
}

vacuum
indagatio_destruere (
    Indagatio* ix)
{
    i32 i;

    si (!ix)
    {
        redde;
    }
    per (i = ZEPHYRUM; ix->genera && i < xar_numerus(ix->genera); i++)
    {
        GenusQuaerendum* g =
            *(GenusQuaerendum**)xar_obtinere(ix->genera,
            i);

        (vacuum)conspectus_auditorem_ponere(ix->repertorium, g->titulus,
            NIHIL, NIHIL);
        g->sordidae  = NIHIL;
        g->ordo      = NIHIL;
    }
    si (ix->index)
    {
        scrinium_claudere(ix->index);
        ix->index = NIHIL;
    }
    si (ix->sordes)
    {
        piscina_destruere(ix->sordes);
        ix->sordes = NIHIL;
    }
    ix->aedificatus = FALSUM;
}


/* ==================================================
 * Quaestio
 * ================================================== */

/* excerptum signatum (SIGNUM_INITII ... SIGNUM_FINIS) in partes */
interior Xar*
excerptum_partire (
      chorda  excerptum,
    Piscina* piscina)
{
     Xar* partes;
     i32  i;
     i32  initium;
     b32  intra;

    partes   = xar_creare(piscina, (i32)magnitudo(IndagatioPars));
    initium  = ZEPHYRUM;
    intra    = FALSUM;
    per (i = ZEPHYRUM; partes && i <= excerptum.mensura; i++)
    {
        character c = i
            < excerptum.mensura ? (character)excerptum.datum[i]
                                             : SIGNUM_FINIS;

        si (c != SIGNUM_INITII && c != SIGNUM_FINIS)
        {
            perge;
        }
        si (i > initium)
        {
            IndagatioPars* p = (IndagatioPars*)xar_addere(partes);

            p->textus.datum    = excerptum.datum + initium;
            p->textus.mensura  = i - initium;
            p->congruit        = intra;
        }
        intra    = (c == SIGNUM_INITII);
        initium  = i + I;
    }
    redde partes;
}

/* inventum unum (id) cum excerpto */
interior b32
inventum_legere (
            Indagatio* ix,
               chorda  match,
                  s64  id,
    IndagatioInventum* inv,
              Piscina* piscina)
{
     ScriniumEnuntiatum* e;
                    b32  bene;

    e = scrinium_praeparare(ix->index, "SELECT o.titulus, o.contextus,"
        " o.semita,"
        " snippet(ix, 1, char(2), char(3), '\xe2\x80\xa6', 12)"
        " FROM ix JOIN ordines o ON o.id = ix.rowid"
        " WHERE ix MATCH ?1 AND ix.rowid = ?2");
    si (!e)
    {
        errorem_ponere(ix, "praeparatio", scrinium_error(ix->index));
        redde FALSUM;
    }
    (vacuum)scrinium_ligare_textum(e, I, match);
    (vacuum)scrinium_ligare_numerum(e, II, id);
    bene = scrinium_gradi(e) == SCRINIUM_ORDO;
    si (bene)
    {
        inv->titulus    = scrinium_columna_textus(e, ZEPHYRUM, piscina);
        inv->contextus  = scrinium_columna_textus(e, I, piscina);
        inv->semita     = scrinium_columna_textus(e, II, piscina);
        inv->excerptum  = excerptum_partire(scrinium_columna_textus(e,
            III, piscina), piscina);
    }
    alioquin
    {
        errorem_ponere(ix, "inventum", scrinium_error(ix->index));
    }
    scrinium_finire(e);
    redde bene;
}

interior integer
greges_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans IndagatioGrex* x = (constans IndagatioGrex*)a;
    constans IndagatioGrex* y = (constans IndagatioGrex*)b;
                       s32  r;

    r = chorda_comparare(x->genus, y->genus);
    redde r < ZEPHYRUM ? -I : r > ZEPHYRUM ? I : ZEPHYRUM;
}

IndagatioFructus*
indagatio_quaerere (
                    Indagatio* ix,
    constans IndagatioPetitio* petitio,
                      Piscina* piscina)
{
        IndagatioFructus* f;
      ScriniumEnuntiatum* e;
                  chorda  match;
                     Xar* ids;          /* s64 per gregem (Xar*) */
                     i32  capacitas;
                     i32  primus;
                     i32  i;
                     i32  k;
                 integer  gradus;

    si (!ix || !petitio || !piscina)
    {
        redde NIHIL;
    }
    ix->error_positus = FALSUM;
    (vacuum)indagatio_renovare(ix);
    si (!ix->aedificatus || ix->error_positus)
    {
        redde NIHIL;
    }
    f = (IndagatioFructus*)piscina_allocare(piscina,
        magnitudo(IndagatioFructus));
    f->greges  = xar_creare(piscina, (i32)magnitudo(IndagatioGrex));
    f->versio  = ix->versio;
    match      = indagatio_verba_formare(petitio->verba, piscina);
    si (match.mensura == ZEPHYRUM)
    {
        redde f;
    }
    capacitas  = petitio->capacitas > ZEPHYRUM ? petitio->capacitas
                                              : INDAGATIO_CAPACITAS;
    primus     = petitio->conspectus.mensura > ZEPHYRUM
                 ? petitio->pagina * capacitas : ZEPHYRUM;
    ids        = xar_creare(piscina, (i32)magnitudo(Xar*));
    e = scrinium_praeparare(ix->index, "SELECT o.id, o.conspectus"
        " FROM ix JOIN ordines o ON o.id = ix.rowid"
        " WHERE ix MATCH ?1"
        " AND (coalesce(?2, '') = '' OR o.conspectus = ?2)"
        " ORDER BY " ORDO_CONGRUENTIAE ", o.id");
    si (!e)
    {
        errorem_ponere(ix, "praeparatio", scrinium_error(ix->index));
        redde NIHIL;
    }
    (vacuum)scrinium_ligare_textum(e, I, match);
    (vacuum)scrinium_ligare_textum(e, II, petitio->conspectus);
    gradus = scrinium_gradi(e);
    dum (gradus == SCRINIUM_ORDO)
    {
                  s64 id = scrinium_columna_numerus(e,
                      ZEPHYRUM);
               chorda conspectus = scrinium_columna_textus(e, I,
                   piscina);
        IndagatioGrex* g = NIHIL;
                  i32  n;

        per (k = ZEPHYRUM; k < xar_numerus(f->greges); k++)
        {
            IndagatioGrex* h = (IndagatioGrex*)xar_obtinere(f->greges,
                k);

            si (chorda_aequalis(h->conspectus, conspectus))
            {
                g = h;
                frange;
            }
        }
        si (!g)
        {
            GenusQuaerendum* q = genus_invenire(ix, conspectus);

            g              = (IndagatioGrex*)xar_addere(f->greges);
            g->conspectus  = conspectus;
            g->genus       = q ? chorda_transcribere(q->genus, piscina)
                               : conspectus;
            g->numerus     = ZEPHYRUM;
            g->inventa     = xar_creare(piscina,
                (i32)magnitudo(IndagatioInventum));
            *(Xar**)xar_addere(ids) = xar_creare(piscina,
                (i32)magnitudo(s64));
            k = xar_numerus(f->greges) - I;
        }
        n = g->numerus++;
        si (n >= primus && n < primus + capacitas)
        {
            *(s64*)xar_addere(*(Xar**)xar_obtinere(ids, k)) = id;
        }
        gradus = scrinium_gradi(e);
    }
    scrinium_finire(e);
    si (gradus == SCRINIUM_ERROR)
    {
        errorem_ponere(ix, "quaestio", scrinium_error(ix->index));
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(f->greges); k++)
    {
        IndagatioGrex* g = (IndagatioGrex*)xar_obtinere(f->greges, k);
                  Xar* l = *(Xar**)xar_obtinere(ids, k);

        per (i = ZEPHYRUM; i < xar_numerus(l); i++)
        {
            IndagatioInventum* inv = (IndagatioInventum*)xar_addere(
                g->inventa);

            si (!inventum_legere(ix, match, *(s64*)xar_obtinere(l, i),
                inv,
                    piscina))
            {
                redde NIHIL;
            }
        }
    }
    /* ordo: optimus = ordo primae apparitionis (bm25); alphabeticus */
    si (   petitio->ordo == INDAGATIO_ORDO_ALPHABETICUS
        && xar_numerus(f->greges) > I)
    {
        IndagatioGrex* tabula;
                  i32  n = xar_numerus(f->greges);

        tabula = (IndagatioGrex*)piscina_allocare(piscina,
            (memoriae_index)n * magnitudo(IndagatioGrex));
        per (k = ZEPHYRUM; k < n; k++)
        {
            tabula[k] = *(IndagatioGrex*)xar_obtinere(f->greges, k);
        }
        qsort(tabula, (size_t)n, magnitudo(IndagatioGrex),
            greges_comparare);
        per (k = ZEPHYRUM; k < n; k++)
        {
            *(IndagatioGrex*)xar_obtinere(f->greges, k) = tabula[k];
        }
    }
    redde f;
}


/* ==================================================
 * Verificatio et mensura
 * ================================================== */

/* inscriptio "conspectus/clavis" -> codex ordinum (chorda*) */
interior vacuum
codicem_addere (
    TabulaDispersa* tabula,
               Xar* inscriptiones,
            chorda  conspectus,
            chorda  clavis,
            chorda  ordo,
           Piscina* piscina)
{
    chorda  inscriptio;
    chorda* codex;
    vacuum* inventum;

    inscriptio = chorda_concatenare(chorda_concatenare(conspectus,
        chorda_ex_literis("/", piscina), piscina), clavis, piscina);
    si (tabula_dispersa_invenire(tabula, inscriptio, &inventum))
    {
        codex   = (chorda*)inventum;
        *codex  = chorda_concatenare(*codex, ordo, piscina);
        redde;
    }
    codex   = (chorda*)piscina_allocare(piscina, magnitudo(chorda));
    *codex  = ordo;
    (vacuum)tabula_dispersa_inserere(tabula, inscriptio, codex);
    *(chorda*)xar_addere(inscriptiones) = inscriptio;
}

interior chorda
ordinem_codificare (
      chorda  titulus,
      chorda  contextus,
      chorda  semita,
      chorda  corpus,
     Piscina* piscina)
{
    chorda s;
    chorda c;

    s = chorda_ex_literis("\x1f", piscina);
    c = chorda_concatenare(titulus, s, piscina);
    c = chorda_concatenare(c, contextus, piscina);
    c = chorda_concatenare(c, s, piscina);
    c = chorda_concatenare(c, semita, piscina);
    c = chorda_concatenare(c, s, piscina);
    c = chorda_concatenare(c, corpus, piscina);
    redde chorda_concatenare(c, chorda_ex_literis("\x1e", piscina),
        piscina);
}

i32
indagatio_verificare (
    Indagatio* ix,
          Xar* discrepantes)
{
            Piscina* p;
     TabulaDispersa* vivum;
     TabulaDispersa* novum;
                Xar* inscriptiones;
 ScriniumEnuntiatum* e;
                i32  i;
                i32  k;
                i32  numerus;

    si (!ix)
    {
        redde ZEPHYRUM;
    }
    (vacuum)indagatio_renovare(ix);
    si (!ix->aedificatus)
    {
        si (discrepantes)
        {
            *(chorda*)xar_addere(discrepantes) = chorda_ex_literis(
                "indagatio: index deficit", ix->piscina);
        }
        redde I;
    }
    p = piscina_generare_dynamicum("indagatio_verificatio",
        XVI * MXXIV);
    si (!p)
    {
        redde ZEPHYRUM;
    }
    vivum          = tabula_dispersa_creare_chorda(p, LXIV);
    novum          = tabula_dispersa_creare_chorda(p, LXIV);
    inscriptiones  = xar_creare(p, (i32)magnitudo(chorda));
    /* index vivus */
    e = scrinium_praeparare(ix->index, "SELECT o.conspectus, o.clavis,"
        " o.titulus, o.contextus, o.semita, ix.corpus"
        " FROM ordines o JOIN ix ON ix.rowid = o.id ORDER BY o.id");
    dum (e && scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        codicem_addere(vivum, inscriptiones,
            scrinium_columna_textus(e, ZEPHYRUM, p),
            scrinium_columna_textus(e, I, p),
            ordinem_codificare(scrinium_columna_textus(e, II, p),
                scrinium_columna_textus(e, III, p),
                scrinium_columna_textus(e, IV, p),
                scrinium_columna_textus(e, V, p), p), p);
    }
    si (e)
    {
        scrinium_finire(e);
    }
    /* ex nihilo: claves enumeratae, conspectus lecti */
    per (i = ZEPHYRUM; i < xar_numerus(ix->genera); i++)
    {
        GenusQuaerendum* g =
            *(GenusQuaerendum**)xar_obtinere(ix->genera,
            i);
                    Xar* claves;

        claves = conspectus_enumerare(ix->repertorium, g->titulus, p);
        per (k = ZEPHYRUM; claves && k < xar_numerus(claves); k++)
        {
                              chorda clavis = *(chorda*)xar_obtinere(
                                  claves, k);
            constans IndagatioConspectus* v;
                                     i32  j;

            v = (constans IndagatioConspectus*)conspectus_legere(
                ix->repertorium, g->titulus, clavis);
            per (j = ZEPHYRUM; v && v->ordines
                 && j < xar_numerus(v->ordines); j++)
            {
                constans IndagatioOrdo* o = (constans IndagatioOrdo*)
                    xar_obtinere(v->ordines, j);

                codicem_addere(novum, inscriptiones, g->nomen_c, clavis,
                    ordinem_codificare(o->titulus, o->contextus,
                    o->semita, o->corpus, p), p);
            }
        }
    }
    /* conferre: inscriptiones utriusque (duplicatae semel) */
    numerus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(inscriptiones); i++)
    {
        chorda  inscriptio  = *(chorda*)xar_obtinere(inscriptiones, i);
        vacuum* a           = NIHIL;
        vacuum* b           = NIHIL;
        chorda  x;
        chorda  y;

        (vacuum)tabula_dispersa_invenire(vivum, inscriptio, &a);
        (vacuum)tabula_dispersa_invenire(novum, inscriptio, &b);
        x = a ? *(chorda*)a : vacua();
        y = b ? *(chorda*)b : vacua();
        si (chorda_aequalis(x, y))
        {
            perge;
        }
        /* inscriptio in utraque addita: semel numeranda */
        per (k = ZEPHYRUM; k < i; k++)
        {
            si (chorda_aequalis(*(chorda*)xar_obtinere(inscriptiones,
                k),
                    inscriptio))
            {
                frange;
            }
        }
        si (k < i)
        {
            perge;
        }
        numerus++;
        si (discrepantes)
        {
            *(chorda*)xar_addere(discrepantes) = chorda_transcribere(
                inscriptio, ix->piscina);
        }
    }
    piscina_destruere(p);
    redde numerus;
}

IndagatioMensura
indagatio_mensura (
    constans Indagatio* ix)
{
    IndagatioMensura m;

    memset(&m, ZEPHYRUM, magnitudo(m));
    si (ix)
    {
        m.ordines      = ix->ordines;
        m.rescripta    = ix->rescripta;
        m.versio       = ix->versio;
        m.aedificatus  = ix->aedificatus;
    }
    redde m;
}

constans character*
indagatio_error (
    constans Indagatio* ix)
{
    redde (ix && ix->error_positus) ? ix->error : NIHIL;
}
