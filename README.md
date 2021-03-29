# twitter-listener

A simple c++ library for using the Twitter streaming API. It makes it possible to create rules for the API and stream tweets matching the rules. A simple command line interface demonstrating the usage is included. 

To use the twitter API, a registration at https://developer.twitter.com/ is needed. After being accepted to the twitter developer program the Bearer Token this twitter-listener needs for authenticating with the API will be available.

## Create a new instance of the api 


```cpp

#include <string>
#include <iostream>
#include "twitter_listener.hpp"

std::string apiKey

std::cout << "Enter API Key: " ;
apiKey << std::cin;

twitter_listener t_listener(apiKey, std::cout);

```

Twitter listener takes two arguments, the API key and stream where to write possible output and error messages. Use standard output, a file or whatever you want. 


## Create a new rule 

```cpp
t_listener.addRule("Rule value", "Rule tag");

```




