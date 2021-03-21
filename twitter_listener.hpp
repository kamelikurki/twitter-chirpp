#include <string>
#include <functional>

class twitter_listener
{
public:

    twitter_listener(const std::string bearerTokenString);

    std::string getRules();
    
    void addRule(const std::string newRuleValue, const std::string newRuleTag);
    
    void removeRule(const std::string ruleTag);
    
    void removeAllRules();

    void run(const int reconnectDelaySeconds);

    void printRules();

    std::function<void(std::string)> itsAMatch;

private:

    const std::string bearerToken;
    std::string messageData;
    static size_t handle_tweet_handle(char *ptr, size_t size, size_t nmemb, void *userdata);
    size_t handle_tweet_impl(char* ptr, size_t size, size_t nmemb);
    static size_t get_data_string_handle(char *ptr, size_t size, size_t nmemb, void *userdata);
    size_t get_data_string_impl(char *ptr, size_t size, size_t nmemb);
};