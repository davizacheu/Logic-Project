#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Token.h"
using namespace std;

class Scanner {
private:
    string input;
    vector<Token> tokenList;
    int line = 1;

public:
    Scanner(const string& input) : input(input) { }

    void PrintTotalNum(){
        cout << "Total Tokens = " << tokenList.size() << endl;
    }
    string StringBuilder(string &token){
        while (input.size() >= 1 && input.at(0) != '\''){
            if(input.at(0)=='\n')++line;
            token += input.at(0);
            input = input.substr(1);
        }
        return token;
    }

    void CreateList(){
        while(!input.empty()){
            if (isspace(input.at(0))){
                if (input.at(0) == '\n'){
                    line++;
                }
                input = input.substr(1);
            }
            else{
                Token t = scanToken();
                if(t.GetTokenType()!= COMMENT){
                  tokenList.push_back(t);
                }
            }
        }
        Token end = Token(eof,"",line);
        tokenList.push_back(end);
    }

    Token scanToken(){
//        cout << "Current input string~" << input << endl;
        if (input.at(0) == ',') {
            Token t = Token(COMMA,"," , line);
            input = input.substr(1);
            return t;

        }
        else if (input.at(0) == '.') {
            Token t = Token(PERIOD, ".", line);
            input = input.substr(1);
            return t;
        }
        else if (input.at(0) == '?') {
            Token t = Token(Q_MARK, "?", line);
            input = input.substr(1);
            return t;
        }
        else if (input.at(0) == '(') {
            Token t = Token(LEFT_PAREN, "(", line);
            input = input.substr(1);
            return t;
        }
        else if (input.at(0) == ')') {
            Token t = Token(RIGHT_PAREN, ")", line);
            input = input.substr(1);
            return t;
        }
        else if (input.at(0) == ':') {
            if (input.size() >= 2 && input.at(1) == '-') {
                Token t = Token(COLON_DASH, ":-", line);
                input = input.substr(2);
                return t;
            }
            else
            {
                Token t = Token(COLON, ":", line);
                input = input.substr(1);
                return t;

            }
        }
        else if (input.at(0) == '*') {
            Token t = Token(MULTIPLY, "*", line);
            input = input.substr(1);
            return t;
        }
        else if (input.at(0) == '+') {
            Token t = Token(ADD, "+", line);
            input = input.substr(1);
            return t;
        }
        else if(isalpha(input.at(0))) {
            string nextToken = "";
            while (input.size() >= 1 && isalnum(input.at(0))) {
                if (input.at(0) == '\n')++line;
                nextToken += input.at(0);
                input = input.substr(1);
            }

            if (nextToken == "Schemes") {
                Token t = Token(SCHEMES, nextToken, line);
                return t;
            } else if (nextToken == "Facts") {
                Token t = Token(FACTS, nextToken, line);
                return t;

            } else if (nextToken == "Rules") {
                Token t = Token(RULES, nextToken, line);
                return t;
            } else if(nextToken == "Queries") {
                    Token t = Token(QUERIES, nextToken, line);
                    return t;
            } else {
                Token t = Token(ID, nextToken, line);
                return t;
            }

        }
        else if (input.at(0) == '\''){
            string nextToken = "";
            int strline = line;
            nextToken += input.at(0);
            input = input.substr(1);
            StringBuilder(nextToken);
            if(input.size()>=2 && input.at(1) == '\''){
                for(int j = 0; j < 2; j++){
                    nextToken += input.at(0);
                    input = input.substr(1);
                }
                StringBuilder(nextToken);

            }

            if(input.size()>=1 && input.at(0)=='\''){
                nextToken += input.at(0);
                Token t = Token(STRING, nextToken,strline);
                input = input.substr(1);
                return t;
            }
            else{
                Token t = Token(UNDEFINED,nextToken,strline);
                return t;
            }

        }
        else if(input.at(0)=='#'){
            string comment = "";
            int comline = line;
            comment += input.at(0);
            input = input.substr(1);
            if(input.size() >= 1 && input.at(0)=='|'){
                while(input.size() >= 2 && !(input.at(0)=='|' && input.at(1)=='#')){
                    if(input.at(0)=='\n')++line;
                    comment += input.at(0);
                    input = input.substr(1);
                }
                if(input.size() >= 2 && input.at(0)=='|' && input.at(1)=='#'){
                    for(int k =0; k < 2; k++){
                        comment += input.at(0);
                        input = input.substr(1);
                    }
                    Token t = Token(COMMENT, comment, comline);
                    return t;
                }
                else{
                    comment += input.at(0);
                    Token t = Token(UNDEFINED, comment, comline);
                    input = input.substr(1);
                    return t;
                }
            }
            else{
                while(input.size() >= 1 && input.at(0) != '\n'){
                    comment += input.at(0);
                    input = input.substr(1);
                }
                Token t = Token(COMMENT,comment,comline);
                return t;
            }
        }
        else{
            string undef = "";
            undef += input.at(0);
            Token t = Token(UNDEFINED,undef, line);
            input = input.substr(1);
            return t;
        }

    }

    void PrintList(){
        for(unsigned int i = 0; i < tokenList.size();i++){
            cout << tokenList[i].toString() << endl;
        }

    }

    vector<Token> getTokenList(){
      return tokenList;
    }

};
