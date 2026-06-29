#ifndef CONFIG_H
#define CONFIG_H

#include <QColor>
#include <QVector>

class Config {
public:
    static const QColor DEFAULT_COLOR;
    static const int DEFAULT_THICKNESS = 3;
    static const int MIN_THICKNESS = 1;
    static const int MAX_THICKNESS = 20;

    static const int ERASER_SIZE = 40;

    static const int TOOLBAR_WIDTH = 900;
    static const int TOOLBAR_HEIGHT = 60;
    static const int TOOLBAR_MARGIN = 20;
    static const int TOOLBAR_SNAP_DISTANCE = 30;
    static const int UNDO_LIMIT = 50;
};

#endif
