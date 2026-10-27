# imago.worklog.md

## 2026-09-24 — `imago_mensuras_ex_file` / `_ex_memoria` (lapide feature-requests/006)

Width and height WITHOUT decoding: `stbi_info` / `stbi_info_from_memory`
read the header only (PNG IHDR etc.), no pixel allocation — the tester
was decoding a 2021×3088 RGBA page (~25 MB) to read two integers. Same
shape as the loaders: plain-C wrappers above `latina.h` (stb_image's
macros collide with latina's), public functions below. On failure both
outputs are 0 and the result FALSUM. The test uses a NON-SQUARE image
(32×16) so a width/height swap shows — the plant that swaps them turns
it red; a square fixture would have hidden it.

## 2026-10-27 - stb_image moved into its own vendor object

imago.c used to compile stb_image inline (`STB_IMAGE_IMPLEMENTATION`
+ `#include "../vendor/stb_image.h"`, warnings hushed with
`#pragma clang diagnostic` under `#ifdef __clang__`). That only works
on clang: stb uses `//` comments, which clang tolerates in -std=c89 as
an extension, but gcc in -std=c89 doesn't treat `//` as a comment at
all, so stb is a parse error there. GCC diagnostic pragmas can't
rescue it. Found by the first full Linux sweep: 18 tests blocked.

Now, like sqlite and bearssl:
- vendor/stb_imago.h - clean C89 declarations of the six stbi_*
  functions imago.c calls (include BEFORE latina.h)
- vendor/stb_imago.c - includes stb_imago.h, then stb_image.h with
  the implementation; built with relaxed vendor flags (aedilis.stml
  <vendor>, compile_tests.sh VENDOR_SOURCES). Including both headers
  in one file means a signature drift is a compile error.
- aedilis picks the .c up by convention (vendor X.h -> X.c), so
  anything that reaches imago through aedilis links it automatically.
  Hand-wired links needed it added: tools/briar_struere.sh now links
  build/stb_imago.o next to build/imago.o.

To use another stb function: add its declaration to stb_imago.h,
copying stb's exact signature.
