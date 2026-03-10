# Tasks (TASK) Schema

Reusable process templates — not one-off todos but classes of work that get run repeatedly.

## Frontmatter Fields

| Field | Required | Type | Description |
|-------|----------|------|-------------|
| id | yes | string | `TASK####` — unique identifier |
| title | yes | string | Task name |
| class | yes | enum | `extract` / `quality` / `aggregate` |
| trigger | no | string | When to run this task (e.g., "after every experiment", "weekly", "on demand") |
| inputs | no | list | What this task needs (e.g., "new experiment results", "all Q records", "all MOD records") |
| outputs | no | list | What this task produces (e.g., "new Q/UNK records", "updated related fields", "ripeness report") |
| method | yes | enum | `agent_fan_out` / `single_agent` / `script` / `manual` / `hybrid` |
| agent_count | no | number | How many parallel agents (if method involves agents) |
| script_path | no | string | Path to script (if method is `script` or `hybrid`) |
| related | no | list | IDs of related records |

## Body
Full description of the task: what it does, step-by-step procedure, agent prompts or script logic, expected outputs, quality criteria.
