#pragma once

#define MUTEX_RELEASED -1

template <class dev_T>
class Mutex {
	const int id;
	static int prev_id;
	static int reserved_by;
	dev_T *dev;
public:
	Mutex(dev_T *dev);
	bool reserve();
	bool isMine();
	bool release();
	bool released();
};