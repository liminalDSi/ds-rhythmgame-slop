# Neon / Pulse

A small four-lane Nintendo DS rhythm game built in C with libnds. Notes scroll on the top screen; the touch screen shows score, combo, judgments, and results. Includes a deterministic 96-note, 120 BPM chart and synthesized audio; no external assets or copyrighted music required.

## Play

Press **START** to begin. Hit **Left**, **Down**, **X**, or **A** as the note in its lane crosses the bright line. START pauses/resumes and restarts after the results screen. Perfect hits earn 1,000 points within 3 frames; good hits earn 500 within 7 frames. Empty hits break your combo. Timing uses the DS VBlank clock (approximately 60 Hz).

## Build a DS ROM

Install the official [devkitPro toolchain](https://devkitpro.org/wiki/Getting_Started), including the `nds-dev` package group. With `DEVKITPRO` and `DEVKITARM` configured by devkitPro:

```sh
make
```

Alternatively, build from the repository directory using the verified devkitPro Docker image (no local SDK installation needed):

```sh
docker run --rm -v "$PWD:/project" -w /project devkitpro/devkitarm@sha256:116afba8df8453961de2936ffab20dd441edf4d682856c1ec8b0e53d7ed0bbf5 make -j2
```

This produces `neon-pulse.nds`. Open it in melonDS or another DS emulator, or copy it to a compatible homebrew flashcart for DS hardware. No BIOS or commercial game assets are included. `make clean` removes build outputs.

## Test game logic without the DS SDK

```sh
cc -std=c99 -Wall -Wextra -Werror -Isource source/rhythm.c tests/rhythm_test.c -o /tmp/neon-pulse-test
/tmp/neon-pulse-test
```

The portable tests cover judgment boundaries, duplicate hits, score, missed notes, and a complete perfect chart. ROM compilation and audiovisual behavior require devkitPro and a DS/emulator respectively.
