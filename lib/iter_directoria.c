#include "iter_directoria.h"
#include "via.h"
#include "chorda_aedificator.h"

#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

/* ==================================================
 * Structura Interna Iteratoris
 * ================================================== */

structura DirectoriumIterator {
    DIR*                  descriptum;       /* opendir() handle */
    Piscina*              piscina;          /* Pro allocationibus */
    chorda                via_basis;        /* Via directorii */
    DirectoriumIntroitus  introitus_nunc;   /* Introitus currens */
    character             buffer_nominis[CCLVI]; /* Buffer temporarius pro nomine */
};


/* ==================================================
 * Functiones Auxiliares
 * ================================================== */

/* Determinare genus introitus ex stat() */
hic_manens IntroitusGenus
_determinare_genus(mode_t modus)
{
    si (S_ISREG(modus)) {
        redde INTROITUS_FILUM;
    }
    si (S_ISDIR(modus)) {
        redde INTROITUS_DIRECTORIUM;
    }
    redde INTROITUS_ALIUS;
}


/* Verificare si titulus est . vel .. */
hic_manens b32
_est_punctum(constans character* titulus_str)
{
    si (titulus_str == NIHIL) {
        redde FALSUM;
    }
    si (titulus_str[0] == '.') {
        si (titulus_str[1] == '\0') {
            redde VERUM;
        }
        si (titulus_str[1] == '.' && titulus_str[2] == '\0') {
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Iterator: Aperire
 * ================================================== */

DirectoriumIterator*
directorium_iterator_aperire(
    constans character* via,
    Piscina*            piscina)
{
    DirectoriumIterator* iterator;
    DIR* descriptum;

    si (via == NIHIL || piscina == NIHIL) {
        redde NIHIL;
    }

    descriptum = opendir(via);
    si (descriptum == NIHIL) {
        redde NIHIL;
    }

    iterator = (DirectoriumIterator*)piscina_allocare(
        piscina,
        magnitudo(DirectoriumIterator)
    );

    iterator->descriptum = descriptum;
    iterator->piscina = piscina;
    iterator->via_basis = chorda_ex_literis(via, piscina);

    /* Initiare introitus_nunc */
    iterator->introitus_nunc.titulus.datum = NIHIL;
    iterator->introitus_nunc.titulus.mensura = 0;
    iterator->introitus_nunc.genus = INTROITUS_IGNOTUS;
    iterator->introitus_nunc.mensura = 0;

    redde iterator;
}


/* ==================================================
 * Iterator: Proximum
 * ================================================== */

DirectoriumIntroitus*
directorium_iterator_proximum(
    DirectoriumIterator* iterator)
{
    structura dirent* introitus;
    structura stat status;
    character via_plena[1024];
    i32 longitudo_basis;
    i32 longitudo_nominis;

    si (iterator == NIHIL || iterator->descriptum == NIHIL) {
        redde NIHIL;
    }

    /* Legere usque ad introitum validum (saltare . et ..) */
    per (;;) {
        introitus = readdir(iterator->descriptum);
        si (introitus == NIHIL) {
            redde NIHIL;
        }

        /* Saltare . et .. */
        si (!_est_punctum(introitus->d_name)) {
            frange;
        }
    }

    /* Copiare nomen ad buffer internum */
    longitudo_nominis = (i32)strlen(introitus->d_name);
    si (longitudo_nominis >= CCLVI) {
        longitudo_nominis = CCLV;
    }
    memcpy(iterator->buffer_nominis, introitus->d_name, (size_t)longitudo_nominis);
    iterator->buffer_nominis[longitudo_nominis] = '\0';

    /* Creare chorda ex buffer */
    iterator->introitus_nunc.titulus = chorda_ex_buffer(
        (i8*)iterator->buffer_nominis,
        longitudo_nominis
    );

    /* Construere viam plenam pro stat() */
    longitudo_basis = iterator->via_basis.mensura;
    si (longitudo_basis + longitudo_nominis + II >= 1024) {
        /* Via nimis longa */
        iterator->introitus_nunc.genus = INTROITUS_IGNOTUS;
        iterator->introitus_nunc.mensura = 0;
        redde &(iterator->introitus_nunc);
    }

    memcpy(via_plena, iterator->via_basis.datum, (size_t)longitudo_basis);
    via_plena[longitudo_basis] = '/';
    memcpy(via_plena + longitudo_basis + I, iterator->buffer_nominis, (size_t)longitudo_nominis);
    via_plena[longitudo_basis + I + longitudo_nominis] = '\0';

    /* Obtinere statum fili */
    si (stat(via_plena, &status) == 0) {
        iterator->introitus_nunc.genus = _determinare_genus(status.st_mode);
        iterator->introitus_nunc.mensura = (memoriae_index)status.st_size;
    } alioquin {
        iterator->introitus_nunc.genus = INTROITUS_IGNOTUS;
        iterator->introitus_nunc.mensura = 0;
    }

    redde &(iterator->introitus_nunc);
}


/* ==================================================
 * Iterator: Claudere
 * ================================================== */

vacuum
directorium_iterator_claudere(
    DirectoriumIterator* iterator)
{
    si (iterator != NIHIL && iterator->descriptum != NIHIL) {
        closedir(iterator->descriptum);
        iterator->descriptum = NIHIL;
    }
    /* Memoria liberata cum piscina destruitur */
}


/* ==================================================
 * Filtrum: Omnia
 * ================================================== */

DirectoriumFiltrum
directorium_filtrum_omnia(
    vacuum)
{
    DirectoriumFiltrum filtrum;

    filtrum.genera_accepta = NIHIL;
    filtrum.genera_numerus = 0;
    filtrum.exemplar.datum = NIHIL;
    filtrum.exemplar.mensura = 0;
    filtrum.profunditas_max = 0;
    filtrum.includere_occultos = FALSUM;

    redde filtrum;
}


/* ==================================================
 * Pattern Matching
 * ================================================== */

/* Implementatio recursiva pattern matching */
hic_manens b32
_congruit_recursivum(
    constans i8* titulus_data,
    i32          titulus_index,
    i32          titulus_longitudo,
    constans i8* exemplar_data,
    i32          exemplar_index,
    i32          exemplar_longitudo)
{
    character c;

    /* Finis utriusque */
    si (exemplar_index >= exemplar_longitudo) {
        redde (titulus_index >= titulus_longitudo);
    }

    /* Character currentis exemplaris */
    c = (character)exemplar_data[exemplar_index];

    si (c == '*') {
        /* * congruit cum zero vel plus characteribus */
        /* Probare: * congruit nihil */
        si (_congruit_recursivum(titulus_data, titulus_index, titulus_longitudo,
                                  exemplar_data, exemplar_index + I, exemplar_longitudo)) {
            redde VERUM;
        }
        /* Probare: * congruit cum uno charactere, tunc continuare */
        si (titulus_index < titulus_longitudo) {
            redde _congruit_recursivum(titulus_data, titulus_index + I, titulus_longitudo,
                                        exemplar_data, exemplar_index, exemplar_longitudo);
        }
        redde FALSUM;
    }

    si (c == '?') {
        /* ? congruit cum exacte uno charactere */
        si (titulus_index < titulus_longitudo) {
            redde _congruit_recursivum(titulus_data, titulus_index + I, titulus_longitudo,
                                        exemplar_data, exemplar_index + I, exemplar_longitudo);
        }
        redde FALSUM;
    }

    /* Character literalis */
    si (titulus_index < titulus_longitudo && (character)titulus_data[titulus_index] == c) {
        redde _congruit_recursivum(titulus_data, titulus_index + I, titulus_longitudo,
                                    exemplar_data, exemplar_index + I, exemplar_longitudo);
    }

    redde FALSUM;
}


b32
directorium_titulus_congruit(
    chorda titulus,
    chorda exemplar)
{
    /* Exemplar vacua congruit cum omnibus */
    si (exemplar.datum == NIHIL || exemplar.mensura == 0) {
        redde VERUM;
    }

    /* Titulus vacua non congruit (nisi exemplar est solum *) */
    si (titulus.datum == NIHIL || titulus.mensura == 0) {
        si (exemplar.mensura == I && exemplar.datum[0] == '*') {
            redde VERUM;
        }
        redde FALSUM;
    }

    redde _congruit_recursivum(
        titulus.datum, 0, titulus.mensura,
        exemplar.datum, 0, exemplar.mensura
    );
}


/* ==================================================
 * Utilitates
 * ================================================== */

b32
directorium_existit(
    constans character* via)
{
    structura stat status;

    si (via == NIHIL) {
        redde FALSUM;
    }

    si (stat(via, &status) != 0) {
        redde FALSUM;
    }

    redde S_ISDIR(status.st_mode) ? VERUM : FALSUM;
}


/* ==================================================
 * Ambulator: Verificare Filtrum
 * ================================================== */

hic_manens b32
_filtrum_acceptat_genus(
    constans DirectoriumFiltrum* filtrum,
    IntroitusGenus               genus)
{
    i32 i;

    si (filtrum == NIHIL || filtrum->genera_accepta == NIHIL || filtrum->genera_numerus == 0) {
        redde VERUM; /* Nullum filtrum = omnia accepta */
    }

    per (i = 0; i < filtrum->genera_numerus; i++) {
        si (filtrum->genera_accepta[i] == genus) {
            redde VERUM;
        }
    }

    redde FALSUM;
}


hic_manens b32
_filtrum_acceptat_introitum(
    constans DirectoriumFiltrum*   filtrum,
    constans DirectoriumIntroitus* introitus)
{
    /* Verificare occultum */
    si (filtrum != NIHIL && !filtrum->includere_occultos) {
        si (introitus->titulus.mensura > 0 && introitus->titulus.datum[0] == '.') {
            redde FALSUM;
        }
    }

    /* Verificare genus */
    si (!_filtrum_acceptat_genus(filtrum, introitus->genus)) {
        redde FALSUM;
    }

    /* Verificare exemplar (solum pro filis, non directoriis) */
    si (filtrum != NIHIL &&
        filtrum->exemplar.datum != NIHIL &&
        filtrum->exemplar.mensura > 0 &&
        introitus->genus == INTROITUS_FILUM) {
        si (!directorium_titulus_congruit(introitus->titulus, filtrum->exemplar)) {
            redde FALSUM;
        }
    }

    redde VERUM;
}


/* ==================================================
 * Ambulator: Recursio Interna
 * ================================================== */

hic_manens s32
_ambulare_recursivum(
    chorda                         via_currens,
    constans DirectoriumFiltrum*   filtrum,
    DirectoriumAmbulatorFunctio    functio,
    vacuum*                        contextus,
    Piscina*                       piscina,
    s32                            profunditas)
{
    DirectoriumIterator* iterator;
    DirectoriumIntroitus* introitus;
    character* via_cstr;
    chorda via_nova;
    chorda partes[II];
    s32 fructus;

    /* Verificare profunditatem maximam */
    si (profunditas > DIRECTORIUM_PROFUNDITAS_MAXIMA) {
        redde 0; /* Limite attacto, sed non error */
    }

    /* Verificare profunditas_max in filtro */
    si (filtrum != NIHIL && filtrum->profunditas_max != 0) {
        si (filtrum->profunditas_max == (s32)-1) {
            redde 0; /* Sine recursione */
        }
        si (profunditas > filtrum->profunditas_max) {
            redde 0;
        }
    }

    /* Convertere chorda ad C string pro opendir */
    via_cstr = chorda_ut_cstr(via_currens, piscina);
    si (via_cstr == NIHIL) {
        redde -I;
    }

    iterator = directorium_iterator_aperire(via_cstr, piscina);
    si (iterator == NIHIL) {
        redde -I;
    }

    dum ((introitus = directorium_iterator_proximum(iterator)) != NIHIL) {
        /* Construere viam plenam */
        partes[0] = via_currens;
        partes[I] = introitus->titulus;
        via_nova = via_iungere(partes, II, piscina);

        /* Verificare si filtrum acceptat */
        si (_filtrum_acceptat_introitum(filtrum, introitus)) {
            /* Vocare callback */
            fructus = functio(via_nova, introitus, contextus);
            si (fructus != 0) {
                directorium_iterator_claudere(iterator);
                redde fructus;
            }
        }

        /* Recursio in directoria */
        si (introitus->genus == INTROITUS_DIRECTORIUM) {
            /* Verificare si non est occultum (vel includere_occultos) */
            si (filtrum == NIHIL ||
                filtrum->includere_occultos ||
                introitus->titulus.datum[0] != '.') {
                fructus = _ambulare_recursivum(
                    via_nova,
                    filtrum,
                    functio,
                    contextus,
                    piscina,
                    profunditas + I
                );
                si (fructus != 0) {
                    directorium_iterator_claudere(iterator);
                    redde fructus;
                }
            }
        }
    }

    directorium_iterator_claudere(iterator);
    redde 0;
}


/* ==================================================
 * Ambulator: Publicus
 * ================================================== */

s32
directorium_ambulare(
    constans character*            via,
    constans DirectoriumFiltrum*   filtrum,
    DirectoriumAmbulatorFunctio    functio,
    vacuum*                        contextus,
    Piscina*                       piscina)
{
    chorda via_chorda;

    si (via == NIHIL || functio == NIHIL || piscina == NIHIL) {
        redde -I;
    }

    via_chorda = chorda_ex_literis(via, piscina);

    redde _ambulare_recursivum(
        via_chorda,
        filtrum,
        functio,
        contextus,
        piscina,
        0
    );
}
