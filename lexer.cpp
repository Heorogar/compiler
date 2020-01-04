#include"lexer.h"
//#include<char_traits>

cmplr_::lexer::lexer() {
}

cmplr_::lexer::treeNode* cmplr_::lexer::scanFile(std::string inputFilePath) {
	treeNode *root = new treeNode;

	std::ifstream inputStream;
	inputStream.open(inputFilePath, std::ifstream::in);
	if (!inputStream)
		std::cout << "exception bro" << std::endl;
	treeNode *r = buildTree(&inputStream, root);
	inputStream.close();
	return r;
}

cmplr_::lexer::treeNode* cmplr_::lexer::buildTree(std::ifstream *inputStream,
		treeNode *root) {
	listNode *temp = root->symbolTable;

	while (!inputStream->eof()) {

		std::cout << "while" << std::endl;

		char inputChar = inputStream->get();
		std::cout << inputChar << std::endl;
		std::string keywordSoFar = "";

		if (inputChar == '{') {
//			treeNode *n = new treeNode;
//			root->next = n;
			if (root->childList == NULL) {
				root->childList = new treeNode::childNode;
				root->childList->child = buildTree(inputStream, new treeNode);
			} else {
				treeNode::childNode *temp = root->childList;
				while (temp->next != NULL) {
					temp = temp->next;
				}
				temp->next = new treeNode::childNode;
				temp->child = buildTree(inputStream, new treeNode);
			}
		}
		if (inputChar == '}') {
			return root;
		}
		if ((inputChar >= 97 && inputChar <= 122)
				|| (inputChar >= 65 && inputChar <= 90)) {
			inputStream = addIdNode(inputStream, &root, inputChar, &temp);
		} else if (inputChar == '+' || inputChar == '-' || inputChar == '*'
				|| inputChar == '/') {
			keywordSoFar += inputChar;
//			root->symbolTable.insert(
//					std::pair<std::string, keywords>(keywordSoFar,
//							keywords::arithmetic_operators));
			temp = insert(temp, root, keywordSoFar,
					keywords::arithmetic_operators);
		} else if (inputChar == '=') {
			keywordSoFar += inputChar;
			temp = insert(temp, root, keywordSoFar,
					keywords::assignment_operator);
		} else if (inputChar == '"') {
			keywordSoFar += inputChar;
			temp = insert(temp, root, keywordSoFar, keywords::quote);
			keywordSoFar = "";
			while (inputStream->peek() != '"') {
				inputChar = inputStream->get();
				keywordSoFar += inputChar;
			}

			if (keywordSoFar != "") {
				temp = insert(temp, root, keywordSoFar,
						keywords::string_literal);

				inputChar = inputStream->get();
				keywordSoFar = "";
				keywordSoFar += inputChar;

				temp = insert(temp, root, keywordSoFar, keywords::quote);
			}
		} else if (inputChar == '>' || inputChar == '<') {
			keywordSoFar += inputChar;
			temp = insert(temp, root, keywordSoFar,
					keywords::relational_operators);
		} else if (inputChar >= 48 && inputChar <= 57) {

			keywordSoFar += inputChar;

			while (inputStream->peek() >= 48 && inputStream->peek() <= 57) {
				inputChar = inputStream->get();
				keywordSoFar += inputChar;
			}
			temp = insert(temp, root, keywordSoFar, keywords::number);

		} else if (inputChar == '(' || inputChar == ')') {
			keywordSoFar += inputChar;
			temp = insert(temp, root, keywordSoFar, keywords::parenthesis);
		} else if (inputChar == ';') {
			keywordSoFar += inputChar;
			temp = insert(temp, root, keywordSoFar, keywords::eol_operator);
		}
	}

	return root;
}

//return root or return shit
std::ifstream* cmplr_::lexer::addIdNode(std::ifstream *inputStream,
		treeNode **root, char inputChar, listNode **temp) {
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
		*temp = insert(*temp, *root, keywordSoFar, keywords::reserved_words);

	else if (keywordSoFar == "int" || "dec" || "string")
		*temp = insert(*temp, *root, keywordSoFar, keywords::primitive_types);

	else {
		*temp = insert(*temp, *root, keywordSoFar, keywords::identifiers);
		std::unordered_set<std::string>::iterator i =
				(*root)->stringsTable.find(keywordSoFar);
		if (i != (*root)->stringsTable.end())
			(*root)->stringsTable.emplace(*i);
	}

	return inputStream;
}

cmplr_::lexer::listNode* cmplr_::lexer::insert(listNode *head, treeNode *root,
		std::string s, keywords k) {
//	if (root == NULL)
//		root = new listNode(s, k, NULL);
//	listNode *temp = root;
//	while (temp->next != NULL)
//		temp = temp->next;
//	temp->next = new listNode(s, k, NULL);
//	return root;
	if (root->symbolTable == NULL) {
		root->symbolTable = new listNode(s, k, NULL);
		return head = root->symbolTable;
	}
	head->next = new listNode(s, k, NULL); //third argument not needed?
	return head->next;
}

int main() {
	cmplr_::lexer l;
	cmplr_::lexer::treeNode *n = l.scanFile(
			"/home/invictus/eclipse-workspace/compiler/src/test.txt");
	std::cout << "here begineth the book of the tales of symbol table"
			<< std::endl;
	l.print(n);

}
