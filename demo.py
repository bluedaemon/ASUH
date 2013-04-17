import re
import os
import getpass
import paramiko
from paramiko import SSHClient

output = open('output.txt', 'w')

user_ = 'patgray'#getpass.getuser()
pass_ = 'GrayIsGood#!'
host_ = 'orissa.engr.rutgers.edu'
port_ = 22

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
#privatekeyfile = os.path.expanduser('~/.ssh/id_rsa')
#mykey = paramiko.RSAKey.from_private_key_file(privatekeyfile)
t = paramiko.Transport((host_,port_))
t.connect(username=user_,password=pass_,hostkey=hostkey)
sftp = paramiko.SFTPClient.from_transport(t)

connection = ssh_client.invoke_shell()
stdin = connection.makefile('wb')
stdout = connection.makefile('rb')

dirlist = sftp.listdir('.')
print "DirList:", dirlist
filepaths = ['c/fileio.c', 'c/fileio.h', 'c/min.c', 'c/max.c', 'c/rand.c']
sftp.mkdir("c")
for i in filepaths:
	sftp.put(i,i)


stdin.write(
'''
cd
gcc -o ran c/fileio.h c/fileio.c c/rand.c
gcc -o min c/fileio.h c/fileio.c c/min.c
gcc -o max c/fileio.h c/fileio.c c/max.c
./ran 100 100 i.csv
./min 100 100 i.csv o1.csv
./max 100 100 i.csv o2.csv
exit
'''
)
output.write(stdout.read())
ssh_client.close()
dirlist = sftp.listdir('.')
print "DirList:", dirlist
sftp.get('o1.csv','o1.csv')
sftp.get('o2.csv','o2.csv')
t.close()
