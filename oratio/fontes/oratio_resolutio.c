/* oratio_resolutio.c - Resolutio ambiguitatis per contextum (T17) */

#include "oratio_resolutio.h"
#include "oratio_registrum.h"
#include "oratio_stml.h"
#include "oratio_partes.h"
#include "oratio_lexema.h"
#include "oratio_lexicon.h"
#include "oratio_forma.h"
#include "materia_arbor.h"
#include "stml.h"
#include "stml_macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior b32
_numerus_attributi (
             StmlNodus* nodus,
    constans character* titulus,
                   i32* exitus);

/* T38 b: tabula ponderum ex plagula (definitio ad finem) */
interior b32
_pondera_onerare (
                     Piscina* piscina,
          constans character* radix,
             OratioProgramma* programma,
    OratioVocabulariumVitium* vitium);

nomen structura {
                       Piscina* piscina;
           InternamentumChorda* intern;
  constans MateriaLexiconRatum* ratum;
      constans OratioProgramma* programma;
                           s32  regulae_numerus;
            constans character* lingua;
         OratioResolutioCensus* census;
    /* probatio cursuum plurium (2026-09-09): prior casuum hoc cursu? */
                            b32  prior_activus;
       constans OratioContextus* contextus;   /* T38 a: dialectus, forma */
} Cursus;

interior chorda
_chorda (
     i8* datum,
    i32  mensura)
{
    chorda c;

    c.datum    = datum;
    c.mensura  = mensura;
    redde c;
}

interior chorda
_copia (
    Piscina* piscina,
     chorda  c)
{
    chorda d;

    d.datum = (i8*)piscina_allocare(piscina, (memoriae_index)c.mensura
        + I);
    si (d.datum == NIHIL)
    {
        redde _chorda(NIHIL, ZEPHYRUM);
    }
    si (c.mensura > ZEPHYRUM)
    {
        memcpy(d.datum, c.datum, (size_t)c.mensura);
    }
    d.datum[c.mensura]  = ZEPHYRUM;
    d.mensura           = c.mensura;
    redde d;
}

interior b32
_aequalis (
                chorda  c,
    constans character* literae)
{
    i32 l = (i32)strlen(literae);

    redde (b32)(c.mensura == l
        && (l == ZEPHYRUM
            || memcmp(c.datum, literae, (size_t)l) == ZEPHYRUM));
}

interior b32
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                chorda* exitus)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    exitus->datum    = NIHIL;
    exitus->mensura  = ZEPHYRUM;
    f                = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde FALSUM;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde FALSUM;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (memoria == NIHIL)
    {
        fclose(f);
        redde FALSUM;
    }
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde FALSUM;
    }
    memoria[longitudo]  = ZEPHYRUM;
    exitus->datum       = (i8*)memoria;
    exitus->mensura     = (i32)longitudo;
    redde VERUM;
}

/* liberi elementi titulo dato ordine (Xar de StmlNodus*) */
interior Xar*
_liberi_titulo (
               Piscina* piscina,
             StmlNodus* nodus,
    constans character* titulus)
{
    Xar* exitus = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
    i32  k;

    si (exitus == NIHIL || nodus == NIHIL || nodus->liberi == NIHIL)
    {
        redde exitus;
    }
    per (k = ZEPHYRUM; k < xar_numerus(nodus->liberi); k++)
    {
        StmlNodus* l = *(StmlNodus**)xar_obtinere(nodus->liberi, k);

        si (   l          != NIHIL && l->genus == STML_NODUS_ELEMENTUM
            && l->titulus != NIHIL && _aequalis(*l->titulus, titulus))
        {
            StmlNodus** s = (StmlNodus**)xar_addere(exitus);

            si (s == NIHIL)
            {
                redde NIHIL;
            }
            *s = l;
        }
    }
    redde exitus;
}

OratioProgramma*
oratio_resolutio_programma_legere (
                     Piscina* piscina,
         InternamentumChorda* intern,
                      chorda  textus,
    OratioVocabulariumVitium* vitium)
{
     OratioProgramma* p;
        StmlResultus  lectio;
                 Xar* regulae;
                 i32  k;

    memset(vitium, ZEPHYRUM, magnitudo(*vitium));
    vitium->plagula = "resolutio.stml";
    p = (OratioProgramma*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(*p));
    si (p == NIHIL)
    {
        vitium->causa = "memoria";
        redde NIHIL;
    }
    p->textus = _copia(piscina, textus);
    p->regulae = xar_creare(piscina, (i32)magnitudo(OratioRegula));
    p->fiducia = NIHIL;   /* T32 a: onerare ponit */
    p->contentiones = NIHIL;   /* T32 c */
    p->pondera = NIHIL;   /* T38 b */
    si (p->textus.datum == NIHIL || p->regulae == NIHIL)
    {
        vitium->causa = "memoria";
        redde NIHIL;
    }
    lectio = stml_legere(p->textus, piscina, intern);
    si (!lectio.successus || lectio.radix == NIHIL)
    {
        vitium->linea = lectio.linea_erroris;
        vitium->causa = "programma non legitur";
        redde NIHIL;
    }
    regulae = _liberi_titulo(piscina, lectio.radix, "regula");
    si (regulae == NIHIL)
    {
        vitium->causa = "memoria";
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(regulae); k++)
    {
           StmlNodus* n        = *(StmlNodus**)xar_obtinere(regulae, k);
              chorda* titulus  = stml_attributum_capere(n, "titulus");
        OratioRegula* r;

        si (titulus == NIHIL || titulus->mensura == ZEPHYRUM)
        {
            vitium->linea = n->linea;
            vitium->causa = "regula sine titulo";
            redde NIHIL;
        }
        si (   n->positus_finis <= n->positus_initium
            || n->positus_finis > p->textus.mensura)
        {
            vitium->linea = n->linea;
            vitium->causa = "regula sine extento";
            redde NIHIL;
        }
        r = (OratioRegula*)xar_addere(p->regulae);
        si (r == NIHIL)
        {
            vitium->causa = "memoria";
            redde NIHIL;
        }
                r->titulus  = _copia(piscina, *titulus);
        r->textus   = _chorda(p->textus.datum + n->positus_initium,
            n->positus_finis - n->positus_initium);
        /* gradus (T19d beta): attributum 'gradus' (>= I), absens = I */
        {
            i32 gradus;

            r->gradus = _numerus_attributi(n, "gradus", &gradus)
                && gradus > ZEPHYRUM ? gradus : I;
        }
    }
    redde p;
}

/* T32 a: numerus decimalis chordae (digiti soli); -I si nullus */
interior s32
_numerus_decimalis (
    chorda c)
{
    s32 n = ZEPHYRUM;
    i32 i;

    si (c.mensura == ZEPHYRUM)
    {
        redde (s32)-I;
    }
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (c.datum[i] < '0' || c.datum[i] > '9')
        {
            redde (s32)-I;
        }
        n = n * (s32)X + (s32)(c.datum[i] - '0');
    }
    redde n;
}

/* T32 a: tabulam fiduciae legere (radix/oratio/probationes/fixa/
 * auctores.tsv: '#' commentum; titulus TAB arcus TAB rectae TAB permille)
 * in programma->fiducia; plagula absens = NIHIL (nihil ordinatur).
 * FALSUM = memoria sola. */
interior b32
_fiduciam_onerare (
               Piscina* piscina,
    constans character* radix,
       OratioProgramma* programma)
{
    character via[1024];
       chorda fons;
          i32 i = ZEPHYRUM;

    sprintf(via, "%s/oratio/probationes/fixa/auctores.tsv", radix);
    si (!_plagulam_legere(piscina, via, &fons))
    {
        redde VERUM;
    }
    programma->fiducia = tabula_dispersa_creare_chorda(piscina,
        (i32)128);
    si (programma->fiducia == NIHIL)
    {
        redde FALSUM;
    }
    dum (i < fons.mensura)
    {
        i32 finis = i;
        i32 campi[4];
        i32 n = ZEPHYRUM;
        i32 k;

        dum (finis < fons.mensura && fons.datum[finis] != '\n')
        {
            finis = finis + I;
        }
        si (finis > i && fons.datum[i] != '#')
        {
            campi[n]  = i;
            n         = I;
            per (k = i; k < finis && n < (i32)IV; k++)
            {
                si (fons.datum[k] == '\t')
                {
                    campi[n]  = k + I;
                    n         = n + I;
                }
            }
            si (n == (i32)IV)
            {
                chorda titulus = _chorda(fons.datum + campi[ZEPHYRUM],
                    campi[I] - I - campi[ZEPHYRUM]);
                s32 permille = _numerus_decimalis(_chorda(fons.datum
                    + campi[(i32)III], finis - campi[(i32)III]));
                i32* valor = (i32*)piscina_allocare(piscina,
                    (memoriae_index)magnitudo(i32));
                chorda clavis = _copia(piscina, titulus);

                si (valor == NIHIL || clavis.datum == NIHIL)
                {
                    redde FALSUM;
                }
                *valor = permille
                    >= ZEPHYRUM ? (i32)permille : ZEPHYRUM;
                (vacuum)tabula_dispersa_inserere(programma->fiducia,
                    clavis, valor);
            }
        }
        i = finis + I;
    }
    redde VERUM;
}

/* T32 c: IUDICIUM contentionis (par regularum) */
nomen structura {
    i32 numerus;
    i32 permille;   /* victae rectae */
} Iudicium;

/* T32 c: tabulam iudicis legere (radix/oratio/probationes/fixa/
 * contentiones.tsv: '#' commentum; victor TAB victa TAB numerus TAB rectae
 * TAB permille) in programma->contentiones; plagula absens = NIHIL.
 * FALSUM = memoria sola. */
interior b32
_contentiones_onerare (
               Piscina* piscina,
    constans character* radix,
       OratioProgramma* programma)
{
    character via[1024];
       chorda fons;
          i32 i = ZEPHYRUM;

    sprintf(via, "%s/oratio/probationes/fixa/contentiones.tsv", radix);
    si (!_plagulam_legere(piscina, via, &fons))
    {
        redde VERUM;
    }
    programma->contentiones = tabula_dispersa_creare_chorda(piscina,
        (i32)256);
    si (programma->contentiones == NIHIL)
    {
        redde FALSUM;
    }
    dum (i < fons.mensura)
    {
        i32 finis = i;
        i32 campi[5];
        i32 n = ZEPHYRUM;
        i32 k;

        dum (finis < fons.mensura && fons.datum[finis] != '\n')
        {
            finis = finis + I;
        }
        si (finis > i && fons.datum[i] != '#')
        {
            campi[n]  = i;
            n         = I;
            per (k = i; k < finis && n < (i32)V; k++)
            {
                si (fons.datum[k] == '\t')
                {
                    campi[n]  = k + I;
                    n         = n + I;
                }
            }
            si (n == (i32)V)
            {
                /* clavis = victor TAB victa (campi 0..1 contigui) */
                chorda clavis = _copia(piscina, _chorda(fons.datum
                    + campi[ZEPHYRUM], campi[(i32)II] - I
                    - campi[ZEPHYRUM]));
                s32 numerus = _numerus_decimalis(_chorda(fons.datum
                    + campi[(i32)II], campi[(i32)III] - I
                    - campi[(i32)II]));
                s32 permille = _numerus_decimalis(_chorda(fons.datum
                    + campi[(i32)IV], finis - campi[(i32)IV]));
                Iudicium* valor = (Iudicium*)piscina_allocare(piscina,
                    (memoriae_index)magnitudo(Iudicium));

                si (valor == NIHIL || clavis.datum == NIHIL)
                {
                    redde FALSUM;
                }
                valor->numerus   = numerus >= ZEPHYRUM ? (i32)numerus
                    : ZEPHYRUM;
                valor->permille  = permille >= ZEPHYRUM ? (i32)permille
                    : ZEPHYRUM;
                (vacuum)tabula_dispersa_inserere(programma->contentiones,
                    clavis, valor);
            }
        }
        i = finis + I;
    }
    redde VERUM;
}

/* IUDEX CONTENTIONUM ex tabula parium (T32 c, 2026-09-09): MENSURATUS ET
 * RECUSATUS (R19). Tabula primo arcus solos numerabat: par 'subiectum
 * vicinum > obiectum vicinum' (1764 contentiones, victa recta 85 %)
 * arcum EUNDEM habet (verbum idem) - contentio LECTIONIS (nom/acc), non
 * arcus; flexa casum -19 Senecae perdidit. Numeris coniunctis (victa
 * sola contra caput victoris) paria discriminantia pauca manent
 * (caput-sequente > obiectum-praecedente 944, obiectum-sequente >
 * obiectum-accusativi 972, caput-incerti > subiectum 852) et iudex
 * limine D: Seneca ligatio +4 arcus recti +43, chartae ligatio -9/-9,
 * casus -4/-2/-1; limine DCCC supportu XXX: arcus recti ubique sursum
 * (+9..+79) sed ligatio chartae -5/-5, Dante -2, casus -3/-3/-2,
 * coactae -2/-6/-4. Iudex lectionum (casus aureus socii) desideratur.
 * ORATIO_IUDEX=1 activat (mensura); ORATIO_IUDEX_LIMEN=permille
 * (ordinarium D), ORATIO_IUDEX_MINIMUM=numerus (ordinarium XX). */
interior b32
_iudex_parametra (
    i32* limen,
    i32* numerus_minimus)
{
    hic_manens i32 lectum     = ZEPHYRUM;
    hic_manens b32 activus    = FALSUM;
    hic_manens i32 limen_h    = (i32)D;
    hic_manens i32 minimus_h  = (i32)XX;

    si (!lectum)
    {
        constans character* a = getenv("ORATIO_IUDEX");
        constans character* l = getenv("ORATIO_IUDEX_LIMEN");
        constans character* s = getenv("ORATIO_IUDEX_MINIMUM");

        activus = (b32)(a != NIHIL && strcmp(a, "1") == ZEPHYRUM);
        si (l != NIHIL && atoi(l) > ZEPHYRUM)
        {
            limen_h = (i32)atoi(l);
        }
        si (s != NIHIL && atoi(s) > ZEPHYRUM)
        {
            minimus_h = (i32)atoi(s);
        }
        lectum = I;
    }
    *limen            = limen_h;
    *numerus_minimus  = minimus_h;
    redde activus;
}

/* T32 c IUDEX CONTENTIONUM: par (stans, nova) in tabula cum supportu et
 * victa recta >= limen -> +I (nova vincit: ordo hodiernus errat); par
 * inversum (nova, stans) ita -> -I (stans manet); aliter 0 (fiducia
 * decidit). Tabula: census contentionum statu ante iudicem (T32 b),
 * plagulis pinnatis; scrinium validatio. */
interior s32
_iudex_contentionis (
    constans OratioProgramma* programma,
                     Piscina* scratch,
                      chorda  stans,
                      chorda  nova)
{
    i32 limen;
    i32 numerus_minimus;
    i32 k;

    si (   !_iudex_parametra(&limen, &numerus_minimus)
        || programma->contentiones == NIHIL
        || stans.mensura == ZEPHYRUM || nova.mensura == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < (i32)II; k++)
    {
           chorda  a = k == ZEPHYRUM ? stans : nova;
           chorda  b = k == ZEPHYRUM ? nova : stans;
        character* datum = (character*)piscina_allocare(scratch,
            (memoriae_index)(a.mensura + b.mensura + I));
        vacuum* valor;

        si (datum == NIHIL)
        {
            redde ZEPHYRUM;
        }
        memcpy(datum, a.datum, (size_t)a.mensura);
        datum[a.mensura] = '\t';
        memcpy(datum + a.mensura + I, b.datum, (size_t)b.mensura);
        si (tabula_dispersa_invenire(programma->contentiones,
                _chorda((i8*)datum, a.mensura + I + b.mensura), &valor))
        {
            constans Iudicium* iud = (constans Iudicium*)valor;

            si (   iud->numerus  >= numerus_minimus
                && iud->permille >= limen)
            {
                redde k == ZEPHYRUM ? (s32)I : (s32)-I;
            }
        }
    }
    redde ZEPHYRUM;
}

/* fiducia regulae titulo: permille aut -I ignota */
interior s32
_fiducia_regulae (
    constans OratioProgramma* programma,
                      chorda  titulus)
{
    vacuum* valor;

    si (   programma->fiducia == NIHIL
        || !tabula_dispersa_invenire(programma->fiducia, titulus,
        &valor))
    {
        redde (s32)-I;
    }
    redde (s32)*(i32*)valor;
}

/* FIDUCIA (T32 a, 2026-09-09): tabula auctores.tsv (praecisio arcuum per
 * regulam plagulis pinnatis) duobus modis adhiberi potest. 'contentio'
 * (ADOPTUM, decisio Frani): petitio fidelior petitionem stantem
 * dependentis revocat, ordo regularum intactus - ligatio chartae
 * 405/428 -> 455/479, Seneca 520 -> 523, scrinium +9..+46, coactae supra
 * pinnas omnes, arcus recti chartis +466 (Seneca -114: tabula chartis
 * dominata regulas laxas Senecae subaestimat - tabula per dialectum
 * FUTURA). 'ordo' (MENSURATUM, RECUSATUM): regulae graduum II+ ordine
 * fiduciae applicantur - ligatio sursum sed casus -10 quinque plagulis,
 * coactae chartarum -14: ordo regularum lectiones quoque decidit et ordo
 * ille mensuratus est (T30 e). 'ambo'; '0' nihil. Modus: 0 nihil, I
 * ordo, II contentio (ordinarium), III ambo. */
interior i32
_fiducia_modus (
    vacuum)
{
    hic_manens i32 lectum  = ZEPHYRUM;
    hic_manens i32 modus   = (i32)II;

    si (!lectum)
    {
        constans character* ambitus = getenv("ORATIO_FIDUCIA");

        si (ambitus != NIHIL)
        {
            modus = strcmp(ambitus, "ordo") == ZEPHYRUM ? I
                : strcmp(ambitus, "contentio") == ZEPHYRUM ? (i32)II
                : strcmp(ambitus, "ambo")
                    == ZEPHYRUM ? (i32)III : ZEPHYRUM;
        }
        lectum = I;
    }
    redde modus;
}

interior b32
_fiducia_activa (
    vacuum)
{
    redde (b32)(_fiducia_modus() & I);
}

/* T32 a: regulas gradus (nodi <regula> expansi) ordine fiduciae non
 * crescente stabiliter reordinare (ignotae in cauda ordine suo);
 * gradus I (priores, regulae adpositionum) intactus - prior classis
 * ante regulas obiecti loquitur (T19g). FALSUM = memoria. */
interior b32
_regulas_fiducia_ordinare (
    constans Cursus* cursus,
            Piscina* scratch,
                Xar* regulae,
                i32  gradus)
{
          i32   n = xar_numerus(regulae);
          s32*  fides;
    StmlNodus** copia;
          i32   i;
          i32   j;

    si (   !_fiducia_activa() || gradus < (i32)II || n < (i32)II
        || cursus->programma->fiducia == NIHIL)
    {
        redde VERUM;
    }
    fides = (s32*)piscina_allocare(scratch, (memoriae_index)n
        * (memoriae_index)magnitudo(s32));
    copia = (StmlNodus**)piscina_allocare(scratch, (memoriae_index)n
        * (memoriae_index)magnitudo(StmlNodus*));
    si (fides == NIHIL || copia == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        StmlNodus* regula   = *(StmlNodus**)xar_obtinere(regulae, i);
           chorda* titulus  = stml_attributum_capere(regula, "titulus");

        copia[i]  = regula;
        fides[i]  = titulus != NIHIL
            ? _fiducia_regulae(cursus->programma, *titulus) : (s32)-I;
    }
    per (i = I; i < n; i++)
    {
         StmlNodus* r = copia[i];
               s32  f = fides[i];

        j = i;
        dum (j > ZEPHYRUM && fides[j - I] < f)
        {
            copia[j]  = copia[j - I];
            fides[j]  = fides[j - I];
            j         = j - I;
        }
        copia[j] = r;
        fides[j] = f;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        *(StmlNodus**)xar_obtinere(regulae, i) = copia[i];
    }
    redde VERUM;
}

OratioProgramma*
oratio_resolutio_programma_onerare (
                     Piscina* piscina,
         InternamentumChorda* intern,
          constans character* radix,
    OratioVocabulariumVitium* vitium)
{
    character via[1024];
       chorda fons;

    si (radix == NIHIL)
    {
        radix = ".";
    }
    sprintf(via, "%s/oratio/partes/resolutio.stml", radix);
    si (!_plagulam_legere(piscina, via, &fons))
    {
        memset(vitium, ZEPHYRUM, magnitudo(*vitium));
        vitium->plagula  = "oratio/partes/resolutio.stml";
        vitium->causa    = "plagula absens";
        redde NIHIL;
    }
    {
        OratioProgramma* p = oratio_resolutio_programma_legere(piscina,
            intern, fons, vitium);

        si (   p != NIHIL
            && (   !_fiduciam_onerare(piscina, radix, p)
                || !_contentiones_onerare(piscina, radix, p)
                || !_pondera_onerare(piscina, radix, p, vitium)))
        {
            redde NIHIL;
        }
        redde p;
    }
}

vacuum
oratio_resolutio_census_vacare (
    OratioResolutioCensus* census)
{
    memset(census, ZEPHYRUM, magnitudo(*census));
}

i32
oratio_resolutio_lingua_censu_index (
    constans i32* linguae)
{
    i32 k;
    i32 optima = ZEPHYRUM;

    per (k = I; k < (i32)ORATIO_LINGUA_NUMERUS; k++)
    {
        si (linguae[k] > linguae[optima])
        {
            optima = k;
        }
    }
    redde optima;
}

constans character*
oratio_resolutio_lingua_censu (
    constans i32* linguae)
{
    redde ORATIO_TITULI_LINGUARUM[
        oratio_resolutio_lingua_censu_index(linguae)];
}

/* regulae numerum in censu augere (titulus copiatur in piscina) */
interior b32
_regulam_numerare (
    Cursus* cursus,
    chorda  titulus)
{
          OratioResolutioCensus* census = cursus->census;
                            i32  k;
    OratioResolutioRegulaCensus* r;

    si (census == NIHIL)
    {
        redde VERUM;
    }
    si (census->per_regulam == NIHIL)
    {
        census->per_regulam = xar_creare(cursus->piscina,
            (i32)magnitudo(OratioResolutioRegulaCensus));
        si (census->per_regulam == NIHIL)
        {
            redde FALSUM;
        }
    }
    per (k = ZEPHYRUM; k < xar_numerus(census->per_regulam); k++)
    {
        r =
            (OratioResolutioRegulaCensus*)xar_obtinere(
                census->per_regulam, k);
        si (   r->titulus.mensura == titulus.mensura
            && memcmp(r->titulus.datum, titulus.datum,
                (size_t)titulus.mensura) == ZEPHYRUM)
        {
            r->numerus = r->numerus + I;
            redde VERUM;
        }
    }
    r = (OratioResolutioRegulaCensus*)xar_addere(census->per_regulam);
    si (r == NIHIL)
    {
        redde FALSUM;
    }
    r->titulus = _copia(cursus->piscina, titulus);
    r->numerus = I;
    redde VERUM;
}

interior b32
_numerus_attributi (
             StmlNodus* nodus,
    constans character* titulus,
                   i32* exitus)
{
    chorda* v = stml_attributum_capere(nodus, titulus);

    si (v == NIHIL)
    {
        redde FALSUM;
    }
    redde materia_arbor_numerus_ex_chorda(v, exitus);
}

/* LEX I CLAUSULAE (T20c, 2026-09-08): ligatio trans clausulas recusatur.
 * T20b numeravit: ligationes remotae limitem clausulae DXLVII / MXVI /
 * CMLVI transeunt et regulae capitis remotae XXXIII-LXX % rectae sunt
 * (T19g bis) - capsa testimonium contra ligationem est. Decisio
 * XXXVII: eliminatio sana solum super cellulis DECISIS - membrum
 * decisum = positum strato puritatis > XC % (semen, extentum, unica),
 * clausura/verbum aperta habentur. Variationes mensuratae (primarium
  * + coactae thesauris V): 0 nulla; I capita sola, strata pura; II
 * relationes omnes, strata pura; III capita sola, strata quaevis; IV
 * ut I sed ligationes REMOTAE solae (distantia >= II: vicinae trans
  * limitem semina ipsa faciunt); V ut I sed neutra clausula COORDINATA
  * (scissio verbi: limes incertissimus); VI CUSTODIA sine capsa: semen
 * certum inter v et w interiectum recusat, stampa neglecta.
 * MENSURATUM (primarium / coactae permille; basis Seneca 799/765,
 * chartae 838/710 + 831/711; EWT numquam movetur): I 798/769 838/711
 * 832/716; II 798/769 839/711 832/716; III 795/775 839/718 833/727;
 * IV ut I; V 798/765 838/711 832/714; VI 797/772 841/726 834/731;
 * forma DURA in exemplari (clausula="$c" in regulis laxis XVI)
 * 797/782 841/723 834/729; forma MOLLIS (exemplaria intra clausulam
 * gradu III, laxa gradu IV) 799/764 837/707 830/707. Ligationes
 * recusatae lectionem rectam LXXI % eligebant: cellula INCERTA.
 * Omnis variatio primarium Senecae deprimit (X-XLVI vocabula): regula
 * 'nullus thesaurus cadat' omnes recusat; custodia VI capsam paene
 * aequat - lex haec semen legit, non capsam (informatio propria
 * capsae = clausura, stratum debilissimum). Interruptor ZEPHYRUM cum
 * numeris manet; lex clausulae quaevis custodiam VI vincere debet. */
/* T28 (2026-09-09, decisio Frani): lex I re-iudicata columna LIGATIONIS
 * (T26), quae T20c deerat. Gradus II (relationes omnes, strata pura)
 * regulis strictis unitis: ligatio 441/386/394 -> 462/398/412, EWT dev
 * 815 -> 817, scrinium +20/+44/+41/+16; coactae 765/711/712 ->
 * 769/713/717; casus 660/654/671 -> 666/653/671 (chartae dev -1 CAUSA
 * NOMINATA, VII verba; Aquinas -2 relatum); primaria immota; revocatio
 * aequalis - arcus remoti falsi sublati, non recti. Gradus I: ligatio
 * 458/391/405. Census auri (caput_census.py): socius concordans
 * proximus caput est 96-99 % ubi concordantia vera; verbum interiectum
 * in versu 30 % arcuum - capsa in FORMA (clausula="$c") versum
 * laederet, recusatio in exsecutore (strata pura) non laedit. */
hic_manens constans i32 RECUSATIO_TRANS_CLAUSULAS = (i32)II;

interior b32
_obiectum_carrier_primus (
    vacuum);

/* T31 b LEX SOCII OBIECTI: recusationes socii (w, b) obiecti verbi -
 * (I) classis: lectio b classem lectionis primae socii mutans (casus
 * solus classem non decidit); (II) adpositionis: socius iam obiectum
 * adpositionis (umbra obiecti carrier adpositionis in w impleta);
 * (III) subiecti: socius subiectum eiusdem verbi (umbra subiecti
 * carrier v in w impleta). Ambitus ORATIO_OBIECTUM_SOCIUS=0 omnes
 * abrogat, =classis|adpositionis|subiecti unam solam (mensura). */
interior b32
_obiectum_socius_recusandus (
    constans MateriaValor* elementa,
                      i32  ne,
                      i32  v,
                      i32  a,
                      i32  w,
                      i32  b);


interior b32
_stratum_purum (
    s32 causa)
{
    redde (b32)(   causa == (s32)ORATIO_CLAUSULA_CAUSA_SEMEN
                || causa == (s32)ORATIO_CLAUSULA_CAUSA_EXTENTUM
                || causa == (s32)ORATIO_CLAUSULA_CAUSA_UNICA);
}

/* clausula et causa elementi k (loci per genus); -I non scripta */
interior vacuum
_clausula_elementi (
    MateriaValor  elementa,
             i32  k,
             s32* clausula,
             s32* causa)
{
    constans MateriaNodus* e = materia_valor_lista_obtinere(elementa,
        k)->datum.nodus;
                      s32 locus = oratio_locus_clausulae(
                          (OratioGenus)e->genus, FALSUM);
                      s32 locus_causae = oratio_locus_clausulae(
                          (OratioGenus)e->genus, VERUM);

    *clausula  = (s32)-I;
    *causa     = (s32)-I;
    si (   locus                >= ZEPHYRUM
        && e->loci[locus].genus == MATERIA_VALOR_INDEX)
    {
        *clausula = e->loci[locus].datum.index;
    }
    si (   locus_causae                >= ZEPHYRUM
        && e->loci[locus_causae].genus == MATERIA_VALOR_INDEX)
    {
        *causa = e->loci[locus_causae].datum.index;
    }
}

/* nodus clausulae c sententiae; NIHIL si non scripta */
interior constans MateriaNodus*
_nodus_clausulae (
    constans MateriaNodus* sententia,
                      s32  c)
{
    constans MateriaValor* clausulae =
        &sententia->loci[ORATIO_SENTENTIA_CLAUSULAE];
    constans MateriaValor* v;

    si (   c < ZEPHYRUM || clausulae->genus != MATERIA_VALOR_LISTA
        || (i32)c >= materia_valor_lista_numerus(*clausulae))
    {
        redde NIHIL;
    }
    v = materia_valor_lista_obtinere(*clausulae, (i32)c);
    si (v == NIHIL || v->genus != MATERIA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde v->datum.nodus;
}

/* species clausulae c; -I si non scripta */
interior s32
_species_clausulae (
    constans MateriaNodus* sententia,
                      s32  c)
{
    constans MateriaNodus* n = _nodus_clausulae(sententia, c);

    si (   n == NIHIL || n->loci[ORATIO_CLAUSULA_SPECIES].genus
        != MATERIA_VALOR_INDEX)
    {
        redde (s32)-I;
    }
    redde n->loci[ORATIO_CLAUSULA_SPECIES].datum.index;
}

/* an elementum k semen clausulae relativae c sit */
interior b32
_semen_relativum (
    constans MateriaNodus* sententia,
                      s32  c,
                      i32  k)
{
    constans MateriaNodus* n = _nodus_clausulae(sententia, c);

    redde (b32)(   n != NIHIL
                && _species_clausulae(sententia, c)
                    == (s32)ORATIO_SPECIES_CLAUSULAE_RELATIVA
                && n->loci[ORATIO_CLAUSULA_SEMEN].genus
                    == MATERIA_VALOR_INDEX
                && n->loci[ORATIO_CLAUSULA_SEMEN].datum.index
                    == (s32)k);
}

/* an semen (stampa 'semen') inter v et w stricte interiaceat */
interior b32
_semen_interiectum (
    MateriaValor elementa,
             i32 v,
             i32 w)
{
    i32 a = v < w ? v : w;
    i32 b = v < w ? w : v;
    i32 k;
    s32 clausula;
    s32 causa;

    per (k = a + I; k < b; k = k + I)
    {
        _clausula_elementi(elementa, k, &clausula, &causa);
        si (causa == (s32)ORATIO_CLAUSULA_CAUSA_SEMEN)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ordo impletionis v -> w trans clausulas recusandus? */
interior b32
_trans_clausulas (
    constans MateriaNodus* sententia,
             MateriaValor  elementa,
                      i32  v,
                      i32  w,
                      s32  relatio)
{
    s32 cv;
    s32 causa_v;
    s32 cw;
    s32 causa_w;

    si (RECUSATIO_TRANS_CLAUSULAS == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (   RECUSATIO_TRANS_CLAUSULAS != (i32)II
        && relatio                   != (s32)ORATIO_RELATIO_CAPUT)
    {
        redde FALSUM;
    }
        si (RECUSATIO_TRANS_CLAUSULAS == (i32)VI)
        {
        redde _semen_interiectum(elementa, v, w);
        }
    _clausula_elementi(elementa, v, &cv, &causa_v);
    _clausula_elementi(elementa, w, &cw, &causa_w);
    si (cv < ZEPHYRUM || cw < ZEPHYRUM || cv == cw)
    {
        redde FALSUM;
    }
    si (   RECUSATIO_TRANS_CLAUSULAS == (i32)V
        && (   _species_clausulae(sententia, cv)
                == (s32)ORATIO_SPECIES_CLAUSULAE_COORDINATA
            || _species_clausulae(sententia, cw)
                == (s32)ORATIO_SPECIES_CLAUSULAE_COORDINATA))
    {
        redde FALSUM;   /* limes scissionis: incertissimus */
    }
    si (   RECUSATIO_TRANS_CLAUSULAS != (i32)III
        && (!_stratum_purum(causa_v) || !_stratum_purum(causa_w)))
    {
        redde FALSUM;   /* cellula aperta: nihil eliminatur */
    }
    si (   _semen_relativum(sententia, cv, v)
        || _semen_relativum(sententia, cw, w))
    {
                redde FALSUM;   /* relativum ad antecedens: limes iure */
    }
    si (   RECUSATIO_TRANS_CLAUSULAS == (i32)IV
        && (v > w ? v - w : w - v) < (i32)II)
    {
        redde FALSUM;   /* vicina: limes seminis ipsius */
    }
    redde VERUM;
}

/* IMPLETIO (T19d): umbra ligandum post permutationes - umbra nodus
 * (stabilis per permutationem listae), vocabulum implens w, analysis
 * implens b (index ANTE permutationem; remittitur) */
/* gradus casuum ordine ORATIO_CASUS: NOM GEN DAT ACC ABL LOC VOC (prior
 * casuum T24: abl > acc > gen > nom > dat > voc > loc; politica parium
 * T25 eodem utitur) */
hic_manens constans s32 ORDO_CASUUM[ORATIO_CASUS_NUMERUS] = {
    (s32)III, (s32)II, (s32)IV, I, ZEPHYRUM, (s32)VI, (s32)V
};

/* gradus casuum POLITICAE PARIUM (T27 b): inter paria concordantia aeque
 * vicina (quaesitio omnes) casus lectionis implentis - tabula propria,
 * mensurata seorsum a priore T24 et hodie eadem: nominativo primo
 * (nom acc abl gen ... aut nom abl acc gen ...) casus 662/644/665 ->
 * 656/634/646 et scrinium -14/-1/-1/-5 (thesauri IX); sine gradu
 * (politica I) 655/629/655. Ablativus primus inter paria vicina
 * concordantia vincit - 'Puella bona ambulat' ablativa legitur, pretium
 * notatum. */
hic_manens constans s32 ORDO_CASUUM_PARIUM[ORATIO_CASUS_NUMERUS] = {
    (s32)III, (s32)II, (s32)IV, I, ZEPHYRUM, (s32)VI, (s32)V
};


/* POLITICA PARIUM (T25, 2026-09-08): ordines impletionis regulae unius
 * ordine exemplaris veniunt - regulae 'praecedente' cursu fratrum ad
 * vocabulum PRIMUM congruens ligant, non proximum (T19g bis), et inter
 * lectiones congruentes ordo listae (dictionarii) vincit: prior tacitus
 * intra regulas testimonii. Politica ordines cuiusque regulae intra
 * greges (carrier v, umbra u) stabiliter reordinat: I distantia |v - w|
 * minima prima; II deinde gradus casuum lectionis implentis (ORDO_CASUUM);
 * ordo originis ultimus. Greges ordine apparitionis manent. 0 = nulla. */
hic_manens constans i32 POLITICA_PARIUM = (i32)II;

nomen structura {
    MateriaNodus* umbra;
             i32  w;
             i32  b;
    /* T33: nodus analysis (w, b) ANTE permutationes captus - referentia
     * eum nominat, ordo novus eum non movet */
    MateriaNodus* analysis;
    /* T32 a: carrier v et titulus regulae (auctor impletionis) */
             i32 v;
          chorda titulus;
} Impletio;

/* decisio vocabuli (T19g, decretum SUDOKU decisio XL): genus
 * (OratioDecisio, -I = nemo decidit) + auctor (titulus regulae in
 * piscina cursus copiatus; lex umbrarum auctorem suum nominat);
 * post gradus omnes in arborem scripta (_decisiones_scribere) */
nomen structura {
       s32 genus;
    chorda auctor;
} Decisio;

/* decisionem vocabuli notare: prima manet (vocantes sub lege
 * 'prima vincit' vocant), lex umbrarum superscribit - actus
 * decisivus ultimus est */
interior vacuum
_decisionem_notare (
           Cursus* cursus,
          Decisio* decisio,
    OratioDecisio  genus,
  constans chorda* titulus)
{
    hic_manens character lex[] = "lex-umbrarum";

    decisio->genus = (s32)genus;
    si (genus == ORATIO_DECISIO_UMBRA)
    {
        decisio->auctor = _chorda((i8*)lex,
            (i32)(magnitudo(lex) - I));
    }
    alioquin si (titulus != NIHIL && titulus->mensura > ZEPHYRUM)
    {
        decisio->auctor = _copia(cursus->piscina, *titulus);
    }
    alioquin
    {
        decisio->auctor.datum    = NIHIL;
        decisio->auctor.mensura  = ZEPHYRUM;
    }
}

/* decisiones in arborem scribere (T19g): vocabulum decisum locum
 * 'decisio' (INDEX OratioDecisio) et 'auctor' (lexema derivatum:
 * titulus regulae, origo = pars prima) accipit - profilum cellulae
 * (coacta | ordinata | aperta) ex arbore legitur, non ex plano
 * abiecto; non decisum nihil scribit (octeti crudi immoti).
 * Scriptum iam (cursus alter) reponitur. Census decisae[].
 * FALSUM = memoria sola. */
interior b32
_decisiones_scribere (
              Cursus* cursus,
        MateriaNodus* sententia,
    constans Decisio* decisiones)
{
    constans MateriaValor* elementa =
        &sententia->loci[ORATIO_SENTENTIA_ELEMENTA];
                      i32 ne = materia_valor_lista_numerus(*elementa);
                      i32 k;

    per (k = ZEPHYRUM; k < ne; k++)
    {
        constans MateriaValor* elementum =
            materia_valor_lista_obtinere(*elementa, k);
                 MateriaNodus* vocabulum;
        constans MateriaValor* partes;
        constans MateriaToken* origo;
                          b32  scriptum;

        si (   decisiones[k].genus < ZEPHYRUM
            || elementum        == NIHIL
            || elementum->genus != MATERIA_VALOR_NODUS
            || elementum->datum.nodus->genus
                != (s32)ORATIO_GENUS_VOCABULUM)
        {
            perge;
        }
        vocabulum  = elementum->datum.nodus;
        partes     = &vocabulum->loci[ORATIO_VOCABULUM_PARTES];
        si (   partes->genus
            != MATERIA_VALOR_LISTA
            || materia_valor_lista_numerus(*partes) == ZEPHYRUM)
        {
            perge;
        }
        origo    = materia_valor_lista_obtinere(*partes,
            ZEPHYRUM)->datum.token;
        scriptum = vocabulum->loci[ORATIO_VOCABULUM_DECISIO].genus
            != MATERIA_VALOR_NIHIL;
        si (!(scriptum
                ? materia_nodus_reponere(vocabulum,
                    (i32)ORATIO_VOCABULUM_DECISIO,
                    materia_valor_index(decisiones[k].genus),
                    MATERIA_LOCUS_INDEX)
                : materia_nodus_ponere(vocabulum,
                    (i32)ORATIO_VOCABULUM_DECISIO,
                    materia_valor_index(decisiones[k].genus),
                    MATERIA_LOCUS_INDEX)))
        {
            redde FALSUM;
        }
        si (decisiones[k].auctor.mensura > ZEPHYRUM)
        {
            MateriaToken* t = oratio_lexema_derivatum(cursus->piscina,
                (s32)ORATIO_LEX_DERIVATUM, decisiones[k].auctor, origo);

            scriptum = vocabulum->loci[ORATIO_VOCABULUM_AUCTOR].genus
                != MATERIA_VALOR_NIHIL;
            si (   t == NIHIL
                || !(scriptum
                    ? materia_nodus_reponere(vocabulum,
                        (i32)ORATIO_VOCABULUM_AUCTOR,
                        materia_valor_token(t), MATERIA_LOCUS_TOKEN)
                    : materia_nodus_ponere(vocabulum,
                        (i32)ORATIO_VOCABULUM_AUCTOR,
                        materia_valor_token(t), MATERIA_LOCUS_TOKEN)))
            {
                redde FALSUM;
            }
        }
        si (cursus->census != NIHIL)
        {
            cursus->census->decisae[decisiones[k].genus] =
                cursus->census->decisae[decisiones[k].genus] + I;
        }
    }
    redde VERUM;
}

/* analysis 'a' vocabuli 'v' intra elementa adest? */
interior b32
_analysis_adest (
    MateriaValor elementa,
             i32 v,
             i32 a)
{
    constans MateriaNodus* vocabulum =
        materia_valor_lista_obtinere(elementa, v)->datum.nodus;
    constans MateriaValor* analyses;

    si (vocabulum->genus != (s32)ORATIO_GENUS_VOCABULUM)
    {
        redde FALSUM;
    }
    analyses = &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
    redde analyses->genus == MATERIA_VALOR_LISTA
        && a < materia_valor_lista_numerus(*analyses);
}

/* accidens INDEX lectionis per titulum loci classis suae; -I si absens */
interior s32
_accidens_lectionis (
    constans MateriaNodus* analysis,
       constans character* titulus)
{
    OratioClassis classis =
        oratio_genus_classis((OratioGenus)analysis->genus);
              s32 locus;

    si (classis >= ORATIO_CLASSIS_NUMERUS_CLASSIUM)
    {
        redde (s32)-I;
    }
    locus = oratio_partes_locus(classis, titulus);
    si (   locus < ZEPHYRUM
        || analysis->loci[locus].genus != MATERIA_VALOR_INDEX)
    {
        redde (s32)-I;
    }
    redde analysis->loci[locus].datum.index;
}

/* lingua lectionis (INDEX communis); -I si absens */
interior s32
_lingua_lectionis (
    constans MateriaNodus* analysis)
{
    constans MateriaValor* l = &analysis->loci[ORATIO_ANALYSIS_LINGUA];

    redde l->genus == MATERIA_VALOR_INDEX ? l->datum.index : (s32)-I;
}

/* lemmata duarum lectionum aequalia (lexemata derivata scripta ambo) */
interior b32
_lemmata_aequalia (
    constans MateriaNodus* x,
    constans MateriaNodus* y)
{
    constans MateriaValor* lx = &x->loci[ORATIO_ANALYSIS_LEMMA];
    constans MateriaValor* ly = &y->loci[ORATIO_ANALYSIS_LEMMA];

    redde lx->genus == MATERIA_VALOR_TOKEN
        && ly->genus == MATERIA_VALOR_TOKEN
        && lx->datum.token != NIHIL && ly->datum.token != NIHIL
        && chorda_aequalis(lx->datum.token->valor,
            ly->datum.token->valor);
}

/* LEX CLASSIS EXPLICITAE (T19i, 2026-09-08): vocabulum regula
 * praelationis decisum CLASSEM tenet, casum non - ordo impletionis
 * lectionem 'a' nominans accipitur si classis eius == classis lectionis
 * decisae 'e' (casus per testimonium inter lectiones classis
 * decisae), aliter si GEMINA exstat: lectio classis decisae eodem
 * lemmate iisdem casu numero genere (copia determinantis lectionis
  * adiectivae: prior 'determinantia' classem dixit, regula capitis
 * adiectivam ligavit) - pro 'a' substituitur. Utrimque LINGUA eadem
 * et LEMMA idem (lectio Latina vocabuli Anglici 'die' substantivum
 * Anglicum non emendat - regula linguae stat, mensuratum EWT -0.3).
 * -I = neutrum: classis manet (prior classis ligationem fortuitam
 * classis alterius vincit, lex 2026-09-07). */
interior s32
_lectio_classis_explicitae (
    MateriaValor elementa,
             i32 v,
             i32 a,
             i32 e)
{
    constans MateriaNodus* vocabulum =
        materia_valor_lista_obtinere(elementa, v)->datum.nodus;
    constans MateriaValor* analyses =
        &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
    constans MateriaNodus* lectio_a;
    constans MateriaNodus* lectio_e;
            OratioClassis  classis_e;
    constans MateriaValor* lemma_a;
                      s32  casus_a;
                      s32  numerus_a;
                      s32  genus_a;
                      i32  n;
                      i32  r;

    si (   analyses->genus != MATERIA_VALOR_LISTA
        || a               >= materia_valor_lista_numerus(*analyses)
        || e               >= materia_valor_lista_numerus(*analyses))
    {
        redde (s32)-I;
    }
    lectio_a = materia_valor_lista_obtinere(*analyses,
        a)->datum.nodus;
    lectio_e = materia_valor_lista_obtinere(*analyses,
        e)->datum.nodus;
        classis_e  = oratio_genus_classis((OratioGenus)lectio_e->genus);
    si (_lingua_lectionis(lectio_a) != _lingua_lectionis(lectio_e))
    {
        redde (s32)-I;
    }
    si (   oratio_genus_classis((OratioGenus)lectio_a->genus)
        == classis_e
        && _lemmata_aequalia(lectio_a, lectio_e))
    {
        redde (s32)a;
    }

    lemma_a    = &lectio_a->loci[ORATIO_ANALYSIS_LEMMA];
    casus_a    = _accidens_lectionis(lectio_a, "casus");
    numerus_a  = _accidens_lectionis(lectio_a, "numerus");
    genus_a    = _accidens_lectionis(lectio_a, "genus");
    n          = materia_valor_lista_numerus(*analyses);
    per (r = ZEPHYRUM; r < n; r++)
    {
        constans MateriaNodus* lectio =
            materia_valor_lista_obtinere(*analyses, r)->datum.nodus;
        constans MateriaValor* lemma =
            &lectio->loci[ORATIO_ANALYSIS_LEMMA];

                si (   oratio_genus_classis((OratioGenus)lectio->genus)
                    != classis_e
                    || _lingua_lectionis(lectio)
                        != _lingua_lectionis(lectio_a)
                    || lemma->genus != MATERIA_VALOR_TOKEN
                    || lemma_a->genus != MATERIA_VALOR_TOKEN
                    || lemma->datum.token == NIHIL
                    || lemma_a->datum.token == NIHIL
                    || !chorda_aequalis(lemma->datum.token->valor,
                    lemma_a->datum.token->valor)
                    || _accidens_lectionis(lectio, "casus") != casus_a

                    || _accidens_lectionis(lectio, "numerus")
                        != numerus_a
                    || _accidens_lectionis(lectio, "genus") != genus_a)
                {
            perge;
                }
        redde (s32)r;
    }
    redde (s32)-I;
}

/* numerus lectionum vocabuli v (0 si non vocabulum) */
interior i32
_numerus_lectionum (
    MateriaValor elementa,
             i32 v)
{
    constans MateriaNodus* vocabulum =
        materia_valor_lista_obtinere(elementa, v)->datum.nodus;
    constans MateriaValor* analyses;

    si (vocabulum->genus != (s32)ORATIO_GENUS_VOCABULUM)
    {
        redde ZEPHYRUM;
    }
    analyses = &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
    redde analyses->genus == MATERIA_VALOR_LISTA
        ? materia_valor_lista_numerus(*analyses) : ZEPHYRUM;
}

/* umbra 'u' analysis 'a' vocabuli 'v' (locus 'umbrae' per titulum
 * classis analysis); NIHIL si absens */
interior MateriaNodus*
_umbram_invenire (
    MateriaValor elementa,
             i32 v,
             i32 a,
             i32 u)
{
    constans MateriaNodus* vocabulum;
    constans MateriaValor* analyses;
    constans MateriaValor* analysis_valor;
             MateriaNodus* analysis;
                      s32  locus;
    constans MateriaValor* umbrae;
    constans MateriaValor* umbra_valor;

    si (!_analysis_adest(elementa, v, a))
    {
        redde NIHIL;
    }
    vocabulum = materia_valor_lista_obtinere(elementa,
        v)->datum.nodus;
    analyses        = &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
    analysis_valor  = materia_valor_lista_obtinere(*analyses, a);
    si (   analysis_valor        == NIHIL
        || analysis_valor->genus != MATERIA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    analysis = analysis_valor->datum.nodus;
    locus    = oratio_partes_locus(oratio_genus_classis(
        (OratioGenus)analysis->genus), "umbrae");
    si (locus < ZEPHYRUM || (i32)locus >= analysis->numerus_locorum)
    {
        redde NIHIL;
    }
    umbrae = &analysis->loci[locus];
    si (   umbrae->genus != MATERIA_VALOR_LISTA
        || u             >= materia_valor_lista_numerus(*umbrae))
    {
        redde NIHIL;
    }
    umbra_valor = materia_valor_lista_obtinere(*umbrae, u);
    si (   umbra_valor        == NIHIL
        || umbra_valor->genus != MATERIA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde umbra_valor->datum.nodus;
}

/* sententiam unam resolvere GRADU uno: proiectio + regulae gradus ->
 * expansio -> consilia -> permutationes -> ligationes. vindicata =
 * vocabula gradibus prioribus vindicata (prima vincit TRANS gradus:
 * ordines in ea repetitae); post gradum vocabula praelata vindicantur.
 * FALSUM = memoria sola. */
/* clavis ordinis impletionis (T25): grex = index apparitionis primae
 * paris (v, u), distantia, gradus casuum lectionis (w, b) */
nomen structura {
    i32 index;
    i32 grex;
    i32 distantia;
    s32 gradus;
} ClavisOrdinis;

/* ordines impletionis regulae intra greges (v, u) stabiliter reordinare
 * secundum POLITICA_PARIUM; FALSUM = memoria */
interior b32
_ordines_ordinare (
         Piscina* scratch,
    MateriaValor  elementa,
             Xar* ordines,
             i32  ne)
{
    i32            n = xar_numerus(ordines);
    ClavisOrdinis* claves;
    StmlNodus**    copia;
    i32*           grex_v;      /* per (v, u) -> index gregis: tabula v * XVI + u */
    i32            greges = ZEPHYRUM;
    i32            i;
    i32            j;

    si (n < (i32)II)
    {
        redde VERUM;
    }
    claves = (ClavisOrdinis*)piscina_allocare(scratch, (memoriae_index)n
        * (memoriae_index)magnitudo(ClavisOrdinis));
    copia  = (StmlNodus**)piscina_allocare(scratch, (memoriae_index)n
        * (memoriae_index)magnitudo(StmlNodus*));
    grex_v = (i32*)piscina_allocare(scratch,
        (memoriae_index)(ne * (i32)XVI)
        * (memoriae_index)magnitudo(i32));
    si (claves == NIHIL || copia == NIHIL || grex_v == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < ne * (i32)XVI; i++)
    {
        grex_v[i] = (i32)-I;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        StmlNodus* ordo = *(StmlNodus**)xar_obtinere(ordines, i);
              i32  v;
              i32  u;
              i32  w;
              i32  b;

        copia[i]             = ordo;
        claves[i].index      = i;
        claves[i].grex       = (i32)-I;
        claves[i].distantia  = ZEPHYRUM;
        claves[i].gradus     = (s32)ORATIO_CASUS_NUMERUS;
        si (   !_numerus_attributi(ordo, "vocabulum", &v)
            || !_numerus_attributi(ordo, "umbra", &u)
            || !_numerus_attributi(ordo, "ad-vocabulum", &w)
            || !_numerus_attributi(ordo, "ad-analysis", &b)
            || v >= ne || w >= ne || u >= (i32)XVI)
        {
            perge;   /* ordo malus: sede sua manet (recusatur postea) */
        }
        si (grex_v[v * (i32)XVI + u] == (i32)-I)
        {
            grex_v[v * (i32)XVI + u]  = greges;
            greges                    = greges + I;
        }
        claves[i].grex       = grex_v[v * (i32)XVI + u];
        claves[i].distantia  = v > w ? v - w : w - v;
        si (   POLITICA_PARIUM >= (i32)II
            && _analysis_adest(elementa, w, b))
        {
            constans MateriaNodus* lectio =
                materia_valor_lista_obtinere(
                materia_valor_lista_obtinere(elementa, w)->datum.nodus
                    ->loci[ORATIO_VOCABULUM_ANALYSES], b)->datum.nodus;
            s32 casus_lectionis = _accidens_lectionis(lectio, "casus");

            si (   casus_lectionis >= ZEPHYRUM
                && casus_lectionis < (s32)ORATIO_CASUS_NUMERUS)
            {
                                claves[i].gradus =
                                    ORDO_CASUUM_PARIUM[casus_lectionis];
            }
        }
    }
    /* insertio stabilis: (grex, distantia, gradus, index); ordines sine
     * grege (mali) sedem originis tenent per indicem solum */
    per (i = I; i < n; i++)
    {
        ClavisOrdinis x = claves[i];

        j = i;
        dum (j > ZEPHYRUM)
        {
            ClavisOrdinis y = claves[j - I];
                      b32 ante;

            si (x.grex == (i32)-I || y.grex == (i32)-I)
            {
                ante = FALSUM;
            }
            alioquin si (x.grex != y.grex)
            {
                ante = (b32)(x.grex < y.grex);
            }
            alioquin si (x.distantia != y.distantia)
            {
                ante = (b32)(x.distantia < y.distantia);
            }
            alioquin
            {
                ante = (b32)(x.gradus < y.gradus);
            }
            si (!ante)
            {
                frange;
            }
            claves[j]  = y;
            j          = j - I;
        }
        claves[j] = x;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        *(StmlNodus**)xar_obtinere(ordines, i) = copia[claves[i].index];
    }
    redde VERUM;
}

/* EMENDATIO SEMEL PER SENTENTIAM (T27 b, 2026-09-08/09): VERUM = casus
 * vocabuli classis explicitae per testimonium (T19i) semel per
 * SENTENTIAM emendatur, FALSUM = semel per gradum (T19i, lex hodierna):
 * regula laxa gradus III vocabulum gradu II emendatum iterum emendat
 * (prima vincit trans gradus hic non valet). MENSURATUM thesauris IX
 * (regulis strictis unitis): VERUM casus 662/644/665 -> 660/654/671,
 * ligatio 437/368/378 -> 441/386/394, scrinium casus +7/+4/+6/+6 et
 * ligatio +10..+12 - Seneca casus -2 (IX verba) solus descendit.
 * ADOPTUM 2026-09-09 (decisio Frani: pinna casus Senecae 662 -> 660
 * causa nominata - lex 'prima vincit trans gradus' emendationi quoque).
 * Ligationes in lectionem alienam spectantes (Hilarius discordes II)
 * olim per _ligationes_remittere sanatae; T33 (2026-09-10):
 * referentiae permutationem ipsae supervivunt, remissio sublata. */
hic_manens constans b32 EMENDATIO_SEMEL_PER_SENTENTIAM = VERUM;

interior constans MateriaValor*
_umbrae_lectionis (
    constans MateriaNodus* analysis);


/* T32 a (2026-09-09): AUCTOR impletionis in umbram scribere - lexema
 * derivatum ex titulo regulae (origo = pars prima vocabuli carrier v,
 * ut auctor decisionis vocabuli); titulus vacuus = nihil. Umbra iam
 * auctorem ferens reponitur (lex capitis 'lex-capitis' scribit).
 * Oraculum hinc praecisionem arcuum per auctorem computat - tabula
 * fiduciae ex qua ordo petitionum (gradus decisus). FALSUM = memoria. */
interior b32
_auctorem_umbrae_scribere (
          Cursus* cursus,
    MateriaValor  elementa,
    MateriaNodus* umbra,
             i32  v,
          chorda  titulus)
{
    constans MateriaValor* partes;
             MateriaToken* t;
                      b32  scriptum;

    si (   titulus.mensura == ZEPHYRUM
        || v               >= materia_valor_lista_numerus(elementa))
    {
        redde VERUM;
    }
    partes = &materia_valor_lista_obtinere(elementa, v)->datum.nodus
        ->loci[ORATIO_VOCABULUM_PARTES];
    si (   partes->genus                        != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*partes) == ZEPHYRUM)
    {
        redde VERUM;
    }
    t = oratio_lexema_derivatum(cursus->piscina,
        (s32)ORATIO_LEX_DERIVATUM, titulus,
        materia_valor_lista_obtinere(*partes, ZEPHYRUM)->datum.token);
    si (t == NIHIL)
    {
        redde FALSUM;
    }
    scriptum = umbra->loci[ORATIO_UMBRA_AUCTOR].genus
        != MATERIA_VALOR_NIHIL;
    redde scriptum
        ? materia_nodus_reponere(umbra, (i32)ORATIO_UMBRA_AUCTOR,
            materia_valor_token(t), MATERIA_LOCUS_TOKEN)
        : materia_nodus_ponere(umbra, (i32)ORATIO_UMBRA_AUCTOR,
            materia_valor_token(t), MATERIA_LOCUS_TOKEN);
}

/* T32 a: PETITIO dependentis - umbra quae eum petit et fiducia eius */
nomen structura {
    MateriaNodus* umbra;
             s32  fides;
             s32  caput;   /* T32 f: index vocabuli capitis petiti; -I */
} Petitio;

/* petitionem stantem revocare: cella impletionis huius gradus vacuatur
 * (umbra NIHIL: scriptio eam praeterit), aut umbra gradus prioris iam
 * scripta impletionem -I accipit (vacua oraculo et legibus). FALSUM =
 * memoria. */
interior b32
_petitionem_revocare (
             Xar* impletiones,
    MateriaNodus* umbra)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(impletiones); k++)
    {
        Impletio* cella = (Impletio*)xar_obtinere(impletiones, k);

        si (cella->umbra == umbra)
        {
            cella->umbra = NIHIL;
            redde VERUM;
        }
    }
    /* T33: revocata = referentia scripta scopo NIHIL (olim index -I);
     * proiectio eam recusat ut olim indicem negativum */
    redde materia_nodus_reponere(umbra, (i32)ORATIO_UMBRA_IMPLETIO,
        materia_valor_referentia(NIHIL), MATERIA_LOCUS_REFERENTIA);
}

/* T32 a (2026-09-09) CAPUT UNUM PER VERBUM: dependens ordinis = carrier
 * v si umbra capitis aut carrier adpositio/particula/auxiliare (UD:
 * amod, case, mark, aux - carrier a socio pendet), socius w aliter
 * (subiectum, obiectum verbi: socius a carrier pendet). Directio eadem
 * qua oraculum arcus iudicat. */
interior i32
_dependens_ordinis (
             MateriaValor  elementa,
                      i32  v,
                      i32  a,
    constans MateriaNodus* umbra,
                      i32  w)
{
    constans MateriaValor* analyses =
        &materia_valor_lista_obtinere(elementa, v)->datum.nodus
        ->loci[ORATIO_VOCABULUM_ANALYSES];
             OratioClassis cc;

    si (   umbra->loci[ORATIO_UMBRA_RELATIO].genus
        == MATERIA_VALOR_INDEX
        && umbra->loci[ORATIO_UMBRA_RELATIO].datum.index
            == (s32)ORATIO_RELATIO_CAPUT)
    {
        redde v;
    }
    si (   analyses->genus != MATERIA_VALOR_LISTA
        || a               >= materia_valor_lista_numerus(*analyses))
    {
        redde w;
    }
    cc = oratio_genus_classis((OratioGenus)materia_valor_lista_obtinere(
        *analyses, a)->datum.nodus->genus);
    redde (   cc == ORATIO_CLASSIS_ADPOSITIO
           || cc == ORATIO_CLASSIS_PARTICULA
           || cc == ORATIO_CLASSIS_AUXILIARE) ? v : w;
}

/* umbra iam in impletionibus huius gradus? (ordo alter eiusdem umbrae
 * repetitus fit in scriptione - dependentem non petit) */
interior b32
_umbra_iam_petita (
             constans Xar* impletiones,
    constans MateriaNodus* umbra)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(impletiones); k++)
    {
        si (((constans Impletio*)xar_obtinere(impletiones, k))->umbra
            == umbra)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* LEX CAPITIS UNIUS (T32 a, ADOPTA 2026-09-09, decisio Frani): verbum
 * dependens unius capitis - ordo cuius dependens ab umbra alia iam
 * petitus est recusatur (prima vincit per dependentem; contentio
 * fiduciae infra petitionem fideliorem praefert). MENSURATUM thesauris
 * IX contra 2a26ab96 (cum contentione): ligatio Seneca 505 -> 523,
 * chartae 396/422 -> 455/479, scrinium +44..+65; coactae et casus
 * sursum ubique fere; arcus recti Senecae 2057 -> 1648 (dependentes
 * contesti 607, petitiones 1331: prima vincit rectam 46 % servabat;
 * arbor ante legem capita duo in verbo uno ferebat) - chartis arcus recti
 * SURSUM. ORATIO_CAPUT_UNUM=0 abrogat (mensura). */
/* T32 a LEX CAPITIS IN CONTENTIONE: umbra stans dependentis si umbra
 * CAPITIS impleta est (adiectivum capiti ligatum), caput eius (index
 * vocabuli, lectionis) reddere - ex cella huius gradus aut ex locis
 * umbrae (gradus prior); -I aliter. Petitio subiecti/obiecti in
 * adiectivum ad caput VICINUM sequitur pro capite revocando ('Puella
 * bona ambulat': bona caput puella tenet, subiectum ambulat puellam
 * accipit) - lex capitis posterior (_ligationes_ad_caput_sequi) idem
 * post factum faciebat, contentio eam praeveniebat. */
/* T32 b: socius petitionis stantis (vocabulum; *b = analysis) ex cella
 * huius gradus aut ex locis umbrae; -I si nulla */
interior s32
_socius_petitionis (
    constans MateriaNodus* stans,
             constans Xar* impletiones,
                      s32* b_socii)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(impletiones); k++)
    {
        constans Impletio* cella =
            (constans Impletio*)xar_obtinere(impletiones, k);

        si (cella->umbra == stans)
        {
            *b_socii = (s32)cella->b;
            redde (s32)cella->w;
        }
    }
    si (oratio_referentia_scripta(stans, (i32)ORATIO_UMBRA_IMPLETIO))
    {
        s32 w_stans;

        oratio_referentiae_ordinales(stans, (i32)ORATIO_UMBRA_IMPLETIO,
            &w_stans, b_socii);
        redde w_stans;
    }
    redde (s32)-I;
}

/* T32 b: auctor petitionis stantis (titulus regulae) ex cella aut ex
 * locu auctoris umbrae; vacuus si nullus */
interior chorda
_auctor_petitionis (
    constans MateriaNodus* stans,
             constans Xar* impletiones)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(impletiones); k++)
    {
        constans Impletio* cella =
            (constans Impletio*)xar_obtinere(impletiones, k);

        si (cella->umbra == stans)
        {
            redde cella->titulus;
        }
    }
    si (   stans->loci[ORATIO_UMBRA_AUCTOR].genus == MATERIA_VALOR_TOKEN
        && stans->loci[ORATIO_UMBRA_AUCTOR].datum.token != NIHIL)
    {
        redde stans->loci[ORATIO_UMBRA_AUCTOR].datum.token->valor;
    }
    redde _chorda(NIHIL, ZEPHYRUM);
}

/* T38 c: an lectio (w, b) iam impletio umbrae aut alterna eius sit
 * (candidata repetita) */
interior b32
_umbra_socium_habet (
    constans MateriaNodus* umbra,
             MateriaValor  elementa,
                      i32  w,
                      i32  b)
{
    constans MateriaNodus* lectio =
        oratio_analysis_ordinalibus(elementa,
        w, b);
    constans MateriaValor* alternae =
        &umbra->loci[ORATIO_UMBRA_ALTERNAE];
                      i32 k;

    si (lectio == NIHIL)
    {
        redde VERUM;
    }
    si (oratio_referentiae_scopus(umbra, (i32)ORATIO_UMBRA_IMPLETIO)
        == lectio)
    {
        redde VERUM;
    }
    si (alternae->genus != MATERIA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*alternae); k++)
    {
        constans MateriaValor* av =
            materia_valor_lista_obtinere(*alternae,
            k);

        si (   av != NIHIL && av->genus == MATERIA_VALOR_NODUS
            && oratio_referentiae_scopus(av->datum.nodus,
                (i32)ORATIO_ALTERNA_SOCIUS) == lectio)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* T32 b (2026-09-09): ALTERNAM umbrae addere - petitio cedens (socius w,
 * b; regula petens; causa; fides; regula victrix) nodus genus alterna in
 * locum ALTERNAE appensus. Origo lexematum derivatorum = pars prima
 * vocabuli socii. FALSUM = memoria. */
interior b32
_alternam_addere (
          Cursus* cursus,
    MateriaValor  elementa,
    MateriaNodus* umbra,
             i32  w,
             i32  b,
          chorda  titulus,
          chorda  victor,
             s32  causa,
             s32  fides)
{
    constans MateriaValor* partes;
             MateriaToken* origo;
             MateriaNodus* alterna;

    si (w >= materia_valor_lista_numerus(elementa))
    {
        redde VERUM;
    }
    partes = &materia_valor_lista_obtinere(elementa, w)->datum.nodus
        ->loci[ORATIO_VOCABULUM_PARTES];
    si (   partes->genus                        != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*partes) == ZEPHYRUM)
    {
        redde VERUM;
    }
    origo   = materia_valor_lista_obtinere(*partes,
        ZEPHYRUM)->datum.token;
    alterna = materia_nodus_creare(cursus->piscina,
        (s32)ORATIO_GENUS_ALTERNA,
        ORATIO_REGISTRUM.genera[ORATIO_GENUS_ALTERNA].loci_numerus);
    si (   alterna == NIHIL
        || !materia_nodus_ponere(alterna, (i32)ORATIO_ALTERNA_SOCIUS,
            materia_valor_referentia(oratio_analysis_ordinalibus(
                elementa, w, b)), MATERIA_LOCUS_REFERENTIA)
        || !materia_nodus_ponere(alterna, (i32)ORATIO_ALTERNA_CAUSA,
            materia_valor_index(causa), MATERIA_LOCUS_INDEX))
    {
        redde FALSUM;
    }
    si (   fides >= ZEPHYRUM
        && !materia_nodus_ponere(alterna, (i32)ORATIO_ALTERNA_FIDES,
            materia_valor_index(fides), MATERIA_LOCUS_INDEX))
    {
        redde FALSUM;
    }
    si (titulus.mensura > ZEPHYRUM)
    {
        MateriaToken* derivatum =
            oratio_lexema_derivatum(cursus->piscina,
            (s32)ORATIO_LEX_DERIVATUM, titulus, origo);

        si (   derivatum == NIHIL
            || !materia_nodus_ponere(alterna,
            (i32)ORATIO_ALTERNA_AUCTOR,
                materia_valor_token(derivatum), MATERIA_LOCUS_TOKEN))
        {
            redde FALSUM;
        }
    }
    si (victor.mensura > ZEPHYRUM)
    {
        MateriaToken* derivatum =
            oratio_lexema_derivatum(cursus->piscina,
            (s32)ORATIO_LEX_DERIVATUM, victor, origo);

        si (   derivatum == NIHIL
            || !materia_nodus_ponere(alterna,
            (i32)ORATIO_ALTERNA_VICTOR,
                materia_valor_token(derivatum), MATERIA_LOCUS_TOKEN))
        {
            redde FALSUM;
        }
    }
    si (cursus->census != NIHIL)
    {
        cursus->census->alternae = cursus->census->alternae + I;
    }
    redde materia_nodus_appendere(cursus->piscina, umbra,
        (i32)ORATIO_UMBRA_ALTERNAE, materia_valor_nodus(alterna),
        MATERIA_LOCUS_LISTA_NODUS);
}

interior s32
_caput_petitionis (
    constans MateriaNodus* stans,
             constans Xar* impletiones,
                      s32* b_capitis)
{
    i32 k;

    si (   stans->loci[ORATIO_UMBRA_RELATIO].genus
        != MATERIA_VALOR_INDEX
        || stans->loci[ORATIO_UMBRA_RELATIO].datum.index
            != (s32)ORATIO_RELATIO_CAPUT)
    {
        redde (s32)-I;
    }
    per (k = ZEPHYRUM; k < xar_numerus(impletiones); k++)
    {
        constans Impletio* cella =
            (constans Impletio*)xar_obtinere(impletiones, k);

        si (cella->umbra == stans)
        {
            *b_capitis = (s32)cella->b;
            redde (s32)cella->w;
        }
    }
    si (oratio_referentia_scripta(stans, (i32)ORATIO_UMBRA_IMPLETIO))
    {
        s32 w_stans;

        oratio_referentiae_ordinales(stans, (i32)ORATIO_UMBRA_IMPLETIO,
            &w_stans, b_capitis);
        redde w_stans;
    }
    redde (s32)-I;
}

interior b32
_caput_unum_activum (
    vacuum)
{
    hic_manens i32 lectum   = ZEPHYRUM;
    hic_manens b32 activum  = VERUM;

    si (!lectum)
    {
        constans character* ambitus = getenv("ORATIO_CAPUT_UNUM");

        activum = (b32)(ambitus == NIHIL
            || strcmp(ambitus, "0") != ZEPHYRUM);
        lectum = I;
    }
    redde activum;
}

/* T32 f: CONTENTIO LECTIONIS - petitio caput idem petens ac petitio stans
 * (subiectum et obiectum eiusdem verbi) arcum eundem petit: fiducia
 * arcuum eam iudicare nequit (T32 c), ordo regularum decidit -
 * recusatur, numquam revocat. ORATIO_CONTENTIO_LECTIONIS=0 abrogat. */
interior b32
_contentio_lectionis_activa (
    vacuum)
{
    hic_manens i32 lectum   = ZEPHYRUM;
    hic_manens b32 activum  = VERUM;

    si (!lectum)
    {
        constans character* ambitus =
            getenv("ORATIO_CONTENTIO_LECTIONIS");

        activum = (b32)(ambitus == NIHIL
            || strcmp(ambitus, "0") != ZEPHYRUM);
        lectum = I;
    }
    redde activum;
}

interior b32
_sententiam_resolvere_gradu (
          Cursus* cursus,
    MateriaNodus* sententia,
             i32  gradus,
             b32* vindicata,
             b32* explicita,
             b32* emendata,
         Decisio* decisiones,
         Petitio* capita_data)
{

                      Piscina* scratch;
            OratioStmlOrnatus  ornatus;
        MateriaArborConsilium  consilium;
        MateriaArborScriptura  scriptura;
        constans MateriaValor* elementa =
            &sententia->loci[ORATIO_SENTENTIA_ELEMENTA];
                          i32  ne;
                          i32  regulae_numerus;
                          i32  mensura;
                          i32  k;
                    character* textus;
                       chorda  fons;
                 StmlResultus  lectio;
         StmlExpansioResultus  expansio;
                                                    Xar* regulae;
                                                    s32* praelata;
                          Xar* impletiones;   /* Impletio (T19d) */


        si (   elementa->genus != MATERIA_VALOR_LISTA
            || materia_valor_lista_numerus(*elementa) == ZEPHYRUM)
        {
        redde VERUM;
        }

    ne               = materia_valor_lista_numerus(*elementa);
    regulae_numerus  = xar_numerus(cursus->programma->regulae);
    si (   cursus->regulae_numerus >= ZEPHYRUM
        && cursus->regulae_numerus < (s32)regulae_numerus)
    {
        regulae_numerus = (i32)cursus->regulae_numerus;
    }
    si (regulae_numerus == ZEPHYRUM)
    {
        redde VERUM;
    }
    scratch = piscina_generare_dynamicum("oratio_resolutio_sententia",
        16777216);
    si (scratch == NIHIL)
    {
        redde FALSUM;
    }
    oratio_stml_consilium_ornatum(&consilium, cursus->ratum, &ornatus,
        cursus->lingua);
    scriptura = materia_arbor_scribere_nodum(scratch, sententia,
        &consilium);
    si (!scriptura.successus)
    {
        si (cursus->census != NIHIL)
        {
            cursus->census->fractae = cursus->census->fractae + I;
        }
        piscina_destruere(scratch);
        redde VERUM;
    }
        /* compositio: proiectio + regulae primae N HUIUS gradus, lineis
     * novis divisae */
    mensura = scriptura.textus.mensura + I;
    per (k = ZEPHYRUM; k < regulae_numerus; k++)
    {
        constans OratioRegula* r =
            (constans OratioRegula*)xar_obtinere(
            cursus->programma->regulae, k);

        si (r->gradus != gradus)
        {
            perge;
        }
        mensura = mensura + r->textus.mensura + I;
    }

    textus = (character*)piscina_allocare(scratch,
        (memoriae_index)mensura
        + I);
    si (textus == NIHIL)
    {
        piscina_destruere(scratch);
        redde FALSUM;
    }
    memcpy(textus, scriptura.textus.datum,
        (size_t)scriptura.textus.mensura);
    mensura          = scriptura.textus.mensura;
    textus[mensura]  = '\n';
    mensura          = mensura + I;
        per (k = ZEPHYRUM; k < regulae_numerus; k++)
        {
        constans OratioRegula* r =
            (constans OratioRegula*)xar_obtinere(
            cursus->programma->regulae, k);

        si (r->gradus != gradus)
        {
            perge;
        }
        memcpy(textus + mensura, r->textus.datum,
            (size_t)r->textus.mensura);

        mensura          = mensura + r->textus.mensura;
        textus[mensura]  = '\n';
        mensura          = mensura + I;
        }
    textus[mensura]  = ZEPHYRUM;
    fons             = _chorda((i8*)textus, mensura);
    lectio           = stml_legere(fons, scratch, cursus->intern);
    si (!lectio.successus || lectio.radix == NIHIL)
    {
        si (cursus->census != NIHIL)
        {
            cursus->census->fractae = cursus->census->fractae + I;
        }
        piscina_destruere(scratch);
        redde VERUM;
    }
    expansio = stml_expandere(lectio.radix, scratch, cursus->intern);
    si (!expansio.successus || expansio.radix_expansa == NIHIL)
    {
        si (cursus->census != NIHIL)
        {
            cursus->census->fractae = cursus->census->fractae + I;
        }
        piscina_destruere(scratch);
        redde VERUM;
    }
    regulae = _liberi_titulo(scratch, expansio.radix_expansa, "regula");
    si (   regulae != NIHIL
        && !_regulas_fiducia_ordinare(cursus, scratch, regulae, gradus))
    {
        piscina_destruere(scratch);
        redde FALSUM;
    }
        praelata = (s32*)piscina_allocare(scratch, (memoriae_index)ne
            * (memoriae_index)magnitudo(s32));
    impletiones = xar_creare(scratch, (i32)magnitudo(Impletio));
    si (   regulae     == NIHIL || praelata == NIHIL
        || impletiones == NIHIL)
    {
        piscina_destruere(scratch);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < ne; k++)
    {
        praelata[k] = (s32)-I;
        si (!EMENDATIO_SEMEL_PER_SENTENTIAM)
        {
            emendata[k] = FALSUM;   /* semel per gradum (T19i) */
        }
    }
    /* ordines consilii cuiusque regulae ordine programmatis: prima
     * vincit */
    per (k = ZEPHYRUM; k < xar_numerus(regulae); k++)
    {
        StmlNodus* regula = *(StmlNodus**)xar_obtinere(regulae, k);
        StmlNodus* consilium_nodus = stml_invenire_liberum(regula,
            "consilium");
           chorda* titulus = stml_attributum_capere(regula, "titulus");
              Xar* ordines;
              i32  o;

        si (consilium_nodus == NIHIL)
        {
            perge;
        }
        ordines = _liberi_titulo(scratch, consilium_nodus, "praelatio");
        si (ordines == NIHIL)
        {
            piscina_destruere(scratch);
            redde FALSUM;
        }
        per (o = ZEPHYRUM; o < xar_numerus(ordines); o++)
        {
                        StmlNodus* ordo;
                              i32  v;
                              i32  a;
            constans MateriaNodus* vocabulum;
            constans MateriaValor* analyses;

            ordo = *(StmlNodus**)xar_obtinere(ordines, o);
            si (   !_numerus_attributi(ordo, "vocabulum", &v)
                || !_numerus_attributi(ordo, "analysis", &a)
                || v >= ne)
            {
                si (cursus->census != NIHIL)
                {
                    cursus->census->recusatae =
                        cursus->census->recusatae + I;
                }
                perge;
            }
            vocabulum = materia_valor_lista_obtinere(*elementa,
                v)->datum.nodus;
            analyses = &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
            si (   vocabulum->genus != (s32)ORATIO_GENUS_VOCABULUM
                || analyses->genus != MATERIA_VALOR_LISTA
                || a >= materia_valor_lista_numerus(*analyses))
            {
                si (cursus->census != NIHIL)
                {
                    cursus->census->recusatae =
                        cursus->census->recusatae + I;
                }
                perge;
            }
            si (cursus->census != NIHIL)
            {
                cursus->census->ordines = cursus->census->ordines + I;
            }
                        si (praelata[v] >= ZEPHYRUM || vindicata[v])
                        {
                si (cursus->census != NIHIL)
                {
                    cursus->census->repetitae =
                        cursus->census->repetitae + I;
                }
                perge;
                        }
                        praelata[v]  = (s32)a;
            explicita[v]             = VERUM;   /* decisio regulae */
            /* T19k: decisio in vocabulo lectionis UNIUS vana est (cellula
             * soluta, non decisa) - non notatur */
            si (_numerus_lectionum(*elementa, v) > I)
            {
                _decisionem_notare(cursus, &decisiones[v],
                    ORATIO_DECISIO_PRAELATIO, titulus);
            }

            si (   titulus != NIHIL
                && !_regulam_numerare(cursus, *titulus))
            {
                piscina_destruere(scratch);
                redde FALSUM;
            }
        }
        /* IMPLETIONES (T19d): umbra (vocabulum v, analysis a, umbra u)
         * a lectione (ad-vocabulum w, ad-analysis b) impletur. Ordo
         * validus notatur (ligatio POST permutationes, indicibus
         * remissis) et utramque lectionem praefert - carrier v/a et
         * implens w/b - prima regula per vocabulum vincente ut
         * praelatio. Ordo malus recusatur. */
                ordines = _liberi_titulo(scratch, consilium_nodus,
                    "impletio");
        /* T25: politica parium - ordines intra greges (v, u) reordinati */
        si (   ordines != NIHIL && POLITICA_PARIUM > ZEPHYRUM
            && !_ordines_ordinare(scratch, *elementa, ordines, ne))
        {
            piscina_destruere(scratch);
            redde FALSUM;
        }
        si (ordines == NIHIL)
        {
            piscina_destruere(scratch);
            redde FALSUM;
        }
        per (o = ZEPHYRUM; o < xar_numerus(ordines); o++)
        {
                        StmlNodus* ordo;
                              i32  v;
                              i32  a;
                              i32  u;
                              i32  w;
                              i32  b;
                                          MateriaNodus* umbra;
                         Impletio* cella;
                              s32  relatio;
                              b32  emendare_v;
                              b32  emendare_w;


            ordo = *(StmlNodus**)xar_obtinere(ordines, o);
            si (   !_numerus_attributi(ordo, "vocabulum", &v)
                || !_numerus_attributi(ordo, "analysis", &a)
                || !_numerus_attributi(ordo, "umbra", &u)
                || !_numerus_attributi(ordo, "ad-vocabulum", &w)
                || !_numerus_attributi(ordo, "ad-analysis", &b)
                || v >= ne || w >= ne
                || (umbra = _umbram_invenire(*elementa, v, a, u))
                    == NIHIL
                || !_analysis_adest(*elementa, w, b))
            {
                si (cursus->census != NIHIL)
                {
                    cursus->census->recusatae =
                        cursus->census->recusatae + I;
                }
                perge;
            }
            /* T31 b LEX CARRIER OBIECTI: lectio carrier prima sola */
            si (   _obiectum_carrier_primus()
                && umbra->loci[ORATIO_UMBRA_RELATIO].genus
                    == MATERIA_VALOR_INDEX
                && umbra->loci[ORATIO_UMBRA_RELATIO].datum.index
                    == (s32)ORATIO_RELATIO_OBIECTUM_VERBI
                && a != ZEPHYRUM)
            {
                si (cursus->census != NIHIL)
                {
                    cursus->census->recusatae =
                        cursus->census->recusatae + I;
                }
                perge;
            }
            /* T31 b LEX SOCII OBIECTI (classis, adpositionis, subiecti) */
            si (   umbra->loci[ORATIO_UMBRA_RELATIO].genus
                    == MATERIA_VALOR_INDEX
                && umbra->loci[ORATIO_UMBRA_RELATIO].datum.index
                    == (s32)ORATIO_RELATIO_OBIECTUM_VERBI
                && _obiectum_socius_recusandus(elementa, ne, v, a, w,
                b))
            {
                si (cursus->census != NIHIL)
                {
                    cursus->census->recusatae =
                        cursus->census->recusatae + I;
                }
                perge;
            }
            si (cursus->census != NIHIL)
            {
                cursus->census->ordines = cursus->census->ordines + I;
            }
            /* vocabulum implens gradu priore vindicatum: veritas eius =
             * lectio PRIMA (n = 0 in proiectione huius gradus); ordo
             * lectionem aliam nominans repetita est (cum puella bona:
             * regula nominativi lectionem NOM 'puellae' iam ablativae
             * inveniret) */
                                    /* LEX CLASSIS EXPLICITAE (T19i): vocabulum regula praelationis
             * decisum classem tenet; testimonium casum inter lectiones
             * classis eius (ipsam aut geminam) emendare potest - carrier
             * per umbram CAPITIS (concordantia: caput lectionem carrier
             * eligit; hoc templum NOM, non ABL prioris), implens per umbram
             * capitis aut per obiectum cuius carrier lectionem SUAM
             * decisam fert (casus obiecti lectionem adpositionis sequitur,
             * non ordinem regularum casuum: in bona terra ablativa
             * manet, in urbem intacta - mensuratum); semel per gradum.
             * Carrier sine lectione classis suae ligatur, non praefertur
             * (ut ante: supra + accusativus); implens aliena = repetita. */
                        relatio =
                            umbra->loci[ORATIO_UMBRA_RELATIO].genus
                            == MATERIA_VALOR_INDEX
                            ? umbra->loci[ORATIO_UMBRA_RELATIO].datum.index : (s32)-I;
            /* T20c lex I: ligatio trans clausulas recusata */
            si (_trans_clausulas(sententia, *elementa, v, w, relatio))
            {
                si (cursus->census != NIHIL)
                {
                    cursus->census->recusatae_clausulis =
                        cursus->census->recusatae_clausulis + I;
                }
                perge;
            }
            emendare_v = FALSUM;
            emendare_w = FALSUM;
            si (   explicita[v] && !emendata[v]
                && relatio == (s32)ORATIO_RELATIO_CAPUT)
            {
                s32 r = _lectio_classis_explicitae(*elementa, v, a,
                    praelata[v] >= ZEPHYRUM ? (i32)praelata[v]
                        : ZEPHYRUM);

                si (r >= ZEPHYRUM)
                {
                    MateriaNodus* umbra_r = r == (s32)a ? umbra
                        : _umbram_invenire(*elementa, v, (i32)r, u);

                    si (umbra_r != NIHIL)
                    {
                        a           = (i32)r;
                        umbra       = umbra_r;
                        emendare_v  = VERUM;
                    }
                }
            }
            si (explicita[w])
            {
                s32 decisa_v = praelata[v] >= ZEPHYRUM ? praelata[v]
                    : ZEPHYRUM;
                s32 decisa_w = praelata[w] >= ZEPHYRUM ? praelata[w]
                    : ZEPHYRUM;
                s32 r = _lectio_classis_explicitae(*elementa, w, b,
                    (i32)decisa_w);
                b32 licet = relatio == (s32)ORATIO_RELATIO_CAPUT
                    || (s32)a == decisa_v;

                si (r >= ZEPHYRUM && licet && !emendata[w])
                {
                    b           = (i32)r;
                    emendare_w  = VERUM;
                }
                alioquin si ((s32)b != decisa_w)
                {
                    si (cursus->census != NIHIL)
                    {
                        cursus->census->repetitae =
                            cursus->census->repetitae + I;
                    }
                    perge;
                }
            }
            alioquin si (vindicata[w] && b != ZEPHYRUM)
            {
                /* T19e: idem INTRA gradum pro decisione EXPLICITA -
                 * vocabulum implens regula praelationis priore hoc
                 * gradu lectione ALIA decisum (a determinans per
                 * exceptionem;
                 * lectio verbi Moby 'a' umbram verbi 'have' impleret:
                 * have a car) non implet. Decisio STRUCTURALIS eiusdem
                 * gradus (ordo regularum accusativi/ablativi) non
                 * obstat: mensuratum, LLCT -0.2 cum obstaret */
                si (cursus->census != NIHIL)
                {
                    cursus->census->repetitae =
                        cursus->census->repetitae + I;
                }
                perge;
            }
            /* T34 (2026-09-14): UMBRA GRADU PRIORE SCRIPTA NON
             * CONTENDIT. Umbra semel scripta (impleta aut revocata)
             * ligari non potest - LIGATIO infra eam praeterit
             * (repetita) - ergo ordo eam nominans nihil lucrari
             * potest, nocere potest: vincens contentionem petitionem
             * LIGABILEM revocat, dependentem vacuum capiens ordines
             * legitimos eiusdem gradus excludit. Custodia
             * _umbra_iam_petita idem INTRA gradum facit; haec per
             * SENTENTIAM, quia capita_data per sententiam manent.
             * MENSURATUM 2026-09-14 (proba temporaria, thesauri III):
             * tales ordines Seneca 1357 / chartae 2057 / Perseus 893,
             * quorum pars maxima innocua (eadem umbra eundem
             * dependentem re-petens), sed revocantes XLVII / XIII / XII
             * et dependentes vacuos capientes LVII / X / XXXVI. */
            /* T32 a CAPUT UNUM PER VERBUM: dependens iam ab umbra ALIA
             * petitus (hoc aut gradu priore) - ordo recusatus, prima
             * vincit per dependentem; umbra iam petita nihil petit */
            si (   _caput_unum_activum()
                && !_umbra_iam_petita(impletiones, umbra)
                && oratio_referentia_scripta(umbra,
                    (i32)ORATIO_UMBRA_IMPLETIO))
            {
                si (cursus->census != NIHIL)
                {
                    cursus->census->recusatae_scriptae =
                        cursus->census->recusatae_scriptae + I;
                }
                /* T38 c: ordo ex umbra gradu priore scripta ALTERNA
                 * PRAEOCCUPATA fit - extra contentionem exsecutoris
                 * (T34), candidata decretoris ('Puella bellum videt':
                 * Puella subiectum post bellum); eadem (w, b) iam
                 * impletio aut alterna = repetita, non scripta */
                si (!_umbra_socium_habet(umbra, *elementa, w, b))
                {
                    s32 fides = titulus != NIHIL
                        ? _fiducia_regulae(cursus->programma, *titulus)
                        : (s32)-I;

                    si (!_alternam_addere(cursus, *elementa, umbra, w,
                        b,
                        titulus != NIHIL ? *titulus
                            : _chorda(NIHIL, ZEPHYRUM),
                        _auctor_petitionis(umbra, impletiones),
                        (s32)ORATIO_ALTERNA_CAUSA_PRAEOCCUPATA, fides))
                    {
                        piscina_destruere(scratch);
                        redde FALSUM;
                    }
                    si (cursus->census != NIHIL)
                    {
                        cursus->census->alternae_praeoccupatae =
                            cursus->census->alternae_praeoccupatae + I;
                    }
                }
            }
            si (   _caput_unum_activum()
                && !_umbra_iam_petita(impletiones, umbra)
                && !oratio_referentia_scripta(umbra,
                    (i32)ORATIO_UMBRA_IMPLETIO))
            {
                i32 dependens = _dependens_ordinis(*elementa, v, a,
                    umbra, w);
                Petitio* petitio = &capita_data[dependens];
                    s32  fides = titulus != NIHIL
                        ? _fiducia_regulae(cursus->programma, *titulus)
                        : (s32)-I;
                s32 iudicium     = ZEPHYRUM;   /* T32 c */
                s32 caput_novum  = (s32)-I;   /* T32 f */
                b32 lectionis    = FALSUM;

                /* LEX CAPITIS IN CONTENTIONE: socius w adiectivum capiti
                 * vicino iam ligatum - petitio subiecti/obiecti ad caput
                 * transit (w, b := caput), dependens caput fit */
                si (   petitio->umbra != NIHIL
                    && petitio->umbra != umbra
                    && dependens      == w
                    && (   relatio == (s32)ORATIO_RELATIO_SUBIECTUM
                        || relatio
                            == (s32)ORATIO_RELATIO_OBIECTUM_VERBI))
                {
                    s32 b2 = (s32)-I;
                    s32 w2 = _caput_petitionis(petitio->umbra,
                        impletiones,
                        &b2);

                    si (   w2 >= ZEPHYRUM && w2 < (s32)ne
                        && b2 >= ZEPHYRUM
                        && w2 != (s32)v
                        && (w2 > (s32)w ? w2 - (s32)w : (s32)w - w2)
                            == I)
                    {
                        /* caput VICINUM adest: petitio ad caput transit si
                         * lectio capitis umbrae concordat (casus, numerus)
                         * et licet; aliter RECUSATUR - petitio subiecti in
                         * adiectivum capiti vicino ligatum caput numquam
                         * revocat (concordantia vicina testimonium fortius,
                         * 743/624 contra 705 permille) */
                        b32 licet = (b32)(   _analysis_adest(*elementa,
                                (i32)w2, (i32)b2)
                            && !(vindicata[w2] && b2 != ZEPHYRUM)
                            && !(   explicita[w2]
                            && praelata[w2] >= ZEPHYRUM
                                 && b2 != praelata[w2]));

                        si (licet)
                        {
                            constans MateriaNodus* lectio =
                                materia_valor_lista_obtinere(
                                materia_valor_lista_obtinere(*elementa,
                                    (i32)w2)->datum.nodus
                                    ->loci[ORATIO_VOCABULUM_ANALYSES],
                                (i32)b2)->datum.nodus;
                            s32 casus_umbrae =
                                umbra->loci[ORATIO_UMBRA_CASUS].genus
                                == MATERIA_VALOR_INDEX
                                ? umbra->loci[ORATIO_UMBRA_CASUS].datum.index
                                : (s32)-I;
                            s32 numerus_umbrae =
                                umbra->loci[ORATIO_UMBRA_NUMERUS].genus
                                == MATERIA_VALOR_INDEX
                                ? umbra->loci[ORATIO_UMBRA_NUMERUS].datum.index
                                : (s32)-I;

                            licet = (b32)(   (   casus_umbrae < ZEPHYRUM
                                              || _accidens_lectionis(lectio,
                                                  "casus")
                                                      == casus_umbrae)
                                          && (   numerus_umbrae
                                              < ZEPHYRUM
                                              || _accidens_lectionis(lectio,
                                                  "numerus")
                                                      == numerus_umbrae));
                        }
                        /* T32 c: iudex - si tabula petitionem in adiectivum
                         * rectam dicit (caput-praecedente > subiectum
                         * 84 %), contentio infra caput revocat */
                        si (!licet)
                        {
                            iudicium =
                                _iudex_contentionis(cursus->programma,
                                scratch,
                                _auctor_petitionis(petitio->umbra,
                                    impletiones),
                                titulus != NIHIL ? *titulus
                                    : _chorda(NIHIL, ZEPHYRUM));
                        }
                        si (!licet && iudicium <= ZEPHYRUM)
                        {
                            si (cursus->census != NIHIL)
                            {
                                cursus->census->recusatae_capitis =
                                    cursus->census->recusatae_capitis
                                        + I;
                            }
                            /* T32 b: petitio recusata alterna fit */
                            si (!_alternam_addere(cursus, *elementa,
                                umbra,
                                    w, b, titulus != NIHIL ? *titulus
                                        : _chorda(NIHIL, ZEPHYRUM),
                                    _auctor_petitionis(petitio->umbra,
                                        impletiones),
                                    (s32)ORATIO_ALTERNA_CAUSA_RECUSATA,
                                    fides))
                            {
                                piscina_destruere(scratch);
                                redde FALSUM;
                            }
                            perge;
                        }
                        si (licet)
                        {
                            w          = (i32)w2;
                            b          = (i32)b2;
                            dependens  = w;
                            petitio    = &capita_data[dependens];
                            si (cursus->census != NIHIL)
                            {
                                cursus->census->ad_caput_secutae =
                                    cursus->census->ad_caput_secutae
                                        + I;
                            }
                        }
                    }
                }
                /* T32 f: caput petitionis novae (dependens socius: carrier v;
                 * dependens carrier: socius w); caput idem ac petitionis
                 * stantis in STELLA VERBI (subiectum contra obiectum verbi
                 * eiusdem, T32 d/e mensuratum) = contentio LECTIONIS -
                 * recusatur sub ordine. Paria alia capite eodem (obiecta
                 * adpositionis accusativum contra ablativum) fiduciae
                 * manent donec mensurentur ('In bona terra est'). */
                caput_novum = dependens == w ? (s32)v : (s32)w;
                {
                    s32 relatio_stans = petitio->umbra != NIHIL
                        && petitio->umbra->loci[ORATIO_UMBRA_RELATIO].genus
                            == MATERIA_VALOR_INDEX
                        ? petitio->umbra->loci[ORATIO_UMBRA_RELATIO]
                            .datum.index
                        : (s32)-I;

                    lectionis = (b32)(   _contentio_lectionis_activa()
                        && petitio->umbra != NIHIL
                        && petitio->umbra != umbra
                        && petitio->caput >= ZEPHYRUM
                        && petitio->caput == caput_novum
                        && (   relatio == (s32)ORATIO_RELATIO_SUBIECTUM
                            || relatio
                                == (s32)ORATIO_RELATIO_OBIECTUM_VERBI)
                        && (   relatio_stans
                                == (s32)ORATIO_RELATIO_SUBIECTUM
                            || relatio_stans
                                == (s32)ORATIO_RELATIO_OBIECTUM_VERBI));
                }
                si (petitio->umbra != NIHIL && petitio->umbra != umbra)
                {
                    /* CONTENTIO: iudex (T32 c, tabula parium) primum, deinde
                     * fiducia (T32 a): petitio fidelior stantem revocat;
                     * contentio lectionis (T32 f) numquam */
                    iudicium = _iudex_contentionis(cursus->programma,
                        scratch,
                        _auctor_petitionis(petitio->umbra, impletiones),
                        titulus != NIHIL ? *titulus : _chorda(NIHIL,
                        ZEPHYRUM));
                    si (   !lectionis
                        && (   iudicium > ZEPHYRUM
                            || (   iudicium == ZEPHYRUM
                                && (_fiducia_modus() & (i32)II)
                                && fides > petitio->fides)))
                    {
                        /* T32 b: petitio stans alterna (revocata) suae
                         * umbrae fit antequam vacuetur */
                        s32 b_stans = (s32)-I;
                        s32 w_stans = _socius_petitionis(petitio->umbra,
                            impletiones, &b_stans);

                        si (   w_stans >= ZEPHYRUM
                            && b_stans >= ZEPHYRUM
                            && !_alternam_addere(cursus, *elementa,
                                petitio->umbra, (i32)w_stans,
                                (i32)b_stans,
                                _auctor_petitionis(petitio->umbra,
                                    impletiones),
                                titulus != NIHIL ? *titulus
                                    : _chorda(NIHIL, ZEPHYRUM),
                                (s32)ORATIO_ALTERNA_CAUSA_REVOCATA,
                                petitio->fides))
                        {
                            piscina_destruere(scratch);
                            redde FALSUM;
                        }
                        si (!_petitionem_revocare(impletiones,
                                petitio->umbra))
                        {
                            piscina_destruere(scratch);
                            redde FALSUM;
                        }
                        si (cursus->census != NIHIL)
                        {
                            cursus->census->revocatae_capitis =
                                cursus->census->revocatae_capitis + I;
                        }
                    }
                    alioquin
                    {
                        si (cursus->census != NIHIL)
                        {
                            cursus->census->recusatae_capitis =
                                cursus->census->recusatae_capitis + I;
                            si (lectionis)
                            {
                                cursus->census->recusatae_lectionis =
                                    cursus->census->recusatae_lectionis
                                        + I;
                            }
                        }
                        /* T32 b: petitio recusata alterna fit */
                        si (!_alternam_addere(cursus, *elementa, umbra,
                            w, b,
                                titulus != NIHIL ? *titulus
                                    : _chorda(NIHIL, ZEPHYRUM),
                                _auctor_petitionis(petitio->umbra,
                                    impletiones),
                                (s32)ORATIO_ALTERNA_CAUSA_RECUSATA,
                                fides))
                        {
                            piscina_destruere(scratch);
                            redde FALSUM;
                        }
                        perge;
                    }
                }
                petitio->umbra = umbra;
                petitio->fides = fides;
                petitio->caput = caput_novum;
            }
            cella = (Impletio*)xar_addere(impletiones);

            si (cella == NIHIL)
            {
                piscina_destruere(scratch);
                redde FALSUM;
            }
            cella->umbra  = umbra;
            cella->w      = w;
            cella->b      = b;
            cella->analysis  = oratio_analysis_ordinalibus(*elementa, w,
                b);
            cella->v         = v;
            cella->titulus  = titulus != NIHIL ? *titulus
                : _chorda(NIHIL, ZEPHYRUM);
                        /* praelationes ambae sub lege 'prima vincit per
             * vocabulum' (trans gradus quoque: vindicata) */
                                    si (emendare_v)
                                    {
                /* classis explicita, casus emendatus: decisio prioris
                 * manet (auctor), lectio classis suae prima */
                praelata[v] = (s32)a;
                emendata[v] = VERUM;
                                    }
                        alioquin si (   praelata[v] < ZEPHYRUM
                                     && !vindicata[v])
                                    {
                praelata[v] = (s32)a;
                si (_numerus_lectionum(*elementa, v) > I)
                {
                    _decisionem_notare(cursus, &decisiones[v],
                        ORATIO_DECISIO_IMPLETIO, titulus);
                }
                                    }
                        si (emendare_w)
                        {
                praelata[w] = (s32)b;
                emendata[w] = VERUM;
                        }
                        alioquin si (   praelata[w] < ZEPHYRUM
                                     && !vindicata[w])
                        {
                praelata[w] = (s32)b;
                si (_numerus_lectionum(*elementa, w) > I)
                {
                    _decisionem_notare(cursus, &decisiones[w],
                        ORATIO_DECISIO_IMPLETIO, titulus);
                }
                        }

            si (   titulus != NIHIL
                && !_regulam_numerare(cursus, *titulus))
            {
                piscina_destruere(scratch);
                redde FALSUM;
            }
        }
    }
    /* permutationes: analysis praelata prima, ceterae ordine suo */
    per (k = ZEPHYRUM; k < ne; k++)
    {
                MateriaNodus* vocabulum;
        constans MateriaValor* analyses;
                          i32  n;
                          i32* ordo;
                          i32  i;
                          i32  j;

        si (praelata[k] <= ZEPHYRUM)
        {
            perge;   /* nihil aut iam prima */
        }
        vocabulum = materia_valor_lista_obtinere(*elementa,
            k)->datum.nodus;
        analyses  = &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
        n         = materia_valor_lista_numerus(*analyses);
        ordo      = (i32*)piscina_allocare(scratch, (memoriae_index)n
            * (memoriae_index)magnitudo(i32));
        si (ordo == NIHIL)
        {
            piscina_destruere(scratch);
            redde FALSUM;
        }
                ordo[ZEPHYRUM]  = (i32)praelata[k];
        j                       = I;
        per (i = ZEPHYRUM; i < n; i++)
        {
            si (i != (i32)praelata[k])
            {
                ordo[j]  = i;
                j        = j + I;
            }
        }
        /* T33: referentiae in analyses huius vocabuli permutationem
         * supervivunt (elementa eadem, ordo solus) - remissio nulla */
        si (   !materia_nodus_lista_permutare(cursus->piscina,
            vocabulum,
                (i32)ORATIO_VOCABULUM_ANALYSES, ordo, n)
            || !oratio_partes_compendia_reponere(cursus->piscina,
                vocabulum))
        {
            piscina_destruere(scratch);
            redde FALSUM;
        }
        si (cursus->census != NIHIL)
        {
            cursus->census->applicatae = cursus->census->applicatae + I;
        }
    }
    /* LIGATIO umbrarum (T19d) post permutationes: referentia ad nodum
     * analysis ante permutationes captum (T33; olim index in ordinem
     * novum remissus); umbra iam ligata manet (prima vincit: repetita).
     * Lex 'semel scribere' ligationem primam per ponere custodit. */
    per (k = ZEPHYRUM; k < xar_numerus(impletiones); k++)
    {
        Impletio* imp = (Impletio*)xar_obtinere(impletiones, k);

        si (imp->umbra == NIHIL)
        {
            perge;   /* T32 a: petitio revocata */
        }
        si (oratio_referentia_scripta(imp->umbra,
                (i32)ORATIO_UMBRA_IMPLETIO))
        {
            si (cursus->census != NIHIL)
            {
                cursus->census->repetitae =
                    cursus->census->repetitae + I;
            }
            perge;
        }
        si (   imp->analysis == NIHIL
            || !materia_nodus_ponere(imp->umbra,
                (i32)ORATIO_UMBRA_IMPLETIO,
                materia_valor_referentia(imp->analysis),
                MATERIA_LOCUS_REFERENTIA)
            || !_auctorem_umbrae_scribere(cursus, *elementa, imp->umbra,
                imp->v, imp->titulus))
        {
            piscina_destruere(scratch);
            redde FALSUM;
        }
                si (cursus->census != NIHIL)
                {
            cursus->census->impletae = cursus->census->impletae + I;
                }
    }
    /* vindicatio: vocabula hoc gradu praelata (etiam iam prima)
     * gradibus sequentibus clausa */
    per (k = ZEPHYRUM; k < ne; k++)
    {
        si (praelata[k] >= ZEPHYRUM)
        {
            vindicata[k] = VERUM;
        }
    }
        piscina_destruere(scratch);
    redde VERUM;
}

/* locus 'umbrae' lectionis: valor LISTA aut NIHIL (genus sine umbris,
 * locus non scriptus) */
interior constans MateriaValor*
_umbrae_lectionis (
    constans MateriaNodus* analysis)
{
    OratioClassis classis =
        oratio_genus_classis((OratioGenus)analysis->genus);
              s32 locus;

    si (classis >= ORATIO_CLASSIS_NUMERUS_CLASSIUM)
    {
        redde NIHIL;
    }
    locus = oratio_partes_locus(classis, "umbrae");
    si (locus < ZEPHYRUM || (i32)locus >= analysis->numerus_locorum)
    {
        redde NIHIL;
    }
    si (analysis->loci[locus].genus != MATERIA_VALOR_LISTA)
    {
        redde NIHIL;
    }
    redde &analysis->loci[locus];
}

/* GRADUS umbrarum lectionis vocabuli k (lex umbrarum, T19d gamma):
 * II = umbram fert et omnes a VICINO impletae (|w - k| = I), I =
 * aliter (sine umbris - lectio Anglica semper -, umbra vacua, implens
 * remotus). MENSURATUM 2026-09-07: depressio lectionum vacuarum sub
 * lectiones sine umbris adiectiva deprimit (implens ignotus aut sine
 * casu: LLCT -3, CIRCSE -4); impletio remota (regula praecedens ad
 * vocabulum PRIMUM congruens ligat, non proximum) substantiva Senecae
 * deprimit (-3) dum adiectiva chartarum tollit (+10): vicinitas sola
 * thesaurum nullum deprimit - ianua 'strictus'/'proximus' in machina
 * exemplarium legem laxaret. */
interior i32
_gradus_umbrarum (

    constans MateriaNodus* analysis,
                      i32  k)
{
    constans MateriaValor* umbrae = _umbrae_lectionis(analysis);
                      i32  n;
                      i32  u;

    si (umbrae == NIHIL)
    {
        redde I;
    }
    n = materia_valor_lista_numerus(*umbrae);
    si (n == ZEPHYRUM)
    {
        redde I;
    }
    per (u = ZEPHYRUM; u < n; u++)
    {
        constans MateriaValor* valor =
            materia_valor_lista_obtinere(*umbrae, u);

                s32 w;
                s32 b;

        si (   valor        == NIHIL
            || valor->genus != MATERIA_VALOR_NODUS
            || !oratio_referentia_scripta(valor->datum.nodus,
                (i32)ORATIO_UMBRA_IMPLETIO))
        {
            redde I;   /* vacua */
        }
        oratio_referentiae_ordinales(valor->datum.nodus,
            (i32)ORATIO_UMBRA_IMPLETIO, &w, &b);
        si (w < ZEPHYRUM || ((i32)w > k ? (i32)w - k : k - (i32)w) != I)
        {
            redde I;   /* implens remotus */
        }

    }
    redde (i32)II;

}

/* LEX UMBRARUM (T19d gamma) post gradus omnes: in vocabulo quoque
 * lectiones ordine graduum umbrarum (II: umbrae a vicinis impletae,
 * I: ceterae), intra gradum ordine manente - lectio cuius umbrae
 * impleri non potuerunt lectioni cuius impletae sunt cedit (in bona
 * terra: adiectivum bonae, gradu I substantivo ut obiecto 'in'
 * praelato, per caput terram tollitur). Vocabula per PRAELATIONEM
 * decisa (regula classis, exceptio, regula linguae) intacta: ligatio
 * fortuita decisionem explicitam non vertit. Permutatio per
 * lista_permutare; ligationes quae in vocabulum permutatum spectant
 * per ordinem novum remissae (impletio-analysis reponere). Lex in C,
 * numquam arithmetica in STML (Fran). FALSUM = memoria sola. */
interior b32
_umbris_ordinare (
          Cursus* cursus,
    MateriaNodus* sententia,
    constans b32* explicita,
         Decisio* decisiones)
{
    constans MateriaValor* elementa =
        &sententia->loci[ORATIO_SENTENTIA_ELEMENTA];
                  Piscina* scratch;
                      i32  ne;
                      i32  k;

    ne       = materia_valor_lista_numerus(*elementa);
    scratch  = piscina_generare_dynamicum("oratio_lex_umbrarum", 65536);
    si (scratch == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < ne; k++)
    {
        constans MateriaValor* elementum =
            materia_valor_lista_obtinere(*elementa, k);
                 MateriaNodus* vocabulum;
        constans MateriaValor* analyses;
                          i32* gradus_lectionum;
                          i32* ordo;
                          i32  n;
                          i32  i;
                          i32  j;
                          s32  g;
                          b32  immotum = VERUM;

        si (   elementum        == NIHIL
            || elementum->genus != MATERIA_VALOR_NODUS
            || elementum->datum.nodus->genus
                != (s32)ORATIO_GENUS_VOCABULUM
            || explicita[k])
        {
            perge;   /* decisio regulae explicitae (praelatio) manet:
                      * ligatio fortuita eam non vertit (EWT 'a' + die
                      * ablativus, LLCT supra + accusativus) */
        }
        vocabulum = elementum->datum.nodus;

        analyses   = &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
        si (analyses->genus != MATERIA_VALOR_LISTA)
        {
            perge;
        }
        n          = materia_valor_lista_numerus(*analyses);
        si (n < (i32)II)
        {
            perge;
        }
        gradus_lectionum = (i32*)piscina_allocare(scratch,
            (memoriae_index)n * (memoriae_index)magnitudo(i32));
        ordo             = (i32*)piscina_allocare(scratch,
            (memoriae_index)n * (memoriae_index)magnitudo(i32));
        si (gradus_lectionum == NIHIL || ordo == NIHIL)
        {
            piscina_destruere(scratch);
            redde FALSUM;
        }
        per (i = ZEPHYRUM; i < n; i++)
        {
            constans MateriaValor* valor =
                materia_valor_lista_obtinere(*analyses, i);

                        gradus_lectionum[i] =
                            valor != NIHIL
                                && valor->genus == MATERIA_VALOR_NODUS
                            ? _gradus_umbrarum(valor->datum.nodus,
                            k) : I;

        }
        j = ZEPHYRUM;
        per (g = (s32)II; g >= ZEPHYRUM; g--)
        {
            per (i = ZEPHYRUM; i < n; i++)
            {
                si (gradus_lectionum[i] == (i32)g)
                {
                    ordo[j]  = i;
                    j        = j + I;
                }
            }
        }
        per (i = ZEPHYRUM; i < n; i++)
        {
            si (ordo[i] != i)
            {
                immotum = FALSUM;
            }
        }
        si (immotum)
        {
            perge;
        }
        si (   !materia_nodus_lista_permutare(cursus->piscina,
            vocabulum,
                (i32)ORATIO_VOCABULUM_ANALYSES, ordo, n)
            || !oratio_partes_compendia_reponere(cursus->piscina,
                vocabulum))
        {
            piscina_destruere(scratch);
            redde FALSUM;
        }
        si (cursus->census != NIHIL)
        {
            cursus->census->umbris_ordinata =
                cursus->census->umbris_ordinata + I;
        }
                /* decisio = actus qui lectionem PRIMAM fecit: lex auctor solum
         * ubi primam mutavit; lectio inferior sublata (bona VOC post
         * NOM) decisionem priorem non tollit */
        si (ordo[ZEPHYRUM] != ZEPHYRUM)
        {
            _decisionem_notare(cursus, &decisiones[k],
                ORATIO_DECISIO_UMBRA, NIHIL);
        }
    }
    /* T33: ligationes referentiae sunt - permutatio eas non movet */
    piscina_destruere(scratch);
    redde VERUM;
}

/* sententiam unam resolvere: gradus I .. maximus regularum primarum N,
 * quisque proiectione nova (permutationes gradus prioris visae),
 * vocabulis vindicatis trans gradus; post gradus omnes lex umbrarum.
 * FALSUM = memoria sola. */
/* PRIOR CASUUM (T24, 2026-09-08; census casus_prior.py post columnam
 * casuum T23): ordo dictionarii flexionum (WORDS: nominativus ante
 * accusativum, dativus ante ablativum, locativus ubi tabula eum ponit)
 * ordo lectionum est ubi nulla regula loquitur (DCCXCIX errata casuum
 * Senecae strati aperti) et ordo quem regulae capitis percurrunt (par
 * congruens PRIMUM ligant). Prior lectiones cuiusque vocabuli intra
 * greges classis eiusdem (ordo gregum manet) ordine casuum MENSURATO
 * permutat, ante regulas omnes; nihil eliminat, nihil decidit (decisio
 * non scribitur). Ordo globalis abl > acc > gen > nom > dat > voc > loc
 * (census: Seneca 61.1 -> 65.3, chartae 56.7/56.9 -> 71.7/72.9 %;
 * optimum per copiam 73.4/73.8/76.4). MENSURATUM post regulas verbis
 * apertis solis: 609/565/567 -> 636/617/630 (ordo globalis), 645/622/637
 * cum entrio abl-acc-nom-voc; primarium et coactae immota. Ante regulas
 * (mensuratum et ablatum) primarium cadebat: regulae capitis par
 * congruens primum ligant. Entria per COPIAM candidatorum
 * (casus lectionum Latinarum gregis): copiae discordes inter thesauros
 * abl/acc/nom/voc, abl/nom/voc, abl/acc (Seneca nom/acc, chartae abl) -
 * quodque solum mensuratum (ORATIO_PRIOR_CASUUM_SOLA=titulus). */
hic_manens constans b32 PRIOR_CASUUM = VERUM;

nomen structura {
    constans character* titulus;
    i32 copia;          /* bitmask casuum gregis (I << casus) */
    s32 casus_primus;   /* casus primus pro copia */
    b32 activa;
} PriorCopiae;

#define COPIA(a) (I << (i32)(a))
hic_manens constans PriorCopiae PRIOR_COPIAE[] = {
    /* MENSURATUM (casus permille Seneca / chartae dev / test; basis
     * 609/565/567; ordo globalis solus 636/617/630; primarium et coactae
     * immota semper): abl-acc-nom-voc -> nom 645/622/637 SERVATUM;
     * abl-nom-voc -> nom 640/617/629 (test cadit) RECUSATUM; abl-acc ->
     * acc 648/596/606 (chartae cadunt) RECUSATUM */
    { "abl-acc-nom-voc-nom", COPIA(ORATIO_CASUS_ABLATIVUS)
        | COPIA(ORATIO_CASUS_ACCUSATIVUS)
            | COPIA(ORATIO_CASUS_NOMINATIVUS)
                | COPIA(ORATIO_CASUS_VOCATIVUS),
                    (s32)ORATIO_CASUS_NOMINATIVUS, VERUM },
    { "abl-nom-voc-nom", COPIA(ORATIO_CASUS_ABLATIVUS)
        | COPIA(ORATIO_CASUS_NOMINATIVUS)
            | COPIA(ORATIO_CASUS_VOCATIVUS),
                (s32)ORATIO_CASUS_NOMINATIVUS, FALSUM },
    { "abl-acc-acc", COPIA(ORATIO_CASUS_ABLATIVUS)
        | COPIA(ORATIO_CASUS_ACCUSATIVUS),
            (s32)ORATIO_CASUS_ACCUSATIVUS, FALSUM }
};
hic_manens constans i32 PRIOR_COPIAE_NUMERUS =
    (i32)(magnitudo(PRIOR_COPIAE) / magnitudo(PRIOR_COPIAE[0]));

/* an entrium e activum sit (tabula, deinde ambitus mensurae SOLA) */
interior b32
_copia_activa (
    i32 e)
{
                   hic_manens i32  lectum  = ZEPHYRUM;
    hic_manens constans character* sola    = NIHIL;

    si (!lectum)
    {
        sola    = getenv("ORATIO_PRIOR_CASUUM_SOLA");
        lectum  = I;
    }
    si (sola != NIHIL)
    {
        redde (b32)(strcmp(sola, PRIOR_COPIAE[e].titulus) == ZEPHYRUM);
    }
    redde PRIOR_COPIAE[e].activa;
}

/* lectiones vocabuli ordine casuum intra greges classis permutare;
 * FALSUM = memoria */
interior b32
_prior_casuum_vocabuli (
          Cursus* cursus,
         Piscina* scratch,
    MateriaNodus* vocabulum)
{
    constans MateriaValor* analyses =
        &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
    i32  n;
        i32* ordo;
    s32* grex;      /* index gregis classis per lectionem (signatus: -I = nondum) */
    s32* gradus;    /* gradus casuum per lectionem; NUMERUS sine casu */
    i32  copiae[ORATIO_CLASSIS_NUMERUS_CLASSIUM];
    s32  primus_gregis[ORATIO_CLASSIS_NUMERUS_CLASSIUM];
    s32  greges = ZEPHYRUM;
    i32  i;
    i32  j;
    b32  identitas = VERUM;

    si (analyses->genus != MATERIA_VALOR_LISTA)
    {
        redde VERUM;
    }
    n = materia_valor_lista_numerus(*analyses);
    si (n < (i32)II)
    {
        redde VERUM;
    }
    ordo   = (i32*)piscina_allocare(scratch, (memoriae_index)n
        * (memoriae_index)magnitudo(i32));
        grex   = (s32*)piscina_allocare(scratch, (memoriae_index)n
            * (memoriae_index)magnitudo(s32));
    gradus = (s32*)piscina_allocare(scratch, (memoriae_index)n
        * (memoriae_index)magnitudo(s32));
    si (ordo == NIHIL || grex == NIHIL || gradus == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM; i++)
    {
                copiae[i]  = ZEPHYRUM;
        primus_gregis[i]   = (s32)-I;
    }
    /* greges (ordine apparitionis classis) et copiae casuum Latinae */
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans MateriaNodus* lectio = materia_valor_lista_obtinere(
            *analyses, i)->datum.nodus;
        OratioClassis cl =
            oratio_genus_classis((OratioGenus)lectio->genus);
        s32 casus_lectionis = _accidens_lectionis(lectio, "casus");
        b32 latina = (b32)(lectio->loci[ORATIO_ANALYSIS_LINGUA].genus
                == MATERIA_VALOR_INDEX
            && lectio->loci[ORATIO_ANALYSIS_LINGUA].datum.index
                == (s32)ORATIO_LINGUA_LATINA);

        si ((i32)cl >= (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM)
        {
            cl = ORATIO_CLASSIS_IGNOTUM;
        }
        si (primus_gregis[cl] < ZEPHYRUM)
        {
            primus_gregis[cl]  = greges;
            greges             = greges + I;
        }
        grex[i]            = primus_gregis[cl];
                gradus[i]  = (s32)ORATIO_CASUS_NUMERUS;   /* sine casu: post */
        si (   latina && casus_lectionis >= ZEPHYRUM
            && casus_lectionis < (s32)ORATIO_CASUS_NUMERUS)
        {
            gradus[i]   = ORDO_CASUUM[casus_lectionis];
            copiae[cl]  = copiae[cl] | COPIA(casus_lectionis);
        }
    }
    /* entria per copiam: casus primus gradum -I accipit */
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans MateriaNodus* lectio = materia_valor_lista_obtinere(
            *analyses, i)->datum.nodus;
        OratioClassis cl =
            oratio_genus_classis((OratioGenus)lectio->genus);
        s32 casus_lectionis = _accidens_lectionis(lectio, "casus");
        i32 e;

        si (   (i32)cl   >= (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
            || gradus[i] >= (s32)ORATIO_CASUS_NUMERUS)
        {
            perge;
        }
        per (e = ZEPHYRUM; e < PRIOR_COPIAE_NUMERUS; e++)
        {
            si (   _copia_activa(e)
                && PRIOR_COPIAE[e].copia        == copiae[cl]
                && PRIOR_COPIAE[e].casus_primus == casus_lectionis)
            {
                                gradus[i] = (s32)-I;   /* entrium copiae: primus */
            }
        }
    }
        /* ordo: lectiones CLASSIS PRIMAE solae (grex 0) inter sedes suas
     * ordine gradus (stabiliter) permutantur; ceterae immotae - nihil
     * consolidatur (Cum: adpositio adverbium coniunctio ... manet) */
    per (i = ZEPHYRUM; i < n; i++)
    {
        ordo[i] = i;
    }
    {
        i32 sedes[ORATIO_CLASSIS_NUMERUS_CLASSIUM * (i32)IV];
        i32 m = ZEPHYRUM;

        per (i = ZEPHYRUM; i < n && m < (i32)(magnitudo(sedes)
            / magnitudo(sedes[0])); i++)
        {
            si (grex[i] == ZEPHYRUM)
            {
                sedes[m]  = i;
                m         = m + I;
            }
        }
        /* insertio stabilis indicum classis primae per gradus */
        per (i = I; i < m; i++)
        {
            i32 x = sedes[i];

            j = i;
            dum (j > ZEPHYRUM && gradus[x] < gradus[sedes[j - I]])
            {
                sedes[j]  = sedes[j - I];
                j         = j - I;
            }
            sedes[j] = x;
        }
        /* sedes veteres classis primae ordine, lectiones ordinatae in eas */
        j = ZEPHYRUM;
        per (i = ZEPHYRUM; i < n && j < m; i++)
        {
            si (grex[i] == ZEPHYRUM)
            {
                ordo[i]  = sedes[j];
                j        = j + I;
            }
        }
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (ordo[i] != i)
        {
            identitas = FALSUM;
        }
    }
    si (identitas)
    {
        redde VERUM;
    }
    si (   !materia_nodus_lista_permutare(cursus->piscina, vocabulum,
            (i32)ORATIO_VOCABULUM_ANALYSES, ordo, n)
        || !oratio_partes_compendia_reponere(cursus->piscina,
        vocabulum))
    {
        redde FALSUM;
    }
    si (cursus->census != NIHIL)
    {
        cursus->census->prior_casuum = cursus->census->prior_casuum + I;
    }
    redde VERUM;
}

/* prior POST regulas omnes, verbis APERTIS solis: nec decisis, nec
 * umbram impletam ferentibus, nec ab umbra impleta petitis (indices
 * analysis implentis in listam petiti spectant - permutatio eos
 * frangeret). MENSURATUM ante regulas (2026-09-08): primarium Senecae
 * 835 -> 808, chartae 863/856 -> 822/811 - ablativo primo ubique
 * regulae capitis laxae congruentiam ablativam spuriam inter vicinos
 * inveniunt et gemellum falsum promovent. */
interior b32
_prior_casuum (
            Cursus* cursus,
      MateriaNodus* sententia,
  constans Decisio* decisiones)
{
    constans MateriaValor* elementa =
        &sententia->loci[ORATIO_SENTENTIA_ELEMENTA];
    Piscina* scratch = piscina_generare_dynamicum("prior_casuum",
        1048576);
    i32  ne = materia_valor_lista_numerus(*elementa);
    b32* tangitur;
    i32  k;

    si (scratch == NIHIL)
    {
        redde FALSUM;
    }
    tangitur = (b32*)piscina_allocare(scratch, (memoriae_index)ne
        * (memoriae_index)magnitudo(b32));
    si (tangitur == NIHIL)
    {
        piscina_destruere(scratch);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < ne; k++)
    {
        tangitur[k] = (b32)(decisiones[k].genus >= ZEPHYRUM);
    }
    /* umbrae impletae: carrier et petitum tanguntur */
    per (k = ZEPHYRUM; k < ne; k++)
    {
        constans MateriaNodus* e =
            materia_valor_lista_obtinere(*elementa,
            k)->datum.nodus;
        constans MateriaValor* analyses;
                          i32  i;

        si (e->genus != (s32)ORATIO_GENUS_VOCABULUM)
        {
            perge;
        }
        analyses = &e->loci[ORATIO_VOCABULUM_ANALYSES];
        si (analyses->genus != MATERIA_VALOR_LISTA)
        {
            perge;
        }
        per (i = ZEPHYRUM; i
            < materia_valor_lista_numerus(*analyses); i++)
        {
            constans MateriaValor* umbrae = _umbrae_lectionis(
                materia_valor_lista_obtinere(*analyses,
                i)->datum.nodus);
            i32 u;

            per (u = ZEPHYRUM; umbrae != NIHIL
                && u < materia_valor_lista_numerus(*umbrae); u++)
            {
                constans MateriaNodus* umbra =
                    materia_valor_lista_obtinere(
                    *umbrae, u)->datum.nodus;
                s32 w;
                s32 b;

                si (   oratio_referentiae_ordinales(umbra,
                        (i32)ORATIO_UMBRA_IMPLETIO, &w, &b)
                    && w < (s32)ne)
                {
                    tangitur[k] = VERUM;
                    tangitur[w] = VERUM;
                }
            }
        }
    }
    per (k = ZEPHYRUM; k < ne; k++)
    {
        MateriaNodus* e = materia_valor_lista_obtinere(*elementa, k)
            ->datum.nodus;

        si (   !tangitur[k] && e->genus == (s32)ORATIO_GENUS_VOCABULUM
            && !_prior_casuum_vocabuli(cursus, scratch, e))
        {
            piscina_destruere(scratch);
            redde FALSUM;
        }
    }
    piscina_destruere(scratch);
    redde VERUM;
}
/* LEX CAPITIS (T31 a, 2026-09-09): ligatio subiecti aut obiecti cuius
 * socius (lectio implens w.b) umbram CAPITIS impletam fert (adiectivum
 * aut determinans substantivo suo ligatum) ad caput illud (w'.b')
 * sequitur - modificator subiectum aut obiectum esse non potest, caput
 * eius est ('Puella bona ambulat': subiectum 'bona' proxima -> 'Puella';
 * 'in magna urbe': obiectum 'magna' -> 'urbe'; directio UD: nsubj et
 * obj/case ad caput nominale). Gradus unus (caput substantivum umbram
 * capitis non fert); post gradus omnes, ante legem umbrarum. Ambitus
 * ORATIO_LEX_CAPITIS=0 abrogat (mensura). */
hic_manens constans b32 LEX_CAPITIS = VERUM;

interior b32
_lex_capitis_activa (
    vacuum)
{
    hic_manens i32 lectum = ZEPHYRUM;
    hic_manens b32 activa = VERUM;

    si (!lectum)
    {
        constans character* ambitus = getenv("ORATIO_LEX_CAPITIS");

        activa = (b32)(LEX_CAPITIS
            && (ambitus == NIHIL || strcmp(ambitus, "0") != ZEPHYRUM));
        lectum = I;
    }
    redde activa;
}

interior b32
_ligationes_ad_caput_sequi (
          Cursus* cursus,
    MateriaNodus* sententia)
{
    constans MateriaValor* elementa =
        &sententia->loci[ORATIO_SENTENTIA_ELEMENTA];
                      i32 ne;
                      i32 j;

    si (   !_lex_capitis_activa()
        || elementa->genus != MATERIA_VALOR_LISTA)
    {
        redde VERUM;
    }
    ne = materia_valor_lista_numerus(*elementa);
    per (j = ZEPHYRUM; j < ne; j++)
    {
        constans MateriaValor* elementum =
            materia_valor_lista_obtinere(*elementa, j);
        constans MateriaValor* analyses;
                          i32  a;

        si (   elementum        == NIHIL
            || elementum->genus != MATERIA_VALOR_NODUS
            || elementum->datum.nodus->genus
                != (s32)ORATIO_GENUS_VOCABULUM)
        {
            perge;
        }
        analyses =
            &elementum->datum.nodus->loci[ORATIO_VOCABULUM_ANALYSES];
        si (analyses->genus != MATERIA_VALOR_LISTA)
        {
            perge;
        }
        per (a = ZEPHYRUM; a < materia_valor_lista_numerus(*analyses);
             a++)
        {
            constans MateriaValor* valor =
                materia_valor_lista_obtinere(*analyses, a);
            constans MateriaValor* umbrae;
                              i32  u;

            si (valor == NIHIL || valor->genus != MATERIA_VALOR_NODUS)
            {
                perge;
            }
            umbrae = _umbrae_lectionis(valor->datum.nodus);
            si (umbrae == NIHIL)
            {
                perge;
            }
            per (u = ZEPHYRUM; u < materia_valor_lista_numerus(*umbrae);
                 u++)
            {
                constans MateriaValor* valor_umbrae =
                    materia_valor_lista_obtinere(*umbrae, u);
                         MateriaNodus* umbra;
                                  s32  relatio;
                                  s32  w;
                                  s32  b;
                constans MateriaValor* socius;
                constans MateriaValor* analyses_socii;
                constans MateriaValor* lectio_socii;
                constans MateriaValor* umbrae_socii;
                                  i32  x;

                si (   valor_umbrae        == NIHIL
                    || valor_umbrae->genus != MATERIA_VALOR_NODUS)
                {
                    perge;
                }
                umbra = valor_umbrae->datum.nodus;
                si (   umbra->loci[ORATIO_UMBRA_RELATIO].genus
                        != MATERIA_VALOR_INDEX
                    || !oratio_referentia_scripta(umbra,
                        (i32)ORATIO_UMBRA_IMPLETIO))
                {
                    perge;
                }
                relatio = umbra->loci[ORATIO_UMBRA_RELATIO].datum.index;
                si (   relatio != (s32)ORATIO_RELATIO_SUBIECTUM
                    && relatio != (s32)ORATIO_RELATIO_OBIECTUM
                    && relatio != (s32)ORATIO_RELATIO_OBIECTUM_VERBI)
                {
                    perge;
                }
                si (!oratio_referentiae_ordinales(umbra,
                        (i32)ORATIO_UMBRA_IMPLETIO, &w, &b))
                {
                    perge;   /* T32 a: petitio revocata */
                }
                si (w < ZEPHYRUM || w >= (s32)ne || b < ZEPHYRUM)
                {
                    perge;
                }
                socius = materia_valor_lista_obtinere(*elementa,
                    (i32)w);
                si (   socius        == NIHIL
                    || socius->genus != MATERIA_VALOR_NODUS
                    || socius->datum.nodus->genus
                        != (s32)ORATIO_GENUS_VOCABULUM)
                {
                    perge;
                }
                analyses_socii =
                    &socius->datum.nodus->loci[ORATIO_VOCABULUM_ANALYSES];
                si (   analyses_socii->genus != MATERIA_VALOR_LISTA
                    || b >= (s32)materia_valor_lista_numerus(
                        *analyses_socii))
                {
                    perge;
                }
                lectio_socii =
                    materia_valor_lista_obtinere(*analyses_socii,
                    (i32)b);
                si (   lectio_socii        == NIHIL
                    || lectio_socii->genus != MATERIA_VALOR_NODUS)
                {
                    perge;
                }
                /* adiectiva sola: determinantia (qui, hic, ille - T19i)
                 * subiecta ipsa sunt, caput eorum antecedens (mensuratum:
                 * cum determinantibus chartae -5/-7, Dante -13) */
                si (oratio_genus_classis((OratioGenus)lectio_socii->datum
                        .nodus->genus) != ORATIO_CLASSIS_ADIECTIVUM)
                {
                    perge;
                }
                umbrae_socii =
                    _umbrae_lectionis(lectio_socii->datum.nodus);
                si (umbrae_socii == NIHIL)
                {
                    perge;
                }
                per (x = ZEPHYRUM;
                     x
                         < materia_valor_lista_numerus(*umbrae_socii); x++)
                {
                    constans MateriaValor* valor_capitis =
                        materia_valor_lista_obtinere(*umbrae_socii, x);
                    constans MateriaNodus* caput;
                                      s32  w2;
                                      s32  b2;

                    si (   valor_capitis        == NIHIL
                        || valor_capitis->genus != MATERIA_VALOR_NODUS)
                    {
                        perge;
                    }
                    caput = valor_capitis->datum.nodus;
                    si (   caput->loci[ORATIO_UMBRA_RELATIO].genus
                            != MATERIA_VALOR_INDEX
                        || caput->loci[ORATIO_UMBRA_RELATIO].datum.index
                            != (s32)ORATIO_RELATIO_CAPUT
                        || !oratio_referentia_scripta(caput,
                            (i32)ORATIO_UMBRA_IMPLETIO))
                    {
                        perge;
                    }
                    oratio_referentiae_ordinales(caput,
                        (i32)ORATIO_UMBRA_IMPLETIO, &w2, &b2);
                    /* caput VICINUM solum: ligationes capitis remotae (gradus
                     * laxus, chartis 39-66 % rectae) secutae subiectum
                     * alio transferunt (mensuratum: chartae -5/-7, Dante -9) */
                    si (   w2 < ZEPHYRUM || w2 >= (s32)ne
                        || w2 == (s32)j
                        || w2 == w || b2 < ZEPHYRUM
                        || (w2 > w ? w2 - w : w - w2) != I)
                    {
                        perge;
                    }
                    /* T33: eundem nodum analysis nominat quem caput */
                    si (!materia_nodus_reponere(umbra,
                            (i32)ORATIO_UMBRA_IMPLETIO,
                            materia_valor_referentia(
                                oratio_referentiae_scopus(caput,
                                    (i32)ORATIO_UMBRA_IMPLETIO)),
                            MATERIA_LOCUS_REFERENTIA))
                    {
                        redde FALSUM;
                    }
                    /* T32 a: arcus nunc legis capitis est - auctor
                     * 'lex-capitis' (tabula fiduciae eum seorsum metitur) */
                    {
                        hic_manens character lex[] = "lex-capitis";

                        si (!_auctorem_umbrae_scribere(cursus,
                            *elementa,
                                umbra, (i32)j,
                                _chorda((i8*)lex,
                                    (i32)(magnitudo(lex) - I))))
                        {
                            redde FALSUM;
                        }
                    }
                    si (cursus->census != NIHIL)
                    {
                        cursus->census->ad_caput_secutae =
                            cursus->census->ad_caput_secutae + I;
                    }
                    frange;
                }
            }
        }
    }
    redde VERUM;
}

/* LEX CARRIER OBIECTI (T31 b, 2026-09-09): ordo obiecti verbi cuius
 * lectio carrier (verbum) lectio PRIMA vocabuli non est recusatur -
 * obiectum verbi classem carrier non decidit (testimonium casus solius
 * debile: homographa 'amor', 'regis', 'canis' verba fiebant, errata
 * classis Senecae substantivum -> verbum CLXXVIII, adiectivum -> verbum
 * LXXVI, primarium 841 -> 815 gradu stricto solo). Subiectum (casus +
 * numerus + persona) classem decidere sinitur. Ambitus
 * ORATIO_OBIECTUM_CARRIER_PRIMUS=0 abrogat (mensura). */
interior b32
_obiectum_carrier_primus (
    vacuum)
{
    hic_manens i32 lectum   = ZEPHYRUM;
    hic_manens b32 activum  = VERUM;

    si (!lectum)
    {
        constans character* ambitus =
            getenv("ORATIO_OBIECTUM_CARRIER_PRIMUS");

        activum = (b32)(ambitus == NIHIL
            || strcmp(ambitus, "0") != ZEPHYRUM);
        lectum = I;
    }
    redde activum;
}

interior b32
_obiectum_socius_lex_activa (
    constans character* titulus)
{
                   hic_manens i32  lectum   = ZEPHYRUM;
    hic_manens constans character* ambitus  = NIHIL;

    si (!lectum)
    {
        ambitus  = getenv("ORATIO_OBIECTUM_SOCIUS");
        lectum   = I;
    }
    /* MENSURATAE 2026-09-09 gradu stricto cum lege carrier (ligatio
     * thesauris VII): classis -1..-3 ubique (revocatio obiecti 47 -> 44 %:
     * promotiones classis rectae quoque recusabat); adpositionis -1..-4
     * (ligationes adpositionum nostrae ipsae falsae: recusatio ex eis
     * peior); subiecti NIHIL mutat (umbra subiecti eodem gradu nondum
     * impleta) - omnes INACTIVAE, ambitu solo */
    si (ambitus == NIHIL)
    {
        redde FALSUM;
    }
    redde (b32)(strcmp(ambitus, titulus) == ZEPHYRUM);
}

interior b32
_obiectum_socius_recusandus (
    constans MateriaValor* elementa,
                      i32  ne,
                      i32  v,
                      i32  a,
                      i32  w,
                      i32  b)
{
    constans MateriaNodus* socius =
        materia_valor_lista_obtinere(*elementa,
        w)->datum.nodus;
    constans MateriaNodus* carrier = materia_valor_lista_obtinere(
        *elementa, v)->datum.nodus;
    constans MateriaValor* analyses =
        &socius->loci[ORATIO_VOCABULUM_ANALYSES];
                      i32 j;

    si (   socius->genus   != (s32)ORATIO_GENUS_VOCABULUM
        || analyses->genus != MATERIA_VALOR_LISTA
        || b               >= materia_valor_lista_numerus(*analyses))
    {
        redde FALSUM;
    }
    /* (I) classis lectionis b == classis lectionis primae socii */
    si (   _obiectum_socius_lex_activa("classis")
        && oratio_genus_classis((OratioGenus)materia_valor_lista_obtinere(
                *analyses, b)->datum.nodus->genus)
            != oratio_genus_classis((OratioGenus)materia_valor_lista_obtinere(
                *analyses, ZEPHYRUM)->datum.nodus->genus))
    {
        redde VERUM;
    }
    /* (III) subiectum eiusdem verbi: umbra subiecti lectionis a carrier
     * in w impleta */
    si (_obiectum_socius_lex_activa("subiecti"))
    {
        constans MateriaValor* analyses_v =
            &carrier->loci[ORATIO_VOCABULUM_ANALYSES];
        constans MateriaValor* umbrae = analyses_v->genus
            == MATERIA_VALOR_LISTA
            && a < materia_valor_lista_numerus(*analyses_v)
            ? _umbrae_lectionis(materia_valor_lista_obtinere(*analyses_v,
            a)
                ->datum.nodus) : NIHIL;
        i32 u;

        per (u = ZEPHYRUM; umbrae != NIHIL
             && u < materia_valor_lista_numerus(*umbrae); u++)
        {
            constans MateriaNodus* umbra = materia_valor_lista_obtinere(
                *umbrae, u)->datum.nodus;

            si (   umbra->loci[ORATIO_UMBRA_RELATIO].genus
                == MATERIA_VALOR_INDEX
                && umbra->loci[ORATIO_UMBRA_RELATIO].datum.index
                    == (s32)ORATIO_RELATIO_SUBIECTUM
                && oratio_referentiae_vocabulum(umbra,
                    (i32)ORATIO_UMBRA_IMPLETIO) == (s32)w)
            {
                redde VERUM;
            }
        }
    }
    /* (II) socius obiectum adpositionis: umbra obiecti (relatio 0)
     * lectionis primae carrier adpositionis cuiusvis in w impleta */
    si (!_obiectum_socius_lex_activa("adpositionis"))
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < ne; j++)
    {
        constans MateriaValor* elementum = materia_valor_lista_obtinere(
            *elementa, j);
        constans MateriaValor* analyses_j;
        constans MateriaNodus* prima;
        constans MateriaValor* umbrae;
                          i32  u;

        si (   j                == v || elementum == NIHIL
            || elementum->genus != MATERIA_VALOR_NODUS
            || elementum->datum.nodus->genus
                != (s32)ORATIO_GENUS_VOCABULUM)
        {
            perge;
        }
        analyses_j =
            &elementum->datum.nodus->loci[ORATIO_VOCABULUM_ANALYSES];
        si (   analyses_j->genus != MATERIA_VALOR_LISTA
            || materia_valor_lista_numerus(*analyses_j) == ZEPHYRUM)
        {
            perge;
        }
        prima = materia_valor_lista_obtinere(*analyses_j,
            ZEPHYRUM)->datum.nodus;
        si (oratio_genus_classis((OratioGenus)prima->genus)
            != ORATIO_CLASSIS_ADPOSITIO)
        {
            perge;
        }
        umbrae = _umbrae_lectionis(prima);
        per (u = ZEPHYRUM; umbrae != NIHIL
             && u < materia_valor_lista_numerus(*umbrae); u++)
        {
            constans MateriaNodus* umbra = materia_valor_lista_obtinere(
                *umbrae, u)->datum.nodus;

            si (   umbra->loci[ORATIO_UMBRA_RELATIO].genus
                == MATERIA_VALOR_INDEX
                && umbra->loci[ORATIO_UMBRA_RELATIO].datum.index
                    == (s32)ORATIO_RELATIO_OBIECTUM
                && oratio_referentiae_vocabulum(umbra,
                    (i32)ORATIO_UMBRA_IMPLETIO) == (s32)w)
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}


/* ==================================================
 * DECRETOR (T38 c, 2026-09-15; decisiones XLVIII, LVII-LIX)
 * ================================================== */

/* Candidata = umbra carrier + lectio implens: impletio stans (causa -I)
 * aut alterna causa fiduciae (recusata, revocata, praeoccupata). Cellula
 * = dependens (directio oraculi, _dependens_ordinis). */
nomen structura {
    MateriaNodus* umbra;
    MateriaNodus* analysis;          /* lectio implens (nodus) */
             i32  v;                 /* carrier */
             i32  a;                 /* lectio carrier */
             i32  w;                 /* implens */
             i32  b;                 /* lectio implentis (ordinalis hodie) */
             i32  dependens;
             i32  caput;             /* vocabulum capitis (alterum) */
          chorda  titulus;           /* regula proponens */
             s32  causa;             /* -I stans | OratioAlternaCausa */
             s32  relatio;
             b32  ante;              /* dependens ante caput */
             i32  distantia;
             s32  numerus_capitis;   /* lectionis carrier; -I */
             s32  numerus_lectionis; /* lectionis implentis; -I */
             s32  pondus;            /* permille; -I nullum */
             s32  pondera[4];        /* per scalam I-IV; -I absens */
             i32  gradus;            /* scala quae respondit; 0 nulla */
             b32  superstes;         /* constrictis superstes */
             b32  lectionis;         /* contentio LECTIONIS: capita eadem in
                                      * cellula - fiducia tacet (R19, T32 f) */
             b32 accepta;
} Candidatus;

hic_manens character DECRETOR_TITULUS[] = "decretor";

/* ORATIO_DECRETOR=1 decretorem accendit; absens aut 0 = exsecutor solus
 * (T38 c: ordinarium OFF donec cursus T38 d pinnas decernat) */
interior b32
_decretor_activus (vacuum)
{
    constans character* ambitus = getenv("ORATIO_DECRETOR");

    redde (b32)(ambitus != NIHIL && strcmp(ambitus, "0") != ZEPHYRUM);
}

/* ORATIO_PONDERA_SINE=<folliculus>: folliculus unus omissus (mensura) */
interior b32
_folliculus_omissus (
    constans character* folliculus)
{
    constans character* ambitus = getenv("ORATIO_PONDERA_SINE");

    redde (b32)(ambitus != NIHIL && strcmp(ambitus, folliculus)
        == ZEPHYRUM);
}

/* numerus lectionis (locus 'numerus' generis eius); -I absens */
interior s32
_numerus_lectionis (
    constans MateriaNodus* analysis)
{
    OratioClassis classis;
              s32 locus;

    si (analysis == NIHIL)
    {
        redde (s32)-I;
    }
    classis = oratio_genus_classis((OratioGenus)analysis->genus);
    si ((i32)classis >= (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM)
    {
        redde (s32)-I;
    }
    locus = oratio_partes_locus(classis, "numerus");
    si (   locus < ZEPHYRUM
        || analysis->loci[locus].genus != MATERIA_VALOR_INDEX)
    {
        redde (s32)-I;
    }
    redde analysis->loci[locus].datum.index;
}

/* titulus regulae ex loco TOKEN (auctor umbrae aut alternae); vacuus */
interior chorda
_titulus_loci (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    constans MateriaValor* v = &nodus->loci[locus];

    si (v->genus == MATERIA_VALOR_TOKEN && v->datum.token != NIHIL)
    {
        redde v->datum.token->valor;
    }
    redde _chorda(NIHIL, ZEPHYRUM);
}

/* vocabulum ordinalis inter elementa (NIHIL extra fines aut aliud) */
interior MateriaNodus*
_vocabulum_ordinalis (
    MateriaValor elementa,
             i32 k)
{
    constans MateriaValor* e;

    si (k >= materia_valor_lista_numerus(elementa))
    {
        redde NIHIL;
    }
    e = materia_valor_lista_obtinere(elementa, k);
    si (   e == NIHIL || e->genus != MATERIA_VALOR_NODUS
        || e->datum.nodus->genus != (s32)ORATIO_GENUS_VOCABULUM)
    {
        redde NIHIL;
    }
    redde e->datum.nodus;
}

/* candidatam addere nisi eadem (umbra, lectio) iam adest (repetita) */
interior b32
_candidatum_addere (
             Xar* candidati,
    MateriaValor  elementa,
    MateriaNodus* umbra,
             i32  v,
             i32  a,
    MateriaNodus* lectio_carrier,
    MateriaNodus* analysis,
             i32  w,
             i32  b,
          chorda  titulus,
             s32  causa)
{
    Candidatus* c;
           i32  k;

    per (k = ZEPHYRUM; k < xar_numerus(candidati); k++)
    {
        constans Candidatus* d =
            (constans Candidatus*)xar_obtinere(candidati, k);

        si (d->umbra == umbra && d->analysis == analysis)
        {
            redde VERUM;
        }
    }
    c = (Candidatus*)xar_addere(candidati);
    si (c == NIHIL)
    {
        redde FALSUM;
    }
    memset(c, ZEPHYRUM, magnitudo(*c));
    c->umbra     = umbra;
    c->analysis  = analysis;
    c->v         = v;
    c->a         = a;
    c->w         = w;
    c->b         = b;
    c->titulus   = titulus;
    c->causa     = causa;
    c->dependens          = _dependens_ordinis(elementa, v, a, umbra,
        w);
    c->caput              = c->dependens == v ? w : v;
    c->relatio            = umbra->loci[ORATIO_UMBRA_RELATIO].genus
        == MATERIA_VALOR_INDEX
            ? umbra->loci[ORATIO_UMBRA_RELATIO].datum.index : (s32)-I;
    c->ante               = (b32)(c->dependens < c->caput);
    c->distantia          = c->caput > c->dependens
        ? c->caput - c->dependens : c->dependens - c->caput;
    c->numerus_capitis    = _numerus_lectionis(lectio_carrier);
    c->numerus_lectionis  = _numerus_lectionis(analysis);
    c->pondus             = (s32)-I;
    redde VERUM;
}

/* candidatae omnes sententiae: per vocabulum, lectionem, umbram -
 * impletio stans et alternae (decretae exceptae: cursus alter nihil
 * mutat) */
interior b32
_candidatos_colligere (
             Xar* candidati,
    MateriaValor  elementa)
{
    i32 ne = materia_valor_lista_numerus(elementa);
    i32 v;

    per (v = ZEPHYRUM; v < ne; v++)
    {
                 MateriaNodus* vocabulum =
                     _vocabulum_ordinalis(elementa,
                     v);
        constans MateriaValor* analyses;
                          i32  a;

        si (vocabulum == NIHIL)
        {
            perge;
        }
        analyses = &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
        si (analyses->genus != MATERIA_VALOR_LISTA)
        {
            perge;
        }
        per (a = ZEPHYRUM; a < materia_valor_lista_numerus(*analyses);
            a++)
        {
                     MateriaNodus* lectio =
                         materia_valor_lista_obtinere(*analyses, a)
                         ->datum.nodus;
                     OratioClassis classis =
                         oratio_genus_classis((OratioGenus)lectio->genus);
                               s32 locus =
                                   (i32)classis
                                   < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
                                   ? oratio_partes_locus(classis,
                                       "umbrae")
                                   : (s32)-I;
            constans MateriaValor* umbrae;
                              i32  u;

            si (   locus < ZEPHYRUM
                || lectio->loci[locus].genus != MATERIA_VALOR_LISTA)
            {
                perge;
            }
            umbrae = &lectio->loci[locus];
            per (u = ZEPHYRUM; u < materia_valor_lista_numerus(*umbrae);
                u++)
            {
                         MateriaNodus* umbra =
                             materia_valor_lista_obtinere(*umbrae, u)
                             ->datum.nodus;
                constans MateriaValor* alternae;
                                  s32  w;
                                  s32  b;
                                  i32  x;

                si (   oratio_referentiae_ordinales(umbra,
                        (i32)ORATIO_UMBRA_IMPLETIO, &w, &b)
                    && w >= ZEPHYRUM && w < (s32)ne)
                {
                    MateriaNodus* scopus =
                        oratio_referentiae_scopus(umbra,
                        (i32)ORATIO_UMBRA_IMPLETIO);

                    si (   scopus != NIHIL
                        && !_candidatum_addere(candidati, elementa,
                            umbra,
                            v, a, lectio, scopus, (i32)w, (i32)b,
                            _titulus_loci(umbra,
                                (i32)ORATIO_UMBRA_AUCTOR),
                            (s32)-I))
                    {
                        redde FALSUM;
                    }
                }
                alternae = &umbra->loci[ORATIO_UMBRA_ALTERNAE];
                si (alternae->genus != MATERIA_VALOR_LISTA)
                {
                    perge;
                }
                per (x = ZEPHYRUM;
                    x < materia_valor_lista_numerus(*alternae); x++)
                {
                    constans MateriaValor* av =
                        materia_valor_lista_obtinere(*alternae, x);
                             MateriaNodus* alterna;
                             MateriaNodus* scopus;
                                      s32  causa;

                    si (av == NIHIL || av->genus != MATERIA_VALOR_NODUS)
                    {
                        perge;
                    }
                    alterna = av->datum.nodus;
                    scopus  = oratio_referentiae_scopus(alterna,
                        (i32)ORATIO_ALTERNA_SOCIUS);
                    causa   = alterna->loci[ORATIO_ALTERNA_CAUSA].genus
                        == MATERIA_VALOR_INDEX
                            ? alterna->loci[ORATIO_ALTERNA_CAUSA].datum.index
                            : (s32)-I;
                    si (   scopus == NIHIL || causa < ZEPHYRUM
                        || causa  == (s32)ORATIO_ALTERNA_CAUSA_DECRETA
                        || !oratio_referentiae_ordinales(alterna,
                            (i32)ORATIO_ALTERNA_SOCIUS, &w, &b)
                        || w < ZEPHYRUM || w >= (s32)ne)
                    {
                        perge;
                    }
                    si (!_candidatum_addere(candidati, elementa, umbra,
                        v,
                        a, lectio, scopus, (i32)w, (i32)b,
                        _titulus_loci(alterna,
                            (i32)ORATIO_ALTERNA_AUCTOR),
                        causa))
                    {
                        redde FALSUM;
                    }
                }
            }
        }
    }
    redde VERUM;
}

/* ordo tabulae sub contextu (dialectus + forma, dialectus, forma, '-')
 * qui limen attingit; -I nullus */
interior s32
_pondus_ordinis (
     constans Cursus* cursus,
              chorda  regula,
  constans character* dialectus,
  constans character* forma,
  constans character* folliculus,
  constans character* valor,
                 i32  limen)
{
    constans character* dd[4];
    constans character* ff[4];
                   i32  i;

    dd[0] = dialectus;   ff[0] = forma;
    dd[1] = dialectus;   ff[1] = "-";
    dd[2] = "-";         ff[2] = forma;
    dd[3] = "-";         ff[3] = "-";
    per (i = ZEPHYRUM; i < (i32)IV; i++)
    {
        constans OratioPondus* p = oratio_resolutio_pondus(
            cursus->programma, regula, dd[i], ff[i], folliculus, valor);

        si (p != NIHIL && p->contentiones >= limen)
        {
            redde (s32)p->permille;
        }
    }
    redde (s32)-I;
}

/* SCALA (decisio LVII) per candidatam: pondera[0] folliculus relationis
 * (numerus-capitis, positio-numerus stellae; directio-distantia capitis),
 * [1] regula plana, [2] structura '@relatio+ante|post+proximo|remoto',
 * [3] fiducia (auctores.tsv); quaeque sub contextu declarato regressu;
 * -I absens. Gradus electus per CELLULAM (_decretor): populi diversi
 * comparari nequeunt. */
interior vacuum
_pondera_candidati (
    constans Cursus* cursus,
         Candidatus* c)
{
    constans character* d = "-";
    constans character* f = "-";
             character  valor[64];
             character  clavis_structurae[96];
                   i32  limen = oratio_resolutio_pondera_limen();
                   i32  r;

    per (r = ZEPHYRUM; r < (i32)IV; r++)
    {
        c->pondera[r] = (s32)-I;
    }
    si (c->titulus.mensura == ZEPHYRUM)
    {
        redde;
    }
    si (cursus->contextus != NIHIL)
    {
        si (   cursus->contextus->dialectus
                == (s32)ORATIO_DIALECTUS_CLASSICUS
            || cursus->contextus->dialectus
                == (s32)ORATIO_DIALECTUS_MEDIUS)
        {
            d = ORATIO_TITULI_DIALECTORUM[cursus->contextus->dialectus];
        }
        si (   cursus->contextus->forma == (s32)ORATIO_FORMA_PROSA
            || cursus->contextus->forma == (s32)ORATIO_FORMA_VERSUS)
        {
            f =
                oratio_forma_titulus((OratioForma)cursus->contextus->forma);
        }
    }
    /* I folliculi */
    si (   c->relatio == (s32)ORATIO_RELATIO_SUBIECTUM
        || c->relatio == (s32)ORATIO_RELATIO_OBIECTUM_VERBI)
    {
        si (   c->numerus_capitis >= ZEPHYRUM
            && c->numerus_capitis
                < (s32)ORATIO_NUMERUS_GRAMMATICUS_NUMERUS
            && !_folliculus_omissus("numerus-capitis"))
        {
            c->pondera[ZEPHYRUM] = _pondus_ordinis(cursus, c->titulus,
                d,
                f, "numerus-capitis",
                ORATIO_TITULI_NUMERORUM[c->numerus_capitis], limen);
        }
        si (   c->pondera[ZEPHYRUM] < ZEPHYRUM
            && c->numerus_lectionis >= ZEPHYRUM
            && c->numerus_lectionis
                < (s32)ORATIO_NUMERUS_GRAMMATICUS_NUMERUS
            && !_folliculus_omissus("positio-numerus"))
        {
            sprintf(valor, "%d+%s", c->ante ? 1 : 0,
                ORATIO_TITULI_NUMERORUM[c->numerus_lectionis]);
            c->pondera[ZEPHYRUM] = _pondus_ordinis(cursus, c->titulus,
                d,
                f, "positio-numerus", valor, limen);
        }
    }
    alioquin si (   c->relatio == (s32)ORATIO_RELATIO_CAPUT
                 && c->distantia > ZEPHYRUM
                 && !_folliculus_omissus("directio-distantia"))
    {
        sprintf(valor, "%s+%s", c->ante ? "ante" : "post",
            c->distantia == I ? "1" : "plures");
        c->pondera[ZEPHYRUM] = _pondus_ordinis(cursus, c->titulus, d, f,
            "directio-distantia", valor, limen);
    }
    /* II regula plana */
    c->pondera[I] = _pondus_ordinis(cursus, c->titulus, d, f, "-", "-",
        limen);
    /* III structura */
    si (   c->relatio >= ZEPHYRUM
        && c->relatio < (s32)ORATIO_RELATIO_NUMERUS
        && c->distantia > ZEPHYRUM)
    {
        sprintf(clavis_structurae, "@%s+%s+%s",
            ORATIO_TITULI_RELATIONUM[c->relatio],
            c->ante ? "ante" : "post",
            c->distantia == I ? "proximo" : "remoto");
        c->pondera[(i32)II] = _pondus_ordinis(cursus,
            _chorda((i8*)clavis_structurae,
                (i32)strlen(clavis_structurae)), d, f, "-", "-", limen);
    }
    /* IV fiducia (arcuum solum: contentio lectionis eam non habet, R19) */
    c->pondera[(i32)III] = _fiducia_regulae(cursus->programma,
        c->titulus);
}

/* an caput -> ... -> dependens per capita accepta (circuitus) */
interior b32
_cyclum_facit (
    constans s32* capita,
             i32  ne,
             i32  caput,
             i32  dependens)
{
    s32 x = (s32)caput;
    i32 gressus;

    per (gressus = ZEPHYRUM; gressus <= ne && x >= ZEPHYRUM
        && x < (s32)ne; gressus++)
    {
        si (x == (s32)dependens)
        {
            redde VERUM;
        }
        x = capita[x];
    }
    redde FALSUM;
}

/* an candidata alia (accepta) umbram eandem teneat */
interior b32
_umbra_capta (
             constans Xar* candidati,
    constans MateriaNodus* umbra,
                      i32  praeter)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(candidati); k++)
    {
        constans Candidatus* d =
            (constans Candidatus*)xar_obtinere(candidati, k);

        si (k != praeter && d->accepta && d->umbra == umbra)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* lectionem primam vocabuli w facere (permutatio ut gradus: analysis
 * praelata prima, ceterae ordine suo; compendia reposita) */
interior b32
_lectionem_promovere (
                   Cursus* cursus,
             MateriaValor  elementa,
                      i32  w,
    constans MateriaNodus* analysis)
{
             MateriaNodus* vocabulum = _vocabulum_ordinalis(elementa,
                 w);
    constans MateriaValor* analyses;
                      i32  n;
                      i32  b = (i32)-I;
                      i32* ordo;
                      i32  i;
                      i32  j;

    si (vocabulum == NIHIL)
    {
        redde VERUM;
    }
    analyses = &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
    si (analyses->genus != MATERIA_VALOR_LISTA)
    {
        redde VERUM;
    }
    n = materia_valor_lista_numerus(*analyses);
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (materia_valor_lista_obtinere(*analyses, i)->datum.nodus
            == analysis)
        {
            b = i;
        }
    }
    si (b <= ZEPHYRUM)
    {
        redde VERUM;   /* absens aut iam prima */
    }
    ordo = (i32*)piscina_allocare(cursus->piscina, (memoriae_index)n
        * (memoriae_index)magnitudo(i32));
    si (ordo == NIHIL)
    {
        redde FALSUM;
    }
    ordo[ZEPHYRUM]  = b;
    j               = I;
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (i != b)
        {
            ordo[j]  = i;
            j        = j + I;
        }
    }
    redde materia_nodus_lista_permutare(cursus->piscina, vocabulum,
        (i32)ORATIO_VOCABULUM_ANALYSES, ordo, n)
        && oratio_partes_compendia_reponere(cursus->piscina, vocabulum);
}

/* habitus in vocabulum scribere (INDEX; scriptum reponitur) */
interior b32
_habitum_scribere (
    MateriaNodus* vocabulum,
             s32  habitus)
{
    b32 scriptum = vocabulum->loci[ORATIO_VOCABULUM_HABITUS].genus
        != MATERIA_VALOR_NIHIL;

    redde scriptum
        ? materia_nodus_reponere(vocabulum,
            (i32)ORATIO_VOCABULUM_HABITUS,
            materia_valor_index(habitus), MATERIA_LOCUS_INDEX)
        : materia_nodus_ponere(vocabulum, (i32)ORATIO_VOCABULUM_HABITUS,
            materia_valor_index(habitus), MATERIA_LOCUS_INDEX);
}

/* petitionem stantem candidatae (umbra, lectio) deponere: alterna DECRETA
 * (victor decretor) et referentia NIHIL (absentia scripta) nisi umbra
 * lectionem aliam iam accepit (tunc alterna sola) */
interior b32
_petitionem_deponere (
                 Cursus* cursus,
           MateriaValor  elementa,
    constans Candidatus* s,
                    b32  revocare)
{
    si (!_alternam_addere(cursus, elementa, s->umbra, s->w, s->b,
        s->titulus, _chorda((i8*)DECRETOR_TITULUS, (i32)VIII),
        (s32)ORATIO_ALTERNA_CAUSA_DECRETA,
        _fiducia_regulae(cursus->programma, s->titulus)))
    {
        redde FALSUM;
    }
    si (!revocare)
    {
        redde VERUM;
    }
    redde materia_nodus_reponere(s->umbra, (i32)ORATIO_UMBRA_IMPLETIO,
        materia_valor_referentia(NIHIL), MATERIA_LOCUS_REFERENTIA);
}

/* DECRETOR: candidatae collectae, cellulae fixae (petitiones stantes non
 * contestatae), pondera, decodatio avida sub constrictis (umbra una,
 * caput unum, sine circuitu, lex classis explicitae), habitus per
 * cellulam, rescriptio, decreta in censum. FALSUM = memoria. */
interior b32
_decretor (
          Cursus* cursus,
    MateriaNodus* sententia,
             b32* explicita,
         Decisio* decisiones)
{
    constans MateriaValor* elementa =
        &sententia->loci[ORATIO_SENTENTIA_ELEMENTA];
                      i32  ne = materia_valor_lista_numerus(*elementa);
                      Xar* candidati;
                      i32  nc;
                      i32* cellae;
                      s32* capita;
                      i32* ordo;
                      i32  k;
                      i32  i;

    candidati = xar_creare(cursus->piscina, (i32)magnitudo(Candidatus));
    si (   candidati == NIHIL
        || !_candidatos_colligere(candidati, *elementa))
    {
        redde FALSUM;
    }
    nc = xar_numerus(candidati);
    si (nc == ZEPHYRUM)
    {
        redde VERUM;
    }
    cellae = (i32*)piscina_allocare(cursus->piscina, (memoriae_index)ne
        * (memoriae_index)magnitudo(i32));
    capita = (s32*)piscina_allocare(cursus->piscina, (memoriae_index)ne
        * (memoriae_index)magnitudo(s32));
    ordo   = (i32*)piscina_allocare(cursus->piscina, (memoriae_index)nc
        * (memoriae_index)magnitudo(i32));
    si (cellae == NIHIL || capita == NIHIL || ordo == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < ne; k++)
    {
        cellae[k] = ZEPHYRUM;
        capita[k] = (s32)-I;
    }
    per (k = ZEPHYRUM; k < nc; k++)
    {
        constans Candidatus* c =
            (constans Candidatus*)xar_obtinere(candidati, k);

        si (c->dependens < ne)
        {
            cellae[c->dependens] = cellae[c->dependens] + I;
        }
    }
    /* contentio LECTIONIS (T32 f): candidatae cellulae caput idem
     * petunt - fiducia arcuum eam iudicare nequit (R19): tabula sola
     * (gradus I-III) respondet, aliter petitio stans manet */
    per (k = ZEPHYRUM; k < nc; k++)
    {
        Candidatus* c = (Candidatus*)xar_obtinere(candidati, k);
               i32  j;

        c->lectionis = VERUM;
        per (j = ZEPHYRUM; j < nc; j++)
        {
            constans Candidatus* d =
                (constans Candidatus*)xar_obtinere(candidati, j);

            si (d->dependens == c->dependens && d->caput != c->caput)
            {
                c->lectionis = FALSUM;
            }
        }
    }
    /* cellulae fixae: petitio stans non contestata manet, umbra capta */
    per (k = ZEPHYRUM; k < nc; k++)
    {
        Candidatus* c = (Candidatus*)xar_obtinere(candidati, k);

        si (   c->dependens < ne && cellae[c->dependens] < (i32)II
            && c->causa < ZEPHYRUM)
        {
            c->accepta            = VERUM;
            c->superstes          = VERUM;
            capita[c->dependens]  = (s32)c->caput;
        }
    }
    /* pondera per scalam (I-IV) cuique candidatae non fixae */
    per (k = ZEPHYRUM; k < nc; k++)
    {
        Candidatus* c = (Candidatus*)xar_obtinere(candidati, k);

        si (!c->accepta)
        {
            _pondera_candidati(cursus, c);
        }
    }
    /* GRADUS UNUS PER CELLULAM: candidatae cellulae eiusdem gradu
     * finissimo qui OMNIBUS respondet aestimantur - populi diversi
     * comparari nequeunt (ordo tabulae contentionem utrique lateri
     * numerat etiam neutro recto, fiducia arcus omnes numerat);
     * contentio lectionis gradum IV non habet (R19); nullus gradus =
     * pondera nulla, petitio stans manet */
    per (k = ZEPHYRUM; k < ne; k++)
    {
        i32 r;

        si (cellae[k] < (i32)II)
        {
            perge;
        }
        per (r = ZEPHYRUM; r < (i32)IV; r++)
        {
            b32 omnes = VERUM;
            i32 j;

            per (j = ZEPHYRUM; j < nc; j++)
            {
                constans Candidatus* c =
                    (constans Candidatus*)xar_obtinere(candidati, j);

                si (   c->dependens == k && !c->accepta
                    && (   c->pondera[r] < ZEPHYRUM
                        || (r == (i32)III && c->lectionis)))
                {
                    omnes = FALSUM;
                }
            }
            si (omnes)
            {
                per (j = ZEPHYRUM; j < nc; j++)
                {
                    Candidatus* c = (Candidatus*)xar_obtinere(candidati,
                        j);

                    si (c->dependens == k && !c->accepta)
                    {
                        c->pondus = c->pondera[r];
                        c->gradus = r + I;
                    }
                }
                frange;
            }
        }
    }
    /* ordo (insertio stabilis: pondus descendens, stans prior inter
     * paria, deinde ordo collectionis) */
    i = ZEPHYRUM;
    per (k = ZEPHYRUM; k < nc; k++)
    {
         Candidatus* c = (Candidatus*)xar_obtinere(candidati, k);
                i32  j;

        si (c->accepta)
        {
            perge;
        }
        j = i;
        dum (j > ZEPHYRUM)
        {
            constans Candidatus* prior =
                (constans Candidatus*)xar_obtinere(candidati,
                ordo[j - I]);

            si (   prior->pondus > c->pondus
                || (   prior->pondus == c->pondus
                    && (prior->causa < ZEPHYRUM
                    || c->causa >= ZEPHYRUM)))
            {
                frange;
            }
            ordo[j]  = ordo[j - I];
            j        = j - I;
        }
        ordo[j]  = k;
        i        = i + I;
    }
    /* decodatio avida */
    per (k = ZEPHYRUM; k < i; k++)
    {
        Candidatus* c = (Candidatus*)xar_obtinere(candidati, ordo[k]);
               b32  liber;

        si (c->dependens >= ne)
        {
            perge;
        }
        liber = (b32)(   !_umbra_capta(candidati, c->umbra, ordo[k])
                      && !_cyclum_facit(capita, ne, c->caput,
                          c->dependens)
                      && !(explicita[c->w] && c->b != ZEPHYRUM));
        c->superstes = liber;
        si (liber && capita[c->dependens] < ZEPHYRUM)
        {
            c->accepta            = VERUM;
            capita[c->dependens]  = (s32)c->caput;
        }
    }
    /* per cellulam: habitus, rescriptio, decretum */
    per (k = ZEPHYRUM; k < ne; k++)
    {
        Candidatus* electa       = NIHIL;
        Candidatus* cedens       = NIHIL;
               i32  superstites  = ZEPHYRUM;
               s32  habitus;
               b32  mutata;
      MateriaNodus* vocabulum  = _vocabulum_ordinalis(*elementa, k);
               i32  j;

        si (vocabulum == NIHIL || cellae[k] == ZEPHYRUM)
        {
            perge;
        }
        per (j = ZEPHYRUM; j < nc; j++)
        {
            Candidatus* c = (Candidatus*)xar_obtinere(candidati, j);

            si (c->dependens != k)
            {
                perge;
            }
            si (c->superstes)
            {
                superstites = superstites + I;
            }
            si (c->accepta)
            {
                electa = c;
            }
            alioquin si (   c->superstes
                         && (   cedens == NIHIL
                             || c->pondus > cedens->pondus
                             || (   c->pondus == cedens->pondus
                                 && c->causa < ZEPHYRUM)))
            {
                cedens = c;
            }
        }
        mutata = (b32)(electa != NIHIL && electa->causa >= ZEPHYRUM);
        si (cellae[k] < (i32)II && !mutata)
        {
            perge;   /* cellula non contestata, intacta */
        }
        si (superstites == I && electa != NIHIL)
        {
            habitus = (s32)ORATIO_HABITUS_COACTUS;
        }
        alioquin si (   electa != NIHIL && cedens != NIHIL
                     && electa->pondus > cedens->pondus)
        {
            habitus = (s32)ORATIO_HABITUS_ORDINATUS;
        }
        alioquin
        {
            habitus = (s32)ORATIO_HABITUS_APERTUS;
        }
        si (mutata)
        {
            MateriaNodus* prior =
                oratio_referentiae_scopus(electa->umbra,
                (i32)ORATIO_UMBRA_IMPLETIO);

            /* petitio stans umbrae electae deponitur (alterna decreta),
             * lectio electa impletio fit, regula auctor */
            per (j = ZEPHYRUM; j < nc; j++)
            {
                constans Candidatus* s =
                    (constans Candidatus*)xar_obtinere(candidati, j);

                si (   s->causa < ZEPHYRUM && s->umbra == electa->umbra
                    && s->analysis == prior && prior != NIHIL
                    && !_petitionem_deponere(cursus, *elementa, s,
                        FALSUM))
                {
                    redde FALSUM;
                }
            }
            si (   !(oratio_referentia_scripta(electa->umbra,
                    (i32)ORATIO_UMBRA_IMPLETIO)
                    ? materia_nodus_reponere(electa->umbra,
                        (i32)ORATIO_UMBRA_IMPLETIO,
                        materia_valor_referentia(electa->analysis),
                        MATERIA_LOCUS_REFERENTIA)
                    : materia_nodus_ponere(electa->umbra,
                        (i32)ORATIO_UMBRA_IMPLETIO,
                        materia_valor_referentia(electa->analysis),
                        MATERIA_LOCUS_REFERENTIA))
                || !_auctorem_umbrae_scribere(cursus, *elementa,
                    electa->umbra, electa->v, electa->titulus)
                || !_lectionem_promovere(cursus, *elementa, electa->w,
                    electa->analysis))
            {
                redde FALSUM;
            }
            /* petitiones stantes ALIAE huius cellulae (caput alterum)
             * deponuntur et revocantur: caput unum per dependentem */
            per (j = ZEPHYRUM; j < nc; j++)
            {
                constans Candidatus* s =
                    (constans Candidatus*)xar_obtinere(candidati, j);

                si (   s->dependens == k && s->causa < ZEPHYRUM
                    && s->umbra     != electa->umbra
                    && oratio_referentiae_scopus(s->umbra,
                        (i32)ORATIO_UMBRA_IMPLETIO) == s->analysis
                    && !_petitionem_deponere(cursus, *elementa, s,
                        VERUM))
                {
                    redde FALSUM;
                }
            }
            decisiones[k].genus   = (s32)ORATIO_DECISIO_DECRETUM;
            decisiones[k].auctor  = _chorda((i8*)DECRETOR_TITULUS,
                (i32)VIII);
            explicita[k]          = VERUM;   /* lex umbrarum eam relinquit */
        }
        si (!_habitum_scribere(vocabulum, habitus))
        {
            redde FALSUM;
        }
        si (cursus->census != NIHIL)
        {
            OratioDecretum* d;

            si (cellae[k] >= (i32)II)
            {
                cursus->census->cellae_contestatae =
                    cursus->census->cellae_contestatae + I;
            }
            cursus->census->habitus[habitus] =
                cursus->census->habitus[habitus] + I;
            si (mutata)
            {
                cursus->census->decretae = cursus->census->decretae + I;
            }
            si (cursus->census->decreta == NIHIL)
            {
                cursus->census->decreta = xar_creare(cursus->piscina,
                    (i32)magnitudo(OratioDecretum));
                si (cursus->census->decreta == NIHIL)
                {
                    redde FALSUM;
                }
            }
            d = (OratioDecretum*)xar_addere(cursus->census->decreta);
            si (d == NIHIL)
            {
                redde FALSUM;
            }
            memset(d, ZEPHYRUM, magnitudo(*d));
            d->dependens          = vocabulum;
            d->habitus            = habitus;
            d->mutata             = mutata;
            d->pondus_electae     = (s32)-I;
            d->pondus_cedentis    = (s32)-I;
            d->numerus_capitis    = (s32)-I;
            d->numerus_lectionis  = (s32)-I;
            d->relatio            = (s32)-I;
            si (electa != NIHIL)
            {
                d->caput             = _vocabulum_ordinalis(*elementa,
                    electa->caput);
                d->umbra_electa       = electa->umbra;
                d->analysis_electa    = electa->analysis;
                d->regula_electa      = electa->titulus;
                d->relatio            = electa->relatio;
                d->ante               = electa->ante;
                d->numerus_capitis    = electa->numerus_capitis;
                d->numerus_lectionis  = electa->numerus_lectionis;
                d->distantia          = electa->distantia;
                d->gradus_electae     = electa->gradus;
                d->pondus_electae     = electa->pondus;
            }
            si (cedens != NIHIL)
            {
                d->umbra_cedens     = cedens->umbra;
                d->caput_cedentis    = _vocabulum_ordinalis(*elementa,
                    cedens->caput);
                d->analysis_cedens  = cedens->analysis;
                d->regula_cedens    = cedens->titulus;
                d->gradus_cedentis  = cedens->gradus;
                d->pondus_cedentis  = cedens->pondus;
                si (electa == NIHIL)
                {
                    d->relatio            = cedens->relatio;
                    d->ante               = cedens->ante;
                    d->numerus_capitis    = cedens->numerus_capitis;
                    d->numerus_lectionis  = cedens->numerus_lectionis;
                    d->distantia          = cedens->distantia;
                }
            }
        }
    }
    redde VERUM;
}

interior b32
_sententiam_resolvere (

          Cursus* cursus,
    MateriaNodus* sententia)
{
    constans MateriaValor* elementa =
        &sententia->loci[ORATIO_SENTENTIA_ELEMENTA];
                                            i32  ne;
                                            i32  regulae_numerus;
                                            i32  gradus_maximus = I;
                                            i32  gradus;
                                            i32  k;
                                                                                        b32* vindicata;
                                            b32* explicita;
                                            b32* emendata;
                                        Decisio* decisiones;
                                        Petitio* capita_data;   /* T32 a */

        si (cursus->census != NIHIL)
        {
        cursus->census->sententiae = cursus->census->sententiae + I;
        }

    si (   elementa->genus                        != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*elementa) == ZEPHYRUM)
    {
        redde VERUM;
    }
        /* T20a: CLAUSULAE ante gradus omnes - stampa seminum (strata I-III:
     * lectiones adsunt, ligationes nondum); idempotens */
        si (!oratio_clausulas_seminare(cursus->piscina, sententia,
            cursus->lingua,
            cursus->census != NIHIL ? &cursus->census->clausulae
                : NIHIL))
        {
        redde FALSUM;
        }
    ne               = materia_valor_lista_numerus(*elementa);
    regulae_numerus  = xar_numerus(cursus->programma->regulae);
    si (   cursus->regulae_numerus >= ZEPHYRUM
        && cursus->regulae_numerus < (s32)regulae_numerus)
    {
        regulae_numerus = (i32)cursus->regulae_numerus;
    }
    per (k = ZEPHYRUM; k < regulae_numerus; k++)
    {
        constans OratioRegula* r =
            (constans OratioRegula*)xar_obtinere(
            cursus->programma->regulae, k);

        si (r->gradus > gradus_maximus)
        {
            gradus_maximus = r->gradus;
        }
    }
        vindicata = (b32*)piscina_allocare(cursus->piscina,
            (memoriae_index)ne * (memoriae_index)magnitudo(b32));
    explicita = (b32*)piscina_allocare(cursus->piscina,
        (memoriae_index)ne * (memoriae_index)magnitudo(b32));
        decisiones = (Decisio*)piscina_allocare(cursus->piscina,
            (memoriae_index)ne * (memoriae_index)magnitudo(Decisio));
        emendata = (b32*)piscina_allocare(cursus->piscina,
            (memoriae_index)ne
            * (memoriae_index)magnitudo(b32));
    capita_data = (Petitio*)piscina_allocare(cursus->piscina,
        (memoriae_index)ne * (memoriae_index)magnitudo(Petitio));
    si (   vindicata == NIHIL || explicita == NIHIL || emendata == NIHIL
        || decisiones == NIHIL || capita_data == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < ne; k++)
    {
                capita_data[k].umbra  = NIHIL;
                capita_data[k].fides  = (s32)-I;
                capita_data[k].caput  = (s32)-I;
                vindicata[k]          = FALSUM;
        explicita[k]                  = FALSUM;
        emendata[k]                   = FALSUM;
        decisiones[k].genus           = (s32)-I;
        decisiones[k].auctor.datum    = NIHIL;
        decisiones[k].auctor.mensura  = ZEPHYRUM;
    }
    per (gradus = I; gradus <= gradus_maximus; gradus++)
    {
                                si (!_sententiam_resolvere_gradu(cursus,
                                    sententia,
                                    gradus,
                                    vindicata, explicita, emendata,
                                    decisiones, capita_data))
                                {
            redde FALSUM;
                                }
    }
        /* lex umbrarum, deinde decisiones in arborem (T19g); T20b: catena
     * clausularum post ligationes omnes (stratum IV) */
    /* T38 c DECRETOR (decisio XLVIII): post gradus omnes, ante legem
     * umbrarum, legem capitis et priorem casuum - proposita eligit,
     * regulas numquam accendit; ORATIO_DECRETOR=0 abrogat */
    si (   _decretor_activus()
        && !_decretor(cursus, sententia, explicita, decisiones))
    {
        redde FALSUM;
    }
    redde _ligationes_ad_caput_sequi(cursus, sententia)
        && _umbris_ordinare(cursus, sententia, explicita, decisiones)
        && _decisiones_scribere(cursus, sententia, decisiones)
        && oratio_clausulas_propagare(cursus->piscina, sententia,
            cursus->lingua,
            cursus->census != NIHIL ? &cursus->census->clausulae
                : NIHIL)
        && (!PRIOR_CASUUM || !cursus->prior_activus
        || _prior_casuum(cursus, sententia, decisiones));
}

interior b32
_sententias_resolvere (
          Cursus* cursus,
    MateriaNodus* nodus)
{
    constans MateriaValor* lista;
                      i32  n;
                      i32  k;

    si (nodus->genus == (s32)ORATIO_GENUS_SENTENTIA)
    {
        redde _sententiam_resolvere(cursus, nodus);
    }
    si (nodus->genus == (s32)ORATIO_GENUS_DOCUMENTUM)
    {
        lista = &nodus->loci[ORATIO_DOCUMENTUM_PARAGRAPHI];
    }
    alioquin si (nodus->genus == (s32)ORATIO_GENUS_PARAGRAPHUS)
    {
        lista = &nodus->loci[ORATIO_PARAGRAPHUS_SENTENTIAE];
    }
    alioquin
    {
        redde VERUM;
    }
    si (lista->genus != MATERIA_VALOR_LISTA)
    {
        redde VERUM;
    }
    n = materia_valor_lista_numerus(*lista);
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*lista,
            k);

        si (   e != NIHIL && e->genus == MATERIA_VALOR_NODUS
            && !_sententias_resolvere(cursus, e->datum.nodus))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
oratio_resolutio_applicare_contextu (
                        Piscina* piscina,
            InternamentumChorda* intern,
   constans MateriaLexiconRatum* ratum,
       constans OratioProgramma* programma,
                            s32  regulae_numerus,
       constans OratioContextus* contextus,
                   MateriaNodus* radix,
          OratioResolutioCensus* census)
{
    Cursus cursus;

    si (programma == NIHIL || radix == NIHIL || contextus == NIHIL)
    {
        redde VERUM;
    }
    cursus.piscina          = piscina;
    cursus.intern           = intern;
    cursus.ratum            = ratum;
    cursus.programma        = programma;
    cursus.regulae_numerus  = regulae_numerus;
    cursus.lingua           = contextus->lingua;
    cursus.contextus        = contextus;   /* T38 a */
    cursus.census           = census;
    /* PROBATIO cursuum plurium (2026-09-09): ORATIO_RESOLUTIO_CURSUS=N
     * resolutionem N-ies super arborem iam resolutam currit (regulae
     * decisiones cursus prioris vident: lectiones permutatae, umbrae
     * impletae; decisio scripta reponitur); ORATIO_RESOLUTIO_PRIOR_ULTIMO=1
     * priorem casuum cursu ultimo solo currit (T24: prior quod regulae
     * vident mutare non debet). Ordinarium: cursus unus, prior activus. */
    {
        constans character* ambitus = getenv("ORATIO_RESOLUTIO_CURSUS");
        constans character* ultimo =
            getenv("ORATIO_RESOLUTIO_PRIOR_ULTIMO");
                       i32 numerus = (ambitus != NIHIL
                           && atoi(ambitus) > I)
                                    ? (i32)atoi(ambitus) : I;
                       i32 i;

        per (i = I; i <= numerus; i++)
        {
            cursus.prior_activus = (b32)(   ultimo == NIHIL
                                         || (   strcmp(ultimo, "1")
                                             != ZEPHYRUM
                                             && strcmp(ultimo,
                                             "numquam") != ZEPHYRUM)
                                         || (   strcmp(ultimo, "1")
                                             == ZEPHYRUM
                                             && i == numerus));
            si (!_sententias_resolvere(&cursus, radix))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

/* T38 a: aditus vetus - contextus {lingua, classicus, prosa} */
b32
oratio_resolutio_applicare (
                       Piscina* piscina,
           InternamentumChorda* intern,
  constans MateriaLexiconRatum* ratum,
      constans OratioProgramma* programma,
                           s32  regulae_numerus,
            constans character* lingua,
                  MateriaNodus* radix,
         OratioResolutioCensus* census)
{
    OratioContextus contextus;

    contextus.lingua     = lingua;
    contextus.dialectus  = (s32)ORATIO_DIALECTUS_CLASSICUS;
    contextus.forma      = (s32)ORATIO_FORMA_PROSA;
    redde oratio_resolutio_applicare_contextu(piscina, intern, ratum,
        programma, regulae_numerus, &contextus, radix, census);
}

vacuum
oratio_resolutio_contextus_documenti (
    constans OratioPartesCensus* census,
          constans MateriaNodus* radix,
                OratioContextus* contextus)
{
    contextus->lingua    = oratio_resolutio_lingua_censu(
        census->vocabula_linguarum);
    contextus->dialectus = census->dialectus
        == (s32)ORATIO_DIALECTUS_MEDIUS
            ? (s32)ORATIO_DIALECTUS_MEDIUS
            : (s32)ORATIO_DIALECTUS_CLASSICUS;
    contextus->forma     = (s32)oratio_forma_documenti_censu(radix,
        NIHIL, NIHIL);
}


/* ==================================================
 * Tabula ponderum decretoris (T38 b, 2026-09-15)
 * ================================================== */

hic_manens constans character* constans TITULI_PONDERUM[] = {
    "regula", "dialectus", "forma", "folliculus", "valor",
    "contentiones", "rectae", "permille"
};

/* initia camporum lineae [i, finis) per TAB in campi[] (ad X servata),
 * numerus camporum redditus (maior X = latitudo aliena) */
interior i32
_campi_ponderum (
    chorda  textus,
       i32  i,
       i32  finis,
       i32* campi)
{
    i32 n = I;
    i32 k;

    campi[ZEPHYRUM] = i;
    per (k = i; k < finis; k++)
    {
        si (textus.datum[k] == '\t')
        {
            si (n < (i32)X)
            {
                campi[n] = k + I;
            }
            n = n + I;
        }
    }
    redde n;
}

/* numerus campi k (initia campi, finis lineae): decimalis, negativus = 0 */
interior i32
_numerus_campi (
          chorda  textus,
    constans i32* campi,
             i32  k,
             i32  n,
             i32  finis)
{
    i32 a = campi[k];
    i32 b = k + I < n ? campi[k + I] - I : finis;
    s32 v = _numerus_decimalis(_chorda(textus.datum + a, b - a));

    redde v > ZEPHYRUM ? (i32)v : ZEPHYRUM;
}

b32
oratio_resolutio_pondera_legere (
                     Piscina* piscina,
             OratioProgramma* programma,
                      chorda  textus,
    OratioVocabulariumVitium* vitium)
{
    i32 i             = ZEPHYRUM;
    i32 linea         = ZEPHYRUM;
    b32 caput_lectum  = FALSUM;

    memset(vitium, ZEPHYRUM, magnitudo(*vitium));
    vitium->plagula    = "pondera.tsv";
    programma->pondera = tabula_dispersa_creare_chorda(piscina,
        (i32)1024);
    si (programma->pondera == NIHIL)
    {
        redde FALSUM;
    }
    dum (i < textus.mensura)
    {
        i32 finis = i;
        i32 campi[10];
        i32 n;
        i32 k;

        dum (finis < textus.mensura && textus.datum[finis] != '\n')
        {
            finis = finis + I;
        }
        linea = linea + I;
        si (finis > i && textus.datum[i] != '#')
        {
            n = _campi_ponderum(textus, i, finis, campi);
            si (n != (i32)VIII)
            {
                vitium->linea = linea;
                vitium->causa = "latitudo ponderum";
                redde FALSUM;
            }
            si (!caput_lectum)
            {
                per (k = ZEPHYRUM; k < (i32)VIII; k++)
                {
                    i32 a = campi[k];
                    i32 b = k + I < n ? campi[k + I] - I : finis;
                    i32 l = (i32)strlen(TITULI_PONDERUM[k]);

                    si (   b - a != l
                        || memcmp(textus.datum + a, TITULI_PONDERUM[k],
                            (size_t)l) != ZEPHYRUM)
                    {
                        character* causa = (character*)piscina_allocare(
                            piscina, (memoriae_index)(b - a)
                            + (memoriae_index)XXXII);

                        si (causa == NIHIL)
                        {
                            redde FALSUM;
                        }
                        sprintf(causa, "caput ponderum: %.*s",
                            (integer)(b - a),
                            (constans character*)textus.datum + a);
                        vitium->linea = linea;
                        vitium->causa = causa;
                        redde FALSUM;
                    }
                }
                caput_lectum = VERUM;
            }
            alioquin
            {
                chorda clavis = _copia(piscina, _chorda(textus.datum
                    + campi[ZEPHYRUM],
                    campi[(i32)V] - I - campi[ZEPHYRUM]));
                OratioPondus* pondus = (OratioPondus*)piscina_allocare(
                    piscina, (memoriae_index)magnitudo(*pondus));

                si (clavis.datum == NIHIL || pondus == NIHIL)
                {
                    redde FALSUM;
                }
                pondus->contentiones = _numerus_campi(textus, campi,
                    (i32)V, n, finis);
                pondus->rectae       = _numerus_campi(textus, campi,
                    (i32)VI, n, finis);
                pondus->permille     = _numerus_campi(textus, campi,
                    (i32)VII, n, finis);
                (vacuum)tabula_dispersa_inserere(programma->pondera,
                    clavis, pondus);
            }
        }
        i = finis + I;
    }
    si (!caput_lectum)
    {
        vitium->causa = "caput ponderum absens";
        redde FALSUM;
    }
    redde VERUM;
}

constans OratioPondus*
oratio_resolutio_pondus (
    constans OratioProgramma* programma,
                      chorda  regula,
          constans character* dialectus,
          constans character* forma,
          constans character* folliculus,
          constans character* valor)
{
    character  clavis[512];
          i32  n;
       vacuum* v;

    si (programma == NIHIL || programma->pondera == NIHIL)
    {
        redde NIHIL;
    }
    n = regula.mensura + (i32)IV + (i32)strlen(dialectus)
        + (i32)strlen(forma) + (i32)strlen(folliculus)
        + (i32)strlen(valor);
    si (n >= (i32)512)
    {
        redde NIHIL;
    }
    sprintf(clavis, "%.*s\t%s\t%s\t%s\t%s", (integer)regula.mensura,
        (constans character*)regula.datum, dialectus, forma, folliculus,
        valor);
    si (!tabula_dispersa_invenire(programma->pondera,
        _chorda((i8*)clavis, n), &v))
    {
        redde NIHIL;
    }
    redde (constans OratioPondus*)v;
}

i32
oratio_resolutio_pondera_limen (vacuum)
{
    constans character* ambitus = getenv("ORATIO_PONDERA_LIMEN");

    redde ambitus != NIHIL && atoi(ambitus) > ZEPHYRUM
        ? (i32)atoi(ambitus) : (i32)XX;
}

/* T38 b: radix/oratio/probationes/fixa/pondera.tsv in programma->pondera;
 * plagula absens = NIHIL (decretor = exsecutor); plagula mala = FALSUM
 * cum vitio (recusatio clara, numquam tacita). */
interior b32
_pondera_onerare (
                     Piscina* piscina,
          constans character* radix,
             OratioProgramma* programma,
    OratioVocabulariumVitium* vitium)
{
    character via[1024];
       chorda fons;

    sprintf(via, "%s/oratio/probationes/fixa/pondera.tsv", radix);
    si (!_plagulam_legere(piscina, via, &fons))
    {
        redde VERUM;
    }
    redde oratio_resolutio_pondera_legere(piscina, programma, fons,
        vitium);
}
