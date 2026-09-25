/* oraculum_silvae.c - ORACULUM CONGELATUM: silva ad pignus contra
 * silvam vivam (phasis V, silva-migratio-plan T2)
 *
 * CUR. Porta shim emissorem materiae contra SILVAM VIVAM confert.
 * Phasis V silvam vivam in materiam vertit - ergo per migrationem
 * 'contra silvam' codicem novum contra se ipsum conferret. Oraculum
 * igitur CONGELATUR ut PROGRAMMA, non ut photographia: silva ad
 * commissum pignoratum, ex amalgamate suo (silva.c + silva.h, solis
 * capitibus systematis), in processu SEPARATO - symbola eius
 * numquam symbolis binarii vivi occurrunt.
 *
 * FONS UNUS, AEDIFICATIONES DUAE (materia/oraculum_silvae_struere.sh):
 *   -DORACULUM_PIGNUS  silva.h amalgamatis ad pignus; NIHIL nectit
 *                      praeter obiectum amalgamatis
 *   (sine)             capita silvae vivae + obiecta silva/build;
 *                      ita binarium vivum CONSUMPTOR facadis est
 *                      (T7 et seq.): nomina haec quinque capitum
 *                      servanda sunt, et hoc instrumentum id
 *                      quoque cursu probat.
 * Differentia sola: substratum vendicatum in amalgamate
 * renominatum est (SilvaPiscina, SilvaChorda, silva_piscina_*);
 * functiones silvae nomina eadem ferunt. Alias infra.
 *
 * EFFUSIO: linea una per plagulam, tabulis separata, deterministica:
 *   via octeti= circuitus= emissio= stml= errores= semantica=
 * emissio/stml/semantica = FNV-1a LXIV in HOC fonte scriptum
 * (aedificatio pignoris nihil nectere licet praeter amalgama).
 * -stml DIR documentum STML plagulae in DIR scribit;
 * -legere DIR (VIVUM solum) documentum pignoris legit et addit:
 *   lectio=     emissio documenti pignoris lecti == fons?
 *   comparator= documentum pignoris lectum contra documentum vivum
 *               lectum (FIDELITAS) - par contra par, ambo lecta.
 * -nudum = sine contextu (ut shim); ordinarium: contextus latinus
 * (lexicon latina.h) - machina expansionis vere exercetur.
 *
 * Exitus: 0 omnes plagulae lectae (verdicta in effusione) ·
 *         2 usus / plagula illegibilis / piscina deficiens. */

#ifdef ORACULUM_PIGNUS
#include "silva.h"      /* amalgama ad pignus, ANTE latina.h */
#include "latina.h"
nomen SilvaPiscina PiscinaOraculi;
nomen SilvaChorda  ChordaOraculi;
#define piscina_oraculi_generare silva_piscina_generare_dynamicum
#define piscina_oraculi_destruere silva_piscina_destruere
#else
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "silva_contextus.h"    /* silva viva: capita fontium */
#include "silva_c89_oraculum.h"
#include "silva_scribere.h"
#include "silva_arbor.h"
#include "silva_c89_semantica.h"
#include "silva_tabulae_c89.h"
nomen Piscina PiscinaOraculi;
nomen chorda  ChordaOraculi;
#define piscina_oraculi_generare piscina_generare_dynamicum
#define piscina_oraculi_destruere piscina_destruere
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITAS_TYPI 1024

/* ---- FNV-1a LXIV (sine litteris 'ULL' - C89) ---- */

nomen structura {
    i64 valor;
} SigillumOraculi;

interior vacuum
_sigillum_initiare (
    SigillumOraculi* s)
{
    s->valor = ((i64)0xcbf29ce4UL << XXXII) | (i64)0x84222325UL;
}

interior vacuum
_sigillum_addere (
       SigillumOraculi* s,
       constans vacuum* datum,
        memoriae_index  mensura)
{
       constans i8* o = (constans i8*)datum;
    memoriae_index  i;
               i64  primus = ((i64)I << XL) | (i64)0x1b3UL;

    per (i = ZEPHYRUM; i < mensura; i++)
    {
        s->valor ^= (i64)o[i];
        s->valor *= primus;
    }
}

interior vacuum
_sigillum_imprimere (
    constans SigillumOraculi* s)
{
    imprimere("%08x%08x",
        (insignatus integer)((s->valor >> XXXII) & 0xffffffffUL),
        (insignatus integer)(s->valor & 0xffffffffUL));
}

/* ---- plagula in memoriam ---- */
interior character*
_plagulam_legere (
    constans character* via,
                   i32* mensura)
{
       FILE* f;
      longus  longitudo;
    character* datum;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    fseek(f, 0L, SEEK_SET);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    datum = (character*)malloc((memoriae_index)longitudo + I);
    si (datum == NIHIL)
    {
        fclose(f);
        redde NIHIL;
    }
    si (fread(datum, I, (memoriae_index)longitudo, f)
        != (memoriae_index)longitudo)
    {
        free(datum);
        fclose(f);
        redde NIHIL;
    }
    fclose(f);
    datum[longitudo]  = '\0';
    *mensura          = (i32)longitudo;
    redde datum;
}

/* DIR + "/" + via ('/' -> '%') + ".stml" */
interior vacuum
_viam_documenti (
    constans character* directorium,
    constans character* via,
             character* exitus,
        memoriae_index  capacitas)
{
    memoriae_index i;
    memoriae_index n;

    n = (memoriae_index)sprintf(exitus, "%s/", directorium);
    per (i = ZEPHYRUM; via[i] != '\0' && n + VI < capacitas; i++)
    {
        exitus[n++] = (via[i] == '/') ? '%' : via[i];
    }
    strcpy(exitus + n, ".stml");
}

/* ---- semantica: symbola ordine indicis + diagnostica ---- */
interior vacuum
_semanticam_sigillare (
    PiscinaOraculi* piscina,
      SilvaParsura* parsura)
{
      SilvaSemantica* sem;
     SigillumOraculi  s;
                 i32  i;
                 i32  n;
           character  linea[CAPACITAS_TYPI + CCLVI];
           character  typus[CAPACITAS_TYPI];

    sem = silva_c89_semantica_analysare(piscina, parsura);
    si (sem == NIHIL)
    {
        imprimere("-");
        redde;
    }
    _sigillum_initiare(&s);
    n = (i32)silva_c89_symbola_numerus(sem);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans SemanticaSymbolum* sym =
            silva_c89_symbolum_per_indicem(sem, i);

        si (sym == NIHIL)
        {
            perge;
        }
        typus[0] = '\0';
        si (sym->typus != NIHIL)
        {
            silva_c89_typum_scribere(sym->typus, typus, CAPACITAS_TYPI);
        }
        _sigillum_addere(&s, sym->titulus.datum,
            (memoriae_index)sym->titulus.mensura);
        sprintf(linea, "\t%d\t%u\t%s\n", (integer)sym->genus,
            (insignatus integer)sym->profunditas, typus);
        _sigillum_addere(&s, linea, strlen(linea));
    }
    n = (i32)silva_c89_diagnostica_numerus(sem);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans SemanticaDiagnosticum* d =
            silva_c89_diagnosticum_per_indicem(sem, i);

        si (d == NIHIL)
        {
            perge;
        }
        sprintf(linea, "D\t%d\t%u\t%u\n", (integer)d->codex,
            (insignatus integer)d->linea,
            (insignatus integer)d->columna);
        _sigillum_addere(&s, linea, strlen(linea));
    }
    _sigillum_imprimere(&s);
}

#ifndef ORACULUM_PIGNUS
/* ---- -legere: documentum pignoris per lectorem VIVUM ---- */

interior vacuum
_documentum_pignoris_iudicare (
        PiscinaOraculi* piscina,
    constans character* via_documenti,
    constans character* fons,
                   i32  mensura,
         ChordaOraculi  documentum_vivum)
{
                character* textus;
                      i32  longitudo;
            ChordaOraculi  documentum;
         SilvaArborVitium  vitium;
             SilvaParsura* lecta;
             SilvaParsura* lecta_viva;
           SilvaScriptura  emissio;
    SilvaArborDifferentia  differentia;

    textus = _plagulam_legere(via_documenti, &longitudo);
    si (textus == NIHIL)
    {
        imprimere("\tlectio=absens\tcomparator=-");
        redde;
    }
    documentum.datum    = (i8*)textus;
    documentum.mensura  = longitudo;
    lecta = silva_arbor_legere_parsuram(piscina, NIHIL, documentum,
        &SILVA_C89_REGISTRUM, "c89", &vitium);
    si (lecta == NIHIL)
    {
        imprimere("\tlectio=vitium:%s@%u\tcomparator=-",
            vitium.causa ? vitium.causa : "?",
            (insignatus integer)vitium.linea);
        free(textus);
        redde;
    }
    emissio = silva_scribere_fontem(piscina, lecta,
        &SILVA_C89_REGISTRUM,
        lecta->fons_princeps);
    imprimere("\tlectio=%s",
        (   emissio.successus
         && emissio.textus.mensura == mensura
         && memcmp(emissio.textus.datum, fons, (memoriae_index)mensura)
            == ZEPHYRUM) ? "idem" : "dispar");

    lecta_viva = silva_arbor_legere_parsuram(piscina, NIHIL,
        documentum_vivum, &SILVA_C89_REGISTRUM, "c89", &vitium);
    si (lecta_viva == NIHIL)
    {
        imprimere("\tcomparator=vivum-illegibile");
    }
    alioquin si (silva_arbor_parsurae_aequales(lecta, lecta_viva,
                     SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia))
    {
        imprimere("\tcomparator=aequales");
    }
    alioquin
    {
        imprimere("\tcomparator=%s",
            differentia.campus ? differentia.campus : "?");
    }
    free(textus);
}
#endif

/* ---- plagula una: linea una ---- */

interior b32
_plagulam_iudicare (
         PiscinaOraculi* piscina,
     constans character* via,
                    b32  nudum,
     constans character* directorium_stml,
     constans character* directorium_legendi)
{
              character* fons;
                    i32  mensura;
         SilvaContextus* contextus;
           SilvaParsura* parsura;
         SilvaScriptura  emissio;
    SilvaArborScriptura  documentum;
        SigillumOraculi  s;
                    b32  idem;

    fons = _plagulam_legere(via, &mensura);
    si (fons == NIHIL)
    {
        fprintf(stderr, "oraculum_silvae: illegibilis: %s\n", via);
        redde FALSUM;
    }
    si (nudum)
    {
        parsura = silva_c89_parsare(piscina, via, fons, mensura, NIHIL);
    }
    alioquin
    {
        contextus = silva_contextus_creare(piscina);
        silva_contextus_latinam_addere(contextus);
        parsura = silva_c89_parsare_cum_contextu(piscina, contextus,
            via,
            fons, mensura, NIHIL);
    }
    imprimere("%s\tocteti=%u", via, (insignatus integer)mensura);
    si (parsura == NIHIL)
    {
        imprimere("\tparsura=nulla\n");
        free(fons);
        redde VERUM;
    }

    emissio = silva_scribere_fontem(piscina, parsura,
        &SILVA_C89_REGISTRUM,
        parsura->fons_princeps);
    idem = (   emissio.successus
            && emissio.textus.mensura == mensura
            && memcmp(emissio.textus.datum, fons,
                   (memoriae_index)mensura) == ZEPHYRUM);
    imprimere("\tcircuitus=%s\temissio=", idem ? "idem" : "dispar");
    _sigillum_initiare(&s);
    si (emissio.successus)
    {
        _sigillum_addere(&s, emissio.textus.datum,
            (memoriae_index)emissio.textus.mensura);
    }
    _sigillum_imprimere(&s);

    documentum = silva_arbor_scribere_parsuram(piscina, parsura,
        &SILVA_C89_REGISTRUM, "c89", parsura->fons_princeps, NIHIL);
    imprimere("\tstml=");
    si (documentum.successus)
    {
        _sigillum_initiare(&s);
        _sigillum_addere(&s, documentum.textus.datum,
            (memoriae_index)documentum.textus.mensura);
        _sigillum_imprimere(&s);
    }
    alioquin
    {
        imprimere("recusatum:%s",
            documentum.causa ? documentum.causa : "?");
    }
    si (directorium_stml != NIHIL && documentum.successus)
    {
        character via_documenti[MMMM];
        FILE* f;

        _viam_documenti(directorium_stml, via, via_documenti,
            magnitudo(via_documenti));
        f = fopen(via_documenti, "wb");
        si (f == NIHIL)
        {
            fprintf(stderr, "oraculum_silvae: scribi non potest: %s\n",
                via_documenti);
            free(fons);
            redde FALSUM;
        }
        fwrite(documentum.textus.datum, I,
            (memoriae_index)documentum.textus.mensura, f);
        fclose(f);
    }

    imprimere("\terrores=%u\tsemantica=",
        (insignatus integer)parsura->numerus_errorum);
    _semanticam_sigillare(piscina, parsura);

#ifndef ORACULUM_PIGNUS
    si (directorium_legendi != NIHIL)
    {
        character via_documenti[MMMM];

        _viam_documenti(directorium_legendi, via, via_documenti,
            magnitudo(via_documenti));
        _documentum_pignoris_iudicare(piscina, via_documenti, fons,
            mensura, documentum.textus);
    }
#else
    (vacuum)directorium_legendi;
#endif
    imprimere("\n");
    free(fons);
    redde VERUM;
}

integer
principale (
          integer   argc,
        character** argv)
{
        PiscinaOraculi* piscina;
    constans character* directorium_stml     = NIHIL;
    constans character* directorium_legendi  = NIHIL;
                   b32  nudum                = FALSUM;
                   b32  bene;
               integer  i;
               integer  plagulae = ZEPHYRUM;

    /* PISCINA NOVA PER PLAGULAM, destructa post - NON vacare.
     * Vacare alveos RETINET: post lib/biblia_dr.c (VI MB fontis,
     * ~IV GB cum -legere) quaeque plagula sequens memoriam retentam
     * tangebat - corpus CCCXC plagularum VIII min XLVII s (CCXCIII s
     * systematis) contra I min XLVII s sine ea. Mensura 2026-09-24. */
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-stml") == ZEPHYRUM && i + I < argc)
        {
            directorium_stml = argv[++i];
        }
        alioquin si (   strcmp(argv[i], "-legere") == ZEPHYRUM
                     && i + I < argc)
        {
#ifdef ORACULUM_PIGNUS
            fprintf(stderr, "oraculum_silvae: -legere solum in binario "
                "VIVO (pignus documenta sua non iudicat)\n");
            redde II;
#else
            directorium_legendi = argv[++i];
#endif
        }
        alioquin si (strcmp(argv[i], "-nudum") == ZEPHYRUM)
        {
            nudum = VERUM;
        }
        alioquin si (argv[i][0] == '-')
        {
            fprintf(stderr,
                "usus: oraculum_silvae [-nudum] [-stml <dir>] "
                "[-legere <dir>] <plagula>...\n");
            redde II;
        }
        alioquin
        {
            piscina = piscina_oraculi_generare("oraculum-silvae",
                16777216);
            si (piscina == NIHIL)
            {
                fprintf(stderr, "oraculum_silvae: piscina deficit\n");
                redde II;
            }
            bene = _plagulam_iudicare(piscina, argv[i], nudum,
                directorium_stml, directorium_legendi);
            piscina_oraculi_destruere(piscina);
            si (!bene)
            {
                redde II;
            }
            plagulae++;
        }
    }
    si (plagulae == ZEPHYRUM)
    {
        fprintf(stderr,
            "oraculum_silvae: nulla plagula - NIHIL CURSUM\n");
        redde II;
    }
    redde ZEPHYRUM;
}
