set xlabel "n"
set ylabel ""

set title "Time"
block = 0
plot "all-data" every :::0::0 u 1:4 title 'LP, p = 0.0' w linespoints lt 1 pt 3, \
     "all-data" every :::0::0 u 1:5 title 'Comb., p = 0.0' w linespoints lt 1 pt 4, \
     "all-data" every :::1::1 u 1:4 title 'LP, p = 0.25' w linespoints lt 2 pt 3, \
     "all-data" every :::1::1 u 1:5 title 'Comb., p = 0.25' w linespoints lt 2 pt 4, \
     "all-data" every :::2::2 u 1:4 title 'LP, p = 0.5' w linespoints lt 3 pt 3, \
     "all-data" every :::2::2 u 1:5 title 'Comb., p = 0.5' w linespoints lt 3 pt 4
pause -1

set title "Number of cuts"
plot "all-data" every :::0::0 u 1:6 title 'p = 0.0' w linespoints, \
     "all-data" every :::1::1 u 1:6 title 'p = 0.25' w linespoints, \
     "all-data" every :::2::2 u 1:6 title 'p = 0.5' w linespoints
pause -1

set title "Number of nodes"
plot "all-data" every :::0::0 u 1:7 title 'p = 0.0' w linespoints, \
     "all-data" every :::1::1 u 1:7 title 'p = 0.25' w linespoints, \
     "all-data" every :::2::2 u 1:7 title 'p = 0.5' w linespoints
pause -1

set title "Solution"
plot "all-data" every :::0::0 u 1:3 title 'p = 0.0' w linespoints, \
     "all-data" every :::1::1 u 1:3 title 'p = 0.25' w linespoints, \
     "all-data" every :::2::2 u 1:3 title 'p = 0.5' w linespoints
pause -1
