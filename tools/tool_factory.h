#ifndef TOOL_FACTORY_H
#define TOOL_FACTORY_H

#include <QString>
#include <QMap>
#include <QColor>
#include <memory>
#include "base_tool.h"

class ToolFactory {
public:
    static BaseToolPtr createTool(const QString &toolName, const QColor &color, int thickness);
};

#endif
