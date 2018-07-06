#!/usr/bin/env python
import subprocess
import os, sys, multiprocessing, traceback
from os.path import join, dirname, basename, realpath, splitext, isdir, isfile
from collections import namedtuple
from PIL import Image

BASE_DIR = dirname(realpath(__file__))
#OUTPUT_DIR = '/opt/for-resize'
OUTPUT_DIR = '/mnt/uray_resized'
INPUT_DIR = '/mnt/20170823_frames'
q = multiprocessing.Queue()

def resize_img(img, save_path, size_tuple):
    img_dir = dirname(img)
    img_base_name = basename(img)
    img_name, img_ext = splitext(img_base_name)
    im = Image.open(img)
    im.resize(size_tuple).save(save_path)

def frame_resize(plock):
    process_name = multiprocessing.current_process().name
    plock.acquire()
    print ">>> Process [ %s ] started!" % process_name
    plock.release()
    while True:
        try:
            frame_dir = q.get(timeout=5)
            plock.acquire()
            print ">>> Process [ %s ] handler [ %s ]!" % (process_name, basename(frame_dir))
            plock.release()

            #new_frame_dir = join(BASE_DIR, "new", basename(frame_dir))
            new_frame_dir = join(OUTPUT_DIR, "new", basename(frame_dir))
            if not isdir(new_frame_dir):
                os.makedirs(new_frame_dir)
                os.makedirs( join(new_frame_dir, 'normal') )
                os.makedirs( join(new_frame_dir, 'zoomin') )

            for class_name in ['normal', 'zoomin']:
                #frames = [join(frame_dir, f) for f in os.listdir(frame_dir) if splitext(f)[1] == '.jpg']
                real_frame_dir = join(frame_dir, class_name);
                frames = [join(real_frame_dir, f) for f in os.listdir(real_frame_dir) if splitext(f)[1] == '.jpg']

                frames.sort()

                for f in frames:
                    save_path = join(new_frame_dir, class_name, basename(f))
                    #resize_img(f, save_path, (320,180))
                    #ffmpeg -i each -vf scale=320:180 save_path
                    #os.system("ffmpeg -i {} -vf scale=320:180 {}".format(each, save_path))
                    command = ['ffmpeg','-i', f, '-vf','scale=320:180', save_path]
                    #ffmpeg = subprocess.Popen(command)
                    #ffmpeg.wait()
                    subprocess.call(command)

        except:
            traceback.print_exc()
            break


plock = multiprocessing.Lock()

def main():
    #camera_list = [d for d in os.listdir(BASE_DIR) if isdir(d) and d.startswith('c1')]
    #camera_list = [d for d in os.listdir(INPUT_DIR) if isdir(d) and d.startswith('c1')]
    camera_list1 = ['c10', 'c11', 'c12', 'c13', 'c14', 'c15', 'c16', 'c17','c18','c19']
    camera_list2 = ['c10-c11-06','c11-c12-06','c12-c13-06','c13-c14-06','c14-c15-06','c15-c16-06','c16-c17-06','c17-c18-06','c18-c19-06']
    camera_list = camera_list1 + camera_list2
    print camera_list
    #camera_list = ['c10']
    camera_list.sort()
    for c in camera_list:
        #c = join(BASE_DIR, c)
        c = join(INPUT_DIR, c)
        q.put(c)

    process_list = []
    for i in range(4):
        p = multiprocessing.Process(target=frame_resize, args=(plock,))
        p.start()

        process_list.append(p)

    for p in process_list:
        p.join()


if __name__ == '__main__':
    main()
