#include "DevicesManager.h"

DevicesManager::DevicesManager(QObject *parent) : QAbstractListModel(parent)
{
  m_devices.append({"Angel Hogan", "Chapel St. 368 ", "Clearwater", "0311 1823993"});
  m_devices.append({"Felicia Patton", "Annadale Lane 2", "Knoxville", "0368 1244494"});
  m_devices.append({"Grant Crawford", "Windsor Drive 34", "Riverdale", "0351 7826892"});
  m_devices.append({"Gretchen Little", "Sunset Drive 348", "Virginia Beach", "0343 1234991"});
  m_devices.append({"Geoffrey Richards", "University Lane 54", "Trussville", "0423 2144944"});
  m_devices.append({"Henrietta Chavez", "Via Volto San Luca 3", "Piobesi Torinese", "0399 2826994"});
  m_devices.append({"Harvey Chandler", "North Squaw Creek 11", "Madisonville", "0343 1244492"});
  m_devices.append({"Miguel Gomez", "Wild Rose Street 13", "Trussville", "0343 9826996"});
  m_devices.append({"Norma Rodriguez", " Glen Eagles Street  53", "Buffalo", "0241 5826596"});
  m_devices.append({"Shelia Ramirez", "East Miller Ave 68", "Pickerington", "0346 4844556"});
  m_devices.append({"Stephanie Moss", "Piazza Trieste e Trento 77", "Roata Chiusani", "0363 0510490"});
  m_devices.append({"Angel Hogan", "Chapel St. 368 ", "Clearwater", "0311 1823993"});
  m_devices.append({"Felicia Patton", "Annadale Lane 2", "Knoxville", "0368 1244494"});
  m_devices.append({"Grant Crawford", "Windsor Drive 34", "Riverdale", "0351 7826892"});
  m_devices.append({"Gretchen Little", "Sunset Drive 348", "Virginia Beach", "0343 1234991"});
  m_devices.append({"Geoffrey Richards", "University Lane 54", "Trussville", "0423 2144944"});
  m_devices.append({"Henrietta Chavez", "Via Volto San Luca 3", "Piobesi Torinese", "0399 2826994"});
  m_devices.append({"Harvey Chandler", "North Squaw Creek 11", "Madisonville", "0343 1244492"});
  m_devices.append({"Miguel Gomez", "Wild Rose Street 13", "Trussville", "0343 9826996"});
  m_devices.append({"Norma Rodriguez", " Glen Eagles Street  53", "Buffalo", "0241 5826596"});
  m_devices.append({"Shelia Ramirez", "East Miller Ave 68", "Pickerington", "0346 4844556"});
  m_devices.append({"Stephanie Moss", "Piazza Trieste e Trento 77", "Roata Chiusani", "0363 0510490"});
  m_devices.append({"Angel Hogan", "Chapel St. 368 ", "Clearwater", "0311 1823993"});
  m_devices.append({"Felicia Patton", "Annadale Lane 2", "Knoxville", "0368 1244494"});
  m_devices.append({"Grant Crawford", "Windsor Drive 34", "Riverdale", "0351 7826892"});
  m_devices.append({"Gretchen Little", "Sunset Drive 348", "Virginia Beach", "0343 1234991"});
  m_devices.append({"Geoffrey Richards", "University Lane 54", "Trussville", "0423 2144944"});
  m_devices.append({"Henrietta Chavez", "Via Volto San Luca 3", "Piobesi Torinese", "0399 2826994"});
  m_devices.append({"Harvey Chandler", "North Squaw Creek 11", "Madisonville", "0343 1244492"});
  m_devices.append({"Miguel Gomez", "Wild Rose Street 13", "Trussville", "0343 9826996"});
  m_devices.append({"Norma Rodriguez", " Glen Eagles Street  53", "Buffalo", "0241 5826596"});
  m_devices.append({"Shelia Ramirez", "East Miller Ave 68", "Pickerington", "0346 4844556"});
  m_devices.append({"Stephanie Moss", "Piazza Trieste e Trento 77", "Roata Chiusani", "0363 0510490"});
  m_devices.append({"Angel Hogan", "Chapel St. 368 ", "Clearwater", "0311 1823993"});
  m_devices.append({"Felicia Patton", "Annadale Lane 2", "Knoxville", "0368 1244494"});
  m_devices.append({"Grant Crawford", "Windsor Drive 34", "Riverdale", "0351 7826892"});
  m_devices.append({"Gretchen Little", "Sunset Drive 348", "Virginia Beach", "0343 1234991"});
  m_devices.append({"Geoffrey Richards", "University Lane 54", "Trussville", "0423 2144944"});
  m_devices.append({"Henrietta Chavez", "Via Volto San Luca 3", "Piobesi Torinese", "0399 2826994"});
  m_devices.append({"Harvey Chandler", "North Squaw Creek 11", "Madisonville", "0343 1244492"});
  m_devices.append({"Miguel Gomez", "Wild Rose Street 13", "Trussville", "0343 9826996"});
  m_devices.append({"Norma Rodriguez", " Glen Eagles Street  53", "Buffalo", "0241 5826596"});
  m_devices.append({"Shelia Ramirez", "East Miller Ave 68", "Pickerington", "0346 4844556"});
  m_devices.append({"Stephanie Moss", "Piazza Trieste e Trento 77", "Roata Chiusani", "0363 0510490"});
  m_devices.append({"Angel Hogan", "Chapel St. 368 ", "Clearwater", "0311 1823993"});
  m_devices.append({"Felicia Patton", "Annadale Lane 2", "Knoxville", "0368 1244494"});
  m_devices.append({"Grant Crawford", "Windsor Drive 34", "Riverdale", "0351 7826892"});
  m_devices.append({"Gretchen Little", "Sunset Drive 348", "Virginia Beach", "0343 1234991"});
  m_devices.append({"Geoffrey Richards", "University Lane 54", "Trussville", "0423 2144944"});
  m_devices.append({"Henrietta Chavez", "Via Volto San Luca 3", "Piobesi Torinese", "0399 2826994"});
  m_devices.append({"Harvey Chandler", "North Squaw Creek 11", "Madisonville", "0343 1244492"});
  m_devices.append({"Miguel Gomez", "Wild Rose Street 13", "Trussville", "0343 9826996"});
  m_devices.append({"Norma Rodriguez", " Glen Eagles Street  53", "Buffalo", "0241 5826596"});
  m_devices.append({"Shelia Ramirez", "East Miller Ave 68", "Pickerington", "0346 4844556"});
  m_devices.append({"Stephanie Moss", "Piazza Trieste e Trento 77", "Roata Chiusani", "0363 0510490"});
  m_devices.append({"Angel Hogan", "Chapel St. 368 ", "Clearwater", "0311 1823993"});
  m_devices.append({"Felicia Patton", "Annadale Lane 2", "Knoxville", "0368 1244494"});
  m_devices.append({"Grant Crawford", "Windsor Drive 34", "Riverdale", "0351 7826892"});
  m_devices.append({"Gretchen Little", "Sunset Drive 348", "Virginia Beach", "0343 1234991"});
  m_devices.append({"Geoffrey Richards", "University Lane 54", "Trussville", "0423 2144944"});
  m_devices.append({"Henrietta Chavez", "Via Volto San Luca 3", "Piobesi Torinese", "0399 2826994"});
  m_devices.append({"Harvey Chandler", "North Squaw Creek 11", "Madisonville", "0343 1244492"});
  m_devices.append({"Miguel Gomez", "Wild Rose Street 13", "Trussville", "0343 9826996"});
  m_devices.append({"Norma Rodriguez", " Glen Eagles Street  53", "Buffalo", "0241 5826596"});
  m_devices.append({"Shelia Ramirez", "East Miller Ave 68", "Pickerington", "0346 4844556"});
  m_devices.append({"Stephanie Moss", "Piazza Trieste e Trento 77", "Roata Chiusani", "0363 0510490"});
}

int DevicesManager::rowCount(const QModelIndex &) const { return m_devices.count(); }

QVariant DevicesManager::data(const QModelIndex &index, int role) const
{
  if (index.row() < rowCount())
    switch (role) {
    case DeviceNameRole:
      return m_devices.at(index.row()).deviceName;
    case AddressRole:
      return m_devices.at(index.row()).address;
    case CityRole:
      return m_devices.at(index.row()).city;
    case NumberRole:
      return m_devices.at(index.row()).number;
    default:
      return QVariant();
    }
  return QVariant();
}

QHash<int, QByteArray> DevicesManager::roleNames() const
{
  static const QHash<int, QByteArray> roles{{DeviceNameRole, "deviceName"}, {AddressRole, "address"}, {CityRole, "city"}, {NumberRole, "number"}};
  return roles;
}

QVariantMap DevicesManager::get(int row) const
{
  const Device contact = m_devices.value(row);
  return {{"deviceName", contact.deviceName}, {"address", contact.address}, {"city", contact.city}, {"number", contact.number}};
}

void DevicesManager::append(const QString &fullName, const QString &address, const QString &city, const QString &number)
{
  int row = 0;
  while (row < m_devices.count() && fullName > m_devices.at(row).deviceName)
    ++row;
  beginInsertRows(QModelIndex(), row, row);
  m_devices.insert(row, {fullName, address, city, number});
  endInsertRows();
}

void DevicesManager::set(int row, const QString &fullName, const QString &address, const QString &city, const QString &number)
{
  if (row < 0 || row >= m_devices.count())
    return;

  m_devices.replace(row, {fullName, address, city, number});
  dataChanged(index(row, 0), index(row, 0), {DeviceNameRole, AddressRole, CityRole, NumberRole});
}

void DevicesManager::remove(int row)
{
  if (row < 0 || row >= m_devices.count())
    return;

  beginRemoveRows(QModelIndex(), row, row);
  m_devices.removeAt(row);
  endRemoveRows();
}
