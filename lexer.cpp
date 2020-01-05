#include"lexer.h"
//#include<char_traits>

void cmplr_::lexer::print(cmplr_::lexer::treeNode *n) {
	if (n == NULL)
		return;
	treeNode::symbolTableNode *temp = n->symbolTableHeadNode;
	while (temp != NULL) {

//			std::cout << i.k << std::endl;
		std::cout << temp->tokenValue;
		temp = temp->nextNode;
	}
	std::cout << std::endl;
	print(n->listOfChildrenHeadNode->child);
}

cmplr_::lexer::lexer(std::string inputFilePath) {
	treeNode *n = scanFile(inputFilePath);
	std::cout << "here begineth the book of the tales of symbol table"
			<< std::endl;
	print(n);

}

cmplr_::lexer::treeNode::symbolTableNode::symbolTableNode(
		std::string tokenValue, keywords tokenType) :
		tokenType(tokenType), tokenValue(tokenValue), nextNode(NULL) {
}

cmplr_::lexer::treeNode::childNode::childNode() :
		child(NULL), nextNode(NULL) {
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
	treeNode::symbolTableNode *temp = root->symbolTableHeadNode;

	while (!inputStream->eof()) {

		std::cout << "while" << std::endl;

		char inputChar = inputStream->get();
		std::cout << inputChar << std::endl;
		std::string keywordSoFar = "";

		if (inputChar == '{') {
//			treeNode *n = new treeNode;
//			root->next = n;
			if (root->listOfChildrenHeadNode == NULL) {
				root->listOfChildrenHeadNode = new treeNode::childNode;
				root->listOfChildrenHeadNode->child = buildTree(inputStream,
						new treeNode);
			} else {
				treeNode::childNode *temp = root->listOfChildrenHeadNode;
				while (temp->nextNode != NULL) {
					temp = temp->nextNode;
				}
				temp->nextNode = new treeNode::childNode;
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
		treeNode **root, char inputChar, treeNode::symbolTableNode **temp) {
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

cmplr_::lexer::treeNode::symbolTableNode* cmplr_::lexer::insert(
		treeNode::symbolTableNode *head, treeNode *root, std::string s,
		keywords k) {
//	if (root == NULL)
//		root = new listNode(s, k, NULL);
//	listNode *temp = root;
//	while (temp->next != NULL)
//		temp = temp->next;
//	temp->next = new listNode(s, k, NULL);
//	return root;
	if (root->symbolTableHeadNode == NULL) {
		root->symbolTableHeadNode = new treeNode::symbolTableNode(s, k);
		return head = root->symbolTableHeadNode;
	}
	head->nextNode = new treeNode::symbolTableNode(s, k); //third argument not needed?
	return head->nextNode;
}

int main() {
	cmplr_::lexer l("/home/invictus/eclipse-workspace/compiler/src/test.txt");

}
