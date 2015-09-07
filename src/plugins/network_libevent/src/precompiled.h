#ifndef ___PRECOMPILED_H_____
#define ___PRECOMPILED_H_____

//	common
#include <stdio.h>
#include <assert.h>

//	libevent;
#include "libevent/event2/util.h"
#include "libevent/event2/buffer.h"
#include "libevent/event2/bufferevent.h"
#include "libevent/event2/bufferevent_compat.h"
#include "libevent/event2/event.h"
#include "libevent/event2/listener.h"
#include "libevent/event2/thread.h"

#include "dxm/platform.h"

#ifndef DXM_PLATFORM_WIN32
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

//	pthread;
#ifdef DXM_PLATFORM_WIN32
#include "pthread/pthread.h"
#endif

// #include "boost/thread/condition.hpp"
// #include "boost/thread/mutex.hpp"
// #include "boost/signals2.hpp"
// #include "boost/thread/shared_mutex.hpp"
// #include "boost/thread/locks.hpp"
// #include "boost/signals2.hpp"
// 
// #include "dxm_util_boost/util_boost_tls_buffer.h"
// #include "dxm_util_boost/util_boost_event_set.hpp"

#include "component/net_service.h"
// #include "dxm_util_component/util_component_log.h"

using namespace Foundation::NetService;

#endif
