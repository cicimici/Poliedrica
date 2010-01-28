/* Matching model */

param n, integer;

set V := 1..n;

set E, within V cross V;

param w{(i,j) in E};

var x{(i,j) in E}, binary;

maximize obj: sum{(i,j) in E} w[i,j] * x[i,j];

s.t. deg{i in V}: sum{j in V: (i,j) in E} x[i,j] + sum{j in V: (j,i) in E} x[j,i] <= 1;

end;
