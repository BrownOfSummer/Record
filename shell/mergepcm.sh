#!/bin/bash
if [ $# -eq 3 ]
then
    pcm1=$1
    pcm2=$2
    out_name=$3
    cat ${pcm1} ${pcm2} > ${out_name}
    echo "Merged ${pcm1} And ${pcm2} ..."
    echo "result in ${out_name}"
elif [ $# -eq 5 ]
then
    pcm1=$1
    pcm2=$2
    end1=$3
    start2=$4
    out_name=$5
    size1=$[ (${end1})*16000  ]
    head -c ${size1} ${pcm1} > tmp1
    size2=`wc -c "${pcm2}" | awk '{print $1}'`
    size3=$[ ${size2}-(${start2}*16000) ]
    tail -c ${size3} ${pcm2} > tmp2
    cat tmp1 tmp2 > ${out_name}
    rm tmp1 tmp2
    echo "Merged ${pcm1} And ${pcm2} ..."
    echo "${pcm1} from 0s ~ ${end1}s;"
    echo "${pcm2} from ${start2}s ~ end;"
    echo "result in ${out_name}"
elif [ $# -eq 7 ]
then
    echo "args = $#"
    pcm1=$1
    pcm2=$2
    start1=$3
    end1=$4
    start2=$5
    end2=$6
    out_name=$7
    size_pcm1=$[ (${end1}-${start1})*16000 ]
    size_pcm2=$[ (${end2}-${start2})*16000 ]
    head -c $[ ($end1)*16000 ] ${pcm1} > tmp_pcm1
    tail -c ${size_pcm1} tmp_pcm1 > tmp1
    head -c $[ ($end2)*16000 ] ${pcm2} > tmp_pcm2
    tail -c ${size_pcm2} ${pcm2} > tmp2
    cat tmp1 tmp2 > ${out_name}

    rm tmp1 tmp2 tmp_pcm1 tmp_pcm2
    echo "Merged ${pcm1} And ${pcm2} ..."
    echo "${pcm1} from ${start1}s ~ ${end1}s;"
    echo "${pcm2} from ${start2}s ~ ${end2}s;"
    echo "result in ${out_name}"
    #echo "$size_pcm1, $size_pcm2"
else
    echo "input file number error !"
    echo "need to input 3 or 5 or 7 paras"
fi
#usage
# (1) ./mergepcm.sh f1.pcm f2.pcm outname.pcm
# (2) ./mergepcm.sh f1.pcm f2.pcm f1_endtime f2_starttime outname.pcm
# (3) ./mergepcm.sh f1.pcm f2.pcm f1_starttime f1_endtime f2_starttime f2_endtime outname.pcm

