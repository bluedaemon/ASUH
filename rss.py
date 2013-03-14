from paramiko import SSHClient
import paramiko

user_ = 'aconeg'
pass_ = 'xxxxxxxx'

ssh_client = SSHClient()
ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
ssh_client.connect('ece-ssh1.rutgers.edu',username=user_, password=pass_)
chan = ssh_client.invoke_shell()
stdin = chan.makefile('wb')
stdout = chan.makefile('rb')
stdin.write('''
cd Desktop
touch pi
ls
exit
''')
print stdout.read()
ssh_client.close()
