#!/bin/bash
lineplot()
{
    gnuplot -persist <<EOF
    set term png
    set output "${outImg}"
    set xlabel "Sample"
    set ylabel "Master"
    set grid
    set key box
    set key Left
    set style line 1 lw 1 lc rgb "#FF0000" pt -1
    plot "$data" using 1:2 with points pt 7 ps 0.8 lc rgb "#000000" t "src point", "$linelog" u 1:2 w lp ls 1 pt 6 ps 1 t " match line"
    quit
EOF
}
realpath()
{
    local TARGET_FILE=$1
    cd $(dirname "$TARGET_FILE")
    TARGET_FILE=$(basename "$TARGET_FILE")
    while [ -L "$TARGET_FILE"  ]
    do
        TARGET_FILE=$(readlink "$TARGET_FILE")
        cd $(dirname "$TARGET_FILE")
        TARGET_FILE=$(basename "$TARGET_FILE")
    done
    echo "$(pwd -P)/$TARGET_FILE"
}
REALPATH=$(realpath "$0")
BASEDIR=$(dirname $REALPATH)

linelog=$(mktemp)
if [ $# -eq 2 ]
then
	data=$1;
    outImg=$2;
	$BASEDIR/showline $data $linelog
else
	echo "====================================================================================="
	echo "          * error: need 2 para-> data.txt outImgName *"
	echo "====================================================================================="
fi

if [ ! -s $linelog ]
then
    exit -1
else
    lineplot
fi
rm $linelog
#echo "The draw result is in the file: $outImg"
