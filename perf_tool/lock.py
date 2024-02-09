import multiprocessing
import time

lock = multiprocessing.Lock()

counter = multiprocessing.Value('i', 0)

def worker():
	end_time = time.time() + 5 

	while time.time() < end_time:
		lock.acquire()
		try:
			print(counter.value)
			counter.value += 1
			time.sleep(0.1)
		finally:
			lock.release()

p1 = multiprocessing.Process(target=worker)
p2 = multiprocessing.Process(target=worker)
p1.start()
p2.start()

p1.join()
p2.join()
