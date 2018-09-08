import subprocess

obj = [{"numpoints" : 10000000, "numthreads" : 2, "wordsize" : 1, "levels" : 3},
{"numpoints" : 10000000, "numthreads" : 2, "wordsize" : 1, "levels" : 4},
{"numpoints" : 10000000, "numthreads" : 2, "wordsize" : 1, "levels" : 5},
{"numpoints" : 10000000, "numthreads" : 2, "wordsize" : 1, "levels" : 6},
{"numpoints" : 10000000, "numthreads" : 3, "wordsize" : 1, "levels" : 3},
{"numpoints" : 10000000, "numthreads" : 3, "wordsize" : 1, "levels" : 4},
{"numpoints" : 10000000, "numthreads" : 4, "wordsize" : 1, "levels" : 2},
{"numpoints" : 10000000, "numthreads" : 4, "wordsize" : 1, "levels" : 3},
{"numpoints" : 10000000, "numthreads" : 5, "wordsize" : 1, "levels" : 2},
]
for pars in obj:
    subprocess.call(["../cmake-build-debug/OctreeSort", "-p", str(pars["numpoints"]), "-t", str(pars["numthreads"]), "-w", str(pars["wordsize"]), "-l", str(pars["levels"])])