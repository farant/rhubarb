
## 2026-10-28 — growth re-places by cached hash (budget B1 C4)

`_tabula_dispersa_crescere` re-inserted every live slot through the
public `tabula_dispersa_inserere`: re-hash through the friatio pointer,
load-factor float test, probe with key comparisons. Growth was 37–41% of
all insert time in the silva profiles. In the fresh table every key is
distinct and there are no tombstones, so inserere only walks from
`hash & mask` past occupied slots to the first empty one, counting
collisions and distance. Now the loop does exactly that with
`hash_cachatum` and copies the whole slot — same slot, so iteration order
and the statistics are unchanged. Proved by a differential harness (old
vs new tabula_dispersa.c, 200k inserts with deletes and overwrites,
iteration + stats dumped): 174,028 lines byte-identical. Wall time in that
bench is cache-miss bound and barely moves; the win is the skipped
hashing/compares visible in the profiles. probatio_tabula_dispersa 22/22,
probatio_internamentum 9/9; plant (hash ^ 1 during re-placement) → :277
lookup red. Amalgams regenerated.
