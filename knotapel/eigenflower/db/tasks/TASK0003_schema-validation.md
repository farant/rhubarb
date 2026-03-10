---
id: TASK0003
title: "Schema validation and cleanup"
class: quality
trigger: "Periodically, or after large extraction passes"
inputs:
  - "All db records"
  - "Schema files for each table"
method: hybrid
outputs:
  - "Anomaly report (missing fields, invalid values, broken references)"
  - "Fixed records (via LLM cleanup agent)"
related: [TASK0001, TASK0002]
---

# Schema Validation and Cleanup

## Purpose
Check all records against their table's schema. Find anomalies. Fix them — either automatically (script) or with judgment (LLM agent).

## When to Run
- After every large extraction pass (TASK0001)
- After schema evolution (new fields added)
- Periodically as hygiene

## Procedure

### Step 1: Script-based validation
Start with a quick overview via `bun tools/count-records-per-table.ts` to see current table sizes.

A script reads each table's `schema.md`, parses every record's YAML frontmatter, and checks:
- All required fields present
- Enum fields have valid values (e.g., status is one of the allowed values)
- ID format matches `PREFIX####`
- Referenced IDs exist (no dangling references)
- No duplicate IDs within a table

Output: anomaly report listing every violation with file path and field.

### Step 2: LLM cleanup agent
Hand the anomaly report to an agent with instructions:
- "These records have issues. For each one, read the record body, infer intent, and fix the frontmatter."
- Missing fields: infer from body text where possible, flag for manual review if not
- Invalid enum values: map to closest valid value
- Broken references: check if the reference was renamed or if it's genuinely wrong
- Duplicates: identify which record is more complete, merge if possible

### Step 3: Post-cleanup verification
Re-run the validation script to confirm all anomalies resolved. Use `bun tools/count-records-per-table.ts` to verify record counts haven't changed unexpectedly (no accidental deletions or duplications).

## Future: Schema Evolution
When a new field is added to a schema:
1. Update `schema.md`
2. Run validation — every existing record will flag the new field as missing
3. Run LLM agent to populate the new field across all existing records from context
4. Re-validate

This is the mechanism that makes the taxonomy evolvable without fear.
