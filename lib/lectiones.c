/* lectiones.c - liber lectionum (fabrica plan 2 T1). Vide lectiones.h.
 *
 * Variabilis ambitus omni vocatione legitur (vilis): probationes eam
 * ponunt et tollunt, et descriptor apertus viae NOVAE non servit.
 * Descriptor per processum servatur dum via eadem manet; filius post
 * fork descriptorem patris heredat - O_APPEND lineas integras servat.
 */
#include "postulata_posix.h"
#include "lectiones.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>


hic_manens  integer  _descriptor = -I;
hic_manens character _via_aperta[IV * MXXIV];

/* MEMORIA LINEARUM (budget B2 I2): fabrica librum ut COPIAM legit
 * (ordine primo); processus eandem lineam saepe iterum scribebat
 * (aedilis per fontem: 1348 lineae / 724 unicae; capita systematis
 * contra omnes radices). Tabula directe mappata (FNV-1a lineae ->
 * locus): linea iam scripta in loco suo -> write() omittitur.
 * Collisio solum rescribit, numquam lineam novam amittit. Vacatur cum
 * descriptor clauditur aut reaperitur (liber absens aut alius). Post
 * fork filius tabulam heredat - recte: quod pater notavit in
 * descriptorem communem O_APPEND scriptum est. libc sola (lectiones
 * sine dependentiis domus). LECTIONES_MEMORIA_LOCI (potentia binaria,
 * ordinarie 16384): probationes collisiones cogunt. */
#define LECTIONES_LOCI_ORDINARII 16384UL

hic_manens      character** _memoria         = NIHIL;
hic_manens insignatus longus _memoria_loci  = 0UL;

interior vacuum
_memoriam_vacare (vacuum)
{
    insignatus longus k;

    si (_memoria == NIHIL)
    {
        redde;
    }
    per (k = 0UL; k < _memoria_loci; k++)
    {
        free(_memoria[k]);
    }
    /* tabula tota: proxima allocatio LECTIONES_MEMORIA_LOCI iterum
     * legit (probationes in filio collisiones cogunt) */
    free(_memoria);
    _memoria       = NIHIL;
    _memoria_loci  = 0UL;
}

/* VERUM si linea (n octeti) iam scripta est; aliter notatur (VERUM
 * numquam pro linea nova) */
interior b32
_linea_iam_scripta (
     constans character* linea,
         memoriae_index  n)
{
    insignatus longus  h;
       memoriae_index  k;
            character* copia;

    si (_memoria == NIHIL)
    {
        constans character* loci = getenv("LECTIONES_MEMORIA_LOCI");

        _memoria_loci = (loci != NIHIL && loci[0] != '\0')
            ? strtoul(loci, NIHIL, 10) : LECTIONES_LOCI_ORDINARII;
        si (   _memoria_loci                           == 0UL
            || (_memoria_loci & (_memoria_loci - 1UL)) != 0UL)
        {
            _memoria_loci = LECTIONES_LOCI_ORDINARII;
        }
        _memoria = (character**)calloc((memoriae_index)_memoria_loci,
            magnitudo(character*));
        si (_memoria == NIHIL)
        {
            redde FALSUM;
        }
    }
    h = 2166136261UL;
    per (k = 0; k < n; k++)
    {
        h ^= (insignatus longus)(insignatus character)linea[k];
        h = (h * 16777619UL) & 0xFFFFFFFFUL;
    }
    h &= _memoria_loci - 1UL;
    si (   _memoria[h] != NIHIL && strlen(_memoria[h]) == n
        && memcmp(_memoria[h], linea, n) == 0)
    {
        redde VERUM;
    }
    copia = (character*)malloc(n + 1);
    si (copia != NIHIL)
    {
        memcpy(copia, linea, n);
        copia[n] = '\0';
        free(_memoria[h]);
        _memoria[h] = copia;
    }
    redde FALSUM;
}

interior character
_littera (
    LectioGenus genus)
{
    commutatio (genus)
    {
        casus LECTIO_LEGIT:       redde 'L';
        casus LECTIO_ABSENS:      redde 'A';
        casus LECTIO_EXSTAT:      redde 'X';
        casus LECTIO_ENUMERAVIT:  redde 'D';
        casus LECTIO_SCRIPSIT:    redde 'S';
        casus LECTIO_AMBITUS:     redde 'E';
        casus LECTIO_EXSECUTIO:   redde 'P';
    }
    redde '?';
}

interior vacuum
_lectionem_notare (
                 LectioGenus  genus,
          constans character* via)
{
    constans character* liber;
             character  linea[IV * MXXIV + IV];
        memoriae_index  mensura_viae;

    liber = getenv("FABRICA_LECTIONES");
    si (liber == NIHIL || liber[0] == '\0')
    {
        /* absens = CLAUSUM: descriptor servatus plagulam interea
         * deletam (eadem via, inodus alius) scriberet - probatio III
         * lineas omnes ita amisit */
        si (_descriptor >= 0)
        {
            (vacuum)close(_descriptor);
            _descriptor = -I;
            _memoriam_vacare();
        }
        redde;
    }
    si (via == NIHIL)
    {
        redde;
    }
    si (_descriptor < 0 || strcmp(liber, _via_aperta) != 0)
    {
        si (_descriptor >= 0)
        {
            (vacuum)close(_descriptor);
            _descriptor = -I;
        }
        _memoriam_vacare();
        si (strlen(liber) >= magnitudo(_via_aperta))
        {
            redde;
        }
        /* CLOEXEC: programma exsecutum (alienum quoque) descriptorem
         * libri non hereditat; filii sine exec (fork) eum
         * communicant */
        /* <tolera codex="CANALIS_PRAETERITUS" (>canalis ipse
         * liber lectionum ipse (se ipsum non notat) */
        _descriptor = open(liber, O_WRONLY | O_APPEND | O_CREAT
            | O_CLOEXEC, 0644);
        si (_descriptor < 0)
        {
            redde;
        }
        strcpy(_via_aperta, liber);
    }
    mensura_viae = strlen(via);
    si (mensura_viae + IV > magnitudo(linea))
    {
        redde;   /* linea numquam scinditur */
    }
    linea[0] = _littera(genus);
    linea[I] = '\t';
    memcpy(linea + II, via, mensura_viae);
    linea[II + mensura_viae] = '\n';
    si (_linea_iam_scripta(linea, mensura_viae + III))
    {
        redde;
    }
    {
        /* optima conatio: defectus hic tractari nequit. Valor ad
         * variabilem, non (vacuum) solum: glibc cum _FORTIFY_SOURCE
         * write() warn_unused_result notat, quod gcc mittendo in
         * vacuum NON tacet (hospitium v3 T9) */
        ssize_t scripta = write(_descriptor, linea, mensura_viae + III);

        (vacuum)scripta;
    }
}

/* errno SERVATUR: notatio vocantem numquam mutat - vocatores errno post
 * vocationem crudam legunt (machinula officinae errno hospiti reddit,
 * filum _filum_error_ponere); liber apertus aut appendens errno
 * mutare posset (2026-10-27) */
vacuum
lectiones_notare (
                 LectioGenus  genus,
          constans character* via)
{
    integer servatum = errno;

    _lectionem_notare(genus, via);
    errno = servatum;
}

constans character*
lectiones_ambitus (
         constans character* titulus)
{
    constans character* valor;
             character  linea[IV * MXXIV];

    si (titulus == NIHIL)
    {
        redde NIHIL;
    }
    valor = getenv(titulus);
    si (strlen(titulus) + (valor != NIHIL ? strlen(valor) : 0) + II
            >= magnitudo(linea))
    {
        redde valor;   /* nimis longum: non notatur, ut via longa */
    }
    strcpy(linea, titulus);
    si (valor != NIHIL)
    {
        strcat(linea, "\t");
        strcat(linea, valor);
    }
    lectiones_notare(LECTIO_AMBITUS, linea);
    redde valor;
}

/* via machinae (/dev/): non ingressus */
interior b32
_machina_est (
    constans character* via)
{
    redde via != NIHIL && strncmp(via, "/dev/", V) == ZEPHYRUM;
}

FILE*
lectiones_fopen (
    constans character* via,
    constans character* modus)
{
     /* <tolera codex="CANALIS_PRAETERITUS" (>canalis ipse
      * lectiones_fopen notat SCRIPSIT */
     FILE* f = fopen(via, modus);
      b32  lectio;

    si (via == NIHIL || modus == NIHIL || _machina_est(via))
    {
        redde f;
    }
    lectio = modus[ZEPHYRUM] == 'r' && strchr(modus, '+') == NIHIL;
    si (lectio)
    {
        lectiones_notare(f != NIHIL ? LECTIO_LEGIT : LECTIO_ABSENS,
            via);
    }
    alioquin si (f != NIHIL)
    {
        lectiones_notare(LECTIO_SCRIPSIT, via);
    }
    redde f;
}

integer
lectiones_stat (
    constans character* via,
       structura stat* status)
{
    /* <tolera codex="CANALIS_PRAETERITUS" (>canalis ipse
     * lectiones_stat notat EXSTAT/ABSENS */
    integer r = stat(via, status);

    si (via != NIHIL && !_machina_est(via))
    {
        lectiones_notare(r == ZEPHYRUM ? LECTIO_EXSTAT : LECTIO_ABSENS,
            via);
    }
    redde r;
}
