#ifndef SPEEDSCORING_H
#define SPEEDSCORING_H

#include "scoringmethod.h"

class MainWindow;

class SpeedScoring : public ScoringMethod
{
public:
    SpeedScoring(MainWindow *mainWindow);

    double windowTop(void) const { return mWindowTop; }
    double windowBottom(void) const { return mWindowBottom; }
    void setWindow(double windowBottom, double windowTop);

    double score(const MainWindow::DataPoints &result);
    QString scoreAsText(double score);

    void prepareDataPlot(DataPlot *plot);

    bool getWindowBounds(const MainWindow::DataPoints &result,
                         DataPoint &dpBottom, DataPoint &dpTop);

    void optimize() { ScoringMethod::optimize(mMainWindow, mWindowBottom); }

private:
    MainWindow *mMainWindow;

    double      mWindowTop;
    double      mWindowBottom;

signals:

public slots:
};

#endif // SPEEDSCORING_H
