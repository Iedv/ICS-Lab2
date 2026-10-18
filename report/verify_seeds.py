#!/usr/bin/env python3
"""Randomized regression check for the BombLab `defuser`.

For every random seed that is NOT in tests/public.json, run the full flow:
  ./defuser --seed S | ./bomb --seed S            (six phases)
  { ./defuser --seed S; ./defuser --secret --seed S; } | ./bomb --seed S   (Honor)
and require that all phases defuse and the secret phase is reached.

Usage:  python3 report/verify_seeds.py [count]      (default 200)

This helper only *reads* / runs the built binaries; it never modifies them.
"""
import json
import random
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent          # repo root
PUBLIC = set(json.loads((ROOT / "tests" / "public.json").read_text())["seeds"])
TIMEOUT = 15


def run(args, data=b""):
    return subprocess.run(
        [str(a) for a in args], cwd=ROOT, input=data,
        capture_output=True, timeout=TIMEOUT,
    )


def check_seed(seed):
    base = ["--seed", str(seed)]
    six = run([ROOT / "defuser", *base])
    if six.returncode != 0:
        return "defuser_six_failed"
    prefix = six.stdout
    if len(prefix.splitlines()) != 6:
        return "defuser_wrong_line_count"
    bomb = run([ROOT / "bomb", *base], prefix)
    if bomb.returncode != 0:
        for phase, line in enumerate(prefix.splitlines(), start=1):
            if run([ROOT / "bomb", *base, "--phase", str(phase)], line + b"\n").returncode:
                return f"phase_{phase}_wrong"
        return "bomb_six_failed"
    secret = run([ROOT / "defuser", "--secret", *base])
    if secret.returncode != 0:
        return "defuser_secret_failed"
    bomb = run([ROOT / "bomb", *base], prefix + secret.stdout)
    if bomb.returncode != 0:
        return "wrong_answer"
    if b"Secret phase defused." not in bomb.stdout.splitlines():
        return "secret_not_reached"
    return None


def main(count):
    rng = random.Random(0xC0FFEE)
    failures = []
    done = 0
    while done < count:
        seed = rng.getrandbits(64) if rng.random() >= 0.4 else rng.randrange(1 << 20)
        if seed in PUBLIC:
            continue
        done += 1
        error = check_seed(seed)
        if error:
            failures.append((seed, error))
            print(f"FAIL seed={seed}: {error}")
        if done % 50 == 0:
            print(f"... {done}/{count} done, failures={len(failures)}")
    print(f"\nTOTAL {count} non-public seeds, failures = {len(failures)}")
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main(int(sys.argv[1]) if len(sys.argv) > 1 else 200))
