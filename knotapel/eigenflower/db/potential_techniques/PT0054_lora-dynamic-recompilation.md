---
id: PT0054
title: "LoRA adapters for dynamic syllogism recompilation"
status: untested
source: ["Early Experience paper (Zhang et al., 2025)"]
applicable_to: []
related: [PT0025, PT0037]
---

Use LoRA (Low-Rank Adaptation) for incremental knowledge updates. Compile base logical rules into main weights. Compile specific theological content as LoRA adapters. To "record a new syllogism," train a small adapter rather than retraining the whole model.

Key property: warm-start with early experience then LoRA reaches HIGHER RL ceilings than starting from imitation learning. New compilations build on old ones rather than overwriting.

The Early Experience paper shows this works at 70B with LoRA. For eigenflower's much smaller model, LoRA adapters would be very small but could still provide meaningful specialization.

To combine domains: merge adapters. To add new content: train new adapter. The separation means the base model's logical reasoning capability is preserved while domain knowledge grows incrementally.

Alternative in the hybrid architecture: LoRA on the fluency layer only. The syllogism layer grows by accretion (adding tensor equations/graph nodes), not by weight modification. This is a cleaner separation.
