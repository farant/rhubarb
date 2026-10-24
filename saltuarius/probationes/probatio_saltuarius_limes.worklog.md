# probatio_saltuarius_limes worklog

## 2026-10-23 — fixture tree moved from /tmp into the credo area (park …BPYFX)

**What broke.** The fixture tree lived at `/tmp/saltuarius_limes_fixum`, so the parent column listed /tmp itself. Its index holds at most `SALT_INTROITUS_MAXIMI` (4096) entries. Once /tmp held more than that (3,884 leaked mensor diaries plus everything else), the fixture's entry fell off the index. The parent then had no selection, and frigida went red at "indices ex disco" (parens selectum == "saltuarius_limes_fixum").

**The fix.** Both trees (fixum, imagines) now go under `credo_area(c)`. The parent column lists only our own directory, parallel runs don't clobber each other, and credo deletes the area after the section. Paths are built at run time through `_aperire` / `_mandare`.

**A/B under a crowded TMPDIR** (5,000 empty files, binary run directly):
- the new layout is green, and TMPDIR is unchanged afterwards (the area cleaned up);
- the old layout, planted back as `$TMPDIR/saltuarius_limes_fixum`, is red at the same assertion frigida hit (line 162, "indices ex disco"). Restored with cp + touch, green.

**Examen along the way.**
- Fixture paths first went in `hic_manens` buffers. Examen's credo rule refused them: written in a section, reset by neither parare nor purgare, so the state survives between sections. They are now locals of the section, passed to `_arborem_figere(fixum)`.
- Every write goes through one helper, `_aperire` → `lectiones_fopen`, so the file has no raw IO left. Its legacy line in `silva/probationes/fixa/examinis/canalis_debita.tsv` ("migratio consulta") was deleted, as the ratchet requires.
