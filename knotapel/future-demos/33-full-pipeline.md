# Full Pipeline: PD -> Braid -> Spin Chain -> Bracket

**Track:** B — Pipeline Composition
**Status:** Proposed
**Smaragda ID:** 01KHM3RSQRKA2P0Q8C70RPPM51
**Dependencies:** Demo 08 (pd_to_braid), Demo 05 (spin chain R-matrices), Demo 01 (oracle)

## Hypothesis

With Demos 07+08 providing the bidirectional PD<->Braid bridge, and Demos 04-05 providing the tensor product representation for arbitrary strand counts, we can now build the FULL PIPELINE: any PD notation -> braid word (Demo 08) -> R-matrices (Demo 05) -> quantum trace -> Kauffman bracket, all in O(n) per braid word. Test: for trefoil, figure-eight, and Hopf, the full pipeline bracket matches the O(2^n) state-sum oracle from Demo 01.

## Approach

Compose pd_to_braid (Demo 08) with the spin chain representation (Demo 05) and quantum trace formula. The main new code is the composition layer — all the pieces already exist. Verify by running the full pipeline on Demo 01's hand-coded PD codes.
