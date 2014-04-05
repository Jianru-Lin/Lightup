#pragma once

class Timer {
public:
	typedef void(*timer_callback)(Timer &timer, void *binding);
	static Timer* Create(int interval, timer_callback callback, void *binding);

public:
	virtual void SetInterval(int v)=0;
	virtual int GetInterval()=0;

public:
	virtual void SetCallback(timer_callback callback, void *binding)=0;
	virtual timer_callback GetCallback()=0;
	virtual void* GetCallbackBinding()=0;

public:
	virtual void Start()=0;
	virtual void Stop()=0;

public:
	virtual bool IsStarted()=0;

public:
	virtual ~Timer() {}
};