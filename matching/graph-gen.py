import sys, random

if len(sys.argv) <= 2:
    print 'Syntax: python graph-gen.py [number of vertices] [edge prob.]'
    sys.exit()

v = int(sys.argv[1])
edges = []

for i in range(1, v+1):
    for j in range(i+1, v+1):
        if random.uniform(0, 1) < float(sys.argv[2]):
            edges.append((i, j, random.randint(1, 100)))

print v, len(edges)

for edge in edges:
    print edge[0], edge[1], edge[2]
