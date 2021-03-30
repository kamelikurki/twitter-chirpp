# twitter-listener

A simple c++ library for using the Twitter streaming API. It makes it possible to create rules for the API and stream tweets matching the rules. A simple command line interface demonstrating the usage is included. 

To use the twitter API, a registration at https://developer.twitter.com/ is needed. After being accepted to the twitter developer program the Bearer Token this twitter-listener needs for authenticating with the API will be available.

## Dependencies

The listener is using [CURL](https://curl.se/) to manage the connections to the twitter API endpoints. Installation instructions can be found here: https://curl.se/docs/install.html . 

## Building the library on linux

### Cmake

The project has Cmake files included and can be built using them.

### VS Code

The project has also VS code settings files included

## Building on Windows or OSX

These options haven't been tested but Cmake should work on them as well.


## Create a new instance of the listener 


```cpp

#include <string>
#include <iostream>
#include "twitter_listener.hpp"

std::string apiKey

std::cout << "Enter API Key: " ;
apiKey << std::cin;

twitter_listener t_listener(apiKey, std::cout);

```

Twitter listener takes two arguments, the API key and stream where to write possible output and error messages. Use standard output, a file or whatever is wanted. 


## Create a new rule 

```cpp
t_listener.addRule("Rule value", "Rule tag");

```

Adding a rule takes two arguments. First, a rule string. Instructions for creating the string are here: https://developer.twitter.com/en/docs/twitter-api/tweets/search/integrate/build-a-query . The tag value is used to identify the tweets when removing rules. The tag value is also returned as a part of the matching tweet. 


## Remove a rule
```cpp
t_listener.removeRule("Rule tag");

```
Rule can be removed by giving it's tag to the removeRule function. 



## Print existing rules
```cpp
t_listener.printRules();

```
Prints all rules to the stream given in the constructor. 

## Remove all rules
```cpp
t_listener.removeAllRules();

```
Removes all rules from the API
