#include <QApplication>
#include "screen_annotator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ScreenAnnotator annotator;
    return annotator.run();
}
