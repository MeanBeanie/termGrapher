#include "tokenizer.hpp"
#include <queue>
#include <stack>

void addToken(std::string& raw, Type& type, std::vector<Token>& tokens){
	Token token;
	token.raw = raw;
	token.type = type;
	if(token.type > WORD && token.type < ADD && token.type != X){
		token.value = std::stof(raw);
	}
	else{
		token.value = 0;
	}

	tokens.push_back(token);
	raw.clear();
	type = NONE;
}

std::queue<Token> tokenize(std::string str){
	std::vector<Token> tokens;
	std::string word;
	Type wordType = NONE;

	for(int i = 0; i < str.size(); i++){
		if(str[i] == 'x'){
			if(wordType != NONE){
				addToken(word, wordType, tokens);
			}
			word.push_back(str[i]);
			wordType = X;
			addToken(word, wordType, tokens);
		}
		else if(isalpha(str[i])){
			if(wordType > WORD){
				Token token("*", MUL, 0);
				tokens.push_back(token);
				addToken(word, wordType, tokens);
			}
			wordType = WORD;
			word.push_back(str[i]);
		}
		else if(isdigit(str[i])){
			if(wordType == WORD || wordType >= ADD){
				addToken(word, wordType, tokens);
			}
			wordType = wordType != DECIMAL ? NUMBER : DECIMAL;
			word.push_back(str[i]);
		}
		else if(str[i] == '.'){
			if(wordType == NONE || wordType == NUMBER){
				wordType = DECIMAL;
				if(wordType == NONE){
					word.push_back('0');
				}
				word.push_back(str[i]);
			}
		}
		else if(str[i] == '+'){
			if(wordType != NONE){
				addToken(word, wordType, tokens);
			}
			wordType = ADD;
			word.push_back(str[i]);
			addToken(word, wordType, tokens);
		}
		else if(str[i] == '-'){
			if(wordType != NONE){
				addToken(word, wordType, tokens);
			}
			wordType = SUB;
			word.push_back(str[i]);
			addToken(word, wordType, tokens);
		}
		else if(str[i] == '*'){
			if(wordType != NONE){
				addToken(word, wordType, tokens);
			}
			wordType = MUL;
			word.push_back(str[i]);
			addToken(word, wordType, tokens);
		}
		else if(str[i] == '/'){
			if(wordType != NONE){
				addToken(word, wordType, tokens);
			}
			wordType = DIV;
			word.push_back(str[i]);
			addToken(word, wordType, tokens);
		}
		else if(str[i] == '^'){
			if(wordType != NONE){
				addToken(word, wordType, tokens);
			}
			wordType = EXPONENT;
			word.push_back(str[i]);
			addToken(word, wordType, tokens);
		}
		else if(str[i] == '(' || str[i] == ')'){
			if(wordType != NONE){
				addToken(word, wordType, tokens);
			}
			word.push_back(str[i]);
			wordType = PARENTHESIS;
			addToken(word, wordType, tokens);
		}
		else if(str[i] == ' ' || str[i] == '\t'){
			continue;
		}
	}
	if(wordType != NONE){
		addToken(word, wordType, tokens);
	}

	// runs the shunting yard algorithm to put the math in infix notation
	std::queue<Token> outputQueue;
	std::stack<Token> operatorStack;

	for(int i = 0; i < tokens.size(); i++){
		if(tokens[i].type == NUMBER || tokens[i].type == DECIMAL || tokens[i].type == X){
			outputQueue.push(tokens[i]);
		}
		else if(tokens[i].type == WORD){
			operatorStack.push(tokens[i]);
		}
		else if(tokens[i].type >= ADD && tokens[i].type < PARENTHESIS){
			while(operatorStack.size() > 0 && operatorStack.top().type >= tokens[i].type && operatorStack.top().raw != "("){
				outputQueue.push(operatorStack.top());
				operatorStack.pop();
			}
			operatorStack.push(tokens[i]);
		}
		else if(tokens[i].type == PARENTHESIS){
			if(tokens[i].raw == "("){
				operatorStack.push(tokens[i]);
			}
			else{
				while(operatorStack.top().raw != "(" && operatorStack.size()-1 > 0){
					outputQueue.push(operatorStack.top());
					operatorStack.pop();
				}
				if(operatorStack.top().type != PARENTHESIS || operatorStack.top().raw != "("){
					// TODO FUCK PANIC SHE SHOWS HER ANKLES
				}
				// pops the left parenthesis from the operator stack, it has been used
				operatorStack.pop();
				if(operatorStack.size() > 0 && operatorStack.top().type == WORD){
					outputQueue.push(operatorStack.top());
					operatorStack.pop();
				}
			}
		}
	}
	while(operatorStack.size() != 0){
		if(operatorStack.top().type == PARENTHESIS){
			// TODO FUCK PANIC WE DIE
		}
		outputQueue.push(operatorStack.top());
		operatorStack.pop();
	}

	return outputQueue;
}
