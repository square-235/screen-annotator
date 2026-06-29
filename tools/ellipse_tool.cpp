#include "ellipse_tool.h"
#include <QPen>
#include <QRect>

EllipseTool::EllipseTool(const QColor &color, int thickness)
    : BaseTool(color, thickness)
{
}

QString EllipseTool::toolName() const
{
    return "ellipse";
}

void EllipseTool::handlePress(const QPoint &point)
{
    m_annotation->startPoint = point;
    m_annotation->endPoint = point;
    m_annotation->hasStartPoint = true;
    m_annotation->hasEndPoint = true;
}

void EllipseTool::handleMove(const QPoint &point)
{
    m_annotation->endPoint = point;
}

void EllipseTool::handleRelease(const QPoint &point)
{
    m_annotation->endPoint = point;
}

void EllipseTool::drawPreview(QPainter &painter)
{
    if (!m_annotation || !m_annotation->hasStartPoint || !m_annotation->hasEndPoint)
        return;
    painter.save();
    QPen pen(m_color, m_thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    QRect rect(m_annotation->startPoint, m_annotation->endPoint);
    rect = rect.normalized();
    painter.drawEllipse(rect);
    painter.restore();
}

void EllipseTool::drawAnnotation(QPainter &painter, const AnnotationPtr &annotation)
{
    if (!annotation->hasStartPoint || !annotation->hasEndPoint)
        return;
    painter.save();
    QPen pen(annotation->color, annotation->thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    QRect rect(annotation->startPoint, annotation->endPoint);
    rect = rect.normalized();
    painter.drawEllipse(rect);
    painter.restore();
}
