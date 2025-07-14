#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "Task.h"

Task::Task(const QString& title, const QString& description, const QDate& dueDate, 
            const QString& category, int priority, bool completed, const QString& id)
        : title(title), description(description), dueDate(dueDate),
          category(category), priority(priority), completed(completed) , id(id) {}

QString Task::getTitle() const {return title;}
QString Task::getDescription() const {return description;}
QDate Task::getDueDate() const {return dueDate;}
QString Task::getCategory() const {return category;}
int Task::getPriority() const {return priority;}
bool Task::isCompleted() const {return completed;}

void Task::setTitle(const QString& t) { title = t; }
void Task::setDescription(const QString& d) { description = d; }
void Task::setDueDate(const QDate& date) { dueDate = date; }
void Task::setCategory(const QString& cat) { category = cat; }
void Task::setPriority(int p) { priority = p; }
void Task::setCompleted(bool c) { completed = c; }

QJsonObject Task::toJson() const{
  QJsonObject obj;
  obj["id"] = id;
  obj["title"] = title;
  obj["description"] = description;
  obj["dueDate"] = dueDate.toString(Qt::ISODate);
  obj["category"] = category;
  obj["priority"] = priority;
  obj["completed"] = completed;
  obj["notified"] = notified;
  return obj;
}

Task Task::fromJson(const QJsonObject& obj) {
  Task task{
    obj["title"].toString(),
    obj["description"].toString(),
    QDate::fromString(obj["dueDate"].toString(), Qt::ISODate),
    obj["category"].toString(),
    obj["priority"].toInt(),
    obj["completed"].toBool(),
    obj["id"].toString()
  };
  task.setNotified(obj["notified"].toBool(false));
  return task;
}