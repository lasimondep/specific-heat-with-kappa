#ifndef DATACHARTFORM_H
#define DATACHARTFORM_H

#include <QWidget>
#include <QtCharts>
#include "modelmixture.h"

namespace Ui {
class DataChartForm;
}

class DataChartForm : public QWidget {
    Q_OBJECT

public:
    explicit DataChartForm(QWidget *parent = nullptr);
    ~DataChartForm();

    void drawChart(const std::vector<ModelMixture::HeatData> &table);

private:
    Ui::DataChartForm *ui;
    QChartView *createChartView(const std::vector<QPointF> &data);
};

#endif // DATACHARTFORM_H
