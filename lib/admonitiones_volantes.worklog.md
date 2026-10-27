
## 2026-10-22 - host notices (`transientes`)

Second, host-owned index `VolantiumMontatio.transientes`: ONLY the toast
layer reads it, so center, unread badge and sound never see host notices by
construction (no flag every consumer would have to honor). `visibiles` now
returns Admonitio pointers merged newest-first from both indices; a click
on a transient body marks it read without calling the action port; 'x'
dismisses by id as before.
