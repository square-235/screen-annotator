#include "arrow_tool.h"
#include <QPen>
#include <QPolygon>
#include <cmath>

ArrowTool::ArrowTool(const QColor &color, int thickness)
    : LineTool(color, thickness)
{
}

QString ArrowTool::toolName() const
{
    return "arrow";
}

void ArrowTool::drawPreview(QPainter &painter)
{
    if (!m_annotation || !m_annotation->hasStartPoint || !m_annotation->hasEndPoint)
        return;
    painter.save();
    QPen pen(m_color, m_thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.setBrush(m_color);
    drawArrow(painter, m_annotation->startPoint, m_annotation->endPoint);
    painter.restore();
}

void ArrowTool::drawAnnotation(QPainter &painter, const AnnotationPtr &annotation)
{
    if (!annotation->hasStartPoint || !annotation->hasEndPoint)
        return;
    painter.save();
    QPen pen(annotation->color, annotation->thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.setBrush(annotation->color);
    drawArrow(painter, annotation->startPoint, annotation->endPoint);
    painter.restore();
}

void ArrowTool::drawArrow(QPainter &painter, const QPoint &start, const QPoint &end)
{
    painter.drawLine(start, end);
    int arrowSize = qMax(m_thickness * 3, 12);
    double angle = std::atan2(end.y() - start.y(), end.x() - start.x());
    QPoint arrowP1(
        end.x() - arrowSize * std::cos(angle - M_PI / 6),
        end.y() - arrowSize * std::sin(angle - M_PI / 6)
    );
    QPoint arrowP2(
        end.x() - arrowSize * std::cos(angle + M_PI / 6),
        end.y() - arrowSize * std::sin(angle + M_PI / 6)
    );
    QPolygon polygon;
    polygon << end << arrowP1 << arrowP2;
    painter.drawPolygon(polygon);
}
