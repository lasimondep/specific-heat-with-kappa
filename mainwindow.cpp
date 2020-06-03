#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAbstractListModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    modelMixture = new ModelMixture();
    ui->tableMixture->setModel(modelMixture);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_buttonSelectParticlesYaml_clicked() {
    particlesYamlFileName = QFileDialog::getOpenFileName(this, "", "", "*.yaml");
    ui->lineParticlesYaml->setText(particlesYamlFileName);
}

void MainWindow::on_buttonSelectInteractionYaml_clicked() {
    interactionYamlFileName = QFileDialog::getOpenFileName(this, "", "", "*.yaml");
    ui->lineInteractionYaml->setText(interactionYamlFileName);
}

void MainWindow::on_buttonLoadData_clicked() {
    modelMixture->loadParticlesData(particlesYamlFileName);
}

void MainWindow::on_buttonCalc_clicked() {
    auto res = modelMixture->get_cv(particlesYamlFileName.toStdString(), interactionYamlFileName.toStdString(), ui->spinBox_p->value(),
                         ui->spinBoxTmin->value(), ui->spinBoxTmax->value(), ui->spinBoxTstep->value());
    DataChartForm *chartForm = new DataChartForm();
    chartForm->drawChart(res);
}
