/* praeparator.c - praeparatio contextus communis (vide .h)
 *
 * Implementationes ex sessio.c levatae (exemplar recentissimum -
 * sanationem latinae-in-systemate fert). Receptum percursus fidele.
 */

#include "praeparator.h"

#include "latina.h"
#include "chorda.h"
#include "tabula_dispersa.h"

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

character*
praeparator_plagulam_legere (Piscina* piscina,
    constans character* via, insignatus integer* mensura_out)
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
    *mensura_out = (insignatus integer)mensura_l;
    redde textus;
}

longus
praeparator_tempus_plagulae (constans character* via)
{
    structura stat indicium;

    si (via == NIHIL || stat(via, &indicium) != ZEPHYRUM)
    {
        redde 0L;
    }
    redde (longus)indicium.st_mtime;
}

/* ==================================================
 * capita praebere (ambulatio dirent; basename primus-vincit)
 * ================================================== */

interior b32
_praetermittendum (constans character* titulus)
{
    redde (strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "results") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM)
        ? VERUM : FALSUM;
}

interior vacuum
_capita_praeparare (Praeparatio* p, Piscina* piscina_capitum,
    TabulaDispersa* visa, constans character* via)
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
            _capita_praeparare(p, piscina_capitum, visa, via_plena);
        }
        alioquin
        {
            m = strlen(introitus->d_name);
            si (m >= III && introitus->d_name[m - II] == '.'
                && introitus->d_name[m - I] == 'h')
            {
                chorda clavis = chorda_ex_literis(introitus->d_name,
                    piscina_capitum);
                character* textus;
                insignatus integer mensura;

                si (tabula_dispersa_continet(visa, clavis))
                {
                    perge;
                }
                textus = praeparator_plagulam_legere(piscina_capitum,
                    via_plena, &mensura);
                si (textus == NIHIL)
                {
                    perge;
                }
                si (silva_contextus_praebere(p->ctx,
                        introitus->d_name, textus, mensura))
                {
                    (vacuum)tabula_dispersa_inserere(visa, clavis,
                        NIHIL);
                    /* basename -> via absoluta (saltus in capita:
                     * legatus definitio URIs inde struit) */
                    si (p->viae_capitum != NIHIL)
                    {
                        memoriae_index mv = strlen(via_plena);
                        character* copia = (character*)
                            piscina_allocare(piscina_capitum,
                                mv + I);

                        si (copia != NIHIL)
                        {
                            memcpy(copia, via_plena, mv + I);
                            (vacuum)tabula_dispersa_inserere(
                                p->viae_capitum, clavis, copia);
                        }
                    }
                }
            }
        }
    }
    closedir(dir);
}

/* ==================================================
 * praeparare / destruere
 * ================================================== */

integer
praeparator_praeparare (Praeparatio* p, Piscina* piscina_capitum,
    constans PraeparatorConfiguratio* cfg)
{
    si (p == NIHIL || piscina_capitum == NIHIL || cfg == NIHIL)
    {
        redde ZEPHYRUM;
    }
    memset(p, ZEPHYRUM, magnitudo(Praeparatio));
    p->piscina = silva_piscina_generare_dynamicum(
        "praeparator_silva", 67108864);
    si (p->piscina == NIHIL)
    {
        redde ZEPHYRUM;
    }
    p->ctx = silva_contextus_creare(p->piscina);
    si (p->ctx == NIHIL)
    {
        redde ZEPHYRUM;
    }
    si (cfg->radix == NIHIL)
    {
        redde I;   /* sine systemate et capitibus - contextus nudus */
    }

    /* systema: ISO [+POSIX] [+latina] concatenata in TEXTUM UNUM -
     * lexicon = canalis macrorum; typedefs per parsuram systematis
     * + oraculum fluunt (lectio chunk B: lexicon separatum custodem
     * definit -> inclusio vera tacet -> typedefs evanescunt) */
    {
        character via_sys[600];
        insignatus integer m_iso = ZEPHYRUM;
        character* fons_sys;
        insignatus integer mensura_sys;

        sprintf(via_sys, "%s/silva/fontes/systema_c89.h",
            cfg->radix);
        fons_sys = praeparator_plagulam_legere(piscina_capitum,
            via_sys, &m_iso);
        si (fons_sys == NIHIL)
        {
            redde ZEPHYRUM;
        }
        mensura_sys = m_iso;
        si (cfg->cum_posix)
        {
            insignatus integer m_px = ZEPHYRUM;
            character* fons_px;
            character* iunctum;

            sprintf(via_sys, "%s/silva/fontes/systema_posix.h",
                cfg->radix);
            fons_px = praeparator_plagulam_legere(piscina_capitum,
                via_sys, &m_px);
            si (fons_px == NIHIL)
            {
                redde ZEPHYRUM;
            }
            iunctum = (character*)piscina_allocare(piscina_capitum,
                (memoriae_index)(mensura_sys + m_px + II));
            si (iunctum == NIHIL)
            {
                redde ZEPHYRUM;
            }
            memcpy(iunctum, fons_sys, (memoriae_index)mensura_sys);
            iunctum[mensura_sys] = '\n';
            memcpy(iunctum + mensura_sys + I, fons_px,
                (memoriae_index)m_px);
            mensura_sys = mensura_sys + I + m_px;
            iunctum[mensura_sys] = '\0';
            fons_sys = iunctum;
        }
        si (cfg->cum_latina)
        {
            insignatus integer m_lat = ZEPHYRUM;
            character* fons_lat;
            character* iunctum;

            sprintf(via_sys, "%s/include/latina.h", cfg->radix);
            fons_lat = praeparator_plagulam_legere(piscina_capitum,
                via_sys, &m_lat);
            si (fons_lat == NIHIL)
            {
                redde ZEPHYRUM;
            }
            iunctum = (character*)piscina_allocare(piscina_capitum,
                (memoriae_index)(mensura_sys + m_lat + II));
            si (iunctum == NIHIL)
            {
                redde ZEPHYRUM;
            }
            memcpy(iunctum, fons_sys, (memoriae_index)mensura_sys);
            iunctum[mensura_sys] = '\n';
            memcpy(iunctum + mensura_sys + I, fons_lat,
                (memoriae_index)m_lat);
            mensura_sys = mensura_sys + I + m_lat;
            iunctum[mensura_sys] = '\0';
            fons_sys = iunctum;
        }
        si (!silva_contextus_lexicon_addere(p->ctx, "systema_c89.h",
                fons_sys, mensura_sys))
        {
            redde ZEPHYRUM;
        }
        p->systema_parsura = silva_c89_parsare(p->piscina,
            "systema_c89.h", fons_sys, mensura_sys, NIHIL);
        si (p->systema_parsura == NIHIL
            || p->systema_parsura->numerus_errorum > ZEPHYRUM)
        {
            redde ZEPHYRUM;
        }
        p->systema_semantica = silva_c89_semantica_analysare(
            p->piscina, p->systema_parsura);
        si (p->systema_semantica == NIHIL)
        {
            redde ZEPHYRUM;
        }
    }

    si (!cfg->sine_capitibus)
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina_capitum, DXII);

        p->viae_capitum = tabula_dispersa_creare_chorda(
            piscina_capitum, DXII);
        si (visa != NIHIL)
        {
            _capita_praeparare(p, piscina_capitum, visa, cfg->radix);
        }
    }
    redde I;
}

vacuum
praeparator_destruere (Praeparatio* p)
{
    si (p == NIHIL)
    {
        redde;
    }
    si (p->piscina != NIHIL)
    {
        silva_piscina_destruere(p->piscina);
    }
    memset(p, ZEPHYRUM, magnitudo(Praeparatio));
}

/* ==================================================
 * receptum bis-analysis (percursus fidele)
 * ================================================== */

SilvaSemantica*
praeparator_analysare (constans Praeparatio* p,
    SilvaPiscina* effimera, constans character* via,
    constans character* fons, insignatus integer mensura,
    SilvaParsura** parsura_out)
{
    SilvaOraculum* oraculum;
    SilvaParsura* parsura;
    SilvaSemantica* sem;

    *parsura_out = NIHIL;
    si (p == NIHIL || effimera == NIHIL)
    {
        redde NIHIL;
    }
    oraculum = silva_oraculum_creare(effimera);
    si (oraculum != NIHIL && p->systema_semantica != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(
            p->systema_semantica, oraculum);
    }
    parsura = silva_c89_parsare_cum_contextu(effimera, p->ctx, via,
        fons, mensura, oraculum);
    si (parsura == NIHIL || !parsura->successus
        || parsura->commissio == NIHIL)
    {
        redde NIHIL;
    }
    sem = silva_c89_semantica_analysare_cum_systemate(effimera,
        parsura, p->systema_parsura);
    si (sem != NIHIL && oraculum != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(sem, oraculum);
        silva_oraculum_responsa_vacare(oraculum);
        (vacuum)silva_recanonicare(parsura->commissio, oraculum,
            silva_c89_resolutor, NIHIL);
        sem = silva_c89_semantica_analysare_cum_systemate(effimera,
            parsura, p->systema_parsura);
    }
    *parsura_out = parsura;
    redde sem;
}
