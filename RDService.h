#pragma once
#include "cpprest\http_listener.h"
#include "cpprest/json.h"
#include "cpprest/filestream.h"
#include "cpprest/containerstream.h"
#include "cpprest/producerconsumerstream.h"
#include "cpprest/http_listener.h"
#include "cpprest/http_client.h"



#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>

#include <iostream>

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

using namespace std;


#pragma warning ( push )
#pragma warning ( disable : 4457 )
#include <agents.h>
#pragma warning ( pop )
#include <locale>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>
#include <map>


#include <boost/algorithm/string.hpp>
#include <DbHelper.h>
using namespace std;
class RDService
{
public:
	RDService();
	RDService(utility::string_t url);
	~RDService();

	pplx::task<void> open();
	pplx::task<void> close();

	void handle_get(web::http::http_request message);
	void handle_post(web::http::http_request message);
	void insertDummyData();
private:
	// Error handlers
	static void handle_error(pplx::task<void>& t);
	// HTTP listener 
	web::http::experimental::listener::http_listener m_listener;
	vector<utility::string_t> RDService::splitStringByAnd(utility::string_t inputString);
};

