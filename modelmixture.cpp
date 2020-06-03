#include "modelmixture.h"

ModelMixture::ModelMixture(QObject *parent)
    : QAbstractTableModel(parent) {
}

QVariant ModelMixture::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Vertical)
        return section;
    switch (section) {
        case NAME:
        return QString("Частица");
        case COMPOSITION:
        return QString("Концентрация, %");
        case TYPE:
        return QString("Тип");
        case SELECTION:
        return QString("Выбор");
        case HARMONIC:
        return QString("Ангармонический осцилятор");
        case RIGIT_ROT:
        return QString("Жесткий ротатор");
    }
    return QVariant();
}

int ModelMixture::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return particles.count();
}

int ModelMixture::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return LAST;
}

QVariant ModelMixture::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || particles.count() <= index.row())
        return QVariant();
    if (role == Qt::CheckStateRole) {
        if (index.column() == SELECTION) {
            if (particles[index.row()][Column(index.column())] == true)
                return Qt::Checked;
            else
                return Qt::Unchecked;
        }
        if (particles[index.row()][TYPE] == "MOLECULE" && (index.column() == HARMONIC || index.column() == RIGIT_ROT)) {
            if (particles[index.row()][Column(index.column())] == true)
                return Qt::Checked;
            else
                return Qt::Unchecked;
        }
    }
    if (role == Qt::DisplayRole
            && index.column() != SELECTION
            && index.column() != HARMONIC
            && index.column() != RIGIT_ROT)
        return particles[index.row()][Column(index.column())];
    return QVariant();
}

Qt::ItemFlags ModelMixture::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    if (index.isValid()) {
        if (index.column() == SELECTION
            || ((index.column() == HARMONIC || index.column() == RIGIT_ROT)
            && particles[index.row()][TYPE] == "MOLECULE")) {
            flags |= Qt::ItemIsUserCheckable;
        }
        if (index.column() == COMPOSITION)
            flags |= Qt::ItemIsEditable;
    }
    return flags;
}

bool ModelMixture::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || particles.count() <= index.row())
        return false;
    if (role == Qt::CheckStateRole) {
        if (qvariant_cast<Qt::CheckState>(value) == Qt::Checked) {
            particles[index.row()][Column(index.column())] = true;
            return true;
        } else {
            particles[index.row()][Column(index.column())] = false;
            return true;
        }
    }
    if (role == Qt::EditRole) {
        double comp = value.toDouble();
        if (comp < 0 || comp > 100)
            return false;
        particles[index.row()][Column(index.column())] = comp;
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

void ModelMixture::loadParticlesData(const QString &filename) {
    YAML::Node file;
    file = YAML::LoadFile(filename.toStdString());
    RowData rowData;
    rowData[COMPOSITION] = 0.;
    rowData[SELECTION] = false;
    rowData[HARMONIC] = true;
    rowData[RIGIT_ROT] = true;
    beginResetModel();
    particles.clear();
    for (auto it(file.begin()); it != file.end(); ++it) {
        rowData[NAME] = QString::fromStdString(it->first.as<std::string>());
        rowData[TYPE] = QString::fromStdString(it->second["Type"].as<std::string>());
        if (rowData[TYPE] == "ATOM" || rowData[TYPE] == "MOLECULE")
            particles.append(rowData);
    }
    endResetModel();
}

std::vector<ModelMixture::HeatData> ModelMixture::get_cv(const std::string &particles_filename,
                                                         const std::string &interaction_filename,
                                                         double p, double Tmin, double Tmax, double Tstep) {
    std::vector<HeatData> res;
    std::vector<kappa::Molecule> molecules;
    std::vector<kappa::Atom> atoms;
    std::vector<double> atom_comp;
    std::vector<double> mol_comp;
    RowData row;
    for (auto it(particles.begin()); it != particles.end(); ++it) {
        row = *it;
        if (row[SELECTION].toBool()) {
            if (row[TYPE] == "ATOM") {
                atoms.push_back(kappa::Atom(row[NAME].toString().toStdString(), particles_filename));
                atom_comp.push_back(row[COMPOSITION].toDouble() / 100);
            }
            if (row[TYPE] == "MOLECULE") {
                molecules.push_back(kappa::Molecule(row[NAME].toString().toStdString(),
                                                    row[HARMONIC].toBool(),
                                                    row[RIGIT_ROT].toBool(),
                                                    particles_filename));
                mol_comp.push_back(row[COMPOSITION].toDouble() / 100);
            }
        }
    }
    mixture = new kappa::Mixture(molecules, atoms, interaction_filename, particles_filename);
    double tot_ndens;
    arma::vec atom_ndens(atoms.size());
    std::vector<arma::vec> mol_ndens(molecules.size());
    HeatData item;
    for (double T = Tmin; T <= Tmax; T += Tstep) {
        tot_ndens = p / (kappa::K_CONST_K * T);
        for (size_t i = 0; i < atom_ndens.size(); ++i)
            atom_ndens[i] = atom_comp[i] * tot_ndens;
        for (size_t i = 0; i < mol_ndens.size(); ++i)
            mol_ndens[i] = mixture->Boltzmann_distribution(T, mol_comp[i] * tot_ndens, molecules[i]);
        item[TEMPERATURE] = T;
        item[C_TR] = mixture->c_tr(mol_ndens, atom_ndens);
        item[C_ROT] = mixture->c_rot(T, mol_ndens, atom_ndens);
        item[C_VIBR] = 0.;
        for (size_t i = 0; i < molecules.size(); ++i)
            item[C_VIBR] += mixture->c_vibr_approx(T, molecules[i]) * mol_comp[i];
        res.push_back(item);
    }
    return res;
}
