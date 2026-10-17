/* multiplexor.c - tmux per lineam imperii (include/multiplexor.h) */
#include "postulata_posix.h"
#include "multiplexor.h"
#include "filum.h"
#include "processus.h"

#include <stdlib.h>
#include <string.h>

#define MULTIPLEXOR_CAMPI_PANELLI  XI
#define MULTIPLEXOR_CAMPI_CLIENTIS II
#define MULTIPLEXOR_ARGUMENTA      XVI

hic_manens constans character FORMA_PANELLORUM[] =
    "#{pane_id}\t#{session_name}\t#{window_index}\t#{window_name}\t"
    "#{pane_index}\t#{pane_pid}\t#{pane_tty}\t#{pane_current_command}\t"
    "#{pane_current_path}\t#{pane_active}\t#{window_active}";

hic_manens constans character FORMA_CLIENTIUM[] =
    "#{client_tty}\t#{session_name}";

/* binarium: datum, aut PATH, aut loci noti (vicus ex Doco PATH brevem
 * habet); NIHIL si nusquam */
interior constans character*
binarium_quaerere (
    constans Multiplexor* m,
                 Piscina* piscina)
{
    constans character* loci[] = { "/opt/homebrew/bin/tmux",
        "/usr/local/bin/tmux", "/usr/bin/tmux", NIHIL };
    constans character* semita;
    constans character* initium;
    constans character* finis;
             character* via;
                   i32  n;
                   i32  i;

    si (m && m->binarium)
    {
        redde m->binarium;   /* datum creditur (absens: exsecutio
                              * deficit) */
    }
    semita   = getenv("PATH");
    initium  = semita;
    dum (initium && *initium)
    {
        finis  = strchr(initium, ':');
        n      = finis ? (i32)(finis - initium) : (i32)strlen(initium);
        si (n > ZEPHYRUM)
        {
            via = (character*)piscina_allocare(piscina,
                (memoriae_index)n + VI + I);
            si (!via)
            {
                redde NIHIL;
            }
            memcpy(via, initium, (size_t)n);
            strcpy(via + n, "/tmux");
            si (filum_existit(via))
            {
                redde via;
            }
        }
        initium = finis ? finis + I : NIHIL;
    }
    per (i = ZEPHYRUM; loci[i]; i++)
    {
        si (filum_existit(loci[i]))
        {
            redde loci[i];
        }
    }
    redde NIHIL;
}

/* vector: binarium [-L s | -S via] et argumenta data (NIHIL finita);
 * NIHIL si tmux non invenitur */
interior constans character**
vectorem_struere (
    constans Multiplexor* m,
                Piscina*  piscina,
    constans character* constans* imperium)
{
     constans character** v;
     constans character*  binarium;
                    i32   n;
                    i32   i;

    binarium = binarium_quaerere(m, piscina);
    v        = (constans character**)piscina_allocare(piscina,
        magnitudo(constans character*) * MULTIPLEXOR_ARGUMENTA);
    si (!binarium || !v)
    {
        redde NIHIL;
    }
    n       = ZEPHYRUM;
    v[n++]  = binarium;
    /* -u: UTF-8 semper - sine LANG/LC_* (vicus ex Doco, portae) tmux
     * characteres regiminis in effusione -F in '_' mutat: tabulatio
     * separatrix periret */
    v[n++]  = "-u";
    si (m && m->socket)
    {
        v[n++] = strchr(m->socket, '/') ? "-S" : "-L";
        v[n++] = m->socket;
    }
    per (i = ZEPHYRUM; imperium[i]
        && n < MULTIPLEXOR_ARGUMENTA - I; i++)
    {
        v[n++] = imperium[i];
    }
    v[n] = NIHIL;
    redde v;
}

/* imperium currere: VERUM si exitus 0; effusio in *exitus (si datum) */
interior b32
currere (
    constans Multiplexor* m,
                Piscina*  piscina,
    constans character* constans* imperium,
                 chorda*  exitus)
{
    constans character** v;
     ProcessusResultus   r;

    v = vectorem_struere(m, piscina, imperium);
    si (!v)
    {
        redde FALSUM;
    }
    r = processus_exsequi(v, m
        && m->mora_ms > ZEPHYRUM ? (i32)m->mora_ms
        : (i32)M, piscina);
    si (!r.successus || r.codex_exitus != ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (exitus)
    {
        *exitus = r.effusio;
    }
    redde VERUM;
}

/* "=sessio": scopus exactus (tmux aliter praefixum congruit) */
interior constans character*
sessio_exacta (
      chorda  sessio,
     Piscina* piscina)
{
    character* s;

    s = (character*)piscina_allocare(piscina, (memoriae_index)
        sessio.mensura + II);
    si (!s)
    {
        redde NIHIL;
    }
    s[ZEPHYRUM] = '=';
    memcpy(s + I, sessio.datum, (size_t)sessio.mensura);
    s[sessio.mensura + I] = '\0';
    redde s;
}

interior b32
numerus (
    chorda  c,
       s32* n)
{
    redde chorda_ut_s32(c, n);
}

Xar*
multiplexor_panellos_legere (
      chorda  effusio,
     Piscina* piscina)
{
                      Xar* l;
    chorda_fissio_fructus  lineae;
    chorda_fissio_fructus  campi;
      MultiplexorPanellus  p;
                      i32  i;
                      s32  activus;
                      s32  fenestra_activa;

    l = xar_creare(piscina, (i32)magnitudo(MultiplexorPanellus));
    si (!l)
    {
        redde NIHIL;
    }
    lineae = chorda_fissio(effusio, '\n', piscina);
    per (i = ZEPHYRUM; i < lineae.numerus; i++)
    {
        campi = chorda_fissio(lineae.elementa[i], '\t', piscina);
        si (campi.numerus != MULTIPLEXOR_CAMPI_PANELLI)
        {
            perge;
        }
        memset(&p, ZEPHYRUM, magnitudo(p));
        p.id                = campi.elementa[ZEPHYRUM];
        p.sessio            = campi.elementa[I];
        p.fenestra_titulus  = campi.elementa[III];
        p.tty               = campi.elementa[VI];
        p.imperium          = campi.elementa[VII];
        p.via               = campi.elementa[VIII];
        si (   p.id.mensura == ZEPHYRUM
            || !numerus(campi.elementa[II], &p.fenestra)
            || !numerus(campi.elementa[IV], &p.index)
            || !numerus(campi.elementa[V], &p.pid)
            || !numerus(campi.elementa[IX], &activus)
            || !numerus(campi.elementa[X], &fenestra_activa))
        {
            perge;
        }
        p.activus = activus != ZEPHYRUM;
        p.fenestra_activa = fenestra_activa != ZEPHYRUM;
        *(MultiplexorPanellus*)xar_addere(l) = p;
    }
    redde l;
}

Xar*
multiplexor_clientes_legere (
      chorda  effusio,
     Piscina* piscina)
{
                      Xar* l;
    chorda_fissio_fructus  lineae;
    chorda_fissio_fructus  campi;
        MultiplexorCliens  k;
                      i32  i;

    l = xar_creare(piscina, (i32)magnitudo(MultiplexorCliens));
    si (!l)
    {
        redde NIHIL;
    }
    lineae = chorda_fissio(effusio, '\n', piscina);
    per (i = ZEPHYRUM; i < lineae.numerus; i++)
    {
        campi = chorda_fissio(lineae.elementa[i], '\t', piscina);
        si (   campi.numerus != MULTIPLEXOR_CAMPI_CLIENTIS
            || campi.elementa[ZEPHYRUM].mensura == ZEPHYRUM)
        {
            perge;
        }
        k.tty                               = campi.elementa[ZEPHYRUM];
        k.sessio                            = campi.elementa[I];
        *(MultiplexorCliens*)xar_addere(l)  = k;
    }
    redde l;
}

Xar*
multiplexor_panelli (
    constans Multiplexor* m,
                 Piscina* piscina)
{
    constans character* imperium[] = { "list-panes", "-a", "-F",
        FORMA_PANELLORUM, NIHIL };
    chorda effusio;

    si (!currere(m, piscina, imperium, &effusio))
    {
        redde NIHIL;
    }
    redde multiplexor_panellos_legere(effusio, piscina);
}

Xar*
multiplexor_clientes (
    constans Multiplexor* m,
                 Piscina* piscina)
{
    constans character* imperium[] = { "list-clients", "-F",
        FORMA_CLIENTIUM, NIHIL };
    chorda effusio;

    si (!currere(m, piscina, imperium, &effusio))
    {
        redde NIHIL;
    }
    redde multiplexor_clientes_legere(effusio, piscina);
}

constans MultiplexorPanellus*
multiplexor_panellum_invenire (
       Xar* panelli,
    chorda  id)
{
    constans MultiplexorPanellus* p;
                             i32  i;

    per (i = ZEPHYRUM; panelli && i < xar_numerus(panelli); i++)
    {
        p = (constans MultiplexorPanellus*)xar_obtinere(panelli, i);
        si (chorda_aequalis(p->id, id))
        {
            redde p;
        }
    }
    redde NIHIL;
}

b32
multiplexor_sessio_exsistit (
    constans Multiplexor* m,
                  chorda  sessio,
                 Piscina* piscina)
{
    constans character* imperium[IV];

    imperium[ZEPHYRUM]  = "has-session";
    imperium[I]         = "-t";
    imperium[II]        = sessio_exacta(sessio, piscina);
    imperium[III]       = NIHIL;
    redde imperium[II] && currere(m, piscina, imperium, NIHIL);
}

b32
multiplexor_panellum_eligere (
    constans Multiplexor* m,
                  chorda  id,
                 Piscina* piscina)
{
    constans character* imperium[IV];

    imperium[ZEPHYRUM]  = "select-window";
    imperium[I]         = "-t";
    imperium[II]        = chorda_ut_cstr(id, piscina);
    imperium[III]       = NIHIL;
    si (!imperium[II] || !currere(m, piscina, imperium, NIHIL))
    {
        redde FALSUM;
    }
    imperium[ZEPHYRUM] = "select-pane";
    redde currere(m, piscina, imperium, NIHIL);
}

b32
multiplexor_clientem_vertere (
    constans Multiplexor* m,
                  chorda  tty,
                  chorda  id,
                 Piscina* piscina)
{
    constans character* imperium[VI];

    imperium[ZEPHYRUM]  = "switch-client";
    imperium[I]         = "-c";
    imperium[II]        = chorda_ut_cstr(tty, piscina);
    imperium[III]       = "-t";
    imperium[IV]        = chorda_ut_cstr(id, piscina);
    imperium[V]         = NIHIL;
    redde imperium[II] && imperium[IV] && currere(m, piscina, imperium,
        NIHIL);
}

constans character**
multiplexor_argumenta_adhaerendi (
    constans Multiplexor* m,
                  chorda  sessio,
                 Piscina* piscina)
{
    constans character* imperium[IV];

    imperium[ZEPHYRUM]  = "attach";
    imperium[I]         = "-t";
    imperium[II]        = sessio_exacta(sessio, piscina);
    imperium[III]       = NIHIL;
    redde imperium[II] ? vectorem_struere(m, piscina, imperium) : NIHIL;
}
