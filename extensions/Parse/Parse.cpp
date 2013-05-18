#include "Parse.h"
#include "ParseManager.h"

NS_CC_EXT_BEGIN

Parse::Parse(void)
{
}


Parse::~Parse(void)
{
}

/// configuration
std::string Parse::applicationId() const
{
	return ParseManager::instance()->applicationId();
}

void Parse::setApplicationId(const std::string &applicationId)
{
	ParseManager::instance()->setApplicationId(applicationId);
}

std::string Parse::apiKey() const
{
	return ParseManager::instance()->apiKey();
}

void Parse::setApiKey(const std::string &apiKey)
{
	ParseManager::instance()->setApiKey(apiKey);
}

std::string Parse::masterKey() const
{
	return ParseManager::instance()->masterKey();
}

void Parse::setMasterKey(const std::string &masterKey)
{
	ParseManager::instance()->setMasterKey(masterKey);
}

bool Parse::trace() const
{
	return ParseManager::instance()->trace();
}

void Parse::setTrace(bool trace)
{
	ParseManager::instance()->setTrace(trace);
}


NS_CC_EXT_END