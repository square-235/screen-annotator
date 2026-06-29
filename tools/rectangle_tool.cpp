#include "rectangle_tool.h"
#include <QPen>
#include <QRect>

RectangleTool::RectangleTool(const QColor &color, int thickness)
    : BaseTool(color, thickness)
{
}

QString RectangleTool::toolName() const
{
    return "rectangle";
}

void RectangleTool::handlePress(const QPoint &point)
{
    m_annotation->startPoint = point;
    m_annotation->endPoint = point;
    m_annotation->hasStartPoint = true;
    m_annotation->hasEndPoint = true;
}

void RectangleTool::handleMove(const QPoint &point)
{
    m_annotation->endPoint = point;
}

void RectangleTool::handleRelease(const QPoint &point)
{
    m_annotation->endPoint = point;
}

void RectangleTool::drawPreview(QPainter &painter)
{
    if (!m_annotation || !m_annotation->hasStartPoint || !m_annotation->hasEndPoint)
        return;
    painter.save();
    QPen pen(m_color, m_thickness, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    painter.setPen(pen);
    QRect rect(m_annotation->startPoint, m_annotation->endPoint);
    rect = rect.normalized();
    painter.drawRect(rect);
    painter.restore();
}

void RectangleTool::drawAnnotation(QPainter &painter, const AnnotationPtr &annotation)
{
    if (!annotation->hasStartPoint || !annotation->hasEndPoint)
        return;
    painter.save();
    QPen pen(annotation->color, annotation->thickness, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    painter.setPen(pen);
    QRect rect(annotation->startPoint, annotation->endPoint);
    rect = rect.normalized();
    painter.drawRect(rect);
    painter.restore();
}
