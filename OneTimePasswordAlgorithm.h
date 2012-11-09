//
//  OneTimePasswordAlgorithm.h
//  TestOTP
//
//  Created by Robson Ventura Rodrigues on 15/05/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef TestOTP_OneTimePasswordAlgorithm_h
#define TestOTP_OneTimePasswordAlgorithm_h

const int OAUTHLIB_BUFFSIZE_LARGE = 20;

#include <sstream>
#include "HMAC_SHA1.h"
#include <string>

#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

std::string generateOCRA(std::string ocraSuite,
        std::string key,
        std::string counter,
        std::string question,
        std::string password,
        std::string sessionInformation,
        std::string timeStamp) {

    int code_digits = 0;
    std::string crypto = "";
    std::string result;
    int ocraSuiteLength = ocraSuite.size();
    int counterLength = 0;
    int questionLength = 0;
    int passwordLength = 0;
    int sessionInformationLength = 0;
    int timeStampLength = 0;

    vector<string> ocra_suites;
    boost::split(ocra_suites, ocraSuite, boost::is_any_of(":"));

    std::string crypto_function = ocra_suites[1];
    std::string data_input = ocra_suites[2];

    boost::algorithm::to_lower(crypto_function);
    boost::algorithm::to_lower(data_input);

    size_t found;
    found = crypto_function.find("sha1");
    if (found != string::npos) {
        crypto = "HmacSHA1";
    }
    found = crypto_function.find("sha256");
    if (found != string::npos) {
        crypto = "HmacSHA256";
    }
    found = crypto_function.find("sha512");
    if (found != string::npos) {
        crypto = "HmacSHA512";
    }

    vector<string> crypto_function_vector;
    boost::split(crypto_function_vector, crypto_function, boost::is_any_of("-"));
    string number = crypto_function_vector[2];

    code_digits = boost::lexical_cast< int >(number);

    if (data_input.c_str()[0] == 'c') {
        // Fix the length of the HEX string
        while (counter.length() < 16) {
            counter = "0" + counter;
        }
        counterLength = 8;
    }
    // Question - always 128 bytes
    found = crypto_function.find("-q");
    if (data_input.c_str()[0] == 'q'
            || found != string::npos) {
        while (question.length() < 256) {
            question = question + "0";
        }
        questionLength = 128;
    }

    // Password - sha1
    found = crypto_function.find("psha1");
    if (found > 1) {
        while (password.length() < 40) {
            password = "0" + password;
        }
        passwordLength = 20;
    }

    // Password - sha256
    found = crypto_function.find("psha256");
    if (found > 1) {
        while (password.length() < 64) {
            password = "0" + password;
        }
        passwordLength = 32;
    }

    // Password - sha512
    found = crypto_function.find("psha512");
    if (found > 1) {
        while (password.length() < 128) {
            password = "0" + password;
        }
        passwordLength = 64;
    }

    // sessionInformation - s064
    found = crypto_function.find("s064");
    if (found > 1) {
        while (sessionInformation.length() < 128) {
            sessionInformation = "0" + sessionInformation;
        }
        sessionInformationLength = 64;
    }

    // sessionInformation - s128
    found = crypto_function.find("s128");
    if (found > 1) {
        while (sessionInformation.length() < 256) {
            sessionInformation = "0" + sessionInformation;
        }
        sessionInformationLength = 128;
    }

    // sessionInformation - s256
    found = crypto_function.find("s256");
    if (found > 1) {
        while (sessionInformation.length() < 512) {
            sessionInformation = "0" + sessionInformation;
        }
        sessionInformationLength = 256;
    }

    // sessionInformation - s512
    found = crypto_function.find("s512");
    if (found > 1) {
        while (sessionInformation.length() < 1024) {
            sessionInformation = "0" + sessionInformation;
        }
        sessionInformationLength = 512;
    }

    // TimeStamp
    found = crypto_function.find("-t");
    if (data_input.c_str()[0] == 't'
            || found > 1) {
        while (timeStamp.length() < 16) {
            timeStamp = "0" + timeStamp;
        }
        timeStampLength = 8;
    }

    // Remember to add "1" for the "00" byte delimiter
    int msg_lenth = ocraSuiteLength + counterLength + questionLength + passwordLength + sessionInformationLength + timeStampLength + 1;
    char msg [msg_lenth];

    // Put the bytes of "ocraSuite" parameters into the message
    const char* bArray = ocraSuite.c_str();
    ocraSuite.copy(msg, 0, ocraSuite.length());
    // Delimiter
    msg[ocraSuite.length()] = 0x00;
    
    
    
    
    
    
    
    //=======================Continuar===================================================
    // Put the bytes of "Counter" to the message
        // Input is HEX encoded
        if (counterLength > 0) {
            bArray = hexStr2Bytes(counter);
            System.arraycopy(bArray, 0, msg, ocraSuiteLength + 1, bArray.length);
        }


        // Put the bytes of "question" to the message
        // Input is text encoded
        if (questionLength > 0) {
            bArray = hexStr2Bytes(question);
            System.arraycopy(bArray, 0, msg, ocraSuiteLength + 1 + counterLength, bArray.length);
        }

        // Put the bytes of "password" to the message
        // Input is HEX encoded
        if (passwordLength > 0) {
            bArray = hexStr2Bytes(password);
            System.arraycopy(bArray, 0, msg, ocraSuiteLength + 1 + counterLength + questionLength, bArray.length);
        }

        // Put the bytes of "sessionInformation" to the message
        // Input is text encoded
        if (sessionInformationLength > 0) {
            bArray = hexStr2Bytes(sessionInformation);
            System.arraycopy(bArray, 0, msg, ocraSuiteLength + 1 + counterLength + questionLength + passwordLength, bArray.length);
        }

        // Put the bytes of "time" to the message
        // Input is text value of minutes
        if (timeStampLength > 0) {
            bArray = hexStr2Bytes(timeStamp);
            System.arraycopy(bArray, 0, msg, ocraSuiteLength + 1 + counterLength + questionLength + passwordLength + sessionInformationLength, bArray.length);
        }

        bArray = hexStr2Bytes(key);
        byte[] hash = hmac_sha1(crypto, bArray, msg);

        // put selected bytes into result int
        int offset = hash[hash.length - 1] & 0xf;

        int binary = ((hash[offset] & 0x7f) << 24)
                | ((hash[offset + 1] & 0xff) << 16)
                | ((hash[offset + 2] & 0xff) << 8)
                | (hash[offset + 3] & 0xff);

        int otp = binary % DIGITS_POWER[codeDigits];

        result = Integer.toString(otp);
        while (result.length() < codeDigits) {
            result = "0" + result;
        }
        return result;

    return number;

    // How many digits should we return
    //codeDigits = Integer.decode(crypto_function.substring(crypto_function.lastIndexOf("-") + 1)).intValue();

}

std::string generate_otp(BYTE *secret, int secret_length,
        long moving_factor,
        int code_digits,
        int truncation_offset) {

    int digits = code_digits;
    BYTE text[8];

    int text_length = sizeof (text) / sizeof (char);

    for (int i = text_length - 1; i >= 0; i--) {
        text[i] = (moving_factor & 0xff);
        moving_factor = moving_factor >> 8;
    }

    for (int i = 0; i < 8; i++) {
        text[i] = static_cast<char> (text[i]);
        std::cout << (int) text[i] << ",";
    }

    std::cout << std::endl << std::endl;

    unsigned char unsigned_hash[OAUTHLIB_BUFFSIZE_LARGE];

    CHMAC_SHA1 objHMACSHA1;
    objHMACSHA1.HMAC_SHA1(text, text_length, secret, secret_length, unsigned_hash);

    char hash[OAUTHLIB_BUFFSIZE_LARGE];
    for (int i = 0; i < OAUTHLIB_BUFFSIZE_LARGE; i++) {
        hash[i] = static_cast<char> (unsigned_hash[i]);
        std::cout << (int) hash[i] << ",";
    }

    std::cout << std::endl;

    int hash_length = sizeof (hash) / sizeof (char);
    int offset = hash[hash_length - 1] & 0xf;

    if ((0 <= truncation_offset) &&
            (truncation_offset < (hash_length - 4))) {
        offset = truncation_offset;
    }

    int binary =
            ((hash[offset] & 0x7f) << 24)
            | ((hash[offset + 1] & 0xff) << 16)
            | ((hash[offset + 2] & 0xff) << 8)
            | (hash[offset + 3] & 0xff);

    int DIGITS_POWER[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

    int otp = binary % DIGITS_POWER[code_digits];

    std::stringstream result_stream;
    result_stream << otp;

    std::string result = result_stream.str();

    while (result.length() < digits) {
        result = "0" + result;
    }

    return result;

}

#endif








































