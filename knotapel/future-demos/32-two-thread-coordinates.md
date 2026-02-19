# Two-Thread Coordinate System

**Track:** A — Early Taxonomy/Traversal
**Status:** Proposed
**Smaragda ID:** 01KHFE6WYM8YM71KDB7S962PB4
**Dependencies:** None (independent of TL track)

## Hypothesis

Given a 2-thread tangle with known crossings, a point traveling along the tangle has coordinates that interpolate between crossings as predicted by the crossing mechanics from the Milnor conversation — frozen coordinate interpolation. This tests the dimensional mapping: 2 threads = 4D coordinate system, crossings couple the coordinates.

## Approach

Implement the 2-thread coordinate system from the Milnor conversation. Two threads, each with (z, theta). Crossings at specified positions. Point travels along thread 1, crosses to thread 2, continues. Track all 4 coordinates. Verify interpolation between crossings matches the formula: at z2=15 between crossings at {5,10} and {10,20}, z1 should be 7.5.
