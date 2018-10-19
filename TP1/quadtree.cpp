#include "quadtree.h"
#include <iostream>

QuadTree::QuadTree(QVector2D botLeft, QVector2D topRight):
    points(0),
    botLeft(0),
    topRight(0),
    width(0),
    height(0)
{
    width = topRight[0] - botLeft[0];
    height = topRight[1] - botLeft[1];
    botLeft = botLeft;
    topRight = topRight;
}

QuadTree::~QuadTree()
{
}

void QuadTree::subdivideQuad()
{
    QuadTree topLeftQuad = QuadTree(new QVector2D(botLeft[0], botLeft[1]+height/2), new QVector2D(topRight[0]-width/2, topRight[1]));
    QuadTree topRightQuad = QuadTree(new QVector2D(botLeft[0]+width/2, botLeft[1]+height/2), topRight);
    QuadTree botLeftQuad = QuadTree(botLeft, new QVector2D(botLeft[0]+width/2, botLeft[1]+height/2));
    QuadTree botRightQuad = QuadTree(new QVector2D(botLeft[0]+width/2, botLeft[1]), new QVector2D(botLeft[0]+width, botLeft[1]));

    for(int i=0; i<points.size(); i++){
        if(isPointInsideQuad(topLeftQuad.botleft, topLeftQuad.topRight, points[i]))
        {
            topLeftQuad.addPoint(points[i]);
        }
        else if(isPointInsideQuad(topRightQuad.botleft, topRightQuad.topRight, points[i]))
        {
            topRightQuad.addPoint(points[i]);
        }
        else if(isPointInsideQuad(botLeftQuad.botleft, botLeftQuad.topRight, points[i]))
        {
            botLeftQuad.addPoint(points[i]);
        }
        else if(isPointInsideQuad(botRightQuad.botleft, botRightQuad.topRight, points[i]))
        {
            botRightQuad.addPoint(points[i]);
        }
    }

    std::cout << "TopLeft: " << topLeftQuad.points.size() << std::endl;
    std::cout << "TopRight: " << topRightQuad.points.size() << std::endl;
    std::cout << "BotLeft: " << botLeftQuad.points.size() << std::endl;
    std::cout << "BotRight: " << botRightQuad.points.size() << std::endl;
}

bool QuadTree::isPointInsideQuad(QVector2D botLeft, QVector2D topRight, QVector2D point)
{
    if(point[0] > botLeft[0] && point[0] < topRight[0] && point[1] > botLeft[1] && point[1] < topRight[1])
        return true;
    return false;
}

void QuadTree::addPoint(QVector2D point)
{
    points.push_back(point);
}
