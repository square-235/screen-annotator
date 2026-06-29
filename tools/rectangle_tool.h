#ifndef RECTANGLE_TOOL_H
#define RECTANGLE_TOOL_H

#include "base_tool.h"

class RectangleTool : public BaseTool {
public:
    RectangleTool(const QColor &color, int thickness);
    ~RectangleTool() override = default;

    QString toolName() const override;
    void drawPreview(QPainter &painter) override;
    void drawAnnotation(QPainter &painter, const AnnotationPtr &annotation) override;

protected:
    void handlePress(const QPoint &point) override;
    void handleMove(const QPoint &point) override;
    void handleRelease(const QPoint &point) override;
};

#endif
