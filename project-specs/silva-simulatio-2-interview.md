# Silva Simulatio ② — Post-Simulation Interview (Raw Q&A)

*2026-07-02. Interview on the open questions from silva-simulatio-2.md
(conditional sub-parse mechanics) plus sign-off on the sim's central mechanism.*

---

## Q: Sign-off on the central mechanism pair: (1) two-track semantics — cpp-faithful evaluated path drives live expansion state while journal + region tree retain every arm; (2) arms sub-parse from the enclosing GSS state and must reconverge to a common landing state (divergent → bounded downstream forks; imbalanced → opaque fringe). Accept as the conditional architecture?

**A:** Accept both. Census-backed: default track matches what a real cpp computes
(canonical parses agree with reality), retention costs ~nothing (partitioned
tokens), and the GSS-state contract makes "localized" operationally exact with a
measured-safe degradation ladder.

## Q: Configuration API: queries take atom assignments, wildcard-across-all-arms default. Named configurations?

**A:** Named presets in contextus. User-defined named configurations (e.g.
"macos", "c89-nudum") = saved atom assignments registered on SilvaContextus,
reusable across queries and serializable with project config. Pseudo-configs like
all-undefined fall out as trivially definable presets. Solarium gets
configurations as data.

## Q: Include guards (est_custos): how does a detected guard appear in the tree?

**A:** Transparent + file metadata. The guarded body parses as unconditional
structure; the guard's three directives remain as plain directive nodes
(roundtrip unaffected); guard-ness is recorded on the file/fons record with
pointers to those nodes. Consumers see the file unwrapped — no skip rule needed
anywhere — and "is this file guarded?" stays queryable.

## Q: False-arm includes (S40, 13 measured sites): resolution timing?

**A:** Lazy default + eager opt-in. First configuration-scoped query touching a
dormant include pays its processing cost; a contextus flag ("resolvere omnes
configurationes") pre-processes every arm's includes for index-building
workloads — which the cross-configuration rename/audit flagship use case will
genuinely need.
