import re
import os
import getpass
import paramiko
from paramiko import SSHClient

#TODO REMOVE ALL THE OUTPUT PRINT STATEMENTS AND REPLACE BY FILE OUTPUT

output = open('output.txt', 'w')

#Insert your username and password here
#We recommend using password-less login via SSH Keys in order to preserve the security of passwords
user_ = 'aconeg'
pass_ ='password'
host_= 'mphase.rutgers.edu'
port_= 22

#SSH Client initialization
hostkeytype = None
hostkey = None
try:
    host_keys = paramiko.util.load_host_keys(os.path.expanduser('~/.ssh/known_hosts'))
except IOError:
    try:
        # try ~/ssh/ too, because windows can't have a folder named ~/.ssh/
        host_keys = paramiko.util.load_host_keys(os.path.expanduser('~/ssh/known_hosts'))
    except IOError:
        print '*** Unable to open host keys file'
        host_keys = {}

if host_keys.has_key(host_):
    hostkeytype = host_keys[host_].keys()[0]
    hostkey = host_keys[host_][hostkeytype]
    print 'Using host key of type %s' % hostkeytype

ssh_client = SSHClient()
ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
ssh_client.connect(hostname=host_, username=user_, password=pass_)

t = paramiko.Transport((host_,port_))
t.connect(username=user_,password=pass_,hostkey=hostkey)
sftp = paramiko.SFTPClient.from_transport(t)


#Variable initialization that store variable and data from text file
workflow_array=[]
parameter_array=[]
commands=["cd\n"]
cuda_fc=[]
c_fc=[]
filepaths = ['c/fileio.c', 'c/fileio.h', 'cuda/fileio.c', 'cuda/fileio.h', 'i.csv']
output = open('output.txt','w')

#Parse input file using regular expressions
f = open("example.txt")
for line in f:
	parameter_array.append(re.search('\((.*?)\)',line).group(1))
	function = re.search('(.*?)\(',line).group(1)
	workflow_array.append(function)
	filepaths.append("c/"+function+".c")
	filepaths.append("cuda/"+function+".cu")

for i in range(0,len(workflow_array)):
	commands.append("gcc -lpthread -o c_"+workflow_array[i]+" c/fileio.h c/fileio.c "+"c/"+workflow_array[i]+".c\n")


	if workflow_array[i]=="tile":
		commands.append("nvcc -o cu_"+workflow_array[i]+" cuda/"+workflow_array[i]+".c"+" cuda/fileio.c\n")	
	else:
		commands.append("nvcc -o cu_"+workflow_array[i]+" cuda/"+workflow_array[i]+".cu"+" cuda/fileio.c\n")


	c_run="./c_"+workflow_array[i]+" "
	cu_run="./cu_"+workflow_array[i]+" "
	#Parameter Parsing
	for prm in parameter_array[i].split(','):
		c_run=c_run+prm+" "
		cu_run=cu_run+prm+" "
	#Add to respective list (c or cuda)
	c_fc.append(c_run)
	cuda_fc.append(cu_run)


dirlist = sftp.listdir('.')
print "DirList:", dirlist
if not 'c' in dirlist:
	sftp.mkdir("c")
if not 'cuda' in dirlist:
	sftp.mkdir("cuda")
for i in filepaths:
	sftp.put(i,i)

chan = ssh_client.invoke_shell()
chan.send('rsh GpuNode01\n')

stdin = chan.makefile('wb')
stdout = chan.makefile('rb')

buff = ''
while not buff.endswith('$ '):
    resp = chan.recv(9999)
    buff += resp

#Run on GPU First
commands.append(cuda_fc[0]+" &\n")

#Load Balance
for cnt in range(1,len(c_fc)):
	commands=commands+["""#!/bin/sh\n""",
          		   """cmd='top -b -n 2 | grep "Cpu(s)" | tail -1 | grep -o -P ".{0,5}%id" | sed "s/.\{3\}$//"'\n""",
		 	   """cpu_load=$((100-($(eval $cmd | bc))))\n""",
		 	   """cmd='nvidia-smi -a | grep -m 1 "Gpu" | sed "s/[^0-9]*//g"'\n""",
          	  	   """gpu_load=$(eval $cmd | bc)\n""",
		  	   "if [[ $gpu_load < $cpu_load ]]\nthen\necho gpu\n"+cuda_fc[cnt]+" &\nelse\necho cpu\n"+c_fc[cnt]+" &\nfi\n",
         ]

#Exit out of RSH
commands=commands+["wait $!\n","""exit\n"""]

for line in commands:
	buff = ''
	print line
	chan.send(line)

	while not buff.endswith('$ '):
		resp = chan.recv(9999)
		buff += resp

	print buff

#Exit out of SSH
stdin.write(
'''
exit
''')


#openssl enc -aes-256-cbc -salt -in file.txt -out file.enc -pass pass:MySillyPassword

output.write(stdout.read())

#SSH Client code termination
ssh_client.close()
sftp.get('o1.csv','o1.csv')
sftp.get('o2.csv','o2.csv')
sftp.close()
#sftp.get('cpu.info','cpu.info')
t.close()
