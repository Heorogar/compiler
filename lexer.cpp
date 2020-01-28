#include"lexer.h"

void cmplr_::lexer::print(cmplr_::lexer::treeNode *n) {
	if (n == NULL)
		return;

	for (std::string a : n->stringsTable)
		std::cout << a << std::endl;

	treeNode::symbolTableNode *temp = n->symbolTableHeadNode;

	while (temp) {
		std::cout << temp->tokenValue;
//		std::cout << (int) temp->tokenType << std::endl;
		temp = temp->nextNode;
	}
	std::cout << std::endl;
	print(n->listOfChildrenHeadNode->child);
}

cmplr_::lexer::lexer(std::string inputFilePath) {
	lexerRootNode = scanFile(inputFilePath);
	std::cout << "here begineth the book of the tales of symbol table"
			<< std::endl;
	print(lexerRootNode);

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
	treeNode *tree = buildTree(&inputStream, root);
	inputStream.close();
	return tree;
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
				while (temp->nextNode) {
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
			addWordNode(inputStream, &root, inputChar, &temp);
		} else if (inputChar == '+' || inputChar == '-' || inputChar == '*'
				|| inputChar == '/') {
			keywordSoFar += inputChar;
			temp = temp->insert(temp, root, keywordSoFar,
					keywords::arithmetic_operators);
		} else if (inputChar == '=') {
			keywordSoFar += inputChar;
			temp = temp->insert(temp, root, keywordSoFar,
					keywords::assignment_operator);
		} else if (inputChar == '"') {
			keywordSoFar += inputChar;
			temp = temp->insert(temp, root, keywordSoFar, keywords::quote);
			keywordSoFar = "";
			while (inputStream->peek() != '"') {
				inputChar = inputStream->get();
				keywordSoFar += inputChar;
			}

			if (keywordSoFar != "") {
				temp = temp->insert(temp, root, keywordSoFar,
						keywords::string_literal);

				inputChar = inputStream->get();
				keywordSoFar = "";
				keywordSoFar += inputChar;

				temp = temp->insert(temp, root, keywordSoFar, keywords::quote);
			}
		} else if (inputChar == '>' || inputChar == '<') {
			keywordSoFar += inputChar;
			temp = temp->insert(temp, root, keywordSoFar,
					keywords::relational_operators);
		} else if (inputChar >= 48 && inputChar <= 57) {

			keywordSoFar += inputChar;

			while (inputStream->peek() >= 48 && inputStream->peek() <= 57) {
				inputChar = inputStream->get();
				keywordSoFar += inputChar;
			}
			temp = temp->insert(temp, root, keywordSoFar, keywords::number);

		} else if (inputChar == '(' || inputChar == ')') {
			keywordSoFar += inputChar;
			temp = temp->insert(temp, root, keywordSoFar,
					keywords::parenthesis);
		} else if (inputChar == ';') {
			keywordSoFar += inputChar;
			temp = temp->insert(temp, root, keywordSoFar,
					keywords::eol_operator);
		}
	}

	return root;
}

//return root or return shit
void cmplr_::lexer::addWordNode(std::ifstream *inputStream, treeNode **root,
		char inputChar, treeNode::symbolTableNode **temp) {
	std::string keywordSoFar = "";
	keywordSoFar += inputChar;

	while ((inputStream->peek() >= 97 && inputStream->peek() <= 122)
			|| (inputStream->peek() >= 65 && inputStream->peek() <= 90)) {
		inputChar = inputStream->get();
		keywordSoFar += inputChar;

	}

	std::cout << keywordSoFar << std::endl;

	if (keywordSoFar == "if" || keywordSoFar == "while"
			|| keywordSoFar == "print")
		*temp = (*temp)->insert(*temp, *root, keywordSoFar,
				keywords::reserved_words);

	else if (keywordSoFar == "int" || keywordSoFar == "dec"
			|| keywordSoFar == "string")
		*temp = (*temp)->insert(*temp, *root, keywordSoFar,
				keywords::primitive_types);

	else {
		*temp = (*temp)->insert(*temp, *root, keywordSoFar,
				keywords::identifiers);
//		std::unordered_set<std::string>::iterator i =
//				(*root)->stringsTable.find(keywordSoFar);
		if ((*root)->stringsTable.find(keywordSoFar)
				== (*root)->stringsTable.end())
			(*root)->stringsTable.emplace(keywordSoFar);
	}

}

cmplr_::lexer::treeNode::symbolTableNode* cmplr_::lexer::treeNode::symbolTableNode::insert(
		treeNode::symbolTableNode *head, treeNode *root, std::string s,
		keywords k) {
	if (head == NULL) {
		root->symbolTableHeadNode = new treeNode::symbolTableNode(s, k);
		return head = root->symbolTableHeadNode;
	}
	head->nextNode = new treeNode::symbolTableNode(s, k);
	return head->nextNode;
}

int main() {
	cmplr_::lexer* l=new cmplr_::lexer("/home/invictus/eclipse-workspace/compiler/src/test.txt");
	delete l;
}
