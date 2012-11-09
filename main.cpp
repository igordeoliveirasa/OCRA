//
//  main.cpp
//  TestOTP
//
//  Created by Robson Ventura Rodrigues on 15/05/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include "OneTimePasswordAlgorithm.h"
#include <boost/algorithm/string.hpp>

using namespace std;

int main(int argc, const char * argv[]) {
    
    string ocraSuite = "OCRA-1:HOTP-SHA1-6:QN08";
    std::string key = "";
    std::string counter = "";
    std::string question = "";
    std::string password = "";
    std::string sessionInformation = "";
    std::string timeStamp = "";

    string res = generateOCRA(ocraSuite, key, counter, question, password,
            sessionInformation, timeStamp);

    cout << res << endl;
    
    return 0;
}

