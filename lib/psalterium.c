/* psalterium.c - psalmi I-CL in latere vici (vide include/psalterium.h)
 *
 * Auxilia pura (incipit, circulus signorum, argumentum), montatio,
 * visus indicis (album CL ordinum, linea ima, cifrae typatae), visus
 * psalmi (aptatio in piscina propria per mutationem, gabc_visus sine
 * continuare, caput). Arbor: radix "psalterium" (PARTES_TABULA, figura
 * tota); index: ordines "r<n>" et sagittae lineae imae; psalmus: nodi
 * capitis "caput_*" et nodi visus (gabc_visus_componere). Figura solum
 * legit et mandata emittit.
 */
#include "psalterium.h"
#include "psalterium_infixum.h"
#include "psalmodia.h"
#include "toni_psalmorum_infixum.h"
#include "membrum__praecepta.h"
#include "membrum__album.h"
#include "semita.h"
#include "motus.h"
#include "thema.h"
#include "fons.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* dispositio (pixela; ut gabc_visus: limbus, linea ima, sagitta) */
#define PSALTERIUM_LATITUDO  CDLXXX    /* superficies ordinaria */
#define PSALTERIUM_ALTITUDO  CCCXX
#define LIMBUS_X             VI
#define LIMBUS_Y             VIII
#define LINEA_PEDIS            XII
#define SAGITTA              XXIV
#define ORDO                 XIV       /* altitudo ordinis indicis */
#define LITTERA              VI        /* latitudo FONS_PARVUS */
#define CIFRAE_MAXIMAE       III
#define MARGO_CAPITIS        XVI       /* caput visus psalmi */
#define LATITUDO_GLOBULI     XVIII     /* "≡" "«" "»" in capite */

#define VISUS_INDICIS        I
#define VISUS_PSALMI         II

#define CREDITUM             "Divinum Officium \xC2\xB7 MIT"
#define SIGNUM_INDICIS       "\xE2\x89\xA1"
#define SIGNUM_PRIORIS       "\xC2\xAB"  /* fons parvus U+2039 caret */
#define SIGNUM_PROXIMI       "\xC2\xBB"
#define INTERPUNCTUM         " \xC2\xB7 "

hic_manens constans character actio_psalterii[] = "psalterium.agere";

nomen structura {
    constans Organum* organum;
     PsalmodiaTabula* tabula;      /* infixa, in piscina montationis */
              chorda  incipit[CL]; /* psalmi n: incipit[n - 1] */
             Piscina* scheda;      /* per actionem vacatur */
              chorda  scriptor;    /* actio semel copiata (memoria) */
           GabcVisus* visus;
                 s32  ordinarium;  /* index "8 G" in circulo */
    /* aptatio currens (eadem non iteratur: restitutio sonum non
     * sistit); -1 = nulla */
                 s32 psalmus_aptatus;
                 s32 signum_aptatum;
} Psalterium;


/* ==================================================
 * Auxilia pura: incipit
 * ================================================== */

interior b32
_spatium_est (
    i8 c)
{
    redde c == ' ' || c == '\t' || c == '\r';
}

/* spatia utrimque dempta */
interior chorda
_praecidere_spatia (
    chorda s)
{
    dum (s.mensura > ZEPHYRUM && _spatium_est(s.datum[ZEPHYRUM]))
    {
        s.datum++;
        s.mensura--;
    }
    dum (s.mensura > ZEPHYRUM && _spatium_est(s.datum[s.mensura - I]))
    {
        s.mensura--;
    }
    redde s;
}

chorda
psalterium_incipit (
      chorda  textus,
     Piscina* piscina)
{
    i32 initium = ZEPHYRUM;
    i32 finis;
    i32 k;

    dum (initium < textus.mensura)
    {
        chorda linea;

        finis = initium;
        dum (finis < textus.mensura && textus.datum[finis] != '\n')
        {
            finis++;
        }
        linea = _praecidere_spatia(chorda_sectio(textus, initium,
            finis));
        initium = finis + I;
        si (linea.mensura == ZEPHYRUM || linea.datum[ZEPHYRUM] == '%')
        {
            perge;
        }
        /* "1. " initio omissum */
        k = ZEPHYRUM;
        dum (   k < linea.mensura && linea.datum[k] >= '0'
             && linea.datum[k] <= '9')
        {
            k++;
        }
        si (   k > ZEPHYRUM && k + I < linea.mensura
            && linea.datum[k] == '.' && linea.datum[k + I] == ' ')
        {
            linea.datum    += k + II;
            linea.mensura  -= k + II;
        }
        /* usque ad '*' aut '\xE2\x80\xA0' (crux) primum */
        per (k = ZEPHYRUM; k < linea.mensura; k++)
        {
            si (   linea.datum[k] == '*'
                || (   k + II < linea.mensura
                    && (insignatus character)linea.datum[k] == 0xE2
                    && (insignatus character)linea.datum[k + I] == 0x80
                    && (insignatus character)linea.datum[k + II]
                       == 0xA0))
            {
                frange;
            }
        }
        linea.mensura = k;
        dum (   linea.mensura > ZEPHYRUM
             && (   _spatium_est(linea.datum[linea.mensura - I])
                 || strchr(",:;.", (integer)linea.datum[linea.mensura
                        - I]) != NIHIL))
        {
            linea.mensura--;
        }
        redde chorda_transcribere(linea, piscina);
    }
    textus.datum    = NIHIL;
    textus.mensura  = ZEPHYRUM;
    redde textus;
}


/* ==================================================
 * Auxilia pura: circulus signorum (D8)
 * ================================================== */

/* terminatio index (ab 0) circuli; FALSUM extra fines */
interior b32
_circuli_terminatio (
           constans PsalmodiaTabula*  tabula,
                                i32   index,
            constans PsalmodiaTonus** tonus,
         constans PsalmodiaClausula** terminatio)
{
    i32 i;
    i32 j;
    i32 numerus = ZEPHYRUM;

    per (i = ZEPHYRUM; tabula != NIHIL && i < tabula->numerus_tonorum;
        i++)
    {
        constans PsalmodiaTonus* t = &tabula->toni[i];

        per (j = ZEPHYRUM; j < t->numerus_terminationum; j++)
        {
            constans PsalmodiaClausula* term = &t->terminationes[j];

            si (term->ad_libitum || term->monosyllaba)
            {
                perge;
            }
            si (numerus == index)
            {
                *tonus       = t;
                *terminatio  = term;
                redde VERUM;
            }
            numerus++;
        }
    }
    redde FALSUM;
}

i32
psalterium_signa_numerus (
    constans PsalmodiaTabula* tabula)
{
    i32 i;
    i32 j;
    i32 numerus = ZEPHYRUM;

    per (i = ZEPHYRUM; tabula != NIHIL && i < tabula->numerus_tonorum;
        i++)
    {
        per (j = ZEPHYRUM; j
            < tabula->toni[i].numerus_terminationum; j++)
        {
            constans PsalmodiaClausula* term =
                &tabula->toni[i].terminationes[j];

            si (!term->ad_libitum && !term->monosyllaba)
            {
                numerus++;
            }
        }
    }
    redde numerus;
}

chorda
psalterium_signum (
    constans PsalmodiaTabula* tabula,
                         i32  index,
                     Piscina* piscina)
{
        constans PsalmodiaTonus* tonus;
     constans PsalmodiaClausula* term;
              ChordaAedificator* a;
                         chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (!_circuli_terminatio(tabula, index, &tonus, &term))
    {
        redde vacua;
    }
    a = chorda_aedificator_creare(piscina, XVI);
    si (a == NIHIL)
    {
        redde vacua;
    }
    chorda_aedificator_appendere_chorda(a, tonus->modus);
    si (term->differentia.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_chorda(a, term->differentia);
    }
    redde chorda_aedificator_finire(a);
}

/* index terminationis in circulo; -1 si non in eo */
interior s32
_index_terminationis (
       constans PsalmodiaTabula* tabula,
     constans PsalmodiaClausula* quaesita)
{
       constans PsalmodiaTonus* tonus;
    constans PsalmodiaClausula* term;
                           i32  k;

    per (k = ZEPHYRUM; quaesita != NIHIL
        && _circuli_terminatio(tabula, k, &tonus, &term); k++)
    {
        si (term == quaesita)
        {
            redde (s32)k;
        }
    }
    redde -I;
}

s32
psalterium_signi_index (
    constans PsalmodiaTabula* tabula,
                      chorda  signum)
{
    redde _index_terminationis(tabula, psalmodia_signum_invenire(tabula,
        signum, NIHIL, NIHIL));
}


/* ==================================================
 * Auxilia pura: argumentum (spec III.3)
 * ================================================== */

#define VITIUM_NUMERI "psalterium: psalm number 1-150"

b32
psalterium_argumentum_legere (
                chorda  argumentum,
               Piscina* scheda,
                   s32* psalmus,
                chorda* signum,
     ChordaAedificator* causa)
{
                        chorda  numerus;
                        chorda  tonus;
               PsalmodiaTabula* tabula;
             ChordaAedificator* ratio;
    constans PsalmodiaClausula* term;
                           s32  valor = ZEPHYRUM;
                           i32  k;

    *psalmus         = -I;
    signum->datum    = NIHIL;
    signum->mensura  = ZEPHYRUM;
    argumentum       = _praecidere_spatia(argumentum);
    si (argumentum.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    numerus        = argumentum;
    tonus.datum    = NIHIL;
    tonus.mensura  = ZEPHYRUM;
    per (k = ZEPHYRUM; k < argumentum.mensura; k++)
    {
        si (argumentum.datum[k] == ',')
        {
            numerus  = chorda_sectio(argumentum, ZEPHYRUM, k);
            tonus    = chorda_sectio(argumentum, k + I,
                argumentum.mensura);
            frange;
        }
    }
    numerus  = _praecidere_spatia(numerus);
    tonus    = _praecidere_spatia(tonus);
    per (k = ZEPHYRUM; k < numerus.mensura; k++)
    {
        si (   numerus.datum[k] < '0' || numerus.datum[k] > '9'
            || k >= CIFRAE_MAXIMAE)
        {
            valor = ZEPHYRUM;
            frange;
        }
        valor = valor * X + (s32)(numerus.datum[k] - '0');
    }
    si (valor < I || valor > CL)
    {
        si (causa != NIHIL)
        {
            chorda_aedificator_appendere_literis(causa, VITIUM_NUMERI);
        }
        redde FALSUM;
    }
    si (tonus.mensura > ZEPHYRUM)
    {
        tabula = psalmodia_tabulam_legere(toni_psalmorum_infixum_textus(
            scheda), scheda);
        ratio  = chorda_aedificator_creare(scheda, LXIV);
        term   = psalmodia_signum_invenire(tabula, tonus, NIHIL, ratio);
        si (   term == NIHIL
            || _index_terminationis(tabula, term) < ZEPHYRUM)
        {
            si (causa != NIHIL)
            {
                chorda_aedificator_appendere_literis(causa,
                    "psalterium: ");
                chorda_aedificator_appendere_chorda(causa,
                    chorda_aedificator_finire(ratio));
            }
            redde FALSUM;
        }
        *signum = psalterium_signum(tabula, (i32)_index_terminationis(
            tabula, term), scheda);
    }
    *psalmus = valor;
    redde VERUM;
}


/* ==================================================
 * Status (ramus)
 * ================================================== */

interior chorda
_campus (
    constans PsalteriumMontatio* m,
             constans character* titulus)
{
     chorda* a = insula_ramus_attributum(&m->ramus, titulus);
     chorda  vacua;

    si (a)
    {
        redde *a;
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

/* campus numerus intra [minimum, maximum]; -1 si abest aut extra */
interior s32
_campus_numerus (
    constans PsalteriumMontatio* m,
             constans character* titulus,
                            s32  minimum,
                            s32  maximum)
{
    chorda a = _campus(m, titulus);
       s32 n;

    redde (a.mensura > ZEPHYRUM && chorda_ut_s32(a, &n) && n >= minimum
        && n <= maximum) ? n : -I;
}

nomen structura {
    constans character* constans* tituli;
    constans character* constans* valores;
                             i32 numerus;
} GrexCamporum;

interior vacuum
_campos_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    GrexCamporum* grex = (GrexCamporum*)ctx;
             i32  i;

    per (i = ZEPHYRUM; i < grex->numerus; i++)
    {
        si (!grex->valores[i] || !grex->valores[i][ZEPHYRUM])
        {
            (vacuum)insula_attributum_tollere(radix, grex->tituli[i]);
        }
        alioquin
        {
            insula_attributum_ponere(radix, p, in, grex->tituli[i],
                grex->valores[i]);
        }
    }
}

/* campi actu uno, scriptore psalterii ("" aut NIHIL tollit) */
interior b32
_campos_scribere (
            PsalteriumMontatio* m,
    constans character* constans* tituli,
    constans character* constans* valores,
                             i32  numerus)
{
    GrexCamporum grex;
          chorda prior;
             b32 ok;

    grex.tituli   = tituli;
    grex.valores  = valores;
    grex.numerus  = numerus;
    prior         = m->ramus.repo->scriptor;
    insula_scriptorem_ponere(m->ramus.repo,
        ((Psalterium*)m->figurae_ctx)->scriptor);
    ok = mutare_ramum(&m->ramus, _campos_ponere, &grex);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

/* visus: psalmus solum si campus psalmi intra I-CL (status malus =
 * index) */
interior i32
_visus (
    constans PsalteriumMontatio* m)
{
    si (   chorda_aequalis_literis(_campus(m, "visus"), "psalmus")
        && _campus_numerus(m, "psalmus", I, CL) > ZEPHYRUM)
    {
        redde VISUS_PSALMI;
    }
    redde VISUS_INDICIS;
}

/* electus indicis: campus, aliter psalmus, aliter I */
interior s32
_electus (
    constans PsalteriumMontatio* m)
{
    s32 e = _campus_numerus(m, "electus", I, CL);

    si (e < ZEPHYRUM)
    {
        e = _campus_numerus(m, "psalmus", I, CL);
    }
    redde e < ZEPHYRUM ? I : e;
}


/* ==================================================
 * Dispositio indicis
 * ================================================== */

interior s32
_superficies (
    constans PsalteriumMontatio* m,
                            b32  latitudo)
{
    s32 valor = latitudo ? (s32)m->latitudo : (s32)m->altitudo;
    s32 n     = _campus_numerus(m, latitudo ? "superficies_latitudo"
                 : "superficies_altitudo", I, MMMCMXCIX);

    si (valor <= ZEPHYRUM)
    {
        valor = latitudo ? PSALTERIUM_LATITUDO : PSALTERIUM_ALTITUDO;
    }
    redde n > ZEPHYRUM ? n : valor;
}

/* ordines in pagina: inter limbum et lineam pedis; saltem I */
interior s32
_capacitas (
    constans PsalteriumMontatio* m)
{
    s32 c = (_superficies(m, FALSUM) - II * LIMBUS_Y - LINEA_PEDIS - II)
          / ORDO;

    redde c < I ? I : c;
}

/* album: res = numerus psalmi, id "r<n>", altitudo I ordo */
interior Album*
_album (
    constans PsalteriumMontatio* m,
                        Piscina* p)
{
        Album* l = album_creare(p, _capacitas(m));
          s32  n;
    character  id[XVI];

    per (n = I; l && n <= CL; n++)
    {
        sprintf(id, "r%d", (integer)n);
        (vacuum)album_addere(l, id, ALBUM_RES, I, n);
    }
    redde l;
}

interior s32
_pagina (
    Album* l,
      s32  electus)
{
    s32 k = album_pagina_rei(l, electus, ZEPHYRUM);

    redde k < ZEPHYRUM ? ZEPHYRUM : k;
}

/* id "r<n>" -> n; 0 si non ordo */
interior s32
_id_ordinis (
    chorda id)
{
    s32 n;

    si (id.mensura < II || id.datum[ZEPHYRUM] != 'r')
    {
        redde ZEPHYRUM;
    }
    id.datum++;
    id.mensura--;
    redde chorda_ut_s32(id, &n) && n >= I && n <= CL ? n : ZEPHYRUM;
}


/* ==================================================
 * Actiones indicis
 * ================================================== */

interior Psalterium*
_privatum (
    constans PsalteriumMontatio* m)
{
    redde (Psalterium*)m->figurae_ctx;
}

/* electus n (extra I-CL: solum cifrae tolluntur) */
interior b32
_electum_scribere (
    PsalteriumMontatio* m,
                   s32  n)
{
    constans character* tituli[II];
    constans character* valores[II];
             character  numerus[XVI];

    tituli[ZEPHYRUM]   = "cifrae";
    valores[ZEPHYRUM]  = "";
    si (n < I || n > CL)
    {
        redde _campus(m, "cifrae").mensura == ZEPHYRUM
            || _campos_scribere(m, tituli, valores, I);
    }
    sprintf(numerus, "%d", (integer)n);
    tituli[I]   = "electus";
    valores[I]  = numerus;
    redde _campos_scribere(m, tituli, valores, II);
}

interior b32
fac_proximum (
    vacuum* ctx)
{
    PsalteriumMontatio* m = (PsalteriumMontatio*)ctx;

    redde _electum_scribere(m, _electus(m) + I);
}

interior b32
fac_priorem (
    vacuum* ctx)
{
    PsalteriumMontatio* m = (PsalteriumMontatio*)ctx;

    redde _electum_scribere(m, _electus(m) - I);
}

interior b32
_paginam_vertere (
    PsalteriumMontatio* m,
                   s32  gradus)
{
    Album* l = _album(m, _privatum(m)->scheda);
      s32  k;

    si (!l)
    {
        redde FALSUM;
    }
    k = _pagina(l, _electus(m)) + gradus;
    si (k < ZEPHYRUM || k >= album_paginae(l))
    {
        redde _electum_scribere(m, ZEPHYRUM);
    }
    redde _electum_scribere(m, album_res_prima(l, k));
}

interior b32
fac_paginam_priorem (
    vacuum* ctx)
{
    redde _paginam_vertere((PsalteriumMontatio*)ctx, -I);
}

interior b32
fac_paginam_proximam (
    vacuum* ctx)
{
    redde _paginam_vertere((PsalteriumMontatio*)ctx, I);
}

/* psalmum n aperire: visus et psalmus (semita), electus idem */
interior b32
_aperire (
    PsalteriumMontatio* m,
                   s32  n)
{
    constans character* tituli[V];
    constans character* valores[V];
             character  numerus[XVI];

    si (n < I || n > CL)
    {
        redde VERUM;
    }
    sprintf(numerus, "%d", (integer)n);
    tituli[IV]         = "pagina";
    valores[IV]        = "";
    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "psalmus";
    tituli[I]          = "psalmus";
    valores[I]         = numerus;
    tituli[II]         = "electus";
    valores[II]        = numerus;
    tituli[III]        = "cifrae";
    valores[III]       = "";
    redde _campos_scribere(m, tituli, valores, V)
        && psalterium_semitam_secuta(m);
}

interior b32
fac_aperire (
    vacuum* ctx)
{
    PsalteriumMontatio* m = (PsalteriumMontatio*)ctx;

    redde _aperire(m, _electus(m));
}

/* cifra typata (Review Focus III): numerus crescens si intra I-CL,
 * aliter a cifra nova incipit; cifra nova sola extra (0) cifras
 * tollit. Electus numerum sequitur. */
interior b32
_cifrae_addere (
    PsalteriumMontatio* m,
             character  cifra)
{
    constans character* tituli[II];
    constans character* valores[II];
             character  cifrae[CIFRAE_MAXIMAE + II];
             character  numerus[XVI];
                chorda  priores = _campus(m, "cifrae");
                   s32  valor;

    valor = ZEPHYRUM;
    si (priores.mensura > ZEPHYRUM && priores.mensura < CIFRAE_MAXIMAE)
    {
        memcpy(cifrae, priores.datum, (size_t)priores.mensura);
        cifrae[priores.mensura]      = cifra;
        cifrae[priores.mensura + I]  = '\0';
        valor                        = (s32)atoi(cifrae);
    }
    si (valor < I || valor > CL)
    {
        cifrae[ZEPHYRUM]  = cifra;
        cifrae[I]         = '\0';
        valor             = (s32)(cifra - '0');
    }
    tituli[ZEPHYRUM] = "cifrae";
    si (valor < I)
    {
        valores[ZEPHYRUM] = "";
        redde _campos_scribere(m, tituli, valores, I);
    }
    sprintf(numerus, "%d", (integer)valor);
    valores[ZEPHYRUM]  = cifrae;
    tituli[I]          = "electus";
    valores[I]         = numerus;
    redde _campos_scribere(m, tituli, valores, II);
}

/* Backspace: cifra ultima tollitur, electus cifras reliquas sequitur */
interior b32
_cifrae_tollere (
    PsalteriumMontatio* m)
{
    constans character* tituli[II];
    constans character* valores[II];
             character  cifrae[CIFRAE_MAXIMAE + I];
                chorda  priores = _campus(m, "cifrae");

    si (priores.mensura == ZEPHYRUM || priores.mensura > CIFRAE_MAXIMAE)
    {
        redde priores.mensura == ZEPHYRUM || _electum_scribere(m,
            ZEPHYRUM);
    }
    memcpy(cifrae, priores.datum, (size_t)(priores.mensura - I));
    cifrae[priores.mensura - I]  = '\0';
    tituli[ZEPHYRUM]             = "cifrae";
    valores[ZEPHYRUM]            = cifrae;
    si (cifrae[ZEPHYRUM] == '\0' || atoi(cifrae) < I)
    {
        valores[ZEPHYRUM] = "";
        redde _campos_scribere(m, tituli, valores, I);
    }
    tituli[I]   = "electus";
    valores[I]  = cifrae;
    redde _campos_scribere(m, tituli, valores, II);
}


/* ==================================================
 * Visus psalmi: aptatio
 * ================================================== */

/* signum lateris in circulo; absens aut ignotum = "8 G" (D3, tacite) */
interior s32
_signum_currens (
    constans PsalteriumMontatio* m)
{
     Psalterium* p = _privatum(m);
         chorda  a = _campus(m, "signum");
            s32  k = a.mensura > ZEPHYRUM ? psalterium_signi_index(
                       p->tabula, a) : -I;

    redde k < ZEPHYRUM ? p->ordinarium : k;
}

/* psalmum campi in signo campi aptat (si non iam aptatus): piscina
 * propria, gabc visui SINE continuare (sonus sistit), deinde
 * destruitur (visus textum copiat); admonitio prima = vexillum.
 * Pagina campi ad paginas novas coercetur (tonus paginam servat;
 * psalmus novus paginam suam ante tollit). */
interior b32
_aptare (
    PsalteriumMontatio* m)
{
                    Psalterium* p = _privatum(m);
                           s32  n = _campus_numerus(m, "psalmus", I,
                               CL);
                              s32  k = _signum_currens(m);
          constans PsalmodiaTonus* tonus;
       constans PsalmodiaClausula* term;
                PsalmodiaOptiones  optiones;
                 PsalmodiaPsalmus* ps;
                ChordaAedificator* a;
                          Piscina* q;
                        character  titulus[XXIV];
                        character  vexillum[CCLVI];

    si (n < ZEPHYRUM || !p->visus)
    {
        redde VERUM;
    }
    si (   n == p->psalmus_aptatus
        && k == p->signum_aptatum
        && gabc_visus_habet_versionem(p->visus))
    {
        redde gabc_visus_paginam_movere(p->visus, ZEPHYRUM);
    }
    si (!_circuli_terminatio(p->tabula, (i32)k, &tonus, &term))
    {
        redde FALSUM;
    }
    q = piscina_generare_dynamicum("psalterium_aptatio",
        (memoriae_index)(CCLVI * MXXIV));
    si (q == NIHIL)
    {
        redde FALSUM;
    }
    sprintf(titulus, "Psalmus %d", (integer)n);
    memset(&optiones, ZEPHYRUM, magnitudo(optiones));
    optiones.titulus = chorda_ex_literis(titulus, q);
    ps = psalmodia_aptare(tonus, term,
        psalterium_infixum_psalmus((i32)n,
        q), &optiones, q);
    a  = chorda_aedificator_creare(q, MXXIV);
    si (!ps || !a || !psalmodia_gabc_scribere(ps, &optiones, a))
    {
        piscina_destruere(q);
        redde FALSUM;
    }
    (vacuum)gabc_visus_textum_ponere(p->visus,
        chorda_aedificator_finire(
        a), FALSUM);
    si (ps->numerus_admonitionum > ZEPHYRUM)
    {
        sprintf(vexillum, "%.20s: %.200s", titulus,
            chorda_ut_cstr(ps->admonitiones[ZEPHYRUM].causa, q));
        gabc_visus_vexillum_ponere(p->visus, vexillum);
    }
    piscina_destruere(q);
    p->psalmus_aptatus  = n;
    p->signum_aptatum   = k;
    redde gabc_visus_paginam_movere(p->visus, ZEPHYRUM);
}


/* ==================================================
 * Visus psalmi: actiones
 * ================================================== */

/* tonus +/- I in circulo (D8; in finibus redit) */
interior b32
_tonum_movere (
    PsalteriumMontatio* m,
                   s32  gradus)
{
             Psalterium* p = _privatum(m);
                    s32  numerus = (s32)psalterium_signa_numerus(
                                       p->tabula);
                    s32  k;
     constans character* tituli[I];
     constans character* valores[I];

    si (numerus <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    k = (_signum_currens(m) + gradus + numerus)
        % numerus;
    tituli[ZEPHYRUM] = "signum";
    valores[ZEPHYRUM]  = chorda_ut_cstr(psalterium_signum(p->tabula,
        (i32)k, p->scheda), p->scheda);
    redde _campos_scribere(m, tituli, valores, I) && _aptare(m);
}

/* psalmus +/- I; ultra CL aut ante I nihil */
interior b32
_psalmum_movere (
    PsalteriumMontatio* m,
                   s32  gradus)
{
    constans character* tituli[III];
    constans character* valores[III];
             character  numerus[XVI];
                   s32  n = _campus_numerus(m, "psalmus", I, CL)
                       + gradus;

    si (n < I || n > CL)
    {
        redde VERUM;
    }
    sprintf(numerus, "%d", (integer)n);
    tituli[ZEPHYRUM]   = "psalmus";
    valores[ZEPHYRUM]  = numerus;
    tituli[I]          = "electus";
    valores[I]         = numerus;
    tituli[II]         = "pagina";
    valores[II]        = "";
    redde _campos_scribere(m, tituli, valores, III) && _aptare(m);
}

interior b32
fac_tonum_proximum (
    vacuum* ctx)
{
    redde _tonum_movere((PsalteriumMontatio*)ctx, I);
}

interior b32
fac_tonum_priorem (
    vacuum* ctx)
{
    redde _tonum_movere((PsalteriumMontatio*)ctx, -I);
}

interior b32
fac_psalmum_proximum (
    vacuum* ctx)
{
    redde _psalmum_movere((PsalteriumMontatio*)ctx, I);
}

interior b32
fac_psalmum_priorem (
    vacuum* ctx)
{
    redde _psalmum_movere((PsalteriumMontatio*)ctx, -I);
}

/* Esc: ad indicem, psalmus electus; sonus sistit (Review Focus IV) */
interior b32
fac_ad_indicem (
    vacuum* ctx)
{
    PsalteriumMontatio* m = (PsalteriumMontatio*)ctx;
    constans character* tituli[III];
    constans character* valores[III];
             character  numerus[XVI];

    sprintf(numerus, "%d", (integer)_campus_numerus(m, "psalmus", I,
        CL));
    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "index";
    tituli[I]          = "psalmus";
    valores[I]         = "";
    tituli[II]         = "electus";
    valores[II]        = numerus;
    (vacuum)gabc_visus_sistere(_privatum(m)->visus);
    redde _campos_scribere(m, tituli, valores, III);
}

interior GabcVisus*
_visus_eius (
    vacuum* ctx)
{
    redde _privatum((PsalteriumMontatio*)ctx)->visus;
}

interior b32
fac_canere (
    vacuum* ctx)
{
    redde gabc_visus_canere(_visus_eius(ctx));
}

interior b32
fac_syllabam_priorem (
    vacuum* ctx)
{
    redde gabc_visus_syllabam_movere(_visus_eius(ctx), -I);
}

interior b32
fac_syllabam_proximam (
    vacuum* ctx)
{
    redde gabc_visus_syllabam_movere(_visus_eius(ctx), I);
}

interior b32
fac_paginam_cantus_proximam (
    vacuum* ctx)
{
    redde gabc_visus_paginam_movere(_visus_eius(ctx), I);
}

interior b32
fac_paginam_cantus_priorem (
    vacuum* ctx)
{
    redde gabc_visus_paginam_movere(_visus_eius(ctx), -I);
}

interior b32
fac_maius (
    vacuum* ctx)
{
    redde gabc_visus_spatium_movere(_visus_eius(ctx), II);
}

interior b32
fac_minus (
    vacuum* ctx)
{
    redde gabc_visus_spatium_movere(_visus_eius(ctx), -II);
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "psalterium.proximus", "Next psalm", "Down", VISUS_INDICIS,
        ZEPHYRUM, fac_proximum },
    { "psalterium.prior", "Previous psalm", "Up", VISUS_INDICIS,
        ZEPHYRUM, fac_priorem },
    { "psalterium.deorsum", "Next psalm", "j", VISUS_INDICIS, ZEPHYRUM,
        fac_proximum },
    { "psalterium.sursum", "Previous psalm", "k", VISUS_INDICIS,
        ZEPHYRUM, fac_priorem },
    { "psalterium.pagina_proxima", "Next page", "PageDown",
        VISUS_INDICIS, ZEPHYRUM, fac_paginam_proximam },
    { "psalterium.pagina_prior", "Previous page", "PageUp",
        VISUS_INDICIS, ZEPHYRUM, fac_paginam_priorem },
    { "psalterium.aperire", "Open psalm", "Enter", VISUS_INDICIS,
        ZEPHYRUM, fac_aperire },
    { "psalterium.canere", "Play / stop", " ", VISUS_PSALMI, ZEPHYRUM,
        fac_canere },
    { "psalterium.syllaba_prior", "Previous syllable", "Left",
        VISUS_PSALMI, ZEPHYRUM, fac_syllabam_priorem },
    { "psalterium.syllaba_proxima", "Next syllable", "Right",
        VISUS_PSALMI, ZEPHYRUM, fac_syllabam_proximam },
    { "psalterium.cantus_proxima", "Next page", "PageDown",
        VISUS_PSALMI, ZEPHYRUM, fac_paginam_cantus_proximam },
    { "psalterium.cantus_prior", "Previous page", "PageUp",
        VISUS_PSALMI, ZEPHYRUM, fac_paginam_cantus_priorem },
    { "psalterium.cantus_deorsum", "Next page", "j", VISUS_PSALMI,
        ZEPHYRUM, fac_paginam_cantus_proximam },
    { "psalterium.cantus_sursum", "Previous page", "k", VISUS_PSALMI,
        ZEPHYRUM, fac_paginam_cantus_priorem },
    { "psalterium.maius", "Zoom in", "+", VISUS_PSALMI, ZEPHYRUM,
        fac_maius },
    { "psalterium.maius_aequale", "Zoom in", "=", VISUS_PSALMI,
        ZEPHYRUM, fac_maius },
    { "psalterium.minus", "Zoom out", "-", VISUS_PSALMI, ZEPHYRUM,
        fac_minus },
    { "psalterium.tonus_proximus", "Next tone", "t", VISUS_PSALMI,
        ZEPHYRUM, fac_tonum_proximum },
    { "psalterium.tonus_prior", "Previous tone", "T", VISUS_PSALMI,
        ZEPHYRUM, fac_tonum_priorem },
    { "psalterium.psalmus_proximus", "Next psalm", "]", VISUS_PSALMI,
        ZEPHYRUM, fac_psalmum_proximum },
    { "psalterium.psalmus_prior", "Previous psalm", "[", VISUS_PSALMI,
        ZEPHYRUM, fac_psalmum_priorem },
    { "psalterium.ad_indicem", "Psalm list", "Esc", VISUS_PSALMI,
        ZEPHYRUM, fac_ad_indicem },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

interior b32
requisita_nulla (
        i32  requisita,
     vacuum* ctx)
{
    (vacuum)requisita;
    (vacuum)ctx;
    redde VERUM;
}

/* cifra sine modificatoribus (praeter Shift et Caps Lock)? */
interior b32
_cifra_est (
    constans Eventus* ev)
{
    redde ev->datum.clavis.producta >= '0'
        && ev->datum.clavis.producta <= '9'
        && !(ev->datum.clavis.modificantes & (i32)(MOD_IMPERIUM
            | MOD_ALT
               | MOD_SUPER));
}

interior b32
agere (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
     PsalteriumMontatio* m = (PsalteriumMontatio*)ctx;
    constans Praeceptum* t;
                    i32  visus;
                    s32  n;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)des;
    si (!m || !m->figurae_ctx)
    {
        redde FALSUM;
    }
    piscina_vacare(_privatum(m)->scheda);
    visus = _visus(m);
    si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        si (visus == VISUS_INDICIS && _cifra_est(ev))
        {
            redde _cifrae_addere(m,
                (character)ev->datum.clavis.producta);
        }
        si (   visus                   == VISUS_INDICIS
            && ev->datum.clavis.clavis == CLAVIS_RETRORSUM)
        {
            redde _cifrae_tollere(m);
        }
        t = praecepta_ad_clavem(TABULA_PRAECEPTORUM, visus, ev);
        redde t && praecepta_status(t, visus, requisita_nulla, m)
            == PRAECEPTUM_PRAESTO && t->fn(m);
    }
    si (   !nodus || ev->genus != EVENTUS_MUS_DEPRESSUS
        || ev->datum.mus.botton != MUS_SINISTER)
    {
        redde FALSUM;
    }
    si (visus == VISUS_PSALMI)
    {
        si (chorda_aequalis_literis(nodus->id, "caput_index"))
        {
            redde fac_ad_indicem(m);
        }
        si (chorda_aequalis_literis(nodus->id, "caput_prior"))
        {
            redde _psalmum_movere(m, -I);
        }
        si (chorda_aequalis_literis(nodus->id, "caput_proximus"))
        {
            redde _psalmum_movere(m, I);
        }
        si (chorda_aequalis_literis(nodus->id, "caput_signum"))
        {
            redde _tonum_movere(m, I);
        }
        redde gabc_visus_premere(_privatum(m)->visus, nodus, ev);
    }
    n = _id_ordinis(nodus->id);
    si (n > ZEPHYRUM)
    {
        redde _aperire(m, n);
    }
    si (chorda_aequalis_literis(nodus->id, "sagitta_prior"))
    {
        redde _paginam_vertere(m, -I);
    }
    si (chorda_aequalis_literis(nodus->id, "sagitta_proxima"))
    {
        redde _paginam_vertere(m, I);
    }
    redde FALSUM;
}


/* ==================================================
 * Figura
 * ================================================== */

/* praecidere ad columnas (runae, non octeti) */
interior chorda
_praecidere (
    chorda textus,
       s32 columnae)
{
    i32 i;
    s32 runae = ZEPHYRUM;

    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si ((textus.datum[i] & 0xC0) != 0x80)
        {
            si (runae == columnae)
            {
                textus.mensura = i;
                redde textus;
            }
            runae++;
        }
    }
    redde textus;
}

interior s32
_runae (
    constans character* textus)
{
    s32 n = ZEPHYRUM;

    dum (*textus)
    {
        si ((*textus & 0xC0) != 0x80)
        {
            n++;
        }
        textus++;
    }
    redde n;
}

interior vacuum
_textum_pingere (
               Mandata* md,
                   s32  x,
                   s32  y,
                chorda  textus,
            ColorThema  color)
{
    mandata_textus(md, x, y, chorda_transcribere(textus, md->piscina),
        FONS_PARVUS, figura_color_thematis(color));
}

/* ordo indicis: "109  Dixit Dóminus Dómino meo"; electus fundo
 * selectionis */
interior vacuum
_ordinem_pingere (
    constans PsalteriumMontatio* m,
                        Mandata* md,
             constans Componens* c,
                            s32  n,
                            b32  electus)
{
       chorda textus;
          s32 columnae = (c->fines.latitudo - VIII) / LITTERA;
    character numerus[XVI];

    si (electus)
    {
        figura_ordinem_electum_pingere(md, c->fines);
    }
    sprintf(numerus, "%3d  ", (integer)n);
    textus = chorda_concatenare(chorda_ex_literis(numerus, md->piscina),
        _privatum(m)->incipit[n - I], md->piscina);
    _textum_pingere(md, c->fines.x + IV, c->fines.y + (ORDO - VIII)
        / II,
        _praecidere(textus, columnae), COLOR_TEXT);
}

/* linea ima indicis: "<  Psalterium  2/11  [10]   Divinum Officium ·
 * MIT  >" */
interior vacuum
_pedem_pingere (
     constans PsalteriumMontatio* m,
                         Mandata* md,
                           Fines  f,
                             s32  pagina,
                             s32  paginae)
{
       chorda cifrae  = _campus(m, "cifrae");
          s32 y       = f.altitudo - LIMBUS_Y - LINEA_PEDIS + II;
    character pes[LXIV];

    _textum_pingere(md, LIMBUS_X + II, y, chorda_ex_literis("<",
        md->piscina), pagina > ZEPHYRUM ? COLOR_TEXT : COLOR_TEXT_DIM);
    si (cifrae.mensura > ZEPHYRUM && cifrae.mensura <= CIFRAE_MAXIMAE)
    {
        sprintf(pes, "Psalterium  %d/%d  [%.*s]", (integer)(pagina + I),
            (integer)paginae, (integer)cifrae.mensura,
            (constans character*)cifrae.datum);
    }
    alioquin
    {
        sprintf(pes, "Psalterium  %d/%d", (integer)(pagina + I),
            (integer)paginae);
    }
    _textum_pingere(md, LIMBUS_X + SAGITTA, y, chorda_ex_literis(pes,
        md->piscina), COLOR_TEXT);
    _textum_pingere(md, f.latitudo - LIMBUS_X - SAGITTA - _runae(
        CREDITUM) * LITTERA, y, chorda_ex_literis(CREDITUM,
        md->piscina),
        COLOR_TEXT_DIM);
    _textum_pingere(md, f.latitudo - LIMBUS_X - VIII, y,
        chorda_ex_literis(">", md->piscina), pagina < paginae - I
        ? COLOR_TEXT : COLOR_TEXT_DIM);
}

interior vacuum
_indicem_pingere (
    constans PsalteriumMontatio* m,
             constans Componens* radix,
                        Mandata* md)
{
    Album* l = _album(m, md->piscina);
      s32  e = _electus(m);
      i32  i;

    si (!l)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(radix); i++)
    {
         Componens* f = componens_liberum(radix, i);
               s32  n = _id_ordinis(f->id);

        si (n > ZEPHYRUM)
        {
            _ordinem_pingere(m, md, f, n, n == e);
        }
    }
    _pedem_pingere(m, md, radix->fines, _pagina(l, e),
        album_paginae(l));
}

/* caput visus psalmi: "≡  «  »   8 G" (nodi caput_* eodem loco);
 * vexillum (admonitio) id tegit */
interior vacuum
_caput_pingere (
     constans PsalteriumMontatio* m,
              constans Componens* radix,
                         Mandata* md,
                          chorda  signum)
{
    s32 n = _campus_numerus(m, "psalmus", I, CL);
    s32 y = LIMBUS_Y + (MARGO_CAPITIS - VIII) / II;
    i32 i;

    si (gabc_visus_vexillum(_privatum(m)->visus)[ZEPHYRUM] != '\0')
    {
        redde;
    }
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(radix); i++)
    {
         Componens* f = componens_liberum(radix, i);
               s32  x = f->fines.x + (LATITUDO_GLOBULI - LITTERA) / II;

        si (chorda_aequalis_literis(f->id, "caput_index"))
        {
            _textum_pingere(md, x, y, chorda_ex_literis(SIGNUM_INDICIS,
                md->piscina), COLOR_TEXT);
        }
        alioquin si (chorda_aequalis_literis(f->id, "caput_prior"))
        {
            _textum_pingere(md, x, y, chorda_ex_literis(SIGNUM_PRIORIS,
                md->piscina), n > I ? COLOR_TEXT : COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "caput_proximus"))
        {
            _textum_pingere(md, x, y, chorda_ex_literis(SIGNUM_PROXIMI,
                md->piscina), n < CL ? COLOR_TEXT : COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "caput_signum"))
        {
            _textum_pingere(md, f->fines.x + LITTERA, y, signum,
                COLOR_ACCENT_PRIMARY);
        }
    }
}

/* visus psalmi: gabc_visus (fundus, pagina, linea ima "Psalmus 109 ·
 * 8 G", vexillum), deinde caput */
interior vacuum
_psalmum_pingere (
    constans PsalteriumMontatio* m,
             constans Componens* c,
                        Mandata* md)
{
     Psalterium* p = _privatum(m);
         chorda  signum = psalterium_signum(p->tabula,
             (i32)_signum_currens(
                    m), md->piscina);
         chorda titulus;

    titulus = chorda_concatenare(chorda_concatenare(gabc_visus_titulus(
        p->visus), chorda_ex_literis(INTERPUNCTUM, md->piscina),
        md->piscina), signum, md->piscina);
    gabc_visus_pingere(p->visus, md, c->fines, chorda_aequalis_literis(
        c->titulus, "focatum"), titulus, NIHIL);
    _caput_pingere(m, c, md, signum);
}

interior vacuum
figura_psalterii (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
    PsalteriumMontatio* m = (PsalteriumMontatio*)ctx;
                 Fines  f;
                 Fines  fundus;

    (vacuum)thema;
    si (m && m->figurae_ctx && _visus(m) == VISUS_PSALMI)
    {
        _psalmum_pingere(m, c, md);
        redde;
    }
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = c->fines.latitudo;
    f.altitudo  = c->fines.altitudo;
    figura_mensam_pingere(md, f, chorda_aequalis_literis(c->titulus,
        "focatum"));
    fundus.x         = LIMBUS_X;
    fundus.y         = LIMBUS_Y;
    fundus.latitudo  = f.latitudo - II * LIMBUS_X;
    fundus.altitudo  = f.altitudo - II * LIMBUS_Y;
    mandata_rectangulum(md, fundus, figura_color_thematis(
        COLOR_BACKGROUND), VERUM);
    si (!m || !m->figurae_ctx)
    {
        redde;
    }
    si (_visus(m) == VISUS_INDICIS)
    {
        _indicem_pingere(m, c, md);
    }
}


/* ==================================================
 * Componere
 * ================================================== */

interior vacuum
_nodum_addere (
                Componens* radix,
                  Piscina* piscina,
      InternamentumChorda* intern,
       constans character* id,
                   Partes  partes,
                    Fines  fines)
{
    Componens* c = componens_creare(piscina, intern, id, partes);

    si (c)
    {
        componens_ponere_fines(c, fines);
        componens_ponere_actio(c, actio_psalterii);
        componens_addere_liberum(radix, c);
    }
}

interior vacuum
_indicem_componere (
    constans PsalteriumMontatio* m,
                      Componens* radix,
                        Piscina* piscina,
            InternamentumChorda* intern)
{
    constans AlbumOrdo* o;
                 Album* l = _album(m, piscina);
                 Fines  f;
                   s32  a;
                   s32  b;
                   s32  t;
                   s32  i;

    si (!l)
    {
        redde;
    }
    album_pagina(l, _pagina(l, _electus(m)), &a, &b, &t);
    per (i = a; i < b; i++)
    {
        o           = album_ordo(l, (i32)i);
        f.x         = LIMBUS_X;
        f.y         = LIMBUS_Y + II + (i - a) * ORDO;
        f.latitudo  = radix->fines.latitudo - II * LIMBUS_X;
        f.altitudo  = ORDO;
        _nodum_addere(radix, piscina, intern, chorda_ut_cstr(o->id,
            piscina), PARTES_ITEM, f);
    }
    f.x         = LIMBUS_X;
    f.y         = radix->fines.altitudo - LIMBUS_Y - LINEA_PEDIS;
    f.latitudo  = SAGITTA;
    f.altitudo  = LINEA_PEDIS;
    _nodum_addere(radix, piscina, intern, "sagitta_prior",
        PARTES_NULLUM,
        f);
    f.x = radix->fines.latitudo - LIMBUS_X - SAGITTA;
    _nodum_addere(radix, piscina, intern, "sagitta_proxima",
        PARTES_NULLUM, f);
}

/* caput (globuli "≡" "«" "»", signum toni) et nodi visus */
interior vacuum
_psalmum_componere (
    constans PsalteriumMontatio* m,
                      Componens* radix,
                        Piscina* piscina,
            InternamentumChorda* intern)
{
     Psalterium* p = _privatum(m);
         chorda  signum = psalterium_signum(p->tabula,
             (i32)_signum_currens(
                    m), piscina);
          Fines f;

    gabc_visus_componere(p->visus, radix, piscina, intern);
    f.x         = LIMBUS_X;
    f.y         = LIMBUS_Y;
    f.latitudo  = LATITUDO_GLOBULI;
    f.altitudo  = MARGO_CAPITIS;
    _nodum_addere(radix, piscina, intern, "caput_index", PARTES_NULLUM,
        f);
    f.x += LATITUDO_GLOBULI;
    _nodum_addere(radix, piscina, intern, "caput_prior", PARTES_NULLUM,
        f);
    f.x += LATITUDO_GLOBULI;
    _nodum_addere(radix, piscina, intern, "caput_proximus",
        PARTES_NULLUM, f);
    f.x         += LATITUDO_GLOBULI + LITTERA;
    f.latitudo  = ((s32)signum.mensura + II) * LITTERA;
    _nodum_addere(radix, piscina, intern, "caput_signum", PARTES_NULLUM,
        f);
}

Componens*
psalterium_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    PsalteriumMontatio* m = (PsalteriumMontatio*)ctx;
             Componens* radix;
                 Fines  f;

    (vacuum)repo;
    si (!m || !m->figurae_ctx)
    {
        redde NIHIL;
    }
    radix = componens_creare(piscina, intern, "psalterium",
        PARTES_TABULA);
    si (!radix)
    {
        redde radix;
    }
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = _superficies(m, VERUM);
    f.altitudo  = _superficies(m, FALSUM);
    componens_ponere_fines(radix, f);
    componens_ponere_focusabilis(radix, VERUM);
    componens_ponere_actio(radix, actio_psalterii);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    si (_visus(m) == VISUS_INDICIS)
    {
        _indicem_componere(m, radix, piscina, intern);
    }
    alioquin
    {
        _psalmum_componere(m, radix, piscina, intern);
    }
    redde radix;
}


/* ==================================================
 * Montatio, argumentum, semita
 * ================================================== */

/* SemitaOrganum lateris: manifestum suum; ctx = PsalteriumMontatio* */
interior constans Organum*
_organa (
    vacuum* ctx,
    chorda  genus)
{
    PsalteriumMontatio* m = (PsalteriumMontatio*)ctx;

    si (   !m || !m->figurae_ctx
        || !chorda_aequalis_literis(genus, "psalterium"))
    {
        redde NIHIL;
    }
    redde _privatum(m)->organum;
}

b32
psalterium_montare (
          PsalteriumMontatio* m,
                     Piscina* piscina,
         InternamentumChorda* intern,
          InsulaRepositorium* repo,
          constans character* id,
          constans character* argumentum,
                         i32  latitudo,
                         i32  altitudo,
     constans GabcVisusPorta* porta,
             StatusSystemata* systemata)
{
    Psalterium* p;
       Piscina* textus;
           s32  n;

    si (!m || !piscina || !intern || !repo)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(PsalteriumMontatio));
    m->piscina   = piscina;
    m->intern    = intern;
    m->latitudo  = latitudo;
    m->altitudo  = altitudo;
    si (porta)
    {
        m->porta = *porta;
    }
    m->ramus = id ? insula_ramus(repo, "psalterium", id)
                  : insula_ramus_radix(repo);
    p = (Psalterium*)piscina_allocare(piscina, magnitudo(Psalterium));
    si (p == NIHIL)
    {
        redde FALSUM;
    }
    memset(p, ZEPHYRUM, magnitudo(Psalterium));
    p->organum = organum_legere_genus(piscina, intern, "psalterium",
        "psalterium");
    si (   !p->organum
        || !organum_ramum_parare(p->organum, &m->ramus, systemata,
               " focus=\"psalterium\"", id ? id : "psalterium", piscina,
               intern))
    {
        redde FALSUM;
    }
    p->tabula = psalmodia_tabulam_legere(toni_psalmorum_infixum_textus(
        piscina), piscina);
    p->scheda = piscina_generare_dynamicum("psalterium_scheda",
        (memoriae_index)(LXIV * MXXIV));
    textus    = piscina_generare_dynamicum("psalterium_textus",
        (memoriae_index)(LXIV * MXXIV));
    si (!p->tabula || !p->scheda || !textus)
    {
        redde FALSUM;
    }
    per (n = I; n <= CL; n++)
    {
        p->incipit[n - I] =
            psalterium_incipit(psalterium_infixum_psalmus(
            (i32)n, textus), piscina);
        piscina_vacare(textus);
    }
    piscina_destruere(textus);
    p->scriptor         = chorda_ex_literis(actio_psalterii, piscina);
    p->ordinarium       = psalterium_signi_index(p->tabula,
        chorda_ex_literis(PSALTERIUM_SIGNUM_ORDINARIUM, piscina));
    p->psalmus_aptatus  = -I;
    p->signum_aptatum   = -I;
    p->visus            = gabc_visus_creare(piscina, &m->ramus,
        actio_psalterii, "psalterium", &m->porta, latitudo, altitudo,
        MARGO_CAPITIS);
    si (!p->visus || p->ordinarium < ZEPHYRUM)
    {
        redde FALSUM;
    }
    m->figurae_ctx  = p;
    m->actiones     = actio_registrum_creare(piscina, intern);
    m->figurae      = figura_registrum_creare(piscina);
    si (   !m->actiones || !m->figurae
        || !praecepta_verificare(TABULA_PRAECEPTORUM)
        || !actio_registrare(m->actiones, actio_psalterii, agere, m)
        || !figura_registrare(m->figurae, PARTES_TABULA, ZEPHYRUM,
               figura_psalterii, m))
    {
        redde FALSUM;
    }
    si (argumentum && argumentum[ZEPHYRUM])
    {
        redde psalterium_argumentum_ponere(m, argumentum);
    }
    redde VERUM;
}

/* URI generis ("psalterium", "psalterium/...", "psalterium?...")? */
interior b32
_uri_est (
    constans character* argumentum)
{
    redde strncmp(argumentum, "psalterium", X) == ZEPHYRUM
        && (   argumentum[X] == '\0' || argumentum[X] == '/'
            || argumentum[X] == '?');
}

b32
psalterium_argumentum_ponere (
     PsalteriumMontatio* m,
     constans character* argumentum)
{
            Psalterium* p;
             StmlNodus* semita;
                chorda* valor;
                chorda  vitium;
                chorda  signum;
                   s32  psalmus = -I;
                   s32  n;
    constans character* tituli[IV];
    constans character* valores[IV];
             character  numerus[XVI];

    si (!m || !m->figurae_ctx)
    {
        redde FALSUM;
    }
    p = _privatum(m);
    piscina_vacare(p->scheda);
    signum.datum    = NIHIL;
    signum.mensura  = ZEPHYRUM;
    argumentum      = argumentum ? argumentum : "";
    si (_uri_est(argumentum))
    {
        semita = semita_ex_argumento(_organa, m, "psalterium",
            argumentum,
            p->scheda, m->intern, &vitium);
        si (!semita)
        {
            redde FALSUM;
        }
        /* psalmus extra I-CL ante scripturam recusatur */
        valor = stml_attributum_capere(semita, "psalmus");
        si (   valor && valor->mensura > ZEPHYRUM
            && (!chorda_ut_s32(*valor, &n) || n < I || n > CL))
        {
            redde FALSUM;
        }
        psalmus = valor && valor->mensura > ZEPHYRUM ? n : -I;
    }
    alioquin
    {
        si (!psalterium_argumentum_legere(chorda_ex_literis(argumentum,
                p->scheda), p->scheda, &psalmus, &signum, NIHIL))
        {
            redde FALSUM;
        }
        numerus[ZEPHYRUM] = '\0';
        si (psalmus > ZEPHYRUM)
        {
            sprintf(numerus, "%d", (integer)psalmus);
        }
        semita = semita_ex_argumento(_organa, m, "psalterium", numerus,
            p->scheda, m->intern, &vitium);
        si (!semita)
        {
            redde FALSUM;
        }
    }
    si (!semita_applicare(_organa, m, &m->ramus, semita, p->scheda,
            &vitium))
    {
        redde FALSUM;
    }
    tituli[ZEPHYRUM]   = "cifrae";
    valores[ZEPHYRUM]  = "";
    n                  = I;
    si (psalmus > ZEPHYRUM)
    {
        sprintf(numerus, "%d", (integer)psalmus);
        tituli[n]   = "electus";
        valores[n]  = numerus;
        n++;
    }
    si (psalmus > ZEPHYRUM && psalmus != p->psalmus_aptatus)
    {
        /* psalmus novus: pagina 0 */
        tituli[n]   = "pagina";
        valores[n]  = "";
        n++;
    }
    si (signum.mensura > ZEPHYRUM)
    {
        tituli[n]   = "signum";
        valores[n]  = chorda_ut_cstr(signum, p->scheda);
        n++;
    }
    redde _campos_scribere(m, tituli, valores, (i32)n)
        && psalterium_semitam_secuta(m);
}

/* post semitam (initium, historia, argumentum): psalmus ex campis
 * aptatur (idem non iteratur); index: sonus sistit */
b32
psalterium_semitam_secuta (
    vacuum* ctx)
{
    PsalteriumMontatio* m = (PsalteriumMontatio*)ctx;

    si (!m || !m->figurae_ctx)
    {
        redde FALSUM;
    }
    si (_visus(m) == VISUS_INDICIS)
    {
        (vacuum)gabc_visus_sistere(_privatum(m)->visus);
        redde VERUM;
    }
    redde _aptare(m);
}

b32
psalterium_pulsare (
    vacuum* ctx)
{
    PsalteriumMontatio* m = (PsalteriumMontatio*)ctx;

    redde m && m->figurae_ctx
        && gabc_visus_pulsare(_privatum(m)->visus);
}

Mora
psalterium_mora (
    vacuum* ctx)
{
    PsalteriumMontatio* m = (PsalteriumMontatio*)ctx;

    redde m && m->figurae_ctx ? gabc_visus_mora(_privatum(m)->visus)
        : -I;
}

chorda
psalterium_titulus (
     vacuum* ctx,
    Piscina* piscina)
{
    PsalteriumMontatio* m = (PsalteriumMontatio*)ctx;
             character  textus[XXIV];

    si (m && m->figurae_ctx && _visus(m) == VISUS_PSALMI)
    {
        sprintf(textus, "Psalmus %d", (integer)_campus_numerus(m,
            "psalmus", I, CL));
        redde chorda_ex_literis(textus, piscina);
    }
    redde chorda_ex_literis("Psalterium", piscina);
}
