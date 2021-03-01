#include <string>


class twitter_listener
{
public:

    twitter_listener(const std::string bearerTokenString);

    std::string getRules();
    void addRule(const std::string newRuleValue, const std::string newRuleTag);
    void removeRule(const std::string ruleTag);
    void removeAllRules();

    void run();

    void printRules();

private:

    const std::string bearerToken;

    static size_t handle_tweet(char *ptr, size_t size, size_t nmemb, void *userdata);
    static size_t get_data_string(char *ptr, size_t size, size_t nmemb, void *userdata);
};