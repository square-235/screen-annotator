#include "annotation.h"
#include <QUuid>

Annotation::Annotation()
    : id(QUuid::createUuid().toString())
    , toolType("")
    , color(QColor(255, 0, 0))
    , thickness(3)
    , hasStartPoint(false)
    , hasEndPoint(false)
{
}

Annotation::Annotation(const QString &toolType, const QColor &color, int thickness)
    : id(QUuid::createUuid().toString())
    , toolType(toolType)
    , color(color)
    , thickness(thickness)
    , hasStartPoint(false)
    , hasEndPoint(false)
{
}

Annotation::Annotation(const QString &toolType, const QColor &color, int thickness, const QVector<QString> &erasedIds)
    : id(QUuid::createUuid().toString())
    , toolType(toolType)
    , color(color)
    , thickness(thickness)
    , hasStartPoint(false)
    , hasEndPoint(false)
    , erasedIds(erasedIds)
{
}

bool Annotation::operator==(const Annotation &other) const
{
    return id == other.id;
}
