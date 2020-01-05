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

public:
	struct treeNode {

		struct symbolTableNode {
		private:
			keywords tokenType;
		public:
			std::string tokenValue; //move to private on print method deletion
			symbolTableNode *nextNode;

			symbolTableNode(std::string tokenValue, keywords tokenType);
			//add insertion method here
		};

		struct childNode {
			treeNode *child;
			childNode *nextNode;
			childNode();
		};
		childNode *listOfChildrenHeadNode;
		symbolTableNode *symbolTableHeadNode; //implemented as a list
		std::unordered_set<std::string> stringsTable;
	};
	lexer(std::string inputFilePath);
	treeNode* scanFile(std::string inputFilePath);

	void print(treeNode *n);
private:
	std::ifstream* addIdNode(std::ifstream *inputStream, treeNode **root,
			char inputChar, treeNode::symbolTableNode **temp);
	treeNode* buildTree(std::ifstream *inputStream, treeNode *root);
	treeNode::symbolTableNode* insert(treeNode::symbolTableNode *head,
			treeNode *root, std::string s, keywords k);
}
;

}
