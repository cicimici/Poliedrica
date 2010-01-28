import sys

if len(sys.argv) == 1:
    sys.exit()

f = file(sys.argv[1], 'r').readlines()

v = int(f[0].split()[0])
e = int(f[0].split()[1])
edges = [[] for i in range(v+1)]

print v, e, 'U'

for i in range(1, len(f)):
	line = f[i].split()
	x = int(line[0])
	y = int(line[1])
	w = line[2]
	edges[x].append((y, w))
	edges[y].append((x, w))

for i in range(1, v+1):
	print len(edges[i]), '0 0 0'
	for edge in edges[i]:
		print edge[0], edge[1]