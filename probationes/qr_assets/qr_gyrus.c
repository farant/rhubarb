/* qr_gyrus.c - generator BMP pro oraculo gyri (probatio_qr_gyrus.sh)
 *
 * Non est probatio ipsa: est DIMIDIUM SCRIBENS gyri. Alterum
 * dimidium (decodere.js) CoreImage adhibet - decodatorem qui de
 * codice nostro nihil scit. Probationes structurales (probatio_qr.c)
 * FORMAM verificant; hoc solum verificat BITS legi posse.
 *
 * Usus: qr_gyrus <textus> <via.bmp> <ecc 0-3>
 * Effusio: "versio=N amplitudo=M" aut "FRACTUM: generatio"
 */

#include "qr.h"
#include "piscina.h"
#include "chorda.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SCALA VIII
#define QUIES IV

interior vacuum
_octetos_scribere (
    FILE* f,
     i32  v,
     i32  n)
{
    i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        fputc((int)((v >> (i * VIII)) & 0xFF), f);
    }
}

s32
principale (
      integer   argc,
    character** argv)
{
    Piscina* p = piscina_generare_dynamicum("qr_gyrus", 4194304);
     chorda  datum;
         QR  qr;
    FILE*    f;
    i32      lat, pad;
    s32      x, y;
    constans character* textus = (argc > I) ? argv[I] : "salve";
    constans character* via    = (argc > II) ? argv[II] : "qr.bmp";
    i32      gradus = (argc > III) ? (i32)atoi(argv[III]) : I;

    si (p == NIHIL)
    {
        imprimere("FRACTUM: piscina\n");
        redde I;
    }
    datum  = chorda_ex_literis(textus, p);
    qr     = qr_generare(datum, (QREcc)gradus, p);
    si (!qr.successus)
    {
        imprimere("FRACTUM: generatio\n");
        redde I;
    }
    imprimere("versio=%d amplitudo=%d\n", (int)qr.versio,
        (int)qr.amplitudo);

    lat = (qr.amplitudo + QUIES * II) * SCALA;
    pad = (IV - (lat * III) % IV) % IV;

    f = fopen(via, "wb");
    si (f == NIHIL)
    {
        redde I;
    }
    /* caput BMP (XXIV bit, sine compressione, ordine infra-supra) */
    fputc('B', f); fputc('M', f);
    _octetos_scribere(f, 54 + (lat * III + pad) * lat, IV);
    _octetos_scribere(f, ZEPHYRUM, IV);
    _octetos_scribere(f, 54, IV);
    _octetos_scribere(f, 40, IV);
    _octetos_scribere(f, lat, IV);
    _octetos_scribere(f, lat, IV);
    _octetos_scribere(f, I, II);
    _octetos_scribere(f, 24, II);
    _octetos_scribere(f, ZEPHYRUM, IV);
    _octetos_scribere(f, (lat * III + pad) * lat, IV);
    _octetos_scribere(f, 2835, IV);
    _octetos_scribere(f, 2835, IV);
    _octetos_scribere(f, ZEPHYRUM, IV);
    _octetos_scribere(f, ZEPHYRUM, IV);

    /* y SIGNATUS esse DEBET: i32 insignatus est, ergo 'y >= 0'
     * semper verum et ansa numquam finiret (decipula domus
     * mensurata - imago 5.9 GB primo conatu) */
    per (y = (s32)lat - I; y >= ZEPHYRUM; y--)
    {
        per (x = ZEPHYRUM; x < (s32)lat; x++)
        {
            s32 m = qr_modulus(&qr, x / SCALA - QUIES,
                y / SCALA - QUIES);
            int v = (m == I) ? 0 : 255;

            fputc(v, f); fputc(v, f); fputc(v, f);
        }
        per (x = ZEPHYRUM; x < (s32)pad; x++)
        {
            fputc(0, f);
        }
    }
    fclose(f);
    redde ZEPHYRUM;
}
