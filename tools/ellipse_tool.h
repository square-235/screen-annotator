#ifndef ELLIPSE_TOOL_H
#define ELLIPSE_TOOL_H

#include "base_tool.h"

class EllipseTool : public BaseTool {
public:
    EllipseTool(const QColor &color, int thickness);
    ~EllipseTool() override = default;

    QString toolName() const override;
    void drawPreview(QPainter &painter) override;
    void drawAnnotation(QPainter &painter, const AnnotationPtr &annotation) override;

protected:
    void handlePress(const QPoint &point) override;
    void handleMove(const QPoint &point) override;
    void handleRelease(const QPoint &point) override;
};

#endif
