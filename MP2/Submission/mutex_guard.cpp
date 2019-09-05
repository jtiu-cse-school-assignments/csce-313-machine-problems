#include "mutex_guard.H"

MutexGuard::MutexGuard(Mutex & m) {
	m.Lock();
}

MutexGuard::~MutexGuard() {
	m->Unlock();
}