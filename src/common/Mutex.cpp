#include <Mutex.h>

template<class dev_T> 
int Mutex<dev_T>::prev_id = MUTEX_RELEASED;

template<class dev_T> 
int Mutex<dev_T>::reserved_by = MUTEX_RELEASED;

template<class dev_T> 
Mutex<dev_T>::Mutex(dev_T *dev) : dev(dev) {
}

template<class dev_T> 
bool Mutex<dev_T>::reserve() {
	if (released() || isMine()) {
		reserved_by = id;
		return true;
	}

	return false;
}

template<class dev_T>
bool Mutex<dev_T>::isMine() {
	return reserved_by == id;
}

template<class dev_T>
bool Mutex<dev_T>::release() {
	if (isMine() || released()) {
		reserved_by == MUTEX_RELEASED;
		return true;
	}

	return false;
}

template<class dev_T>
bool Mutex<dev_T>::released() {
	return reserved_by == MUTEX_RELEASED;
}
