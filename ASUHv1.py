import re
import paramiko
from paramiko import SSHClient

ASUH_rand='''
def ASUH_rand(N,M):
	return [[random.randrange(1,100) for x in range(N)] for y in range(M)]

'''

ASUH_min='''
import random, Queue, threading
def ASUH_min(m):
	print m
	q = Queue.PriorityQueue()
	for i in range(0,len(m)):
		tz = threading.Thread(target=a_min,args=(m[i],q))
		tz.start()
	print q.get()

def a_min(a,q):
	q.put(min(a))

'''

#variable initialization
variable_array=[]
function_array=[]
parameter_array=[]
ASUHfuncs=["rand","min"]
temp=[]

#parse input file
f = open("b.txt")
for line in f:
	parameter_array.append(re.search('\((.*?)\)',line).group(1))
	variable_array.append(re.search('(.*?)\=',line).group(1))
	function_array.append(re.search('\=(.*?)\(',line).group(1))

#execute valid ASUH functions
for i in xrange(len(function_array)):
	if function_array[i] in ASUHfuncs:
		temp.append((variable_array[i]+'='+'ASUH_'+function_array[i]+'('+parameter_array[i]+')'))
user_ = 'jl'
pass_ = 'Sprite123'

ssh_client = SSHClient()
ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
ssh_client.connect('twentyseven.info',username=user_, password=pass_)
chan = ssh_client.invoke_shell()
stdin = chan.makefile('wb')
stdout = chan.makefile('rb')
stdin.write(
'''
python
'''
+ASUH_rand+ASUH_min)
for i in temp:
	stdin.write('\n'+i)
stdin.write('''
exit()
exit
''')
print stdout.read()
ssh_client.close()
