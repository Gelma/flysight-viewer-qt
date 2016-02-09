#ifndef PLOTVALUE_H
#define PLOTVALUE_H

#include <QColor>
#include <QSettings>
#include <QString>

#include "datapoint.h"
#include "qcustomplot.h"

#define METERS_TO_FEET 3.28084
#define MPS_TO_MPH     2.23694
#define MPS_TO_KMH     3.6

class PlotValue: public QObject
{
    Q_OBJECT

public:
    typedef enum
    {
        Metric = 0,
        Imperial
    } Units;

    PlotValue(bool visible, QColor color): mVisible(visible), mColor(color),
        mMinimum(0), mMaximum(1), mUseMinimum(false), mUseMaximum(false) {}

    virtual const QString title() const = 0;
    virtual const QString title(Units units) const = 0;

    void setColor(const QColor &color) { mColor = color; }
    const QColor &color() const { return mColor; }

    double value(const DataPoint &dp, Units units) const
    {
        return rawValue(dp) * factor(units);
    }

    virtual double rawValue(const DataPoint &dp) const = 0;
    virtual double factor(Units units) const
    {
        Q_UNUSED(units);
        return 1;
    }

    void setMinimum(double minimum) { mMinimum = minimum; }
    double minimum() const { return mMinimum; }

    void setMaximum(double maximum) { mMaximum = maximum; }
    double maximum() const { return mMaximum; }

    void setUseMinimum(double useMinimum) { mUseMinimum = useMinimum; }
    double useMinimum() const { return mUseMinimum; }

    void setUseMaximum(double useMaximum) { mUseMaximum = useMaximum; }
    double useMaximum() const { return mUseMaximum; }

    QCPAxis *addAxis(QCustomPlot *plot, Units units)
    {
        mAxis = plot->axisRect()->addAxis(QCPAxis::atLeft);
        mAxis->setLabelColor(color());
        mAxis->setTickLabelColor(color());
        mAxis->setBasePen(QPen(color()));
        mAxis->setTickPen(QPen(color()));
        mAxis->setSubTickPen(QPen(color()));
        mAxis->setLabel(title(units));
        return mAxis;
    }
    QCPAxis *axis() const { return mAxis; }

    bool visible() const { return mVisible; }
    void setVisible(bool visible) { mVisible = visible; }

    void readSettings()
    {
        QSettings settings("FlySight", "Viewer");
        settings.beginGroup("plotValue/" + key());
        mVisible = settings.value("visible", mVisible).toBool();
        mColor = settings.value("color", mColor).value<QColor>();
        mMinimum = settings.value("minimum", mMinimum).toDouble();
        mMaximum = settings.value("maximum", mMaximum).toDouble();
        mUseMinimum = settings.value("useMinimum", mUseMinimum).toBool();
        mUseMaximum = settings.value("useMaximum", mUseMaximum).toBool();
        settings.endGroup();
    }

    void writeSettings() const
    {
        QSettings settings("FlySight", "Viewer");
        settings.beginGroup("plotValue/" + key());
        settings.setValue("visible", mVisible);
        settings.setValue("color", mColor);
        settings.setValue("minimum", mMinimum);
        settings.setValue("maximum", mMaximum);
        settings.setValue("useMinimum", mUseMinimum);
        settings.setValue("useMaximum", mUseMaximum);
        settings.endGroup();
    }

    virtual bool hasOptimal() const { return false; }

private:
    bool     mVisible;
    QColor   mColor;
    double   mMinimum, mMaximum;
    bool     mUseMinimum, mUseMaximum;
    QCPAxis *mAxis;

    const QString key() const
    {
        return metaObject()->className();
    }
};

class PlotElevation: public PlotValue
{
    Q_OBJECT

public:
    PlotElevation(): PlotValue(true, Qt::black) {}
    const QString title() const
    {
        return tr("Elevation");
    }
    const QString title(Units units) const
    {
        if (units == Metric) return title() + tr(" (m)");
        else                 return title() + tr(" (ft)");
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::elevation(dp);
    }
    double factor(Units units) const
    {
        return (units == Metric) ? 1
                                 : METERS_TO_FEET;
    }

    bool hasOptimal() const { return true; }
};

class PlotVerticalSpeed: public PlotValue
{
    Q_OBJECT

public:
    PlotVerticalSpeed(): PlotValue(false, Qt::green) {}
    const QString title() const
    {
        return tr("Vertical Speed");
    }
    const QString title(Units units) const
    {
        if (units == Metric) return title() + tr(" (km/h)");
        else                 return title() + tr(" (mph)");
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::verticalSpeed(dp);
    }
    double factor(Units units) const
    {
        return (units == Metric) ? MPS_TO_KMH
                                 : MPS_TO_MPH;
    }

    bool hasOptimal() const { return true; }
};

class PlotHorizontalSpeed: public PlotValue
{
    Q_OBJECT

public:
    PlotHorizontalSpeed(): PlotValue(false, Qt::red) {}
    const QString title() const
    {
        return tr("Horizontal Speed");
    }
    const QString title(Units units) const
    {
        if (units == Metric) return title() + tr(" (km/h)");
        else                 return title() + tr(" (mph)");
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::horizontalSpeed(dp);
    }
    double factor(Units units) const
    {
        return (units == Metric) ? MPS_TO_KMH
                                 : MPS_TO_MPH;
    }

    bool hasOptimal() const { return true; }
};

class PlotTotalSpeed: public PlotValue
{
    Q_OBJECT

public:
    PlotTotalSpeed(): PlotValue(false, Qt::blue) {}
    const QString title() const
    {
        return tr("Total Speed");
    }
    const QString title(Units units) const
    {
        if (units == Metric) return title() + tr(" (km/h)");
        else                 return title() + tr(" (mph)");
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::totalSpeed(dp);
    }
    double factor(Units units) const
    {
        return (units == Metric) ? MPS_TO_KMH
                                 : MPS_TO_MPH;
    }

    bool hasOptimal() const { return true; }
};

class PlotDiveAngle: public PlotValue
{
    Q_OBJECT

public:
    PlotDiveAngle(): PlotValue(false, Qt::magenta) {}
    const QString title() const
    {
        return tr("Dive Angle");
    }
    const QString title(Units units) const
    {
        Q_UNUSED(units);
        return title() + tr(" (deg)");
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::diveAngle(dp);
    }

    bool hasOptimal() const { return true; }
};

class PlotCurvature: public PlotValue
{
    Q_OBJECT

public:
    PlotCurvature(): PlotValue(false, Qt::darkYellow) {}
    const QString title() const
    {
        return tr("Dive Rate");
    }
    const QString title(Units units) const
    {
        Q_UNUSED(units);
        return title() + tr(" (deg/s)");
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::curvature(dp);
    }

    bool hasOptimal() const { return true; }
};

class PlotGlideRatio: public PlotValue
{
    Q_OBJECT

public:
    PlotGlideRatio(): PlotValue(false, Qt::darkCyan) {}
    const QString title() const
    {
        return tr("Glide Ratio");
    }
    const QString title(Units units) const
    {
        Q_UNUSED(units);
        return title();
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::glideRatio(dp);
    }

    bool hasOptimal() const { return true; }
};

class PlotHorizontalAccuracy: public PlotValue
{
    Q_OBJECT

public:
    PlotHorizontalAccuracy(): PlotValue(false, Qt::darkRed) {}
    const QString title() const
    {
        return tr("Horizontal Accuracy");
    }
    const QString title(Units units) const
    {
        if (units == Metric) return title() + tr(" (m)");
        else                 return title() + tr(" (ft)");
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::horizontalAccuracy(dp);
    }
    double factor(Units units) const
    {
        return (units == Metric) ? 1
                                 : METERS_TO_FEET;
    }
};

class PlotVerticalAccuracy: public PlotValue
{
    Q_OBJECT

public:
    PlotVerticalAccuracy(): PlotValue(false, Qt::darkGreen) {}
    const QString title() const
    {
        return tr("Vertical Accuracy");
    }
    const QString title(Units units) const
    {
        if (units == Metric) return title() + tr(" (m)");
        else                 return title() + tr(" (ft)");
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::verticalAccuracy(dp);
    }
    double factor(Units units) const
    {
        return (units == Metric) ? 1
                                 : METERS_TO_FEET;
    }
};

class PlotSpeedAccuracy: public PlotValue
{
    Q_OBJECT

public:
    PlotSpeedAccuracy(): PlotValue(false, Qt::darkBlue) {}
    const QString title() const
    {
        return tr("Speed Accuracy");
    }
    const QString title(Units units) const
    {
        if (units == Metric) return title() + tr(" (km/h)");
        else                 return title() + tr(" (mph)");
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::speedAccuracy(dp);
    }
    double factor(Units units) const
    {
        return (units == Metric) ? MPS_TO_KMH
                                 : MPS_TO_MPH;
    }
};

class PlotNumberOfSatellites: public PlotValue
{
    Q_OBJECT

public:
    PlotNumberOfSatellites(): PlotValue(false, Qt::darkMagenta) {}
    const QString title() const
    {
        return tr("Number of Satellites");
    }
    const QString title(Units units) const
    {
        Q_UNUSED(units);
        return title();
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::numberOfSatellites(dp);
    }
};

class PlotTime: public PlotValue
{
    Q_OBJECT

public:
    PlotTime(): PlotValue(false, Qt::black) {}
    const QString title() const
    {
        return tr("Time");
    }
    const QString title(Units units) const
    {
        Q_UNUSED(units);
        return title() + tr(" (s)");
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::time(dp);
    }

    bool hasOptimal() const { return true; }
};

class PlotDistance2D: public PlotValue
{
    Q_OBJECT

public:
    PlotDistance2D(): PlotValue(false, Qt::black) {}
    const QString title() const
    {
        return tr("Horizontal Distance");
    }
    const QString title(Units units) const
    {
        if (units == Metric) return title() + tr(" (m)");
        else                 return title() + tr(" (ft)");
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::distance2D(dp);
    }
    double factor(Units units) const
    {
        return (units == Metric) ? 1
                                 : METERS_TO_FEET;
    }

    bool hasOptimal() const { return true; }
};

class PlotDistance3D: public PlotValue
{
    Q_OBJECT

public:
    PlotDistance3D(): PlotValue(false, Qt::black) {}
    const QString title() const
    {
        return tr("Total Distance");
    }
    const QString title(Units units) const
    {
        if (units == Metric) return title() + tr(" (m)");
        else                 return title() + tr(" (ft)");
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::distance3D(dp);
    }
    double factor(Units units) const
    {
        return (units == Metric) ? 1
                                 : METERS_TO_FEET;
    }

    bool hasOptimal() const { return true; }
};

class PlotAcceleration: public PlotValue
{
    Q_OBJECT

public:
    PlotAcceleration(): PlotValue(false, Qt::darkRed) {}
    const QString title() const
    {
        return tr("Acceleration");
    }
    const QString title(Units units) const
    {
        Q_UNUSED(units);
        return title() + tr(" (m/s^2)");
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::acceleration(dp);
    }

    bool hasOptimal() const { return true; }
};

class PlotTotalEnergy: public PlotValue
{
    Q_OBJECT

public:
    PlotTotalEnergy(): PlotValue(false, Qt::darkGreen) {}
    const QString title() const
    {
        return tr("Total Energy");
    }
    const QString title(Units units) const
    {
        Q_UNUSED(units);
        return title() + tr(" (J/kg)");
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::totalEnergy(dp);
    }

    bool hasOptimal() const { return true; }
};

class PlotEnergyRate: public PlotValue
{
    Q_OBJECT

public:
    PlotEnergyRate(): PlotValue(false, Qt::darkBlue) {}
    const QString title() const
    {
        return tr("Energy Rate");
    }
    const QString title(Units units) const
    {
        Q_UNUSED(units);
        return title() + tr(" (J/kg/s)");
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::energyRate(dp);
    }

    bool hasOptimal() const { return true; }
};

class PlotLift: public PlotValue
{
    Q_OBJECT

public:
    PlotLift(): PlotValue(false, Qt::darkGreen) {}
    const QString title() const
    {
        return tr("Lift Coefficient");
    }
    const QString title(Units units) const
    {
        Q_UNUSED(units);
        return title();
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::liftCoefficient(dp);
    }

    bool hasOptimal() const { return true; }
};

class PlotDrag: public PlotValue
{
    Q_OBJECT

public:
    PlotDrag(): PlotValue(false, Qt::darkBlue) {}
    const QString title() const
    {
        return tr("Drag Coefficient");
    }
    const QString title(Units units) const
    {
        Q_UNUSED(units);
        return title();
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::dragCoefficient(dp);
    }

    bool hasOptimal() const { return true; }
};

class PlotCourse: public PlotValue
{
    Q_OBJECT

public:
    PlotCourse(): PlotValue(false, Qt::cyan) {}
    const QString title() const
    {
        return tr("Course");
    }
    const QString title(Units units) const
    {
        Q_UNUSED(units);
        return title() + tr(" (deg)");
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::course(dp);
    }

    bool hasOptimal() const { return false; }
};

class PlotCourseAccuracy: public PlotValue
{
    Q_OBJECT

public:
    PlotCourseAccuracy(): PlotValue(false, Qt::darkCyan) {}
    const QString title() const
    {
        return tr("Course Accuracy");
    }
    const QString title(Units units) const
    {
        Q_UNUSED(units);
        return title() + tr(" (deg)");
    }
    double rawValue(const DataPoint &dp) const
    {
        return DataPoint::courseAccuracy(dp);
    }

    bool hasOptimal() const { return false; }
};

#endif // PLOTVALUE_H
