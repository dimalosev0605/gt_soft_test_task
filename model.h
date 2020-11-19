#ifndef MODEL_H
#define MODEL_H

#include <QAbstractListModel>
#include <QDebug>

#include "client.h"

class Model: public QAbstractListModel
{
    Q_OBJECT
    QHash<int, QByteArray> roles;
    QVector<std::tuple<QString, double>> model_data;
    QJsonDocument j_doc_data;
    Client client;

private:
    QHash<int, QByteArray> roleNames() const override;

private slots:
    void receive_json(const QJsonDocument& j_doc);

public:
    enum class RolesNames {
        name = Qt::UserRole,
        latitude
    };
    explicit Model(QObject* parent = nullptr);
    virtual int rowCount(const QModelIndex &index = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

signals:
    void show_state(const QString& state);
};

#endif // MODEL_H
