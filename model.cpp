#include "model.h"

Model::Model(QObject *parent)
    : QAbstractListModel(parent)
{
    roles[static_cast<int>(RolesNames::name)] = "name";
    roles[static_cast<int>(RolesNames::latitude)] = "latitude";
    connect(&client, &Client::json_ready, this, &Model::receive_json);
    connect(&client, &Client::state, this, &Model::show_state);
}

int Model::rowCount(const QModelIndex& index) const
{
    return model_data.size();
}

QVariant Model::data(const QModelIndex& index, int role) const
{
    const int row = index.row();
    if(row < 0 || row >= model_data.size()) return QVariant{};

    switch(role) {
    case static_cast<int>(RolesNames::name): {
        return std::get<0>(model_data[row]);
    }

    case static_cast<int>(RolesNames::latitude): {
        return std::get<1>(model_data[row]);
    }
    }

    return QVariant{};
}

QHash<int, QByteArray> Model::roleNames() const
{
    return roles;
}

void Model::receive_json(const QJsonDocument& j_doc)
{
    const auto j_arr = j_doc.array();
    if(!j_arr.isEmpty()) {
        for(int i = 0; i < j_arr.size(); ++i) {
            const auto j_obj = j_arr[i].toObject();
            const auto j_obj_map = j_obj.toVariantMap();
            const auto name = j_obj_map["name"].toString();
            const auto address_obj = j_obj_map["address"].toJsonObject();
            const auto address_obj_map = address_obj.toVariantMap();
            const auto geo_obj = address_obj_map["geo"].toJsonObject();
            const auto geo_obj_map = geo_obj.toVariantMap();
            const auto lat = geo_obj_map["lat"].toDouble();
            beginInsertRows(QModelIndex(), model_data.size(), model_data.size());
            model_data.push_back(std::make_tuple(name, lat));
            endInsertRows();
        }
    }
}
