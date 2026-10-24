/* probatio_fiducia_diffisa.c - indicium diffisarum domus (fiducia-linux
 * T2): subiectum+clavis certificatum eius clavis OMNE (geminus: clavis
 * eadem, series alia - instar transsignati) reicit; emittens+series
 * unum, octetis exactis (0x00 initiale); diffisa in medio catenae
 * invenitur; causa nominat locum et rationem; tabula ordinaria ex
 * OneCRL fixo (2026-10-21) sex intermedia eMudhra TLS continet -
 * subiecta et clavium SHA-256 ex instantanea IPSA lecta, numquam
 * transcripta. Certificata probationis in tempore generata
 * (probationes/fixa/tls/generare.sh). Sine rete. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "filum.h"
#include "processus.h"
#include "json.h"
#include "base64.h"
#include "sigillum.h"
#include "fiducia.h"
#include "fiducia_der.h"
#include "fiducia_diffisa.h"
#include "fiducia_diffisa_tabula.h"
#include <stdio.h>
#include <string.h>

#define MUTATIONES_FIXAE \
    "probationes/fixa/fiducia/onecrl/2026-10-21/changeset.json"

nomen structura {
    character fixa[DXII];
} Locus;

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
                  Locus* l = (Locus*)piscina_allocare(c->piscina,
                      magnitudo(Locus));
     constans character* area = credo_area(c);
     constans character* argumenta[III];
      ProcessusResultus  pr;

    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_NECESSE_NON_NIHIL(area);
    sprintf(l->fixa, "%s/certa", area);
    argumenta[ZEPHYRUM] = "probationes/fixa/tls/generare.sh";
    argumenta[I] = l->fixa;
    argumenta[II] = NIHIL;
    pr = processus_exsequi(argumenta, LX * M, c->piscina);
    CREDO_NECESSE(pr.successus && pr.codex_exitus == ZEPHYRUM);
    c->datum = l;
}

interior FiduciaCertificatum
_certum (
         CredoContextus* c,
    constans character* titulus)
{
         constans Locus* l = (constans Locus*)c->suita;
    FiduciaCertificatum  fc;
              character  via[MXXIV];
                 chorda  der;

    sprintf(via, "%s/%s.der", l->fixa, titulus);
    der = filum_legere_totum(via, c->piscina);
    CREDO_NECESSE(der.mensura > ZEPHYRUM);
    fc.octeti   = der.datum;
    fc.mensura  = der.mensura;
    redde fc;
}

interior FiduciaPartes
_partes (
    FiduciaCertificatum fc)
{
          FiduciaPartes  p;
     constans character* causa = NIHIL;

    (vacuum)fiducia_partes_legere(fc.octeti, fc.mensura, &p, &causa);
    redde p;
}

/* diffisa subiectum+clavis ex certificato dato */
interior FiduciaDiffisa
_subiectum_clavis (
    FiduciaCertificatum  fc,
                Piscina* p)
{
    FiduciaPartes partes = _partes(fc);
         Sigillum s = sigillum_computare(partes.clavis_publica.octeti,
                         (memoriae_index)partes.clavis_publica.mensura);
               i8* h = (i8*)piscina_allocare(p,
                         (memoriae_index)SIGILLUM_OCTETI);
    FiduciaDiffisa d;

    memcpy(h, s.octeti, (size_t)SIGILLUM_OCTETI);
    d.genus             = FIDUCIA_DIFFISA_SUBIECTUM_CLAVIS;
    d.primum            = partes.subiectum.octeti;
    d.primum_mensura    = partes.subiectum.mensura;
    d.secundum          = h;
    d.secundum_mensura  = SIGILLUM_OCTETI;
    d.causa             = "probatio subiectum+clavis";
    redde d;
}

/* diffisa emittens+series; sine_zephyro: octetus 0x00 initialis
 * omittitur (forma FALSA - nihil reicere debet) */
interior FiduciaDiffisa
_emittens_series (
    FiduciaCertificatum fc,
                    b32 sine_zephyro)
{
     FiduciaPartes partes = _partes(fc);
    FiduciaDiffisa d;

    d.genus             = FIDUCIA_DIFFISA_EMITTENS_SERIES;
    d.primum            = partes.emittens.octeti;
    d.primum_mensura    = partes.emittens.mensura;
    d.secundum          = partes.series.octeti;
    d.secundum_mensura  = partes.series.mensura;
    si (   sine_zephyro && partes.series.mensura > I
        && partes.series.octeti[ZEPHYRUM] == 0x00)
    {
        d.secundum          = partes.series.octeti + I;
        d.secundum_mensura  = partes.series.mensura - I;
    }
    d.causa = "probatio emittens+series";
    redde d;
}

interior vacuum
_sectio_subiectum_clavis (
    CredoContextus* c)
{
    FiduciaCertificatum catena[I];
         FiduciaDiffisa tabula[I];
                    i32 positio = XCIX;

    tabula[ZEPHYRUM] = _subiectum_clavis(_certum(c, "localhost"),
        c->piscina);
    catena[ZEPHYRUM] = _certum(c, "localhost");
    CREDO_AEQUALIS_S32(fiducia_diffisam_invenire(catena, I, tabula, I,
        &positio), ZEPHYRUM);
    CREDO_AEQUALIS_I32(positio, ZEPHYRUM);
    /* geminus: subiectum et clavis eadem, series alia (instar
     * transsignati) - eadem diffisa eum quoque reicit */
    catena[ZEPHYRUM]  = _certum(c, "geminus");
    positio           = XCIX;
    CREDO_AEQUALIS_S32(fiducia_diffisam_invenire(catena, I, tabula, I,
        &positio), ZEPHYRUM);
    CREDO_AEQUALIS_I32(positio, ZEPHYRUM);
    /* clavis eadem sed subiectum aliud (localhost_rsa: clavis alia; ergo
     * alienus: subiectum et clavis alia) - non tangitur */
    catena[ZEPHYRUM] = _certum(c, "alienus");
    CREDO_AEQUALIS_S32(fiducia_diffisam_invenire(catena, I, tabula, I,
        &positio), -I);
}

interior vacuum
_sectio_emittens_series (
    CredoContextus* c)
{
    FiduciaCertificatum catena[I];
         FiduciaDiffisa tabula[I];
                    i32 positio;

    tabula[ZEPHYRUM] = _emittens_series(_certum(c, "serie_alta"),
        FALSUM);
    CREDO_AEQUALIS_I32(tabula[ZEPHYRUM].secundum_mensura, III);
    catena[ZEPHYRUM] = _certum(c, "serie_alta");
    CREDO_AEQUALIS_S32(fiducia_diffisam_invenire(catena, I, tabula, I,
        &positio), ZEPHYRUM);
    /* emittens idem, series alia: non tangitur */
    catena[ZEPHYRUM] = _certum(c, "localhost");
    CREDO_AEQUALIS_S32(fiducia_diffisam_invenire(catena, I, tabula, I,
        &positio), -I);
    /* series sine 0x00 initiali: octeti non exacti - nihil */
    tabula[ZEPHYRUM] = _emittens_series(_certum(c, "serie_alta"),
        VERUM);
    CREDO_AEQUALIS_I32(tabula[ZEPHYRUM].secundum_mensura, II);
    catena[ZEPHYRUM] = _certum(c, "serie_alta");
    CREDO_AEQUALIS_S32(fiducia_diffisam_invenire(catena, I, tabula, I,
        &positio), -I);
}

/* diffisa non folium: in loco I catenae */
interior vacuum
_sectio_diffisa_media (
    CredoContextus* c)
{
    FiduciaCertificatum catena[II];
         FiduciaDiffisa tabula[II];
                    i32 positio = XCIX;

    tabula[ZEPHYRUM] = _emittens_series(_certum(c, "serie_longa"),
        FALSUM);
    tabula[I] = _subiectum_clavis(_certum(c, "localhost"), c->piscina);
    catena[ZEPHYRUM] = _certum(c, "alienus");
    catena[I] = _certum(c, "localhost");
    CREDO_AEQUALIS_S32(fiducia_diffisam_invenire(catena, II, tabula, II,
        &positio), I);
    CREDO_AEQUALIS_I32(positio, I);
}

interior b32
_continet (
                 chorda  c,
     constans character* fragmentum)
{
    i32 k = (i32)strlen(fragmentum);
    i32 i;

    per (i = ZEPHYRUM; i + k <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, fragmentum, (size_t)k) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_sectio_causa (
    CredoContextus* c)
{
    FiduciaCertificatum catena[II];
         FiduciaDiffisa tabula[I];
                 chorda causa;

    catena[ZEPHYRUM]  = _certum(c, "alienus");
    catena[I]         = _certum(c, "localhost");
    causa.datum       = NIHIL;
    causa.mensura     = ZEPHYRUM;
    /* tabula ordinaria: certificata probationis munda */
    CREDO_VERUM(fiducia_diffisas_iudicare(catena, II, NIHIL, ZEPHYRUM,
        &causa, c->piscina));
    tabula[ZEPHYRUM] = _subiectum_clavis(catena[I], c->piscina);
    CREDO_FALSUM(fiducia_diffisas_iudicare(catena, II, tabula, I,
        &causa, c->piscina));
    CREDO_VERUM(_continet(causa, "certificatum reiectum"));
    CREDO_VERUM(_continet(causa, "clavis diffisa"));
    CREDO_VERUM(_continet(causa, "catena [1]"));
    CREDO_VERUM(_continet(causa, "probatio subiectum+clavis"));
}

/* tabula ordinaria: magnitudo, tempus, sex intermedia eMudhra TLS ex
 * instantanea ipsa (subiectum + pubKeyHash) in tabula */
interior vacuum
_sectio_tabula_vera (
    CredoContextus* c)
{
          chorda textus = filum_legere_totum(MUTATIONES_FIXAE,
              c->piscina);
    JsonResultus  r;
       JsonValor* mutationes;
             i32  k;
             i32  inventa   = ZEPHYRUM;
             i32  quaesita  = ZEPHYRUM;

    CREDO_VERUM(fiducia_diffisae_numerus >= MCC);
    CREDO_VERUM(fiducia_diffisae_numerus <= MCCLXII);
    CREDO_VERUM(strcmp(fiducia_diffisae_tempus, "2026-10-21")
        == ZEPHYRUM);
    CREDO_NECESSE(textus.mensura > ZEPHYRUM);
    r = json_legere(textus, c->piscina);
    CREDO_NECESSE(r.successus && r.radix != NIHIL);
    mutationes = json_objectum_capere(r.radix, "changes");
    CREDO_NECESSE(json_est_tabulatum(mutationes));
    per (k = ZEPHYRUM; k < json_tabulatum_numerus(mutationes); k++)
    {
            JsonValor* m = json_tabulatum_obtinere(mutationes, k);
               chorda  subiectum = json_ad_chorda(
                   json_objectum_capere(m, "subject"));
               chorda clavis = json_ad_chorda(
                   json_objectum_capere(m, "pubKeyHash"));
        Base64Fructus s;
        Base64Fructus h;
                  i32 j;

        si (subiectum.mensura == ZEPHYRUM || clavis.mensura == ZEPHYRUM)
        {
            perge;
        }
        s = base64_decodificare(subiectum, c->piscina);
        h = base64_decodificare(clavis, c->piscina);
        CREDO_NECESSE(s.datum != NIHIL && h.datum != NIHIL);
        si (!_continet(chorda_ex_buffer(s.datum, s.mensura), "eMudhra")
            || !_continet(chorda_ex_buffer(s.datum, s.mensura),
                "TLS CA - G3"))
        {
            perge;
        }
        quaesita++;
        per (j = ZEPHYRUM; j < fiducia_diffisae_numerus; j++)
        {
            constans FiduciaDiffisa* d = &fiducia_diffisae[j];

            si (   d->genus == FIDUCIA_DIFFISA_SUBIECTUM_CLAVIS
                && d->primum_mensura == s.mensura
                && d->secundum_mensura == h.mensura
                && memcmp(d->primum, s.datum, (size_t)s.mensura)
                    == ZEPHYRUM
                && memcmp(d->secundum, h.datum, (size_t)h.mensura)
                    == ZEPHYRUM)
            {
                CREDO_VERUM(strstr(d->causa, "2083382") != NIHIL);
                inventa++;
                frange;
            }
        }
    }
    CREDO_AEQUALIS_I32(quaesita, VI);
    CREDO_AEQUALIS_I32(inventa, VI);
}

/* certificata VERA (crt.sh, NOTICE): EM DV TLS CA - G3B sub emSign et
 * transsignatum sub AAA Certificate Services (Sectigo) - utrumque tabula
 * ordinaria reicit, transsignatum per subiectum+clavis */
interior FiduciaCertificatum
_verum (
         CredoContextus* c,
    constans character* titulus)
{
    FiduciaCertificatum fc;
              character via[CCLVI];
                 chorda der;

    sprintf(via, "probationes/fixa/fiducia/emudhra/%s.der", titulus);
    der = filum_legere_totum(via, c->piscina);
    CREDO_NECESSE(der.mensura > ZEPHYRUM);
    fc.octeti   = der.datum;
    fc.mensura  = der.mensura;
    redde fc;
}

interior vacuum
_sectio_intermedia_vera (
    CredoContextus* c)
{
    FiduciaCertificatum catena[I];
                 chorda causa;
                    i32 positio = XCIX;
                    s32 index;

    catena[ZEPHYRUM] = _verum(c, "em_dv_g3b_aaa");
    index = fiducia_diffisam_invenire(catena, I, fiducia_diffisae,
        fiducia_diffisae_numerus, &positio);
    CREDO_NECESSE(index >= ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)fiducia_diffisae[index].genus,
        (i32)FIDUCIA_DIFFISA_SUBIECTUM_CLAVIS);
    CREDO_AEQUALIS_I32(positio, ZEPHYRUM);
    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    CREDO_FALSUM(fiducia_diffisas_iudicare(catena, I, NIHIL, ZEPHYRUM,
        &causa, c->piscina));
    CREDO_VERUM(_continet(causa, "2083382"));
    catena[ZEPHYRUM] = _verum(c, "em_dv_g3b_emsign");
    CREDO_FALSUM(fiducia_diffisas_iudicare(catena, I, NIHIL, ZEPHYRUM,
        &causa, c->piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "subiectum_clavis", _sectio_subiectum_clavis, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "emittens_series", _sectio_emittens_series, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "diffisa_media", _sectio_diffisa_media, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "causa", _sectio_causa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tabula_vera", _sectio_tabula_vera, NIHIL, NIHIL, NIHIL, NIHIL },
    { "emudhra_vera", _sectio_intermedia_vera, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("fiducia_diffisa", SECTIONES,
        _suitam_parare, NIHIL);
}
