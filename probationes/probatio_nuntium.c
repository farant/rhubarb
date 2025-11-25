/* probatio_nuntium.c - Probationes Nuntii (Protobuf-style serialization) */
#include "latina.h"
#include "nuntium.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* Tag numeri pro probationibus */
#define TAG_ID      I
#define TAG_NOMEN   II
#define TAG_AETAS   III
#define TAG_ACTIVUS IV
#define TAG_PRETIUM V
#define TAG_DATUM   VI
#define TAG_NIDUM   VII

s32 principale(vacuum)
{
	Piscina* piscina;
	    b32  praeteritus;

	/* Aperire credo et piscina */
	piscina = piscina_generare_dynamicum("probatio_nuntium", MMMMXCVI);
	si (!piscina)
	{
		imprimere("FRACTA: piscina_generatio\n");
		redde I;
	}
	credo_aperire(piscina);


	/* ========================================================
	 * PROBARE: Varint encoding/decoding
	 * ======================================================== */

	{
		NuntiumScriptor* scriptor;
		NuntiumLector*   lector;
		chorda           datum;
		i32              tag;
		i32              wire_type;
		i64              valor;

		imprimere("\n--- Probans varint encoding/decoding ---\n");

		scriptor = nuntium_scriptor_creare(piscina, CXXVIII);
		CREDO_NON_NIHIL(scriptor);

		/* Scribere varintes diversos */
		CREDO_VERUM(nuntium_scribere_varint(scriptor, TAG_ID, 0));
		CREDO_VERUM(nuntium_scribere_varint(scriptor, TAG_ID, I));
		CREDO_VERUM(nuntium_scribere_varint(scriptor, TAG_ID, CXXVII));   /* 127 - max 1 byte */
		CREDO_VERUM(nuntium_scribere_varint(scriptor, TAG_ID, CXXVIII));  /* 128 - needs 2 bytes */
		CREDO_VERUM(nuntium_scribere_varint(scriptor, TAG_ID, CCLV));     /* 255 */
		CREDO_VERUM(nuntium_scribere_varint(scriptor, TAG_ID, M));        /* 1000 */

		datum = nuntium_scriptor_datum(scriptor);
		CREDO_VERUM(datum.mensura > ZEPHYRUM);

		/* Legere et verificare */
		lector = nuntium_lector_creare(piscina, datum);
		CREDO_NON_NIHIL(lector);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		CREDO_AEQUALIS_I32(tag, TAG_ID);
		CREDO_AEQUALIS_I32(wire_type, NUNTIUM_WIRE_VARINT);
		valor = nuntium_legere_varint(lector);
		CREDO_AEQUALIS_I32((i32)valor, ZEPHYRUM);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_varint(lector);
		CREDO_AEQUALIS_I32((i32)valor, I);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_varint(lector);
		CREDO_AEQUALIS_I32((i32)valor, CXXVII);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_varint(lector);
		CREDO_AEQUALIS_I32((i32)valor, CXXVIII);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_varint(lector);
		CREDO_AEQUALIS_I32((i32)valor, CCLV);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_varint(lector);
		CREDO_AEQUALIS_I32((i32)valor, M);

		CREDO_FALSUM(nuntium_lector_habet_plus(lector));
		CREDO_FALSUM(nuntium_lector_error(lector));
	}


	/* ========================================================
	 * PROBARE: Signed varint (zigzag encoding)
	 * ======================================================== */

	{
		NuntiumScriptor* scriptor;
		NuntiumLector*   lector;
		chorda           datum;
		i32              tag;
		i32              wire_type;
		s64              valor;

		imprimere("\n--- Probans signed varint (zigzag) ---\n");

		scriptor = nuntium_scriptor_creare(piscina, CXXVIII);
		CREDO_NON_NIHIL(scriptor);

		/* Scribere numeros signatos */
		CREDO_VERUM(nuntium_scribere_svarint(scriptor, TAG_AETAS, 0));
		CREDO_VERUM(nuntium_scribere_svarint(scriptor, TAG_AETAS, -1));
		CREDO_VERUM(nuntium_scribere_svarint(scriptor, TAG_AETAS, 1));
		CREDO_VERUM(nuntium_scribere_svarint(scriptor, TAG_AETAS, -2));
		CREDO_VERUM(nuntium_scribere_svarint(scriptor, TAG_AETAS, 2));
		CREDO_VERUM(nuntium_scribere_svarint(scriptor, TAG_AETAS, -100));
		CREDO_VERUM(nuntium_scribere_svarint(scriptor, TAG_AETAS, 100));

		datum = nuntium_scriptor_datum(scriptor);

		lector = nuntium_lector_creare(piscina, datum);
		CREDO_NON_NIHIL(lector);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_svarint(lector);
		CREDO_AEQUALIS_S32((s32)valor, ZEPHYRUM);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_svarint(lector);
		CREDO_AEQUALIS_S32((s32)valor, -I);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_svarint(lector);
		CREDO_AEQUALIS_S32((s32)valor, I);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_svarint(lector);
		CREDO_AEQUALIS_S32((s32)valor, -II);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_svarint(lector);
		CREDO_AEQUALIS_S32((s32)valor, II);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_svarint(lector);
		CREDO_AEQUALIS_S32((s32)valor, -C);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_svarint(lector);
		CREDO_AEQUALIS_S32((s32)valor, C);

		CREDO_FALSUM(nuntium_lector_error(lector));
	}


	/* ========================================================
	 * PROBARE: Zigzag encode/decode directly
	 * ======================================================== */

	{
		imprimere("\n--- Probans zigzag encode/decode ---\n");

		/* 0 -> 0 */
		CREDO_AEQUALIS_I32((i32)nuntium_zigzag_encode(0), ZEPHYRUM);
		CREDO_AEQUALIS_S32((s32)nuntium_zigzag_decode(0), ZEPHYRUM);

		/* -1 -> 1 */
		CREDO_AEQUALIS_I32((i32)nuntium_zigzag_encode(-1), I);
		CREDO_AEQUALIS_S32((s32)nuntium_zigzag_decode(1), -I);

		/* 1 -> 2 */
		CREDO_AEQUALIS_I32((i32)nuntium_zigzag_encode(1), II);
		CREDO_AEQUALIS_S32((s32)nuntium_zigzag_decode(2), I);

		/* -2 -> 3 */
		CREDO_AEQUALIS_I32((i32)nuntium_zigzag_encode(-2), III);
		CREDO_AEQUALIS_S32((s32)nuntium_zigzag_decode(3), -II);

		/* 2 -> 4 */
		CREDO_AEQUALIS_I32((i32)nuntium_zigzag_encode(2), IV);
		CREDO_AEQUALIS_S32((s32)nuntium_zigzag_decode(4), II);
	}


	/* ========================================================
	 * PROBARE: Fixed 32-bit
	 * ======================================================== */

	{
		NuntiumScriptor* scriptor;
		NuntiumLector*   lector;
		chorda           datum;
		i32              tag;
		i32              wire_type;
		i32              valor;

		imprimere("\n--- Probans fixed 32-bit ---\n");

		scriptor = nuntium_scriptor_creare(piscina, CXXVIII);
		CREDO_NON_NIHIL(scriptor);

		CREDO_VERUM(nuntium_scribere_32bit(scriptor, TAG_ID, ZEPHYRUM));
		CREDO_VERUM(nuntium_scribere_32bit(scriptor, TAG_ID, CXXVIII));
		CREDO_VERUM(nuntium_scribere_32bit(scriptor, TAG_ID, 0x12345678));

		datum = nuntium_scriptor_datum(scriptor);

		lector = nuntium_lector_creare(piscina, datum);
		CREDO_NON_NIHIL(lector);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		CREDO_AEQUALIS_I32(wire_type, NUNTIUM_WIRE_32BIT);
		valor = nuntium_legere_32bit(lector);
		CREDO_AEQUALIS_I32(valor, ZEPHYRUM);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_32bit(lector);
		CREDO_AEQUALIS_I32(valor, CXXVIII);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_32bit(lector);
		CREDO_AEQUALIS_I32(valor, 0x12345678);

		CREDO_FALSUM(nuntium_lector_error(lector));
	}


	/* ========================================================
	 * PROBARE: Fixed 64-bit
	 * ======================================================== */

	{
		NuntiumScriptor* scriptor;
		NuntiumLector*   lector;
		chorda           datum;
		i32              tag;
		i32              wire_type;
		i64              valor;

		imprimere("\n--- Probans fixed 64-bit ---\n");

		scriptor = nuntium_scriptor_creare(piscina, CXXVIII);
		CREDO_NON_NIHIL(scriptor);

		CREDO_VERUM(nuntium_scribere_64bit(scriptor, TAG_ID, ZEPHYRUM));
		CREDO_VERUM(nuntium_scribere_64bit(scriptor, TAG_ID, 0x123456789ABCDEF0ULL));

		datum = nuntium_scriptor_datum(scriptor);

		lector = nuntium_lector_creare(piscina, datum);
		CREDO_NON_NIHIL(lector);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		CREDO_AEQUALIS_I32(wire_type, NUNTIUM_WIRE_64BIT);
		valor = nuntium_legere_64bit(lector);
		CREDO_AEQUALIS_I32((i32)valor, ZEPHYRUM);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_64bit(lector);
		CREDO_VERUM(valor == 0x123456789ABCDEF0ULL);

		CREDO_FALSUM(nuntium_lector_error(lector));
	}


	/* ========================================================
	 * PROBARE: Float (f32)
	 * ======================================================== */

	{
		NuntiumScriptor* scriptor;
		NuntiumLector*   lector;
		chorda           datum;
		i32              tag;
		i32              wire_type;
		f32              valor;

		imprimere("\n--- Probans f32 ---\n");

		scriptor = nuntium_scriptor_creare(piscina, CXXVIII);
		CREDO_NON_NIHIL(scriptor);

		CREDO_VERUM(nuntium_scribere_f32(scriptor, TAG_PRETIUM, 0.0f));
		CREDO_VERUM(nuntium_scribere_f32(scriptor, TAG_PRETIUM, 3.14159f));
		CREDO_VERUM(nuntium_scribere_f32(scriptor, TAG_PRETIUM, -123.456f));

		datum = nuntium_scriptor_datum(scriptor);

		lector = nuntium_lector_creare(piscina, datum);
		CREDO_NON_NIHIL(lector);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_f32(lector);
		CREDO_VERUM(valor == 0.0f);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_f32(lector);
		CREDO_VERUM(valor > 3.14f && valor < 3.15f);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_f32(lector);
		CREDO_VERUM(valor < -123.0f && valor > -124.0f);

		CREDO_FALSUM(nuntium_lector_error(lector));
	}


	/* ========================================================
	 * PROBARE: Double (f64)
	 * ======================================================== */

	{
		NuntiumScriptor* scriptor;
		NuntiumLector*   lector;
		chorda           datum;
		i32              tag;
		i32              wire_type;
		f64              valor;

		imprimere("\n--- Probans f64 ---\n");

		scriptor = nuntium_scriptor_creare(piscina, CXXVIII);
		CREDO_NON_NIHIL(scriptor);

		CREDO_VERUM(nuntium_scribere_f64(scriptor, TAG_PRETIUM, 0.0));
		CREDO_VERUM(nuntium_scribere_f64(scriptor, TAG_PRETIUM, 3.141592653589793));

		datum = nuntium_scriptor_datum(scriptor);

		lector = nuntium_lector_creare(piscina, datum);
		CREDO_NON_NIHIL(lector);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_f64(lector);
		CREDO_VERUM(valor == 0.0);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		valor = nuntium_legere_f64(lector);
		CREDO_VERUM(valor > 3.14159265 && valor < 3.14159266);

		CREDO_FALSUM(nuntium_lector_error(lector));
	}


	/* ========================================================
	 * PROBARE: Chorda (length-delimited string)
	 * ======================================================== */

	{
		NuntiumScriptor* scriptor;
		NuntiumLector*   lector;
		chorda           datum;
		chorda           nomen_in;
		chorda           nomen_out;
		i32              tag;
		i32              wire_type;

		imprimere("\n--- Probans chorda ---\n");

		scriptor = nuntium_scriptor_creare(piscina, CXXVIII);
		CREDO_NON_NIHIL(scriptor);

		nomen_in = chorda_ex_literis("Salve Munde", piscina);
		CREDO_VERUM(nuntium_scribere_chorda(scriptor, TAG_NOMEN, nomen_in));

		/* Chorda vacua */
		nomen_in.datum = NIHIL;
		nomen_in.mensura = ZEPHYRUM;
		CREDO_VERUM(nuntium_scribere_chorda(scriptor, TAG_NOMEN, nomen_in));

		datum = nuntium_scriptor_datum(scriptor);

		lector = nuntium_lector_creare(piscina, datum);
		CREDO_NON_NIHIL(lector);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		CREDO_AEQUALIS_I32(tag, TAG_NOMEN);
		CREDO_AEQUALIS_I32(wire_type, NUNTIUM_WIRE_LENGTH_DELIMITED);
		nomen_out = nuntium_legere_chorda(lector);
		CREDO_AEQUALIS_I32(nomen_out.mensura, XI);
		CREDO_VERUM(memcmp(nomen_out.datum, "Salve Munde", XI) == ZEPHYRUM);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		nomen_out = nuntium_legere_chorda(lector);
		CREDO_AEQUALIS_I32(nomen_out.mensura, ZEPHYRUM);

		CREDO_FALSUM(nuntium_lector_error(lector));
	}


	/* ========================================================
	 * PROBARE: Bytes (raw binary data)
	 * ======================================================== */

	{
		NuntiumScriptor* scriptor;
		NuntiumLector*   lector;
		chorda           datum;
		chorda           bytes_out;
		i8               bytes_in[] = {0x00, 0x01, 0x02, (i8)0xFF, (i8)0xFE};
		i32              tag;
		i32              wire_type;

		imprimere("\n--- Probans bytes ---\n");

		scriptor = nuntium_scriptor_creare(piscina, CXXVIII);
		CREDO_NON_NIHIL(scriptor);

		CREDO_VERUM(nuntium_scribere_bytes(scriptor, TAG_DATUM, bytes_in, V));

		datum = nuntium_scriptor_datum(scriptor);

		lector = nuntium_lector_creare(piscina, datum);
		CREDO_NON_NIHIL(lector);

		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		bytes_out = nuntium_legere_bytes(lector);
		CREDO_AEQUALIS_I32(bytes_out.mensura, V);
		CREDO_VERUM(memcmp(bytes_out.datum, bytes_in, V) == ZEPHYRUM);

		CREDO_FALSUM(nuntium_lector_error(lector));
	}


	/* ========================================================
	 * PROBARE: Nuntium compositum (multiple fields)
	 * ======================================================== */

	{
		NuntiumScriptor* scriptor;
		NuntiumLector*   lector;
		chorda           datum;
		chorda           nomen_in;
		chorda           nomen_out;
		i32              tag;
		i32              wire_type;
		i64              id;
		s64              aetas;
		b32              activus;

		imprimere("\n--- Probans nuntium compositum ---\n");

		scriptor = nuntium_scriptor_creare(piscina, CXXVIII);
		CREDO_NON_NIHIL(scriptor);

		/* Scribere nuntium cum multiplices campos */
		CREDO_VERUM(nuntium_scribere_varint(scriptor, TAG_ID, XLII));
		nomen_in = chorda_ex_literis("Franciscus", piscina);
		CREDO_VERUM(nuntium_scribere_chorda(scriptor, TAG_NOMEN, nomen_in));
		CREDO_VERUM(nuntium_scribere_svarint(scriptor, TAG_AETAS, XXX));
		CREDO_VERUM(nuntium_scribere_varint(scriptor, TAG_ACTIVUS, I));

		datum = nuntium_scriptor_datum(scriptor);

		/* Legere et verificare */
		lector = nuntium_lector_creare(piscina, datum);
		CREDO_NON_NIHIL(lector);

		id = 0;
		nomen_out.datum = NIHIL;
		nomen_out.mensura = ZEPHYRUM;
		aetas = 0;
		activus = FALSUM;

		dum (nuntium_legere_field(lector, &tag, &wire_type))
		{
			commutatio (tag)
			{
				casus TAG_ID:
					id = nuntium_legere_varint(lector);
					frange;
				casus TAG_NOMEN:
					nomen_out = nuntium_legere_chorda(lector);
					frange;
				casus TAG_AETAS:
					aetas = nuntium_legere_svarint(lector);
					frange;
				casus TAG_ACTIVUS:
					activus = (b32)nuntium_legere_varint(lector);
					frange;
				ordinarius:
					nuntium_saltare_field(lector, wire_type);
					frange;
			}
		}

		CREDO_AEQUALIS_I32((i32)id, XLII);
		CREDO_AEQUALIS_I32(nomen_out.mensura, X);
		CREDO_VERUM(memcmp(nomen_out.datum, "Franciscus", X) == ZEPHYRUM);
		CREDO_AEQUALIS_S32((s32)aetas, XXX);
		CREDO_VERUM(activus);

		CREDO_FALSUM(nuntium_lector_error(lector));
	}


	/* ========================================================
	 * PROBARE: Saltare field ignotum
	 * ======================================================== */

	{
		NuntiumScriptor* scriptor;
		NuntiumLector*   lector;
		chorda           datum;
		chorda           nomen_in;
		i32              tag;
		i32              wire_type;
		i64              id;

		imprimere("\n--- Probans saltare field ignotum ---\n");

		scriptor = nuntium_scriptor_creare(piscina, CXXVIII);
		CREDO_NON_NIHIL(scriptor);

		/* Scribere campos cum tag ignotum in medio */
		CREDO_VERUM(nuntium_scribere_varint(scriptor, TAG_ID, XLII));
		nomen_in = chorda_ex_literis("ignorare hoc", piscina);
		CREDO_VERUM(nuntium_scribere_chorda(scriptor, C, nomen_in));  /* Tag 100 = ignotum */
		CREDO_VERUM(nuntium_scribere_32bit(scriptor, CI, 0x12345678)); /* Tag 101 = ignotum */
		CREDO_VERUM(nuntium_scribere_varint(scriptor, TAG_AETAS, XXV));

		datum = nuntium_scriptor_datum(scriptor);

		lector = nuntium_lector_creare(piscina, datum);
		CREDO_NON_NIHIL(lector);

		id = 0;

		dum (nuntium_legere_field(lector, &tag, &wire_type))
		{
			commutatio (tag)
			{
				casus TAG_ID:
					id = nuntium_legere_varint(lector);
					frange;
				casus TAG_AETAS:
					/* Verificare id iam lectum */
					CREDO_AEQUALIS_I32((i32)id, XLII);
					CREDO_AEQUALIS_I32((i32)nuntium_legere_varint(lector), XXV);
					frange;
				ordinarius:
					/* Saltare campos ignotos */
					CREDO_VERUM(nuntium_saltare_field(lector, wire_type));
					frange;
			}
		}

		CREDO_FALSUM(nuntium_lector_error(lector));
	}


	/* ========================================================
	 * PROBARE: Nuntium nidum (nested message)
	 * ======================================================== */

	{
		NuntiumScriptor* scriptor_exterior;
		NuntiumScriptor* scriptor_interior;
		NuntiumLector*   lector_exterior;
		NuntiumLector*   lector_interior;
		chorda           datum_exterior;
		chorda           datum_interior;
		chorda           titulus;
		i32              tag;
		i32              wire_type;

		imprimere("\n--- Probans nuntium nidum ---\n");

		/* Creare nuntium interiorem */
		scriptor_interior = nuntium_scriptor_creare(piscina, CXXVIII);
		CREDO_NON_NIHIL(scriptor_interior);

		titulus = chorda_ex_literis("Interior", piscina);
		CREDO_VERUM(nuntium_scribere_varint(scriptor_interior, TAG_ID, XCIX));
		CREDO_VERUM(nuntium_scribere_chorda(scriptor_interior, TAG_NOMEN, titulus));

		datum_interior = nuntium_scriptor_datum(scriptor_interior);

		/* Creare nuntium exteriorem cum nido */
		scriptor_exterior = nuntium_scriptor_creare(piscina, CXXVIII);
		CREDO_NON_NIHIL(scriptor_exterior);

		CREDO_VERUM(nuntium_scribere_varint(scriptor_exterior, TAG_ID, I));
		CREDO_VERUM(nuntium_scribere_nuntium(scriptor_exterior, TAG_NIDUM, datum_interior));

		datum_exterior = nuntium_scriptor_datum(scriptor_exterior);

		/* Legere nuntium exteriorem */
		lector_exterior = nuntium_lector_creare(piscina, datum_exterior);
		CREDO_NON_NIHIL(lector_exterior);

		CREDO_VERUM(nuntium_legere_field(lector_exterior, &tag, &wire_type));
		CREDO_AEQUALIS_I32(tag, TAG_ID);
		CREDO_AEQUALIS_I32((i32)nuntium_legere_varint(lector_exterior), I);

		CREDO_VERUM(nuntium_legere_field(lector_exterior, &tag, &wire_type));
		CREDO_AEQUALIS_I32(tag, TAG_NIDUM);
		datum_interior = nuntium_legere_nuntium(lector_exterior);

		/* Legere nuntium interiorem */
		lector_interior = nuntium_lector_creare(piscina, datum_interior);
		CREDO_NON_NIHIL(lector_interior);

		CREDO_VERUM(nuntium_legere_field(lector_interior, &tag, &wire_type));
		CREDO_AEQUALIS_I32(tag, TAG_ID);
		CREDO_AEQUALIS_I32((i32)nuntium_legere_varint(lector_interior), XCIX);

		CREDO_VERUM(nuntium_legere_field(lector_interior, &tag, &wire_type));
		CREDO_AEQUALIS_I32(tag, TAG_NOMEN);
		titulus = nuntium_legere_chorda(lector_interior);
		CREDO_AEQUALIS_I32(titulus.mensura, VIII);
		CREDO_VERUM(memcmp(titulus.datum, "Interior", VIII) == ZEPHYRUM);

		CREDO_FALSUM(nuntium_lector_error(lector_interior));
		CREDO_FALSUM(nuntium_lector_error(lector_exterior));
	}


	/* ========================================================
	 * PROBARE: Scriptor purgare et reuse
	 * ======================================================== */

	{
		NuntiumScriptor* scriptor;
		NuntiumLector*   lector;
		chorda           datum;
		i32              tag;
		i32              wire_type;

		imprimere("\n--- Probans scriptor purgare ---\n");

		scriptor = nuntium_scriptor_creare(piscina, CXXVIII);
		CREDO_NON_NIHIL(scriptor);

		/* Prima scriptio */
		CREDO_VERUM(nuntium_scribere_varint(scriptor, TAG_ID, I));
		datum = nuntium_scriptor_datum(scriptor);
		CREDO_VERUM(datum.mensura > ZEPHYRUM);

		/* Purgare */
		nuntium_scriptor_purgare(scriptor);
		datum = nuntium_scriptor_datum(scriptor);
		CREDO_AEQUALIS_I32(datum.mensura, ZEPHYRUM);

		/* Secunda scriptio */
		CREDO_VERUM(nuntium_scribere_varint(scriptor, TAG_ID, II));
		datum = nuntium_scriptor_datum(scriptor);

		lector = nuntium_lector_creare(piscina, datum);
		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		CREDO_AEQUALIS_I32((i32)nuntium_legere_varint(lector), II);

		CREDO_FALSUM(nuntium_lector_error(lector));
	}


	/* ========================================================
	 * PROBARE: Varint mensura
	 * ======================================================== */

	{
		imprimere("\n--- Probans varint mensura ---\n");

		CREDO_AEQUALIS_I32(nuntium_varint_mensura(0), I);
		CREDO_AEQUALIS_I32(nuntium_varint_mensura(CXXVII), I);    /* 127 */
		CREDO_AEQUALIS_I32(nuntium_varint_mensura(CXXVIII), II);  /* 128 */
		CREDO_AEQUALIS_I32(nuntium_varint_mensura(0x3FFF), II);   /* 16383 */
		CREDO_AEQUALIS_I32(nuntium_varint_mensura(0x4000), III);  /* 16384 */
	}


	/* ========================================================
	 * PROBARE: Error handling - read past end
	 * ======================================================== */

	{
		NuntiumLector* lector;
		chorda         datum;
		i8             datum_bytes[I];
		i32            tag;
		i32            wire_type;

		imprimere("\n--- Probans error handling ---\n");

		/* Creare datum breve */
		datum_bytes[ZEPHYRUM] = 'x';
		datum.datum = datum_bytes;
		datum.mensura = I;

		lector = nuntium_lector_creare(piscina, datum);
		CREDO_NON_NIHIL(lector);

		/* Legere field (debet fracta) */
		CREDO_VERUM(nuntium_legere_field(lector, &tag, &wire_type));
		/* Trying to read more should fail */
		CREDO_FALSUM(nuntium_legere_field(lector, &tag, &wire_type));
	}


	/* ========================================================
	 * PROBARE: NIHIL handling
	 * ======================================================== */

	{
		chorda datum;

		imprimere("\n--- Probans NIHIL handling ---\n");

		datum.datum = NIHIL;
		datum.mensura = ZEPHYRUM;

		CREDO_NIHIL(nuntium_scriptor_creare(NIHIL, CXXVIII));
		CREDO_NIHIL(nuntium_lector_creare(NIHIL, datum));

		CREDO_VERUM(nuntium_scriptor_error(NIHIL));
		CREDO_VERUM(nuntium_lector_error(NIHIL));

		CREDO_FALSUM(nuntium_scribere_varint(NIHIL, I, I));
		CREDO_FALSUM(nuntium_legere_field(NIHIL, NIHIL, NIHIL));
	}


	/* ========================================================
	 * Compendium
	 * ======================================================== */

	imprimere("\n");
	credo_imprimere_compendium();

	praeteritus = credo_omnia_praeterierunt();

	/* Destructio */
	credo_claudere();
	piscina_destruere(piscina);

	si (praeteritus)
	{
		redde ZEPHYRUM;
	}
	alioquin
	{
		redde I;
	}
}
