/*
 * imago_png.h - Imaginem in PNG codificare
 *
 * PORTABILE. Nullum stratum platformae, nulla dependentia: super
 * flatura (DEFLATE) et flatura_crc32 (IEEE 802.3, idem polynomium
 * quo PNG utitur) solis stat.
 *
 * CUR NON STRATUM PLATFORMAE: limes platformae ad PIXELA CAPIENDA
 * pertinet, non ad formam plagulae. "Contenta huius fenestrae
 * capere" vere macOS est; "RGBA in PNG convertere" arithmetica est.
 * Antequam haec bibliotheca esset, AppKit utrumque faciebat - ideo
 * codificatio bis descripta erat (lib/vitrea_macos.m,
 * tools/vitrea_spica.m), quia locus portabilis nullus erat quo
 * poneretur.
 *
 * FORMA EMISSA: profunditas VIII bitorum, genus coloris VI (RGBA),
 * filtrum NULLUM in omni versu, sine intertextura, IDAT unum.
 * Emissio DETERMINATA est: eadem imago eosdem bytes semper parit.
 * Hoc probationibus necessarium est - artificia quae inter
 * percursiones variant sua ipsorum quaestio fiunt.
 *
 * FILTRA NON ADHIBENTUR: PNG quinque filtra praebet quae
 * compressionem iuvant. Filtrum NULLUM plagulas maiores facit sed
 * codicem multo minorem. Si postea filtra addere volumus, id
 * MUTATIO IMPLEMENTATIONIS SOLA est - haec interfacies eadem manet.
 * (Regula Eskilis: interfaciem primo recte facere, implementationem
 * postea meliorem.)
 *
 * DECODIFICATIO hic NON est. Ea per imago_caricare_ex_* fit, quae
 * stb_image vendoratum adhibet. Asymmetria consulta est:
 * codificatio unam viam scribit, decodificatio omnes legere debet
 * (profunditates, palettas, intertexturam, quinque filtra) - opus
 * multo maius.
 */

#ifndef IMAGO_PNG_H
#define IMAGO_PNG_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "imago.h"

/* ============================================================
 * Typi
 * ============================================================ */

/*
 * PngFructus - Fructus codificationis
 *
 * @datum:   bytes PNG (in piscina; non liberandi)
 * @mensura: longitudo bytorum
 * @error:   causa NOMINATA si !successus (numquam tacet)
 */
nomen structura {
    b32    successus;
    i8*    datum;
    i32    mensura;
    chorda error;
} PngFructus;

/* ============================================================
 * Functiones
 * ============================================================ */

/*
 * imago_png_codificare - Imaginem in bytes PNG vertere
 *
 * @imago:   imago RGBA vertenda
 * @piscina: arena pro emissione (et pro nuntio erroris)
 *
 * Redde: PngFructus. Nihil scribitur, nihil legitur - computatio
 *        pura, ergo sine plagula probari potest.
 */
PngFructus
imago_png_codificare (constans Imago* imago, Piscina* piscina);

/*
 * imago_png_scribere - Imaginem in plagulam PNG scribere
 *
 * @imago:   imago RGBA scribenda
 * @via:     via plagulae
 * @piscina: arena
 *
 * Redde: PngFructus (datum/mensura ea sunt quae scripta sunt).
 *        Commoditas sola: codificare + scribere. Qui bytes solos
 *        vult imago_png_codificare vocet.
 */
PngFructus
imago_png_scribere (constans Imago*         imago,
                    constans character*     via,
                    Piscina*                piscina);

#endif /* IMAGO_PNG_H */
