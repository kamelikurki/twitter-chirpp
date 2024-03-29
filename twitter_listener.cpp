#include "nlohmann/json.hpp"
#include <iostream>
#include "twitter_listener.hpp"

using json = nlohmann::json;

twitter_listener::twitter_listener(const std::string bearerTokenString) : bearerToken(bearerTokenString)
{
    curl = curl_easy_init();
}

twitter_listener::~twitter_listener()
{
    curl_easy_cleanup(curl);
}

std::string twitter_listener::getRules()
{
    CURLcode res;
    struct curl_slist *slist1;
    slist1 = NULL;

    std::string authStr = "Authorization: Bearer ";
    authStr = authStr + bearerToken;

    slist1 = curl_slist_append(slist1, authStr.c_str());
    curl_easy_reset(curl);
    curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 102400L);
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.twitter.com/2/tweets/search/stream/rules");
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, twitter_listener::get_data_string_handle);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_operation() failed : %s\n", curl_easy_strerror(res));
    }

    return messageData;
}


void twitter_listener::printRules(std::ostream& printDestination)
{
    auto rules = getRules();
    auto rulesJSON = json::parse(rules);

    for(auto rule : rulesJSON["data"])
    {
        std::cout << std::endl;
        if(rule.contains("id"))
        {
            printDestination << "Rule ID: " << rule["id"].dump() << std::endl;          
        }

        if(rule.contains("value"))
        {
            printDestination << "Rule value: " << rule["value"].dump() << std::endl;
        }

        if(rule.contains("tag"))
        {
            printDestination << "Rule tag: " << rule["tag"].dump() << std::endl;          
        }   
    }
}

size_t twitter_listener::get_data_string_impl(char *ptr, size_t size, size_t nmemb)
{
    messageData.append((char*)ptr, size * nmemb);

    return size*nmemb;
}

size_t twitter_listener::get_data_string_handle(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    return static_cast<twitter_listener*>(userdata)->get_data_string_impl(ptr, size, nmemb);
}

void twitter_listener::addRule(const std::string newRuleValue, const std::string newRuleTag)
{
    json add;
    json value;
    value["value"] = newRuleValue;
    value["tag"] = newRuleTag;
    add["add"].push_back(value);
    auto vals = add.dump();

	CURLcode res;
	if (curl)
	{
        struct curl_slist *slist1;

        curl_easy_reset(curl);

        curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 102400L);
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.twitter.com/2/tweets/search/stream/rules");
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, vals.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)vals.length());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, twitter_listener::get_data_string_handle);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_operation() failed : %s\n", curl_easy_strerror(res));
		}
        else
        {
            auto outputJson = json::parse(messageData);

            if(outputJson["meta"]["summary"]["created"] == 1)
            {
                if(outputJson["meta"]["summary"]["valid"] == 1)
                {
                    std::cout << "Succesfully created a new rule" << std::endl;
                    std::cout << "Value: " << outputJson["data"][0]["value"]<<std::endl;
                    std::cout << "Tag: " << outputJson["data"][0]["tag"]<<std::endl;
                }
                else
                {
                    std::cout << " Invalid rule" << std::endl; 
                }
            }
            else
            {
                std::cout << "Rule not created"<<std::endl;
            }
        }
	}
}


void twitter_listener::removeRule(const std::string ruleTag)
{
    // get a rule with the tag that's been asked to remove 

    auto rules = getRules();
    auto rulesJson = json::parse(rules);

    for(int i = 0; i < rulesJson["data"].size(); ++i)
    {
        auto currentTag = rulesJson["data"][i]["tag"].dump();
        
        currentTag.erase(std::remove(currentTag.begin(), currentTag.end(), '\"'), currentTag.end());

        if(currentTag.compare(ruleTag) == 0)
        {
            auto currentId = rulesJson["data"][i]["id"].dump();
            curl = curl_easy_init();
            CURLcode res;
            if (curl)
            {
                struct curl_slist *slist1;
                json jsonrequest;

                currentId.erase(std::remove(currentId.begin(), currentId.end(), '\"'), currentId.end());

                jsonrequest["delete"]["ids"].push_back(currentId.c_str()); 

                auto jsonrequeststr = jsonrequest.dump();

                slist1 = NULL;
                slist1 = curl_slist_append(slist1, "Content-type: application/json");

                std::string authStr = "Authorization: Bearer ";
                authStr = authStr + bearerToken;
                slist1 = curl_slist_append(slist1, authStr.c_str());
                curl_easy_reset(curl);
                curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 102400L);
                curl_easy_setopt(curl, CURLOPT_URL, "https://api.twitter.com/2/tweets/search/stream/rules");
                curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonrequeststr.c_str());
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)jsonrequeststr.length());
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
                curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
                curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, twitter_listener::get_data_string_handle);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

                res = curl_easy_perform(curl);
                if (res != CURLE_OK)
                {
                    fprintf(stderr, "curl_easy_operation() failed : %s\n", curl_easy_strerror(res));
                }
            }
        }
    }
}


void twitter_listener::removeAllRules()
{
    auto rules = getRules();
    auto rulesJson = json::parse(rules);

    for(int i = 0; i < rulesJson["data"].size(); ++i)
    {
        auto currentId = rulesJson["data"][i]["id"].dump();
        curl = curl_easy_init();
        CURLcode res;
        if (curl)
        {
            struct curl_slist *slist1;
            json jsonrequest;

            currentId.erase(std::remove(currentId.begin(), currentId.end(), '\"'), currentId.end());

            jsonrequest["delete"]["ids"].push_back(currentId.c_str()); 

            auto jsonrequeststr = jsonrequest.dump();

            slist1 = NULL;
            slist1 = curl_slist_append(slist1, "Content-type: application/json");

            std::string authStr = "Authorization: Bearer ";
            authStr = authStr + bearerToken;
            slist1 = curl_slist_append(slist1, authStr.c_str());

            curl_easy_reset(curl);
            curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 102400L);
            curl_easy_setopt(curl, CURLOPT_URL, "https://api.twitter.com/2/tweets/search/stream/rules");
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonrequeststr.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)jsonrequeststr.length());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
            curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
            curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, twitter_listener::get_data_string_handle);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
            {
                fprintf(stderr, "curl_easy_operation() failed : %s\n", curl_easy_strerror(res));
            }
        }
    }
}


size_t twitter_listener::handle_tweet_impl(char *ptr, size_t size, size_t nmemb)
{

    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    std::string message;

    for(int i = 0; i < nmemb; ++i) message.push_back(ptr[i]);

    itsAMatch(message, response_code);

    return nmemb;
}

size_t twitter_listener::handle_tweet_handle(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    return static_cast<twitter_listener*>(userdata)->handle_tweet_impl(ptr, size, nmemb);
}

void twitter_listener::run(std::ostream& logDestination, const int reconnectDelaySeconds)
{
	CURLcode res;
    
    struct curl_slist *slist1;
    slist1 = NULL;

    std::string authStr = "Authorization: Bearer ";
    authStr = authStr + bearerToken;

    slist1 = curl_slist_append(slist1, authStr.c_str());

    curl_easy_reset(curl);
    curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 102400L);
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.twitter.com/2/tweets/search/stream");
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, twitter_listener::handle_tweet_handle);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        logDestination << "curl_easy_operation() failed : %s\n" << curl_easy_strerror(res);
    }
}