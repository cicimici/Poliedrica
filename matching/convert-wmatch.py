import sys

if len(sys.argv) == 1:
    sys.exit()

f = file(sys.argv[1], 'r').readlines()

vt = int(f[0].split()[0])
e = int(f[0].split()[1])
edges = []

vline = 1
v = 1
while vline < len(f):
    if len(f[vline].split()) == 0: break
    deg = int(f[vline].split()[0])
    for i in range(1, deg+1):
        u = int(f[vline+i].split()[0])
        w = int(f[vline+i].split()[1])
        if v < u:
            edges.append((v, u, w))
    vline += deg + 1
    v += 1

print vt, e
for edge in edges:
    print edge[0], edge[1], edge[2]
