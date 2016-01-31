#include "configdialog.h"
#include "ui_configdialog.h"

#include <QComboBox>
#include <QwwColorComboBox>

#include "dataplot.h"
#include "mainwindow.h"

ConfigDialog::ConfigDialog(MainWindow *mainWindow) :
    QDialog(mainWindow),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    // Add page
    ui->contentsWidget->addItems(
                QStringList() << tr("General"));
    ui->contentsWidget->addItems(
                QStringList() << tr("Aerodynamics"));
    ui->contentsWidget->addItems(
                QStringList() << tr("Plots"));

    // Add units
    ui->unitsCombo->addItems(
                QStringList() << tr("Metric") << tr("Imperial"));

    // Color list
    QStringList colorNames = QColor::colorNames();

    // Set up plots widget
    ui->plotTable->setColumnCount(2);
    ui->plotTable->setRowCount(DataPlot::yaLast);

    ui->plotTable->setHorizontalHeaderLabels(
                QStringList() << tr("Label") << tr("Colour"));

    for (int i = 0; i < DataPlot::yaLast; ++i)
    {
        PlotValue *yValue = mainWindow->plotArea()->yValue(i);

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(yValue->title());

        QwwColorComboBox *combo = new QwwColorComboBox();
        foreach (const QString &colorName, colorNames)
        {
            const QColor &color(colorName);
            combo->addColor(color, colorName);
            if (color == yValue->color())
            {
                combo->setCurrentText(colorName);
            }
        }

        ui->plotTable->setItem(i, 0, item);
        ui->plotTable->setCellWidget(i, 1, combo);
    }

    // Connect contents panel to stacked widget
    connect(ui->contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));

    // Go to first page
    ui->contentsWidget->setCurrentRow(0);
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::changePage(
        QListWidgetItem *current,
        QListWidgetItem *previous)
{
    if (!current) current = previous;
    ui->pagesWidget->setCurrentIndex(
                ui->contentsWidget->row(current));
}

void ConfigDialog::setUnits(
        PlotValue::Units units)
{
    ui->unitsCombo->setCurrentIndex(units);
}

PlotValue::Units ConfigDialog::units() const
{
    return (PlotValue::Units) ui->unitsCombo->currentIndex();
}

void ConfigDialog::setMass(
        double mass)
{
    ui->massEdit->setText(QString("%1").arg(mass));
}

double ConfigDialog::mass() const
{
    return ui->massEdit->text().toDouble();
}

void ConfigDialog::setPlanformArea(
        double area)
{
    ui->areaEdit->setText(QString("%1").arg(area));
}

double ConfigDialog::planformArea() const
{
    return ui->areaEdit->text().toDouble();
}

void ConfigDialog::setMinDrag(
        double minDrag)
{
    ui->minDragEdit->setText(QString("%1").arg(minDrag));
}

double ConfigDialog::minDrag() const
{
    return ui->minDragEdit->text().toDouble();
}

void ConfigDialog::setMinLift(
        double minLift)
{
    ui->minLiftEdit->setText(QString("%1").arg(minLift));
}

double ConfigDialog::minLift() const
{
    return ui->minLiftEdit->text().toDouble();
}

void ConfigDialog::setMaxLift(
        double maxLift)
{
    ui->maxLiftEdit->setText(QString("%1").arg(maxLift));
}

double ConfigDialog::maxLift() const
{
    return ui->maxLiftEdit->text().toDouble();
}

void ConfigDialog::setMaxLD(
        double maxLD)
{
    ui->maxLDEdit->setText(QString("%1").arg(maxLD));
}

double ConfigDialog::maxLD() const
{
    return ui->maxLDEdit->text().toDouble();
}

void ConfigDialog::setSimulationTime(
        int simulationTime)
{
    ui->simTimeSpinBox->setValue(simulationTime);
}

int ConfigDialog::simulationTime() const
{
    return ui->simTimeSpinBox->value();
}

QColor ConfigDialog::plotColor(
        int i) const
{
    QComboBox *combo = (QComboBox *) ui->plotTable->cellWidget(i, 1);
    return QColor(combo->currentText());
}
