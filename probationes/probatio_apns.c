/* probatio_apns.c - apns sine rete (http2-plan T11): configuratio,
 * regula finis (spec III.4: BadDeviceToken transit; errores tesserae
 * aut thematis deficiunt; alia post signum nullum iterantur), signum
 * fortuitum. Petitio vera contra servum Go: probatio_apns_vivum. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "apns.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens Piscina* piscina;

interior vacuum
_parare (
    CredoContextus* c)
{
    piscina = c->piscina;
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    piscina = NIHIL;
}

interior b32
configurationem_scribere (
              character* via,
     constans character* textus)
{
    constans character* tmp = getenv("TMPDIR");

    si (!tmp || strlen(tmp) > CD)
    {
        redde FALSUM;
    }
    sprintf(via, "%s/apns_probatio.stml", tmp);
    redde filum_scribere(via, chorda_ex_literis(textus, piscina));
}

interior vacuum
_sectio_configuratio (
    CredoContextus* c)
{
    ApnsConfiguratio cfg;
              chorda causa;
           character via[DXII];

    (vacuum)c;
    CREDO_GRADUS("plena: campi, portus ordinarius CDXLIII");
    CREDO_NECESSE(configurationem_scribere(via, "<apns clavis_via=\""
        "/x/AuthKey_PROBA.p8\" clavis_id=\"PROBA12345\" turma=\""
        "TURMA12345\" thema=\"earth.golden.pictor\" hospes=\""
        "api.sandbox.push.apple.com\"/>"));
    CREDO_VERUM(apns_configurationem_legere(via, &cfg, &causa,
        piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(cfg.clavis_id, "PROBA12345");
    CREDO_CHORDA_AEQUALIS_LITERIS(cfg.turma, "TURMA12345");
    CREDO_CHORDA_AEQUALIS_LITERIS(cfg.thema, "earth.golden.pictor");
    CREDO_CHORDA_AEQUALIS_LITERIS(cfg.hospes,
        "api.sandbox.push.apple.com");
    CREDO_AEQUALIS_I32(cfg.portus, CDXLIII);
    CREDO_AEQUALIS_I32(cfg.ancora.mensura, ZEPHYRUM);

    CREDO_GRADUS("portus et ancora (probationes); ~/ expanditur");
    CREDO_NECESSE(configurationem_scribere(via, "<apns clavis_via=\""
        "~/AuthKey_PROBA.p8\" clavis_id=\"PROBA12345\" turma=\"TURMA12345\""
        " thema=\"earth.golden.pictor\" hospes=\"localhost\" portus=\""
        "8443\" ancora=\"/x/ca.der\"/>"));
    CREDO_VERUM(apns_configurationem_legere(via, &cfg, &causa,
        piscina));
    CREDO_AEQUALIS_I32(cfg.portus, 0x20fb);
    CREDO_CHORDA_AEQUALIS_LITERIS(cfg.ancora, "/x/ca.der");
    CREDO_VERUM(cfg.clavis_via.mensura > ZEPHYRUM
        && cfg.clavis_via.datum[ZEPHYRUM] == '/');

    CREDO_GRADUS("campus absens nominatur; plagula absens");
    CREDO_NECESSE(configurationem_scribere(via,
        "<apns clavis_via=\"/x\""
        " clavis_id=\"PROBA12345\" thema=\"t\" hospes=\"h\"/>"));
    CREDO_FALSUM(apns_configurationem_legere(via, &cfg, &causa,
        piscina));
    CREDO_CHORDA_CONTINET(causa, chorda_ex_literis("turma", piscina));
    (vacuum)filum_delere(via);
    CREDO_FALSUM(apns_configurationem_legere(via, &cfg, &causa,
        piscina));
}

interior ApnsIudicium
iudicium (
                    b32  transita,
                    i32  status,
     constans character* ratio,
                    b32  signum_nullum)
{
    ApnsResponsum r;

    memset(&r, ZEPHYRUM, magnitudo(r));
    r.transita  = transita;
    r.status    = status;
    r.ratio     = chorda_ex_literis(ratio, piscina);
    redde apns_iudicare(&r, signum_nullum);
}

interior vacuum
_sectio_regula (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_GRADUS("BadDeviceToken transit");
    CREDO_AEQUALIS_I32((i32)iudicium(VERUM, CD, "BadDeviceToken",
        VERUM),
        (i32)APNS_TRANSITUS);
    CREDO_GRADUS("tessera aut thema mala deficiunt");
    CREDO_AEQUALIS_I32((i32)iudicium(VERUM, CDIII,
        "InvalidProviderToken",
        VERUM), (i32)APNS_DEFECTUS);
    CREDO_AEQUALIS_I32((i32)iudicium(VERUM, CDIII,
        "ExpiredProviderToken",
        FALSUM), (i32)APNS_DEFECTUS);
    CREDO_AEQUALIS_I32((i32)iudicium(VERUM, CDIII,
        "MissingProviderToken",
        VERUM), (i32)APNS_DEFECTUS);
    CREDO_AEQUALIS_I32((i32)iudicium(VERUM, CD, "TopicDisallowed",
        VERUM),
        (i32)APNS_DEFECTUS);
    CREDO_AEQUALIS_I32((i32)iudicium(VERUM, CD, "BadTopic", VERUM),
        (i32)APNS_DEFECTUS);
    CREDO_GRADUS("aliud post signum nullum: iterandum; post fortuitum: transit");
    CREDO_AEQUALIS_I32((i32)iudicium(VERUM, CD,
        "DeviceTokenNotForTopic",
        VERUM), (i32)APNS_ITERANDUM);
    CREDO_AEQUALIS_I32((i32)iudicium(VERUM, CD,
        "DeviceTokenNotForTopic",
        FALSUM), (i32)APNS_TRANSITUS);
    CREDO_GRADUS("transportus fractus deficit");
    CREDO_AEQUALIS_I32((i32)iudicium(FALSUM, ZEPHYRUM, "", VERUM),
        (i32)APNS_DEFECTUS);
}

interior vacuum
_sectio_signum (
    CredoContextus* c)
{
    character a[LXV];
    character b[LXV];
          i32 i;
          b32 hex;
          b32 nullum;

    (vacuum)c;
    CREDO_GRADUS("LXIV hex minusculae, non nullum, semine mutatum");
    apns_signum_fortuitum(a, 1792800000);
    apns_signum_fortuitum(b, 1792800001);
    CREDO_AEQUALIS_I32((i32)strlen(a), LXIV);
    hex     = VERUM;
    nullum  = VERUM;
    per (i = ZEPHYRUM; i < LXIV; i++)
    {
        si (!((a[i] >= '0' && a[i] <= '9')
            || (a[i] >= 'a' && a[i] <= 'f')))
        {
            hex = FALSUM;
        }
        si (a[i] != '0')
        {
            nullum = FALSUM;
        }
    }
    CREDO_VERUM(hex);
    CREDO_FALSUM(nullum);
    CREDO_VERUM(strcmp(a, b) != ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "configuratio", _sectio_configuratio, _parare, _purgare, NIHIL,
        NIHIL },
    { "regula (spec III.4)", _sectio_regula, _parare, _purgare, NIHIL,
        NIHIL },
    { "signum fortuitum", _sectio_signum, _parare, _purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("apns", SECTIONES);
}
