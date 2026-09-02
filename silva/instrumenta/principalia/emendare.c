/* emendare.c - emendator codicum (actio scriptoria PRIMA ecologiae
 * instrumentorum, 2026-08-03; desideratum 01KYTGNA36 pars III)
 *
 * Usus:  emendare <codex> <plagulae...> [-scribere]
 * Exitus: 0 omnia sana(re)ntur | 1 aliqua fracta | 2 apparatus/usus
 *
 * Exemplar renominare: consilium ORDINARIUM (nihil tangitur),
 * -scribere applicat. Classis mechanica UNA adhuc: codex 85
 * (postulata desunt) - lineam '#include "postulata_posix.h"' ante
 * inclusionem primam inserere. Codices 86/87 CONSULTO recusantur:
 * emendatio eorum signaturas mutat (nanosleep timespec petit;
 * MSG_NOSIGNAL ad sedem send pertinet, non setsockopt) - manus
 * hominis requiritur.
 *
 * CUSTODIA (petitio Franis): textus emendatus ANTE scriptionem
 * iudicatur - 85 evanescere debet, NULLUS codex crescere, REICE non
 * induci. Verificatio fracta = nihil scribitur, exitus I. */

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_parsare.h"
#include "silva_c89_semantica.h"
#include "silva_iudicium.h"
#include <stdio.h>
#include <string.h>

#define LINEA_POSTULATORUM "#include \"postulata_posix.h\"\n"

hic_manens b32 scribere_volo = FALSUM;

/* numeri diagnosticorum per codicem (tabula parva in acervum) */
interior vacuum
_numeros_colligere (
    constans SilvaSemantica* sem,
                        i32* numeri)
{
    i32 i;
    i32 m = (i32)silva_c89_diagnostica_numerus(sem);

    memset(numeri, ZEPHYRUM,
        magnitudo(i32) * (memoriae_index)EXAMEN_CODEX_NUMERUS);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaDiagnosticum* d =
            silva_c89_diagnosticum_per_indicem(sem, i);

        si (   d != NIHIL && d->codex >= ZEPHYRUM
            && d->codex < (s32)EXAMEN_CODEX_NUMERUS)
        {
            numeri[d->codex]++;
        }
    }
}

interior b32
_reice_est (
    constans IudiciumFructus* fr)
{
    i32 i;
    i32 m;

    si (   fr->parsura != NIHIL && fr->parsura->numerus_errorum
        > ZEPHYRUM)
    {
        redde VERUM;
    }
    si (fr->sem == NIHIL)
    {
        redde FALSUM;
    }
    m = (i32)silva_c89_diagnostica_numerus(fr->sem);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaDiagnosticum* d =
            silva_c89_diagnosticum_per_indicem(fr->sem, i);

        si (   d != NIHIL && d->severitas == EXAMEN_VIOLATIO
            && !d->provisionale)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior constans character*
_causa_codicis (
    constans SilvaSemantica* sem,
                        s32  codex)
{
    i32 i;
    i32 m = (i32)silva_c89_diagnostica_numerus(sem);

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaDiagnosticum* d =
            silva_c89_diagnosticum_per_indicem(sem, i);

        si (d != NIHIL && d->codex == codex)
        {
            redde d->causa;
        }
    }
    redde NIHIL;
}

/* ancora: initium lineae inclusionis PRIMAE fontis principalis (ex
 * lineis directivarum parsurae - arbor, non textus). Nulla inclusio
 * = 0 (caput plagulae). */
interior s32
_ancoram_invenire (
    constans SilvaParsura* parsura)
{
    i32 i;
    i32 m;

    si (parsura->directivae == NIHIL)
    {
        redde ZEPHYRUM;
    }
    m = xar_numerus(parsura->directivae);
    per (i = ZEPHYRUM; i < m; i++)
    {
        Xar* linea = *(Xar**)xar_obtinere(parsura->directivae, i);
        i32 tm;
        i32 t;
        constans SilvaToken* primum  = NIHIL;
        b32 est_inclusio             = FALSUM;

        si (linea == NIHIL)
        {
            perge;
        }
        tm = xar_numerus(linea);
        per (t = ZEPHYRUM; t < tm; t++)
        {
            constans SilvaToken* tok = *(SilvaToken**)xar_obtinere(
                linea, t);

            si (tok == NIHIL)
            {
                perge;
            }
            si (primum == NIHIL)
            {
                primum = tok;
            }
            si (   tok->genus         == SILVA_LEX_IDENTIFICATOR
                && tok->valor.mensura == VII
                && strncmp((constans character*)tok->valor.datum,
                       "include", VII) == ZEPHYRUM)
            {
                est_inclusio = VERUM;
                frange;
            }
        }
        si (   !est_inclusio || primum == NIHIL
            || primum->fons_index != parsura->fons_princeps
            || primum->byte_offset < ZEPHYRUM)
        {
            perge;
        }
        redde primum->byte_offset - ((s32)primum->columna - I);
    }
    redde ZEPHYRUM;
}

/* VERUM = sana(re)tur aut iam sana; FALSUM = fracta */
interior b32
_plagulam_emendare (
     IudiciumApparatus* app,
    constans character* via)
{
            Piscina* opus;
    IudiciumFructus  ante;
    IudiciumFructus  post;
          character* fons;
                i32  mensura = ZEPHYRUM;
          character* novum;
                i32  mensura_novi;
                i32  linea_m = (i32)strlen(LINEA_POSTULATORUM);
                s32  ancora;
                i32  numeri_ante[EXAMEN_CODEX_NUMERUS];
                i32  numeri_post[EXAMEN_CODEX_NUMERUS];
                b32  bene = VERUM;
                s32  c;

    opus = piscina_generare_dynamicum("emendare_opus", 268435456);
    si (opus == NIHIL)
    {
        imprimere("%s: APPARATUS (piscina)\n", via);
        redde FALSUM;
    }
    fons = silva_iudicium_plagulam_legere(opus, via, &mensura);
    si (fons == NIHIL)
    {
        imprimere("%s: APPARATUS (plagula illegibilis)\n", via);
        piscina_destruere(opus);
        redde FALSUM;
    }
    si (!silva_iudicium_iudicare(app, opus, via, fons, mensura,
            &ante))
    {
        imprimere("%s: %s - praetermissa\n", via,
            ante.recusatum ? "RECUSO (fines)" : "APPARATUS");
        piscina_destruere(opus);
        redde FALSUM;
    }
    _numeros_colligere(ante.sem, numeri_ante);
    si (numeri_ante[EXAMEN_CODEX_POSTULATA_DESUNT] == ZEPHYRUM)
    {
        imprimere("%s: iam sana (codex 85 non flagrat)\n", via);
        piscina_destruere(opus);
        redde VERUM;
    }

    ancora = _ancoram_invenire(ante.parsura);
    si (ancora < ZEPHYRUM || ancora > (s32)mensura)
    {
        imprimere("%s: ancora extra fines (%d) - praetermissa\n",
            via, (int)ancora);
        piscina_destruere(opus);
        redde FALSUM;
    }

    mensura_novi = mensura + linea_m;
    novum = (character*)piscina_allocare(opus,
        (memoriae_index)mensura_novi + I);
    si (novum == NIHIL)
    {
        imprimere("%s: APPARATUS (memoria)\n", via);
        piscina_destruere(opus);
        redde FALSUM;
    }
    memcpy(novum, fons, (memoriae_index)ancora);
    memcpy(novum + ancora, LINEA_POSTULATORUM,
        (memoriae_index)linea_m);
    memcpy(novum + ancora + linea_m, fons + ancora,
        (memoriae_index)((s32)mensura - ancora));
    novum[mensura_novi] = '\0';

    si (!silva_iudicium_iudicare(app, opus, via, novum, mensura_novi,
            &post))
    {
        imprimere("%s: NON SANAT (iudicium post fractum)\n", via);
        piscina_destruere(opus);
        redde FALSUM;
    }
    _numeros_colligere(post.sem, numeri_post);
    si (numeri_post[EXAMEN_CODEX_POSTULATA_DESUNT] != ZEPHYRUM)
    {
        imprimere("%s: NON SANAT (85 manet post insertionem)\n",
            via);
        bene = FALSUM;
    }
    per (c = ZEPHYRUM; c < (s32)EXAMEN_CODEX_NUMERUS; c++)
    {
        si (numeri_post[c] > numeri_ante[c])
        {
            imprimere("%s: NON SANAT (codex %d crevit %d -> %d)\n",
                via, (int)c, (int)numeri_ante[c],
                (int)numeri_post[c]);
            bene = FALSUM;
        }
    }
    si (!_reice_est(&ante) && _reice_est(&post))
    {
        imprimere("%s: NON SANAT (REICE inductum)\n", via);
        bene = FALSUM;
    }
    si (!bene)
    {
        piscina_destruere(opus);
        redde FALSUM;
    }

    si (scribere_volo)
    {
        FILE* pl = fopen(via, "wb");

        si (   pl == NIHIL
            || fwrite(novum, I, (memoriae_index)mensura_novi, pl)
                != (memoriae_index)mensura_novi)
        {
            imprimere("%s: SCRIPTIO FRACTA\n", via);
            si (pl != NIHIL)
            {
                fclose(pl);
            }
            piscina_destruere(opus);
            redde FALSUM;
        }
        fclose(pl);
        imprimere("%s: SANATA (linea inserta ante offset %d;"
            " verificata: 85 evanuit, nihil crevit)\n", via,
            (int)ancora);
    }
    alioquin
    {
        constans character* causa = _causa_codicis(ante.sem,
            (s32)EXAMEN_CODEX_POSTULATA_DESUNT);

        imprimere("%s: SANARET (offset %d; verificata in memoria:"
            " 85 evanesceret, nihil cresceret)\n    %s\n", via,
            (int)ancora, causa != NIHIL ? causa : "");
    }
    piscina_destruere(opus);
    redde VERUM;
}

s32
principale (
      integer   argc,
    character** argv)
{
               Piscina* piscina;
     IudiciumApparatus* app;
    constans character* codex_arg  = NIHIL;
                   i32  sanae      = ZEPHYRUM;
                   i32  fractae    = ZEPHYRUM;
               integer  k;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-scribere") == ZEPHYRUM)
        {
            scribere_volo = VERUM;
        }
        alioquin si (codex_arg == NIHIL)
        {
            codex_arg = argv[k];
        }
    }
    si (codex_arg == NIHIL)
    {
        fprintf(stderr, "usus: emendare <codex> <plagulae...>"
            " [-scribere]\n");
        redde II;
    }
    si (strcmp(codex_arg, "85") != ZEPHYRUM)
    {
        fprintf(stderr, "emendare: codex %s emendationem mechanicam"
            " non habet.\n  85 = sola classis mechanica (linea una"
            " inseritur).\n  86 (vernaculum): MSG_NOSIGNAL ad sedem"
            " send pertinet, non setsockopt - manus.\n  87"
            " (obsoletum): nanosleep struct timespec petit -"
            " signatura mutanda, manus.\n", codex_arg);
        redde II;
    }

    piscina = piscina_generare_dynamicum("emendare", 268435456);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "emendare: piscina deest\n");
        redde II;
    }
    app = silva_iudicium_praeparare(piscina);
    si (app == NIHIL)
    {
        redde II;
    }

    per (k = I; k < argc; k++)
    {
        si (argv[k][ZEPHYRUM] == '-' || argv[k] == codex_arg)
        {
            perge;
        }
        si (_plagulam_emendare(app, argv[k]))
        {
            sanae++;
        }
        alioquin
        {
            fractae++;
        }
    }
    imprimere("emendare: %d sanae/sanarentur, %d fractae%s\n",
        (int)sanae, (int)fractae,
        scribere_volo ? "" : " (consilium - nihil scriptum;"
            " -scribere applicat)");
    piscina_destruere(piscina);
    redde (fractae > ZEPHYRUM) ? I : ZEPHYRUM;
}
