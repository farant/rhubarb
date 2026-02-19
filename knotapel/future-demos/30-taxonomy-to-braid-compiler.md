# Taxonomy-to-Braid Compiler

**Track:** A — Early Taxonomy/Traversal
**Status:** Proposed
**Smaragda ID:** 01KHFE6WXFFQ8JC4KN35DE77XW
**Dependencies:** Demo 1, Demo 3

## Hypothesis

A simple taxonomy (e.g., Animal > Dog > Poodle, Animal > Cat > Siamese, with properties like 'has_fur', 'barks', 'meows') can be converted into a set of linear logic implications, and those implications can be represented as a braid word where each implication is a crossing.

## Approach

Define a tiny taxonomy as C89 structs (entity-relation-entity triples). Write a compiler that converts each is-a edge into a linear implication, then converts implications into braid crossings. The output is a braid word. Verify the braid word is valid (Yang-Baxter from Demo 3). This is the taxonomy-to-braid-word step of the Compilation Pipeline.
