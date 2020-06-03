#ifndef MODELMIXTURE_H
#define MODELMIXTURE_H

#include <QAbstractTableModel>
#include <kappa++/kappa.hpp>
#include <yaml.h>
#include <yaml-cpp/yaml.h>

class ModelMixture : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit ModelMixture(QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &) const override;
    bool setData(const QModelIndex&, const QVariant&, int role) override;

    void loadParticlesData(const QString&);

    enum Column {
        NAME = 0,
        COMPOSITION,
        TYPE,
        SELECTION,
        HARMONIC,
        RIGIT_ROT,
        LAST
    };
    enum Heat {
        TEMPERATURE = 0,
        C_TR,
        C_ROT,
        C_VIBR
    };

    typedef QHash<Column, QVariant> RowData;
    typedef QHash<Heat, double> HeatData;

    std::vector<HeatData> get_cv(const std::string &particles_filename,
                                 const std::string &interaction_filename,
                                 double p, double Tmin, double Tmax, double Tstep);

private:
    QList<RowData> particles;
    kappa::Mixture *mixture;
};

#endif // MODELMIXTURE_H
