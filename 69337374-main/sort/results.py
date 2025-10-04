import subprocess

txtfiles = [
    "random5000", "random10000", "random50000", "reversed5000", "reversed10000",
    "reversed50000", "sorted5000", "sorted10000", "sorted50000"
]
programs = ["sort1", "sort2", "sort3"]

results = open("./results.txt", "ab")

for p in programs:
    for t in txtfiles:
        info = f'echo "./{p} {t}.txt" >> results.txt'
        command = f"{{ time ./{p} {t}.txt >/dev/null ; }} 2>>results.txt"
        barrier = f'echo "################################################################################" >> results.txt'

        infop = subprocess.run(["/bin/bash", "-c", info])
        commandp = subprocess.run(["/bin/bash", "-c", command])
        barrierp = subprocess.run(["/bin/bash", "-c", barrier])

results.close()
