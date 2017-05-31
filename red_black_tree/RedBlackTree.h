#pragma once

#include <string>

using namespace std;

struct RBNode
{
    int key;
    string value;
    int color;

    RBNode *left;
    RBNode *right;
    RBNode *parent;
};

enum Colors
{
    RED,
    BLACK
};

enum Position
{
    LEFT,
    RIGHT
};

class RedBlackTree
{
private:
    RBNode* root;

public:
    RedBlackTree();
    ~RedBlackTree();

    void Insert(int key, string value);
    void Delete(int key);
    string Search(int value);

private:
    RBNode* CreateNewNode(int key, string value);
    RBNode *GetUncle(RBNode *node);
    RBNode *GetMaxNode(RBNode *node);
    RBNode *SearchNode(int key);
    RBNode *GetBrother(RBNode *node, RBNode *parent);
    RBNode *GetParent(RBNode *node);
    RBNode *GetClosestNephew(RBNode *node, RBNode *parent);
    RBNode *GetFarthestNephew(RBNode *node, RBNode *parent);

    bool ValidateNode(RBNode *node);
    bool IsNodeNearByUncle(RBNode *node);

    int GetParentColor(RBNode *node);
    int GetUncleColor(RBNode *node);
    int GetNodeLeftRightPosition(RBNode *node);

    void InsertNodeToTree(RBNode *temp, RBNode *newNode);
    void BalanceTree(RBNode * invalideNode);
    void RotateToLeft(RBNode *node);
    void RotateToRight(RBNode *node);
    void SetColor(RBNode *node, int color);
    void MergeNodeWithGrandparent(RBNode *node, RBNode *grandparent);
    void UpdateRoot(RBNode *node);
    void BalanceAfterDeleteCase1(RBNode *node);
    void BalanceAfterDeleteCase2(RBNode *node);
    void BalanceAfterDeleteCase3(RBNode *node, RBNode *closestNephew);
    void BalanceAfterDeleteCase4(RBNode *node, RBNode *farthestNephew);
    void Delete(RBNode *node);

};

