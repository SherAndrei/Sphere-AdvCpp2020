#ifndef HTTP_COR_SERVICE_H
#define HTTP_COR_SERVICE_H
#include "connection_container.h"
#include "coroutine.h"
#include "corlistener.h"
#include "corconnection.h"
#include "httpservice.h"

namespace http {
namespace cor {

class CoroutineService : public HttpService {
 public:
    CoroutineService(const tcp::Address& addr, ICoroutineListener* listener, size_t workersSize,
                     size_t connection_timeout_sec = CONNECTION_TIMEOUT,
                     size_t keep_alive_timeout_sec = KEEP_ALIVE_CONNECTION_TIMEOUT);

    virtual ~CoroutineService() = default;

 public:
    ICoroutineListener* getListener();
    void setListener(ICoroutineListener* listener);

 private:
    void work(size_t thread_num) override;
    void serve_client(net::ConnectionAndData* p_place);

    net::ConnectionAndData* emplace_connection(tcp::NonBlockConnection&& cn) override;
    net::ConnectionAndData* try_replace_closed_with_new_conn(tcp::NonBlockConnection&& cn) override;

    void subscribe(net::ConnectionAndData* p_place, net::OPTION opt)   const override;
    void unsubscribe(net::ConnectionAndData* p_place, net::OPTION opt) const override;

 private:
    bool try_read_request(net::ConnectionAndData* p_place) override;
    bool try_write_responce(net::ConnectionAndData* p_place) override;

 private:
    CorConnection* get_connection_and_try_reset_last_activity_time(net::ConnectionAndData* p_place);

 private:
    void activate_workers() override;
};

}  // namespace cor
}  // namespace http

#endif  // HTTP_COR_SERVICE_H
