/* icones.c - icones .app (contractus in capite).
 *
 * SEPTEM redditiones, DECEM plagulae: tabula ORDINES ter idem latus
 * sub nominibus logicis diversis nominat, ergo quodque latus SEMEL
 * scalatur et SEMEL codificatur, et partes geminae eundem indicem
 * octetorum ferunt. Ita .iconset et .icns constructione consentiunt,
 * non duabus semitis quae consentire debent.
 *
 * Nihil decodificat: Imago a vocante venit (par. III, D7).
 */

#include "icones.h"
#include "imago_opus.h"
#include "imago_png.h"
#include "filum.h"
#include "via.h"
#include "chorda_aedificator.h"

#include <string.h>

nomen structura {
                i32  latera;
 constans character* semita;
 constans character* codex;     /* codex .icns */
                i32  vexillum;
} IconesOrdo;

/* Mensuratum per iconutil 2026-09-12 (spec par. IV), non ex chartis:
 * tria latera BIS apparent, et paria gemina octetim aequalia exeunt. */
hic_manens constans IconesOrdo ORDINES[X] = {
    {     XVI, "icon_16x16.png",      "ic04", ICONES_LATERA_XVI     },
    {   XXXII, "icon_16x16@2x.png",   "ic11", ICONES_LATERA_XXXII   },
    {   XXXII, "icon_32x32.png",      "ic05", ICONES_LATERA_XXXII   },
    {    LXIV, "icon_32x32@2x.png",   "ic12", ICONES_LATERA_LXIV    },
    { CXXVIII, "icon_128x128.png",    "ic07", ICONES_LATERA_CXXVIII },
    {   CCLVI, "icon_128x128@2x.png", "ic13", ICONES_LATERA_CCLVI   },
    {   CCLVI, "icon_256x256.png",    "ic08", ICONES_LATERA_CCLVI   },
    {    DXII, "icon_256x256@2x.png", "ic14", ICONES_LATERA_DXII    },
    {    DXII, "icon_512x512.png",    "ic09", ICONES_LATERA_DXII    },
    {   MXXIV, "icon_512x512@2x.png", "ic10", ICONES_LATERA_MXXIV   }
};

/* Nomina camporum ut sedes recusationum, in memoria STATICA: sedes
 * etiam ubi piscina ipsa deest nominari debet. Tabulae MUTABILES, non
 * literae: literae sub vexillis domus constantes sunt, et const in i8*
 * abiicere -Wcast-qual vetat. */
hic_manens character SEDES_PETITIO[]      = "petitio";
hic_manens character SEDES_FRUCTUS[]      = "fructus";
hic_manens character SEDES_PISCINA[]      = "piscina";
hic_manens character SEDES_TITULUS[]      = "titulus";
hic_manens character SEDES_FONS[]         = "fons";
hic_manens character SEDES_PIXELA[]       = "pixela";
hic_manens character SEDES_LATITUDO[]     = "latitudo";
hic_manens character SEDES_ALTITUDO[]     = "altitudo";
hic_manens character SEDES_PARTES[]       = "partes";
hic_manens character SEDES_VIA[]          = "via";
hic_manens character SEDES_VIA_RADICIS[]  = "via_radicis";

#define SEDES_FIXA(tabula) \
    chorda_ex_buffer((i8*)(tabula), (i32)(magnitudo(tabula) - I))

interior b32
_recusare (
    IconesStatus* status,
          chorda* sedes_vitii,
    IconesStatus  valor,
          chorda  sedes)
{
    *status       = valor;
    *sedes_vitii  = sedes;
    redde FALSUM;
}

/* Quadratum ad centrum, deinde ad latera exacta. Dimensiones IPSI
 * computamus et non 'intra limites' petimus, quia creatores
 * aspectum servantes rationem et productum truncant (imago_opus.c:
 * 263-271) et 512 ex crop 512 lato 511 dare possunt. Ad 1:1 nihil
 * scalatur: imago fontis ipsa redditur. */
interior Imago
_quadratum_scalare (
     constans Imago* fons,
                s32  latus_fontis,
                s32  latera,
            Piscina* piscina)
{
    s32 x = ((s32)fons->latitudo - latus_fontis) / II;
    s32 y = ((s32)fons->altitudo - latus_fontis) / II;

    si (latera == latus_fontis && x == ZEPHYRUM && y == ZEPHYRUM)
    {
        redde *fons;
    }
    redde imago_extrahere_et_scalare(fons, (i32)x, (i32)y,
                                     (i32)latus_fontis,
                                     (i32)latus_fontis, (i32)latera,
                                     (i32)latera, IMAGO_SCALA_AREA,
                                     piscina);
}

/* Longitudo magni-endiana octetum post octetum, significantissimum
 * PRIMUM - numquam memcpy i32 hospitis, qui in machina parvi-endiana
 * inversus scriberetur. Forma ipsa imago_png.c (ibi interior, ergo
 * hic iterata). */
interior vacuum
_be32_scribere (
     i8* destinatio,
    i32  valor)
{
    destinatio[ZEPHYRUM]  = (i8)((valor >> XXIV) & CCLV);
    destinatio[I]         = (i8)((valor >> XVI) & CCLV);
    destinatio[II]        = (i8)((valor >> VIII) & CCLV);
    destinatio[III]       = (i8)(valor & CCLV);
}

/* Codex .icns per SEMITAM partis. Partes codicem non ferunt (spec
 * par. III) et ordinem tabulae CUM LACUNIS sequuntur (latera omissa
 * aut non petita), ergo index directus in ORDINES falleret. NIHIL si
 * semita tabulae ignota est. */
interior constans character*
_codicem_invenire (
    chorda semita)
{
    i32 i;

    per (i = ZEPHYRUM; i < X; i++)
    {
        si (chorda_aequalis_literis(semita, ORDINES[i].semita))
        {
            redde ORDINES[i].codex;
        }
    }
    redde NIHIL;
}

/* Chunkus unus .icns: codex IV litterarum, longitudo magni-endiana
 * INCLUSO capite VIII octetorum, onus PNG. Longitudo SCRIPTA et sedes
 * REDDITA separatim computantur: culpa in longitudine tabulam in
 * memoria non rumpit, sed ambulationem legentis (I3) fallit.
 *
 * Redde: sedes proxima post chunkum. */
interior i32
_ordinem_scribere (
                 i8* tela,
                i32  sedes,
 constans character* codex,
             chorda  octeti)
{
    tela[sedes]        = (i8)codex[ZEPHYRUM];
    tela[sedes + I]    = (i8)codex[I];
    tela[sedes + II]   = (i8)codex[II];
    tela[sedes + III]  = (i8)codex[III];

    _be32_scribere(tela + sedes + IV, (i32)VIII + octeti.mensura);
    memcpy(tela + sedes + VIII, octeti.datum, (size_t)octeti.mensura);

    redde sedes + (i32)VIII + octeti.mensura;
}

/* Latera petita ut textus ("128 1024"): sedes recusationis quae QUID
 * petitum sit dicit, non vexilla nuda. Ordines tabulae ascendunt et
 * gemini adiacent, ergo latus ultimum scriptum duplicata tollit.
 * Magnitudo IGNOTA et TEXTUS: munus ipsum chorda_aedificator (cf.
 * continens .icns, ubi magnitudo nota est et octeti crudi). */
interior chorda
_latera_nominare (
         i32  petita,
     Piscina* piscina)
{
    ChordaAedificator* aedificator;
               chorda  vacua;
                  i32  ultimum;
                  i32  i;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    aedificator = chorda_aedificator_creare(piscina, XXXII);
    si (!aedificator)
    {
        redde vacua;
    }
    ultimum = ZEPHYRUM;
    per (i = ZEPHYRUM; i < X; i++)
    {
        si (   (petita & ORDINES[i].vexillum) == ZEPHYRUM
            || ORDINES[i].latera              == ultimum)
        {
            perge;
        }
        si (ultimum != ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(aedificator, ' ');
        }
        chorda_aedificator_appendere_i32(aedificator,
            ORDINES[i].latera);
        ultimum = ORDINES[i].latera;
    }
    redde chorda_aedificator_finire(aedificator);
}

/* Fructus IUDICATUR antequam scriptor quicquam tangat. Fructus manu
 * factus (non per icones_reddere) partes nullas, semitam tabulae
 * ignotam aut octetos vacuos ferre potest, et quisque .icns aut
 * .iconset VALIDUM sed inanem daret: icon vacuus sine errore, forma
 * ipsa quam haec bibliotheca claudere exsistit. Semita VACUA hic
 * LATERA est, non SCRIPTIO: via_iungere partes vacuas praeterit, ergo
 * sine iudice in directorium ipsum scriberetur.
 *
 * Sedes: nomen campi, aut SEMITA partis vitiosae. */
interior b32
_fructum_iudicare (
    constans IconesFructus* fructus,
              IconesStatus* status,
                    chorda* sedes_vitii)
{
    i32 numerus;
    i32 i;

    si (!fructus)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_FRUCTUS));
    }
    si (!fructus->partes || xar_numerus(fructus->partes) == ZEPHYRUM)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_PARTES));
    }
    numerus = xar_numerus(fructus->partes);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        IconesPars* pars = (IconesPars*)xar_obtinere(fructus->partes,
            i);

        /* intra limites NUMQUAM NIHIL: ramus defensivus quem nulla
         * probatio attingit, ergo nulla planta */
        si (!pars)
        {
            redde _recusare(status, sedes_vitii, ICONES_ERROR_MEMORIA,
                            SEDES_FIXA(SEDES_PARTES));
        }
        si (!_codicem_invenire(pars->semita))
        {
            redde _recusare(status, sedes_vitii, ICONES_ERROR_LATERA,
                            pars->semita);
        }
        si (chorda_vacua(pars->octeti))
        {
            redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                            pars->semita);
        }
    }
    redde VERUM;
}

b32
icones_reddere (
    constans Icones* petitio,
      IconesFructus* fructus,
       IconesStatus* status,
             chorda* sedes_vitii,
            Piscina* piscina)
{
    chorda vacua;
    chorda cache[X];        /* octeti per ORDINEM, communes geminis */
       b32 habet[X];
       i32 petita;
       s32 latus;
       i32 i;
       i32 j;
       b32 tegit;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    si (!status || !sedes_vitii)
    {
        redde FALSUM;
    }
    *status       = ICONES_SUCCESSUS;
    *sedes_vitii  = vacua;

    si (!petitio)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_PETITIO));
    }
    si (!fructus)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_FRUCTUS));
    }
    si (!piscina)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_PISCINA));
    }
    si (chorda_vacua(petitio->titulus))
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_TITULUS));
    }

    /* Fons ABSENS = DESUNT, ut commentarium enumerationis dicit ('fons
     * aut titulus'); fons PRAESENS sed vitiosus = FONS, campo nominato.
     * Olim ambo FONS erant, contra caput ipsum. */
    si (!petitio->fons)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_FONS));
    }
    si (!petitio->fons->pixela)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_FONS,
                        SEDES_FIXA(SEDES_PIXELA));
    }
    si (petitio->fons->latitudo == (i32)ZEPHYRUM)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_FONS,
                        SEDES_FIXA(SEDES_LATITUDO));
    }
    si (petitio->fons->altitudo == (i32)ZEPHYRUM)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_FONS,
                        SEDES_FIXA(SEDES_ALTITUDO));
    }

    latus = ((s32)petitio->fons->latitudo
             < (s32)petitio->fons->altitudo)
        ? (s32)petitio->fons->latitudo : (s32)petitio->fons->altitudo;

    si (latus < XVI)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_MINIMUS,
                        chorda_ex_s64((s64)latus, piscina));
    }

    petita = (petitio->latera_petita == ZEPHYRUM)
        ? (i32)ICONES_SETUM_APPLE : petitio->latera_petita;
    /* bita IGNOTA SOLA nominantur, non petitio tota */
    si ((petita & ~((i32)ICONES_SETUM_APPLE)) != ZEPHYRUM)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_LATERA,
            chorda_ex_s64((s64)(petita & ~((i32)ICONES_SETUM_APPLE)),
                          piscina));
    }

    /* D10 LATIUS: fons qui latus PETITUM nullum tegit nihil reddit, et
     * successus sine parte icon nullus esset - recusatio, non fructus
     * vacuus cum omissis plenis. Sedes latera petita nominat. */
    tegit = FALSUM;
    per (i = ZEPHYRUM; i < X; i++)
    {
        si (   (petita & ORDINES[i].vexillum) != ZEPHYRUM
            && ORDINES[i].latera              <= (i32)latus)
        {
            tegit = VERUM;
            frange;
        }
    }
    si (!tegit)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_MINIMUS,
                        _latera_nominare(petita, piscina));
    }

    fructus->partes         = xar_creare(piscina,
        (i32)magnitudo(IconesPars));
    fructus->omissa         = ZEPHYRUM;
    fructus->latera_fontis  = (i32)latus;
    fructus->titulus        = petitio->titulus;
    si (!fructus->partes)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_MEMORIA,
                        vacua);
    }

    per (i = ZEPHYRUM; i < X; i++)
    {
        cache[i] = vacua;
        habet[i] = FALSUM;
    }

    per (i = ZEPHYRUM; i < X; i++)
    {
         IconesPars* pars;
                b32  iam = FALSUM;

        si ((petita & ORDINES[i].vexillum) == ZEPHYRUM)
        {
            perge;
        }
        si (ORDINES[i].latera > (i32)latus)
        {
            fructus->omissa |= ORDINES[i].vexillum;
            perge;
        }

        /* octeti per LATUS communes: ordinem priorem eiusdem lateris
         * quaerimus antequam iterum codificemus */
        per (j = ZEPHYRUM; j < i; j++)
        {
            si (habet[j] && ORDINES[j].latera == ORDINES[i].latera)
            {
                cache[i]  = cache[j];
                habet[i]  = VERUM;
                iam       = VERUM;
                frange;
            }
        }
        si (!iam)
        {
                  Imago parva;
             PngFructus png;

            parva = _quadratum_scalare(petitio->fons, latus,
                                       (s32)ORDINES[i].latera,
                                       piscina);
            si (parva.pixela == NIHIL)
            {
                redde _recusare(status, sedes_vitii,
                                ICONES_ERROR_MEMORIA, vacua);
            }
            png = imago_png_codificare(&parva, piscina);
            si (!png.successus)
            {
                /* nuntius codificatoris IPSIUS, non meus generalis */
                redde _recusare(status, sedes_vitii, ICONES_ERROR_PNG,
                                png.error);
            }
            cache[i].datum    = png.datum;
            cache[i].mensura  = png.mensura;
            habet[i]          = VERUM;
        }

        pars = (IconesPars*)xar_addere(fructus->partes);
        si (!pars)
        {
            redde _recusare(status, sedes_vitii, ICONES_ERROR_MEMORIA,
                            vacua);
        }
        pars->latera = ORDINES[i].latera;
        pars->semita = chorda_ex_literis(ORDINES[i].semita, piscina);
        pars->octeti = cache[i];
    }
    redde VERUM;
}

b32
icones_iconset_scribere (
    constans IconesFructus* fructus,
        constans character* via_radicis,
              IconesStatus* status,
                    chorda* sedes_vitii,
                   Piscina* piscina)
{
        chorda  vacua;
        chorda  relativa;     /* "AppIcon.iconset" - in sedibus */
        chorda  directorium;
        chorda  iungenda[II];
     character* via_directorii;
           i32  numerus;
           i32  i;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    si (!status || !sedes_vitii)
    {
        redde FALSUM;
    }
    *status       = ICONES_SUCCESSUS;
    *sedes_vitii  = vacua;

    si (!via_radicis)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_VIA_RADICIS));
    }
    si (!piscina)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_PISCINA));
    }
    si (!_fructum_iudicare(fructus, status, sedes_vitii))
    {
        redde FALSUM;
    }
    /* titulus vacuus directorium '.iconset' daret - OCCULTUM et sine
     * nomine */
    si (chorda_vacua(fructus->titulus))
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_TITULUS));
    }

    /* Suffixum .iconset FORMAE est, non electionis: iconutil
     * directorium sine eo recusat. Ergo bibliotheca id possidet. */
    relativa = chorda_concatenare(fructus->titulus,
        chorda_ex_literis(".iconset", piscina), piscina);
    iungenda[ZEPHYRUM]  = chorda_ex_literis(via_radicis, piscina);
    iungenda[I]         = relativa;
    directorium         = via_iungere(iungenda, II, piscina);
    via_directorii      = chorda_ut_cstr(directorium, piscina);
    /* allocatio sola hic deficere potest: nulla probatio id attingit */
    si (!relativa.datum || !via_directorii)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_MEMORIA,
                        vacua);
    }

    /* Consumptor ALTER mkdir -p (primus fasciculum): radix nidificata
     * cuius parentes absunt aliter ENOENT caderet */
    si (!filum_directorium_creare_cum_parentibus(via_directorii))
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DIRECTORIUM,
                        relativa);
    }

    numerus = xar_numerus(fructus->partes);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        IconesPars* pars;
            chorda  via_plagulae;
         character* via_c;

        /* partes iam IUDICATAE: intra limites, semita nota */
        pars                = (IconesPars*)xar_obtinere(fructus->partes,
            i);
        iungenda[ZEPHYRUM]  = directorium;
        iungenda[I]         = pars->semita;
        via_plagulae        = via_iungere(iungenda, II, piscina);
        via_c               = chorda_ut_cstr(via_plagulae, piscina);
        si (!via_c)
        {
            redde _recusare(status, sedes_vitii, ICONES_ERROR_MEMORIA,
                            vacua);
        }

        /* partes geminae EOSDEM octetos scribunt: nihil hic
         * duplicatur nisi scriptio ipsa */
        si (!filum_scribere(via_c, pars->octeti))
        {
            iungenda[ZEPHYRUM] = relativa;
            redde _recusare(status, sedes_vitii, ICONES_ERROR_SCRIPTIO,
                            via_iungere(iungenda, II, piscina));
        }
    }
    redde VERUM;
}

chorda
icones_icns_codificare (
    constans IconesFructus* fructus,
              IconesStatus* status,
                   Piscina* piscina)
{
    chorda vacua;
    chorda continens;
    chorda sedes_neglecta;   /* iudex sedem poscit; codificatio sedem
                               * non refert */
        i8* tela;
       i32  mensura;
       i32  sedes;
       i32  numerus;
       i32  i;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    si (!status)
    {
        redde vacua;
    }
    *status = ICONES_SUCCESSUS;

    si (!piscina)
    {
        *status = ICONES_ERROR_DESUNT;
        redde vacua;
    }
    si (!_fructum_iudicare(fructus, status, &sedes_neglecta))
    {
        redde vacua;
    }

    /* Magnitudo NOTA ante allocationem, ergo tela semel allocatur - non
     * chorda_aedificator, qui magnitudini ignotae et TEXTO destinatur
     * (caput eius ipsum id dicit). Partes iam IUDICATAE: codex cuiusque
     * notus, octeti non vacui. */
    numerus = xar_numerus(fructus->partes);
    mensura = (i32)VIII;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        IconesPars* pars = (IconesPars*)xar_obtinere(fructus->partes,
            i);

        mensura += (i32)VIII + pars->octeti.mensura;
    }

    tela = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (!tela)
    {
        *status = ICONES_ERROR_MEMORIA;
        redde vacua;
    }

    memcpy(tela, "icns", (size_t)IV);
    _be32_scribere(tela + IV, mensura);

    sedes = (i32)VIII;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        IconesPars* pars = (IconesPars*)xar_obtinere(fructus->partes,
            i);

        sedes = _ordinem_scribere(tela, sedes,
                                  _codicem_invenire(pars->semita),
                                  pars->octeti);
    }

    continens.datum    = tela;
    continens.mensura  = mensura;
    redde continens;
}

b32
icones_icns_scribere (
    constans IconesFructus* fructus,
        constans character* via,
              IconesStatus* status,
                    chorda* sedes_vitii,
                   Piscina* piscina)
{
    chorda vacua;
    chorda continens;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    si (!status || !sedes_vitii)
    {
        redde FALSUM;
    }
    *status       = ICONES_SUCCESSUS;
    *sedes_vitii  = vacua;

    si (!via)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_VIA));
    }
    si (!piscina)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_PISCINA));
    }
    /* iudicium HIC quoque, non solum in codificatione: sedem fert */
    si (!_fructum_iudicare(fructus, status, sedes_vitii))
    {
        redde FALSUM;
    }

    /* Gemellus TENUIS: octetos codificationis purae scribit et nihil
     * aliud computat, ergo plagula et chorda pura constructione
     * congruunt (I6 id octetim probat). Post iudicium sola MEMORIA hic
     * cadere potest. */
    continens = icones_icns_codificare(fructus, status, piscina);
    si (*status != ICONES_SUCCESSUS)
    {
        redde FALSUM;
    }
    si (!filum_scribere(via, continens))
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_SCRIPTIO,
                        chorda_ex_literis(via, piscina));
    }
    redde VERUM;
}
