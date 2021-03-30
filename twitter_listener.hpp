#include <string>
#include <functional>
#include "curl/curl.h"

class twitter_listener
{
public:

    //! Constructor
    /*!
      Constructs a new twitter listener object
      \param bearerTokenString bearer token for authenticating
    */
    twitter_listener(const std::string bearerTokenString);

    //! Destructor
    /*!
      Destructs a  twitter listener object
      \param bearerTokenString bearer token for authenticating
    */
    ~twitter_listener();

    //! Function returning all currently active rules
    /*!
      \return String listing all the rules returned by the twitter API
    */
    std::string getRules();
    
    //! Function adding a new rule 
    /*!
      \param newRuleValue constant string which is to be used as the new rule value 
      \param newRuleTag constant string to use as the new rule tag. Tag is used to identify rules when later edited. 
    */
    void addRule(const std::string newRuleValue, const std::string newRuleTag);
    
    //! Function removing a rule
    /*!
      \param ruleTag constant string to use as the new rule tag. Tag is used to identify rules when later edited. 
    */
    void removeRule(const std::string ruleTag);
    
    //! Function removing all rules
    /*!     
    */
    void removeAllRules();

    //! Function printing all rules to a given output stream
    /*!    
    \param printDestination Output stream to print the rules to 
    */
    void printRules(std::ostream& printDestination);

    //! Function running the listener
    /*!    
      Creates a connection to the twitter streaming API and calls itsAMatch function every time a tweet matching the defined rules is found. 
    \param reconnectDelaySeconds 
    */
    void run(std::ostream& infoDestination, const int reconnectDelaySeconds = 300);

    static size_t print_header(char* ptr, size_t size, size_t nitems, void *userdata) ;

    //! Function variable
    /*!    
      A function which returns void and takes string and a long as arguments. 
    */
    std::function<void(std::string, long)> itsAMatch;

private:

    const std::string bearerToken; /**< bearer token used for authentication purposes */
    std::string messageData;       /**< variable to hold message data returned by the endpoint */
    CURL* curl;                    /**< curl object */

    //! Handle to a function which handles a tweet
    /*!    
    \param ptr character pointer to the data delivered by the endpoint
    \param size size of a member in the delivered data
    \param nmemb number of members in the delivered data. Always 1 (from curl documentation)
    \param userdata void pointer to the userdata. Userdata is set by with CURLOPT_WRITEDATA option.
    \return size_t variable with the information how many bytes were handled
      A function which returns void and takes string as an argument. 
    */
    static size_t handle_tweet_handle(char *ptr, size_t size, size_t nmemb, void *userdata);


    //! Implementation of a function which handles a tweet
    /*!    
    \param ptr character pointer to the data delivered by the endpoint
    \param size size of a member in the delivered data
    \param nmemb number of members in the delivered data. Always 1 (from curl documentation)
    \param userdata void pointer to the userdata. Userdata is set by with CURLOPT_WRITEDATA option.
    \return size_t variable with the information how many bytes were handled
      A function which returns void and takes string as an argument. 
    */
    size_t handle_tweet_impl(char* ptr, size_t size, size_t nmemb);

    //! Handle to a function which retrieves a string from the endpoint
    /*!    
    \param ptr character pointer to the data delivered by the endpoint
    \param size size of a member in the delivered data
    \param nmemb number of members in the delivered data. Always 1 (from curl documentation)
    \param userdata void pointer to the userdata. Userdata is set by with CURLOPT_WRITEDATA option.
    \return size_t variable with the information how many bytes were handled
      A function which returns void and takes string as an argument. 
    */
    static size_t get_data_string_handle(char *ptr, size_t size, size_t nmemb, void *userdata);

    //! Implementation of a function which retrieves a string from the endpoint
    /*!    
    \param ptr character pointer to the data delivered by the endpoint
    \param size size of a member in the delivered data
    \param nmemb number of members in the delivered data. Always 1 (from curl documentation)
    \param userdata void pointer to the userdata. Userdata is set by with CURLOPT_WRITEDATA option.
    \return size_t variable with the information how many bytes were handled
      A function which returns void and takes string as an argument. 
    */
    size_t get_data_string_impl(char *ptr, size_t size, size_t nmemb);
};