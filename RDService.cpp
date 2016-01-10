#define _SCL_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNIGHS

#include "RDService.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;


RDService::RDService()
{

}


RDService::RDService(utility::string_t url) : m_listener(url)
{
	m_listener.support(methods::GET, std::bind(&RDService::handle_get, this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&RDService::handle_get, this, std::placeholders::_1));
	DbHelper::getInstance()->initializeDatabase();
	//uncomment this to insert some dummy data
	//DbHelper::getInstance()->insertData();
}


void RDService::insertDummyData()
{
	DbHelper::getInstance()->insertData();
}
RDService::~RDService()
{
}


void RDService::handle_error(pplx::task<void>& t)
{
	try
	{
		t.get();
	}
	catch (...)
	{
		// Ignore the error, Log it if a logger is available 
	}
}
pplx::task<void> RDService::open()
{
	return m_listener.open().then([](pplx::task<void> t) { handle_error(t); });
}

pplx::task<void> RDService::close()
{
	return m_listener.close().then([](pplx::task<void> t) { handle_error(t); });
}

vector<utility::string_t> RDService::splitStringByAnd(utility::string_t inputString)
{
	std::vector<utility::string_t> tok;
	split(tok, inputString, boost::is_any_of(L"&"));

	for (std::vector<std::wstring>::iterator tok_iter = tok.begin();
		tok_iter != tok.end(); ++tok_iter)
	{
		std::wcout << *tok_iter << '\n';
	}
	return tok;
}

void RDService::handle_get(http_request message)
{
	std::cout << "GET request got" << std::endl;
	web::uri reqUri = message.relative_uri();

	wcout << "Query:" << reqUri.query() << endl << reqUri.resource().to_string() << endl;

	utility::string_t queryStr = reqUri.query();
	
	auto path = reqUri.path();
	vector<utility::string_t> queryList = splitStringByAnd(queryStr);

	wstring conditions = U("");

	if (queryList.size() > 0)
	{
		conditions += queryList[0];
	}

	for (size_t i = 1; i < queryList.size(); i++)
	{
		conditions += U(" AND ") + queryList[i];
	}

	string finalCondition(conditions.begin(), conditions.end());

	vector<Vehicle> dbResult = DbHelper::getInstance()->getVehicleList((char *)finalCondition.c_str());

	string table = "";


	auto path2 = message.relative_uri().path();
	string reply;

	for (int i = 0; i < dbResult.size(); i++)
	{
		Vehicle v = dbResult[i];
		string tr = "";
		tr += v.Registration + "#";
		tr +=  to_string(v.Make) + "#";
		tr += "" + v.Model + "#";
		tr += "" + v.Owner + "#";
	
		table += tr;
	}

	utility::string_t replyText(table.begin(), table.end());
	message.reply(status_codes::OK, replyText).then([](pplx::task<void> t) { handle_error(t); });

}
