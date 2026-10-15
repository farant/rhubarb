# dispositio worklog

## 2026-10-13 — dispositio_ad_pixela (shared cells -> pixels)

scriba_componentia (`ad_pixela`) and pictor_componentia (`_ad_pixela`)
had byte-identical private copies of the cells -> pixels conversion
(edges at cell multiples, EXCEPT an edge touching the surface, which
goes to the surface itself - window surfaces are not cell multiples).
The calendarium (agenda-plan V0b) would have been the third, so it is
now `dispositio_ad_pixela` here; both copies deleted.
Plant (remainder ignored) -> red in probatio_dispositio ONLY: the scriba
and pictor suites use surfaces that are exact cell multiples, so they
cannot see the remainder rule - the dedicated test is its only guard.
