#include "capitula.h"
#include "numerus_romanus.h"

/* ====================================================================
 * CAPITULA - implementatio. Vide capitula.h de rationibus.
 * ==================================================================== */

interior b32
_album (i8 c)
{
    redde (c == ' ' || c == '\t') ? VERUM : FALSUM;
}

interior b32
_cifra (i8 c)
{
    redde (c >= '0' && c <= '9') ? VERUM : FALSUM;
}

/* --------------------------------------------------------------------
 * IUDICIUM INSCRIPTIONIS
 * -------------------------------------------------------------------- */

/* Regula lata (post separatorem): cifram continet AUT numerus Romanus
 * integer est. "Capitulum 3" transit; "The Long Road" non. */
interior b32
_inscriptio_valet (chorda s)
{
    i32 i;
    i32 ignotum;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        si (_cifra(s.datum[i])) redde VERUM;
    }
    redde numerus_romanus_legere(s, &ignotum);
}

/* Regula angusta (vocabulum princeps): omnia segmenta punctis divisa
 * aut cifrae solae aut numeri Romani. Ita "1", "1.2.3", "XIV", "I.4"
 * transeunt; "Prooemium" et "Chapter" non. */
interior b32
_signum_valet (chorda s)
{
    i32 initium = ZEPHYRUM;
    i32 i       = ZEPHYRUM;
    i32 segmenta = ZEPHYRUM;

    dum (i <= s.mensura)
    {
        si (i == s.mensura || s.datum[i] == '.')
        {
            chorda seg = chorda_sectio(s, initium, i);
            i32    k;
            b32    cifrae = VERUM;
            i32    ignotum;

            si (seg.mensura == ZEPHYRUM) redde FALSUM;
            per (k = ZEPHYRUM; k < seg.mensura; k++)
            {
                si (!_cifra(seg.datum[k])) { cifrae = FALSUM; frange; }
            }
            si (!cifrae && !numerus_romanus_legere(seg, &ignotum))
            {
                redde FALSUM;
            }
            segmenta++;
            initium = i + I;
        }
        i++;
    }
    redde (segmenta > ZEPHYRUM) ? VERUM : FALSUM;
}

/* --------------------------------------------------------------------
 * SEPARATOR - '-' ':' aut lineola UTF-8 (en E2 80 93, em E2 80 94)
 * -------------------------------------------------------------------- */

interior i32
_separator_mensura (chorda s, i32 i)
{
    si (s.datum[i] == '-' || s.datum[i] == ':')
    {
        redde I;
    }
    si (i + II < s.mensura
        && s.datum[i]      == (i8)0xE2
        && s.datum[i + I]  == (i8)0x80
        && (s.datum[i + II] == (i8)0x93 || s.datum[i + II] == (i8)0x94))
    {
        redde III;
    }
    redde ZEPHYRUM;
}

/* --------------------------------------------------------------------
 * SCISSIO LINEAE
 *
 * SEPARATOR PRIMUS SOLUS CANDIDAT. Si pars ante eum inscriptio esse
 * non potest, nulla est - separatorem proximum quaerere titulos cum
 * subtitulis ("The Long Road - and What Came After - Part Two") in
 * medio scinderet, quod damnum tacitum est.
 * -------------------------------------------------------------------- */

interior vacuum
_scindere (chorda linea, chorda* inscriptio, chorda* titulus)
{
    i32    i = ZEPHYRUM;
    i32    fin;
    chorda vacua;

    vacua.mensura = ZEPHYRUM;
    vacua.datum   = NIHIL;

    /* forma I: <inscriptio> <separator> <titulus> */
    dum (i < linea.mensura)
    {
        i32 ms = _separator_mensura(linea, i);

        si (ms > ZEPHYRUM && i > ZEPHYRUM && _album(linea.datum[i - I])
            && i + ms < linea.mensura && _album(linea.datum[i + ms]))
        {
            chorda ante = chorda_praecidere(
                chorda_sectio(linea, ZEPHYRUM, i));
            chorda post = chorda_praecidere(
                chorda_sectio(linea, i + ms, linea.mensura));

            si (ante.mensura > ZEPHYRUM && post.mensura > ZEPHYRUM
                && ante.mensura <= CAPITULA_INSCRIPTIO_MAXIMA
                && _inscriptio_valet(ante))
            {
                *inscriptio = ante;
                *titulus    = post;
                redde;
            }
            frange;
        }
        i++;
    }

    /* forma II: <signum><punctum?> <titulus> */
    fin = ZEPHYRUM;
    dum (fin < linea.mensura && !_album(linea.datum[fin]))
    {
        fin++;
    }
    si (fin > ZEPHYRUM && fin < linea.mensura)
    {
        chorda signum   = chorda_sectio(linea, ZEPHYRUM, fin);
        chorda reliquum = chorda_praecidere(
            chorda_sectio(linea, fin, linea.mensura));

        /* punctum aut uncinum finale detrahere: ancora '1' est,
         * non '1.' - inscriptio id quod auctor NUMERAVIT est */
        dum (signum.mensura > ZEPHYRUM
            && (signum.datum[signum.mensura - I] == '.'
                || signum.datum[signum.mensura - I] == ')'))
        {
            signum.mensura--;
        }

        si (reliquum.mensura > ZEPHYRUM
            && signum.mensura > ZEPHYRUM
            && signum.mensura <= CAPITULA_INSCRIPTIO_MAXIMA
            && _signum_valet(signum))
        {
            *inscriptio = signum;
            *titulus    = reliquum;
            redde;
        }
    }

    *inscriptio = vacua;
    *titulus    = chorda_praecidere(linea);
}

/* --------------------------------------------------------------------
 * INDENTATIO
 * -------------------------------------------------------------------- */

interior i32
_indentationem_metiri (chorda linea, i32* offset)
{
    i32 lat = ZEPHYRUM;
    i32 i   = ZEPHYRUM;

    dum (i < linea.mensura)
    {
        si (linea.datum[i] == ' ')
        {
            lat++;
        }
        alioquin si (linea.datum[i] == '\t')
        {
            lat = (lat / CAPITULA_TABULI_LATITUDO + I)
                * CAPITULA_TABULI_LATITUDO;
        }
        alioquin
        {
            frange;
        }
        i++;
    }
    *offset = i;
    redde lat;
}

/* --------------------------------------------------------------------
 * LECTIO
 * -------------------------------------------------------------------- */

IndexLibri
capitula_legere (chorda fons, Piscina* piscina)
{
    IndexLibri fructus;
    i32        acervus[CAPITULA_PROFUNDITAS_MAXIMA];
    i32        alta = ZEPHYRUM;
    i32        i    = ZEPHYRUM;
    i32        numerus_lineae = ZEPHYRUM;
    i32        ordo = ZEPHYRUM;

    fructus.capitula       = xar_creare(piscina, (i32)magnitudo(Capitulum));
    fructus.gradus_maximus = ZEPHYRUM;

    si (fructus.capitula == NIHIL)
    {
        redde fructus;
    }

    dum (i < fons.mensura)
    {
        i32        initium = i;
        i32        finis;
        i32        offset;
        i32        lat;
        i32        gradus;
        chorda     linea;
        chorda     inscriptio;
        chorda     titulus;
        Capitulum* cap;

        dum (i < fons.mensura && fons.datum[i] != '\n'
            && fons.datum[i] != '\r')
        {
            i++;
        }
        finis = i;
        si (i < fons.mensura && fons.datum[i] == '\r') i++;
        si (i < fons.mensura && fons.datum[i] == '\n') i++;

        numerus_lineae++;
        linea = chorda_sectio(fons, initium, finis);
        lat   = _indentationem_metiri(linea, &offset);
        linea = chorda_praecidere_dextram(
            chorda_sectio(linea, offset, linea.mensura));

        si (linea.mensura == ZEPHYRUM)
        {
            perge;   /* linea vacua: nullum capitulum, nullus gradus */
        }

        /* acervus indentationum -> gradus */
        dum (alta > ZEPHYRUM && acervus[alta - I] > lat)
        {
            alta--;
        }
        si (alta == ZEPHYRUM || acervus[alta - I] < lat)
        {
            si (alta < CAPITULA_PROFUNDITAS_MAXIMA)
            {
                acervus[alta] = lat;
                alta++;
            }
        }
        gradus = (alta > ZEPHYRUM) ? alta - I : ZEPHYRUM;

        _scindere(linea, &inscriptio, &titulus);

        cap = (Capitulum*)xar_addere(fructus.capitula);
        si (cap == NIHIL)
        {
            frange;
        }
        ordo++;
        cap->inscriptio = chorda_transcribere(inscriptio, piscina);
        cap->titulus    = chorda_transcribere(titulus, piscina);
        cap->gradus     = gradus;
        cap->ordo       = ordo;
        cap->linea      = numerus_lineae;

        si (gradus > fructus.gradus_maximus)
        {
            fructus.gradus_maximus = gradus;
        }
    }

    redde fructus;
}
