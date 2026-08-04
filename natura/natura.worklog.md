# natura worklog — the transformation record

This worklog is load-bearing differently than the code worklogs:
per the Alexander method, the artifact grows by structure-
preserving transformations, and THIS is where each transformation
and its reason is recorded. The models say what is true; this
file says how we came to say it.

## 2026-08-04 — birth: lingua_programmandi v0 → v1 in one day

Founding decisions (conversation with Fran, on the ledger res
01KZ6TNNAM):
- Discipline is PIEPER: describe things as they are, not as apps
  want them — properties recorded whether useful or not. A
  use-case schema is flattery and breaks at use case N+1; truth
  doesn't rot.
- Shape is LINNAEUS: taxonomy + dictionary in one artifact —
  the form of the genus AND the described species/individuals.
- Process is ALEXANDER: hand-crafted, discussed, format learned
  from content, plastic for a long time. Not mechanical.
- Latin is load-bearing here, not decorative: a semantic library
  is the universal-language application (Linnaeus's own reason).
- Rule of residence: the library holds what is timelessly
  describable (kinds at every rank + notable individuals of
  public record); YOUR individuals live in app volumina and cite
  the library.

v0 → v1 transformations (each forced by content, none planned):
1. "compiled/interpreted" DIED as a language property within the
   first hour — it belongs to implementations (officina interprets
   C). The commonest schema lie in this domain, caught by the
   discipline. → genus implementatio_linguae, where
   modus_exsecutionis truthfully lives, plus
   tutela_memoriae_addita (Fil-C: implementation-granted safety
   the language never promised).
2. Rust bent the gestio_memoriae enum mid-draft: per_possessionem
   is a third thing, neither manual nor collected. Recorded in a
   comment at the option itself.
3. The c89 dubium (varietas or edition?) RESOLVED by Fran's
   versio_linguae genus: an edition is an EVENT crystallized into
   a citable thing — the exact shape of a conditio in the volumen
   (named point in the log). c89/c99/c11 are individua of
   versio_linguae, chained by 'sequitur'.
4. auctores-as-textus died: Dennis Ritchie is a persona, not a
   string. → natura/persona.stml (Boethius' definitio for the
   genus), with the format's species/individuum distinction made
   EXPLICIT: C is a species; Ritchie and c89 are individua.
5. normae-as-textus died the same death → natura/norma.stml:
   norma has an editor, a date, a state machine
   (proposita/vigens/substituta/retracta — old standards don't
   vanish, C89 is our witness), and 'substituit' chains.
6. HISTORICAL LAYER (Fran's pull, shape found): a thing's history
   is its event log — <historia> holds dated <eventum> entries;
   current status is a FOLD of the historia. The storage-layer
   doctrine (acta = truth, state = projection) recurring one
   level up, unforced. annus_ortus migrated from property to
   first historia event.

Open dubia carried IN the artifacts (deliberately — honest
descriptions carry their unresolved tensions): bash's genus
(lingua_iussuum? a wider lingua_formalis?), organizatio surfacing
from norma.editor and persona (ISO is not a persona), POSIX's
continuous-renewal identity (same puzzle as language versions —
the versio solution may generalize).

Format so far: <natura modulus> → <genus> → <definitio> (dictionary
prose), <proprietates> (typed via the genera palette, options
inline), <machina_statuum> (+ named transitus), <actiones>,
<relationes> (modulus= for cross-model, externum= for
not-yet-described, ad="*" for open relations), then <species> and
<individuum> with <valor>/<relatum>/<historia>/<dubium>/<nota>.
Everything provisional; the next models (plagula_computatralis,
systema_operativum) will bend it again.
