#include<string>
#include<fstream>
#include<iostream>
#include<unordered_set>
#pragma once

namespace cmplr_perseus {

class Lexer {

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

	struct TreeNode {

		struct SymbolTableNode {
			keywords tokenType;
			std::string tokenValue;
			SymbolTableNode *nextNode;
			SymbolTableNode(std::string tokenValue, keywords tokenType);
			~SymbolTableNode();
		};

		struct ChildNode {
			TreeNode *child;
			ChildNode *nextNode;
			ChildNode();
			~ChildNode();
		};

		ChildNode *listOfChildrenHeadNode;
		SymbolTableNode *symbolTableHeadNode; //implemented as a list since the built-in stl list container's emplace function was calling the copy constructor; tldr: efficiency
		std::unordered_set<std::string> stringsTable;
		TreeNode();
		~TreeNode();
	};

	class Lexeme {
	protected:
		TreeNode::SymbolTableNode* insertTreeNodeDefault(TreeNode *root,
				TreeNode::SymbolTableNode *currentlistNode,
				std::string tokenValue, keywords tokenType);
	public:
		virtual ~Lexeme();
		virtual TreeNode::SymbolTableNode* insertTreeNode(TreeNode *root,
				TreeNode::SymbolTableNode *currentListNode,
				std::ifstream *inputStream, char inputChar)=0;
	};

	class ArithmeticOperators: public Lexeme {
	public:
		TreeNode::SymbolTableNode* insertTreeNode(TreeNode *root,
				TreeNode::SymbolTableNode *currentListNode,
				std::ifstream *inputStream, char inputChar) override;
	};

	class AssignmentOperator: public Lexeme {
	public:
		TreeNode::SymbolTableNode* insertTreeNode(TreeNode *root,
				TreeNode::SymbolTableNode *currentListNode,
				std::ifstream *inputStream, char inputChar) override;
	};

	class Quote: public Lexeme {
	public:
		TreeNode::SymbolTableNode* insertTreeNode(TreeNode *root,
				TreeNode::SymbolTableNode *currentListNode,
				std::ifstream *inputStream, char inputChar) override;
	};

	class StringLiteral: public Lexeme {
	public:
		TreeNode::SymbolTableNode* insertTreeNode(TreeNode *root,
				TreeNode::SymbolTableNode *currentListNode,
				std::ifstream *inputStream, char inputChar) override;
	};

	class Word: public Lexeme {
	public:
		TreeNode::SymbolTableNode* insertTreeNode(TreeNode *root,
				TreeNode::SymbolTableNode *currentListNode,
				std::ifstream *inputStream, char inputChar) override;
	};

	class RelationalOperator: public Lexeme {
	public:
		TreeNode::SymbolTableNode* insertTreeNode(TreeNode *root,
				TreeNode::SymbolTableNode *curentListNode,
				std::ifstream *inputStream, char inputChar) override;
	};

	class Parethesis: public Lexeme {
	public:
		TreeNode::SymbolTableNode* insertTreeNode(TreeNode *root,
				TreeNode::SymbolTableNode *currentListNode,
				std::ifstream *inputStream, char inputChar) override;
	};

	class EolOperator: public Lexeme {
	public:
		TreeNode::SymbolTableNode* insertTreeNode(TreeNode *root,
				TreeNode::SymbolTableNode *currentListNode,
				std::ifstream *inputStream, char inputChar) override;
	};

	class Number: public Lexeme {
	public:
		TreeNode::SymbolTableNode* insertTreeNode(TreeNode *root,
				TreeNode::SymbolTableNode *currentListNode,
				std::ifstream *inputStream, char inputChar) override;
	};

	TreeNode* scanFile(std::string inputFilePath);
	TreeNode* buildTree(std::ifstream *inputStream, TreeNode *root);
public:
	TreeNode *lexerRootNode;
	Lexer(std::string inputFilePath);
	~Lexer();
	void printTree(const TreeNode *n) const;
};

}
