/* silva_iudicium.c - recipe iudicii examinis (doctrina in capite).
 * Mechanica ex examen.c principale transplantata verbatim ubi
 * potuit - divergentia recipe = divergentia verdicti (lectio
 * 01KYBAG1MJ: XXI violationes falsae ex semita divergente). */

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_contextus.h"
#include "silva_parsare.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_semantica.h"
#include "silva_lexicon.h"
#include "silva_iudicium.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>   /* instrumentum solum - macOS/POSIX */

character*
silva_iudicium_plagulam_legere (Piscina* piscina,
    constans character* via, i32* mensura_out)
{
    FILE* pl = fopen(via, "rb");
    long mensura_l;
    character* textus;

    *mensura_out = ZEPHYRUM;
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura_l > 0L ? mensura_l + 1L : I));
    si (textus == NIHIL
        || (mensura_l > 0L
            && fread(textus, I, (memoriae_index)mensura_l, pl)
                != (memoriae_index)mensura_l))
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    textus[mensura_l] = '\0';
    *mensura_out = (i32)mensura_l;
    redde textus;
}

interior b32
_praetermittendum (constans character* titulus)
{
    redde strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM;
}

interior vacuum
_capita_colligere (IudiciumApparatus* app, constans character* via)
{
    DIR* dir = opendir(via);
    structura dirent* introitus;

    si (dir == NIHIL)
    {
        redde;
    }
    dum ((introitus = readdir(dir)) != NIHIL)
    {
        character via_plena[1024];
        memoriae_index m;

        si (introitus->d_name[ZEPHYRUM] == '.')
        {
            perge;
        }
        si (_praetermittendum(introitus->d_name))
        {
            perge;
        }
        si (strlen(via) + strlen(introitus->d_name) + II
            >= magnitudo(via_plena))
        {
            perge;
        }
        sprintf(via_plena, "%s/%s", via, introitus->d_name);
        si (introitus->d_type == DT_DIR)
        {
            _capita_colligere(app, via_plena);
        }
        alioquin
        {
            m = strlen(introitus->d_name);
            si (m >= III && introitus->d_name[m - II] == '.'
                && introitus->d_name[m - I] == 'h')
            {
                IudiciumCaput* caput;
                i32 mensura = ZEPHYRUM;
                character* textus = silva_iudicium_plagulam_legere(
                    app->piscina, via_plena, &mensura);

                si (textus == NIHIL)
                {
                    perge;
                }
                caput = (IudiciumCaput*)xar_addere(app->capita);
                si (caput == NIHIL)
                {
                    perge;
                }
                caput->via = (character*)piscina_allocare(
                    app->piscina, strlen(via_plena) + I);
                si (caput->via == NIHIL)
                {
                    perge;
                }
                strcpy(caput->via, via_plena);
                caput->textus = textus;
                caput->mensura = mensura;
            }
        }
    }
    closedir(dir);
}

IudiciumApparatus*
silva_iudicium_praeparare (Piscina* piscina)
{
    IudiciumApparatus* app;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    app = (IudiciumApparatus*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(IudiciumApparatus));
    si (app == NIHIL)
    {
        redde NIHIL;
    }
    app->piscina = piscina;
    app->capita = xar_creare(piscina,
        (i32)magnitudo(IudiciumCaput));
    si (app->capita == NIHIL)
    {
        redde NIHIL;
    }
    app->fons_iso = silva_iudicium_plagulam_legere(piscina,
        "silva/fontes/systema_c89.h", &app->mensura_iso);
    app->fons_px = silva_iudicium_plagulam_legere(piscina,
        "silva/fontes/systema_posix.h", &app->mensura_px);
    si (app->fons_iso == NIHIL || app->fons_px == NIHIL)
    {
        fprintf(stderr, "iudicium: systema deest (curre ex radice"
            " repositorii)\n");
        redde NIHIL;
    }
    _capita_colligere(app, ".");
    redde app;
}

b32
silva_iudicium_iudicare (IudiciumApparatus* app, Piscina* opus,
    constans character* via, constans character* fons, i32 mensura,
    IudiciumFructus* fructus)
{
    SilvaContextus* ctx;
    SilvaParsura* systema_parsura;
    SilvaSemantica* systema_semantica;
    SilvaOraculum* oraculum;
    SilvaParsura* parsura;
    SilvaSemantica* sem;
    character* fons_sys;
    i32 mensura_sys = ZEPHYRUM;
    b32 ext_fractum = FALSUM;
    i32 i;
    i32 m;

    fructus->parsura = NIHIL;
    fructus->sem = NIHIL;
    fructus->apparatus_fractus = FALSUM;
    fructus->recusatum = FALSUM;

    ctx = silva_contextus_creare(opus);
    si (ctx == NIHIL)
    {
        fructus->apparatus_fractus = VERUM;
        redde FALSUM;
    }
    fons_sys = silva_lexicon_componere(app->fons_iso,
        app->mensura_iso, app->fons_px, app->mensura_px, fons,
        mensura, FALSUM, opus, &mensura_sys, via,
        &ext_fractum);
    si (ext_fractum || fons_sys == NIHIL)
    {
        fructus->apparatus_fractus = VERUM;
        redde FALSUM;
    }
    si (!silva_contextus_lexicon_addere(ctx, "systema_c89.h",
            fons_sys, mensura_sys))
    {
        fructus->apparatus_fractus = VERUM;
        redde FALSUM;
    }
    systema_parsura = silva_c89_parsare(opus,
        "systema_c89.h", fons_sys, mensura_sys, NIHIL);
    si (systema_parsura == NIHIL
        || systema_parsura->numerus_errorum > ZEPHYRUM)
    {
        fructus->apparatus_fractus = VERUM;
        redde FALSUM;
    }
    systema_semantica = silva_c89_semantica_analysare(opus,
        systema_parsura);
    si (systema_semantica == NIHIL)
    {
        fructus->apparatus_fractus = VERUM;
        redde FALSUM;
    }
    m = xar_numerus(app->capita);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans IudiciumCaput* caput = (constans IudiciumCaput*)
            xar_obtinere(app->capita, i);

        si (caput != NIHIL)
        {
            (vacuum)silva_contextus_praebere(ctx, caput->via,
                caput->textus, caput->mensura);
        }
    }
    oraculum = silva_oraculum_creare(opus);
    si (oraculum != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(
            systema_semantica, oraculum);
    }
    parsura = silva_c89_parsare_cum_contextu(opus, ctx, via,
        fons, mensura, oraculum);
    si (parsura == NIHIL || !parsura->successus
        || parsura->commissio == NIHIL)
    {
        fructus->apparatus_fractus = VERUM;
        redde FALSUM;
    }
    si (parsura->fines_tactae)
    {
        fructus->parsura = parsura;
        fructus->recusatum = VERUM;
        redde FALSUM;
    }
    sem = silva_c89_semantica_analysare_cum_systemate(opus,
        parsura, systema_parsura);
    si (sem != NIHIL && oraculum != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(sem, oraculum);
        silva_oraculum_responsa_vacare(oraculum);
        (vacuum)silva_recanonicare(parsura->commissio, oraculum,
            silva_c89_resolutor, NIHIL);
        sem = silva_c89_semantica_analysare_cum_systemate(
            opus, parsura, systema_parsura);
    }
    si (sem == NIHIL)
    {
        fructus->apparatus_fractus = VERUM;
        redde FALSUM;
    }
    fructus->parsura = parsura;
    fructus->sem = sem;
    redde VERUM;
}
