#include "Timer.h"
#include <windows.h>

class TimerImp : public Timer {
public:
	int interval;
	timer_callback callback;
	void *callback_binding;
	HANDLE h_thread;

public:
	virtual void SetInterval(int v);
	virtual int GetInterval();

public:
	typedef void (*timer_callback)(Timer &timer, void *binding);
	virtual void SetCallback(timer_callback callback, void *binding);
	virtual timer_callback GetCallback();
	virtual void* GetCallbackBinding();

public:
	virtual void Start();
	virtual void Stop();

public:
	virtual bool IsStarted();

public:
	TimerImp():
		interval(0),
		callback(nullptr),
		callback_binding(nullptr),
		h_thread(INVALID_HANDLE_VALUE) {

	}

public:
	~TimerImp() {
		this->Stop();
	}
};

Timer* Timer::Create(int interval, timer_callback callback, void *binding) {
	TimerImp *timer = new TimerImp;
	timer->interval = interval;
	timer->callback = callback;
	timer->callback_binding = binding;
	return timer;
}

void TimerImp::SetInterval(int v) {
	this->interval = v;
}

int TimerImp::GetInterval() {
	return this->interval;
}

void TimerImp::SetCallback(timer_callback callback, void* binding) {
	this->callback = callback;
	this->callback_binding = binding;
}

Timer::timer_callback TimerImp::GetCallback() {
	return this->callback;
}

void* TimerImp:: GetCallbackBinding() {
	return this->callback_binding;
}

DWORD WINAPI loop_thread(LPVOID param) {
	TimerImp *self = (TimerImp*)param;

	for (;;) {
		Sleep(self->interval);
		if (self->callback != nullptr) {
			self->callback(*self, self->callback_binding);
		}
	}
}

void TimerImp::Start() {
	if (this->h_thread != INVALID_HANDLE_VALUE) return;

	DWORD thread_id;

	HANDLE h_thread = CreateThread(
		NULL,
		0,
		loop_thread,
		(LPVOID)this,
		0,
		&thread_id
		);

	if (h_thread != NULL) {
		this->h_thread = h_thread;
	}
}

void TimerImp::Stop() {
	if (this->h_thread == INVALID_HANDLE_VALUE) return;
	TerminateThread(this->h_thread, 0);
	this->h_thread = INVALID_HANDLE_VALUE;
}

bool TimerImp::IsStarted() {
	return this->h_thread != INVALID_HANDLE_VALUE;
}