#include<string>
#include<fstream>
#include<iostream>
#include<unordered_set>
//#pragma once
namespace cmplr_ {

class lexer {

	enum class keywords {
		primitive_types,
		arithmetic_operators,
		identifiers,
		reserved_words,
		assignment_operator,
		quote,
		string_literal,
		relational_operators,
		number,
		eol_operator,
		parenthesis
	};

	struct treeNode {

		struct symbolTableNode {
			keywords tokenType;
			std::string tokenValue;
			symbolTableNode *nextNode;

			symbolTableNode(std::string tokenValue, keywords tokenType);
			~symbolTableNode();

			symbolTableNode* insert(treeNode::symbolTableNode *head,
					treeNode *root, std::string s, keywords k);
		};

		struct childNode {
			treeNode *child;
			childNode *nextNode;

			childNode();
			~childNode();
		};

		childNode *listOfChildrenHeadNode;
		symbolTableNode *symbolTableHeadNode; //implemented as a list since the built-in stl list container's emplace function was calling the copy constructor; tldr: efficiency
		std::unordered_set<std::string> stringsTable;

		treeNode();
		~treeNode();
	};

	class lexeme {
	protected:
		treeNode::symbolTableNode* insertTreeNodeDefault(treeNode *root,
				treeNode::symbolTableNode *head, std::string tokenValue,
				keywords tokenType);
	public:
		virtual treeNode::symbolTableNode* insertTreeNode(treeNode *root,
				treeNode::symbolTableNode *head, std::ifstream *inputStream,
				char inputChar)=0;
//		virtual ~lexeme();
	};

	class arithmeticOperators: public lexeme {
	public:
		virtual treeNode::symbolTableNode* insertTreeNode(treeNode *root,
				treeNode::symbolTableNode *head, std::ifstream *inputStream,
				char inputChar) {
			std::string keywordSoFar = "";
			keywordSoFar += inputChar;
			return insertTreeNodeDefault(root, head, keywordSoFar,
					keywords::arithmetic_operators);
		}
	};

	class assignmentOperator: public lexeme {
	public:
		virtual treeNode::symbolTableNode* insertTreeNode(treeNode *root,
				treeNode::symbolTableNode *head, std::ifstream *inputStream,
				char inputChar) {
			std::string keywordSoFar = "";
			keywordSoFar += inputChar;
			return insertTreeNodeDefault(root, head, keywordSoFar,
					keywords::assignment_operator);

		}
	};

	class quote: public lexeme {
	public:
		virtual treeNode::symbolTableNode* insertTreeNode(treeNode *root,
				treeNode::symbolTableNode *head, std::ifstream *inputStream,
				char inputChar) {
			std::string keywordSoFar = "";
			keywordSoFar += inputChar;
			return insertTreeNodeDefault(root, head, keywordSoFar,
					keywords::quote);
		}
	};

	class stringLiteral: public lexeme {
	public:
		treeNode::symbolTableNode* insertTreeNode(treeNode *root,
				treeNode::symbolTableNode *head, std::ifstream *inputStream,
				char inputChar) {
			std::string keywordSoFar = "";
			while (inputStream->peek() != '"') {
				inputChar = inputStream->get();
				keywordSoFar += inputChar;
			}

			if (keywordSoFar != "")
				return insertTreeNodeDefault(root, head, keywordSoFar,
						keywords::string_literal);
			else
				return head;
		}
	};

	class word: public lexeme {
	public:
		treeNode::symbolTableNode* insertTreeNode(treeNode *root,
				treeNode::symbolTableNode *head, std::ifstream *inputStream,
				char inputChar) {

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
				return insertTreeNodeDefault(root, head, keywordSoFar,
						keywords::reserved_words);

			else if (keywordSoFar == "int" || keywordSoFar == "dec"
					|| keywordSoFar == "string")
				return insertTreeNodeDefault(root, head, keywordSoFar,
						keywords::primitive_types);

			else {
				if (root->stringsTable.find(keywordSoFar)
						== root->stringsTable.end())
					root->stringsTable.emplace(keywordSoFar);
				return insertTreeNodeDefault(root, head, keywordSoFar,
						keywords::identifiers);
				//		std::unordered_set<std::string>::iterator i =
				//				(*root)->stringsTable.find(keywordSoFar);
			}

		}
	};

	class relationalOperator: public lexeme {
	public:
		virtual treeNode::symbolTableNode* insertTreeNode(treeNode *root,
				treeNode::symbolTableNode *head, std::ifstream *inputStream,
				char inputChar) {
			std::string keywordSoFar = "";
			keywordSoFar += inputChar;
			return insertTreeNodeDefault(root, head, keywordSoFar,
					keywords::relational_operators);
		}
	};

	class parethesis: public lexeme {
	public:
		virtual treeNode::symbolTableNode* insertTreeNode(treeNode *root,
				treeNode::symbolTableNode *head, std::ifstream *inputStream,
				char inputChar) {
			std::string keywordSoFar = "";
			keywordSoFar += inputChar;
			return insertTreeNodeDefault(root, head, keywordSoFar,
					keywords::parenthesis);
		}
	};

	class eolOperator: public lexeme {
	public:
		virtual treeNode::symbolTableNode* insertTreeNode(treeNode *root,
				treeNode::symbolTableNode *head, std::ifstream *inputStream,
				char inputChar) {
			std::string keywordSoFar = "";
			keywordSoFar += inputChar;
			return insertTreeNodeDefault(root, head, keywordSoFar,
					keywords::eol_operator);
		}
	};

	class number: public lexeme {
	public:
		virtual treeNode::symbolTableNode* insertTreeNode(treeNode *root,
				treeNode::symbolTableNode *head, std::ifstream *inputStream,
				char inputChar) {
			std::string keywordSoFar = "";
			while (inputStream->peek() >= 48 && inputStream->peek() <= 57) {
				inputChar = inputStream->get();
				keywordSoFar += inputChar;
			}
			return insertTreeNodeDefault(root, head, keywordSoFar,
					keywords::number);
		}
	};

	treeNode* scanFile(std::string inputFilePath);

	void addWordNode(std::ifstream *inputStream, treeNode **root,
			char inputChar, treeNode::symbolTableNode **temp);
	treeNode* buildTree(std::ifstream *inputStream, treeNode *root);

public:
	void print(treeNode *n);
	treeNode *lexerRootNode;
	lexer(std::string inputFilePath);
	~lexer();
};

}
