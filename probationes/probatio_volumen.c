/* probatio_volumen.c - Probationes Voluminis (documentum = plagula
 * sqlite una; acta veritas, plagulae/massae proiectio) */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "volumen.h"
#include "hora_hybrida.h"
#include "machina.h"
#include "scrinium.h"
#include "credo.h"
#include "memoria_processus.h"
#include "filum.h"
#include <string.h>
#include <stdlib.h>

#include <stdio.h>

/* plagula communis sectionum in area SUITAE (credo_area contextus
 * suitae, per sectiones viva; via per c->suita, lectio sola): olim
 * build/probatio_volumen.db extra vestigium membri (-journal per VFS
 * vestigii notatum) */
#define VIA_PROBATIONIS ((constans character*)c->suita)

interior b32
_continet (
    constans character* fenum,
    constans character* acus)
{
    redde (strstr(fenum, acus) != NIHIL) ? VERUM : FALSUM;
}

/* area suitae recens; sectiones sequentes plagulam VIA_PROBATIONIS
 * ordine tabulae aedificant */
interior vacuum
_suitam_parare (
    CredoContextus* c)
{
    c->datum = chorda_ut_cstr(chorda_concatenare(chorda_ex_literis(
        credo_area(c), c->piscina), chorda_ex_literis(
        "/probatio_volumen.db", c->piscina), c->piscina), c->piscina);
}


    /* ========================================================
     * PROBARE: volumen_creare - ortus + recusatio duplicati
     * ======================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Volumen* vol;
        Volumen* iterum;

        vol = volumen_creare(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);
        CREDO_VERUM(filum_existit(VIA_PROBATIONIS));

        /* actum ortus iam scriptum */
        CREDO_AEQUALIS_S64(volumen_summa_actorum(vol), (s64)1);
        CREDO_AEQUALIS_S64(volumen_summa_plagularum(vol), (s64)0);

        /* creare super exsistentem = recusatio */
        iterum = volumen_creare(piscina, VIA_PROBATIONIS);
        CREDO_NIHIL(iterum);

        volumen_claudere(vol);
    }
}


    /* ========================================================
     * PROBARE: aperire_aut_creare (ambulatio red-team 2026-08-04)
     * ======================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Volumen* vol;

        /* exsistens: aperit (actum ortus iam ibi, non iteratum) */
        vol = volumen_aperire_aut_creare(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);
        CREDO_AEQUALIS_S64(volumen_summa_actorum(vol), (s64)1);
        volumen_claudere(vol);

        /* absens (area sectionis recens): creat */
        vol = volumen_aperire_aut_creare(piscina, chorda_ut_cstr(
            chorda_concatenare(chorda_ex_literis(credo_area(c),
            piscina), chorda_ex_literis("/probatio_volumen_aut.db",
            piscina), piscina), piscina));
        CREDO_NON_NIHIL(vol);
        CREDO_AEQUALIS_S64(volumen_summa_actorum(vol), (s64)1);
        volumen_claudere(vol);
    }
}


    /* ========================================================
     * PROBARE: actum_appendere - seq crescens
     * ======================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Volumen* vol;
         chorda  datum;
            s64  seq;

        vol = volumen_aperire(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);

        datum  = chorda_ex_literis("{\"probatio\":1}", piscina);
        seq    = volumen_actum_appendere(vol, "probatio-actus", datum);
        CREDO_AEQUALIS_S64(seq, (s64)2);
        seq = volumen_actum_appendere(vol, "probatio-actus", datum);
        CREDO_AEQUALIS_S64(seq, (s64)3);
        CREDO_AEQUALIS_S64(volumen_summa_actorum(vol), (s64)3);

        volumen_claudere(vol);
    }
}


    /* ========================================================
     * PROBARE: plagulam condere/promere - circuitus integer
     * ======================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Volumen* vol;
         chorda  via;
         chorda  contentum;
         chorda  relatum;
            b32  inventum;

        vol = volumen_aperire(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);

        via = chorda_ex_literis("lib/exemplum.c", piscina);
        contentum = chorda_ex_literis(
            "/* exemplum */\ninteger quadraginta_duo = 42;\n",
            piscina);
        CREDO_VERUM(volumen_plagulam_condere(vol, via, contentum,
            "vendicata:lib/exemplum.c"));
        CREDO_AEQUALIS_S64(volumen_summa_plagularum(vol), (s64)1);

        relatum = volumen_plagulam_promere(vol, via, piscina,
            &inventum);
        CREDO_VERUM(inventum);
        CREDO_CHORDA_AEQUALIS(relatum, contentum);

        /* via ignota */
        relatum = volumen_plagulam_promere(vol,
            chorda_ex_literis("non/exsistit.c", piscina), piscina,
            &inventum);
        CREDO_FALSUM(inventum);

        volumen_claudere(vol);
    }
}


    /* ========================================================
     * PROBARE: massae dedup + upsert per viam
     * ======================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Volumen* vol;
         chorda  contentum;
         chorda  alterum;
         chorda  relatum;
            b32  inventum;
            s64  massae_ante;

        vol = volumen_aperire(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);

        contentum = chorda_ex_literis("contentum idem", piscina);
        CREDO_VERUM(volumen_plagulam_condere(vol,
            chorda_ex_literis("a.txt", piscina), contentum,
            "genita"));
        massae_ante = volumen_summa_massarum(vol);
        CREDO_VERUM(volumen_plagulam_condere(vol,
            chorda_ex_literis("b.txt", piscina), contentum,
            "genita"));
        /* contentum idem -> massa nova NULLA */
        CREDO_AEQUALIS_S64(volumen_summa_massarum(vol), massae_ante);
        CREDO_AEQUALIS_S64(volumen_summa_plagularum(vol), (s64)3);

        /* upsert: via eadem, contentum novum */
        alterum = chorda_ex_literis("contentum alterum", piscina);
        CREDO_VERUM(volumen_plagulam_condere(vol,
            chorda_ex_literis("a.txt", piscina), alterum,
            "genita"));
        CREDO_AEQUALIS_S64(volumen_summa_plagularum(vol), (s64)3);
        relatum = volumen_plagulam_promere(vol,
            chorda_ex_literis("a.txt", piscina), piscina,
            &inventum);
        CREDO_VERUM(inventum);
        CREDO_CHORDA_AEQUALIS(relatum, alterum);

        volumen_claudere(vol);
    }
}


    /* ========================================================
     * PROBARE: plagula vacua legitima
     * ======================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Volumen* vol;
         chorda  vacua;
         chorda  relatum;
            b32  inventum;

        vol = volumen_aperire(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);

        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        CREDO_VERUM(volumen_plagulam_condere(vol,
            chorda_ex_literis("vacua.txt", piscina), vacua,
            "genita"));
        relatum = volumen_plagulam_promere(vol,
            chorda_ex_literis("vacua.txt", piscina), piscina,
            &inventum);
        CREDO_VERUM(inventum);
        CREDO_AEQUALIS_I32(relatum.mensura, (i32)ZEPHYRUM);

        volumen_claudere(vol);
    }
}


    /* ========================================================
     * PROBARE: massae nudae - circuitus binarius (octeti nulli!)
     * ======================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
          Volumen* vol;
               i8  octeti[6];
           chorda  contentum;
           chorda  relatum;
        character  hex[SIGILLUM_HEX_MENSURA];
        character  hex_iterum[SIGILLUM_HEX_MENSURA];
              b32  inventum;
              s64  massae_ante;

        vol = volumen_aperire(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);

        octeti[0] = (i8)0x89;
        octeti[1] = 'P';
        octeti[2] = 0;          /* octetus nullus intra! */
        octeti[3] = 'N';
        octeti[4] = 0;
        octeti[5] = (i8)0xFF;
        contentum = chorda_ex_buffer(octeti, 6);

        CREDO_VERUM(volumen_massam_condere(vol, contentum, hex));
        CREDO_VERUM(hex[0] != '\0');

        relatum = volumen_massam_promere(vol,
            chorda_ex_literis(hex, piscina), piscina, &inventum);
        CREDO_VERUM(inventum);
        CREDO_AEQUALIS_I32(relatum.mensura, (i32)6);
        CREDO_VERUM(relatum.datum[2] == 0 && relatum.datum[4] == 0);
        CREDO_VERUM((insignatus character)relatum.datum[5] == 0xFF);

        /* dedup: contentum idem -> sigillum idem, massa nova nulla */
        massae_ante = volumen_summa_massarum(vol);
        CREDO_VERUM(volumen_massam_condere(vol, contentum,
            hex_iterum));
        CREDO_AEQUALIS_S64(volumen_summa_massarum(vol), massae_ante);
        CREDO_CHORDAE_AEQUALES(hex, hex_iterum);

        /* sigillum ignotum */
        volumen_massam_promere(vol,
            chorda_ex_literis("deadbeef", piscina), piscina,
            &inventum);
        CREDO_FALSUM(inventum);

        volumen_claudere(vol);
    }
}


    /* ========================================================
     * PROBARE: enumerare + permanentia trans aperturas
     * ======================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
               Volumen* vol;
                   Xar* manifestum;
        VolumenPlagula* p;

        vol = volumen_aperire(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);

        manifestum = volumen_plagulas_enumerare(vol, piscina);
        CREDO_NON_NIHIL(manifestum);
        CREDO_AEQUALIS_I32((i32)xar_numerus(manifestum), (i32)4);

        /* ordo per viam: a.txt primum */
        p = (VolumenPlagula*)xar_obtinere(manifestum, 0);
        CREDO_CHORDA_AEQUALIS_LITERIS(p->via, "a.txt");
        CREDO_VERUM(p->sigillum_hex.mensura == 64);
        CREDO_CHORDA_AEQUALIS_LITERIS(p->origo, "genita");

        volumen_claudere(vol);
    }
}


    /* ========================================================
     * PROBARE: plicam_ad - iter temporis replicando acta
     * ======================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
               Volumen* vol;
                   Xar* praesens;
                   Xar* manifestum;
                   Xar* vetus;
        VolumenPlagula* p;
        VolumenPlagula* q;
                   i32  index;
                   s64  ante_remotionem;

        vol = volumen_aperire(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);

        /* PINNA: plica praesens (ad 0) manifesto aequalis -
         * replicatio upsert-logicam manifesti probat */
        praesens    = volumen_plicam_ad(vol, (s64)0, piscina);
        manifestum  = volumen_plagulas_enumerare(vol, piscina);
        CREDO_NON_NIHIL(praesens);
        CREDO_NON_NIHIL(manifestum);
        CREDO_AEQUALIS_I32((i32)xar_numerus(praesens),
            (i32)xar_numerus(manifestum));
        per (index = 0; index < xar_numerus(praesens);
            index = index + 1)
        {
            p = (VolumenPlagula*)xar_obtinere(praesens, index);
            q = (VolumenPlagula*)xar_obtinere(manifestum, index);
            CREDO_CHORDA_AEQUALIS(p->via, q->via);
            CREDO_CHORDA_AEQUALIS(p->sigillum_hex, q->sigillum_hex);
        }

        /* ad seq 6 (ante upsert a.txt): a.txt contentum pristinum -
         * sigillum idem ac b.txt (ambae "contentum idem" erant);
         * vacua.txt (seq 8) nondum nata */
        vetus = volumen_plicam_ad(vol, (s64)6, piscina);
        CREDO_NON_NIHIL(vetus);
        CREDO_AEQUALIS_I32((i32)xar_numerus(vetus), (i32)3);
        {
            VolumenPlagula* a_vetus  = NIHIL;
            VolumenPlagula* b_vetus  = NIHIL;
            VolumenPlagula* a_nunc   = NIHIL;

            per (index = 0; index < xar_numerus(vetus);
                index = index + 1)
            {
                p = (VolumenPlagula*)xar_obtinere(vetus, index);
                si (chorda_aequalis_literis(p->via, "a.txt"))
                {
                    a_vetus = p;
                }
                si (chorda_aequalis_literis(p->via, "b.txt"))
                {
                    b_vetus = p;
                }
            }
            per (index = 0; index < xar_numerus(manifestum);
                index = index + 1)
            {
                q = (VolumenPlagula*)xar_obtinere(manifestum, index);
                si (chorda_aequalis_literis(q->via, "a.txt"))
                {
                    a_nunc = q;
                }
            }
            CREDO_VERUM(a_vetus != NIHIL && b_vetus != NIHIL
                && a_nunc != NIHIL);
            CREDO_CHORDA_AEQUALIS(a_vetus->sigillum_hex,
                b_vetus->sigillum_hex);
            CREDO_FALSUM(chorda_aequalis(a_vetus->sigillum_hex,
                a_nunc->sigillum_hex));
        }

        /* remotio replicata: plica praesens sine b.txt, plica ANTE
         * remotionem eam adhuc fert (historia integra) */
        ante_remotionem = volumen_summa_actorum(vol);
        CREDO_VERUM(volumen_plagulam_removere(vol,
            chorda_ex_literis("b.txt", piscina)));
        praesens = volumen_plicam_ad(vol, (s64)0, piscina);
        CREDO_NON_NIHIL(praesens);
        CREDO_AEQUALIS_I32((i32)xar_numerus(praesens), (i32)3);
        vetus = volumen_plicam_ad(vol, ante_remotionem, piscina);
        CREDO_NON_NIHIL(vetus);
        CREDO_AEQUALIS_I32((i32)xar_numerus(vetus), (i32)4);

        volumen_claudere(vol);
    }
}


/* ========================================================
 * PROBARE: acta generum datorum (historia documenti sui sola) -
 * ordo seq, post_seq exclusivum, genera exacta (non praefixa),
 * index acta_genus in plagula
 * ======================================================== */

interior vacuum
_sectio_acta_generum (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                   Volumen* vol;
                       Xar* x;
                  Scrinium* s;
        ScriniumEnuntiatum* e;
                       s64  primum;
        constans character* genera[III];
        constans character* nulla[I];

        genera[ZEPHYRUM] = "d/mutatio";
        genera[I] = "d/ramus";
        genera[II] = NIHIL;
        nulla[ZEPHYRUM] = NIHIL;
        vol = volumen_temporarium(piscina, "probatio_volumen_genera");
        CREDO_NECESSE_NON_NIHIL(vol);
        primum = volumen_actum_appendere(vol, "d/mutatio",
            chorda_ex_literis("a1", piscina));
        (vacuum)volumen_actum_appendere(vol, "e/mutatio",
            chorda_ex_literis("alienum", piscina));
        (vacuum)volumen_actum_appendere(vol, "d/ramus",
            chorda_ex_literis("r", piscina));
        (vacuum)volumen_actum_appendere(vol, "d/mutationes",
            chorda_ex_literis("praefixum", piscina));
        (vacuum)volumen_actum_appendere(vol, "d/mutatio",
            chorda_ex_literis("a2", piscina));
        x = volumen_acta_generum_legere(vol, ZEPHYRUM, genera, piscina);
        CREDO_NECESSE_NON_NIHIL(x);
        CREDO_AEQUALIS_I32(xar_numerus(x), III);
        CREDO_CHORDA_AEQUALIS_LITERIS(((VolumenActum*)xar_obtinere(x,
            ZEPHYRUM))->datum, "a1");
        CREDO_CHORDA_AEQUALIS_LITERIS(((VolumenActum*)xar_obtinere(x,
            I))
            ->genus, "d/ramus");
        CREDO_CHORDA_AEQUALIS_LITERIS(((VolumenActum*)xar_obtinere(x,
            II))
            ->datum, "a2");
        CREDO_VERUM(((VolumenActum*)xar_obtinere(x, ZEPHYRUM))->seq
            < ((VolumenActum*)xar_obtinere(x, I))->seq);
        /* post_seq exclusivum */
        x = volumen_acta_generum_legere(vol, primum, genera, piscina);
        CREDO_AEQUALIS_I32(xar_numerus(x), II);
        CREDO_CHORDA_AEQUALIS_LITERIS(((VolumenActum*)xar_obtinere(x,
            ZEPHYRUM))->datum, "r");
        /* genera nulla: nihil */
        x = volumen_acta_generum_legere(vol, ZEPHYRUM, nulla, piscina);
        CREDO_NECESSE_NON_NIHIL(x);
        CREDO_AEQUALIS_I32(xar_numerus(x), ZEPHYRUM);
        /* index in plagula (migratio) */
        s = scrinium_aperire(piscina, volumen_via(vol));
        CREDO_NECESSE_NON_NIHIL(s);
        e = scrinium_praeparare(s, "SELECT COUNT(*) FROM sqlite_master"
            " WHERE type = 'index' AND name = 'acta_genus'");
        CREDO_NECESSE_NON_NIHIL(e);
        CREDO_VERUM(scrinium_gradi(e) == SCRINIUM_ORDO);
        CREDO_AEQUALIS_S64(scrinium_columna_numerus(e, ZEPHYRUM),
            (s64)I);
        scrinium_finire(e);
        scrinium_claudere(s);
        volumen_claudere(vol);
    }
}


/* ========================================================
 * PROBARE: praesentia (D1) - ponere/legere, scriptor ultimus
 * vincit, valor vacuus legitimus, NEC acta NEC massae crescunt
 * ======================================================== */

interior vacuum
_sectio_praesentiae (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Volumen* vol;
         chorda  relatum;
         chorda  vacua;
            b32  inventum;
            s64  acta_ante;
            s64  massae_ante;


        vol = volumen_temporarium(piscina, "probatio_volumen_praesens");
        CREDO_NECESSE_NON_NIHIL(vol);
        acta_ante    = volumen_summa_actorum(vol);
        massae_ante  = volumen_summa_massarum(vol);

        relatum = volumen_praesens_legere(vol,
            chorda_ex_literis("status/a", piscina), piscina, &inventum);
        CREDO_FALSUM(inventum);

        CREDO_VERUM(volumen_praesens_ponere(vol,
            chorda_ex_literis("status/a", piscina),
            chorda_ex_literis("primum", piscina)));
        CREDO_VERUM(volumen_praesens_ponere(vol,
            chorda_ex_literis("status/b", piscina),
            chorda_ex_literis("alterum", piscina)));
        CREDO_VERUM(volumen_praesens_ponere(vol,
            chorda_ex_literis("status/a", piscina),
            chorda_ex_literis("secundum", piscina)));

        relatum = volumen_praesens_legere(vol,
            chorda_ex_literis("status/a", piscina), piscina, &inventum);
        CREDO_VERUM(inventum);
        CREDO_CHORDA_AEQUALIS_LITERIS(relatum, "secundum");
        relatum = volumen_praesens_legere(vol,
            chorda_ex_literis("status/b", piscina), piscina, &inventum);
        CREDO_VERUM(inventum);
        CREDO_CHORDA_AEQUALIS_LITERIS(relatum, "alterum");

        /* valor vacuus: inventus, mensura nulla */
        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        CREDO_VERUM(volumen_praesens_ponere(vol,
            chorda_ex_literis("status/vacua", piscina), vacua));
        relatum = volumen_praesens_legere(vol,
            chorda_ex_literis("status/vacua", piscina), piscina,
            &inventum);
        CREDO_VERUM(inventum);
        CREDO_AEQUALIS_I32(relatum.mensura, (i32)ZEPHYRUM);

        /* extra acta: diarium et massae intacta, manifestum quoque */
        CREDO_AEQUALIS_S64(volumen_summa_actorum(vol), acta_ante);
        CREDO_AEQUALIS_S64(volumen_summa_massarum(vol), massae_ante);
        CREDO_AEQUALIS_S64(volumen_summa_plagularum(vol), (s64)0);
        volumen_claudere(vol);
    }
}


/* ========================================================
 * PROBARE: migratio VI - volumen formae veteris (migrationes
 * I-IV tantum) instantanea status/ et vicus/latera in praesentiam
 * semel copiat; plagulae aliae non; plagulae ipsae manent
 * ======================================================== */

/* via in area sectionis (credo_area): plagulae probationis olim sub
 * build/ extra vestigium membri (lineae S, quaestio 4GY9MJ4) */
interior constans character*
_in_area (
         CredoContextus* c,
     constans character* titulus)
{
    redde chorda_ut_cstr(chorda_concatenare(chorda_ex_literis(
        credo_area(c), c->piscina), chorda_ex_literis(titulus,
        c->piscina), c->piscina), c->piscina);
}


/* ==================================================
 * communio D2b: identitas actus = (hora, replica); replica propria
 * cuique volumini (ULID), servata trans aperturas, in omni actu et in
 * basi
 * ================================================== */

interior vacuum
_sectio_replicae (
    CredoContextus* c)
{
                Piscina* piscina;
     constans character* via;
                Volumen* vol;
                 chorda  replica;
       VolumenIdentitas  prima;
       VolumenIdentitas  secunda;
                    Xar* acta;
                    s64  seq_primum;
                    s64  seq_secundum;

    piscina  = c->piscina;
    via      = _in_area(c, "/replica.volumen");
    vol      = volumen_creare(piscina, via);
    CREDO_NECESSE_NON_NIHIL(vol);
    replica = volumen_replica(vol);
    CREDO_AEQUALIS_I32(replica.mensura, XXVI);
    replica = chorda_ex_literis(chorda_ut_cstr(replica, piscina),
        piscina);
    seq_primum = volumen_actum_appendere(vol, "r/actum",
        chorda_ex_literis("{}", piscina));
    CREDO_NECESSE(volumen_actum_identitas(vol, seq_primum, piscina,
        &prima));
    CREDO_CHORDA_AEQUALIS(prima.replica, replica);
    CREDO_CHORDA_AEQUALIS(prima.machina, machina_identitas(piscina));
    seq_secundum = volumen_actum_super_appendere(vol, "r/actum",
        chorda_ex_literis("{}", piscina), prima);
    CREDO_NECESSE(volumen_actum_identitas(vol, seq_secundum, piscina,
        &secunda));
    CREDO_CHORDA_AEQUALIS(secunda.replica, replica);
    acta = volumen_acta_legere(vol, seq_primum, piscina);
    CREDO_NECESSE_NON_NIHIL(acta);
    CREDO_NECESSE(xar_numerus(acta) == I);
    CREDO_CHORDA_AEQUALIS(((VolumenActum*)xar_obtinere(acta,
        ZEPHYRUM))->identitas.replica, replica);
    CREDO_CHORDA_AEQUALIS(((VolumenActum*)xar_obtinere(acta,
        ZEPHYRUM))->basis.replica, replica);
    CREDO_AEQUALIS_S64(((VolumenActum*)xar_obtinere(acta,
        ZEPHYRUM))->basis.hora, prima.hora);
    volumen_claudere(vol);

    CREDO_GRADUS("reapertio: replica eadem");
    vol = volumen_aperire(piscina, via);
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_CHORDA_AEQUALIS(volumen_replica(vol), replica);
    volumen_claudere(vol);
}

/* duo volumina eiusdem machinae: replicae diversae, identitates
 * diversae etiam si horae concurrunt (Review Focus 1) */
interior vacuum
_sectio_replicarum_distinctarum (
    CredoContextus* c)
{
             Piscina* piscina;
             Volumen* a;
             Volumen* b;
    VolumenIdentitas  ia;
    VolumenIdentitas  ib;
                 s64  sa;
                 s64  sb;

    piscina = c->piscina;
    a = volumen_temporarium(piscina, "probatio_volumen_replica_a");
    b = volumen_temporarium(piscina, "probatio_volumen_replica_b");
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_NECESSE_NON_NIHIL(b);
    CREDO_VERUM(!chorda_aequalis(volumen_replica(a),
        volumen_replica(b)));
    sa = volumen_actum_appendere(a, "r/actum",
        chorda_ex_literis("{}", piscina));
    sb = volumen_actum_appendere(b, "r/actum",
        chorda_ex_literis("{}", piscina));
    CREDO_NECESSE(volumen_actum_identitas(a, sa, piscina, &ia));
    CREDO_NECESSE(volumen_actum_identitas(b, sb, piscina, &ib));
    CREDO_CHORDA_AEQUALIS(ia.machina, ib.machina);
    CREDO_VERUM(!chorda_aequalis(ia.replica, ib.replica));
    volumen_claudere(a);
    volumen_claudere(b);
}

/* volumen ante communionem (sine columna replicae): aperitur, acta
 * vetera replicam huius voluminis accipiunt (Review Focus 5) */
interior vacuum
_sectio_replicae_veteris (
    CredoContextus* c)
{
                Piscina* piscina;
     constans character* via_vetus;
    constans character* constans veteres[I] = {
        "CREATE TABLE acta ("
        " seq INTEGER PRIMARY KEY AUTOINCREMENT,"
        " momentum TEXT NOT NULL"
        "   DEFAULT (strftime('%Y-%m-%dT%H:%M:%fZ','now')),"
        " genus TEXT NOT NULL,"
        " datum TEXT NOT NULL)"
    };
           Scrinium* s;
            Volumen* vol;
                Xar* acta;
                i32  i;

    piscina    = c->piscina;
    via_vetus  = _in_area(c, "/replica_vetus.db");
    s          = scrinium_aperire(piscina, via_vetus);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_VERUM(scrinium_migrare(s, veteres, I));
    CREDO_VERUM(scrinium_exsequi(s,
        "INSERT INTO acta (momentum, genus, datum) VALUES"
        " ('2026-10-20T10:00:00.000Z', 'a', '{}'),"
        " ('2026-10-20T10:00:01.000Z', 'b', '{}')"));
    scrinium_claudere(s);
    vol = volumen_aperire(piscina, via_vetus);
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_AEQUALIS_I32(volumen_replica(vol).mensura, XXVI);
    acta = volumen_acta_legere(vol, ZEPHYRUM, piscina);
    CREDO_NECESSE_NON_NIHIL(acta);
    CREDO_AEQUALIS_I32(xar_numerus(acta), II);
    per (i = ZEPHYRUM; i < xar_numerus(acta); i++)
    {
        CREDO_CHORDA_AEQUALIS(((VolumenActum*)xar_obtinere(acta, i))
            ->identitas.replica, volumen_replica(vol));
    }
    CREDO_VERUM(volumen_actum_appendere(vol, "c",
        chorda_ex_literis("{}", piscina)) > ZEPHYRUM);
    volumen_claudere(vol);
}

/* communio II.1: actum alienum importare - identitas, momentum,
 * genus, datum servantur, seq novum, seq_originis scriptoris;
 * idempotens; conflictus; proprium recusatur */
interior VolumenActum
actum_ultimum (
     Volumen* vol,
     Piscina* piscina)
{
             Xar* l;
    VolumenActum  vacuum_actum;

    memset(&vacuum_actum, ZEPHYRUM, magnitudo(vacuum_actum));
    l = volumen_acta_legere(vol, ZEPHYRUM, piscina);
    si (!l || xar_numerus(l) == ZEPHYRUM)
    {
        redde vacuum_actum;
    }
    redde *(VolumenActum*)xar_obtinere(l, xar_numerus(l) - I);
}

interior vacuum
_sectio_importationis (
    CredoContextus* c)
{
         Piscina* piscina;
         Volumen* a;
         Volumen* b;
    VolumenActum  originale;
    VolumenActum  copia;
    VolumenActum  mutatum;
             s64  numerus;

    piscina  = c->piscina;
    a        = volumen_temporarium(piscina, "probatio_volumen_imp_a");
    b        = volumen_temporarium(piscina, "probatio_volumen_imp_b");
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_NECESSE_NON_NIHIL(b);
    (vacuum)volumen_actum_appendere(b, "x/proprium",
        chorda_ex_literis("{\"b\":1}", piscina));
    CREDO_NECESSE(volumen_actum_appendere(a, "x/actum",
        chorda_ex_literis("{\"n\":1}", piscina)) > ZEPHYRUM);
    originale = actum_ultimum(a, piscina);
    CREDO_AEQUALIS_S64(originale.seq_originis, originale.seq);

    CREDO_GRADUS("importatum: identitas et contentum servata");
    numerus = volumen_summa_actorum(b);
    CREDO_AEQUALIS_I32((i32)volumen_actum_importare(b, &originale),
        (i32)VOLUMEN_IMPORTATUM);
    CREDO_AEQUALIS_S64(volumen_summa_actorum(b), numerus + I);
    copia = actum_ultimum(b, piscina);
    CREDO_AEQUALIS_S64(copia.identitas.hora, originale.identitas.hora);
    CREDO_CHORDA_AEQUALIS(copia.identitas.replica,
        volumen_replica(a));
    CREDO_CHORDA_AEQUALIS(copia.identitas.machina,
        originale.identitas.machina);
    CREDO_CHORDA_AEQUALIS(copia.momentum, originale.momentum);
    CREDO_CHORDA_AEQUALIS(copia.genus, originale.genus);
    CREDO_CHORDA_AEQUALIS(copia.datum, originale.datum);
    CREDO_AEQUALIS_S64(copia.seq_originis, originale.seq);
    CREDO_VERUM(copia.seq != originale.seq || copia.seq > ZEPHYRUM);

    CREDO_GRADUS("relatio: numeratio scriptoris servatur (A-B-C)");
    {
         Volumen* cc;
    VolumenActum  relatum;

        cc = volumen_temporarium(piscina, "probatio_volumen_imp_c");
        CREDO_NECESSE_NON_NIHIL(cc);
        (vacuum)volumen_actum_appendere(cc, "x/proprium",
            chorda_ex_literis("{}", piscina));
        (vacuum)volumen_actum_appendere(cc, "x/proprium",
            chorda_ex_literis("{}", piscina));
        CREDO_VERUM(copia.seq != copia.seq_originis);
        CREDO_AEQUALIS_I32((i32)volumen_actum_importare(cc, &copia),
            (i32)VOLUMEN_IMPORTATUM);
        relatum = actum_ultimum(cc, piscina);
        CREDO_AEQUALIS_S64(relatum.seq_originis, originale.seq);
        CREDO_CHORDA_AEQUALIS(relatum.identitas.replica,
            volumen_replica(a));
        volumen_claudere(cc);
    }

    CREDO_GRADUS("idempotens");
    CREDO_AEQUALIS_I32((i32)volumen_actum_importare(b, &originale),
        (i32)VOLUMEN_IAM_ADEST);
    CREDO_AEQUALIS_S64(volumen_summa_actorum(b), numerus + I);

    CREDO_GRADUS("conflictus: identitas eadem, datum aliud");
    mutatum        = originale;
    mutatum.datum  = chorda_ex_literis("{\"n\":2}", piscina);
    CREDO_AEQUALIS_I32((i32)volumen_actum_importare(b, &mutatum),
        (i32)VOLUMEN_CONFLICTUS);
    CREDO_AEQUALIS_S64(volumen_summa_actorum(b), numerus + I);

    CREDO_GRADUS("proprium recusatur");
    numerus = volumen_summa_actorum(b);
    (vacuum)volumen_actum_appendere(b, "x/proprium",
        chorda_ex_literis("{\"b\":2}", piscina));
    mutatum = actum_ultimum(b, piscina);
    CREDO_AEQUALIS_I32((i32)volumen_actum_importare(b, &mutatum),
        (i32)VOLUMEN_IMPORTATIO_MALA);
    CREDO_AEQUALIS_S64(volumen_summa_actorum(b), numerus + I);
    volumen_claudere(a);
    volumen_claudere(b);
}

/* communio VIII-c: hora futura - intra limitem accipitur et horologium
 * locale eam praeterit; ultra limitem recusatur (Review Focus 2) */
interior vacuum
_sectio_horae_futurae (
    CredoContextus* c)
{
         Piscina* piscina;
         Volumen* b;
    VolumenActum  alienum;
    VolumenActum  proprium;
        Momentum  nunc;
             s64  numerus;

    piscina  = c->piscina;
    b        = volumen_temporarium(piscina, "probatio_volumen_futura");
    CREDO_NECESSE_NON_NIHIL(b);
    nunc = hora_hybrida_physica();
    memset(&alienum, ZEPHYRUM, magnitudo(alienum));
    alienum.seq           = VII;
    alienum.seq_originis  = VII;
    alienum.momentum           = chorda_ex_literis(
        "2026-10-26T12:00:00.000Z", piscina);
    alienum.genus = chorda_ex_literis("x/actum", piscina);
    alienum.datum = chorda_ex_literis("{}", piscina);
    alienum.identitas.replica  = chorda_ex_literis(
        "01ZZZZZZZZZZZZZZZZZZZZZZZZ", piscina);
    alienum.identitas.machina  = chorda_ex_literis("aliena", piscina);
    alienum.identitas.hora     = hora_hybrida_componere(nunc
        + (Momentum)(V * LX * M), ZEPHYRUM);

    CREDO_GRADUS("V minutae in futuro: accipitur");
    CREDO_AEQUALIS_I32((i32)volumen_actum_importare(b, &alienum),
        (i32)VOLUMEN_IMPORTATUM);
    CREDO_NECESSE(volumen_actum_appendere(b, "x/proprium",
        chorda_ex_literis("{}", piscina)) > ZEPHYRUM);
    proprium = actum_ultimum(b, piscina);
    CREDO_VERUM(proprium.identitas.hora > alienum.identitas.hora);

    CREDO_GRADUS("XX minutae in futuro: recusatur");
    numerus               = volumen_summa_actorum(b);
    alienum.seq           = VIII;
    alienum.seq_originis  = VIII;
    alienum.identitas.hora  = hora_hybrida_componere(nunc
        + (Momentum)(XX * LX * M), ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)volumen_actum_importare(b, &alienum),
        (i32)VOLUMEN_HORA_FUTURA);
    CREDO_AEQUALIS_S64(volumen_summa_actorum(b), numerus);
    volumen_claudere(b);
}

/* communio II.1: plagula aliena in manifestum sub
 * "aliena/<replica>/<via>" - propria intacta; massa prius necessaria;
 * remota aliena tollit */
interior vacuum
_sectio_plagulae_alienae (
    CredoContextus* c)
{
         Piscina* piscina;
         Volumen* a;
         Volumen* b;
    VolumenActum  condita;
    VolumenActum  remota;
          chorda  via_aliena;
          chorda  lectum;
             b32  inventum;
       character  hex[SIGILLUM_HEX_MENSURA];

    piscina  = c->piscina;
    a        = volumen_temporarium(piscina, "probatio_volumen_plag_a");
    b        = volumen_temporarium(piscina, "probatio_volumen_plag_b");
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_NECESSE_NON_NIHIL(b);
    CREDO_NECESSE(volumen_plagulam_condere(b, chorda_ex_literis(
        "documentum", piscina), chorda_ex_literis("B", piscina),
        "probatio"));
    CREDO_NECESSE(volumen_plagulam_condere(a, chorda_ex_literis(
        "documentum", piscina), chorda_ex_literis("A", piscina),
        "probatio"));
    condita = actum_ultimum(a, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(condita.genus, "plagula-condita");
    via_aliena = chorda_concatenare(chorda_concatenare(
        chorda_ex_literis("aliena/", piscina), volumen_replica(a),
        piscina), chorda_ex_literis("/documentum", piscina), piscina);

    CREDO_GRADUS("massa deest: nihil scribitur");
    CREDO_AEQUALIS_I32((i32)volumen_actum_importare(b, &condita),
        (i32)VOLUMEN_MASSA_DEEST);
    (vacuum)volumen_plagulam_promere(b, via_aliena, piscina, &inventum);
    CREDO_FALSUM(inventum);

    CREDO_GRADUS("massa praesens: plagula aliena, propria intacta");
    CREDO_NECESSE(volumen_massam_condere(b, chorda_ex_literis("A",
        piscina), hex));
    CREDO_AEQUALIS_I32((i32)volumen_actum_importare(b, &condita),
        (i32)VOLUMEN_IMPORTATUM);
    lectum = volumen_plagulam_promere(b, via_aliena, piscina,
        &inventum);
    CREDO_VERUM(inventum);
    CREDO_CHORDA_AEQUALIS_LITERIS(lectum, "A");
    lectum = volumen_plagulam_promere(b, chorda_ex_literis("documentum",
        piscina), piscina, &inventum);
    CREDO_CHORDA_AEQUALIS_LITERIS(lectum, "B");

    CREDO_GRADUS("remota aliena tollit");
    CREDO_NECESSE(volumen_plagulam_removere(a, chorda_ex_literis(
        "documentum", piscina)));
    remota = actum_ultimum(a, piscina);
    CREDO_AEQUALIS_I32((i32)volumen_actum_importare(b, &remota),
        (i32)VOLUMEN_IMPORTATUM);
    (vacuum)volumen_plagulam_promere(b, via_aliena, piscina, &inventum);
    CREDO_FALSUM(inventum);
    lectum = volumen_plagulam_promere(b, chorda_ex_literis("documentum",
        piscina), piscina, &inventum);
    CREDO_CHORDA_AEQUALIS_LITERIS(lectum, "B");
    volumen_claudere(a);
    volumen_claudere(b);
}

interior vacuum
_sectio_migrationis_praesentiae (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* via_vetus = _in_area(c, "/vetus.db");
        constans character* constans veteres[IV] = {
            "CREATE TABLE acta ("
            " seq INTEGER PRIMARY KEY AUTOINCREMENT,"
            " momentum TEXT NOT NULL"
            "   DEFAULT (strftime('%Y-%m-%dT%H:%M:%fZ','now')),"
            " genus TEXT NOT NULL,"
            " datum TEXT NOT NULL)",
            "CREATE TABLE massae ("
            " sigillum TEXT PRIMARY KEY,"
            " contentum BLOB NOT NULL)",
            "CREATE TABLE plagulae ("
            " via TEXT PRIMARY KEY,"
            " sigillum TEXT NOT NULL REFERENCES massae(sigillum),"
            " origo TEXT NOT NULL,"
            " momentum TEXT NOT NULL"
            "   DEFAULT (strftime('%Y-%m-%dT%H:%M:%fZ','now')))",
            "CREATE INDEX acta_genus ON acta (genus, seq)"
        };
         Scrinium* s;
          Volumen* vol;
           chorda  relatum;
              b32  inventum;


        si (filum_existit(via_vetus))
        {
            filum_delere(via_vetus);
        }
        s = scrinium_aperire(piscina, via_vetus);
        CREDO_NECESSE_NON_NIHIL(s);
        CREDO_VERUM(scrinium_migrare(s, veteres, IV));
        CREDO_VERUM(scrinium_exsequi(s,
            "INSERT INTO massae (sigillum, contentum) VALUES"
            " ('s1', 'status-contentum'), ('s2', 'index-laterum'),"
            " ('s3', 'alienum')"));
        CREDO_VERUM(scrinium_exsequi(s,
            "INSERT INTO plagulae (via, sigillum, origo) VALUES"
            " ('status/x', 's1', 'conservatio'),"
            " ('vicus/latera', 's2', 'vicus'),"
            " ('lib/a.c', 's3', 'genita'),"
            " ('statusque', 's3', 'genita')"));
        scrinium_claudere(s);

        vol = volumen_aperire(piscina, via_vetus);
        CREDO_NECESSE_NON_NIHIL(vol);
        relatum = volumen_praesens_legere(vol,
            chorda_ex_literis("status/x", piscina), piscina, &inventum);
        CREDO_VERUM(inventum);
        CREDO_CHORDA_AEQUALIS_LITERIS(relatum, "status-contentum");
        relatum = volumen_praesens_legere(vol,
            chorda_ex_literis("vicus/latera", piscina), piscina,
            &inventum);
        CREDO_VERUM(inventum);
        CREDO_CHORDA_AEQUALIS_LITERIS(relatum, "index-laterum");
        (vacuum)volumen_praesens_legere(vol,
            chorda_ex_literis("lib/a.c", piscina), piscina, &inventum);
        CREDO_FALSUM(inventum);
        (vacuum)volumen_praesens_legere(vol,
            chorda_ex_literis("statusque", piscina), piscina,
            &inventum);
        CREDO_FALSUM(inventum);

        /* plagula vetus manet (acta eius historia sunt) */
        relatum = volumen_plagulam_promere(vol,
            chorda_ex_literis("status/x", piscina), piscina, &inventum);
        CREDO_VERUM(inventum);
        CREDO_CHORDA_AEQUALIS_LITERIS(relatum, "status-contentum");

        /* semel: valor novus apertione iterata non obruitur */
        CREDO_VERUM(volumen_praesens_ponere(vol,
            chorda_ex_literis("status/x", piscina),
            chorda_ex_literis("novum", piscina)));
        volumen_claudere(vol);
        vol = volumen_aperire(piscina, via_vetus);
        CREDO_NECESSE_NON_NIHIL(vol);
        relatum = volumen_praesens_legere(vol,
            chorda_ex_literis("status/x", piscina), piscina, &inventum);
        CREDO_VERUM(inventum);
        CREDO_CHORDA_AEQUALIS_LITERIS(relatum, "novum");
        volumen_claudere(vol);
        filum_delere(via_vetus);
    }
}


/* ========================================================
 * PROBARE: identitas actus (D2) - hora stricte crescens ordine
 * seq etiam intra millisecundam, prope tempus parietis; machina
 * = identitas huius machinae
 * ======================================================== */

interior vacuum
_sectio_horae (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
              Volumen* vol;
                  Xar* acta;
         VolumenActum* a;
               chorda  machina;
                  i32  i;
                  s64  prior;
             Momentum  nunc;


        vol = volumen_temporarium(piscina, "probatio_volumen_hora");
        CREDO_NECESSE_NON_NIHIL(vol);
        per (i = ZEPHYRUM; i < L; i++)
        {
            CREDO_VERUM(volumen_actum_appendere(vol, "celere",
                chorda_ex_literis("{}", piscina)) > ZEPHYRUM);
        }
        nunc     = hora_hybrida_physica();
        machina  = machina_identitas(piscina);
        CREDO_NECESSE(machina.mensura > ZEPHYRUM);
        acta = volumen_acta_legere(vol, ZEPHYRUM, piscina);
        CREDO_NECESSE_NON_NIHIL(acta);
        CREDO_AEQUALIS_I32(xar_numerus(acta), (i32)LI);
        prior = ZEPHYRUM;
        per (i = ZEPHYRUM; i < xar_numerus(acta); i++)
        {
            a = (VolumenActum*)xar_obtinere(acta, i);
            CREDO_VERUM(a->identitas.hora > prior);
            CREDO_CHORDA_AEQUALIS(a->identitas.machina, machina);
            CREDO_VERUM(hora_hybrida_momentum(a->identitas.hora) > nunc
                - LX * M);
            CREDO_VERUM(hora_hybrida_momentum(a->identitas.hora)
                <= nunc);
            prior = a->identitas.hora;
        }
        volumen_claudere(vol);
    }
}


/* ========================================================
 * PROBARE: basis actus (D2b) - super_appendere basim servat,
 * appendere simplex sine basi; identitas per seq lectori aequalis
 * ======================================================== */

interior vacuum
_sectio_basis (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                 Volumen* vol;
                     Xar* acta;
            VolumenActum* a;
        VolumenIdentitas  prima;
        VolumenIdentitas  iterum;
                     s64  seq_primum;
                     s64  seq_secundum;


        vol = volumen_temporarium(piscina, "probatio_volumen_basis");
        CREDO_NECESSE_NON_NIHIL(vol);
        seq_primum = volumen_actum_appendere(vol, "d/mutatio",
            chorda_ex_literis("primum", piscina));
        CREDO_NECESSE(seq_primum > ZEPHYRUM);
        CREDO_NECESSE(volumen_actum_identitas(vol, seq_primum, piscina,
            &prima));
        CREDO_VERUM(prima.hora > ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS(prima.machina,
            machina_identitas(piscina));
        seq_secundum = volumen_actum_super_appendere(vol, "d/mutatio",
            chorda_ex_literis("secundum", piscina), prima);
        CREDO_NECESSE(seq_secundum > seq_primum);
        /* seq ignotum */
        CREDO_FALSUM(volumen_actum_identitas(vol, seq_secundum + C,
            piscina, &iterum));
        /* actum non ultimum: identitas SUA, non posterioris */
        CREDO_NECESSE(volumen_actum_identitas(vol, seq_primum, piscina,
            &iterum));
        CREDO_AEQUALIS_S64(iterum.hora, prima.hora);

        acta = volumen_acta_legere(vol, seq_primum - I, piscina);
        CREDO_NECESSE_NON_NIHIL(acta);
        CREDO_NECESSE(xar_numerus(acta) == II);
        a = (VolumenActum*)xar_obtinere(acta, ZEPHYRUM);
        CREDO_AEQUALIS_S64(a->identitas.hora, prima.hora);
        CREDO_AEQUALIS_S64(a->basis.hora, (s64)ZEPHYRUM);
        a = (VolumenActum*)xar_obtinere(acta, I);
        CREDO_AEQUALIS_S64(a->basis.hora, prima.hora);
        CREDO_CHORDA_AEQUALIS(a->basis.machina, prima.machina);
        CREDO_NECESSE(volumen_actum_identitas(vol, seq_secundum,
            piscina,
            &iterum));
        CREDO_AEQUALIS_S64(iterum.hora, a->identitas.hora);
        CREDO_CHORDA_AEQUALIS(iterum.machina, a->identitas.machina);
        /* lector generum basim quoque fert */
        {
            constans character* genera[II];

            genera[ZEPHYRUM]  = "d/mutatio";
            genera[I]         = NIHIL;
            acta = volumen_acta_generum_legere(vol, seq_primum, genera,
                piscina);
            CREDO_NECESSE(acta && xar_numerus(acta) == I);
            CREDO_AEQUALIS_S64(((VolumenActum*)xar_obtinere(acta,
                ZEPHYRUM))->basis.hora, prima.hora);
        }
        volumen_claudere(vol);
    }
}


/* ========================================================
 * PROBARE: migratio VII-IX - acta vetera supplentur ordine seq:
 * millisecunda eadem et horologium retro saltans numeratorem
 * augent; actum a codice vetere post migrationem scriptum (hora
 * NIHIL) apertione sequenti suppletur
 * ======================================================== */

interior vacuum
_sectio_supplementi_horarum (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* via_vetus = _in_area(c, "/horae.db");
        constans character* constans veteres[IV] = {
            "CREATE TABLE acta ("
            " seq INTEGER PRIMARY KEY AUTOINCREMENT,"
            " momentum TEXT NOT NULL"
            "   DEFAULT (strftime('%Y-%m-%dT%H:%M:%fZ','now')),"
            " genus TEXT NOT NULL,"
            " datum TEXT NOT NULL)",
            "CREATE TABLE massae ("
            " sigillum TEXT PRIMARY KEY,"
            " contentum BLOB NOT NULL)",
            "CREATE TABLE plagulae ("
            " via TEXT PRIMARY KEY,"
            " sigillum TEXT NOT NULL REFERENCES massae(sigillum),"
            " origo TEXT NOT NULL,"
            " momentum TEXT NOT NULL"
            "   DEFAULT (strftime('%Y-%m-%dT%H:%M:%fZ','now')))",
            "CREATE INDEX acta_genus ON acta (genus, seq)"
        };
            Scrinium* s;
             Volumen* vol;
                 Xar* acta;
        VolumenActum* a;
              chorda  machina;
            Momentum  decima;


        decima   = (Momentum)1792490400 * M;   /* MMXXVI-X-XX X:00 */
        machina  = machina_identitas(piscina);
        si (filum_existit(via_vetus))
        {
            filum_delere(via_vetus);
        }
        s = scrinium_aperire(piscina, via_vetus);
        CREDO_NECESSE_NON_NIHIL(s);
        CREDO_VERUM(scrinium_migrare(s, veteres, IV));
        CREDO_VERUM(scrinium_exsequi(s,
            "INSERT INTO acta (momentum, genus, datum) VALUES"
            " ('2026-10-20T10:00:00.000Z', 'a', '{}'),"
            " ('2026-10-20T10:00:00.000Z', 'b', '{}'),"
            " ('2026-10-20T09:59:59.000Z', 'c', '{}'),"
            " ('2026-10-20T10:00:05.250Z', 'd', '{}')"));
        scrinium_claudere(s);

        vol = volumen_aperire(piscina, via_vetus);
        CREDO_NECESSE_NON_NIHIL(vol);
        acta = volumen_acta_legere(vol, ZEPHYRUM, piscina);
        CREDO_NECESSE_NON_NIHIL(acta);
        CREDO_AEQUALIS_I32(xar_numerus(acta), IV);
        a = (VolumenActum*)xar_obtinere(acta, ZEPHYRUM);
        CREDO_AEQUALIS_S64(a->identitas.hora,
            hora_hybrida_componere(decima,
            ZEPHYRUM));
        /* acta ante D2b: basis ignota */
        CREDO_AEQUALIS_S64(a->basis.hora, (s64)ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS(a->identitas.machina, machina);
        a = (VolumenActum*)xar_obtinere(acta, I);
        CREDO_AEQUALIS_S64(a->identitas.hora,
            hora_hybrida_componere(decima, I));
        /* horologium retro: millisecunda prior, numerator tamen */
        a = (VolumenActum*)xar_obtinere(acta, II);
        CREDO_AEQUALIS_S64(a->identitas.hora,
            hora_hybrida_componere(decima, II));
        a = (VolumenActum*)xar_obtinere(acta, III);
        CREDO_AEQUALIS_S64(a->identitas.hora,
            hora_hybrida_componere(decima
            + V * M + CCL, ZEPHYRUM));
        CREDO_CHORDA_AEQUALIS(a->identitas.machina, machina);
        volumen_claudere(vol);

        /* codex vetus post migrationem: hora et machina NIHIL */
        s = scrinium_aperire(piscina, via_vetus);
        CREDO_NECESSE_NON_NIHIL(s);
        CREDO_VERUM(scrinium_exsequi(s,
            "INSERT INTO acta (momentum, genus, datum) VALUES"
            " ('2026-10-20T09:00:00.000Z', 'e', '{}')"));
        scrinium_claudere(s);
        vol = volumen_aperire(piscina, via_vetus);
        CREDO_NECESSE_NON_NIHIL(vol);
        acta = volumen_acta_legere(vol, ZEPHYRUM, piscina);
        CREDO_NECESSE_NON_NIHIL(acta);
        CREDO_AEQUALIS_I32(xar_numerus(acta), V);
        a = (VolumenActum*)xar_obtinere(acta, IV);
        CREDO_AEQUALIS_S64(a->identitas.hora,
            hora_hybrida_componere(decima
            + V * M + CCL, I));
        CREDO_CHORDA_AEQUALIS(a->identitas.machina, machina);
        volumen_claudere(vol);
        filum_delere(via_vetus);
    }
}


/* ========================================================
 * PROBARE: volumen_seq_ultimum (proiectiones D4c) - sequitur
 * appendere, ansam alteram videt, enuntiatum servatum in claudendo
 * finitur (aliter conexio sqlite non clauditur et memoria crescit)
 * ======================================================== */

/* via voluminis cyclorum: in TMPDIR (area membri sub fabrica, suitae
 * sub credo) - non build/ (scriptura extra vestigium, porta radix) */
interior vacuum
_viam_seq_ponere (
     character* via,
           i32  capacitas)
{
    constans character* tmp;

    tmp = getenv("TMPDIR");
    si (!tmp || !tmp[ZEPHYRUM] || (i32)strlen(tmp) + XXXII >= capacitas)
    {
        tmp = "build";
    }
    sprintf(via, "%s/probatio_volumen_seq.db", tmp);
}

interior i64
_vestigium (vacuum)
{
    MemoriaProcessus mp;

    redde memoria_processus_legere(ZEPHYRUM, &mp) ? mp.vestigium
        : ZEPHYRUM;
}

/* aperire, seq legere (enuntiatum servatur), claudere - n vicibus */
interior vacuum
_seq_cyclos_currere (
    constans character* via,
                    i32 n)
{
     Piscina* scheda;
     Volumen* vol;
         i32  i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        scheda = piscina_generare_dynamicum("probatio_seq",
            IV * MXXIV);
        CREDO_NECESSE_NON_NIHIL(scheda);
        vol = volumen_aperire(scheda, via);
        CREDO_NECESSE_NON_NIHIL(vol);
        CREDO_NECESSE(volumen_seq_ultimum(vol) > ZEPHYRUM);
        volumen_claudere(vol);
        piscina_destruere(scheda);
    }
}

interior vacuum
_sectio_seq_ultimi (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Volumen* vol;
        Volumen* altera;
            s64  seq;

        vol = volumen_temporarium(piscina, "probatio_volumen_seq");
        CREDO_NECESSE_NON_NIHIL(vol);
        /* ortus iam scriptus: seq eius, non zephyrum */
        CREDO_AEQUALIS_S64(volumen_seq_ultimum(vol),
            volumen_summa_actorum(vol));
        seq = volumen_actum_appendere(vol, "d/mutatio",
            chorda_ex_literis("primum", piscina));
        CREDO_NECESSE(seq > ZEPHYRUM);
        CREDO_AEQUALIS_S64(volumen_seq_ultimum(vol), seq);
        seq = volumen_actum_appendere(vol, "d/mutatio",
            chorda_ex_literis("secundum", piscina));
        CREDO_AEQUALIS_S64(volumen_seq_ultimum(vol), seq);

        /* ansa altera (alius processus): scripta eius videntur, et
         * vice versa - quaestio vera, non numerus memor */
        altera = volumen_aperire(piscina, volumen_via(vol));
        CREDO_NECESSE_NON_NIHIL(altera);
        CREDO_AEQUALIS_S64(volumen_seq_ultimum(altera), seq);
        seq = volumen_actum_appendere(altera, "d/mutatio",
            chorda_ex_literis("ab altera", piscina));
        CREDO_NECESSE(seq > ZEPHYRUM);
        CREDO_AEQUALIS_S64(volumen_seq_ultimum(vol), seq);
        seq = volumen_actum_appendere(vol, "d/mutatio",
            chorda_ex_literis("ab prima", piscina));
        CREDO_AEQUALIS_S64(volumen_seq_ultimum(altera), seq);
        volumen_claudere(altera);
        volumen_claudere(vol);
    }
    {
        Volumen* vol;
            i64  ante;
            i64  post;
      character  via[DXII];

        /* claudere enuntiatum servatum finit: sine eo sqlite3_close
         * BUSY reddit et conexio (cum memoria sua) manet. Cycli
         * calefacientes primum (vestigium prima vice crescit). */
        _viam_seq_ponere(via, (i32)magnitudo(via));
        si (filum_existit(via))
        {
            (vacuum)filum_delere(via);
        }
        vol = volumen_creare(piscina, via);
        CREDO_NECESSE_NON_NIHIL(vol);
        volumen_claudere(vol);
        _seq_cyclos_currere(via, L);
        ante = _vestigium();
        _seq_cyclos_currere(via, D);
        post = _vestigium();
        imprimere("  seq_ultimum cycli D: vestigium %+ld KiB\n",
            (longus)((s64)post - (s64)ante) / MXXIV);
        CREDO_VERUM(post < ante
            || post - ante < (i64)II * MXXIV * MXXIV);
        (vacuum)filum_delere(via);
    }
}


/* ========================================================
 * PROBARE: volumen temporarium (scratch)
 * ======================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                   Volumen* tmp;
        constans character* via_tmp;
                 character  via_servata[CCLVI];
                       s64  seq;

        tmp = volumen_temporarium(piscina, "probatio_volumen");
        CREDO_NON_NIHIL (tmp);
        si (tmp != NIHIL)
        {
            /* Plagula VERA est dum vivit - scribi et legi potest. */
            via_tmp = volumen_via(tmp);
            CREDO_VERUM (via_tmp != NIHIL);
            si (via_tmp != NIHIL)
            {
                strcpy(via_servata, via_tmp);
                CREDO_VERUM (filum_existit(via_servata));
                CREDO_VERUM (_continet(via_servata,
                    "probatio_volumen"));

                seq = volumen_actum_appendere(tmp, "probatum",
                    chorda_ex_literis("{}", piscina));
                CREDO_VERUM (seq > 0);

                /* Claudere DELET - id est totum consilium. */
                volumen_claudere(tmp);
                CREDO_FALSUM (filum_existit(via_servata));
            }
        }
    }
}


    /* ========================================================
     * PROBARE: volumen ORDINARIUM claudendo NON deletur
     *
     * Custos contra vitium quod campum addendo paene induxi:
     * structura per piscina_allocare sumitur et CAMPO-POST-CAMPUM
     * impletur, sine memset. Campus 'temporarium' non initializatus
     * purgamentum ferret, et purgamentum non-nullum volumen VERUM in
     * claudendo deleret.
     *
     * VENENUM, NON REPETITIO. Prima forma huius probationis volumen
     * decies creabat et claudebat, sperans purgamentum aliquando
     * non-nullum fore. MENSURATUM: culpa plantata (initializatione
     * ablata) probationem VIRIDEM reliquit - paginae arenae recentes
     * a systemate ZERO-IMPLETAE veniunt, ergo 'purgamentum' fideliter
     * FALSUM legebatur. Probatio fortunam probabat, et fortuna cum
     * vitio stabat.
     *
     * Ergo memoria ipsa VENENATUR: nota sumitur, spatium 0xFF
     * impletur, piscina ad notam reficitur - et structura sequens in
     * illos ipsos octetos cadit. Purgamentum nunc CERTUM est, non
     * speratum.
     * ======================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                   Volumen* ordinarium;
        constans character* via_ord = _in_area(c,
            "/ordinarium.volumen");
            PiscinaNotatio  nota;
                        i8* venenum;

        (vacuum)filum_delere(via_ord);

        /* NULLA notatio credo inter notare et reficere: credo
         * notationes in EADEM piscina allocat, ergo notatio ibi facta
         * a reficere liberatur dum in indice credo manet, et structura
         * sequens eam obruit. Sub ordinatione I fortuito non
         * attingebatur; ordinatio VIII (2026-10-05) indicem corrupit
         * (compendium ruit, SIGSEGV). */
        nota     = piscina_notare(piscina);
        venenum  = (i8*)piscina_allocare(piscina, (memoriae_index)512);
        si (venenum != NIHIL)
        {
            memset(venenum, 0xFF, (memoriae_index)512);
        }
        piscina_reficere(piscina, nota);

        /* Structura Voluminis PRIMA post reficere: in octetos venenatos
         * cadit (notationes credo post eam veniunt). */
        ordinarium = (venenum != NIHIL) ?
            volumen_creare(piscina, via_ord) : NIHIL;
        CREDO_NON_NIHIL (venenum);
        CREDO_NON_NIHIL (ordinarium);
        si (ordinarium != NIHIL)
        {
            volumen_claudere(ordinarium);
            /* SI 'temporarium' non initializatur, hic PERIIT. */
            CREDO_VERUM (filum_existit(via_ord));
            (vacuum)filum_delere(via_ord);
        }
    }
}

/* volumen_in_transactione: vocator transactionem tenetne? (canalis T4
 * - admissio eam iungit) */
interior vacuum
_sectio_in_transactione (
    CredoContextus* c)
{
    Volumen* v = volumen_temporarium(c->piscina, "probatio_volumen_tr");

    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_FALSUM(volumen_in_transactione(v));
    CREDO_NECESSE(volumen_transactionem_incipere(v));
    CREDO_VERUM(volumen_in_transactione(v));
    CREDO_NECESSE(volumen_transactionem_committere(v));
    CREDO_FALSUM(volumen_in_transactione(v));
    CREDO_NECESSE(volumen_transactionem_incipere(v));
    CREDO_NECESSE(volumen_transactionem_revolvere(v));
    CREDO_FALSUM(volumen_in_transactione(v));
    CREDO_FALSUM(volumen_in_transactione(NIHIL));
    volumen_claudere(v);
}


/* ========================================================
 * PROBARE: volumen temporarium sub $TMPDIR (android-gradus II.2)
 *
 * Android /tmp caret; cursor tabulae TMPDIR sub radice sua
 * ponit. Basis: $TMPDIR non vacuum (virgulae finales abscissae),
 * aliter /tmp - regula credo (lib/credo.c).
 * ======================================================== */

interior b32
_incipit (
    constans character* via,
    constans character* praefixum)
{
    redde strncmp(via, praefixum, strlen(praefixum)) == ZEPHYRUM;
}

/* casus unus: TMPDIR = valor (NIHIL = absens) -> volumen sub
 * 'exspectatum'; exspectatum NIHIL = basis /tmp, quae in Android
 * deest - ibi volumen nasci NON debet (nec alibi) */
interior vacuum
_temporarium_sub (
        CredoContextus* c,
    constans character* valor,
    constans character* exspectatum)
{
      Volumen* vol;
    character  via[DXII];

    si (valor != NIHIL)
    {
        (vacuum)setenv("TMPDIR", valor, I);
    }
    alioquin
    {
        (vacuum)unsetenv("TMPDIR");
    }
    vol = volumen_temporarium(c->piscina, "tmpdir_probatio");
    si (exspectatum == NIHIL && !filum_directorium_existit("/tmp"))
    {
        CREDO_NIHIL (vol);
        redde;
    }
    CREDO_NON_NIHIL (vol);
    si (vol == NIHIL)
    {
        redde;
    }
    strcpy(via, volumen_via(vol));
    CREDO_VERUM (_incipit(via, exspectatum != NIHIL ? exspectatum
        : "/tmp/tmpdir_probatio-"));
    CREDO_VERUM (filum_existit(via));
    volumen_claudere(vol);
    CREDO_FALSUM (filum_existit(via));
}

interior vacuum
_sectio_temporarii_ambitus (
    CredoContextus* c)
{
    constans character* vetus;
    constans character* dir;
    constans character* duplex_virgula;
    constans character* praefixum;
             character  servatum[DXII];
                   b32  habuit;

    vetus   = getenv("TMPDIR");
    habuit  = vetus != NIHIL && strlen(vetus) < DXII;
    si (habuit)
    {
        strcpy(servatum, vetus);
    }
    /* directorium in area sectionis - via ANTE setenv computata
     * (credo_area ex TMPDIR derivatur) */
    dir             = _in_area(c, "/tmpdir");
    duplex_virgula  = _in_area(c, "/tmpdir//");
    praefixum       = _in_area(c, "/tmpdir/tmpdir_probatio-");
    CREDO_VERUM (filum_directorium_creare_cum_parentibus(dir));

    /* I: directorium datum */
    _temporarium_sub(c, dir, praefixum);
    /* II: virgulae finales abscinduntur (macOS TMPDIR '/' terminat) */
    _temporarium_sub(c, duplex_virgula, praefixum);
    /* III-IV: vacuum aut absens -> /tmp */
    _temporarium_sub(c, "", NIHIL);
    _temporarium_sub(c, NIHIL, NIHIL);

    si (habuit)
    {
        (vacuum)setenv("TMPDIR", servatum, I);
    }
    alioquin
    {
        (vacuum)unsetenv("TMPDIR");
    }
}


/* communio plan-2 T2: acta replicarum selectarum post seq locale,
 * ordine seq, limite servato (tractio servi) */
interior vacuum
_sectio_replicarum_selectarum (
    CredoContextus* c)
{
      Piscina* piscina;
      Volumen* fontes[III];
      Volumen* d;
       chorda  selectae[II];
          Xar* l;
          i32  i;
          i32  k;
          s64  prior;
    character  titulus[XXXII];

    piscina  = c->piscina;
    d        = volumen_temporarium(piscina, "probatio_volumen_sel_d");
    CREDO_NECESSE_NON_NIHIL(d);
    per (i = ZEPHYRUM; i < III; i++)
    {
        sprintf(titulus, "probatio_volumen_sel_%d", (integer)i);
        fontes[i] = volumen_temporarium(piscina, titulus);
        CREDO_NECESSE_NON_NIHIL(fontes[i]);
    }
    /* acta alternata: A B C A B C, quaeque in D importata */
    per (k = ZEPHYRUM; k < II; k++)
    {
        per (i = ZEPHYRUM; i < III; i++)
        {
            VolumenActum a;

            CREDO_NECESSE(volumen_actum_appendere(fontes[i], "x/actum",
                chorda_ex_literis("{}", piscina)) > ZEPHYRUM);
            a = actum_ultimum(fontes[i], piscina);
            CREDO_NECESSE((i32)volumen_actum_importare(d, &a)
                == (i32)VOLUMEN_IMPORTATUM);
        }
    }
    selectae[ZEPHYRUM]  = volumen_replica(fontes[ZEPHYRUM]);
    selectae[I]         = volumen_replica(fontes[II]);

    CREDO_GRADUS("solae selectae, ordine seq");
    l = volumen_acta_replicarum_legere(d, ZEPHYRUM, selectae, II, X,
        piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), IV);
    prior = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(l); i++)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(l, i);

        CREDO_VERUM(   chorda_aequalis(a->identitas.replica,
                           selectae[ZEPHYRUM])
                    || chorda_aequalis(a->identitas.replica,
                           selectae[I]));
        CREDO_VERUM(a->seq > prior);
        prior = a->seq;
    }

    CREDO_GRADUS("limes et post_seq");
    l = volumen_acta_replicarum_legere(d, ZEPHYRUM, selectae, II, III,
        piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), III);
    prior = ((VolumenActum*)xar_obtinere(l, II))->seq;
    l = volumen_acta_replicarum_legere(d, prior, selectae, II, X,
        piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), I);

    CREDO_GRADUS("nulla replica: vacuum");
    l = volumen_acta_replicarum_legere(d, ZEPHYRUM, selectae, ZEPHYRUM,
        X, piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), ZEPHYRUM);
    per (i = ZEPHYRUM; i < III; i++)
    {
        volumen_claudere(fontes[i]);
    }
    volumen_claudere(d);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "volumen_creare", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "aperire_aut_creare", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "volumen_actum_appendere", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "plagulam condere/promere", _sectio_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "dedup sigillo + upsert", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "plagulam vacuam", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "massas nudas (binarias)", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "enumerare + permanentiam", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "plicam_ad (iter temporis)", _sectio_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "acta_generum_legere", _sectio_acta_generum,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "praesentia (D1)", _sectio_praesentiae,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "migratio praesentiae (V-VI)", _sectio_migrationis_praesentiae,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "hora actorum (D2)", _sectio_horae,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "basis actuum (D2b)", _sectio_basis,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "supplementum horarum (VII-XI)", _sectio_supplementi_horarum,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "seq_ultimum (D4c)", _sectio_seq_ultimi,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "replica (communio D2b)", _sectio_replicae, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "replicae distinctae eadem machina",
        _sectio_replicarum_distinctarum,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "volumen vetus: replica suppletur", _sectio_replicae_veteris,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "importatio actus alieni (communio II.1)", _sectio_importationis,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "hora futura (limes)", _sectio_horae_futurae, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "plagula aliena", _sectio_plagulae_alienae, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "replicae selectae", _sectio_replicarum_selectarum, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "volumen_temporarium", _sectio_decima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "volumen_temporarium sub TMPDIR", _sectio_temporarii_ambitus,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "volumen ordinarium SUPERSTES", _sectio_decima_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "in_transactione", _sectio_in_transactione, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("volumen", SECTIONES, _suitam_parare,
        NIHIL);
}
