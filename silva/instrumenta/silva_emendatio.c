/* silva_emendatio.c - custodia emendationis (doctrina in capite) */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "via.h"
#include "processus.h"
#include "silva_c89_semantica.h"
#include "silva_iudicium.h"
#include "silva_emendatio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* numeri diagnosticorum per codicem */
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

    si (   fr->parsura != NIHIL
        && fr->parsura->numerus_errorum > ZEPHYRUM)
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

interior EmendatioVerdictum
_fractum (
               Piscina* opus,
    EmendatioVerdictum  v,
    constans character* causa)
{
    v.bene   = FALSUM;
    v.causa  = chorda_ex_literis(causa, opus);
    redde v;
}

EmendatioVerdictum
silva_emendationem_iudicare (
         IudiciumApparatus* app,
                   Piscina* opus,
       constans character* via,
    constans IudiciumFructus* ante,
       constans character* textus_novus,
                      i32  mensura_novi,
                      s32  codex,
                      s32  decrementum)
{
    EmendatioVerdictum v;
       IudiciumFructus post;
                   i32 numeri_ante[EXAMEN_CODEX_NUMERUS];
                   i32 numeri_post[EXAMEN_CODEX_NUMERUS];
                   s32 c;
             character nuntius[CCLVI];

    v.bene   = VERUM;
    v.causa  = chorda_ex_literis("", opus);
    v.ante   = ZEPHYRUM;
    v.post   = ZEPHYRUM;
    si (ante == NIHIL || ante->sem == NIHIL || ante->parsura == NIHIL)
    {
        redde _fractum(opus, v, "iudicium ante deest");
    }
    si (!silva_iudicium_iudicare(app, opus, via, textus_novus,
            mensura_novi, &post))
    {
        redde _fractum(opus, v, post.recusatum
            ? "iudicium post: fines examinis tactae"
            : "iudicium post fractum (parsura cum erroribus)");
    }
    si (post.parsura->numerus_errorum > ante->parsura->numerus_errorum)
    {
        sprintf(nuntius, "errores parsurae novi (%u -> %u)",
            (insignatus integer)ante->parsura->numerus_errorum,
            (insignatus integer)post.parsura->numerus_errorum);
        redde _fractum(opus, v, nuntius);
    }
    _numeros_colligere(ante->sem, numeri_ante);
    _numeros_colligere(post.sem, numeri_post);
    si (codex >= ZEPHYRUM && codex < (s32)EXAMEN_CODEX_NUMERUS)
    {
        s32 exspectatum;

        v.ante = numeri_ante[codex];
        v.post = numeri_post[codex];
        exspectatum  = decrementum == EMENDATIO_AD_NIHIL
            ? ZEPHYRUM : (s32)v.ante - decrementum;
        si ((s32)v.post != exspectatum)
        {
            sprintf(nuntius, "codex %d: %u -> %u, exspectatum %d",
                (integer)codex, (insignatus integer)v.ante,
                (insignatus integer)v.post, (integer)exspectatum);
            redde _fractum(opus, v, nuntius);
        }
    }
    per (c = ZEPHYRUM; c < (s32)EXAMEN_CODEX_NUMERUS; c++)
    {
        si (c != codex && numeri_post[c] > numeri_ante[c])
        {
            sprintf(nuntius, "codex %d crevit %u -> %u: %s", (integer)c,
                (insignatus integer)numeri_ante[c],
                (insignatus integer)numeri_post[c],
                silva_c89_codicis_causa(c));
            redde _fractum(opus, v, nuntius);
        }
    }
    si (!_reice_est(ante) && _reice_est(&post))
    {
        redde _fractum(opus, v, "REICE inductum");
    }
    redde v;
}

/* directorium temporarium unicum (mkdir atomicum: cursus concurrentes
 * nomina diversa accipiunt) */
interior character*
_directorium_temporarium (
    Piscina* piscina)
{
     constans character* radix = getenv("TMPDIR");
              character* via;
                    i32  k;

    si (radix == NIHIL || radix[ZEPHYRUM] == '\0')
    {
        radix = "/tmp";
    }
    via = (character*)piscina_allocare(piscina, strlen(radix) + LXIV);
    si (via == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < M; k++)
    {
        sprintf(via, "%s/transformare_custodia_%u", radix,
            (insignatus integer)k);
        si (filum_directorium_creare(via))
        {
            redde via;
        }
    }
    redde NIHIL;
}

/* lineae primae (V) errati */
interior chorda
_lineae_primae (
    chorda textus,
       i32 numerus)
{
    i32 k;
    i32 lineae = ZEPHYRUM;

    per (k = ZEPHYRUM; k < textus.mensura; k++)
    {
        si (textus.datum[k] == '\n' && ++lineae >= numerus)
        {
            redde chorda_sectio(textus, ZEPHYRUM, k);
        }
    }
    redde textus;
}

b32
silva_emendationem_compilare (
                     Piscina* piscina,
         constans character* via,
         constans character* textus,
                        i32  mensura,
    constans character* constans* vexilla,
                     chorda* erratum_out)
{
             character*  directorium;
             character*  temporaria;
             character*  origo;
                chorda   nomen_plagulae;
                chorda   directorium_originis;
    constans character** argumenta;
                   i32   n = ZEPHYRUM;
                   i32   k;
     ProcessusResultus   r;
                   b32   bene;
    unio {
        constans character* c;
                        i8* m;
    } contentum;

    *erratum_out  = chorda_ex_literis("", piscina);
    directorium   = _directorium_temporarium(piscina);
    si (directorium == NIHIL)
    {
        *erratum_out =
            chorda_ex_literis("directorium temporarium deest",
            piscina);
        redde FALSUM;
    }
    nomen_plagulae = via_nomen(chorda_ex_literis(via, piscina),
        piscina);
    directorium_originis = via_directorium(chorda_ex_literis(via,
        piscina), piscina);
    temporaria = (character*)piscina_allocare(piscina,
        strlen(directorium) + (memoriae_index)nomen_plagulae.mensura
            + II);
    origo = chorda_ut_cstr(directorium_originis, piscina);
    per (k = ZEPHYRUM; vexilla != NIHIL && vexilla[k] != NIHIL; k++)
    {
        n++;
    }
    argumenta = (constans character**)piscina_allocare(piscina,
        (memoriae_index)(n + VII) * magnitudo(character*));
    si (temporaria == NIHIL || origo == NIHIL || argumenta == NIHIL)
    {
        (vacuum)filum_arborem_delere(directorium);
        redde FALSUM;
    }
    sprintf(temporaria, "%s/%.*s", directorium,
        (integer)nomen_plagulae.mensura,
        (constans character*)nomen_plagulae.datum);
    contentum.c = textus;
    si (!filum_scribere(temporaria, chorda_ex_buffer(contentum.m,
            mensura)))
    {
        (vacuum)filum_arborem_delere(directorium);
        *erratum_out =
            chorda_ex_literis("plagula temporaria non scripta",
            piscina);
        redde FALSUM;
    }
    argumenta[ZEPHYRUM]  = "clang";
    argumenta[I]         = "-fsyntax-only";
    per (k = ZEPHYRUM; k < n; k++)
    {
        argumenta[II + k] = vexilla[k];
    }
    argumenta[II + n]   = "-iquote";
    argumenta[III + n]  = origo;
    argumenta[IV + n]   = temporaria;
    argumenta[V + n]    = NIHIL;
    r = processus_exsequi((constans character* constans*)argumenta,
        CXX * M, piscina);
    bene = r.successus && r.codex_exitus == ZEPHYRUM && r.signum
        == ZEPHYRUM;
    si (!bene)
    {
        *erratum_out = r.successus
            ? _lineae_primae(r.erratum, V)
            : chorda_ex_literis(processus_error_nomen(r.error),
            piscina);
    }
    (vacuum)filum_arborem_delere(directorium);
    redde bene;
}
