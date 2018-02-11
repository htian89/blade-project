#ifndef QUERY_SERVER_H_
#define QUERY_SERVER_H_

#include <string>
#include <map>
#include <set>
#include <boost/thread.hpp>
#include "http_frame/base_handler.h"
#include "http_frame/server.h"
#include "joft/config/IniFileReader.h"
#include "Ad_Servers/AdServer/AdServerCore.h"
#include "Ad_Servers/AdServer/AdServerControl.h"

using std::string;
using std::map;

namespace search_frame {

class QueryServerControl;

class AdserverRequestHandler : public ads::http_server::BaseHandler {
 public:
  explicit AdserverRequestHandler(http::I_CoreWork* server_core)
      : server_core_(server_core) {}
  virtual ~AdserverRequestHandler() {}
 private:
  virtual void Init(const std::string& str) {}
  virtual bool Work(const toft::HttpRequest &request,
                    toft::HttpResponse* response);
  bool ParseRequest(const toft::HttpRequest& request,
                    std::string* request_str);
  void CallMethod(std::string* request_str,
                  toft::HttpResponse* response,
                  std::string* response_str,
                  google::protobuf::Closure* done);
  virtual bool Work(toft::HttpRequest* request,
                    toft::HttpResponse* response,
                    google::protobuf::Closure* done);
  http::I_CoreWork* server_core_;
};

class QueryServer
{
 public:
	QueryServer()
        : _ini_reader(NULL),
        _server_core(NULL),
        _server_control(NULL),
        _server_thread(NULL),
        _server_thread_control(NULL)
    {
        m_num_threads = sysconf(_SC_NPROCESSORS_ONLN);
    }


  ~QueryServer() {
    delete _server_thread;
    delete _server_thread_control;
    delete _server_control;
    delete _server_core;
  }

	/* 读取ini配置文件,　完成QueryServer的初始化 */
	void Init(joft::config::IniFileReader& ini_reader, std::string &ini_file);

	/* 运行QueryServer */
	void Run();

    int Update(std::vector<std::string> resource_para, bool force)
    {
        return _server_core->ResourceMgrUpdate(resource_para, force);
    }
    QueryServerCore * GetServerCore()
    {
        return _server_core;
    }
    void DeleteCacheSkus(const std::vector<std::string>& del_skus) {
      _server_core->DeleteCacheSkus(del_skus);
    }
 private:
    /* 启动查询端口的服务 */
    void StartService();

    void StartControl();
    int load_values_int(std::set<int> &values, const std::string frameName,
            const std::string &paraName, const std::string &defaultParaValue);
 private:
    /* 用于读取ini配置的文件的工具类. 在主线程中初始化, 本类只持有引用 */
    joft::config::IniFileReader *_ini_reader;

    /* QueryServer中真正处理每个请求的类 */
    QueryServerCore *_server_core;

    QueryServerControl *_server_control;

    /* 工作线程 */
    boost::thread *_server_thread;

    boost::thread *_server_thread_control;

    int m_num_threads;

    toft::scoped_ptr<ads::http_server::BaseHandler> service_request_handler_;
    toft::scoped_ptr<ads::http_server::Server> http_server_service_;
    toft::scoped_ptr<ads::http_server::BaseHandler> control_request_handler_;
    toft::scoped_ptr<ads::http_server::Server> http_server_control_;
    /* QueryExpand工具类 */
    //	QueryExpand *_qe;

    //	QueryExpand * _qe1;
    //	QueryExpand * _qe2;
    //	map<string, string> catid_brnad_hotword;
};

}
#endif
