import subprocess
import sys

result = subprocess.run(["./sim_test.exe"], capture_output=True, text=True)
output = result.stdout

print("____Firmware Output____")
#print(output)

checks = [
    ("3 passed, 0 failed", "all C tests passed"),
    ("[PASS] sensor above threshold", "above threshold test"),
    ("[PASS] sensor below threshold", "below threshold test"),
    ("[PASS] sensor at boundary", "boundary value test"),
]

all_ok = True

for expected, description in checks:
    if expected in output:
        print("[OK] " + description)
    else:
        print("[FAIL] " + description)
        all_ok = False
        
sys.exit(0 if all_ok else 1)