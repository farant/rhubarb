/* vigilia.c - vigilia processus residentis (vide vigilia.h)
 *
 * Extracta ex tabularium.c (vigilia disci, K2.1) + legatus.c
 * (vigilia fontium, excubitor chunk 3) - forma tertia manu-voluta
 * PRAEVENTA (regula duorum ad tria hic stetit). */

#include "vigilia.h"
#include "sigillum.h"
#include "xar.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>   /* sutura POSIX - macOS ex decreto */

structura Vigilia {
    Piscina*            piscina;
    VigiliaStatus       status;
    constans character* signum;          /* hex plenum aut NIHIL */
    character           signum_breve[XVII];
    constans character* via_binarii;     /* NIHIL = quieta */
    Xar*                fontes;          /* character* (NUL-term) */
    s64                 ortus_mtempus;   /* nanosecundis */
    s64                 ortus_magnitudo;
    constans character* cautio;          /* NIHIL dum recens */
    constans character* causa;           /* "" dum recens */

    /* politica tacendi (2026-07-17): agnitio explicita */
    i32                 tacita_reliqua;
    constans character* causa_tacita;    /* fingerprint agnitionis */
    VigiliaStatus       status_tacitus;
    MomentumSec         tempus_ultimum;  /* secunda; horologium iniectum */
    s64                 commissi_mtempus; /* stampa ad agnitionem (ns) */
};

/* ==================================================
 * auxilia
 * ================================================== */

/* mtempus nanosecundis (st_mtimespec = nomen verum macOS; fenestra
 * eiusdem-secundi clausa - parcum 'Mtimes nanosecundorum') */
interior s64
_mtempus_ns (constans structura stat* status_disci)
{
    redde (s64)status_disci->st_mtimespec.tv_sec
            * (s64)1000000000
        + (s64)status_disci->st_mtimespec.tv_nsec;
}

interior constans character*
_litterae_copiare (Piscina* piscina, constans character* fons)
{
    memoriae_index m = strlen(fons);
    character* copia = (character*)piscina_allocare(piscina, m + I);

    si (copia == NIHIL)
    {
        redde "";
    }
    memcpy(copia, fons, m);
    copia[m] = '\0';
    redde copia;
}

/* plagulam totam legere (NUL appenso); NIHIL si illegibilis */
interior character*
_plagulam_legere (Piscina* piscina, constans character* via,
    i32* mensura_out)
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

/* manifestum onerare: via una per lineam, vacuae + '#' omissae */
interior vacuum
_manifestum_onerare (Vigilia* v, constans character* via)
{
    i32 mensura = ZEPHYRUM;
    character* textus = _plagulam_legere(v->piscina, via,
        &mensura);
    i32 cursor = ZEPHYRUM;

    si (textus == NIHIL)
    {
        redde;   /* manifestum abest = vigilia fontium quieta */
    }
    v->fontes = xar_creare(v->piscina,
        (i32)magnitudo(character*));
    si (v->fontes == NIHIL)
    {
        redde;
    }
    dum (cursor < mensura)
    {
        i32 initium = cursor;
        i32 finis;

        dum (cursor < mensura && textus[cursor] != '\n')
        {
            cursor++;
        }
        finis = cursor;
        si (cursor < mensura)
        {
            cursor++;
        }
        si (finis == initium || textus[initium] == '#')
        {
            perge;
        }
        {
            character** locus = (character**)xar_addere(v->fontes);

            si (locus != NIHIL)
            {
                character* via_f = (character*)piscina_allocare(
                    v->piscina,
                    (memoriae_index)(finis - initium) + I);

                si (via_f != NIHIL)
                {
                    memcpy(via_f, textus + initium,
                        (memoriae_index)(finis - initium));
                    via_f[finis - initium] = '\0';
                    *locus = via_f;
                }
                alioquin
                {
                    *locus = NIHIL;
                }
            }
        }
    }
}

/* cautionem figere (glutinosa; textus in piscinam perennem) */
interior vacuum
_figere (Vigilia* v, VigiliaStatus status,
    constans character* causa)
{
    character* buf;
    memoriae_index m = strlen(causa);

    v->status = status;
    v->causa = _litterae_copiare(v->piscina, causa);
    buf = (character*)piscina_allocare(v->piscina,
        m + (memoriae_index)CXCII);
    si (buf == NIHIL)
    {
        v->cautio = "CAUTIO VIGILIAE: residens obsoletus - /mcp"
            " reconnect";
        redde;
    }
    si (status == VIGILIA_BINARIUM_NOVIUS)
    {
        sprintf(buf, "CAUTIO VIGILIAE: binarium in disco novius"
            " me - instrumentum renovare voca (aut /mcp"
            " reconnect)");
    }
    alioquin
    {
        sprintf(buf, "CAUTIO VIGILIAE: fontes me superant (%s"
            " recentior binario) - instrumentum renovare voca"
            " (aut reaedificatio + /mcp reconnect)", causa);
    }
    v->cautio = buf;
}

/* ==================================================
 * facies publica
 * ================================================== */

Vigilia*
vigilia_creare (Piscina* piscina,
    constans VigiliaConfiguratio* cfg)
{
    Vigilia* v;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    v = (Vigilia*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)magnitudo(Vigilia),
        (memoriae_index)magnitudo(vacuum*));
    si (v == NIHIL)
    {
        redde NIHIL;
    }
    memset(v, ZEPHYRUM, magnitudo(Vigilia));
    v->piscina = piscina;
    v->causa = "";
    si (cfg == NIHIL)
    {
        redde v;   /* quieta */
    }
    si (cfg->signum != NIHIL)
    {
        memoriae_index m = strlen(cfg->signum);
        memoriae_index breve = m < (memoriae_index)XVI
            ? m : (memoriae_index)XVI;

        v->signum = _litterae_copiare(piscina, cfg->signum);
        memcpy(v->signum_breve, cfg->signum, breve);
        v->signum_breve[breve] = '\0';
    }
    si (cfg->via_binarii != NIHIL)
    {
        structura stat status_disci;

        si (stat(cfg->via_binarii, &status_disci) == ZEPHYRUM)
        {
            v->via_binarii = _litterae_copiare(piscina,
                cfg->via_binarii);
            v->ortus_mtempus = _mtempus_ns(&status_disci);
            v->ortus_magnitudo = (s64)status_disci.st_size;
        }
    }
    si (cfg->via_manifesti != NIHIL && v->via_binarii != NIHIL)
    {
        _manifestum_onerare(v, cfg->via_manifesti);
    }
    redde v;
}

VigiliaStatus
vigilia_inspicere (Vigilia* vigilia, Piscina* effimera)
{
    si (vigilia == NIHIL)
    {
        redde VIGILIA_RECENS;
    }
    si (vigilia->status == VIGILIA_BINARIUM_NOVIUS
        || vigilia->via_binarii == NIHIL)
    {
        redde vigilia->status;   /* terminalis aut quieta */
    }
    /* vigilia disci (etiam ex FONTES_SUPERANT promovet -
     * reaedificatio facta nuntium meliorem meret) */
    si (vigilia->signum != NIHIL)
    {
        structura stat status_disci;

        si (stat(vigilia->via_binarii, &status_disci) == ZEPHYRUM
            && (_mtempus_ns(&status_disci) != vigilia->ortus_mtempus
                || (s64)status_disci.st_size
                    != vigilia->ortus_magnitudo))
        {
            i32 mensura = ZEPHYRUM;
            character* octeti = (effimera != NIHIL)
                ? _plagulam_legere(effimera, vigilia->via_binarii,
                      &mensura)
                : NIHIL;

            si (octeti != NIHIL)
            {
                Sigillum s = sigillum_computare(octeti,
                    (memoriae_index)mensura);
                character hex[SIGILLUM_HEX_MENSURA];

                sigillum_hex(&s, hex);
                si (strcmp(hex, vigilia->signum) == ZEPHYRUM)
                {
                    /* contentum idem - tactum/reaedificatum par */
                    vigilia->ortus_mtempus =
                        _mtempus_ns(&status_disci);
                    vigilia->ortus_magnitudo =
                        (s64)status_disci.st_size;
                }
                alioquin
                {
                    _figere(vigilia, VIGILIA_BINARIUM_NOVIUS,
                        vigilia->via_binarii);
                    redde vigilia->status;
                }
            }
        }
    }
    /* vigilia fontium (semel figitur - glutinosa) */
    si (vigilia->status == VIGILIA_RECENS
        && vigilia->fontes != NIHIL)
    {
        i32 k;

        per (k = ZEPHYRUM; k < xar_numerus(vigilia->fontes); k++)
        {
            character** via = (character**)xar_obtinere(
                vigilia->fontes, k);
            structura stat status_disci;

            si (via == NIHIL || *via == NIHIL)
            {
                perge;
            }
            si (stat(*via, &status_disci) == ZEPHYRUM
                && _mtempus_ns(&status_disci)
                    > vigilia->ortus_mtempus)
            {
                _figere(vigilia, VIGILIA_FONTES_SUPERANT, *via);
                frange;
            }
        }
    }
    redde vigilia->status;
}

VigiliaStatus
vigilia_status (constans Vigilia* vigilia)
{
    redde (vigilia != NIHIL) ? vigilia->status : VIGILIA_RECENS;
}

constans character*
vigilia_signum_breve (constans Vigilia* vigilia)
{
    redde (vigilia != NIHIL) ? vigilia->signum_breve : "";
}

constans character*
vigilia_cautio (constans Vigilia* vigilia)
{
    redde (vigilia != NIHIL) ? vigilia->cautio : NIHIL;
}

constans character*
vigilia_causa (constans Vigilia* vigilia)
{
    redde (vigilia != NIHIL) ? vigilia->causa : "";
}

/* stampa commissionis (ns; ZEPHYRUM = abest - excitator quietus) */
interior s64
_commissi_mtempus (vacuum)
{
    structura stat status_disci;

    si (stat(VIGILIA_VIA_COMMISSI, &status_disci) != ZEPHYRUM)
    {
        redde (s64)ZEPHYRUM;
    }
    redde _mtempus_ns(&status_disci);
}

b32
vigilia_tacere (Vigilia* vigilia, i32 responsa, MomentumSec nunc)
{
    si (vigilia == NIHIL || vigilia->cautio == NIHIL
        || responsa <= ZEPHYRUM)
    {
        redde FALSUM;   /* nihil tacendum */
    }
    si (responsa > D)
    {
        responsa = D;   /* limen sanitatis */
    }
    vigilia->tacita_reliqua = responsa;
    vigilia->causa_tacita = vigilia->causa;
    vigilia->status_tacitus = vigilia->status;
    vigilia->tempus_ultimum = nunc;
    vigilia->commissi_mtempus = _commissi_mtempus();
    redde VERUM;
}

constans character*
vigilia_cautio_dicenda (Vigilia* vigilia, MomentumSec nunc)
{
    MoraSec quies;   /* momentum - momentum = mora (algebra signata) */

    si (vigilia == NIHIL)
    {
        redde NIHIL;
    }
    quies = nunc - vigilia->tempus_ultimum;
    vigilia->tempus_ultimum = nunc;
    si (vigilia->cautio == NIHIL)
    {
        redde NIHIL;
    }
    si (vigilia->tacita_reliqua <= ZEPHYRUM)
    {
        redde vigilia->cautio;
    }
    /* re-armatio: quies longa - lector fortasse alius (compactio) */
    si (quies >= (s64)VIGILIA_QUIES_SECUNDA)
    {
        vigilia->tacita_reliqua = ZEPHYRUM;
        redde vigilia->cautio;
    }
    /* re-armatio: causa aut status NOVUS - agnitio vetus novum
     * nuntium non tegit */
    si (vigilia->status != vigilia->status_tacitus
        || vigilia->causa_tacita == NIHIL
        || strcmp(vigilia->causa, vigilia->causa_tacita) != ZEPHYRUM)
    {
        vigilia->tacita_reliqua = ZEPHYRUM;
        redde vigilia->cautio;
    }
    /* re-armatio: commissio - mundus mutatus, limen semanticum */
    si (_commissi_mtempus() != vigilia->commissi_mtempus)
    {
        vigilia->tacita_reliqua = ZEPHYRUM;
        redde vigilia->cautio;
    }
    vigilia->tacita_reliqua--;
    redde NIHIL;
}

b32
vigilia_continet (constans Vigilia* vigilia, chorda via)
{
    i32 k;

    si (vigilia == NIHIL || vigilia->fontes == NIHIL
        || via.mensura == ZEPHYRUM || via.datum == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(vigilia->fontes); k++)
    {
        character** locus = (character**)xar_obtinere(
            vigilia->fontes, k);
        memoriae_index f_m;

        si (locus == NIHIL || *locus == NIHIL)
        {
            perge;
        }
        f_m = strlen(*locus);
        si (f_m < (memoriae_index)via.mensura)
        {
            perge;
        }
        si (memcmp(*locus + (f_m - (memoriae_index)via.mensura),
                via.datum, (memoriae_index)via.mensura)
            != ZEPHYRUM)
        {
            perge;
        }
        si (f_m == (memoriae_index)via.mensura
            || (*locus)[f_m - (memoriae_index)via.mensura - I]
                == '/')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}
