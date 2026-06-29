#include "tool_factory.h"
#include "pen_tool.h"
#include "line_tool.h"
#include "arrow_tool.h"
#include "rectangle_tool.h"
#include "ellipse_tool.h"
#include "eraser_tool.h"

BaseToolPtr ToolFactory::createTool(const QString &toolName, const QColor &color, int thickness)
{
    if (toolName == "pen") {
        return std::make_shared<PenTool>(color, thickness);
    } else if (toolName == "line") {
        return std::make_shared<LineTool>(color, thickness);
    } else if (toolName == "arrow") {
        return std::make_shared<ArrowTool>(color, thickness);
    } else if (toolName == "rectangle") {
        return std::make_shared<RectangleTool>(color, thickness);
    } else if (toolName == "ellipse") {
        return std::make_shared<EllipseTool>(color, thickness);
    } else if (toolName == "eraser") {
        return std::make_shared<EraserTool>(color, thickness);
    }
    return nullptr;
}
