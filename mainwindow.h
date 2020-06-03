#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <kappa++/kappa.hpp>
#include "modelmixture.h"
#include "datachartform.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buttonSelectParticlesYaml_clicked();

    void on_buttonSelectInteractionYaml_clicked();

    void on_buttonLoadData_clicked();

    void on_buttonCalc_clicked();

private:
    Ui::MainWindow *ui;
    QString particlesYamlFileName;
    QString interactionYamlFileName;
    ModelMixture *modelMixture;
};

#endif // MAINWINDOW_H
