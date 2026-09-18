/* diagnostica.c - Diagnostica plagularum per clientes materiae
 * (tools/diagnostica.sh; materia-sedes-spec par. XI)
 *
 * Usus: diagnostica <plagula...> [-machina] [-sine-excerpto]
 * Suffixum clientem eligit (.sh crusta, .css css). Quaeque plagula:
 * parsura, diagnostica derivata (et emissa), deinde proiectio una -
 * refutatio scriptoris ut materia:scriptura. Exitus: 0 nullum
 * erratum, 1 erratum, 2 nihil iudicatum.
 *
 * Ordo TSV (XII campi): via linea columna linea_finis columna_finis
 * initium finis gravitas codex causa | sedes_relatae nota_primaria.
 * Campus XI sedes relatas fert ut 'L:C-L:C@B-B|nota' per ';' iunctas
 * (vacuus si nullae); campus XII notam sedis primariae (vacuus si
 * NIHIL). Sedes relata OCTETOS quoque fert, sicut primaria campis
 * VI-VII: pictor octetis pingit, non lineis, ergo forma sine eis
 * excerptum reddere non posset. Campi ADDITI sunt, numquam ordines
 * novi: ordo unus diagnosticum unum manet. Codex CRUDUS in machina;
 * linea humana '<grammatica>:' praefigit codici sine ':' (codices
 * substrati eum iam ferunt).
 *
 * Modus '-lege': TSV ex stdin legit et formam humanam pingit, ut
 * inventa ALIUNDE nata (regulae lintris per pythonica) eundem
 * pictorem adeant. REVERSIO OCTETIM: 'diagnostica x' idem est ac
 * 'diagnostica -machina x | diagnostica -lege'.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda_aedificator.h"
#include "excerptum.h"
#include "materia_arbor.h"
#include "materia_diagnostica.h"
#include "materia_lexicon.h"
#include "crusta_arbor.h"
#include "crusta_diagnostica.h"
#include "crusta_lexicon.h"
#include "crusta_registrum.h"
#include "css_arbor.h"
#include "css_lexicon.h"
#include "css_registrum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Tabula tota in memoriam semel legitur, ne linea nimis longa tacite
 * truncetur; supra hunc limitem refutatio NOMINATA. */
#define TABULA_MAXIMA 4194304

nomen structura {
    i32 errata;
    i32 monita;
    i32 plagulae;
} Summa;

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

interior b32
_suffixum (
    constans character* via,
    constans character* suffixum)
{
    size_t longitudo_viae = strlen(via);
    size_t longitudo_suff = strlen(suffixum);

    redde (b32)(   longitudo_viae >= longitudo_suff
                && strcmp(via + longitudo_viae - longitudo_suff,
                       suffixum) == ZEPHYRUM);
}

/* Nota in TSV: '|', ';', TAB et NOVA LINEA spatio uno mutantur.
 * Damnosum quidem (nota mutilatur), corrumpens numquam: campus
 * numquam in duos scinditur. Notae substrati nihil horum ferunt; nota
 * a regula lintris data quidlibet ferre potest. */
/* Lineam in campos per TAB scindere (in loco: TAB -> NUL). Reddit
 * numerum camporum TOTUM, etiam supra 'maximum', ut vocans formam
 * pravam agnoscat potius quam truncatam accipiat. */
interior i32
_campos_scindere (
      character*  linea,
      character** campi,
            i32   maximum)
{
           i32  numerus  = ZEPHYRUM;
     character* initium  = linea;
     character* p        = linea;

    dum (VERUM)
    {
        si (*p == '\t' || *p == '\0')
        {
            b32 ultimus = (b32)(*p == '\0');

            si (numerus < maximum)
            {
                campi[numerus] = initium;
            }
            numerus++;
            si (ultimus)
            {
                frange;
            }
            *p       = '\0';
            initium  = p + I;
        }
        p++;
    }
    redde numerus;
}

/* 'L:C-L:C@B-B|nota' -> tractus + nota. Nota post '|' manet, ergo
 * quidlibet ferre potest praeter ';' quod scriptor iam mutavit. */
interior b32
_sedem_legere (
              character*  frustum,
         MateriaTractus*  tractus,
     constans character** nota)
{
     character* barra          = strchr(frustum, '|');
       integer  linea          = 0;
       integer  columna        = 0;
       integer  linea_finis    = 0;
       integer  columna_finis  = 0;
       integer  initium        = 0;
       integer  finis          = 0;

    *nota = NIHIL;
    si (barra != NIHIL)
    {
        *barra  = '\0';
        *nota   = barra + I;
    }
    si (sscanf(frustum, "%d:%d-%d:%d@%d-%d", &linea, &columna,
            &linea_finis, &columna_finis, &initium, &finis) != VI)
    {
        redde FALSUM;
    }
    memset(tractus, ZEPHYRUM, magnitudo(*tractus));
    tractus->linea          = (i32)linea;
    tractus->columna        = (i32)columna;
    tractus->linea_finis    = (i32)linea_finis;
    tractus->columna_finis  = (i32)columna_finis;
    tractus->initium        = (s32)initium;
    tractus->finis          = (s32)finis;
    tractus->est_fons       = VERUM;
    redde VERUM;
}

/* campum XI in seriem MateriaSedesRelata solvere */
interior b32
_relata_legere (
                Piscina*  piscina,
              character*  campus,
     MateriaSedesRelata** exitus,
                    i32*  numerus)
{
    MateriaSedesRelata* series;
             character* p;
                    i32  quot = ZEPHYRUM;
                    i32  k;

    *exitus   = NIHIL;
    *numerus  = ZEPHYRUM;
    si (campus[0] == '\0')
    {
        redde VERUM;
    }
    quot = (i32)I;
    per (p = campus; *p != '\0'; p++)
    {
        si (*p == ';')
        {
            quot++;
        }
    }
    series = (MateriaSedesRelata*)piscina_allocare(piscina,
        (memoriae_index)quot * magnitudo(MateriaSedesRelata));
    si (series == NIHIL)
    {
        redde FALSUM;
    }
    p = campus;
    per (k = ZEPHYRUM; k < quot; k++)
    {
        character* punctum = strchr(p, ';');

        si (punctum != NIHIL)
        {
            *punctum = '\0';
        }
        si (!_sedem_legere(p, &series[k].tractus, &series[k].nota))
        {
            redde FALSUM;
        }
        p = punctum != NIHIL ? punctum + I : p + strlen(p);
    }
    *exitus   = series;
    *numerus  = quot;
    redde VERUM;
}

interior vacuum
_notam_scribere (
    constans character* nota)
{
    i32 i;

    si (nota == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; nota[i] != '\0'; i++)
    {
        character c = nota[i];

        si (   c == '|' || c == ';' || c == '\t' || c == '\n'
            || c == '\r')
        {
            c = ' ';
        }
        putchar((integer)c);
    }
}

/* Pictor seriem ORDINATAM poscit et eam non ordinat; recordum sedem
 * primariam seorsum a relatis servat, ergo ordo per constructionem
 * non datur. Numerus minimus est (hodie II), ergo insertio. */
interior vacuum
_sedes_ordinare (
    ExcerptumSedes* sedes,
               i32  numerus)
{
    i32 i;

    per (i = I; i < numerus; i++)
    {
        ExcerptumSedes cella  = sedes[i];
                   i32 j      = i;

        dum (j > ZEPHYRUM && sedes[j - I].initium > cella.initium)
        {
            sedes[j] = sedes[j - I];
            j--;
        }
        sedes[j] = cella;
    }
}

interior vacuum
_diagnosticum_imprimere (
                     Piscina* piscina,
          constans character* via,
          constans character* grammatica,
          constans character* fons,
                         i32  mensura,
constans MateriaDiagnosticum* d,
                         b32  machina,
                         b32  excerptum,
                       Summa* summa)
{
    b32 monitum = (b32)(d->gravitas == (s32)MATERIA_GRAVITAS_MONITUM);
    b32 substrati = (b32)(strchr(d->codex, ':') != NIHIL);

    si (monitum)
    {
        summa->monita++;
    }
    alioquin
    {
        summa->errata++;
    }
    si (machina)
    {
        i32 r;

        imprimere("%s\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%s\t%s\t", via,
            (integer)d->tractus.linea, (integer)d->tractus.columna,
            (integer)d->tractus.linea_finis,
            (integer)d->tractus.columna_finis,
            (integer)d->tractus.initium, (integer)d->tractus.finis,
            monitum ? "monitum" : "erratum", d->codex, d->causa);
        /* XI: sedes relatae, 'L:C-L:C|nota' per ';' iunctae. ORDINES
         * NOVI NON: ordo unus diagnosticum unum manet, aliter numerus
         * quisque qui TSV quaerit tacite cresceret. */
        per (r = ZEPHYRUM; r < d->numerus_relatorum; r++)
        {
            constans MateriaTractus* t = &d->relata[r].tractus;

            si (r > ZEPHYRUM)
            {
                putchar((integer)';');
            }
            imprimere("%d:%d-%d:%d@%d-%d", (integer)t->linea,
                (integer)t->columna, (integer)t->linea_finis,
                (integer)t->columna_finis, (integer)t->initium,
                (integer)t->finis);
            si (d->relata[r].nota != NIHIL)
            {
                putchar((integer)'|');
                _notam_scribere(d->relata[r].nota);
            }
        }
        /* XII: nota sedis primariae */
        putchar((integer)'\t');
        _notam_scribere(d->nota);
        putchar((integer)'\n');
        redde;
    }
    imprimere("%s:%d:%d: [%s] %s%s%s\n  %s\n", via,
        (integer)d->tractus.linea, (integer)d->tractus.columna,
        monitum ? "monitum" : "erratum",
        substrati ? "" : grammatica, substrati ? "" : ":", d->codex,
        d->causa);
    si (excerptum && d->tractus.initium >= ZEPHYRUM)
    {
        ChordaAedificator* aedificator;
           ExcerptumSedes* sedes;
                      i32  numerus = I;
                      i32  r;

        sedes = (ExcerptumSedes*)piscina_allocare(piscina,
            (memoriae_index)(d->numerus_relatorum + I)
                * magnitudo(ExcerptumSedes));
        si (sedes == NIHIL)
        {
            redde;
        }
        sedes[ZEPHYRUM].initium  = d->tractus.initium;
        sedes[ZEPHYRUM].finis    = d->tractus.finis;
        sedes[ZEPHYRUM].linea    = d->tractus.linea;
        sedes[ZEPHYRUM].nota     = d->nota;
        per (r = ZEPHYRUM; r < d->numerus_relatorum; r++)
        {
            constans MateriaTractus* t = &d->relata[r].tractus;

            /* Sedes relata insana excerptum TOTUM perderet, quia
             * pictor seriem totam recusat - ergo praetermittitur et
             * primaria superest, ut ante hoc opus. */
            si (   t->initium < ZEPHYRUM || t->initium > (s32)mensura
                || t->linea == ZEPHYRUM)
            {
                perge;
            }
            sedes[numerus].initium  = t->initium;
            sedes[numerus].finis    = t->finis;
            sedes[numerus].linea    = t->linea;
            sedes[numerus].nota     = d->relata[r].nota;
            numerus++;
        }
        _sedes_ordinare(sedes, numerus);
        aedificator = chorda_aedificator_creare(piscina,
            (memoriae_index)CCLVI);
        si (   aedificator != NIHIL
            && excerptum_scribere_multa(aedificator, fons, mensura,
                   sedes, numerus))
        {
            chorda textus = chorda_aedificator_finire(aedificator);

            fwrite(textus.datum, I, (size_t)textus.mensura, stdout);
        }
    }
}

interior b32
_plagulam_iudicare (
    constans character* via,
                   b32  machina,
                   b32  excerptum,
                 Summa* summa)
{
                   Piscina* piscina;
                 character* fons;
                       i32  mensura = ZEPHYRUM;
       MateriaLexiconRatum  ratum;
        MateriaLexIudicium  iudicium;
     MateriaArborConsilium  consilium;
     MateriaArborScriptura  scriptura;
             CrustaParsura  relatio;
              MateriaNodus* radix        = NIHIL;
                       Xar* diagnostica  = NIHIL;
        constans character* grammatica   = NIHIL;
                       i32  k;

    piscina = piscina_generare_dynamicum("diagnostica", 4194304);
    si (piscina == NIHIL)
    {
        redde FALSUM;
    }
    fons = _plagulam_legere(piscina, via, &mensura);
    si (fons == NIHIL)
    {
        fprintf(stderr, "diagnostica: plagula illegibilis: %s\n", via);
        piscina_destruere(piscina);
        redde FALSUM;
    }
    si (_suffixum(via, ".sh"))
    {
        grammatica = "crusta";
        si (materia_lexicon_ratum_facere(&ratum, &CRUSTA_LEXICON,
                &iudicium))
        {
            materia_arbor_consilium_nudum(&consilium,
                &CRUSTA_REGISTRUM, &ratum, grammatica);
            memset(&relatio, ZEPHYRUM, magnitudo(relatio));
            radix = crusta_arbor_parsare(piscina, fons, mensura,
                &CRUSTA_BASH, &relatio);
            diagnostica = radix != NIHIL
                ? crusta_diagnostica(piscina, radix, &relatio) : NIHIL;
        }
    }
    alioquin si (_suffixum(via, ".css"))
    {
        grammatica = "css";
        si (materia_lexicon_ratum_facere(&ratum, &CSS_LEXICON,
                &iudicium))
        {
            materia_arbor_consilium_nudum(&consilium, &CSS_REGISTRUM,
                &ratum, grammatica);
            radix = css_arbor_parsare(piscina, fons, mensura);
            diagnostica = radix != NIHIL
                ? materia_diagnostica_derivare(piscina, radix,
                      &CSS_REGISTRUM, &CSS_DIAGNOSTICA, NIHIL, NIHIL)
                : NIHIL;
        }
    }
    alioquin
    {
        fprintf(stderr, "diagnostica: suffixum sine cliente: %s\n",
            via);
        piscina_destruere(piscina);
        redde FALSUM;
    }
    si (radix == NIHIL || diagnostica == NIHIL)
    {
        fprintf(stderr,
            "diagnostica: parsura aut derivatio fracta: %s\n", via);
        piscina_destruere(piscina);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(diagnostica); k++)
    {
        _diagnosticum_imprimere(piscina, via, grammatica, fons,
            mensura,
            (constans MateriaDiagnosticum*)xar_obtinere(diagnostica,
                k), machina, excerptum, summa);
    }
    scriptura = materia_arbor_proicere_nodum(piscina, radix,
        &consilium);
    si (!scriptura.successus)
    {
        MateriaDiagnosticum refutatio;

        memset(&refutatio, ZEPHYRUM, magnitudo(refutatio));
        refutatio.gravitas  = (s32)MATERIA_GRAVITAS_ERRATUM;
        refutatio.codex     = MATERIA_CODEX_SCRIPTURA;
        refutatio.causa     = scriptura.causa != NIHIL
            ? scriptura.causa : "scriptura fracta";
        refutatio.tractus   = scriptura.tractus;
        _diagnosticum_imprimere(piscina, via, grammatica, fons,
            mensura, &refutatio, machina, excerptum, summa);
    }
    summa->plagulae++;
    piscina_destruere(piscina);
    redde VERUM;
}

/* Modus -lege: TSV ex stdin in formam humanam. Inventa ALIUNDE nata
 * (regulae lintris per pythonica) eundem pictorem adeunt, ergo caret
 * unus in domo est, non duo. Plagulae fontium semel lectae et
 * servatae (ordines eiusdem plagulae contigui esse solent).
 *
 * Numerus camporum EXACTE poscitur: forma tacite mutata pernicies
 * esset, et lector qui campos ignotos praetermittit eam celaret. */
interior b32
_tabulam_legere (
      b32  excerptum,
    Summa* summa)
{
               Piscina* piscina;
             character* tabula;
             character* p;
             character* via_lecta      = NIHIL;
             character* fons_lectus    = NIHIL;
                   i32  mensura_lecta  = ZEPHYRUM;
                   b32  sanum          = VERUM;
                size_t  legenda;

    piscina = piscina_generare_dynamicum("diagnostica-lege", 8388608);
    si (piscina == NIHIL)
    {
        redde FALSUM;
    }
    tabula = (character*)piscina_allocare(piscina,
        (memoriae_index)TABULA_MAXIMA);
    si (tabula == NIHIL)
    {
        piscina_destruere(piscina);
        redde FALSUM;
    }
    legenda = fread(tabula, I, (size_t)(TABULA_MAXIMA - I), stdin);
    tabula[legenda] = '\0';
    si (legenda == (size_t)(TABULA_MAXIMA - I))
    {
        fprintf(stderr,
            "diagnostica: tabula nimis magna (> %d octeti)\n",
            (integer)(TABULA_MAXIMA - I));
        piscina_destruere(piscina);
        redde FALSUM;
    }
    p = tabula;
    dum (*p != '\0')
    {
                 character* linea  = p;
                 character* nova   = strchr(p, '\n');
                 character* campi[XVI];
                       i32  numerus;
       MateriaDiagnosticum  d;
        MateriaSedesRelata* relata_lecta = NIHIL;
        constans character* grammatica;

        si (nova != NIHIL)
        {
            *nova  = '\0';
            p      = nova + I;
        }
        alioquin
        {
            p = linea + strlen(linea);
        }
        si (linea[0] == '\0')
        {
            perge;
        }
        numerus = _campos_scindere(linea, campi, (i32)XVI);
        si (numerus != (i32)XII)
        {
            fprintf(stderr,
                "diagnostica: ordo TSV pravus (%d campi pro "
                "XII): %s\n", (integer)numerus, campi[0]);
            sanum = FALSUM;
            perge;
        }
        memset(&d, ZEPHYRUM, magnitudo(d));
        d.gravitas = strcmp(campi[7], "monitum") == ZEPHYRUM
            ? (s32)MATERIA_GRAVITAS_MONITUM
            : (s32)MATERIA_GRAVITAS_ERRATUM;
        d.codex = campi[8];
        d.causa = campi[9];
        d.nota = campi[11][0] != '\0' ? campi[11] : NIHIL;
        d.tractus.linea = (i32)atoi(campi[1]);
        d.tractus.columna = (i32)atoi(campi[2]);
        d.tractus.linea_finis = (i32)atoi(campi[3]);
        d.tractus.columna_finis = (i32)atoi(campi[4]);
        d.tractus.initium = (s32)atoi(campi[5]);
        d.tractus.finis = (s32)atoi(campi[6]);
        d.tractus.est_fons = VERUM;
        si (!_relata_legere(piscina, campi[10], &relata_lecta,
                &d.numerus_relatorum))
        {
            fprintf(stderr, "diagnostica: sedes relatae pravae: %s\n",
                campi[10]);
            sanum = FALSUM;
            perge;
        }
        d.relata = relata_lecta;
        si (   via_lecta                   == NIHIL
            || strcmp(via_lecta, campi[0]) != ZEPHYRUM)
        {
            via_lecta      = campi[0];
            mensura_lecta  = ZEPHYRUM;
            fons_lectus  = _plagulam_legere(piscina, campi[0],
                &mensura_lecta);
        }
        /* grammatica ex suffixo, ut in via ordinaria: TSV codicem
         * CRUDUM fert, linea humana praefixum addit */
        grammatica = _suffixum(campi[0], ".css") ? "css" : "crusta";
        _diagnosticum_imprimere(piscina, campi[0], grammatica,
            fons_lectus, mensura_lecta, &d, FALSUM, excerptum, summa);
    }
    summa->plagulae++;
    piscina_destruere(piscina);
    redde sanum;
}

integer
principale (
      integer   argc,
    character** argv)
{
      Summa summa;
        b32 machina    = FALSUM;
        b32 excerptum  = VERUM;
        b32 legere     = FALSUM;
    integer i;

    memset(&summa, ZEPHYRUM, magnitudo(summa));
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[i], "-sine-excerpto") == ZEPHYRUM)
        {
            excerptum = FALSUM;
        }
        alioquin si (strcmp(argv[i], "-lege") == ZEPHYRUM)
        {
            legere = VERUM;
        }
    }
    si (legere)
    {
        si (!_tabulam_legere(excerptum, &summa))
        {
            fprintf(stderr, "%d diagnostica (erratum %d, monitum %d)\n",
                (integer)(summa.errata + summa.monita),
                (integer)summa.errata, (integer)summa.monita);
            redde II;
        }
        fprintf(stderr, "%d diagnostica (erratum %d, monitum %d) ex "
            "tabula\n", (integer)(summa.errata + summa.monita),
            (integer)summa.errata, (integer)summa.monita);
        redde summa.errata > ZEPHYRUM ? I : ZEPHYRUM;
    }
    per (i = I; i < argc; i++)
    {
        si (argv[i][0] != '-')
        {
            (vacuum)_plagulam_iudicare(argv[i], machina, excerptum,
                &summa);
        }
    }
    fprintf(stderr, "%d diagnostica (erratum %d, monitum %d) in %d "
        "plagulis\n", (integer)(summa.errata + summa.monita),
        (integer)summa.errata, (integer)summa.monita,
        (integer)summa.plagulae);
    si (summa.plagulae == ZEPHYRUM)
    {
        fprintf(stderr, "usus: diagnostica <plagula...> [-machina] "
            "[-sine-excerpto] (nihil iudicatum)\n");
        redde II;
    }
    redde summa.errata > ZEPHYRUM ? I : ZEPHYRUM;
}
