#include "datachartform.h"
#include "ui_datachartform.h"

DataChartForm::DataChartForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataChartForm) {
    ui->setupUi(this);
}

DataChartForm::~DataChartForm() {
    delete ui;
}

void DataChartForm::drawChart(const std::vector<ModelMixture::HeatData> &table) {
    std::vector<QPointF> c_v;
    for (auto it(table.begin()); it != table.end(); ++it) {
        c_v.push_back(QPointF((*it)[ModelMixture::TEMPERATURE],
            (*it)[ModelMixture::C_TR] + (*it)[ModelMixture::C_ROT] + (*it)[ModelMixture::C_VIBR]));
    }
    ui->horizontalLayout->insertWidget(0, createChartView(c_v));
    ui->textRawData->append(QString("%1%2%3%4%5").arg("T", 15).arg("c_tr", 15).arg("c_rot", 15).arg("c_vibr", 15).arg("summary", 15));
    for (auto it(table.begin()); it != table.end(); ++it)
        ui->textRawData->append(QString("%1%2%3%4%5").arg(
                    (*it)[ModelMixture::TEMPERATURE], 15).arg(
                (*it)[ModelMixture::C_TR], 15).arg(
                (*it)[ModelMixture::C_ROT], 15).arg(
                (*it)[ModelMixture::C_VIBR], 15).arg(
                (*it)[ModelMixture::C_TR] + (*it)[ModelMixture::C_ROT] + (*it)[ModelMixture::C_VIBR], 15));
    show();
}

QChartView *DataChartForm::createChartView(const std::vector<QPointF> &data) {
    QLineSeries *series = new QLineSeries();
    for (auto it(data.begin()); it != data.end(); ++it)
        *series << *it;
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();
    axisX->setTitleText("T, [K]");
    axisY->setTitleText("c_v");
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    QChartView *chartView = new QChartView();
    chartView->setChart(chart);
    return chartView;
}
