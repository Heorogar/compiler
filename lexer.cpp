#include"lexer.h"
//#pragma once
void cmplr_::lexer::print(cmplr_::lexer::treeNode *n) {
	if (n == NULL)
		return;

//	for (std::string a : n->stringsTable)
//		std::cout << a << std::endl;

	treeNode::symbolTableNode *temp = n->symbolTableHeadNode;

	while (temp) {
		std::cout << temp->tokenValue;
//		std::cout << (int) temp->tokenType << std::endl;
		temp = temp->nextNode;
	}
	std::cout << std::endl;
	if (n->listOfChildrenHeadNode)
		print(n->listOfChildrenHeadNode->child);
	else
		return;
	if (n->listOfChildrenHeadNode->nextNode)
		print(n->listOfChildrenHeadNode->nextNode->child);
	else
		return;
}

cmplr_::lexer::lexer(std::string inputFilePath) {
	lexerRootNode = scanFile(inputFilePath);
	std::cout << "here begineth the book of the tales of symbol table"
			<< std::endl;
//	print(lexerRootNode);

}

cmplr_::lexer::~lexer() {
	delete lexerRootNode;
}

cmplr_::lexer::treeNode::treeNode() :
		listOfChildrenHeadNode(NULL), symbolTableHeadNode(NULL) {

}

cmplr_::lexer::treeNode::~treeNode() {
//delete is recursive
	delete symbolTableHeadNode;
	delete listOfChildrenHeadNode;
}

cmplr_::lexer::treeNode::symbolTableNode::symbolTableNode(
		std::string tokenValue, keywords tokenType) :
		tokenType(tokenType), tokenValue(tokenValue), nextNode(NULL) {
}
cmplr_::lexer::treeNode::symbolTableNode::~symbolTableNode() {
	delete nextNode;
}

cmplr_::lexer::treeNode::childNode::childNode() :
		child(NULL), nextNode(NULL) {
}

cmplr_::lexer::treeNode::childNode::~childNode() {
	delete child;
	delete nextNode;
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
				temp->nextNode->child = buildTree(inputStream, new treeNode);
			}
		}
		if (inputChar == '}') {
			return root;
		}
		if ((inputChar >= 97 && inputChar <= 122)
				|| (inputChar >= 65 && inputChar <= 90)) {
//			addWordNode(inputStream, &root, inputChar, &temp);
			word w;
			temp = w.insertTreeNode(root, temp, inputStream, inputChar);
		} else if (inputChar == '+' || inputChar == '-' || inputChar == '*'
				|| inputChar == '/') {
//			keywordSoFar += inputChar;
//			temp = temp->insert(temp, root, keywordSoFar,
//					keywords::arithmetic_operators);
			arithmeticOperators a;
			temp = a.insertTreeNode(root, temp, inputStream, inputChar);
		} else if (inputChar == '=') {
//			keywordSoFar += inputChar;
//			temp = temp->insert(temp, root, keywordSoFar,
//					keywords::assignment_operator);
			assignmentOperator p;
			temp = p.insertTreeNode(root, temp, inputStream, inputChar);
		} else if (inputChar == '"') {
//			keywordSoFar += inputChar;
//			temp = temp->insert(temp, root, keywordSoFar, keywords::quote);
//			keywordSoFar = "";
//			while (inputStream->peek() != '"') {
//				inputChar = inputStream->get();
//				keywordSoFar += inputChar;
//			}
//
//			if (keywordSoFar != "") {
//				temp = temp->insert(temp, root, keywordSoFar,
//						keywords::string_literal);
//
//				inputChar = inputStream->get();
//				keywordSoFar = "";
//				keywordSoFar += inputChar;
//
//				temp = temp->insert(temp, root, keywordSoFar, keywords::quote);
//			}
			quote q;
			temp = q.insertTreeNode(root, temp, inputStream, inputChar);
			stringLiteral l;
			inputChar = inputStream->get();
			temp = l.insertTreeNode(root, temp, inputStream, inputChar); //garbage-in-garbage out principle; if the second quote is not found out comes the garbage
			inputChar = inputStream->get();
			temp = q.insertTreeNode(root, temp, inputStream, inputChar);
		} else if (inputChar == '>' || inputChar == '<') {
//			keywordSoFar += inputChar;
//			temp = temp->insert(temp, root, keywordSoFar,
//					keywords::relational_operators);
			relationalOperator r;
			temp = r.insertTreeNode(root, temp, inputStream, inputChar);
		} else if (inputChar >= 48 && inputChar <= 57) {
//
//			keywordSoFar += inputChar;
//
//			while (inputStream->peek() >= 48 && inputStream->peek() <= 57) {
//				inputChar = inputStream->get();
//				keywordSoFar += inputChar;
//			}
//			temp = temp->insert(temp, root, keywordSoFar, keywords::number);
			number n;
			temp = n.insertTreeNode(root, temp, inputStream, inputChar);
		} else if (inputChar == '(' || inputChar == ')') {
//			keywordSoFar += inputChar;
//			temp = temp->insert(temp, root, keywordSoFar,
//					keywords::parenthesis);
			parethesis g;
			temp = g.insertTreeNode(root, temp, inputStream, inputChar);
		} else if (inputChar == ';') {
//			keywordSoFar += inputChar;
//			temp = temp->insert(temp, root, keywordSoFar,
//					keywords::eol_operator);
			eolOperator e;
			temp = e.insertTreeNode(root, temp, inputStream, inputChar);
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

class a {
public:
	a() {
		std::cout << "a constructor" << std::endl;
	}
	~a() {
		std::cout << "a destructor" << std::endl;
	}
};

class b: public a {
public:
	b() {
		std::cout << "b constructor" << std::endl;
	}
	~b() {
		std::cout << "b destructor" << std::endl;
	}
};

int main() {
//	cmplr_::lexer *l = new cmplr_::lexer(
//			"/home/invictus/eclipse-workspace/compiler/src/test.txt");
//	l->print(l->lexerRootNode);
//	delete l;
//	std::cout << "foo" << std::endl;
	a foo = b();
	std::cout << "scope end" << std::endl;
}

cmplr_::lexer::treeNode::symbolTableNode* cmplr_::lexer::lexeme::insertTreeNodeDefault(
		treeNode *root, treeNode::symbolTableNode *head, std::string tokenValue,
		keywords tokenType) {
	if (head == NULL) {
		root->symbolTableHeadNode = new treeNode::symbolTableNode(tokenValue,
				tokenType);
		return head = root->symbolTableHeadNode;
	}
	head->nextNode = new treeNode::symbolTableNode(tokenValue, tokenType);
	return head->nextNode;

}
