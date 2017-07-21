#!/usr/bin/python
#filename: draw.py

import string
import sys
import os
import time
import tempfile

#print sys.argv[0]
#print sys.argv[1]
#print sys.argv[2]
######################PARAMETERS########################
tools0='./resamplevdna'
tools1='./2dgraph'
tools2='./fixedoffset'
#fps = 6
#seg_dur = 2

t_b = time.time()

image_path = sys.argv[3]
#print os.path.splitext(image_path)[1]
if os.path.splitext(image_path)[1] != '.png':
    image_path+='.png'

#print image_path

#tmpdir=tempfile.mkdtemp()
tmpdir='.'
#t0 = time.time()

os.system(tools0+' '+sys.argv[1]+' '+tmpdir+'/m.dna')
os.system(tools0+' '+sys.argv[2]+' '+tmpdir+'/s.dna')

if not os.path.exists(tmpdir+'/m.dna') or not os.path.exists(tmpdir+'/s.dna'):
    exit(1)

#print time.time() - t0,"sec resamplevdna"
#t0 = time.time()
#print tmpdir

os.system(tools1+' '+tmpdir+'/m.dna'+' '+tmpdir+'/s.dna >'+tmpdir+'/2dgraph.dat')

#print time.time() - t0,"sec matching to get 2dgraph.dat"
#t0 = time.time()

f = file(tmpdir+'/2dgraph.dat','r')
f0 = file(tmpdir+'/100-94', 'w')
f1 = file(tmpdir+'/93-87', 'w')
f2 = file(tmpdir+'/86-80', 'w')
f3 = file(tmpdir+'/79-73', 'w')
list = []

for line in f.readlines():
    list = line.split()
    score = string.atoi(list[2])
    if score >= 94:
        print>>f0, list[0],list[1]
    elif score >= 87:
        print>>f1, list[0],list[1]
    elif score >= 80:
        print>>f2, list[0],list[1]
    elif score >= 73:
        print>>f3, list[0],list[1]

f.close()
f0.close()
f1.close()
f2.close()
f3.close()

#print time.time() - t0,"sec split 2dgraph.dat to 4 files"
#t0 = time.time()

#os.system(tools2+' '+tmpdir+'/2dgraph.dat '+list[0]+' '+list[1]+' >'+tmpdir+'/1dgraph.dat')
os.system(tools2+' '+tmpdir+'/2dgraph.dat '+list[0]+' '+list[1]+' '+tmpdir+'/1dgraph.dat')

#print time.time() - t0,"sec 1dgraph.dat"
#t0 = time.time()

arr=[]
arr.append("set terminal png size 1600, 2000\n")
arr.append("set output \""+image_path+"\"\n")
arr.append("set multiplot layout 2,1\n")
arr.append("plot \""+tmpdir+"/79-73\" using 1:2 with points pt 7 ps 0.5 lc rgbcolor '#0000ff' title \"73% - 79%\", ")
arr.append("\""+tmpdir+"/86-80\" using 1:2 with points pt 7 ps 0.8 lc rgbcolor '#00ffff' title \"80% - 86%\", ")
arr.append("\""+tmpdir+"/93-87\" using 1:2 with points pt 7 ps 1.2 lc rgbcolor '#ffa500' title \"87% - 93%\", ")
arr.append("\""+tmpdir+"/100-94\" using 1:2 with points pt 7 ps 1.7 lc rgbcolor '#ff0000' title \"94% - 100%\"\n")
arr.append("plot \""+tmpdir+"/1dgraph.dat\" using 1:2 with lp lw 1.2 pt 7 ps 0.8 lc rgbcolor '#ff0000' title \"fixed-offset score\"\n")
arr.append("unset multiplot\n")

gnuf = file(tmpdir+'/tmp.gnu', 'w')
#gnuf = file('tmp.gnu', 'w')
gnuf.writelines(arr)
gnuf.close()

#os.system('gnuplot tmp.gnu')
os.system('gnuplot '+tmpdir+'/tmp.gnu')

#print time.time() - t0,"sec draw graph"
print "total ",time.time() - t_b,"sec"
