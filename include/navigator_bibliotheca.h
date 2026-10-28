/* navigator_bibliotheca.h - articuli servati et versiones eorum
 * (navigator N2.4; project-specs/navigator-spec.md V.3, V.11).
 * In volumine: VERITAS = acta duorum generum (JSON):
 *   "navigator-articulus" {id, url, situs}
 *   "navigator-versio"    {id, articulus, massa, url, titulus}
 * HTML crudum versionis = massa (sigillo addressata - communio eam
 * per nomen in actu sequitur). Index in memoria ex actis PROPRIIS
 * (replica huius voluminis) in apertione; scripturae eum renovant.
 * Versio nova solum si CONTENTUM differt: HTML novum et versio
 * ultima servata EODEM adaptatore extrahuntur, articulus_sigillum
 * comparatur - adaptator melior solus numquam "versionem novam"
 * facit. Versiones numquam delentur. Communis omnibus lateribus:
 * vicus semel aperit. */

#ifndef NAVIGATOR_BIBLIOTHECA_H
#define NAVIGATOR_BIBLIOTHECA_H

/* <aedilis corpus="lib/navigator_bibliotheca.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"
#include "moneta.h"
#include "articulus.h"

nomen structura NavigatorBibliotheca NavigatorBibliotheca;

nomen structura {
    chorda identitas;   /* ULID versionis */
    chorda momentum;    /* cum petita (ISO 8601, ex actu) */
    chorda url;         /* URL ultima petitionis (post redirectiones) */
    chorda titulus;     /* titulus articuli in hac versione */
    chorda massa;       /* SHA-256 hex HTML crudi */
} NavigatorVersio;

nomen structura {
    chorda identitas;   /* ULID articuli */
    chorda url;         /* URL canonica - clavis
                         * (vicipaedia_url_articuli) */
    chorda  situs;       /* "vicipaedia" */
       Xar* versiones;   /* NavigatorVersio, vetustissima prima */
       b32  favor;       /* N3.4: favor (actum ultimum vincit) */
} NavigatorArticulus;

/* adaptator situs: HTML -> Articulus (vicipaedia_articulus) */
nomen Articulus* (*NavigatorExtrahere)(chorda html,
    constans character* fons, Piscina* piscina);

/* fons monetae NIHIL = moneta_ulid (probationes: moneta_numerator) */
NavigatorBibliotheca*
navigator_bibliotheca_aperire (
       Volumen* volumen,
    MonetaFons  fons,
        vacuum* fontis_ctx,
       Piscina* piscina);

i32
navigator_articulorum_numerus (
    constans NavigatorBibliotheca* b);

/* ordine creationis; NIHIL extra ambitum */
constans NavigatorArticulus*
navigator_articulus (
    constans NavigatorBibliotheca* b,
                              i32  k);

/* per URL canonicam; NIHIL si non servatus */
constans NavigatorArticulus*
navigator_articulum_invenire (
    constans NavigatorBibliotheca* b,
                           chorda  url);

nomen enumeratio {
    NAVIGATOR_NOVUM = ZEPHYRUM,   /* articulus + versio prima scripta */
    NAVIGATOR_VERSIO_NOVA,        /* contentum mutatum: versio addita */
    NAVIGATOR_IDEM,               /* contentum idem: nihil scriptum */
    NAVIGATOR_VITIUM              /* extractio defecit aut volumen */
} NavigatorExitus;

/* exemplar petitum offerre. url = clavis canonica, url_finalis =
 * http_vocatio_url. *versio (NIHIL licet) = versio nova, aut ultima
 * si IDEM. */
NavigatorExitus
navigator_versionem_offerre (
              NavigatorBibliotheca*  b,
                            chorda   url,
                            chorda   url_finalis,
                constans character*  situs,
                            chorda   html,
                NavigatorExtrahere   extrahere,
          constans NavigatorVersio** versio);

/* favor ponere/tollere: actum "navigator-favor" {articulus, favor};
 * status idem = nihil scriptum. FALSUM si articulus ignotus */
b32
navigator_favorem_ponere (
    NavigatorBibliotheca* b,
                  chorda  articulus,
                     b32  favor);

/* positio lectionis (N3.4c): versio lecta et octetus primus paginae
 * currentis - in textu articuli aut indicis nexuum. PRAESENTIA
 * voluminis "navigator/positio/<ULID articuli>" (ultima vincit; EXTRA
 * acta - motus crebri, nec diarium nec massas auget). Offset, non
 * numerus paginae: area mutata paginas recomputat, positio manet */
nomen structura {
    chorda versio;       /* ULID versionis lectae */
       i32 offset;       /* octetus in textu */
       b32 in_nexibus;   /* in paginis indicis nexuum (N3.1b) */
} NavigatorPositio;

/* positionem servare; FALSUM si articulus ignotus */
b32
navigator_positionem_scribere (
              NavigatorBibliotheca* b,
                            chorda  articulus,
         constans NavigatorPositio* positio);

/* positio servata (chordae ex piscina); FALSUM si nulla */
b32
navigator_positionem_legere (
    NavigatorBibliotheca* b,
                  chorda  articulus,
        NavigatorPositio* positio,
                 Piscina* piscina);

/* HTML versionis ex massa; vacua si massa abest */
chorda
navigator_versionis_html (
           NavigatorBibliotheca* b,
       constans NavigatorVersio* versio,
                        Piscina* piscina);

#endif /* NAVIGATOR_BIBLIOTHECA_H */
