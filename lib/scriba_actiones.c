/* scriba_actiones.c - pagina.clavis: vim super folium laboris */

#include "scriba_actiones.h"
#include "vim.h"
#include "eventus.h"
#include "stml.h"

#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

/* capsa effugita maxima: XXXII lineae x (LXVII + '\n') */
#define CAPSA_OCTETI \
    (VIM_CLIPBOARD_LINEAE_MAXIMAE * VIM_CLIPBOARD_LINEA_MAXIMA)

interior s32
attributum_s32 (
    InsulaRepositorium* repo,
    constans character* titulus,
                   s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = insula_attributum(repo, INSULA_EPHEMERA, titulus);
    si (a && chorda_ut_s32(*a, &v))
    {
        redde v;
    }
    redde praestitutum;
}

/* tempora (ms) s64: Eventus.tempus s32 excedere potest */
interior s64
attributum_s64 (
    InsulaRepositorium* repo,
    constans character* titulus)
{
    chorda* a;
       s64  v;

    a = insula_attributum(repo, INSULA_EPHEMERA, titulus);
    si (a && chorda_ut_s64(*a, &v))
    {
        redde v;
    }
    redde ZEPHYRUM;
}

interior b32
attributum_est (
    InsulaRepositorium* repo,
    constans character* titulus,
    constans character* valor)
{
    chorda* a;

    a = insula_attributum(repo, INSULA_EPHEMERA, titulus);
    redde a ? chorda_aequalis_literis(*a, valor) : FALSUM;
}

interior constans character*
modus_titulus (
    ModoVim modus)
{
    commutatio (modus)
    {
        casus MODO_VIM_INSERERE: redde "inserere";
        casus MODO_VIM_VISUALIS: redde "visualis";
        ordinarius:              redde "normalis";
    }
}

interior i32
limitare (
    s32 v,
    i32 maximum)
{
    si (v < ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si ((i32)v >= maximum)
    {
        redde maximum > ZEPHYRUM ? maximum - I : ZEPHYRUM;
    }
    redde (i32)v;
}

/* folium laboris = proiectio documenti (post revocare/reficere) */
interior vacuum
laboris_reficere (
    ScribaActiones* sa)
{
    constans TabulaCharacterum* t;

    t = scriba_documentum_tabula(sa->doc);
    memcpy(sa->laboris.cellulae, t->cellulae,
           (size_t)(t->latitudo * t->altitudo));
    memcpy(sa->laboris.indentatio, t->indentatio,
           (size_t)t->altitudo * magnitudo(s32));
}


/* ==================================================
 * Status: lectio et scriptura
 * ================================================== */

/* capsa effugita -> clipboard vim (lineae '\n' separatae) */
interior vacuum
capsam_legere (
    InsulaRepositorium* repo,
          VimClipboard* capsa)
{
        chorda* a;
     character  octeti[CAPSA_OCTETI];
           s32  n;
           s32  longitudo;
           s32  i;
           i32  linea;
           i32  columna;

    vim_clipboard_initiare(capsa);
    n = attributum_s32(repo, "capsa_lineae", ZEPHYRUM);
    si (n <= ZEPHYRUM)
    {
        redde;
    }
    si (n > VIM_CLIPBOARD_LINEAE_MAXIMAE)
    {
        n = VIM_CLIPBOARD_LINEAE_MAXIMAE;
    }
    a = insula_attributum(repo, INSULA_EPHEMERA, "capsa");
    longitudo = a ? scriba_solvere(*a, octeti, CAPSA_OCTETI) : ZEPHYRUM;
    si (longitudo < ZEPHYRUM)
    {
        redde;
    }
    linea    = ZEPHYRUM;
    columna  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < longitudo; i++)
    {
        si (octeti[i] == '\n')
        {
            linea++;
            columna = ZEPHYRUM;
            si ((s32)linea >= n)
            {
                frange;
            }
            perge;
        }
        si (columna < VIM_CLIPBOARD_LINEA_MAXIMA - I)
        {
            capsa->lineae[linea][columna] = octeti[i];
            columna++;
        }
    }
    capsa->numerus_linearum = (i32)n;
}

/* clipboard vim -> capsa effugita; vacua si nullus octetus */
interior chorda
capsam_scribere (
    constans VimClipboard* capsa,
                  Piscina* piscina)
{
    character octeti[CAPSA_OCTETI];
          i32 n;
          i32 i;
          i32 c;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < capsa->numerus_linearum; i++)
    {
        si (i > ZEPHYRUM)
        {
            octeti[n] = '\n';
            n++;
        }
        per (c = ZEPHYRUM; c < VIM_CLIPBOARD_LINEA_MAXIMA - I
             && capsa->lineae[i][c] != '\0'; c++)
        {
            octeti[n] = capsa->lineae[i][c];
            n++;
        }
    }
    redde scriba_effugere(octeti, n, piscina);
}

interior VimStatus
status_legere (
    InsulaRepositorium* repo,
        ScribaActiones* sa,
          VimClipboard* capsa)
{
    VimStatus  st;
       chorda* a;
    character  c;

    capsam_legere(repo, capsa);
    st = vim_initiare_cum_contextu(&sa->laboris, capsa, NIHIL);
    st.cursor_linea    = limitare(attributum_s32(repo, "cursor_linea",
        ZEPHYRUM), sa->laboris.altitudo);
    st.cursor_columna  = limitare(attributum_s32(repo, "cursor_columna",
        ZEPHYRUM), sa->laboris.latitudo);
    st.modo = attributum_est(repo, "modus",
        "inserere") ? MODO_VIM_INSERERE
            : attributum_est(repo, "modus",
            "visualis") ? MODO_VIM_VISUALIS
            : MODO_VIM_NORMALIS;
    st.visualis_tipo = attributum_est(repo, "visualis_genus",
        "character")
                     ? MODO_VIM_VISUALIS_CHARACTER
                     : MODO_VIM_VISUALIS_LINEA;
    st.selectio_initium_linea    = attributum_s32(repo,
        "selectio_linea",
        -I);
    st.selectio_initium_columna  = attributum_s32(repo,
        "selectio_columna", ZEPHYRUM);
    a = insula_attributum(repo, INSULA_EPHEMERA, "clavis_praecedens");
    si (a && scriba_solvere(*a, &c, I) == I)
    {
        st.clavis_praecedens = c;
    }
    si (attributum_est(repo, "fd_exspectans", "verum"))
    {
        st.esperans_fd = VERUM;
        st.tempus_f = (f64)attributum_s64(repo, "fd_tempus")
            / 1000.0;
    }
    redde st;
}

nomen structura {
         VimStatus st;
            chorda capsa;         /* effugita; vacua = tollere */
               i32 capsa_lineae;
            chorda praecedens;    /* effugita; vacua = tollere */
} Scriptura;

interior vacuum
numerum_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
     constans character* titulus,
                    s64  valor)
{
    insula_attributum_ponere(radix, p, in, titulus,
        chorda_ut_cstr(chorda_ex_s64(valor, p), p));
}

/* textum ponere aut, si vacuus, TOLLERE (nullus valor vacuus) */
interior vacuum
textum_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
     constans character* titulus,
                 chorda  valor)
{
    si (chorda_vacua(valor))
    {
        (vacuum)insula_attributum_tollere(radix, titulus);
        redde;
    }
    insula_attributum_ponere(radix, p, in, titulus,
                             chorda_ut_cstr(valor, p));
}

interior vacuum
status_scribere_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    constans Scriptura* s;

    s = (constans Scriptura*)ctx;
    numerum_ponere(radix, p, in, "cursor_linea",
        (s64)s->st.cursor_linea);
    numerum_ponere(radix, p, in, "cursor_columna",
                   (s64)s->st.cursor_columna);
    insula_attributum_ponere(radix, p, in, "modus",
                             modus_titulus(s->st.modo));
    insula_attributum_ponere(radix, p, in, "visualis_genus",
        s->st.visualis_tipo == MODO_VIM_VISUALIS_CHARACTER
        ? "character" : "linea");
    numerum_ponere(radix, p, in, "selectio_linea",
                   (s64)s->st.selectio_initium_linea);
    numerum_ponere(radix, p, in, "selectio_columna",
                   (s64)s->st.selectio_initium_columna);
    textum_ponere(radix, p, in, "clavis_praecedens", s->praecedens);
    insula_attributum_ponere(radix, p, in, "fd_exspectans",
                             s->st.esperans_fd ? "verum" : "falsum");
    numerum_ponere(radix, p, in, "fd_tempus",
                   (s64)(s->st.tempus_f * 1000.0 + 0.5));
    textum_ponere(radix, p, in, "capsa", s->capsa);
    numerum_ponere(radix, p, in, "capsa_lineae", (s64)s->capsa_lineae);
}

interior vacuum
status_scribere (
       InsulaRepositorium* repo,
           ScribaActiones* sa,
                VimStatus  st,
    constans VimClipboard* capsa)
{
    Scriptura s;
    character c;

    s.st                  = st;
    s.capsa               = capsam_scribere(capsa, sa->doc->piscina);
    s.capsa_lineae        = capsa->numerus_linearum;
    s.praecedens.datum    = NIHIL;
    s.praecedens.mensura  = ZEPHYRUM;
    si (st.clavis_praecedens != '\0')
    {
        c             = st.clavis_praecedens;
        s.praecedens  = scriba_effugere(&c, I, sa->doc->piscina);
    }
    (vacuum)mutare_ephemera(repo, status_scribere_mutator, &s);
}


/* ==================================================
 * Gestus: vim in porta, effusio
 * ================================================== */

nomen structura {
    VimStatus st;
          s32 clavis;
          s64 tempus;   /* ms, Eventus.tempus */
} Pactum;

/* '\0' -> ' ' post clavem quamque: tabula '\0' in columnam ultimam
 * scribit cum trahit sinistram (tabula_trahere_sinistram), cetera
 * spatia - eadem vacuitas tabulae et vim, octeti diversi: sine hoc
 * mutatio invisibilis actum faceret ('ifd' - f insertum et deletum) */
interior vacuum
albare (
    TabulaCharacterum* t)
{
    i32 i;
    i32 n;

    n = t->latitudo * t->altitudo;
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (t->cellulae[i] == '\0')
        {
            t->cellulae[i] = ' ';
        }
    }
}

interior vacuum
vim_mutator (
     Motus* motus,
    vacuum* ctx)
{
    Pactum* pactum;

    (vacuum)motus;
    pactum     = (Pactum*)ctx;
    pactum->st = vim_tractare_clavem_cum_tempore(pactum->st,
        pactum->clavis,
        (f64)pactum->tempus / 1000.0);
    albare(pactum->st.tabula);
}

/* frustum primum insertionis actum simplex, cetera coniuncta; extra
 * insertionem effusio insertionem claudit */
interior b32
gestum_effundere (
                vacuum* gestus,
    InsulaRepositorium* repo,
                vacuum* ctx)
{
    ScribaActiones* sa;
               s64  seq;

    (vacuum)repo;
    (vacuum)ctx;
    sa  = (ScribaActiones*)gestus;
    seq = sa->insertio_commissa
        ? scriba_documentum_committere_coniunctum(sa->doc, &sa->laboris)
        : scriba_documentum_committere(sa->doc, &sa->laboris);
    si (sa->inserere)
    {
        si (seq > ZEPHYRUM)
        {
            sa->insertio_commissa = VERUM;
        }
    }
    alioquin
    {
        sa->insertio_commissa = FALSUM;
    }
    redde VERUM;
}

/* revocare (retro) aut reficere: pendentia primum effunduntur */
interior vacuum
historiam_movere (
       InsulaRepositorium* repo,
                    Motus* motus,
           ScribaActiones* sa,
                VimStatus  st,
    constans VimClipboard* capsa,
                      b32  retro)
{
    (vacuum)motus_gestum_effundere(motus, repo);
    si (retro)
    {
        (vacuum)scriba_documentum_revocare(sa->doc);
    }
    alioquin
    {
        (vacuum)scriba_documentum_reficere(sa->doc);
    }
    laboris_reficere(sa);
    sa->insertio_commissa  = FALSUM;
    st.clavis_praecedens   = '\0';
    status_scribere(repo, sa, st, capsa);
}

interior vacuum
clavem_tractare (
    InsulaRepositorium* repo,
                 Motus* motus,
        ScribaActiones* sa,
                   s32  clavis,
                   s64  tempus)
{
    VimClipboard capsa;
          Pactum pactum;

    pactum.st = status_legere(repo, sa, &capsa);
    si (   pactum.st.modo              == MODO_VIM_NORMALIS
        && pactum.st.clavis_praecedens == '\0' && clavis == 'u')
    {
        historiam_movere(repo, motus, sa, pactum.st, &capsa, VERUM);
        redde;
    }
    pactum.clavis = clavis;
    pactum.tempus = tempus;
    mutare_gestum(motus, vim_mutator, &pactum, tempus);
    sa->inserere = (b32)(pactum.st.modo == MODO_VIM_INSERERE);
    status_scribere(repo, sa, pactum.st, &capsa);
    si (!sa->inserere)
    {
        (vacuum)motus_gestum_effundere(motus, repo);
    }
}

interior s32
clavis_nominata (
    constans Eventus* ev)
{
    commutatio (ev->datum.clavis.clavis)
    {
        casus CLAVIS_SURSUM:     redde VIM_CLAVIS_SURSUM;
        casus CLAVIS_DEORSUM:    redde VIM_CLAVIS_DEORSUM;
        casus CLAVIS_SINISTER:   redde VIM_CLAVIS_SINISTRAM;
        casus CLAVIS_DEXTER:     redde VIM_CLAVIS_DEXTRAM;
        casus CLAVIS_RETRORSUM:  redde VIM_CLAVIS_BACKSPACE;
        casus CLAVIS_DELERE:     redde VIM_CLAVIS_DELETE;
        casus CLAVIS_REDITUS:    redde VIM_CLAVIS_ENTER;
        casus CLAVIS_TABULA:     redde VIM_CLAVIS_TAB;
        casus CLAVIS_EFFUGIUM:   redde VIM_CLAVIS_ESCAPE;
        casus CLAVIS_DOMUS:      redde VIM_CLAVIS_HOME;
        casus CLAVIS_FINIS:      redde VIM_CLAVIS_END;
        ordinarius:              redde ZEPHYRUM;
    }
}


/* ==================================================
 * Tractator
 * ================================================== */

/* <tractator/> */
b32
scriba_pagina_clavis (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
     ScribaActiones* sa;
       VimClipboard  capsa;
          VimStatus  st;
                s32  clavis;
                i32  i;
                 i8  o;

    (vacuum)destinatio;
    (vacuum)nodus;
    sa = (ScribaActiones*)ctx;
    si (!repo || !motus || !ev || !sa)
    {
        redde FALSUM;
    }
    si (ev->genus == EVENTUS_TEXTUS)
    {
        si (ev->datum.textus.genus != EVENTUS_TEXTUS_COMMISSUM)
        {
            redde FALSUM;
        }
        per (i = ZEPHYRUM; i < ev->datum.textus.contentum.mensura; i++)
        {
            o = ev->datum.textus.contentum.datum[i];
            si (o == '\n' || o == '\r')
            {
                clavis = VIM_CLAVIS_ENTER;
            }
            alioquin si (o >= XXXII && o < CXXVII)
            {
                clavis = (s32)o;
            }
            alioquin
            {
                perge;   /* octeti >= 0x80: v1 ignorati */
            }
            clavem_tractare(repo, motus, sa, clavis, ev->tempus);
        }
        redde VERUM;
    }
    si (ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    /* Ctrl-R in modo normali: reficere */
    si (   (ev->datum.clavis.modificantes & MOD_IMPERIUM)
        && ev->datum.clavis.runa == 'r')
    {
        si (   !attributum_est(repo, "modus", "normalis")
            && insula_attributum(repo, INSULA_EPHEMERA, "modus"))
        {
            redde FALSUM;
        }
        st = status_legere(repo, sa, &capsa);
        historiam_movere(repo, motus, sa, st, &capsa, FALSUM);
        redde VERUM;
    }
    clavis = clavis_nominata(ev);
    si (clavis == ZEPHYRUM)
    {
        redde FALSUM;   /* imprimibilis: per TEXTUM venit */
    }
    /* Tab in modo normali: focus proximus (dispensator) */
    si (   clavis == VIM_CLAVIS_TAB
        && !attributum_est(repo, "modus", "inserere"))
    {
        redde FALSUM;
    }
    clavem_tractare(repo, motus, sa, clavis, ev->tempus);
    redde VERUM;
}


/* ==================================================
 * Vita
 * ================================================== */

vacuum
scriba_actiones_initiare (
      ScribaActiones* sa,
    ScribaDocumentum* doc,
             Piscina* piscina)
{
    constans TabulaCharacterum* t;

    si (!sa || !doc || !piscina)
    {
        redde;
    }
    memset(sa, ZEPHYRUM, magnitudo(ScribaActiones));
    sa->doc  = doc;
    t        = scriba_documentum_tabula(doc);
    tabula_initiare(&sa->laboris, piscina, t->latitudo, t->altitudo);
    laboris_reficere(sa);
}

vacuum
scriba_actiones_registrare (
    ActioRegistrum* reg,
    ScribaActiones* sa)
{
    (vacuum)actio_registrare(reg, "pagina.clavis", scriba_pagina_clavis,
                             sa);
}

vacuum
scriba_gestum_ponere (
             Motus* motus,
    ScribaActiones* sa)
{
    motus_gestum_ponere(motus, sa, gestum_effundere, NIHIL,
                        (s64)SCRIBA_QUIES_MS);
}
