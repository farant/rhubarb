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
