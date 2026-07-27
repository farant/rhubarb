/* praeparator.c - praeparatio contextus communis (vide .h)
 *
 * Implementationes ex sessio.c levatae (exemplar recentissimum -
 * sanationem latinae-in-systemate fert). Receptum percursus fidele.
 */

#include "praeparator.h"

#include "latina.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include "silva_lexicon.h"

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

/* tempus capitis commemorare (via copiatur in piscinam - fontes
 * saepe in quadris effimeris) */
interior vacuum
_tempus_commemorare (Praeparatio* p, Piscina* piscina_capitum,
    constans character* via)
{
    PraeparatorCaputTempus* introitus;
    memoriae_index m;
    character* copia;

    si (p->tempora_capitum == NIHIL)
    {
        redde;
    }
    m = strlen(via);
    copia = (character*)piscina_allocare(piscina_capitum, m + I);
    si (copia == NIHIL)
    {
        redde;
    }
    memcpy(copia, via, m + I);
    introitus = (PraeparatorCaputTempus*)xar_addere(
        p->tempora_capitum);
    si (introitus != NIHIL)
    {
        introitus->via = copia;
        introitus->tempus = praeparator_tempus_plagulae(copia);
    }
}

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
    TabulaDispersa* visa, constans character* via,
    memoriae_index radix_m)
{
    DIR* dir = opendir(via);
    structura dirent* introitus;

    si (dir == NIHIL)
    {
        redde;
    }
    /* DIRECTORIUM ipsum commemoratur (2026-07-14, inventio capitum
     * NOVORUM): plagula nova/deleta/renominata mtime directorii
     * parentis pulsat (POSIX) - caput_stalum ita creationem videt,
     * quam tempora capitum notorum sola videre non possunt.
     * ANTE percursum stat-atur: creatio durante ambulatione
     * proximo examine detegitur (conservativum). */
    _tempus_commemorare(p, piscina_capitum, via);
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
            _capita_praeparare(p, piscina_capitum, visa, via_plena,
                radix_m);
        }
        alioquin
        {
            m = strlen(introitus->d_name);
            si (m >= III && introitus->d_name[m - II] == '.'
                && introitus->d_name[m - I] == 'h')
            {
                /* clavis = via RADICI-RELATIVA (01KYJ6740K):
                 * expansor canonicam + basename seponit; resolutio
                 * includenti-relativa viam plenam petit. Spatium
                 * viarum legati radici-relativum est (ordines,
                 * extenta) - clavis absoluta ei aliena esset;
                 * valores viae_capitum absoluti manent (URIs) */
                constans character* via_clavis = via_plena;
                chorda clavis;
                character* textus;
                insignatus integer mensura;

                si (radix_m > ZEPHYRUM
                    && strlen(via_plena) > radix_m + I
                    && via_plena[radix_m] == '/')
                {
                    via_clavis = via_plena + radix_m + I;
                }
                clavis = chorda_ex_literis(via_clavis,
                    piscina_capitum);
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
                        via_clavis, textus, mensura))
                {
                    (vacuum)tabula_dispersa_inserere(visa, clavis,
                        NIHIL);
                    _tempus_commemorare(p, piscina_capitum,
                        via_plena);
                    /* basename -> via absoluta (saltus in capita:
                     * legatus definitio URIs inde struit). Clavis
                     * BASENAME manet quamvis praebitio via plena
                     * clavetur (01KYJ6740K) - primus vincit, ut
                     * ante */
                    si (p->viae_capitum != NIHIL)
                    {
                        chorda basis = chorda_ex_literis(
                            introitus->d_name, piscina_capitum);

                        si (!tabula_dispersa_continet(
                                p->viae_capitum, basis))
                        {
                            memoriae_index mv = strlen(via_plena);
                            character* copia = (character*)
                                piscina_allocare(piscina_capitum,
                                    mv + I);

                            si (copia != NIHIL)
                            {
                                memcpy(copia, via_plena, mv + I);
                                (vacuum)tabula_dispersa_inserere(
                                    p->viae_capitum, basis, copia);
                            }
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
    p->tempora_capitum = xar_creare(piscina_capitum,
        (i32)magnitudo(PraeparatorCaputTempus));

    /* systema: ISO [+POSIX totum-aut-derivatum +externa] [+latina]
     * concatenata in TEXTUM UNUM - lexicon = canalis macrorum;
     * typedefs per parsuram systematis + oraculum fluunt (lectio
     * chunk B: lexicon separatum custodem definit -> inclusio vera
     * tacet -> typedefs evanescunt). Compositio POSIX/externa in
     * silva_lexicon UNO loco (design B 2026-07-27, eadem quam
     * examen/percursus): cum_posix totum onerat ut ante (escape);
     * fons_plagulae sectiones ex inclusionibus plagulae derivat
     * blocosque externa eius excerpit. */
    {
        character via_sys[600];
        insignatus integer m_iso = ZEPHYRUM;
        insignatus integer m_px = ZEPHYRUM;
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
        _tempus_commemorare(p, piscina_capitum, via_sys);
        mensura_sys = m_iso;
        /* POSIX SEMPER lectus et vigilatus (etiam si systema basis
         * eum non texit): consumptores clavem derivationis ex
         * p->fons_posix legunt, et staleness editiones eius videt
         * antequam praeparatio derivata mendax fiat */
        sprintf(via_sys, "%s/silva/fontes/systema_posix.h",
            cfg->radix);
        p->fons_posix = praeparator_plagulam_legere(piscina_capitum,
            via_sys, &m_px);
        si (p->fons_posix == NIHIL)
        {
            redde ZEPHYRUM;
        }
        p->mensura_posix = m_px;
        _tempus_commemorare(p, piscina_capitum, via_sys);
        si (cfg->cum_posix || cfg->fons_plagulae != NIHIL)
        {
            character* compositum;
            i32 m_comp = ZEPHYRUM;
            b32 fractum = FALSUM;

            compositum = silva_lexicon_componere(fons_sys,
                (i32)m_iso, p->fons_posix, (i32)m_px,
                cfg->fons_plagulae, (i32)cfg->mensura_plagulae,
                cfg->cum_posix ? VERUM : FALSUM, piscina_capitum,
                &m_comp, cfg->via_plagulae != NIHIL
                    ? cfg->via_plagulae : "(plagula)", &fractum);
            si (fractum || compositum == NIHIL)
            {
                /* externa fracta: nuntius iam in stderr; iudicium
                 * mundum ex annotatione prava numquam */
                redde ZEPHYRUM;
            }
            fons_sys = compositum;
            mensura_sys = (insignatus integer)m_comp;
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
            _tempus_commemorare(p, piscina_capitum, via_sys);
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
            _capita_praeparare(p, piscina_capitum, visa, cfg->radix,
                strlen(cfg->radix));
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

constans character*
praeparator_caput_stalum (constans Praeparatio* p)
{
    i32 n;
    i32 i;

    si (p == NIHIL || p->tempora_capitum == NIHIL)
    {
        redde NIHIL;
    }
    n = xar_numerus(p->tempora_capitum);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans PraeparatorCaputTempus* introitus =
            (constans PraeparatorCaputTempus*)xar_obtinere(
                p->tempora_capitum, i);

        /* identitas cum se ipso: differentia quaevis (etiam retro -
         * git checkout) = stalum */
        si (introitus != NIHIL
            && praeparator_tempus_plagulae(introitus->via)
                   != introitus->tempus)
        {
            redde introitus->via;
        }
    }
    redde NIHIL;
}
