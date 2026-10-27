/* probatio_vigilia.c - vigilia processus residentis (lib/vigilia)
 *
 * Fixturae = plagulae fictae in build/. Determinismus SINE mora:
 * mtempora nanosecundis (APFS) - plagula post alteram scripta
 * strictissime recentior est etiam intra idem secundum (lex ns,
 * parcum 'Mtimes nanosecundorum'). */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "vigilia.h"
#include "sigillum.h"
#include "credo.h"
#include "lectiones.h"
#include "filum.h"
#include <stdio.h>
#include <string.h>

#define VIA_BIN "build/probatio_vigilia_binarium"
#define VIA_MAN "build/probatio_vigilia_manifestum"
#define VIA_FON "build/probatio_vigilia_fons.c"
/* stampa commissionis PROPRIA (vigilia_viam_commissi_ponere): vera
 * radicis (VIGILIA_VIA_COMMISSI) residentium est, numquam tangenda */
#define VIA_COM "build/probatio_vigilia_commissum"

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    (vacuum)filum_delere(VIA_BIN);
    (vacuum)filum_delere(VIA_MAN);
    (vacuum)filum_delere(VIA_FON);
    (vacuum)filum_delere(VIA_COM);
}

interior b32
_scribere (
    constans character* via,
    constans character* contentum)
{
    FILE* pl = lectiones_fopen(via, "wb");

    si (pl == NIHIL)
    {
        redde FALSUM;
    }
    fputs(contentum, pl);
    fclose(pl);
    redde VERUM;
}

interior vacuum
_sigillum_hex (
    constans character* contentum,
             character  hex[SIGILLUM_HEX_MENSURA])
{
    Sigillum s = sigillum_computare(contentum, strlen(contentum));

    sigillum_hex(&s, hex);
}

interior chorda
_ch (
    constans character* litterae)
{
    chorda c;
    unio { constans character* l; i8* m; } u;

    u.l        = litterae;
    c.datum    = u.m;
    c.mensura  = (i32)strlen(litterae);
    redde c;
}


    /* ========================================================
     * I. Quieta: cfg NIHIL / campi absentes = numquam monet
     * ======================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                    Vigilia* v = vigilia_creare(piscina, NIHIL);
        VigiliaConfiguratio  cfg;

        CREDO_NON_NIHIL (v);
        CREDO_VERUM (vigilia_inspicere(v, piscina)
            == VIGILIA_RECENS);
        CREDO_VERUM (vigilia_cautio(v) == NIHIL);
        CREDO_VERUM (strcmp(vigilia_signum_breve(v), "")
            == ZEPHYRUM);

        cfg.signum         = "abcd";
        cfg.via_binarii    = NIHIL;
        cfg.via_manifesti  = NIHIL;
        v                  = vigilia_creare(piscina, &cfg);
        CREDO_NON_NIHIL (v);
        CREDO_VERUM (vigilia_inspicere(v, piscina)
            == VIGILIA_RECENS);
        /* signum breve etiam sine vigilia disci */
        CREDO_VERUM (strcmp(vigilia_signum_breve(v), "abcd")
            == ZEPHYRUM);
    }
}


    /* ========================================================
     * II. Vigilia disci: contentum idem renovat tacite; contentum
     * alium figit BINARIUM_NOVIUS (glutinosum)
     * ======================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        VigiliaConfiguratio  cfg;
                    Vigilia* v;
                  character  hex[SIGILLUM_HEX_MENSURA];
         constans character* contentum = "binarium fictum v1";

        CREDO_VERUM (_scribere(VIA_BIN, contentum));
        _sigillum_hex(contentum, hex);
        cfg.signum         = hex;
        cfg.via_binarii    = VIA_BIN;
        cfg.via_manifesti  = NIHIL;
        v                  = vigilia_creare(piscina, &cfg);
        CREDO_NON_NIHIL (v);
        CREDO_VERUM (vigilia_inspicere(v, piscina)
            == VIGILIA_RECENS);
        /* signum breve = praefixum XVI hexorum */
        CREDO_AEQUALIS_I32 ((i32)strlen(vigilia_signum_breve(v)),
            XVI);
        CREDO_VERUM (memcmp(vigilia_signum_breve(v), hex, XVI)
            == ZEPHYRUM);

        /* rescriptio contento EODEM: mtempus movet (ns), sigillum
         * congruit -> renovatio tacita */
        CREDO_VERUM (_scribere(VIA_BIN, contentum));
        CREDO_VERUM (vigilia_inspicere(v, piscina)
            == VIGILIA_RECENS);
        CREDO_VERUM (vigilia_cautio(v) == NIHIL);

        /* rescriptio contento ALIO -> BINARIUM_NOVIUS + cautio */
        CREDO_VERUM (_scribere(VIA_BIN,
            "binarium fictum v2 - contentum aliud"));
        CREDO_VERUM (vigilia_inspicere(v, piscina)
            == VIGILIA_BINARIUM_NOVIUS);
        CREDO_NON_NIHIL (vigilia_cautio(v));
        CREDO_VERUM (strstr(vigilia_cautio(v),
            "binarium in disco novius") != NIHIL);
        CREDO_VERUM (strstr(vigilia_causa(v), VIA_BIN) != NIHIL);

        /* glutinosum: inspectio proxima idem */
        CREDO_VERUM (vigilia_inspicere(v, piscina)
            == VIGILIA_BINARIUM_NOVIUS);
    }
}


    /* ========================================================
     * III. Vigilia fontium (lex ns): fons post binarium scriptus
     * intra idem secundum detegitur; promotio FONTES ->
     * BINARIUM_NOVIUS cum discus quoque mutatur
     * ======================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        VigiliaConfiguratio  cfg;
                    Vigilia* v;
                  character  hex[SIGILLUM_HEX_MENSURA];
         constans character* contentum = "binarium fictum fontium";

        CREDO_VERUM (_scribere(VIA_BIN, contentum));
        _sigillum_hex(contentum, hex);
        CREDO_VERUM (_scribere(VIA_MAN,
            "# manifestum probationis\n"
            "\n"
            VIA_FON "\n"));
        cfg.signum         = hex;
        cfg.via_binarii    = VIA_BIN;
        cfg.via_manifesti  = VIA_MAN;
        v                  = vigilia_creare(piscina, &cfg);
        CREDO_NON_NIHIL (v);

        /* fons nondum exsistit -> quieta */
        CREDO_VERUM (vigilia_inspicere(v, piscina)
            == VIGILIA_RECENS);

        /* fons NUNC scriptus - post binarium, fortasse eodem
         * secundo: ns eum tamen recentiorem monstrant */
        CREDO_VERUM (_scribere(VIA_FON, "integer x;\n"));
        CREDO_VERUM (vigilia_inspicere(v, piscina)
            == VIGILIA_FONTES_SUPERANT);
        CREDO_NON_NIHIL (vigilia_cautio(v));
        CREDO_VERUM (strstr(vigilia_cautio(v),
            "fontes me superant") != NIHIL);
        CREDO_VERUM (strstr(vigilia_causa(v), VIA_FON) != NIHIL);

        /* promotio: discus quoque mutatur -> nuntius melior */
        CREDO_VERUM (_scribere(VIA_BIN,
            "binarium reaedificatum - contentum novum"));
        CREDO_VERUM (vigilia_inspicere(v, piscina)
            == VIGILIA_BINARIUM_NOVIUS);
        CREDO_VERUM (strstr(vigilia_cautio(v),
            "binarium in disco novius") != NIHIL);
    }
}


    /* ========================================================
     * IV. continet: congruentia suffixi cum limite '/'
     * ======================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        VigiliaConfiguratio  cfg;
                    Vigilia* v;
                  character  hex[SIGILLUM_HEX_MENSURA];
         constans character* contentum = "binarium continet";

        CREDO_VERUM (_scribere(VIA_BIN, contentum));
        _sigillum_hex(contentum, hex);
        CREDO_VERUM (_scribere(VIA_MAN,
            "lib/vigilia_ficta.c\n"
            "gesta/fontes/gesta_fictum.c\n"));
        cfg.signum         = hex;
        cfg.via_binarii    = VIA_BIN;
        cfg.via_manifesti  = VIA_MAN;
        v                  = vigilia_creare(piscina, &cfg);
        CREDO_NON_NIHIL (v);

        CREDO_VERUM (vigilia_continet(v,
            _ch("lib/vigilia_ficta.c")));
        CREDO_VERUM (vigilia_continet(v,
            _ch("vigilia_ficta.c")));       /* limes '/' */
        CREDO_VERUM (!vigilia_continet(v,
            _ch("ficta.c")));               /* limes '_' - non */
        CREDO_VERUM (vigilia_continet(v,
            _ch("gesta_fictum.c")));
        CREDO_VERUM (!vigilia_continet(v, _ch("nusquam.c")));
        CREDO_VERUM (!vigilia_continet(v, _ch("")));
    }
}


    /* ========================================================
     * V. Politica tacendi: agnitio explicita + re-armationes
     * quattuor (numerus/quies/commissio/causa) - horologium
     * iniectum, determinismus sine somnis
     * ======================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        VigiliaConfiguratio  cfg;
                    Vigilia* v;
                  character  hex[SIGILLUM_HEX_MENSURA];
         constans character* contentum  = "binarium tacendi";
                        s64  t          = (s64)1000;

        (vacuum)filum_delere(VIA_COM);
        CREDO_VERUM (_scribere(VIA_BIN, contentum));
        _sigillum_hex(contentum, hex);
        CREDO_VERUM (_scribere(VIA_MAN, VIA_FON "\n"));
        cfg.signum         = hex;
        cfg.via_binarii    = VIA_BIN;
        cfg.via_manifesti  = VIA_MAN;
        v                  = vigilia_creare(piscina, &cfg);
        CREDO_NON_NIHIL (v);
        vigilia_viam_commissi_ponere(v, VIA_COM);

        /* recens: nihil tacendum; dicenda NIHIL */
        CREDO_VERUM (!vigilia_tacere(v, X, t));
        CREDO_VERUM (vigilia_cautio_dicenda(v, t) == NIHIL);

        /* fons superat -> cautio dicenda */
        CREDO_VERUM (_scribere(VIA_FON, "integer x;\n"));
        CREDO_VERUM (vigilia_inspicere(v, piscina)
            == VIGILIA_FONTES_SUPERANT);
        t++;
        CREDO_NON_NIHIL (vigilia_cautio_dicenda(v, t));

        /* NUMERUS: III tacita, quartum loquitur */
        CREDO_VERUM (vigilia_tacere(v, III, t));
        t++;
        CREDO_VERUM (vigilia_cautio_dicenda(v, t) == NIHIL);
        t++;
        CREDO_VERUM (vigilia_cautio_dicenda(v, t) == NIHIL);
        t++;
        CREDO_VERUM (vigilia_cautio_dicenda(v, t) == NIHIL);
        t++;
        CREDO_NON_NIHIL (vigilia_cautio_dicenda(v, t));

        /* QUIES: saltus horologii >= CCC re-armat ante numerum */
        CREDO_VERUM (vigilia_tacere(v, X, t));
        t++;
        CREDO_VERUM (vigilia_cautio_dicenda(v, t) == NIHIL);
        t += (s64)CCC;
        CREDO_NON_NIHIL (vigilia_cautio_dicenda(v, t));

        /* COMMISSIO: stampa mota re-armat */
        CREDO_VERUM (vigilia_tacere(v, X, t));
        t++;
        CREDO_VERUM (vigilia_cautio_dicenda(v, t) == NIHIL);
        CREDO_VERUM (_scribere(VIA_COM, "stampa\n"));
        t++;
        CREDO_NON_NIHIL (vigilia_cautio_dicenda(v, t));

        /* CAUSA NOVA: promotio fontes -> binarium loquitur per
         * agnitionem veterem */
        CREDO_VERUM (vigilia_tacere(v, X, t));
        t++;
        CREDO_VERUM (vigilia_cautio_dicenda(v, t) == NIHIL);
        CREDO_VERUM (_scribere(VIA_BIN, "binarium reaedificatum"));
        CREDO_VERUM (vigilia_inspicere(v, piscina)
            == VIGILIA_BINARIUM_NOVIUS);
        t++;
        CREDO_NON_NIHIL (vigilia_cautio_dicenda(v, t));

        (vacuum)filum_delere(VIA_COM);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "Quieta: cfg NIHIL / campi absentes = numquam monet",
        _sectio_prima, _purgare, _purgare, NIHIL, NIHIL },
    { "Vigilia disci: renovatio tacita, BINARIUM_NOVIUS glutinosum",
        _sectio_secunda, _purgare, _purgare, NIHIL, NIHIL },
    { "Vigilia fontium (lex ns): promotio FONTES -> BINARIUM_NOVIUS",
        _sectio_tertia, _purgare, _purgare, NIHIL, NIHIL },
    { "continet: congruentia suffixi cum limite '/'", _sectio_quarta,
        _purgare, _purgare, NIHIL, NIHIL },
    { "Politica tacendi: agnitio explicita + re-armationes quattuor",
        _sectio_quinta, _purgare, _purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vigilia", SECTIONES);
}
