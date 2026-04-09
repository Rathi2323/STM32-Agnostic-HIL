import subprocess
import sys
import platform

binary = "./sim_test.exe" if platform.system() =="Windows" else "./sim_test"

result = subprocess.run([binary], capture_output=True, text=True)
output = result.stdout

print("____Firmware Output____")
#print(output)

checks = [
    ("5 passed, 0 failed", "all C tests passed"),
    ("[PASS] LED ON above threshold", "above threshold test"),
    ("[PASS] LED OFF below threshold", "below threshold test"),
    ("[PASS] LED OFF at boundary", "boundary value test"),
    ("[PASS] Higher Priority IRQ preempts lower priority IRQ", "nested preemption test"),
    ("[PASS] Lower priority IRQ stays pending until ISR exits", "pending lower priority test"),
]

all_ok = True

for expected, description in checks:
    if expected in output:
        print("[OK] " + description)
    else:
        print("[FAIL] " + description)
        all_ok = False
        
sys.exit(0 if all_ok else 1)