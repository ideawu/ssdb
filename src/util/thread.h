#ifndef UTIL_THREAD_H_
#define UTIL_THREAD_H_

#include "../include.h"
#include <pthread.h>
#include <queue>
#include <vector>

// Thread safe queue
template <class T>
class Queue{
	private:
		pthread_cond_t cond;
		pthread_mutex_t mutex;
		std::queue<T> items;
	public:
		Queue();
		~Queue();

		bool empty();
		int size();
		int push(const T item);
		int pop(T *data);
};


// Selectable queue, multi writers, single reader
template <class T>
class SelectableQueue{
	private:
		int fds[2];
		pthread_mutex_t mutex;
		std::queue<T> items;
	public:
		SelectableQueue();
		~SelectableQueue();
		int fd(){
			return fds[0];
		}

		// multi writer
		int push(const T item);
		// single reader
		int pop(T *data);
};


template<class W, class JOB>
class WorkerPool{
	public:
		class Worker{
			public:
				int id;
				virtual int proc(JOB *job) = 0;
		};
	private:
		Queue<JOB> jobs;
		SelectableQueue<JOB> results;

		int num_works;
		std::vector<pthread_t> tids;
		bool started;

		struct run_arg{
			int id;
			WorkerPool *tp;
		};
		static void* _run_worker(void *arg);
	public:
		WorkerPool(int num_works);
		~WorkerPool();

		int fd_result();
		void add_job(JOB job);
		void get_result(JOB *job);
		int start();
		int stop();
};





template <class T>
Queue<T>::Queue(){
	pthread_cond_init(&cond, NULL);
	pthread_mutex_init(&mutex, NULL);
}

template <class T>
Queue<T>::~Queue(){
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
}

template <class T>
bool Queue<T>::empty(){
	bool ret = false;
	if(pthread_mutex_lock(&mutex) != 0){
		return -1;
	}
	ret = items.empty();
	pthread_mutex_unlock(&mutex);
	return ret;
}

template <class T>
int Queue<T>::size(){
	int ret = -1;
	if(pthread_mutex_lock(&mutex) != 0){
		return -1;
	}
	ret = items.size();
	pthread_mutex_unlock(&mutex);
	return ret;
}

template <class T>
int Queue<T>::push(const T item){
	if(pthread_mutex_lock(&mutex) != 0){
		return -1;
	}
	{
		items.push(item);
	}
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond);
	return 1;
}

template <class T>
int Queue<T>::pop(T *data){
	if(pthread_mutex_lock(&mutex) != 0){
		return -1;
	}
	{
		// 必须放在循环中, 因为 pthread_cond_wait 可能抢不到锁而被其它处理了
		while(items.empty()){
			//fprintf(stderr, "%d wait\n", pthread_self());
			if(pthread_cond_wait(&cond, &mutex) != 0){
				//fprintf(stderr, "%s %d -1!\n", __FILE__, __LINE__);
				return -1;
			}
			//fprintf(stderr, "%d wait 2\n", pthread_self());
		}
		*data = items.front();
		//fprintf(stderr, "%d job: %d\n", pthread_self(), (int)*data);
		items.pop();
	}
	if(pthread_mutex_unlock(&mutex) != 0){
		//fprintf(stderr, "error!\n");
		return -1;
	}
		//fprintf(stderr, "%d wait end 2, job: %d\n", pthread_self(), (int)*data);
	return 1;
}


template <class T>
SelectableQueue<T>::SelectableQueue(){
	pipe(fds);
	pthread_mutex_init(&mutex, NULL);
}

template <class T>
SelectableQueue<T>::~SelectableQueue(){
	pthread_mutex_destroy(&mutex);
	close(fds[0]);
	close(fds[1]);
}

template <class T>
int SelectableQueue<T>::push(const T item){
	if(pthread_mutex_lock(&mutex) != 0){
		return -1;
	}
	{
		items.push(item);
	}
	::write(fds[1], "1", 1);
	pthread_mutex_unlock(&mutex);
	return 1;
}

template <class T>
int SelectableQueue<T>::pop(T *data){
	int n, ret = 1;
	char buf[1];

	while(1){
		n = ::read(fds[0], buf, 1);
		if(n < 0){
			if(errno == EINTR){
				continue;
			}else{
				return -1;
			}
		}else if(n == 0){
			ret = -1;
		}else{
			if(pthread_mutex_lock(&mutex) != 0){
				return -1;
			}
			{
				if(items.empty()){
					fprintf(stderr, "%s %d error!\n", __FILE__, __LINE__);
					pthread_mutex_unlock(&mutex);
					return -1;
				}
				*data = items.front();
				items.pop();
			}
			pthread_mutex_unlock(&mutex);
		}
		break;
	}
	return ret;
}



template<class W, class JOB>
WorkerPool<W, JOB>::WorkerPool(int num_works){
	this->num_works = num_works;
	this->started = false;
}

template<class W, class JOB>
WorkerPool<W, JOB>::~WorkerPool(){
	if(started){
		stop();
	}
}

template<class W, class JOB>
int WorkerPool<W, JOB>::fd_result(){
	return results.fd_read();
}

template<class W, class JOB>
void WorkerPool<W, JOB>::add_job(JOB job){
	this->jobs.push(job);
}

template<class W, class JOB>
void WorkerPool<W, JOB>::get_result(JOB *job){
	this->results.pop(job);
}

template<class W, class JOB>
void* WorkerPool<W, JOB>::_run_worker(void *arg){
	struct run_arg *p = (struct run_arg*)arg;
	int id = p->id;
	WorkerPool *tp = p->tp;
	delete p;

	W w;
	Worker *worker = (Worker *)&w;
	worker->id = id;
	while(1){
		JOB job;
		if(tp->jobs.pop(&job) == -1){
			fprintf(stderr, "jobs.pop error\n");
			::exit(0);
			break;
		}
		worker->proc(&job);
		if(tp->results.push(job) == -1){
			fprintf(stderr, "results.push error\n");
			::exit(0);
			break;
		}
	}
	return (void *)NULL;
}

template<class W, class JOB>
int WorkerPool<W, JOB>::start(){
	if(started){
		return 0;
	}
	int err;
	pthread_t tid;
	for(int i=0; i<num_works; i++){
		struct run_arg *arg = new run_arg();
		arg->id = i;
		arg->tp = this;

		err = pthread_create(&tid, NULL, &WorkerPool::_run_worker, arg);
		if(err != 0){
			fprintf(stderr, "can't create thread: %s\n", strerror(err));
		}else{
			tids.push_back(tid);
		}
	}
	started = true;
	return 0;
}

template<class W, class JOB>
int WorkerPool<W, JOB>::stop(){
	// TODO: notify works quit and wait
	for(int i=0; i<tids.size(); i++){
		pthread_cancel(tids[i]);
	}
	return 0;
}



#if 0
class MyWorker : public WorkerPool<MyWorker, int>::Worker{
	public:
		int proc(int *job){
			*job = (id + 1) * 100000 + *job;
			return 0;
		}
};

int main(){
	int num_jobs = 1000;
	WorkerPool<MyWorker, int> tp(10);
	tp.start();
	for(int i=0; i<num_jobs; i++){
		//usleep(200 * 1000);
		//printf("job: %d\n", i);
		tp.add_job(i);
	}
	printf("add end\n");
	for(int i=0; i<num_jobs; i++){
		int job;
		tp.get_result(&job);
		printf("result: %d, %d\n", i, job);
	}
	printf("end\n");
	//tp.stop();
	return 0;
}
#endif

#endif


