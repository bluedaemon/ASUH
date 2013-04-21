#Regular Expression Library for parsing
import re
#Paramiko for SSH
import paramiko
import time
from paramiko import SSHClient

#Strings as functions

ASUH_rand='''
def ASUH_rand(N,M):
	return [[random.randrange(1,100) for x in range(N)] for y in range(M)]

'''

ASUH_min='''
import random, Queue, threading
def ASUH_min(m):
#	print m
	q = Queue.PriorityQueue()
	for i in range(0,len(m)):
		tz = threading.Thread(target=a_min,args=(m[i],q))
		tz.start()
	print q.get()

def a_min(a,q):
	q.put(min(a))

'''

#Variable initialization that store variable and data from text file
variable_array=[]
workflow_array=[]
parameter_array=[]
ASUHfuncs=["rand","min"]
function_array=[]
output = open('output.txt','w')

#Parse input file using regular expressions
f = open("b.txt")
for line in f:
	parameter_array.append(re.search('\((.*?)\)',line).group(1))
	variable_array.append(re.search('(.*?)\=',line).group(1))
	workflow_array.append(re.search('\=(.*?)\(',line).group(1))

#Conversion of workflow functions into server computations
for i in xrange(len(workflow_array)):
	if workflow_array[i] in ASUHfuncs:
		function_array.append((variable_array[i]+'='+'ASUH_'+workflow_array[i]+'('+parameter_array[i]+')'))

#Insert your username and password here
#We recommend using password-less login via SSH Keys in order to preserve the security of passwords
user_ = 'outcode'
pass_ ='127001402'
host= 'mphase.rutgers.edu'
port= 22
#SSH Client initialization
ssh_client = SSHClient()
ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
ssh_client.connect(host,username=user_, password=pass_)
transport = paramiko.Transport((host,port))
transport.connect(username=user_,password=pass_)
sftp = paramiko.SFTPClient.from_transport(transport)

#download
#filepath = ''
#localpath= ''
#sftp.get(filepath,localpath)

#upload
#filepath = ''
#localpath= ''
#sftp.put(filepath,localpath)

chan = ssh_client.invoke_shell()
stdin = chan.makefile('wb')
stdout = chan.makefile('rb')

#SSH Client code execution
stdin.write(
#calc=$(eval $load | bc)
'''
#!/bin/sh 
cmd="top -b -n 2 | grep 'Cpu(s)' | tail -1 | grep -o -P '.{0,5}%id' | sed 's/.\{3\}$//'"
load=$(eval $cmd | bc)
echo $load
if [[ $load > 40 ]]
then
openssl enc -aes-256-cbc -salt -in file.txt -out file.enc -pass pass:MySillyPassword
else
echo "Running on the GPU"
fi
exit
''')

#for i in function_array:
	#stdin.write('\n'+i)

#stdin.write('''

#''')

output.write(stdout.read())

#SSH Client code termination
ssh_client.close()
sftp.close()

