set xlabel "n"
set ylabel ""

set title "Time"
block = 0
plot "all-data" every :::3::3 u 1:4 title 'LP, p = lg(n)/n' w linespoints lt 1 pt 3, \
     "all-data" every :::3::3 u 1:5 title 'Comb., p = lg(n)/n' w linespoints lt 1 pt 4, \
     "all-data" every :::4::4 u 1:4 title 'LP, p = 0.1' w linespoints lt 2 pt 3, \
     "all-data" every :::4::4 u 1:5 title 'Comb., p = 0.1' w linespoints lt 2 pt 4, \
     "all-data" every :::5::5 u 1:4 title 'LP, p = 0.5' w linespoints lt 3 pt 3, \
     "all-data" every :::5::5 u 1:5 title 'Comb., p = 0.5' w linespoints lt 3 pt 4, \
     "all-data" every :::6::6 u 1:4 title 'LP, p = 1.0' w linespoints lt 5 pt 3, \
     "all-data" every :::6::6 u 1:5 title 'Comb., p = 1.0' w linespoints lt 5 pt 4
pause -1

set title "Number of cuts"
plot "all-data" every :::3::3 u 1:6 title 'p = lg(n)/n' w linespoints lt 1, \
     "all-data" every :::4::4 u 1:6 title 'p = 0.1' w linespoints lt 2, \
     "all-data" every :::5::5 u 1:6 title 'p = 0.5' w linespoints lt 3, \
     "all-data" every :::6::6 u 1:6 title 'p = 1.0' w linespoints lt 5
pause -1

set title "Number of nodes"
plot "all-data" every :::3::3 u 1:7 title 'p = lg(n)/n' w linespoints lt 1, \
     "all-data" every :::4::4 u 1:7 title 'p = 0.1' w linespoints lt 2, \
     "all-data" every :::5::5 u 1:7 title 'p = 0.5' w linespoints lt 3, \
     "all-data" every :::6::6 u 1:7 title 'p = 1.0' w linespoints lt 5
pause -1

set title "Solution"
plot "all-data" every :::3::3 u 1:3 title 'p = lg(n)/n' w linespoints lt 1, \
     "all-data" every :::4::4 u 1:3 title 'p = 0.1' w linespoints lt 2, \
     "all-data" every :::5::5 u 1:3 title 'p = 0.5' w linespoints lt 3, \
     "all-data" every :::6::6 u 1:3 title 'p = 1.0' w linespoints lt 5

pause -1
