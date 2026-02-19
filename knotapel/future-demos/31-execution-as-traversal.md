# Execution-as-Traversal

**Track:** A — Early Taxonomy/Traversal
**Status:** Proposed
**Smaragda ID:** 01KHFE6WY1AVXPAH07MKB9873V
**Dependencies:** Demos 1-4

## Hypothesis

A point traveling along the braid from the taxonomy compiler demo, multiplied by crossing matrices at each crossing, produces a state vector whose final value correctly classifies inputs according to the taxonomy. Input 'Poodle' should produce output that encodes 'Dog' and 'Animal'. Input 'Siamese' should produce output that encodes 'Cat' and 'Animal'.

## Approach

Build on Demos 1-4. Encode taxonomy entities as small state vectors. Run each entity through the braid's matrix product. Check whether the output correctly reflects the taxonomy relationships. This is the Execution-as-Traversal test — does tangle traversal actually compute taxonomy classification?
