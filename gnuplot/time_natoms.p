set title "TIMES USING DIFFERENT N° OF ATOMS\n{LOCAL PC}"
#set title "Big Title\n{/*0.5 subtitle}"
#set label "Subtitle" at screen 0.5, 0.9 font "Arial,8"
set xlabel "N optimization [-]"
set ylabel "Time [s]"
#   set format y "%.2f"
#set logscale
#set xrange [1:1000]
#set yrange [:2]
#set ytics 2
#set style line 1  lc rgb '#FFD700' lt 2 lw 2.0 pt 2
set style line 2  lc rgb '#FF0000' lt 2 lw 2.0 pt 4
#set style line 3  lc rgb '#0000FF' lt 2 lw 2.0 pt 6
   plot "time_natoms.dat" u 1:2 t '' w linespoints ls 2,\
   
set terminal png
set output 'time_natoms.png'
rep
unset terminal
#-------------------------------------------------------------------------------------------------------------------
# COLORS '#...'
#https://www2.uni-hamburg.de/Wiss/FB/15/Sustainability/schneider/gnuplot/colors.htm
# yellow FFD700
# red FF0000
# blue 0000FF

# green 008000
# pink FF69B4
# fucsia FF1493
# violet 9400D3
# light blue 1E90FF

# PREVIOUS COLOURS
# green   009e73
# orange  e69f00
# blue    0072b2
#-------------------------------------------------------------------------------------------------------------------
# SYMBOLS (pt)
# crosses  2
# squares   4
# circles  6
#-------------------------------------------------------------------------------------------------------------------
#GNUPLOT TICS
#http://lowrank.net/gnuplot/tics-e.html
