/* icones_instrumentum.c - icones .iconset et .icns ex imagine struere,
 * et tabulam chunkorum .icns legere
 *
 * Usus:
 *   bin/icones_instrumentum -fons <x.png> -radix <directorium>
 *       [-titulus <AppIcon>] [-latera 16,32,64,128,256,512,1024]
 *   bin/icones_instrumentum -legere <x.icns>
 *
 * CUR EXSISTIT: icones bibliotheca imaginem DECODIFICATAM accipit
 * (spec D7), ergo sine hoc instrumento nihil in arbore iconem ex PNG
 * vero facere poterat. stb_image HIC SOLO vivit (imago.h): bibliotheca
 * ipsa clausuram sine vendoribus servat.
 *
 * -legere tabulam chunkorum imprimit, linea per chunkum:
  *   "<codex> <longitudo> png <lat>x<alt>", "<codex> <longitudo> argb"
 *   (Apple ic04/ic05), aut "<codex> <longitudo> aliud"
 * Lector hic in INSTRUMENTO vivit, non in bibliotheca: lector
 * metadatorum in bibliotheca parcatus est (spec par. X), sed
 * probatio_icones_iconutil.sh tabulas nostram et Apple conferre debet.
 * Longitudo magni-endiana octetum post octetum legitur - tertia copia
 * adiutoris IV linearum (lib/icones.c scribit, probatio legit).
 *
 * Exitus: 0 factum; 1 RECUSATUM (causa nominatur); 2 NIHIL FACTUM
 * (argumenta desunt aut ignota).
 */

#include "latina.h"
#include "icones.h"
#include "imago.h"
#include "filum.h"
#include "via.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <string.h>

/* \x89 P N G \r \n \x1a \n */
hic_manens constans i8 SIGNUM_PNG[VIII] = {
    (i8)CXXXVII, (i8)LXXX, (i8)LXXVIII, (i8)LXXI,
    (i8)XIII,    (i8)X,    (i8)XXVI,    (i8)X
};

nomen structura {
    i32 pixela;
    i32 vexillum;
} LatusVexillum;

hic_manens constans LatusVexillum LATERA[VII] = {
    {     XVI, ICONES_LATERA_XVI     },
    {   XXXII, ICONES_LATERA_XXXII   },
    {    LXIV, ICONES_LATERA_LXIV    },
    { CXXVIII, ICONES_LATERA_CXXVIII },
    {   CCLVI, ICONES_LATERA_CCLVI   },
    {    DXII, ICONES_LATERA_DXII    },
    {   MXXIV, ICONES_LATERA_MXXIV   }
};

interior constans character*
_titulus_status (
    IconesStatus status)
{
    commutatio (status)
    {
        casus ICONES_SUCCESSUS:
            redde "SUCCESSUS";
        casus ICONES_ERROR_DESUNT:
            redde "DESUNT";
        casus ICONES_ERROR_FONS:
            redde "FONS";
        casus ICONES_ERROR_MINIMUS:
            redde "MINIMUS";
        casus ICONES_ERROR_LATERA:
            redde "LATERA";
        casus ICONES_ERROR_PNG:
            redde "PNG";
        casus ICONES_ERROR_DIRECTORIUM:
            redde "DIRECTORIUM";
        casus ICONES_ERROR_SCRIPTIO:
            redde "SCRIPTIO";
        casus ICONES_ERROR_MEMORIA:
            redde "MEMORIA";
        ordinarius:
            redde "IGNOTUS";
    }
}

/* chorda_ut_cstr chordam vacuam in NIHIL vertit, et %s cum NIHIL
 * mos indefinitus est */
interior constans character*
_sedes (
      chorda  sedes,
     Piscina* piscina)
{
    constans character* c = chorda_ut_cstr(sedes, piscina);

    redde c ? c : "(nulla)";
}

interior i32
_be32_legere (
    constans i8* p)
{
    redde ((i32)p[ZEPHYRUM] << XXIV) | ((i32)p[I] << XVI)
         | ((i32)p[II] << VIII) | (i32)p[III];
}

interior vacuum
_usus (vacuum)
{
    imprimere("usus:\n");
    imprimere("  icones_instrumentum -fons <x.png> -radix <dir>"
              " [-titulus <t>] [-latera 16,32,...]\n");
    imprimere("  icones_instrumentum -legere <x.icns>\n");
}

/* "16,32,1024" -> vexilla. Latus extra setum Apple argumentum ignotum
 * est (exitus II), non recusatio bibliothecae: vexillum ei nullum
 * est. */
interior b32
_latera_legere (
    constans character* textus,
                   i32* vexilla)
{
     constans character* p;
                    i32  numerus    = ZEPHYRUM;
                    b32  in_numero  = FALSUM;
                    i32  j;

    *vexilla = ZEPHYRUM;
    per (p = textus; ; p++)
    {
        si (*p >= '0' && *p <= '9')
        {
            numerus    = (numerus * X) + (i32)(*p - '0');
            in_numero  = VERUM;
            si (numerus > MMMMXCVI)
            {
                imprimere("latus nimis magnum in -latera\n");
                redde FALSUM;
            }
            perge;
        }
        si (*p != ',' && *p != '\0')
        {
            imprimere("character ignotus in -latera: '%c'\n", *p);
            redde FALSUM;
        }
        si (!in_numero)
        {
            imprimere("latus vacuum in -latera\n");
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < VII; j++)
        {
            si (LATERA[j].pixela == numerus)
            {
                frange;
            }
        }
        si (j == VII)
        {
            imprimere("latus ignotum in -latera: %u"
                      " (16 32 64 128 256 512 1024)\n", numerus);
            redde FALSUM;
        }
        *vexilla |= LATERA[j].vexillum;
        si (*p == '\0')
        {
            redde VERUM;
        }
        numerus    = ZEPHYRUM;
        in_numero  = FALSUM;
    }
}

interior vacuum
_omissa_imprimere (
    i32 omissa)
{
    i32 j;

    si (omissa == ZEPHYRUM)
    {
        redde;
    }
    imprimere("omissa:");
    per (j = ZEPHYRUM; j < VII; j++)
    {
        si ((omissa & LATERA[j].vexillum) != ZEPHYRUM)
        {
            imprimere(" %u", LATERA[j].pixela);
        }
    }
    imprimere("\n");
}

interior s32
_struere (
    constans character* via_fontis,
    constans character* via_radicis,
    constans character* titulus,
                   i32  vexilla,
               Piscina* piscina)
{
     ImagoFructus  decodificata;
           Icones  petitio;
    IconesFructus  fructus;
     IconesStatus  status;
           chorda  sedes;
           chorda  iungenda[II];
        character* via_iconset;
        character* via_icns;

    decodificata = imago_caricare_ex_file(via_fontis, piscina);
    si (!decodificata.successus)
    {
        imprimere("RECUSATUM DECODIFICATIO: %s: %s\n", via_fontis,
                  _sedes(decodificata.error, piscina));
        redde I;
    }

    memset(&petitio, ZEPHYRUM, magnitudo(Icones));
    petitio.fons           = &decodificata.imago;
    petitio.titulus        = chorda_ex_literis(titulus, piscina);
    petitio.recidere       = ICONES_RECIDERE_CENTRUM;
    petitio.latera_petita  = vexilla;

    si (!icones_reddere(&petitio, &fructus, &status, &sedes, piscina))
    {
        imprimere("RECUSATUM %s: %s\n", _titulus_status(status),
                  _sedes(sedes, piscina));
        redde I;
    }
    si (!icones_iconset_scribere(&fructus, via_radicis, &status, &sedes,
                                 piscina))
    {
        imprimere("RECUSATUM %s: %s\n", _titulus_status(status),
                  _sedes(sedes, piscina));
        redde I;
    }

    iungenda[ZEPHYRUM]  = chorda_ex_literis(via_radicis, piscina);
    iungenda[I]         = chorda_concatenare(petitio.titulus,
        chorda_ex_literis(".iconset", piscina), piscina);
    via_iconset         = chorda_ut_cstr(via_iungere(iungenda, II,
                                                     piscina), piscina);
    iungenda[I]         = chorda_concatenare(petitio.titulus,
        chorda_ex_literis(".icns", piscina), piscina);
    via_icns            = chorda_ut_cstr(via_iungere(iungenda, II,
                                                     piscina), piscina);
    si (!via_iconset || !via_icns)
    {
        imprimere("RECUSATUM MEMORIA: viae\n");
        redde I;
    }

    si (!icones_icns_scribere(&fructus, via_icns, &status, &sedes,
                              piscina))
    {
        imprimere("RECUSATUM %s: %s\n", _titulus_status(status),
                  _sedes(sedes, piscina));
        redde I;
    }

    imprimere("%s\n", via_iconset);
    imprimere("%s\n", via_icns);
    imprimere("partes %u, latus fontis %u\n",
              xar_numerus(fructus.partes), fructus.latera_fontis);
    _omissa_imprimere(fructus.omissa);
    redde ZEPHYRUM;
}

/* Tabula chunkorum. Custodiae ANTE quamque lectionem: signum,
 * longitudo declarata == vera, caput chunki integrum, longitudo >= VIII
 * nec ultra finem - aliter RECUSATUM cum sede, numquam lectio extra
 * limites. */
interior s32
_legere (
    constans character* via,
               Piscina* piscina)
{
    chorda  continens;
       i32  positio;
       i32  longitudo;
       i32  declarata;
        i8* onus;
       i32  mensura_oneris;

    continens = filum_legere_totum(via, piscina);
    si (continens.datum == NIHIL || continens.mensura == ZEPHYRUM)
    {
        imprimere("RECUSATUM LECTIO: %s\n", via);
        redde I;
    }
    si (   continens.mensura < (i32)VIII
        || memcmp(continens.datum, "icns", (size_t)IV) != ZEPHYRUM)
    {
        imprimere("RECUSATUM FORMA: signum 'icns' abest\n");
        redde I;
    }
    declarata = _be32_legere(continens.datum + IV);
    si (declarata != continens.mensura)
    {
        imprimere("RECUSATUM LONGITUDO: declarata %u, vera %u\n",
                  declarata, continens.mensura);
        redde I;
    }

    positio = (i32)VIII;
    dum (positio < continens.mensura)
    {
        si (continens.mensura - positio < (i32)VIII)
        {
            imprimere("RECUSATUM CHUNKUS: caput truncum ad %u\n",
                      positio);
            redde I;
        }
        longitudo = _be32_legere(continens.datum + positio + IV);
        si (   longitudo < (i32)VIII
            || longitudo > continens.mensura - positio)
        {
            imprimere("RECUSATUM CHUNKUS: ad %u longitudo %u\n",
                      positio, longitudo);
            redde I;
        }
        onus            = continens.datum + positio + VIII;
        mensura_oneris  = longitudo - (i32)VIII;
        si (   mensura_oneris                         >= (i32)XXIV
            && memcmp(onus, SIGNUM_PNG, (size_t)VIII) == ZEPHYRUM
            && memcmp(onus + XII, "IHDR", (size_t)IV) == ZEPHYRUM)
        {
            imprimere("%.4s %u png %ux%u\n",
                      (character*)(continens.datum + positio),
                      longitudo, _be32_legere(onus + XVI),
                      _be32_legere(onus + XX));
        }
        alioquin si (   mensura_oneris                   >= (i32)IV
                     && memcmp(onus, "ARGB", (size_t)IV) == ZEPHYRUM)
        {
            /* Apple ic04/ic05 sic implet (iconutil, mensuratum
             * 2026-09-12): ARGB compressum, non PNG */
            imprimere("%.4s %u argb\n",
                      (character*)(continens.datum + positio),
                      longitudo);
        }
        alioquin
        {
            imprimere("%.4s %u aliud\n",
                      (character*)(continens.datum + positio),
                      longitudo);
        }
        positio += longitudo;
    }
    redde ZEPHYRUM;
}

s32
principale (
       integer   numerus,
     character** argumenta)
{
                Piscina* piscina;
     constans character* via_fontis   = NIHIL;
     constans character* via_radicis  = NIHIL;
     constans character* titulus      = "AppIcon";
     constans character* latera       = NIHIL;
     constans character* via_legenda  = NIHIL;
                    i32  vexilla      = ZEPHYRUM;
                integer  i;
                    s32  fructus      = II;

    per (i = I; i < numerus; i++)
    {
        constans character* a = argumenta[i];

        si (i + I >= numerus)
        {
            imprimere("argumentum sine valore: %s\n", a);
            _usus();
            redde II;
        }
        alioquin si (strcmp(a, "-fons") == ZEPHYRUM)
        {
            via_fontis = argumenta[++i];
        }
        alioquin si (strcmp(a, "-radix") == ZEPHYRUM)
        {
            via_radicis = argumenta[++i];
        }
        alioquin si (strcmp(a, "-titulus") == ZEPHYRUM)
        {
            titulus = argumenta[++i];
        }
        alioquin si (strcmp(a, "-latera") == ZEPHYRUM)
        {
            latera = argumenta[++i];
        }
        alioquin si (strcmp(a, "-legere") == ZEPHYRUM)
        {
            via_legenda = argumenta[++i];
        }
        alioquin
        {
            imprimere("argumentum ignotum: %s\n", a);
            _usus();
            redde II;
        }
    }

    si (!via_legenda && (!via_fontis || !via_radicis))
    {
        _usus();
        redde II;
    }
    si (latera && !_latera_legere(latera, &vexilla))
    {
        _usus();
        redde II;
    }

    piscina = piscina_generare_dynamicum("icones_instrumentum",
                                         16777216);
    si (!piscina)
    {
        imprimere("piscina creari non potuit\n");
        redde II;
    }

    si (via_legenda)
    {
        fructus = _legere(via_legenda, piscina);
    }
    alioquin
    {
        fructus = _struere(via_fontis, via_radicis, titulus, vexilla,
                           piscina);
    }

    piscina_destruere(piscina);
    redde fructus;
}
