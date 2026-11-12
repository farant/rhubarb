/*
 * plastica_textura.h - plastica: texturae genitae
 *
 * Tegula parva pixelorum (saxum, cortex, tabulae ...) ex genere,
 * magnitudine, semine et tribus coloribus fit. Functio pura: eadem
 * argumenta, eadem pixela in omni machina (generator integer; nulla
 * arithmetica fluitans). Nihil de solidis aut de statua scit;
 * plastica_legere eam vocat ubi solidum textura="..." habet
 * (project-specs/plastica-spec.md 13, 14, IV.2).
 *
 * DENSITAS: PLASTICA_TEXELLA_PER_UNITATEM texella per unitatem mundi
 * in omni facie - mensura ipsa loci 'clearing' (planum.glb: XVI in
 * omni materia), ut res nostrae iuxta eum eodem grano stent.
 *
 * COLORES: tres, ordine obscurus, medius, clarus (rgb, IX octeti).
 * Pixelum omne EXACTE unus ex tribus est, numquam mixtura - grana
 * dura manent et dithering tres tantum colores videt. Opaca omnia.
 *
 * TEGULA: margines conveniunt - tegula iuxta se posita suturam non
 * ostendit, dextrorsum et deorsum, in omni magnitudine.
 *
 * GENERA:
 *
 *   cortex    striae erectae inaequales (trunci)
 *   tabulae   tabulae erectae cum iuncturis et nodis raris
 *   saxum     saxa inaequalia cum rimis obscuris (muri sicci)
 *   lateres   ordines laterum alterni cum caemento
 *   stramen   fasciculi pendentes in ordinibus (tecta)
 *   frondes   maculae foliorum (comae arborum, frutices)
 *   muscus    maculae molles, clarae in obscuro
 *   humus     terra cum glebis et lapillis
 *   granum    color medius cum punctis sparsis (cetera omnia)
 *
 * IN DESCRIPTIONE (plastica.h):
 *
 *   <capsa magnitudo="2 1 2" textura="saxum" semen="3"
 *          colores="#555 #777 #999"/>
 *
 *   textura   genus supra, aut titulus texturae manu scriptae
 *   semen     ordinarius 0 (idem semen quod asperitas sphaerae
 *             legit: solidum unum, semen unum)
 *   colores   tres; si absunt ex 'color' fiunt: medius = color,
 *             obscurus = color per III/IV, clarus = color quarta
 *             parte ad album motus
 *
 * Tegula genita in descriptione semper PLASTICA_TEXTURA_LATUS
 * quadrata est (duae unitates). Solida quae genus, semen et colores
 * eosdem habent texturam UNAM in statua communicant.
 *
 * COORDINATAE: texella quadrata, numquam extenta. Facies plana:
 * u secundum latus horizontale faciei, v deorsum secundum clivum
 * eius (ordo summus texturae sursum); facies iacens: u = x, v = z.
 * Superficies curva (latera cylindri, tornus, sphaera, latera
 * extrusionis): u secundum ambitum currit, v secundum altitudinem
 * aut profilum; sutura una manet ubi ambitus clauditur (tegula
 * ambitum non dividit nisi casu). Ubi superficies angustatur (conus,
 * sphaera, tornus obliquus) texella in quaque facie recta manent et
 * suturae inter facies fiunt: conus sine extensione non explicatur.
 * Mensurae in spatio solidi ipsius sumuntur: sparsio cum
 * magnitudo_varia texella cum re scalat; exemplaria seriei texturam
 * eandem eodem loco ferunt.
 *
 * TEXTURA MANU SCRIPTA (liber <res>, ante usum):
 *
 *   <textura titulus="fenestra" latitudo="4"
 *            colores="#2a2a33 #ffcc55">
 *     aaaa
 *     abba
 *     abba
 *     aaaa
 *   </textura>
 *
 * Litterae a .. p colores ordine indicant (I .. XVI colores); spatia
 * et lineae novae nihil valent, ordines ex 'latitudo' secantur.
 * Maxima XVI x XVI. Repetitur ut genitae.
 *
 * RECUSAT plastica_legere, cum linea: genus ignotum (nec genitum nec
 * manu scriptum); colores qui tres non sunt; texturam manu scriptam
 * maiorem quam XVI x XVI, cum littera extra colores, cum ordine
 * imperfecto, sine titulo, cum titulo generis geniti aut iterato.
 */

#ifndef PLASTICA_TEXTURA_H
#define PLASTICA_TEXTURA_H

/* <aedilis corpus="lib/plastica_textura.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "imago.h"

#define PLASTICA_TEXELLA_PER_UNITATEM  XVI
#define PLASTICA_TEXTURA_LATUS         XXXII
#define PLASTICA_TEXTURA_MANU_MAXIMA   XVI
#define PLASTICA_TEXTURA_GENERA        IX

/*
 * plastica_textura_genus - nomen generis
 *
 * @index: 0 .. PLASTICA_TEXTURA_GENERA - I, ordine tabulae supra
 *
 * Redde: nomen (C, staticum); NIHIL extra fines
 */
constans character*
plastica_textura_genus (
    i32 index);

/*
 * plastica_textura_generare - tegulam pingere
 *
 * @genus:     nomen generis
 * @latitudo:  I .. CCLVI
 * @altitudo:  I .. CCLVI
 * @semen:     semen idem, pixela eadem
 * @colores:   IX octeti: obscurus, medius, clarus (r g b)
 * @exitus:    imago RGBA impletur (alpha CCLV); pixela in piscina
 * @piscina:   pixela exitus
 *
 * Redde: VERUM; FALSUM (exitus non tactus) si genus ignotum,
 * magnitudo extra fines, aut argumentum NIHIL
 */
b32
plastica_textura_generare (
           chorda  genus,
              i32  latitudo,
              i32  altitudo,
              i32  semen,
      constans i8* colores,
            Imago* exitus,
          Piscina* piscina);

#endif /* PLASTICA_TEXTURA_H */
