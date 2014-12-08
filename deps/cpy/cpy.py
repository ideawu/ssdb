# encoding=utf-8
#################################
# Author: ideawu
# Link: http://www.ideawu.net/
#################################

import sys, os
import signal
def __sigint__(n, f):
	sys.exit(0)
signal.signal(signal.SIGINT, __sigint__);

def usage():
	print ('Cpy - A C-like scripting language.')
	print ('Copyright (c) 2012 ideawu.com')
	print ('')
	print ('Usage:')
	print ('    cpy source_file')

# 不然管道时报错
reload(sys)
sys.setdefaultencoding('utf-8')

from engine import CpyEngine
cpy = CpyEngine()

if len(sys.argv) < 2:
	usage()
	sys.exit(0)

is_compile = False;
if sys.argv[1] == '-c':
	is_compile = True
	if len(sys.argv) >= 3:
		srcfile = sys.argv[2]
	else:
		usage()
		sys.exit(0)
else:
	srcfile = sys.argv[1]

if not srcfile.endswith('.cpy'):
	srcfile += '.cpy'
if not os.path.exists(srcfile):
	print ("File not found!: " + srcfile)
	sys.exit(0)

base_dir, tail = os.path.split(srcfile)
if len(base_dir) == 0:
	base_dir = '.'

dstfile = cpy.compile(srcfile, base_dir, base_dir + '/_cpy_')

#print ('-----')
#print (''.join(open(dstfile, 'r').readlines()))
#print ('-----')

dstfile = os.path.abspath(dstfile)
sys.path.append(os.path.dirname(os.path.abspath(srcfile)));
sys.path.append(os.path.dirname(os.path.abspath(dstfile)));

os.chdir(os.path.dirname(os.path.abspath(srcfile)));
#print os.getcwd();

if not is_compile:
	sys.argv = sys.argv[1 :]
	sys.path.insert(0, os.path.dirname(dstfile))
	try:
		execfile(dstfile)
	except Exception:
		import traceback
		sys.stderr.write(traceback.format_exc())
		pass
