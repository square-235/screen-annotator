#ifndef ARROW_TOOL_H
#define ARROW_TOOL_H

#include "line_tool.h"

class ArrowTool : public LineTool {
public:
    ArrowTool(const QColor &color, int thickness);
    ~ArrowTool() override = default;

    QString toolName() const override;
    void drawPreview(QPainter &painter) override;
    void drawAnnotation(QPainter &painter, const AnnotationPtr &annotation) override;

private:
    void drawArrow(QPainter &painter, const QPoint &start, const QPoint &end);
};

#endif
