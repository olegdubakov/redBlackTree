#include <iostream>
#include <string>
#include "RedBlackTree.h"

using namespace std;

RedBlackTree::RedBlackTree()
{
}

RedBlackTree::~RedBlackTree()
{
}

void RedBlackTree::Insert(int key, string value)
{
    RBNode *newNode = CreateNewNode(key, value);

    if (root == NULL)
    {
        root = newNode;
        root->color = BLACK;
    }
    else
    {
        InsertNodeToTree(root, newNode);
    }

    bool isInsertValid = ValidateNode(newNode);

    if (isInsertValid == false)
    {
        BalanceTree(newNode);
    }
}

void RedBlackTree::Delete(int key)
{
    RBNode *nodeToDelete = SearchNode(key);

    Delete(nodeToDelete);
}

void RedBlackTree::Delete(RBNode *nodeToDelete)
{
    if (nodeToDelete != NULL)
    {
        if (nodeToDelete->left != NULL && nodeToDelete->right != NULL)
        {
            RBNode *nodeToReplace = GetMaxNode(nodeToDelete->left);

            nodeToDelete->key = nodeToReplace->key;
            nodeToDelete->value = nodeToReplace->value;

            Delete(nodeToReplace);
        }
        else
        {
            RBNode *child = NULL;
            RBNode *newParent = GetParent(nodeToDelete);
            RBNode *brother = GetBrother(nodeToDelete, newParent);
            RBNode *closestNephew = GetClosestNephew(nodeToDelete, newParent);
            RBNode *farthestNephew = GetFarthestNephew(nodeToDelete, newParent);

            if (nodeToDelete->left != NULL)
            {
                child = nodeToDelete->left;
            }
            else
            {
                child = nodeToDelete->right;
            }

            if (child != NULL)
            {
                MergeNodeWithGrandparent(child, nodeToDelete->parent);
            }
            else
            {
                if (newParent->left == nodeToDelete)
                {
                    newParent->left = NULL;
                }
                else
                {
                    newParent->right = NULL;
                }
            }

            if (child != NULL && child->color == RED && nodeToDelete->color == BLACK)
            {
                child->color = BLACK;
            }
            else
            {
                if (brother != NULL && brother->color == RED)
                {
                    // Delete case 1
                    BalanceAfterDeleteCase1(brother);
                }
                else if ((brother == NULL || brother->color == BLACK)
                    && (closestNephew == NULL || closestNephew->color == BLACK)
                    && (farthestNephew == NULL || farthestNephew->color == BLACK))
                {
                    // Delete case 2
                    BalanceAfterDeleteCase2(brother);
                }
                else if ((brother == NULL || brother->color == BLACK)
                    && (closestNephew != NULL && closestNephew->color == RED)
                    && (farthestNephew == NULL || farthestNephew->color == BLACK))
                {
                    // Delete case 2
                    BalanceAfterDeleteCase3(brother, closestNephew);
                }
                else if ((brother == NULL || brother->color == BLACK)
                    && farthestNephew != NULL && farthestNephew->color == RED)
                {
                    // Delete case 4
                    BalanceAfterDeleteCase4(brother, farthestNephew);
                }
            }

            delete nodeToDelete;
        }
    }
}

void RedBlackTree::BalanceAfterDeleteCase1(RBNode *node)
{
    int position = GetNodeLeftRightPosition(node);

    node->color = BLACK;
    node->parent->color = RED;

    if (position == RIGHT)
    {
        RotateToLeft(node);
    }
    else
    {
        RotateToRight(node);
    }
}
void RedBlackTree::BalanceAfterDeleteCase2(RBNode *node)
{
    if (node == NULL)
    {
        return;
    }

    node->parent->color = BLACK;
    node->color = RED;
}

void RedBlackTree::BalanceAfterDeleteCase3(RBNode *brother, RBNode *closestNephew)
{
    brother->color = RED;
    closestNephew->color = BLACK;
    RBNode *newFarthestNephew;

    int position = GetNodeLeftRightPosition(brother);

    if (position == RIGHT)
    {
        RotateToRight(closestNephew);
    }
    else
    {
        RotateToLeft(closestNephew);
    }

    BalanceAfterDeleteCase4(closestNephew, brother);
}

void RedBlackTree::BalanceAfterDeleteCase4(RBNode *brother, RBNode *farthestNephew)
{
    RBNode *parent = GetParent(brother);

    brother->color = parent->color; 
    parent->color = BLACK;
    farthestNephew->color = BLACK;

    int position = GetNodeLeftRightPosition(brother);

    if (position == RIGHT)
    {
        RotateToLeft(brother);
    }
    else
    {
        RotateToRight(brother);
    }

}

RBNode *RedBlackTree::GetBrother(RBNode *node, RBNode *parent)
{
    if (parent->left == node)
    {
        return parent->right;
    }

    return parent->left;
}

RBNode *RedBlackTree::GetParent(RBNode *node)
{
    if (node->parent != NULL)
    {
        return node->parent;
    }

    return NULL;
}

RBNode *RedBlackTree::GetClosestNephew(RBNode *node, RBNode *parent)
{
    if (parent != NULL)
    {
        RBNode *brother = GetBrother(node, parent);

        if (brother == NULL)
        {
            return NULL;
        }
        else if (parent->left == node)
        {
            return brother->left;
        }
        else
        {
            return brother->right;
        }
    }

    return NULL;
}

RBNode *RedBlackTree::GetFarthestNephew(RBNode *node, RBNode *parent)
{
    if (parent != NULL)
    {
        RBNode *brother = GetBrother(node, parent);

        if (brother == NULL)
        {
            return NULL;
        }
        else if (parent->left == node)
        {
            return brother->right;
        }
        else
        {
            return brother->left;
        }
    }

    return NULL;
}

string RedBlackTree::Search(int key)
{
    RBNode *temp = SearchNode(key);

    if (temp != NULL)
    {
        return temp->value;
    }

    return "";
}

RBNode *RedBlackTree::SearchNode(int key)
{
    RBNode *temp = root;

    do
    {
        if (temp->key != key)
        {
            if (temp->key > key)
            {
                if (temp->left != NULL)
                {
                    temp = temp->left;
                }
                else
                {
                    return NULL;
                }
            }
            else
            {
                if (temp->right != NULL)
                {
                    temp = temp->right;
                }
                else
                {
                    return NULL;
                }
            }
        }
    } while (temp->key != key);

    return temp;
}

void RedBlackTree::BalanceTree(RBNode * invalideNode)
{
    bool isNodeValid = false;

    do
    {
        if (invalideNode->parent == NULL)
        {
            invalideNode->color = BLACK;
            root = invalideNode;
        }
        else
        {
            int uncleColor = GetUncleColor(invalideNode);

            if (uncleColor == RED)
            {
                /* Case 1: the parent and the uncle are red
                *
                *        (black)
                *        /     \
                *    (red)     (red)
                *    /
                * (red) <- invalideNode
                */

                RBNode *grandparent = invalideNode->parent->parent;

                SetColor(grandparent, RED);
                SetColor(grandparent->left, BLACK);
                SetColor(grandparent->right, BLACK);

                invalideNode = grandparent;
            }
            else
            {
                bool isNodeNearByUncle = IsNodeNearByUncle(invalideNode);

                if (isNodeNearByUncle == true)
                {
                    /* Case 2: The parent is red but the uncle is black; also, the current node is near by uncle
                    *     (black)
                    *     /     \
                    * (red)     (black)
                    *    \
                    *    (red) <- invalideNode
                    */
                   
                    int nodePosition = GetNodeLeftRightPosition(invalideNode);
                    RBNode *parent = invalideNode->parent;

                    if (nodePosition == LEFT)
                    {
                        RotateToRight(invalideNode);
                    }
                    else
                    {
                        RotateToLeft(invalideNode);
                    }

                    invalideNode = parent;
                }
                else
                {
                    /* Case 2: The parent is red but the uncle is black, the current node isn't near by uncle
                    *          (black)
                    *          /     \
                    *      (red)     (black)
                    *      /
                    *  (red)
                    */

                    int nodePosition = GetNodeLeftRightPosition(invalideNode);
                    RBNode *parent = invalideNode->parent;

                    SetColor(parent, BLACK);
                    SetColor(parent->parent, RED);

                    if (nodePosition == LEFT)
                    {
                        RotateToRight(parent);
                    }
                    else
                    {
                        RotateToLeft(parent);
                    }

                    invalideNode = parent;
                }
            }
        }

        isNodeValid = ValidateNode(invalideNode);

    } while (isNodeValid == false);

    UpdateRoot(invalideNode);

}

RBNode* RedBlackTree::CreateNewNode(int key, string value)
{
    RBNode* node = new RBNode();
    node->color = RED;
    node->key = key;
    node->value = value;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;

    return node;
}

bool RedBlackTree::ValidateNode(RBNode *node)
{
    if (node->parent != NULL)
    {
        int parentColor = GetParentColor(node);

        if (parentColor == RED && parentColor == node->color)
        {
            return false;
        }
    }
    else
    {
        // if it is root element, color should be BLACK
        if (node->color == RED)
        {
            return false;
        }
    }

    return true;
}

int RedBlackTree::GetParentColor(RBNode *node)
{
    return node->parent->color;
}

int RedBlackTree::GetUncleColor(RBNode *node)
{
    RBNode *uncle = GetUncle(node);

    return uncle == NULL ? BLACK : uncle->color;
}

RBNode* RedBlackTree::GetUncle(RBNode *node)
{
    if (node->parent != NULL && node->parent->parent != NULL)
    {
        RBNode *parent = node->parent;
        RBNode *grandparent = parent->parent;

        RBNode *leftUncle = grandparent->left;
        RBNode *rightUncle = grandparent->right;

        if (leftUncle != parent && leftUncle != NULL)
        {
            return leftUncle;
        }
        else if (rightUncle != parent && rightUncle != NULL)
        {
            return rightUncle;
        }
    }

    return NULL;
}

void RedBlackTree::InsertNodeToTree(RBNode *temp, RBNode *newNode)
{
    if (temp->key > newNode->key)
    {
        if (temp->left != NULL)
        {
            InsertNodeToTree(temp->left, newNode);
        }
        else
        {
            temp->left = newNode;
            newNode->parent = temp;
        }
    }
    else
    {
        if (temp->right != NULL)
        {
            InsertNodeToTree(temp->right, newNode);
        }
        else
        {
            temp->right = newNode;
            newNode->parent = temp;
        }
    }
}

void RedBlackTree::SetColor(RBNode *node, int color)
{
    node->color = color;
}

bool RedBlackTree::IsNodeNearByUncle(RBNode *node)
{
    if (node->parent != NULL && node->parent->parent != NULL)
    {
        RBNode *parent = node->parent;
        RBNode *grandparent = parent->parent;

        RBNode *leftUncle = grandparent->left;
        RBNode *rightUncle = grandparent->right;

        int unclePosition;

        if (leftUncle == parent)
        {
            unclePosition = 1;
        }
        else
        {
            unclePosition = 0;
        }

        int nodePosition;

        if (parent->left == node)
        {
            nodePosition = 1;
        }
        else
        {
            nodePosition = 0;
        }

        return nodePosition == unclePosition ? false : true;
    }

    return false;
}

void RedBlackTree::RotateToLeft(RBNode *node)
{
    if (node->parent == NULL)
    {
        return;
    }

    RBNode *parent = node->parent;
    RBNode *grandparent = NULL;
    RBNode *leftChild = NULL;

    if (parent->parent != NULL)
    {
        grandparent = parent->parent;
    }

    if (node->left != NULL)
    {
        leftChild = node->left;
    }

    MergeNodeWithGrandparent(node, grandparent);

    if (leftChild != NULL)
    {
        parent->right = leftChild;
        leftChild->parent = parent;
    }
    else
    {
        parent->right = NULL;
    }

    node->left = parent;
    parent->parent = node;
}

void RedBlackTree::RotateToRight(RBNode *node)
{
    if (node->parent == NULL)
    {
        return;
    }

    RBNode *parent = node->parent;
    RBNode *grandparent = NULL;
    RBNode *rightChild = NULL;

    if (parent->parent != NULL)
    {
        grandparent = parent->parent;
    }

    if (node->right != NULL)
    {
        rightChild = node->right;
    }

    MergeNodeWithGrandparent(node, grandparent);

    if (rightChild != NULL)
    {
        parent->left = rightChild;
        rightChild->parent = parent;
    }
    else
    {
        parent->left = NULL;
    }

    node->right = parent;
    parent->parent = node;
}

int RedBlackTree::GetNodeLeftRightPosition(RBNode *node)
{
    RBNode *parent = node->parent;

    if (parent->left == node)
    {
        return LEFT;
    }
    else
    {
        return RIGHT;
    }
}

void RedBlackTree::UpdateRoot(RBNode *node)
{

    while (node->parent != NULL)
    {
        node = node->parent;
    }

    root = node;
}

void RedBlackTree::MergeNodeWithGrandparent(RBNode *node, RBNode *grandparent)
{
    if (grandparent != NULL)
    {
        int parentPosition = GetNodeLeftRightPosition(node->parent);

        if (parentPosition == LEFT)
        {
            grandparent->left = node;
        }
        else
        {
            grandparent->right = node;
        }

        node->parent = grandparent;
    }
    else
    {
        node->parent = NULL;
    }
}

RBNode *RedBlackTree::GetMaxNode(RBNode *node)
{
    while (node->right != NULL)
    {
        node = node->right;
    }

    return node;
}