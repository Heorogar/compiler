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
			~symbolTableNode() {
				delete nextNode;
			}

			symbolTableNode* insert(treeNode::symbolTableNode *head,
					treeNode *root, std::string s, keywords k);
		};

		struct childNode {
			treeNode *child;
			childNode *nextNode;

			childNode();
			~childNode() {
				delete child;
				delete nextNode;
			}
		};

		childNode *listOfChildrenHeadNode;
		symbolTableNode *symbolTableHeadNode; //implemented as a list since the built-in stl list container's emplace function was calling the copy constructor; tldr: efficiency
		std::unordered_set<std::string> stringsTable;

		~treeNode() {
			stringsTable.clear();
			symbolTableNode *temp, *foo = symbolTableHeadNode;
			while (temp) {
				temp = temp->nextNode;
				delete foo;
				foo = temp;
			}
		}
	};

	treeNode* scanFile(std::string inputFilePath);
	void print(treeNode *n);

	void addWordNode(std::ifstream *inputStream, treeNode **root,
			char inputChar, treeNode::symbolTableNode **temp);
	treeNode* buildTree(std::ifstream *inputStream, treeNode *root);

public:
	treeNode *lexerRootNode;
	lexer(std::string inputFilePath);
	~lexer() {
		treeNode::childNode *temp, *foo = lexerRootNode->listOfChildrenHeadNode;
		while (temp) {
			temp = temp->nextNode;
			delete foo;
			foo = temp;
		}
	}
};

}
