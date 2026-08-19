#ifndef CAPITULA_H
#define CAPITULA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"


/* ====================================================================
 * CAPITULA - index libri ex textu adglutinato
 *
 * Lector notarum indicem capitulorum SEMEL scribit, more quo in libro
 * ipso impressus est:
 *
 *     Prooemium
 *     I - De methodo
 *         1. Quid quaeratur
 *         2. Quid non quaeratur
 *     II - De materia
 *     Appendix A - Tabulae
 *
 * et ex eo capitula NASCUNTUR - entia vera in conditorio, quibus
 * postea notae, tagi, nexus adhaerere possunt.
 *
 * FUNCTIO PURA: textus intrat, structura exit. Nihil scribitur, nihil
 * legitur. Scriptio consumptoris est (velamen fori), non huius.
 * ==================================================================== */

/* --------------------------------------------------------------------
 * I. HAEC LECTIO NUMQUAM RECUSAT - et cur id a sententiis differt
 *
 * `sententiae_legere` culpam STRUCTURAE reddere potest quia locus
 * duplex proiectionem FALSAM pareret: duae sententiae unam ancoram
 * vindicarent. Index capitulorum tale invariantum NON habet. Quaelibet
 * linea non-vacua capitulum est; nulla compositio linearum se ipsam
 * refellit. Ergo nulla CulpaStructurae hic est, consulto.
 *
 * Quod NON significat lectionem semper rectam esse - significat eam
 * semper ALIQUID reddere, quod consumptor ante scriptionem monstrare
 * DEBET. Praevisio porta est, non ornamentum: scriptio XL entium ex
 * lectione tacita error est qui XL emendationes poscit.
 * -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
 * II. GRADUS PER ACERVUM, non per numerum spatiorum
 *
 * Indentatio significat nidificationem, sed numerus spatiorum NIHIL
 * significat: qui duobus spatiis indentat idem dicit quod qui quattuor
 * aut tabulo. Ergo latitudo indentationis per ACERVUM in gradum
 * vertitur (regula Pythonis: latior => ingressus, angustior =>
 * egressus usque ad congruentiam).
 *
 * Consequentia quam consumptor scire debet: indentatio INCONSTANS
 * (e.g. tria spatia ubi duo praecesserunt) gradum PHANTASMA generat.
 * Non emendamus - coniectura quae 'errorem' corrigit indentationem
 * VERAM aeque facile destruit. Praevisio id monstrat; auctor videt et
 * textum emendat.
 *
 * Tabulus ad multiplum quattuor procedit (mos universalis).
 * -------------------------------------------------------------------- */

#define CAPITULA_PROFUNDITAS_MAXIMA VIII
#define CAPITULA_TABULI_LATITUDO    IV

/* --------------------------------------------------------------------
 * III. INSCRIPTIO - et cur regula ANGUSTA est
 *
 * Linea in duas partes scinditur: inscriptionem (index ab auctore
 * scriptus: "1", "XIV", "Capitulum 3") et titulum. Formae acceptae:
 *
 *     1 - De methodo          separator spatiis cinctus: - : – —
 *     1. De methodo           numerus, punctum aut uncinum, spatium
 *     1) De methodo
 *     XIV - De materia
 *     Prooemium               nulla inscriptio; tota linea titulus est
 *
 * INSCRIPTIO ANCORA FUTURA EST. Notae ad eam adhaerebunt sicut in
 * sententiis ad locum ab auctore scriptum. Ergo lex eadem valet:
 * ancora MENTIENS peior est quam absens.
 *
 * Ideo inscriptio agnoscitur SOLUM si cifram continet aut numerus
 * Romanus VERUS est (maiusculis, forma subtractiva stricta). Aliter
 * "The Long Road - and What Came After" inscriptionem "The Long Road"
 * pareret - quae numquam index fuit, sed pars tituli.
 *
 * Numerus Romanus stricte iudicatur ne "DID" aut "MIX" per casum
 * inscriptio fiat: verba Anglica ex litteris IVXLCDM composita non
 * pauca sunt, et regula laxa ea omnia caperet.
 * -------------------------------------------------------------------- */

#define CAPITULA_INSCRIPTIO_MAXIMA XXIV

/* --------------------------------------------------------------------
 * IV. CAPITULUM
 *
 * `ordo` series LEGENDI est, plana et ab I - non per gradum numerata.
 * Nidificatio in `gradus` sola vivit. Ita "capitulum quintum quod
 * legam" quaestio una est, non transitus arboris.
 *
 * Omnis chorda in piscinam datam TRANSCRIBITUR: fons in arena
 * vocationis vivere potest dum index superstes est (eadem ratio quam
 * sententiae sequuntur).
 * -------------------------------------------------------------------- */

nomen structura
{
    chorda inscriptio;   /* "1", "XIV", "Capitulum 3"; vacua si nulla */
    chorda titulus;      /* nomen capituli - NUMQUAM vacuum */
       i32 gradus;       /* profunditas; ZEPHYRUM = summa */
       i32 ordo;         /* positio in serie legendi, ab I */
       i32 linea;        /* linea fontis, ab I - pro nuntiis */
} Capitulum;

nomen structura
{
    Xar* capitula;         /* Capitulum */
    i32  gradus_maximus;   /* pro latitudine praevisionis */
} IndexLibri;

/* --------------------------------------------------------------------
 * V. LECTIO
 *
 * Fines linearum omnium generum accipiuntur (\n, \r\n, \r solum):
 * textus adglutinatus ex quocumque fonte venire potest, et linea
 * ultima sine fine legitima est.
 * -------------------------------------------------------------------- */

IndexLibri
capitula_legere (
     chorda  fons,
    Piscina* piscina);

#endif /* CAPITULA_H */
