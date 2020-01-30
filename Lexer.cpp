#include "Lexer.h"

void cmplr_perseus::Lexer::printTree(
		const cmplr_perseus::Lexer::TreeNode *n) const {
	if (n == NULL)
		return;
	TreeNode::SymbolTableNode *temp = n->symbolTableHeadNode;
	std::cout << std::endl;
	std::cout << "Tree node" << std::endl;
	for (std::string setElement : n->stringsTable)
		std::cout << "Hashed identifier: " << setElement << std::endl;
	while (temp) {
		std::cout << temp->tokenValue << " ";
		std::cout << "(Token type: " << (int) temp->tokenType << ") ";
		temp = temp->nextNode;
	}
	std::cout << std::endl;
	if (n->listOfChildrenHeadNode) {
		printTree(n->listOfChildrenHeadNode->child);
		delete temp;
	} else {
		return;
		delete temp;
	}
	if (n->listOfChildrenHeadNode->nextNode) {
		printTree(n->listOfChildrenHeadNode->nextNode->child);
		delete temp;
	} else {
		return;
		delete temp;
	}
}

cmplr_perseus::Lexer::Lexer(std::string inputFilePath) {
	lexerRootNode = scanFile(inputFilePath);
	std::cout << "Here bygynneth the Book of the Tales of Symbol Table"
			<< std::endl;
}

cmplr_perseus::Lexer::~Lexer() {
	delete lexerRootNode;
}

cmplr_perseus::Lexer::TreeNode::TreeNode() :
		listOfChildrenHeadNode(NULL), symbolTableHeadNode(NULL) {
}

cmplr_perseus::Lexer::TreeNode::~TreeNode() {
	delete symbolTableHeadNode;
	delete listOfChildrenHeadNode;
}

cmplr_perseus::Lexer::TreeNode::SymbolTableNode::SymbolTableNode(
		std::string tokenValue, keywords tokenType) :
		tokenType(tokenType), tokenValue(tokenValue), nextNode(NULL) {
}

cmplr_perseus::Lexer::TreeNode::SymbolTableNode::~SymbolTableNode() {
	delete nextNode;
}

cmplr_perseus::Lexer::TreeNode::ChildNode::ChildNode() :
		child(NULL), nextNode(NULL) {
}

cmplr_perseus::Lexer::TreeNode::ChildNode::~ChildNode() {
	delete child;
	delete nextNode;
}

cmplr_perseus::Lexer::TreeNode* cmplr_perseus::Lexer::scanFile(
		std::string inputFilePath) {
	std::ifstream inputStream;
	inputStream.open(inputFilePath, std::ifstream::in);
	if (!inputStream)
		throw "File not found!";
	TreeNode *root = new TreeNode;
	root = buildTree(&inputStream, root);
	inputStream.close();
	return root;
}

cmplr_perseus::Lexer::TreeNode* cmplr_perseus::Lexer::buildTree(
		std::ifstream *inputStream, TreeNode *root) {
	TreeNode::SymbolTableNode *temp = root->symbolTableHeadNode;
	while (!inputStream->eof()) {
		Lexeme *lexemeType = NULL; //in order to increase efficiency the pointer tracks the postion of traversed
		char inputChar = inputStream->get();
		if (inputChar == '{') {
			if (root->listOfChildrenHeadNode == NULL) {
				root->listOfChildrenHeadNode = new TreeNode::ChildNode;
				root->listOfChildrenHeadNode->child = buildTree(inputStream,
						new TreeNode);
			} else {
				TreeNode::ChildNode *temp = root->listOfChildrenHeadNode;
				while (temp->nextNode) {
					temp = temp->nextNode;
				}
				temp->nextNode = new TreeNode::ChildNode;
				temp->nextNode->child = buildTree(inputStream, new TreeNode);
			}
		}
		if (inputChar == '}') {
			return root;
		}
		if ((inputChar >= 97 && inputChar <= 122)
				|| (inputChar >= 65 && inputChar <= 90))
			lexemeType = new Word();
		else if (inputChar == '+' || inputChar == '-' || inputChar == '*'
				|| inputChar == '/')
			lexemeType = new ArithmeticOperators();
		else if (inputChar == '=')
			lexemeType = new AssignmentOperator();
		else if (inputChar == '"') {
			Quote lexemeTypeQuote;
			temp = lexemeTypeQuote.insertTreeNode(root, temp, inputStream,
					inputChar);
			StringLiteral lexemeTypeStringLiteral;
			inputChar = inputStream->get();
			temp = lexemeTypeStringLiteral.insertTreeNode(root, temp,
					inputStream, inputChar); //garbage-in-garbage-out principle; if the second quote is not found out comes the garbage
			inputChar = inputStream->get();
			temp = lexemeTypeQuote.insertTreeNode(root, temp, inputStream,
					inputChar);
			continue;
		} else if (inputChar == '>' || inputChar == '<')
			lexemeType = new RelationalOperator();
		else if (inputChar >= 48 && inputChar <= 57)
			lexemeType = new Number();
		else if (inputChar == '(' || inputChar == ')')
			lexemeType = new Parethesis();
		else if (inputChar == ';')
			lexemeType = new EolOperator();
		if (lexemeType)
			temp = lexemeType->insertTreeNode(root, temp, inputStream,
					inputChar);
		delete lexemeType;
	}
	return root;
}

cmplr_perseus::Lexer::Lexeme::~Lexeme() {
}

cmplr_perseus::Lexer::TreeNode::SymbolTableNode* cmplr_perseus::Lexer::Lexeme::insertTreeNodeDefault(
		TreeNode *root, TreeNode::SymbolTableNode *currentListNode,
		std::string tokenValue, keywords tokenType) {
	if (currentListNode == NULL) {
		root->symbolTableHeadNode = new TreeNode::SymbolTableNode(tokenValue,
				tokenType);
		return currentListNode = root->symbolTableHeadNode;
	}
	currentListNode->nextNode = new TreeNode::SymbolTableNode(tokenValue,
			tokenType);
	return currentListNode->nextNode;
}

cmplr_perseus::Lexer::TreeNode::SymbolTableNode* cmplr_perseus::Lexer::Word::insertTreeNode(
		TreeNode *root, TreeNode::SymbolTableNode *currentListNode,
		std::ifstream *inputStream, char inputChar) {
	std::string keywordSoFar = "";
	keywordSoFar += inputChar;
	while ((inputStream->peek() >= 97 && inputStream->peek() <= 122)
			|| (inputStream->peek() >= 65 && inputStream->peek() <= 90)) {
		inputChar = inputStream->get();
		keywordSoFar += inputChar;
	}
	if (keywordSoFar == "if" || keywordSoFar == "while"
			|| keywordSoFar == "print")
		return insertTreeNodeDefault(root, currentListNode, keywordSoFar,
				keywords::reserved_words);
	else if (keywordSoFar == "int" || keywordSoFar == "dec"
			|| keywordSoFar == "string")
		return insertTreeNodeDefault(root, currentListNode, keywordSoFar,
				keywords::primitive_types);
	else {
		if (root->stringsTable.find(keywordSoFar) == root->stringsTable.end())
			root->stringsTable.emplace(keywordSoFar);
		return insertTreeNodeDefault(root, currentListNode, keywordSoFar,
				keywords::identifiers);
	}
}

cmplr_perseus::Lexer::TreeNode::SymbolTableNode* cmplr_perseus::Lexer::StringLiteral::insertTreeNode(
		TreeNode *root, TreeNode::SymbolTableNode *currentListNode,
		std::ifstream *inputStream, char inputChar) {
	std::string keywordSoFar = "";
	keywordSoFar += inputChar;
	while (inputStream->peek() != '"') {
		inputChar = inputStream->get();
		keywordSoFar += inputChar;
	}
	if (keywordSoFar != "\"")
		return insertTreeNodeDefault(root, currentListNode, keywordSoFar,
				keywords::string_literal);
	else
		return currentListNode;
}

cmplr_perseus::Lexer::TreeNode::SymbolTableNode* cmplr_perseus::Lexer::RelationalOperator::insertTreeNode(
		TreeNode *root, TreeNode::SymbolTableNode *currentListNode,
		std::ifstream *inputStream, char inputChar) {
	std::string keywordSoFar = "";
	keywordSoFar += inputChar;
	return insertTreeNodeDefault(root, currentListNode, keywordSoFar,
			keywords::relational_operators);
}

cmplr_perseus::Lexer::TreeNode::SymbolTableNode* cmplr_perseus::Lexer::Parethesis::insertTreeNode(
		TreeNode *root, TreeNode::SymbolTableNode *currentListNode,
		std::ifstream *inputStream, char inputChar) {
	std::string keywordSoFar = "";
	keywordSoFar += inputChar;
	return insertTreeNodeDefault(root, currentListNode, keywordSoFar,
			keywords::parenthesis);
}

cmplr_perseus::Lexer::TreeNode::SymbolTableNode* cmplr_perseus::Lexer::EolOperator::insertTreeNode(
		TreeNode *root, TreeNode::SymbolTableNode *currentListNode,
		std::ifstream *inputStream, char inputChar) {
	std::string keywordSoFar = "";
	keywordSoFar += inputChar;
	return insertTreeNodeDefault(root, currentListNode, keywordSoFar,
			keywords::eol_operator);
}

cmplr_perseus::Lexer::TreeNode::SymbolTableNode* cmplr_perseus::Lexer::Number::insertTreeNode(
		TreeNode *root, TreeNode::SymbolTableNode *currentListNode,
		std::ifstream *inputStream, char inputChar) {
	std::string keywordSoFar = "";
	keywordSoFar += inputChar;
	while (inputStream->peek() >= 48 && inputStream->peek() <= 57) {
		inputChar = inputStream->get();
		keywordSoFar += inputChar;
	}
	return insertTreeNodeDefault(root, currentListNode, keywordSoFar,
			keywords::number);
}

cmplr_perseus::Lexer::TreeNode::SymbolTableNode* cmplr_perseus::Lexer::Quote::insertTreeNode(
		TreeNode *root, TreeNode::SymbolTableNode *currentListNode,
		std::ifstream *inputStream, char inputChar) {
	std::string keywordSoFar = "";
	keywordSoFar += inputChar;
	return insertTreeNodeDefault(root, currentListNode, keywordSoFar,
			keywords::quote);
}

cmplr_perseus::Lexer::TreeNode::SymbolTableNode* cmplr_perseus::Lexer::AssignmentOperator::insertTreeNode(
		TreeNode *root, TreeNode::SymbolTableNode *currentListNode,
		std::ifstream *inputStream, char inputChar) {
	std::string keywordSoFar = "";
	keywordSoFar += inputChar;
	return insertTreeNodeDefault(root, currentListNode, keywordSoFar,
			keywords::assignment_operator);
}

cmplr_perseus::Lexer::TreeNode::SymbolTableNode* cmplr_perseus::Lexer::ArithmeticOperators::insertTreeNode(
		TreeNode *root, TreeNode::SymbolTableNode *currentListNode,
		std::ifstream *inputStream, char inputChar) {

	std::string keywordSoFar = "";
	keywordSoFar += inputChar;
	return insertTreeNodeDefault(root, currentListNode, keywordSoFar,
			keywords::arithmetic_operators);
}

int main() {
	cmplr_perseus::Lexer *l = new cmplr_perseus::Lexer("type path here");
	l->printTree(l->lexerRootNode);
	delete l;
	std::cout << "Here endeth the Book of the Tales of Symbol Table"
			<< std::endl;
}
