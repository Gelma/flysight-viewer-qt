#ifndef PPCSCORING_H
#define PPCSCORING_H

#include "scoringmethod.h"

class MainWindow;

class PPCScoring : public ScoringMethod
{
public:
    typedef enum {
        Time, Distance, Speed
    } Mode;

    PPCScoring(MainWindow *mainWindow);

    Mode mode() const { return mMode; }
    void setMode(Mode mode);

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

    Mode        mMode;
    double      mWindowTop;
    double      mWindowBottom;

signals:

public slots:
};

#endif // PPCSCORING_H
