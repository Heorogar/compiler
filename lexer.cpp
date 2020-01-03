#include"lexer.h"

cmplr_::lexer::lexer() {
}

cmplr_::lexer::treeNode* cmplr_::lexer::scanFile(std::string inputFilePath) {
	treeNode *root = new treeNode;

	std::ifstream inputStream;
	inputStream.open(inputFilePath, std::ifstream::in);
	if (!inputStream)
		std::cout << "exception bro" << std::endl;

	return buildTree(&inputStream, root);
}

cmplr_::lexer::treeNode* cmplr_::lexer::buildTree(std::ifstream *inputStream,
		treeNode *root) {

	while (inputStream->good()) {

		std::cout << "while" << std::endl;

		char inputChar = inputStream->get();
		std::cout << inputChar << std::endl;
		std::string keywordSoFar = "";

		if (inputChar == '{') {
			treeNode *n = new treeNode;
			root->next = n;
			return buildTree(inputStream, n);
		}
		if (inputChar == '}') {
			return root;
		}
		if ((inputChar >= 97 && inputChar <= 122)
				|| (inputChar >= 65 && inputChar <= 90)) {
			inputStream = addIdNode(inputStream, root, inputChar);
		} else if (inputChar == '+' || inputChar == '-' || inputChar == '*'
				|| inputChar == '/') {
			keywordSoFar += inputChar;
			root->symbolTable.insert(
					std::pair<std::string, keywords>(keywordSoFar,
							keywords::arithmetic_operators));
		} else if (inputChar == '=') {
			keywordSoFar += inputChar;
			root->symbolTable.insert(
					std::pair<std::string, keywords>(keywordSoFar,
							keywords::assignment_operator));
		} else if (inputChar == '"') {
			keywordSoFar += inputChar;
			root->symbolTable.insert(
					std::pair<std::string, keywords>(keywordSoFar,
							keywords::quote));
			keywordSoFar = "";
			while (inputStream->peek() != '"') {
				inputChar = inputStream->get();
				keywordSoFar += inputChar;
			}
			if (keywordSoFar != "")
				root->symbolTable.insert(
						std::pair<std::string, keywords>(keywordSoFar,
								keywords::string_literal));
			inputChar = inputStream->get();
			root->symbolTable.insert(
					std::pair<std::string, keywords>(keywordSoFar,
							keywords::quote));

		} else if (inputChar == '>' || inputChar == '<') {
			keywordSoFar += inputChar;
			root->symbolTable.insert(
					std::pair<std::string, keywords>(keywordSoFar,
							keywords::relational_operators));
		} else if (inputChar >= 48 && inputChar <= 57) {

			keywordSoFar += inputChar;

			while (inputStream->peek() >= 48 && inputStream->peek() <= 57) {
				inputChar = inputStream->get();
				keywordSoFar += inputChar;
			}
			root->symbolTable.insert(
					std::pair<std::string, keywords>(keywordSoFar,
							keywords::number));

		} else if (inputChar == '(' || inputChar == ')') {
			keywordSoFar += inputChar;
			root->symbolTable.insert(
					std::pair<std::string, keywords>(keywordSoFar,
							keywords::parenthesis));
		} else if (inputChar == ';') {
			keywordSoFar += inputChar;
			root->symbolTable.insert(
					std::pair<std::string, keywords>(keywordSoFar,
							keywords::eol_operator));
		}
	}
	inputStream->close();
	return root;
}

std::ifstream* cmplr_::lexer::addIdNode(std::ifstream *inputStream,
		treeNode *root, char inputChar) {
	std::string keywordSoFar = "";
	keywordSoFar += inputChar;

//	std::cout << keywordSoFar<< std::endl;

//	inputChar = inputStream->get();

	while ((inputStream->peek() >= 97 && inputStream->peek() <= 122)
			|| (inputStream->peek() >= 65 && inputStream->peek() <= 90)) {
		inputChar = inputStream->get();
		keywordSoFar += inputChar;

	}

	std::cout << keywordSoFar << std::endl;

	if (keywordSoFar == "if" || keywordSoFar == "while" || "print")
		root->symbolTable.insert(
				std::pair<std::string, keywords>(keywordSoFar,
						keywords::reserved_words));

	else if (keywordSoFar == "int" || "dec" || "string")
		root->symbolTable.insert(
				std::pair<std::string, keywords>(keywordSoFar,
						keywords::primitive_types));

	else {
		root->symbolTable.insert(
				std::pair<std::string, keywords>(keywordSoFar,
						keywords::identifiers));
		std::unordered_set<std::string>::iterator i = root->stringsTable.find(
				keywordSoFar);
		if (i != root->stringsTable.end())
			root->stringsTable.emplace(*i);
	}

	return inputStream;
}

int main() {
	cmplr_::lexer l;
	cmplr_::lexer::treeNode *n = l.scanFile(
			"/home/invictus/eclipse-workspace/compiler/src/test.txt");
	l.print(n);

}
