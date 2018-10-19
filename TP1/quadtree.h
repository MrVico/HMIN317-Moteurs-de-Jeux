#ifndef QUADTREE_H
#define QUADTREE_H

#include <QObject>
#include <QVector2D>

class QuadTree
{
    Q_OBJECT
public:
    explicit QuadTree(QVector2D botLeft, QVector2D topRight);
    ~QuadTree();
    QVector2D botleft;
    QVector2D topRight;
    std::vector<QVector2D> points;
    void subdivideQuad();

protected:
    bool isPointInsideQuad(QVector2D botLeft, QVector2D topRight, QVector2D point);
    void addPoint(QVector2D point);

private:
    float width;
    float height;
};

#endif // QUADTREE_H
