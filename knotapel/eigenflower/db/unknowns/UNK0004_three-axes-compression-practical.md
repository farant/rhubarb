---
id: UNK0004
title: "Whether the 'three axes of compression' (24x fewer autoregressive steps) holds in practice"
status: open
source: "notes/01-analysis.md"
related: []
---

Group B's observation that spatial, representational, and structural compression yield 24x fewer autoregressive steps is presented as a useful framing. But it is unclear whether this 24x factor is a theoretical upper bound, an empirical average, or an aspirational target. The claim comes from combining three independent compression ratios, but in practice the three axes may not be independent — compressing along one axis may reduce the benefit of compressing along another. This matters for the performance targets: if 24x compression holds, the model needs far fewer generation steps, directly impacting throughput toward the 70 tok/s target.
