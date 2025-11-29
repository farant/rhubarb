#include "tempestivum.h"
#include "tempus.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Interior - Obtinere tempus currentem in ms
 * ================================================== */

interior s64
_tempus_nunc_ms(vacuum)
{
    redde (s64)(tempus_nunc() * 1000.0);
}


/* ==================================================
 * Interior - Cached property keys
 * ================================================== */

interior chorda*
_internare_literis(
    InternamentumChorda* intern,
    Piscina*             piscina,
    constans character*  cstr)
{
    chorda temp;
    temp = chorda_ex_literis(cstr, piscina);
    redde chorda_internare(intern, temp);
}


/* ==================================================
 * Creatio
 * ================================================== */

AdministratorTempestivi*
administrator_tempestivi_creare(
    Piscina*             piscina,
    EntitasRepositorium* repositorium,
    CursusNuntiorum*     cursus,
    InternamentumChorda* intern)
{
    AdministratorTempestivi* admin;

    si (!piscina || !repositorium || !cursus || !intern)
    {
        redde NIHIL;
    }

    admin = (AdministratorTempestivi*)piscina_allocare(piscina, magnitudo(AdministratorTempestivi));
    si (!admin)
    {
        redde NIHIL;
    }

    admin->piscina      = piscina;
    admin->repositorium = repositorium;
    admin->cursus       = cursus;
    admin->intern       = intern;

    /* Cache genus tempestivi */
    admin->genus_tempestivi = _internare_literis(intern, piscina, TEMPESTIVUM_GENUS);
    si (!admin->genus_tempestivi)
    {
        redde NIHIL;
    }

    redde admin;
}


/* ==================================================
 * Tempestiva - Creatio et Deletio
 * ================================================== */

Entitas*
tempestivum_creare_cum_delay(
    AdministratorTempestivi* admin,
    chorda*                  destinatio_id,
    constans character*      genus_nuntii,
    s64                      intervallum_ms,
    s64                      delay_ms,
    b32                      repetere)
{
    Entitas*  tempestivum;
    character buffer[LXIV];
    character dest_buffer[CXXVIII];
    s64       proximum_tempus;
    i32       dest_len;

    si (!admin || !destinatio_id || !genus_nuntii || intervallum_ms <= ZEPHYRUM)
    {
        redde NIHIL;
    }

    /* Creare entitatem tempestivi */
    tempestivum = admin->repositorium->entitas_creare(
        admin->repositorium->datum,
        TEMPESTIVUM_GENUS);

    si (!tempestivum)
    {
        redde NIHIL;
    }

    /* Convertere destinatio_id ad cstr (chorda non est null-terminata) */
    dest_len = destinatio_id->mensura;
    si (dest_len > CXXVII)
    {
        dest_len = CXXVII;
    }
    memcpy(dest_buffer, destinatio_id->datum, (memoriae_index)dest_len);
    dest_buffer[dest_len] = '\0';

    /* Ponere destinatio_id */
    admin->repositorium->proprietas_ponere(
        admin->repositorium->datum,
        tempestivum,
        TEMPESTIVUM_PROP_DESTINATIO,
        dest_buffer);

    /* Ponere genus_nuntii */
    admin->repositorium->proprietas_ponere(
        admin->repositorium->datum,
        tempestivum,
        TEMPESTIVUM_PROP_GENUS_NUNTII,
        genus_nuntii);

    /* Ponere intervallum */
    sprintf(buffer, "%lld", (longus longus)intervallum_ms);
    admin->repositorium->proprietas_ponere(
        admin->repositorium->datum,
        tempestivum,
        TEMPESTIVUM_PROP_INTERVALLUM,
        buffer);

    /* Ponere repetere */
    admin->repositorium->proprietas_ponere(
        admin->repositorium->datum,
        tempestivum,
        TEMPESTIVUM_PROP_REPETERE,
        repetere ? "verum" : "falsum");

    /* Calculare proximum tempus */
    proximum_tempus = _tempus_nunc_ms() + delay_ms;
    sprintf(buffer, "%lld", (longus longus)proximum_tempus);
    admin->repositorium->proprietas_ponere(
        admin->repositorium->datum,
        tempestivum,
        TEMPESTIVUM_PROP_PROXIMUM,
        buffer);

    /* Ponere activum */
    admin->repositorium->proprietas_ponere(
        admin->repositorium->datum,
        tempestivum,
        TEMPESTIVUM_PROP_ACTIVUM,
        "verum");

    redde tempestivum;
}

Entitas*
tempestivum_creare(
    AdministratorTempestivi* admin,
    chorda*                  destinatio_id,
    constans character*      genus_nuntii,
    s64                      intervallum_ms,
    b32                      repetere)
{
    /* Delay initialis = intervallum (prima activatio post intervallum) */
    redde tempestivum_creare_cum_delay(
        admin,
        destinatio_id,
        genus_nuntii,
        intervallum_ms,
        intervallum_ms,
        repetere);
}

b32
tempestivum_annullare(
    AdministratorTempestivi* admin,
    chorda*                  tempestivum_id)
{
    si (!admin || !tempestivum_id)
    {
        redde FALSUM;
    }

    redde admin->repositorium->entitas_delere(
        admin->repositorium->datum,
        tempestivum_id);
}

b32
tempestivum_pausare(
    AdministratorTempestivi* admin,
    chorda*                  tempestivum_id)
{
    Entitas* tempestivum;

    si (!admin || !tempestivum_id)
    {
        redde FALSUM;
    }

    tempestivum = admin->repositorium->capere_entitatem(
        admin->repositorium->datum,
        tempestivum_id);

    si (!tempestivum)
    {
        redde FALSUM;
    }

    redde admin->repositorium->proprietas_ponere(
        admin->repositorium->datum,
        tempestivum,
        TEMPESTIVUM_PROP_ACTIVUM,
        "falsum");
}

b32
tempestivum_resumere(
    AdministratorTempestivi* admin,
    chorda*                  tempestivum_id)
{
    Entitas* tempestivum;

    si (!admin || !tempestivum_id)
    {
        redde FALSUM;
    }

    tempestivum = admin->repositorium->capere_entitatem(
        admin->repositorium->datum,
        tempestivum_id);

    si (!tempestivum)
    {
        redde FALSUM;
    }

    redde admin->repositorium->proprietas_ponere(
        admin->repositorium->datum,
        tempestivum,
        TEMPESTIVUM_PROP_ACTIVUM,
        "verum");
}


/* ==================================================
 * Processare
 * ================================================== */

i32
tempestivum_processare(
    AdministratorTempestivi* admin)
{
    Xar*      tempestiva;
    i32       i;
    i32       numerus;
    i32       missi;
    s64       tempus_nunc_ms;
    chorda*   clavis_activum;
    chorda*   clavis_proximum;
    chorda*   clavis_intervallum;
    chorda*   clavis_repetere;
    chorda*   clavis_destinatio;
    chorda*   clavis_genus_nuntii;

    si (!admin)
    {
        redde ZEPHYRUM;
    }

    /* Quaerere omnes tempestiva */
    tempestiva = admin->repositorium->quaerere_cum_praefixo_notae(
        admin->repositorium->datum,
        "#");

    /* Si nullae entitates cum nota, quaerere per genus */
    /* Pro nunc, iterare per omnes entitates et filtrare per genus */
    /* TODO: Addere quaerere_per_genus ad repositorium */

    /* Cached property keys */
    clavis_activum      = _internare_literis(admin->intern, admin->piscina, TEMPESTIVUM_PROP_ACTIVUM);
    clavis_proximum     = _internare_literis(admin->intern, admin->piscina, TEMPESTIVUM_PROP_PROXIMUM);
    clavis_intervallum  = _internare_literis(admin->intern, admin->piscina, TEMPESTIVUM_PROP_INTERVALLUM);
    clavis_repetere     = _internare_literis(admin->intern, admin->piscina, TEMPESTIVUM_PROP_REPETERE);
    clavis_destinatio   = _internare_literis(admin->intern, admin->piscina, TEMPESTIVUM_PROP_DESTINATIO);
    clavis_genus_nuntii = _internare_literis(admin->intern, admin->piscina, TEMPESTIVUM_PROP_GENUS_NUNTII);

    tempus_nunc_ms = _tempus_nunc_ms();
    missi = ZEPHYRUM;

    /* Quaerere per textum genus (workaround) */
    tempestiva = admin->repositorium->quaerere_textum(
        admin->repositorium->datum,
        TEMPESTIVUM_GENUS);

    si (!tempestiva)
    {
        redde ZEPHYRUM;
    }

    numerus = xar_numerus(tempestiva);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        Entitas** slot;
        Entitas*  tempestivum;
        b32       activum;
        s64       proximum_tempus;
        s64       intervallum;
        b32       repetere;
        chorda*   destinatio_id;
        chorda*   genus_nuntii;

        slot = (Entitas**)xar_obtinere(tempestiva, i);
        si (!slot || !*slot)
        {
            perge;
        }
        tempestivum = *slot;

        /* Verificare genus */
        si (tempestivum->genus != admin->genus_tempestivi)
        {
            perge;
        }

        /* Verificare activum */
        si (!entitas_proprietas_capere_b32(tempestivum, clavis_activum, &activum) || !activum)
        {
            perge;
        }

        /* Capere proximum tempus */
        si (!entitas_proprietas_capere_s64(tempestivum, clavis_proximum, &proximum_tempus))
        {
            perge;
        }

        /* Verificare si tempus est */
        si (tempus_nunc_ms < proximum_tempus)
        {
            perge;
        }

        /* Capere datum pro mittendo */
        destinatio_id = entitas_proprietas_capere(tempestivum, clavis_destinatio);
        genus_nuntii  = entitas_proprietas_capere(tempestivum, clavis_genus_nuntii);

        si (!destinatio_id || !genus_nuntii)
        {
            perge;
        }

        /* Mittere nuntium */
        /* Nota: genus_nuntii est chorda*, debemus convertere ad cstr */
        {
            character genus_buffer[LXIV];
            i32       len;

            len = genus_nuntii->mensura;
            si (len > LXIII)
            {
                len = LXIII;
            }
            memcpy(genus_buffer, genus_nuntii->datum, (memoriae_index)len);
            genus_buffer[len] = '\0';

            cursus_mittere_simplex(
                admin->cursus,
                NIHIL,  /* mittens = systema */
                destinatio_id,
                genus_buffer);

            missi++;
        }

        /* Capere repetere */
        si (!entitas_proprietas_capere_b32(tempestivum, clavis_repetere, &repetere))
        {
            repetere = FALSUM;
        }

        si (repetere)
        {
            /* Actualizare proximum tempus */
            character buffer[LXIV];

            si (!entitas_proprietas_capere_s64(tempestivum, clavis_intervallum, &intervallum))
            {
                intervallum = M;  /* Default 1 second */
            }

            proximum_tempus = tempus_nunc_ms + intervallum;
            sprintf(buffer, "%lld", (longus longus)proximum_tempus);

            admin->repositorium->proprietas_ponere(
                admin->repositorium->datum,
                tempestivum,
                TEMPESTIVUM_PROP_PROXIMUM,
                buffer);
        }
        alioquin
        {
            /* Non repetere - deactivare */
            admin->repositorium->proprietas_ponere(
                admin->repositorium->datum,
                tempestivum,
                TEMPESTIVUM_PROP_ACTIVUM,
                "falsum");
        }
    }

    redde missi;
}


/* ==================================================
 * Quaestio
 * ================================================== */

i32
tempestivum_numerus_activorum(
    AdministratorTempestivi* admin)
{
    Xar*    tempestiva;
    i32     i;
    i32     numerus;
    i32     activa;
    chorda* clavis_activum;

    si (!admin)
    {
        redde ZEPHYRUM;
    }

    clavis_activum = _internare_literis(admin->intern, admin->piscina, TEMPESTIVUM_PROP_ACTIVUM);

    tempestiva = admin->repositorium->quaerere_textum(
        admin->repositorium->datum,
        TEMPESTIVUM_GENUS);

    si (!tempestiva)
    {
        redde ZEPHYRUM;
    }

    numerus = xar_numerus(tempestiva);
    activa  = ZEPHYRUM;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        Entitas** slot;
        Entitas*  tempestivum;
        b32       activum;

        slot = (Entitas**)xar_obtinere(tempestiva, i);
        si (!slot || !*slot)
        {
            perge;
        }
        tempestivum = *slot;

        si (tempestivum->genus != admin->genus_tempestivi)
        {
            perge;
        }

        si (entitas_proprietas_capere_b32(tempestivum, clavis_activum, &activum) && activum)
        {
            activa++;
        }
    }

    redde activa;
}

Xar*
tempestivum_pro_destinatione(
    AdministratorTempestivi* admin,
    chorda*                  destinatio_id)
{
    Xar*    tempestiva;
    Xar*    resultus;
    i32     i;
    i32     numerus;
    chorda* clavis_destinatio;

    si (!admin || !destinatio_id)
    {
        redde NIHIL;
    }

    clavis_destinatio = _internare_literis(admin->intern, admin->piscina, TEMPESTIVUM_PROP_DESTINATIO);

    tempestiva = admin->repositorium->quaerere_textum(
        admin->repositorium->datum,
        TEMPESTIVUM_GENUS);

    si (!tempestiva)
    {
        redde NIHIL;
    }

    resultus = xar_creare(admin->piscina, magnitudo(Entitas*));
    si (!resultus)
    {
        redde NIHIL;
    }

    numerus = xar_numerus(tempestiva);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        Entitas** slot;
        Entitas*  tempestivum;
        chorda*   dest;

        slot = (Entitas**)xar_obtinere(tempestiva, i);
        si (!slot || !*slot)
        {
            perge;
        }
        tempestivum = *slot;

        si (tempestivum->genus != admin->genus_tempestivi)
        {
            perge;
        }

        dest = entitas_proprietas_capere(tempestivum, clavis_destinatio);
        si (dest && chorda_aequalis(*dest, *destinatio_id))
        {
            Entitas** res_slot;
            res_slot = (Entitas**)xar_addere(resultus);
            si (res_slot)
            {
                *res_slot = tempestivum;
            }
        }
    }

    redde resultus;
}

b32
tempestivum_est_activum(
    AdministratorTempestivi* admin,
    chorda*                  tempestivum_id)
{
    Entitas* tempestivum;
    chorda*  clavis_activum;
    b32      activum;

    si (!admin || !tempestivum_id)
    {
        redde FALSUM;
    }

    tempestivum = admin->repositorium->capere_entitatem(
        admin->repositorium->datum,
        tempestivum_id);

    si (!tempestivum)
    {
        redde FALSUM;
    }

    clavis_activum = _internare_literis(admin->intern, admin->piscina, TEMPESTIVUM_PROP_ACTIVUM);

    si (entitas_proprietas_capere_b32(tempestivum, clavis_activum, &activum))
    {
        redde activum;
    }

    redde FALSUM;
}


/* ==================================================
 * Utilitas
 * ================================================== */

vacuum
tempestivum_imprimere(
    AdministratorTempestivi* admin)
{
    Xar*    tempestiva;
    i32     i;
    i32     numerus;
    chorda* clavis_activum;
    chorda* clavis_destinatio;
    chorda* clavis_genus_nuntii;
    chorda* clavis_intervallum;

    si (!admin)
    {
        imprimere("AdministratorTempestivi: NIHIL\n");
        redde;
    }

    clavis_activum      = _internare_literis(admin->intern, admin->piscina, TEMPESTIVUM_PROP_ACTIVUM);
    clavis_destinatio   = _internare_literis(admin->intern, admin->piscina, TEMPESTIVUM_PROP_DESTINATIO);
    clavis_genus_nuntii = _internare_literis(admin->intern, admin->piscina, TEMPESTIVUM_PROP_GENUS_NUNTII);
    clavis_intervallum  = _internare_literis(admin->intern, admin->piscina, TEMPESTIVUM_PROP_INTERVALLUM);

    tempestiva = admin->repositorium->quaerere_textum(
        admin->repositorium->datum,
        TEMPESTIVUM_GENUS);

    imprimere("AdministratorTempestivi:\n");

    si (!tempestiva)
    {
        imprimere("  Tempestiva: 0\n");
        redde;
    }

    numerus = xar_numerus(tempestiva);
    imprimere("  Tempestiva: %d\n", numerus);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        Entitas** slot;
        Entitas*  tempestivum;
        b32       activum;
        chorda*   dest;
        chorda*   genus;
        s64       intervallum;

        slot = (Entitas**)xar_obtinere(tempestiva, i);
        si (!slot || !*slot)
        {
            perge;
        }
        tempestivum = *slot;

        si (tempestivum->genus != admin->genus_tempestivi)
        {
            perge;
        }

        entitas_proprietas_capere_b32(tempestivum, clavis_activum, &activum);
        dest  = entitas_proprietas_capere(tempestivum, clavis_destinatio);
        genus = entitas_proprietas_capere(tempestivum, clavis_genus_nuntii);
        entitas_proprietas_capere_s64(tempestivum, clavis_intervallum, &intervallum);

        imprimere("  [%.*s] -> %.*s.%.*s every %lldms [%s]\n",
                  tempestivum->id->mensura, tempestivum->id->datum,
                  dest ? dest->mensura : III, dest ? (character*)dest->datum : "???",
                  genus ? genus->mensura : III, genus ? (character*)genus->datum : "???",
                  (longus longus)intervallum,
                  activum ? "activum" : "pausatum");
    }
}
