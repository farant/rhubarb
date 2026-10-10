/* scriba_actiones.c - pagina.clavis: vim super folium laboris */

#include "scriba_actiones.h"
#include "vim.h"
#include "eventus.h"
#include "stml.h"
#include "destinatio.h"

#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

/* capsa effugita maxima: XXXII lineae x (LXVII + '\n') */
#define CAPSA_OCTETI \
    (VIM_CLIPBOARD_LINEAE_MAXIMAE * VIM_CLIPBOARD_LINEA_MAXIMA)

interior s32
attributum_s32 (
    constans InsulaRamus* ramus,
      constans character* titulus,
                     s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = insula_ramus_attributum(ramus, INSULA_EPHEMERA, titulus);
    si (a && chorda_ut_s32(*a, &v))
    {
        redde v;
    }
    redde praestitutum;
}

/* tempora (ms) s64: Eventus.tempus s32 excedere potest */
interior s64
attributum_s64 (
    constans InsulaRamus* ramus,
      constans character* titulus)
{
    chorda* a;
       s64  v;

    a = insula_ramus_attributum(ramus, INSULA_EPHEMERA, titulus);
    si (a && chorda_ut_s64(*a, &v))
    {
        redde v;
    }
    redde ZEPHYRUM;
}

interior b32
attributum_est (
    constans InsulaRamus* ramus,
      constans character* titulus,
      constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(ramus, INSULA_EPHEMERA, titulus);
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
    /* S2b: folium laboris ex hac versione */
    sa->cursor_laboris = scriba_documentum_cursor(sa->doc);
}


/* ==================================================
 * Status: lectio et scriptura
 * ================================================== */

/* capsa effugita -> clipboard vim (lineae '\n' separatae) */
interior vacuum
capsam_legere (
    constans InsulaRamus* ramus,
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
    n = attributum_s32(ramus, "capsa_lineae", ZEPHYRUM);
    si (n <= ZEPHYRUM)
    {
        redde;
    }
    si (n > VIM_CLIPBOARD_LINEAE_MAXIMAE)
    {
        n = VIM_CLIPBOARD_LINEAE_MAXIMAE;
    }
    a = insula_ramus_attributum(ramus, INSULA_EPHEMERA, "capsa");
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
    constans InsulaRamus* ramus,
          ScribaActiones* sa,
            VimClipboard* capsa)
{
    VimStatus  st;
       chorda* a;
    character  c;

    capsam_legere(ramus, capsa);
    st = vim_initiare_cum_contextu(&sa->laboris, capsa, NIHIL);
    st.cursor_linea    = limitare(attributum_s32(ramus, "cursor_linea",
        ZEPHYRUM), sa->laboris.altitudo);
    st.cursor_columna  = limitare(attributum_s32(ramus,
        "cursor_columna",
        ZEPHYRUM), sa->laboris.latitudo);
    st.modo = attributum_est(ramus, "modus",
        "inserere") ? MODO_VIM_INSERERE
            : attributum_est(ramus, "modus",
            "visualis") ? MODO_VIM_VISUALIS
            : MODO_VIM_NORMALIS;
    st.visualis_tipo = attributum_est(ramus, "visualis_genus",
        "character")
                     ? MODO_VIM_VISUALIS_CHARACTER
                     : MODO_VIM_VISUALIS_LINEA;
    st.selectio_initium_linea    = attributum_s32(ramus,
        "selectio_linea",
        -I);
    st.selectio_initium_columna  = attributum_s32(ramus,
        "selectio_columna", ZEPHYRUM);
    a = insula_ramus_attributum(ramus, INSULA_EPHEMERA,
        "clavis_praecedens");
    si (a && scriba_solvere(*a, &c, I) == I)
    {
        st.clavis_praecedens = c;
    }
    si (attributum_est(ramus, "fd_exspectans", "verum"))
    {
        st.esperans_fd = VERUM;
        st.tempus_f = (f64)attributum_s64(ramus, "fd_tempus")
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
       constans InsulaRamus* ramus,
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
    (vacuum)mutare_ramum(ramus, INSULA_EPHEMERA,
        status_scribere_mutator, &s);
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
    /* S2b: proiectio == folium laboris post commissionem */
    sa->cursor_laboris = scriba_documentum_cursor(sa->doc);
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
       constans InsulaRamus* ramus,
                      Motus* motus,
             ScribaActiones* sa,
                  VimStatus  st,
      constans VimClipboard* capsa,
                        b32  retro)
{
    (vacuum)motus_gestum_effundere(motus, ramus->repo);
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
    status_scribere(ramus, sa, st, capsa);
}

interior vacuum
clavem_tractare (
    constans InsulaRamus* ramus,
                   Motus* motus,
          ScribaActiones* sa,
                     s32  clavis,
                     s64  tempus)
{
    VimClipboard capsa;
          Pactum pactum;

    pactum.st = status_legere(ramus, sa, &capsa);
    si (   pactum.st.modo              == MODO_VIM_NORMALIS
        && pactum.st.clavis_praecedens == '\0' && clavis == 'u')
    {
        historiam_movere(ramus, motus, sa, pactum.st, &capsa, VERUM);
        redde;
    }
    pactum.clavis = clavis;
    pactum.tempus = tempus;
    mutare_gestum(motus, vim_mutator, &pactum, tempus);
    sa->inserere = (b32)(pactum.st.modo == MODO_VIM_INSERERE);
    status_scribere(ramus, sa, pactum.st, &capsa);
    si (!sa->inserere)
    {
        (vacuum)motus_gestum_effundere(motus, ramus->repo);
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

/* dimensiones folii in ramo durabili (S2b: pagina mutata) */
interior vacuum
dimensiones_mutator (
              StmlNodus* nodus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    constans ScribaDocumentum* doc;

    doc = (constans ScribaDocumentum*)ctx;
    insula_attributum_ponere(nodus, p, in, "latitudo",
        chorda_ut_cstr(chorda_ex_s32((s32)doc->latitudo, p), p));
    insula_attributum_ponere(nodus, p, in, "altitudo",
        chorda_ut_cstr(chorda_ex_s32((s32)doc->altitudo, p), p));
}

/* S2b/S3d: visus ad paginam 'novum' libri. Gestus pendens primum
 * effunditur; deinde documentum, folium laboris, plagula visus, cursor
 * ad (linea, columna) praecisus, modo normali (status pagina.clavis est
 * - ideo hic), dimensiones folii in ramo durabili. FALSUM si pagina
 * aperiri nequit. */
interior b32
paginam_ponere (
    constans InsulaRamus* ramus,
                   Motus* motus,
          ScribaActiones* sa,
                  chorda  novum,
                     s32  linea,
                     s32  columna)
{
         VimClipboard  capsa;
            VimStatus  st;
     ScribaDocumentum* doc;

    (vacuum)motus_gestum_effundere(motus, ramus->repo);
    doc = scriba_liber_pagina(sa->liber, novum);
    si (!doc)
    {
        redde FALSUM;
    }
    si (   doc->latitudo != sa->laboris.latitudo
        || doc->altitudo != sa->laboris.altitudo)
    {
        tabula_initiare(&sa->laboris, doc->piscina, doc->latitudo,
            doc->altitudo);
    }
    sa->doc                = doc;
    sa->inserere           = FALSUM;
    sa->insertio_commissa  = FALSUM;
    laboris_reficere(sa);
    (vacuum)volumen_plagulam_condere(doc->volumen, sa->visus, novum,
        "scriba:visus");
    st                           = status_legere(ramus, sa, &capsa);
    st.cursor_linea              = (i32)(linea < (s32)doc->altitudo
                                         ? linea : ZEPHYRUM);
    st.cursor_columna            = (i32)(columna < (s32)doc->latitudo
                                         ? columna : ZEPHYRUM);
    st.modo                      = MODO_VIM_NORMALIS;
    st.selectio_initium_linea    = -I;
    st.selectio_initium_columna  = ZEPHYRUM;
    st.clavis_praecedens         = '\0';
    st.esperans_fd               = FALSUM;
    status_scribere(ramus, sa, st, &capsa);
    (vacuum)mutare_ramum(ramus, INSULA_DURABILIS, dimensiones_mutator,
        doc);
    redde VERUM;
}

/* index paginae visus in libro (plagula visus; absens aut ignota: 0) */
interior s32
index_visus (
    constans ScribaActiones* sa)
{
    chorda currens;
       b32 inventum;
       s32 i;

    currens = volumen_plagulam_promere(sa->doc->volumen, sa->visus,
        sa->doc->piscina, &inventum);
    i = inventum ? scriba_liber_index(sa->liber, currens) : -I;
    redde i < ZEPHYRUM ? ZEPHYRUM : i;
}

/* S2b: pagina proxima (gradus I) aut prior (-I) libri; ultra ultimam
 * nova, ante primam nihil; cursor ad 0,0 */
interior vacuum
paginam_mutare (
    constans InsulaRamus* ramus,
                   Motus* motus,
          ScribaActiones* sa,
                     s32  gradus)
{
    chorda novum;
       s32 i;

    (vacuum)motus_gestum_effundere(motus, ramus->repo);
    i = index_visus(sa) + gradus;
    si (i < ZEPHYRUM)
    {
        redde;
    }
    novum = i >= (s32)scriba_liber_numerus(sa->liber)
          ? scriba_liber_pagina_nova(sa->liber)
          : scriba_liber_nomen(sa->liber, (i32)i);
    (vacuum)paginam_ponere(ramus, motus, sa, novum, ZEPHYRUM, ZEPHYRUM);
}

/* S3a: cellula sub puncto schirmi ex fines nodi (folium in pixelis)
 * / folium, praecisa ad folium; FALSUM si nodus folio minor */
interior b32
cellulam_ictam (
        constans ScribaActiones* sa,
             constans Componens* nodus,
                        Punctum  p,
                            s32* linea,
                            s32* columna)
{
    s32 cw;
    s32 ch;

    cw = nodus->fines.latitudo / (s32)sa->laboris.latitudo;
    ch = nodus->fines.altitudo / (s32)sa->laboris.altitudo;
    si (cw <= ZEPHYRUM || ch <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    p         = destinatio_ad_locale(nodus, p);
    *columna  = p.x < ZEPHYRUM ? ZEPHYRUM : p.x / cw;
    *linea    = p.y < ZEPHYRUM ? ZEPHYRUM : p.y / ch;
    si (*columna >= (s32)sa->laboris.latitudo)
    {
        *columna = (s32)sa->laboris.latitudo - I;
    }
    si (*linea >= (s32)sa->laboris.altitudo)
    {
        *linea = (s32)sa->laboris.altitudo - I;
    }
    redde VERUM;
}

/* S3a: cursor in cellulam (linea, columna). Gestus pendens primum
 * effunditur et insertio clauditur (ut vim: ictus unitatem revocandi
 * frangit), modus inserendi manet; visualis ad normalem redit,
 * selectio et clavis praecedens tolluntur. */
interior vacuum
cursorem_ponere (
    constans InsulaRamus* ramus,
                   Motus* motus,
          ScribaActiones* sa,
                     s32  linea,
                     s32  columna)
{
    VimClipboard capsa;
       VimStatus st;

    (vacuum)motus_gestum_effundere(motus, ramus->repo);
    sa->insertio_commissa  = FALSUM;
    st                     = status_legere(ramus, sa, &capsa);
    st.cursor_linea        = (i32)linea;
    st.cursor_columna      = (i32)columna;
    si (st.modo == MODO_VIM_VISUALIS)
    {
        st.modo = MODO_VIM_NORMALIS;
    }
    st.selectio_initium_linea    = -I;
    st.selectio_initium_columna  = ZEPHYRUM;
    st.clavis_praecedens         = '\0';
    st.esperans_fd               = FALSUM;
    status_scribere(ramus, sa, st, &capsa);
}

/* S3b-2: nuntius in linea status (vacuus = tollere) */
interior vacuum
nuntius_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    textum_ponere(radix, p, in, "nuntius", *(constans chorda*)ctx);
}

interior vacuum
nuntium_ponere (
    constans InsulaRamus* ramus,
                  chorda  nuntius)
{
    (vacuum)mutare_ramum(ramus, INSULA_EPHEMERA, nuntius_mutator,
        &nuntius);
}

/* S3b-2: nuntius ad clavem aut ictum proximum tollitur */
interior vacuum
nuntium_tollere (
    constans InsulaRamus* ramus)
{
    chorda vacua;

    si (insula_ramus_attributum(ramus, INSULA_EPHEMERA, "nuntius"))
    {
        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        nuntium_ponere(ramus, vacua);
    }
}

/* S3b: substitutio iussi in folio laboris (intra gestum) */
nomen structura {
      ScribaActiones* sa;
     constans Iussum* iussum;
                 b32  consumit;
              chorda  textus;
                 s32  columna;    /* exitus: post textum insertum */
} Substitutio;

interior vacuum
substitutio_mutator (
     Motus* motus,
    vacuum* ctx)
{
          Substitutio* sub;
    TabulaCharacterum* t;
                  s32  a;
                  s32  i;

    (vacuum)motus;
    sub  = (Substitutio*)ctx;
    t    = &sub->sa->laboris;
    a    = sub->consumit ? sub->iussum->initium : sub->iussum->finis;
    si (sub->consumit)
    {
        per (i = sub->iussum->initium; i < sub->iussum->finis; i++)
        {
            tabula_delere_characterem(t, (i32)sub->iussum->linea,
                (i32)a);
        }
    }
    sub->columna = a;
    si (   sub->textus.mensura > ZEPHYRUM
        && tabula_inserere_spatium(t, (i32)sub->iussum->linea, (i32)a,
               sub->textus.mensura))
    {
        per (i = ZEPHYRUM; i < (s32)sub->textus.mensura
                           && a + i < (s32)t->latitudo; i++)
        {
            t->cellulae[(i32)sub->iussum->linea * t->latitudo
                + (i32)(a + i)] = (character)sub->textus.datum[i];
        }
        sub->columna = a + i;
    }
    albare(t);
}

/* S3b: iussum ictum currit. Gestus pendens primum effunditur;
 * consumens signum suum textu effectus substituit, aliter textus post
 * signum inseritur; mutatio commissio UNA (u signum restituit),
 * cursor post textum. Error aut functio FALSUM: nihil mutatur,
 * nuntius in linea status (S3b-2; FALSUM sine errore: "<verbum>:
 * defecit"). */
interior vacuum
iussum_exsequi (
    constans InsulaRamus* ramus,
                   Motus* motus,
          ScribaActiones* sa,
         constans Iussum* iussum,
                     s64  tempus)
{
    IussumEffectus eff;
       Substitutio sub;

    (vacuum)motus_gestum_effundere(motus, ramus->repo);
    sa->insertio_commissa = FALSUM;
    si (!iussum_currere(sa->iussa, iussum, sa->doc->piscina, &eff))
    {
        nuntium_ponere(ramus, eff.error.mensura > ZEPHYRUM ? eff.error
            : chorda_concatenare(iussum->verbum, chorda_ex_literis(
                  ": defecit", sa->doc->piscina), sa->doc->piscina));
        redde;
    }
    si (eff.error.mensura > ZEPHYRUM)
    {
        nuntium_ponere(ramus, eff.error);
        redde;
    }
    sub.sa        = sa;
    sub.iussum    = iussum;
    sub.consumit  = iussum_consumit(sa->iussa, iussum->verbum);
    sub.textus    = eff.textus;
    sub.columna   = iussum->finis;
    si (sub.consumit || sub.textus.mensura > ZEPHYRUM)
    {
        mutare_gestum(motus, substitutio_mutator, &sub, tempus);
    }
    /* cursorem_ponere effundit: substitutio commissio una */
    cursorem_ponere(ramus, motus, sa, iussum->linea,
        sub.columna < (s32)sa->laboris.latitudo
        ? sub.columna : (s32)sa->laboris.latitudo - I);
}

/* verbum nexus numerus totus? ('#3' = pagina id III) */
interior b32
numerus_est (
    chorda c)
{
    i32 i;

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (c.datum[i] < '0' || c.datum[i] > '9')
        {
            redde FALSUM;
        }
    }
    redde c.mensura > ZEPHYRUM;
}

/* S3d: nexus ictus (Franus: paginae numeris solis nominantur; tags
 * per paginas cycli). '#next' '#prev' ut Ctrl+Shift+sagittae, '#first'
 * '#last', '#N' pagina id N; ceteri tags: pagina ALIA proxima (post
 * visam, circulo) quae '#tag' continet, cursor in eo; retro
 * (Shift+ictus, Franus): ANTE visam. Nihil: nuntius ('nulla pagina' /
 * 'nulla alia pagina'). */
interior vacuum
nexum_sequi (
    constans InsulaRamus* ramus,
                   Motus* motus,
          ScribaActiones* sa,
                  chorda  verbum,
                     b32  retro)
{
    constans TabulaCharacterum* t;
                        Iussum  x;
                        chorda  nomen_paginae;
                           s32  n;
                           s32  i;
                           s32  k;
                           s32  l;
                           s32  a;
                       Piscina* p;

    p = sa->doc->piscina;
    (vacuum)motus_gestum_effundere(motus, ramus->repo);
    sa->insertio_commissa  = FALSUM;
    n                      = (s32)scriba_liber_numerus(sa->liber);
    si (   chorda_aequalis_literis(verbum, "next")
        || chorda_aequalis_literis(verbum, "prev"))
    {
        paginam_mutare(ramus, motus, sa,
            chorda_aequalis_literis(verbum, "next") ? I : -I);
        redde;
    }
    si (   (chorda_aequalis_literis(verbum, "first")
            || chorda_aequalis_literis(verbum, "last"))
        && n > ZEPHYRUM)
    {
        (vacuum)paginam_ponere(ramus, motus, sa, scriba_liber_nomen(
            sa->liber, chorda_aequalis_literis(verbum, "first")
            ? ZEPHYRUM : (i32)(n - I)), ZEPHYRUM, ZEPHYRUM);
        redde;
    }
    si (   numerus_est(verbum) && scriba_liber_index(sa->liber, verbum)
            >= ZEPHYRUM)
    {
        (vacuum)paginam_ponere(ramus, motus, sa, verbum, ZEPHYRUM,
            ZEPHYRUM);
        redde;
    }
    si (!numerus_est(verbum))
    {
        i = index_visus(sa);
        /* paginae ALIAE solum (visa ipsa: nuntius 'nulla alia') */
        per (k = I; k < n; k++)
        {
            nomen_paginae = scriba_liber_nomen(sa->liber,
                (i32)((retro ? i + n - k : i + k) % n));
            si (!scriba_liber_pagina(sa->liber, nomen_paginae))
            {
                perge;
            }
            t = scriba_documentum_tabula(scriba_liber_pagina(sa->liber,
                nomen_paginae));
            per (l = ZEPHYRUM; l < (s32)t->altitudo; l++)
            {
                a = ZEPHYRUM;
                dum (iussum_nexus_proximus(t, l, a, p, &x))
                {
                    si (chorda_aequalis(x.verbum, verbum))
                    {
                        (vacuum)paginam_ponere(ramus, motus, sa,
                            nomen_paginae, l, x.initium);
                        redde;
                    }
                    a = x.finis;
                }
            }
        }
    }
    nuntium_ponere(ramus, chorda_concatenare(chorda_concatenare(
        chorda_ex_literis("#", p), verbum, p), chorda_ex_literis(
        numerus_est(verbum) ? ": nulla pagina" : ": nulla alia pagina",
        p), p));
}

b32
scriba_iussum_ad_punctum (
            ScribaActiones* sa,
        constans Componens* pagina,
                   Punctum  schirmi)
{
    Iussum iussum;
       s32 linea;
       s32 columna;

    si (   !sa || !pagina || !sa->doc
        || !cellulam_ictam(sa, pagina, schirmi, &linea, &columna))
    {
        redde FALSUM;
    }
    /* S3d: nexus quoque uno ictu */
    redde (   sa->iussa
           && iussum_ad_locum(&sa->laboris, linea, columna,
                  iussum_registrum_notum, sa->iussa, sa->doc->piscina,
                  &iussum))
        || (   sa->liber
            && iussum_nexus_ad_locum(&sa->laboris, linea, columna,
                   sa->doc->piscina, &iussum));
}

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
        InsulaRamus  ramus;
     ScribaActiones* sa;
       VimClipboard  capsa;
          VimStatus  st;
                s32  clavis;
                i32  i;
                 i8  o;
                s32  linea;
                s32  columna;
             Iussum  iussum;
            Punctum  punctum;

    (vacuum)destinatio;
    sa = (ScribaActiones*)ctx;
    si (!repo || !motus || !ev || !sa)
    {
        redde FALSUM;
    }
    /* R4: status scribae per ramum (sine eo radix repositorii) */
    ramus = sa->ramus.repo ? sa->ramus : insula_ramus_radix(repo);
    /* S3b-2: clavis aut ictus nuntium tollit (ante quidquam aliud:
     * iussum ictum nuntium novum ponere potest) */
    si (   ev->genus == EVENTUS_CLAVIS_DEPRESSUS
        || ev->genus == EVENTUS_MUS_DEPRESSUS
        || (   ev->genus == EVENTUS_TEXTUS
            && ev->datum.textus.genus == EVENTUS_TEXTUS_COMMISSUM))
    {
        nuntium_tollere(&ramus);
    }
    si (ev->genus == EVENTUS_MUS_DEPRESSUS)
    {
        si (!nodus || ev->datum.mus.botton != MUS_SINISTER)
        {
            redde FALSUM;
        }
        punctum.x = ev->datum.mus.x;
        punctum.y = ev->datum.mus.y;
        si (!cellulam_ictam(sa, nodus, punctum, &linea, &columna))
        {
            redde FALSUM;
        }
        si (   sa->iussa
            && iussum_ad_locum(&sa->laboris, linea, columna,
                   iussum_registrum_notum, sa->iussa, sa->doc->piscina,
                   &iussum))
        {
            iussum_exsequi(&ramus, motus, sa, &iussum, ev->tempus);
            redde VERUM;
        }
        /* S3d: nexus (sine libro nulli) */
        si (   sa->liber
            && iussum_nexus_ad_locum(&sa->laboris, linea, columna,
                   sa->doc->piscina, &iussum))
        {
            nexum_sequi(&ramus, motus, sa, iussum.verbum,
                (ev->datum.mus.modificantes & MOD_SHIFT) != ZEPHYRUM);
            redde VERUM;
        }
        cursorem_ponere(&ramus, motus, sa, linea, columna);
        redde VERUM;
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
            clavem_tractare(&ramus, motus, sa, clavis, ev->tempus);
        }
        redde VERUM;
    }
    si (ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    /* S2b: Ctrl+Shift+Sinister/Dexter - pagina prior/proxima (sine
     * libro: non tractatur) */
    si (   (ev->datum.clavis.modificantes & MOD_IMPERIUM)
        && (ev->datum.clavis.modificantes & MOD_SHIFT)
        && (   ev->datum.clavis.clavis == CLAVIS_DEXTER
            || ev->datum.clavis.clavis == CLAVIS_SINISTER))
    {
        si (!sa->liber)
        {
            redde FALSUM;
        }
        paginam_mutare(&ramus, motus, sa,
            ev->datum.clavis.clavis == CLAVIS_DEXTER ? I : -I);
        redde VERUM;
    }
    /* Ctrl-[ = Esc (ut in terminali; Franus): runa '[' aut character
     * productus ESC (dispositiones ubi '[' alibi iacet) */
    si (   (ev->datum.clavis.modificantes & MOD_IMPERIUM)
        && (   ev->datum.clavis.runa == '['
            || ev->datum.clavis.producta == XXVII))
    {
        clavem_tractare(&ramus, motus, sa, VIM_CLAVIS_ESCAPE,
            ev->tempus);
        redde VERUM;
    }
    /* Ctrl-R in modo normali: reficere */
    si (   (ev->datum.clavis.modificantes & MOD_IMPERIUM)
        && ev->datum.clavis.runa == 'r')
    {
        si (   !attributum_est(&ramus, "modus", "normalis")
            && insula_ramus_attributum(&ramus, INSULA_EPHEMERA,
            "modus"))
        {
            redde FALSUM;
        }
        st = status_legere(&ramus, sa, &capsa);
        historiam_movere(&ramus, motus, sa, st, &capsa, FALSUM);
        redde VERUM;
    }
    clavis = clavis_nominata(ev);
    si (clavis == ZEPHYRUM)
    {
        redde FALSUM;   /* imprimibilis: per TEXTUM venit */
    }
    /* Tab in modo normali: focus proximus (dispensator) */
    si (   clavis == VIM_CLAVIS_TAB
        && !attributum_est(&ramus, "modus", "inserere"))
    {
        redde FALSUM;
    }
    clavem_tractare(&ramus, motus, sa, clavis, ev->tempus);
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
