# http2_forma worklog

## 2026-10-31 - T4: frame codec (http2-plan T4)

Pure codec for RFC 9113 frames: 9-octet header, all ten types, padding (DATA/HEADERS/PUSH_PROMISE), HEADERS priority fields stripped, validation of section 6 reported as `Http2Iudicium` (code + stream-vs-connection) - the codec judges, the connection acts.

- **Oversized frames** (above our SETTINGS_MAX_FRAME_SIZE) return just the 9 header octets with a connection FRAME_SIZE_ERROR, without waiting for the payload: the connection dies anyway, and waiting would let a peer make us buffer 16 MiB.
- **SETTINGS values are judged here too** (6.5.2: ENABLE_PUSH > 1 and MAX_FRAME_SIZE outside 16384..2^24-1 -> PROTOCOL_ERROR, INITIAL_WINDOW_SIZE > 2^31-1 -> FLOW_CONTROL_ERROR); unknown ids are ignored. Not in the plan's test list, cheap to add.
- **Stream vs connection:** PRIORITY of wrong length and WINDOW_UPDATE with increment 0 on a stream are stream errors; everything else in section 6 is a connection error.
- Padding rule: after the pad-length octet, padding > remaining payload (i.e. >= the whole payload) is PROTOCOL_ERROR; padding equal to the remainder (empty data) is legal.
- A test bug cost one red: a HEADERS frame declared 11 octets of payload but carried 10 - the extra uninitialised octet showed up as header block.
