/*
 * clipboard_platform.h - Accessus ad clipboard systematis
 *
 * Platform-agnostic API pro clipboard imaginum.
 * Implementatio in clipboard_platform_macos.m (macOS).
 */

#ifndef CLIPBOARD_PLATFORM_H
#define CLIPBOARD_PLATFORM_H

#include "latina.h"
#include "piscina.h"
#include "imago.h"

/* ============================================================
 * Functiones Principales
 * ============================================================ */

/*
 * clipboard_habet_imaginem - Quaerere si clipboard habet imaginem
 *
 * Redde: VERUM si imago est in clipboard, FALSUM aliter
 */
b32
clipboard_habet_imaginem(vacuum);

/*
 * clipboard_capere_imaginem - Capere imaginem ex clipboard
 *
 * @piscina: Piscina pro allocare pixela
 *
 * Redde: ImagoFructus cum RGBA pixela.
 *        Si non est imago, successus = FALSUM.
 */
ImagoFructus
clipboard_capere_imaginem(Piscina* piscina);

#endif /* CLIPBOARD_PLATFORM_H */
