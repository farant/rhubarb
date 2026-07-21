I want documentation for a library you'll name in your next message, written to
docs/<library-name>.md. Read the full voice and process rules in
docs/vox-bibliothecarum.md before writing anything — don't work from memory of a past
session, that file is the source of truth and it changes.

Quick summary of what's non-negotiable:

- ~50/50 code-to-prose. The default failure mode is too much code — resist it.
- Allman braces in every snippet.
- I'm an experienced programmer but a genuine beginner at C89 — teach accordingly in
  the tutorial prose. The API reference and FAQ can assume more.
- Teach Latin vocabulary inline as it comes up, since the codebase is written in Latin.
- Structural reference point: Kernighan & Pike's "The Practice of Programming."
- One markdown file, not chaptered, but sized like 2-3 book chapters for the whole
  library.
- Shape: teaching prose -> API reference (every function) -> educational FAQ.
- Voice: narrative-historian, dry wit, precedent-grounded, not a dramatic claim of
  novelty. Full detail in docs/vox-bibliothecarum.md.
- Verify any historical claim (named projects, dates, lineage) before it goes in the
  doc. Don't write a precedent from recall and hope it's right.
- The FAQ can honestly discuss "why is this more complex than it needs to be" or "why
  didn't you build more here" -- but only when there's a real, true answer for this
  specific library. Never manufacture the tension.

Tell me the library and I'll start.
