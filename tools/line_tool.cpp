#include "line_tool.h"
#include <QPen>

LineTool::LineTool(const QColor &color, int thickness)
    : BaseTool(color, thickness)
{
}

QString LineTool::toolName() const
{
    return "line";
}

void LineTool::handlePress(const QPoint &point)
{
    m_annotation->startPoint = point;
    m_annotation->endPoint = point;
    m_annotation->hasStartPoint = true;
    m_annotation->hasEndPoint = true;
}

void LineTool::handleMove(const QPoint &point)
{
    m_annotation->endPoint = point;
}

void LineTool::handleRelease(const QPoint &point)
{
    m_annotation->endPoint = point;
}

void LineTool::drawPreview(QPainter &painter)
{
    if (!m_annotation || !m_annotation->hasStartPoint || !m_annotation->hasEndPoint)
        return;
    painter.save();
    QPen pen(m_color, m_thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawLine(m_annotation->startPoint, m_annotation->endPoint);
    painter.restore();
}

void LineTool::drawAnnotation(QPainter &painter, const AnnotationPtr &annotation)
{
    if (!annotation->hasStartPoint || !annotation->hasEndPoint)
        return;
    painter.save();
    QPen pen(annotation->color, annotation->thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawLine(annotation->startPoint, annotation->endPoint);
    painter.restore();
}
