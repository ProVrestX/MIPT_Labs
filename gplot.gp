set terminal png size 800,600

set xl "U, В"
set yl "I, мА"
set grid

set output "plot.png"
plot "data.txt" w lp pt 7 lw 2 ps 0.7 not
set output