#include<string>
#include<fstream>
#include<iostream>
#include<unordered_set>

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

		~treeNode();
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
