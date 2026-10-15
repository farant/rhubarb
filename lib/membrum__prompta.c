/* membrum__prompta.c - linea promptae (include/membrum__prompta.h).
 * Ex calendario translata (kalendarium A3b, V2a): vim modo inserendi in
 * tabula characterum unius lineae; textus in insula effugitus. */
#include "membrum__prompta.h"
#include "scriba_documentum.h"   /* scriba_effugere / scriba_solvere */
#include "tabula_characterum.h"
#include "vim.h"
#include "figura.h"
#include "fons.h"
#include "thema.h"

#include <stdio.h>
#include <string.h>

nomen structura {
    constans character* titulus;
    constans character* valor;
} ParCampi;

nomen structura {
    ParCampi* pares;
         i32  numerus;
} GrexCamporum;

interior vacuum
campos_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
     GrexCamporum* grex;
         ParCampi* par;
              i32  i;

    grex = (GrexCamporum*)ctx;
    per (i = ZEPHYRUM; i < grex->numerus; i++)
    {
        par = grex->pares + i;
        si (!par->valor || !par->valor[ZEPHYRUM])
        {
            /* "" internari non potest: vacuum = tollere */
            (vacuum)insula_attributum_tollere(radix, par->titulus);
            perge;
        }
        insula_attributum_ponere(radix, p, in, par->titulus,
            par->valor);
    }
}

/* status scribere (scriptura una): genus ("nulla" claudit), textus
 * crudus (effugitur), cursor; scriptor hospitis, prior restituitur */
interior b32
promptam_scribere (
            InsulaRamus* ramus,
     constans character* scriptor,
     constans character* genus,
     constans character* textus,
                    i32  longitudo,
                    i32  cursor,
                Piscina* p)
{
        ParCampi pares[III];
    GrexCamporum grex;
          chorda prior;
          chorda effugitus;
       character numerus[XVI];
             b32 ok;

    effugitus = scriba_effugere(textus, longitudo, p);
    sprintf(numerus, "%d", (integer)cursor);
    pares[ZEPHYRUM].titulus  = "prompta";
    pares[ZEPHYRUM].valor    = genus;
    pares[I].titulus         = "prompta_textus";
    pares[I].valor           = longitudo > ZEPHYRUM
                             ? chorda_ut_cstr(effugitus, p) : "";
    pares[II].titulus  = "prompta_cursor";
    pares[II].valor    = longitudo > ZEPHYRUM ? numerus : "";
    grex.pares         = pares;
    grex.numerus       = III;
    prior              = ramus->repo->scriptor;
    insula_scriptorem_ponere(ramus->repo, chorda_ex_literis(scriptor,
        p));
    ok = mutare_ramum(ramus, campos_ponere, &grex);
    insula_scriptorem_ponere(ramus->repo, prior);
    redde ok;
}

b32
prompta_aperta (
    constans InsulaRamus* ramus,
                  chorda* genus)
{
    chorda* a;

    a = ramus ? insula_ramus_attributum(ramus, "prompta") : NIHIL;
    si (!a || chorda_aequalis_literis(*a, "nulla"))
    {
        redde FALSUM;
    }
    si (genus)
    {
        *genus = *a;
    }
    redde VERUM;
}

i32
prompta_textus (
    constans InsulaRamus* ramus,
               character* exitus)
{
    chorda* a;
       s32  n;

    a = ramus ? insula_ramus_attributum(ramus,
        "prompta_textus") : NIHIL;
    n = a ? scriba_solvere(*a, exitus, PROMPTA_MAXIMA) : ZEPHYRUM;
    redde n > ZEPHYRUM ? (i32)n : ZEPHYRUM;
}

b32
prompta_aperire (
            InsulaRamus* ramus,
     constans character* scriptor,
     constans character* genus,
                 chorda  textus,
                Piscina* piscina)
{
    si (!ramus || !scriptor || !genus || !piscina)
    {
        redde FALSUM;
    }
    si (textus.mensura >= PROMPTA_MAXIMA)
    {
        textus.mensura = PROMPTA_MAXIMA - I;
    }
    redde promptam_scribere(ramus, scriptor, genus, textus.mensura
        > ZEPHYRUM ? (constans character*)textus.datum : "",
        textus.mensura, textus.mensura, piscina);
}

b32
prompta_claudere (
            InsulaRamus* ramus,
     constans character* scriptor,
                Piscina* piscina)
{
    si (!ramus || !scriptor || !piscina)
    {
        redde FALSUM;
    }
    redde promptam_scribere(ramus, scriptor, "nulla", "", ZEPHYRUM,
        ZEPHYRUM, piscina);
}

/* clavis eventus -> clavis vim (nominatae et imprimibiles); 0 =
 * nulla */
interior s32
clavis_vim (
    constans Eventus* ev)
{
    commutatio (ev->datum.clavis.clavis)
    {
        casus CLAVIS_RETRORSUM:  redde VIM_CLAVIS_BACKSPACE;
        casus CLAVIS_DELERE:     redde VIM_CLAVIS_DELETE;
        casus CLAVIS_SINISTER:   redde VIM_CLAVIS_SINISTRAM;
        casus CLAVIS_DEXTER:     redde VIM_CLAVIS_DEXTRAM;
        casus CLAVIS_DOMUS:      redde VIM_CLAVIS_HOME;
        casus CLAVIS_FINIS:      redde VIM_CLAVIS_END;
        ordinarius:              frange;
    }
    si (   ev->datum.clavis.producta >= ' '
        && ev->datum.clavis.producta < CXXVII)
    {
        redde ev->datum.clavis.producta;
    }
    redde ZEPHYRUM;
}

PromptaExitus
prompta_clavem_agere (
            InsulaRamus* ramus,
     constans character* scriptor,
       constans Eventus* ev,
                 chorda* textus,
                Piscina* piscina)
{
    TabulaCharacterum  t;
            VimStatus  st;
            character  crudus[PROMPTA_MAXIMA];
               chorda  genus;
               chorda* a;
                  s32  cursor;
                  s32  clavis;
                  i32  n;
                  i32  i;

    si (   !ramus || !ev || ev->genus != EVENTUS_CLAVIS_DEPRESSUS
        || !prompta_aperta(ramus, &genus))
    {
        redde PROMPTA_NIHIL;
    }
    n = prompta_textus(ramus, crudus);
    si (ev->datum.clavis.clavis == CLAVIS_EFFUGIUM)
    {
        (vacuum)prompta_claudere(ramus, scriptor, piscina);
        redde PROMPTA_EFFUGIUM;
    }
    si (ev->datum.clavis.clavis == CLAVIS_REDITUS)
    {
        chorda c;

        dum (n > ZEPHYRUM && crudus[n - I] == ' ')
        {
            n--;
        }
        c.datum    = (i8*)crudus;
        c.mensura  = n;
        si (textus)
        {
            *textus = chorda_transcribere(c, piscina);
        }
        redde PROMPTA_REDITUS;
    }
    clavis = clavis_vim(ev);
    si (   clavis == ZEPHYRUM
        || (clavis > ZEPHYRUM && n >= PROMPTA_MAXIMA - I))
    {
        /* consumpta: prompta claves omnes capit */
        redde PROMPTA_EDITA;
    }
    a = insula_ramus_attributum(ramus, "prompta_cursor");
    si (   !a || !chorda_ut_s32(*a, &cursor) || cursor < ZEPHYRUM
        || cursor > (s32)n)
    {
        cursor = (s32)n;
    }
    /* Home / End: cursor solus (vim modo inserendi eas non tractat -
     * in calendario olim nihil faciebant) */
    si (clavis == VIM_CLAVIS_HOME || clavis == VIM_CLAVIS_END)
    {
        (vacuum)promptam_scribere(ramus, scriptor, chorda_ut_cstr(genus,
            piscina), crudus, n, clavis == VIM_CLAVIS_HOME ? ZEPHYRUM
            : (i32)n, piscina);
        redde PROMPTA_EDITA;
    }
    tabula_initiare(&t, piscina, PROMPTA_MAXIMA, I);
    memcpy(t.cellulae, crudus, (size_t)n);
    st = vim_initiare(&t);
    st = vim_ponere_modum(st, MODO_VIM_INSERERE);
    st = vim_ponere_cursor(st, ZEPHYRUM, (i32)cursor);
    st = vim_tractare_clavem(st, clavis);
    /* longitudo: vim cellulas vacuas spatiis implet - ultima cellula
     * non alba, aut cursor (spatium modo typatum manet donec littera
     * sequitur; ad Enter abscinditur) */
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < PROMPTA_MAXIMA; i++)
    {
        si (t.cellulae[i] != '\0' && t.cellulae[i] != ' ')
        {
            n = i + I;
        }
    }
    si ((s32)n < (s32)st.cursor_columna)
    {
        n = st.cursor_columna;
    }
    (vacuum)promptam_scribere(ramus, scriptor, chorda_ut_cstr(genus,
        piscina), t.cellulae, n, st.cursor_columna, piscina);
    redde PROMPTA_EDITA;
}

vacuum
prompta_pingere (
                 Mandata* md,
                   Fines  f,
      constans character* titulus_generis,
    constans InsulaRamus* ramus)
{
     Fines  campus;
     Fines  cursor_fines;
    chorda  textus;
    chorda* a;
 character  crudus[PROMPTA_MAXIMA];
       s32  cursor;
       s32  littera;
       s32  maximum;
       s32  y;
       i32  n;

    si (!md || !ramus || !prompta_aperta(ramus, NIHIL))
    {
        redde;
    }
    littera  = fons_latitudo(FONS_PARVUS);
    y        = f.y + (f.altitudo - fons_altitudo(FONS_PARVUS)) / II;
    si (!titulus_generis)
    {
        titulus_generis = "";
    }
    /* titulus generis, deinde campus (superficies, margo) cum textu et
     * cursore tenui */
    mandata_textus(md, f.x + IV, y,
        chorda_transcribere(chorda_ex_literis(
        titulus_generis, md->piscina), md->piscina), FONS_PARVUS,
        figura_color_thematis(COLOR_TEXT_DIM));
    campus.x         = f.x + IV + ((s32)strlen(titulus_generis) + I)
                     * littera;
    campus.y         = f.y + I;
    campus.latitudo  = f.x + f.latitudo - campus.x - IV;
    campus.altitudo  = f.altitudo - II;
    figura_campum_pingere(md, campus);
    n               = prompta_textus(ramus, crudus);
    textus.datum    = (i8*)crudus;
    textus.mensura  = n;
    maximum         = (campus.latitudo - VIII) / littera;
    si (maximum < ZEPHYRUM)
    {
        maximum = ZEPHYRUM;
    }
    si ((s32)textus.mensura > maximum)
    {
        textus.mensura = (i32)maximum;
    }
    mandata_textus(md, campus.x + IV, y, chorda_transcribere(textus,
        md->piscina), FONS_PARVUS, figura_color_thematis(COLOR_TEXT));
    a = insula_ramus_attributum(ramus, "prompta_cursor");
    si (!a || !chorda_ut_s32(*a, &cursor) || cursor < ZEPHYRUM)
    {
        cursor = (s32)n;
    }
    cursor_fines.x         = campus.x + IV + cursor * littera - I;
    cursor_fines.y         = campus.y + II;
    cursor_fines.latitudo  = II;
    cursor_fines.altitudo  = campus.altitudo - IV;
    mandata_rectangulum(md, cursor_fines, figura_color_thematis(
        COLOR_CURSOR), VERUM);
}
