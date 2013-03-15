import random, Queue
import threading

def m_min(m):
	q = Queue.PriorityQueue()
	for i in range(0,len(m)):
		tz = threading.Thread(target=a_min,args=(m[i],q))
		tz.start()
	print q.get()
			
def a_min(a,q):
	q.put(min(a))

N=8
M=7
t = [[random.randrange(1,100) for x in range(N)] for y in range(M)]
print t

m_min(t)
