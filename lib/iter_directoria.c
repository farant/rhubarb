#include "postulata_posix.h"
#include "iter_directoria.h"
#include "via.h"
#include "lectiones.h"
#include "chorda_aedificator.h"

#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <fcntl.h>
#include <unistd.h>
#include <sys/attr.h>
#include <sys/vnode.h>
#endif

/* mtime cum nanosecundis: nomen membri inter systemata differt.
 * Bracchium Linux CUSTODITUR (ut tools/fabrica_sutura.c): lexicon silvae
 * Darwin-veritas est, examen bracchium sine custodia aestimat */
#ifdef __linux__
#define _MTIME(st) ((st).st_mtim)
#else
#define _MTIME(st) ((st).st_mtimespec)
#endif

/* ENUMERATIO UNO VOCAMINE (macOS, 2026-10-22): getattrlistbulk nomen,
 * genus, mtime et mensuram MULTORUM introituum uno vocamine reddit -
 * olim readdir + stat() per introitum (et photographia fabricae lstat
 * iterum). Mensuratum (build/sonda_partes/sonda_ambulatio.c): ~V x
 * velocius frigide, ~II x calide; aequalitas cum lstat super CCCXLIX
 * milia introituum (directoria et socketa incluso) octetim
 * (sonda_aequalitas.c). Nexus symbolici stat() vocant ut antea (status
 * SECUTUS servatur). Bracchium __APPLE__ silva non examinat (vernaculum
 * Darwin, mos uuid.c / machina_posix.c); bracchium portabile examinat. */
#define _MASSA_ENUMERATIONIS (XXXII * MXXIV)


/* ==================================================
 * Structura Interna Iteratoris
 * ================================================== */

structura DirectoriumIterator {
#ifdef __APPLE__
                  integer  fd;               /* open(O_DIRECTORY) */
               character* massa;            /* getattrlistbulk (malloc) */
               character* cursor;           /* introitus proximus in massa */
                  integer  residua;          /* introitus in massa restantes */
                      b32  finitum;
#else
                     DIR* descriptum;       /* opendir() handle */
#endif
                 Piscina* piscina;          /* Pro allocationibus */
                  chorda  via_basis;        /* Via directorii */
    DirectoriumIntroitus  introitus_nunc;   /* Introitus currens */
               character  buffer_nominis[CCLVI]; /* Buffer temporarius pro nomine */
};


/* ==================================================
 * Functiones Auxiliares
 * ================================================== */

/* Determinare genus introitus ex stat() */
hic_manens IntroitusGenus
_determinare_genus (
    mode_t modus)
{
    si (S_ISREG(modus))
    {
        redde INTROITUS_FILUM;
    }
    si (S_ISDIR(modus))
    {
        redde INTROITUS_DIRECTORIUM;
    }
    redde INTROITUS_ALIUS;
}

/* Verificare si titulus est . vel .. */
hic_manens b32
_est_punctum (
    constans character* titulus_str)
{
    si (titulus_str == NIHIL)
    {
        redde FALSUM;
    }
    si (titulus_str[0] == '.')
    {
        si (titulus_str[1] == '\0')
        {
            redde VERUM;
        }
        si (titulus_str[1] == '.' && titulus_str[2] == '\0')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Nomen in buffer_nominis copiare et titulum ponere */
hic_manens vacuum
_nomen_ponere (
    DirectoriumIterator* iterator,
     constans character* nomen_introitus)
{
    i32 longitudo_nominis;

    longitudo_nominis = (i32)strlen(nomen_introitus);
    si (longitudo_nominis >= CCLVI)
    {
        longitudo_nominis = CCLV;
    }
    memcpy(iterator->buffer_nominis, nomen_introitus,
        (size_t)longitudo_nominis);
    iterator->buffer_nominis[longitudo_nominis] = '\0';
    iterator->introitus_nunc.titulus = chorda_ex_buffer(
        (i8*)iterator->buffer_nominis,
        longitudo_nominis
        );
}

/* Statum SECUTUM per stat() implere (genus, mensura, tempus_ns) -
 * via plena ex basi + nomine. Liber: eventus D directorii (nomina)
 * hanc lectionem tegit - introitus novus/ablatus nomina mutat. LACUNA
 * nota (plan 2 T3): genus/mensura sub nomine eodem mutata (filum ->
 * directorium) sigillum D non mutant. */
hic_manens vacuum
_statum_secutum_implere (
    DirectoriumIterator* iterator)
{
    structura stat  status;
         character via_plena[1024];
               i32 longitudo_basis;
               i32 longitudo_nominis;

    longitudo_basis = iterator->via_basis.mensura;
    longitudo_nominis = iterator->introitus_nunc.titulus.mensura;
    iterator->introitus_nunc.genus = INTROITUS_IGNOTUS;
    iterator->introitus_nunc.mensura = 0;
    iterator->introitus_nunc.tempus_ns = 0;
    si (longitudo_basis + longitudo_nominis + II >= 1024)
    {
        redde;   /* via nimis longa */
    }
    memcpy(via_plena, iterator->via_basis.datum,
        (size_t)longitudo_basis);
    via_plena[longitudo_basis] = '/';
    memcpy(via_plena + longitudo_basis + I, iterator->buffer_nominis,
        (size_t)longitudo_nominis);
    via_plena[longitudo_basis + I + longitudo_nominis] = '\0';
    /* <tolera codex="CANALIS_PRAETERITUS" (>canalis ipse
     * per directorium_iterator_aperire (ENUMERAVIT)
     * lectiones: notatur */
    si (stat(via_plena, &status) == 0)
    {
        iterator->introitus_nunc.genus =
            _determinare_genus(status.st_mode);
        iterator->introitus_nunc.mensura =
            (memoriae_index)status.st_size;
        iterator->introitus_nunc.tempus_ns =
            (s64)_MTIME(status).tv_sec * (s64)M * (s64)M * (s64)M
            + (s64)_MTIME(status).tv_nsec;
    }
}

#ifdef __APPLE__
/* attributa petita: nomen, genus, mtime; mensura directorii et fili */
hic_manens vacuum
_attributa_parare (
    structura attrlist* attributa)
{
    memset(attributa, 0, magnitudo(*attributa));
    attributa->bitmapcount  = ATTR_BIT_MAP_COUNT;
    attributa->commonattr   = ATTR_CMN_RETURNED_ATTRS | ATTR_CMN_NAME
                            | ATTR_CMN_OBJTYPE | ATTR_CMN_MODTIME;
    attributa->dirattr      = ATTR_DIR_DATALENGTH;
    attributa->fileattr     = ATTR_FILE_DATALENGTH;
}
#endif


/* ==================================================
 * Iterator: Aperire
 * ================================================== */

DirectoriumIterator*
directorium_iterator_aperire (
    constans character* via,
               Piscina* piscina)
{
    DirectoriumIterator* iterator;
#ifdef __APPLE__
    integer  fd;
    character* massa;
#else
    DIR* descriptum;
#endif

    si (via == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }

#ifdef __APPLE__
    /* lectiones: notatur */
    fd = open(via, O_RDONLY | O_DIRECTORY);
    si (fd < 0)
    {
        lectiones_notare(LECTIO_ABSENS, via);
        redde NIHIL;
    }
    massa = (character*)memoriae_allocare(_MASSA_ENUMERATIONIS);
    si (massa == NIHIL)
    {
        close(fd);
        redde NIHIL;
    }
#else
    /* <tolera codex="CANALIS_PRAETERITUS" (>canalis ipse
     * directorium_iterator_aperire notat ABSENS/ENUMERAVIT
     * lectiones: notatur */
    descriptum = opendir(via);
    si (descriptum == NIHIL)
    {
        lectiones_notare(LECTIO_ABSENS, via);
        redde NIHIL;
    }
#endif
    lectiones_notare(LECTIO_ENUMERAVIT, via);

    iterator = (DirectoriumIterator*)piscina_allocare(
        piscina,
        magnitudo(DirectoriumIterator)
        );

#ifdef __APPLE__
    iterator->fd       = fd;
    iterator->massa    = massa;
    iterator->cursor   = massa;
    iterator->residua  = 0;
    iterator->finitum  = FALSUM;
#else
    iterator->descriptum  = descriptum;
#endif
    iterator->piscina    = piscina;
    iterator->via_basis  = chorda_ex_literis(via, piscina);

    /* Initiare introitus_nunc */
    iterator->introitus_nunc.titulus.datum    = NIHIL;
    iterator->introitus_nunc.titulus.mensura  = 0;
    iterator->introitus_nunc.genus            = INTROITUS_IGNOTUS;
    iterator->introitus_nunc.mensura          = 0;
    iterator->introitus_nunc.tempus_ns        = 0;
    iterator->introitus_nunc.est_nexus        = FALSUM;

    redde iterator;
}


/* ==================================================
 * Iterator: Proximum
 * ================================================== */

DirectoriumIntroitus*
directorium_iterator_proximum (
    DirectoriumIterator* iterator)
{
#ifdef __APPLE__
    structura attrlist  attributa;

    si (iterator == NIHIL || iterator->massa == NIHIL)
    {
        redde NIHIL;
    }
    per (;;)
    {
                     character* introitus;
                   unsigned int  longitudo;
                attribute_set_t  reddita;
                attrreference_t  referentia_nominis;
                   fsobj_type_t  typus;
            structura timespec  tempus;
                          off_t  mensura;
                     character* campus;
            constans character* nomen_introitus;
                        integer  numerus;

        si (iterator->residua == 0)
        {
            si (iterator->finitum)
            {
                redde NIHIL;
            }
            _attributa_parare(&attributa);
            numerus = getattrlistbulk(iterator->fd, &attributa,
                iterator->massa, (size_t)_MASSA_ENUMERATIONIS, 0);
            si (numerus <= 0)
            {
                iterator->finitum = VERUM;
                redde NIHIL;
            }
            iterator->residua = numerus;
            iterator->cursor  = iterator->massa;
        }
        /* introitus: longitudo, attributa reddita, nomen (referentia),
         * genus, mtime, mensura (si reddita) - ordine attrlist */
        introitus = iterator->cursor;
        memcpy(&longitudo, introitus, magnitudo(longitudo));
        campus = introitus + magnitudo(longitudo);
        memcpy(&reddita, campus, magnitudo(reddita));
        campus += magnitudo(reddita);
        memcpy(&referentia_nominis, campus,
            magnitudo(referentia_nominis));
        nomen_introitus = campus + referentia_nominis.attr_dataoffset;
        campus += magnitudo(referentia_nominis);
        memcpy(&typus, campus, magnitudo(typus));
        campus += magnitudo(typus);
        memcpy(&tempus, campus, magnitudo(tempus));
        campus += magnitudo(tempus);
        mensura = -1;
        si (   (reddita.dirattr & ATTR_DIR_DATALENGTH)
            || (reddita.fileattr & ATTR_FILE_DATALENGTH))
        {
            memcpy(&mensura, campus, magnitudo(mensura));
        }
        iterator->cursor  += longitudo;
        iterator->residua -= 1;

        si (_est_punctum(nomen_introitus))
        {
            perge;
        }
        _nomen_ponere(iterator, nomen_introitus);
        iterator->introitus_nunc.est_nexus = (b32)(typus == VLNK);
        si (typus == VLNK || mensura < 0)
        {
            /* nexus: status SECUTUS (destinatio), ut antea */
            _statum_secutum_implere(iterator);
        }
        alioquin
        {
            iterator->introitus_nunc.genus =
                  typus == VREG ? INTROITUS_FILUM
                : typus == VDIR ? INTROITUS_DIRECTORIUM
                : INTROITUS_ALIUS;
            iterator->introitus_nunc.mensura    =
                (memoriae_index)mensura;
            iterator->introitus_nunc.tempus_ns  =
                (s64)tempus.tv_sec * (s64)M * (s64)M * (s64)M
                + (s64)tempus.tv_nsec;
        }
        redde &(iterator->introitus_nunc);
    }
#else
    structura dirent* introitus;

    si (iterator == NIHIL || iterator->descriptum == NIHIL)
    {
        redde NIHIL;
    }

    /* Legere usque ad introitum validum (saltare . et ..) */
    per (;;)
    {
        introitus = readdir(iterator->descriptum);
        si (introitus == NIHIL)
        {
            redde NIHIL;
        }
        si (!_est_punctum(introitus->d_name))
        {
            frange;
        }
    }
    _nomen_ponere(iterator, introitus->d_name);
    _statum_secutum_implere(iterator);
    iterator->introitus_nunc.est_nexus = (b32)(introitus->d_type
        == DT_LNK);
    si (introitus->d_type == DT_UNKNOWN)
    {
        /* systema genus non dat: lstat solum hic */
        structura stat  status_proprius;
             character via_plena[1024];
                   i32 longitudo_basis = iterator->via_basis.mensura;
                   i32 longitudo_nominis =
                            iterator->introitus_nunc.titulus.mensura;

        si (longitudo_basis + longitudo_nominis + II < 1024)
        {
            memcpy(via_plena, iterator->via_basis.datum,
                (size_t)longitudo_basis);
            via_plena[longitudo_basis] = '/';
            memcpy(via_plena + longitudo_basis + I,
                iterator->buffer_nominis, (size_t)longitudo_nominis);
            via_plena[longitudo_basis + I + longitudo_nominis] = '\0';
            /* <tolera codex="CANALIS_PRAETERITUS" (>canalis ipse
             * per directorium_iterator_aperire (ENUMERAVIT)
             * lectiones: notatur */
            si (lstat(via_plena, &status_proprius) == 0)
            {
                iterator->introitus_nunc.est_nexus =
                    (b32)S_ISLNK(status_proprius.st_mode);
            }
        }
    }
    redde &(iterator->introitus_nunc);
#endif
}


/* ==================================================
 * Iterator: Claudere
 * ================================================== */

vacuum
directorium_iterator_claudere (
    DirectoriumIterator* iterator)
{
#ifdef __APPLE__
    si (iterator != NIHIL && iterator->massa != NIHIL)
    {
        close(iterator->fd);
        liberare(iterator->massa);
        iterator->massa  = NIHIL;
        iterator->fd     = -1;
    }
#else
    si (iterator != NIHIL && iterator->descriptum != NIHIL)
    {
        closedir(iterator->descriptum);
        iterator->descriptum = NIHIL;
    }
#endif
    /* Structura cum piscina liberatur; massa (malloc) hic */
}


/* ==================================================
 * Filtrum: Omnia
 * ================================================== */

DirectoriumFiltrum
directorium_filtrum_omnia (
    vacuum)
{
    DirectoriumFiltrum filtrum;

    filtrum.genera_accepta      = NIHIL;
    filtrum.genera_numerus      = 0;
    filtrum.exemplar.datum      = NIHIL;
    filtrum.exemplar.mensura    = 0;
    filtrum.profunditas_max     = 0;
    filtrum.includere_occultos  = FALSUM;

    redde filtrum;
}


/* ==================================================
 * Pattern Matching
 * ================================================== */

/* Implementatio recursiva pattern matching */
hic_manens b32
_congruit_recursivum (
    constans i8* titulus_data,
            i32  titulus_index,
            i32  titulus_longitudo,
    constans i8* exemplar_data,
            i32  exemplar_index,
            i32  exemplar_longitudo)
{
    character c;

    /* Finis utriusque */
    si (exemplar_index >= exemplar_longitudo)
    {
        redde (titulus_index >= titulus_longitudo);
    }

    /* Character currentis exemplaris */
    c = (character)exemplar_data[exemplar_index];

    si (c == '*')
    {
        /* * congruit cum zero vel plus characteribus */
        /* Probare: * congruit nihil */
        si (_congruit_recursivum(titulus_data, titulus_index,
            titulus_longitudo,
                                  exemplar_data, exemplar_index + I,
                                  exemplar_longitudo))
        {
            redde VERUM;
        }
        /* Probare: * congruit cum uno charactere, tunc continuare */
        si (titulus_index < titulus_longitudo)
        {
            redde _congruit_recursivum(titulus_data, titulus_index + I,
                titulus_longitudo,
                                        exemplar_data, exemplar_index,
                                        exemplar_longitudo);
        }
        redde FALSUM;
    }

    si (c == '?')
    {
        /* ? congruit cum exacte uno charactere */
        si (titulus_index < titulus_longitudo)
        {
            redde _congruit_recursivum(titulus_data, titulus_index + I,
                titulus_longitudo,
                                        exemplar_data, exemplar_index
                                            + I, exemplar_longitudo);
        }
        redde FALSUM;
    }

    /* Character literalis */
    si (   titulus_index < titulus_longitudo
        && (character)titulus_data[titulus_index] == c)
    {
        redde _congruit_recursivum(titulus_data, titulus_index + I,
            titulus_longitudo,
                                    exemplar_data, exemplar_index + I,
                                    exemplar_longitudo);
    }

    redde FALSUM;
}

hic_manens constans character* constans _tituli_systematis[] = {
    ".DS_Store", NIHIL
};

b32
directorium_titulus_systematis (
    chorda titulus)
{
    i32 k;

    per (k = ZEPHYRUM; _tituli_systematis[k] != NIHIL; k++)
    {
        si (chorda_aequalis_literis(titulus, _tituli_systematis[k]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
directorium_titulus_congruit (
    chorda titulus,
    chorda exemplar)
{
    /* Exemplar vacua congruit cum omnibus */
    si (exemplar.datum == NIHIL || exemplar.mensura == 0)
    {
        redde VERUM;
    }

    /* Titulus vacua non congruit (nisi exemplar est solum *) */
    si (titulus.datum == NIHIL || titulus.mensura == 0)
    {
        si (exemplar.mensura == I && exemplar.datum[0] == '*')
        {
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
directorium_existit (
    constans character* via)
{
    structura stat status;

    si (via == NIHIL)
    {
        redde FALSUM;
    }

    /* <tolera codex="CANALIS_PRAETERITUS" (>canalis ipse
     * directorium_existit notat ABSENS/EXSTAT
     * lectiones: notatur */
    si (stat(via, &status) != 0)
    {
        lectiones_notare(LECTIO_ABSENS, via);
        redde FALSUM;
    }
    lectiones_notare(LECTIO_EXSTAT, via);

    redde S_ISDIR(status.st_mode) ? VERUM : FALSUM;
}


/* ==================================================
 * Ambulator: Verificare Filtrum
 * ================================================== */

hic_manens b32
_filtrum_acceptat_genus (
    constans DirectoriumFiltrum* filtrum,
                 IntroitusGenus  genus)
{
    i32 i;

    si (   filtrum == NIHIL || filtrum->genera_accepta == NIHIL
        || filtrum->genera_numerus == 0)
    {
        redde VERUM; /* Nullum filtrum = omnia accepta */
    }

    per (i = 0; i < filtrum->genera_numerus; i++)
    {
        si (filtrum->genera_accepta[i] == genus)
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}

hic_manens b32
_filtrum_acceptat_introitum (
      constans DirectoriumFiltrum* filtrum,
    constans DirectoriumIntroitus* introitus)
{
    /* Verificare occultum */
    si (filtrum != NIHIL && !filtrum->includere_occultos)
    {
        si (   introitus->titulus.mensura > 0
            && introitus->titulus.datum[0] == '.')
        {
            redde FALSUM;
        }
    }

    /* Verificare genus */
    si (!_filtrum_acceptat_genus(filtrum, introitus->genus))
    {
        redde FALSUM;
    }

    /* Verificare exemplar (solum pro filis, non directoriis) */
    si (   filtrum                 != NIHIL
        && filtrum->exemplar.datum != NIHIL
        && filtrum->exemplar.mensura > 0
        && introitus->genus        == INTROITUS_FILUM)
    {
        si (!directorium_titulus_congruit(introitus->titulus,
            filtrum->exemplar))
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}


/* ==================================================
 * Ambulator: Recursio Interna
 * ================================================== */

hic_manens s32
_ambulare_recursivum (
                         chorda  via_currens,
    constans DirectoriumFiltrum* filtrum,
    DirectoriumAmbulatorFunctio  functio,
                         vacuum* contextus,
                        Piscina* piscina,
                            s32  profunditas)
{
     DirectoriumIterator* iterator;
    DirectoriumIntroitus* introitus;
               character* via_cstr;
                  chorda  via_nova;
                  chorda  partes[II];
                     s32  fructus;

    /* Verificare profunditatem maximam */
    si (profunditas > DIRECTORIUM_PROFUNDITAS_MAXIMA)
    {
        redde 0; /* Limite attacto, sed non error */
    }

    /* Verificare profunditas_max in filtro */
    si (filtrum != NIHIL && filtrum->profunditas_max != 0)
    {
        si (filtrum->profunditas_max == (s32)-1)
        {
            redde 0; /* Sine recursione */
        }
        si (profunditas > filtrum->profunditas_max)
        {
            redde 0;
        }
    }

    /* Convertere chorda ad C string pro opendir */
    via_cstr = chorda_ut_cstr(via_currens, piscina);
    si (via_cstr == NIHIL)
    {
        redde -I;
    }

    iterator = directorium_iterator_aperire(via_cstr, piscina);
    si (iterator == NIHIL)
    {
        redde -I;
    }

    dum ((introitus = directorium_iterator_proximum(iterator)) != NIHIL)
    {
        /* Construere viam plenam */
        partes[0]  = via_currens;
        partes[I]  = introitus->titulus;
        via_nova   = via_iungere(partes, II, piscina);

        /* Verificare si filtrum acceptat */
        si (_filtrum_acceptat_introitum(filtrum, introitus))
        {
            /* Vocare callback */
            fructus = functio(via_nova, introitus, contextus);
            si (fructus != 0)
            {
                directorium_iterator_claudere(iterator);
                redde fructus;
            }
        }

        /* Recursio in directoria */
        si (introitus->genus == INTROITUS_DIRECTORIUM)
        {
            /* Verificare si non est occultum (vel includere_occultos) */
            si (   filtrum                     == NIHIL
                || filtrum->includere_occultos
                || introitus->titulus.datum[0] != '.')
            {
                fructus = _ambulare_recursivum(
                    via_nova,
                    filtrum,
                    functio,
                    contextus,
                    piscina,
                    profunditas + I
                    );
                si (fructus != 0)
                {
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
directorium_ambulare (
             constans character* via,
    constans DirectoriumFiltrum* filtrum,
    DirectoriumAmbulatorFunctio  functio,
                         vacuum* contextus,
                        Piscina* piscina)
{
    chorda via_chorda;

    si (via == NIHIL || functio == NIHIL || piscina == NIHIL)
    {
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
